//////////////////////////////////////////////////////////////////////////////
//
//	file: QEAtest.cpp
//	Date: 2002.11.7
//	Copyright 2002 Kuk-Hyun Han. All rights reserved.
//	Description: QEA standard structure test cpp
//
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <time.h>
#include <iostream>



//#define DEJONG1
//#define DEJONG2
//#define DEJONG3
//#define DEJONG4
#define DEJONG5

#ifdef DEJONG1
#define NVARS			3			// variables 
#define NBITS			18			// varialbe  bit 
#define LBND			-5.12		// varialbe  lower bound
#define UBND			5.12		// varialbe  upper bound
#endif

#ifdef DEJONG2
#define NVARS			2			// variables 
#define NBITS			18			// varialbe  bit 
#define LBND			-2.048		// varialbe  lower bound
#define UBND			2.048		// varialbe  upper bound
#endif

#ifdef DEJONG3
#define NVARS	5			// variables 
#define NBITS	18			// varialbe  bit 
#define LBND	-5.12		// variables  lower bound
#define UBND	5.12		// variables  upper bound
#endif


#ifdef DEJONG4
#define NVARS	30			// variables 
#define NBITS	18			// varialbe  bit 
#define LBND	-1.28		// variables  lower bound
#define UBND	1.28		// variables  upper bound
#endif


#ifdef DEJONG5
#define NVARS	2			// variables 
#define NBITS	18			// varialbe  bit 
#define LBND	-65.536		// variables  lower bound
#define UBND	65.536		// variables  upper bound
#endif


/* QEA variables */

#define QEA_POPSIZE		50			// population size => Q-Bit Individual 
#define QEA_NVARS		NVARS		// variable 
#define QEA_NBITpVAR	NBITS		// variable bit
#define QEA_NBITS		(QEA_NVARS*QEA_NBITpVAR)	// bit string length => #variables*#bits/var or SUM(bits/var) => , 1 array
#define QEA_NOBSERV		2			// Q-bit individual   observing => binary string 
#define QEA_TGLOBAL		100			// Global migration period
#define QEA_TLOCAL		1			// Local migration period
#define QEA_NUMINGRP	3			// 1 group Q-bit  (, local migration)

#define QEA_NRUNS		10		// QEA    

#define QEA_MAXGENS		500		// QEA  generation  

// �ʱ� lookup table
#define QEA_ANGLE		0.01*M_PI
#define QEA_INIT_LOOKUP {0., 0., QEA_ANGLE, 0., -QEA_ANGLE, 0., 0., 0.}	



//#define	USER_INIT			// void USER_initialize(void) 
//#define	USER_REPAIR			// void USER_repair(void) : constraint solution

#define	QEA_MINIMIZE		// store the best solutions among P(t) into B(t)
								// Ǯ  maximize, Ǯ minimize 

//#define	USER_FEASIBILITY	// bool USER_bFeasible(unsigned char x[]) : Binary solution feasibility user.
#define	USER_OBJECTIVE		// double USER_objective(unsigned char x[]) : user objective function

#define	USER_TERMINATE		// bool USER_isTerminate(void) : termination condition user.


#define USER_TERMI_BEST		0.01		// Must be set really low for De Jong 3: 0.0000065
										// Also for De Jong 5: 0.01
										// Default: 0.9 

#define FILE_SOLUTION			//  run best solution

#include "QEAStandard.h"


double Dejong_F1(double x[])
{
	double value = 0.;

	for (int i=0; i<NVARS; i++) {
		value += x[i]*x[i];
	}
	return value;
//	return (100.-value);
}

double Dejong_F2(double x[])
{
	double value = 100.*(x[0]*x[0] - x[1])*(x[0]*x[0] - x[1]) + (1.-x[0])*(1.-x[0]);

	return value;
//	return (1./(value+.01));
//	return (100.-value);
}

double Dejong_F3(double *x)
{
	int i;
	double y=0.;
	for (i=0; i<NVARS; i++){
		if(x[i]<0)
			x[i]-=1;
		y+=(int)(x[i]);
	}
	return y;
}

double gaussian(double Xmean,double dev)
{
	double X=0;
	double y;
	X=2.56*(double)rand()/RAND_MAX;
	y=(1./sqrt(2*M_PI*dev*dev))*exp(-pow(X-Xmean,2)/(2.*dev*dev));
	return y;
}

double Dejong_F4(double *x)
{
	int i;
	double y=0.;
	for (i=0; i<NVARS; i++){
		y +=(i+1)*pow(x[i],4);
	}
	y +=gaussian(0.,1.);
	return y;
}

double Dejong_F5(double *x)
{
	int i,j;
	double y=0.;
	double f=0.;
	int A[2][25];
	
	for (j=0; j<25; j++){
		if((j%5)==0){
			A[0][j]=-32;
			if (j==0)
				A[1][j]=-32;
			else
				A[1][j]=A[1][j-1]+16;
		}
		else{
			A[0][j] = A[0][j-1] +16;
			A[0][j] = A[1][j-1];
		}
	}
	
	
	
	for (j=0; j<25; j++){
		f = j+1;
		for (i=0; i<2; i++){
			f +=pow ( (x[i]-A[i][j]),6);
		}
		y +=1./f;
	}
	y+=1./500;
	y=1./y;
	return y;
}

// Binary to Double
double Binary2Double(unsigned char x[], int v)
{
	double val = 0.;
	double rate = 1.;
	int k;

	for (k=(NBITS+v*NBITS); k>v*NBITS; k--) {
		val += (double)x[k-1]*rate;
		rate *= 2.;
	}

	val = LBND + val*(UBND-LBND)/(rate-1.);
	return val;
}

#ifdef USER_TERMI_BEST
// 1. best termination condition
bool termi_best(double gamma)
{
	double total_prob = 0.;
	double probbest[QEA_POPSIZE];

	for (int i=0; i<QEA_POPSIZE; i++) {
		probbest[i] = 1.;
		for (int k=0; k<QEA_NBITS; k++) {
			probbest[i] *= qea.Qbit[i][k][qea.bb.sol[k]]*qea.Qbit[i][k][qea.bb.sol[k]];
		}
		total_prob += probbest[i];
	}
	total_prob /= QEA_POPSIZE;

	if (total_prob >= gamma)
		return true;
	else 
		return false;
}
#endif



void Analysis_JH(void)
{
	double temp_sum = 0;
	double tmp;
	double temp_best = qeaResult.best_in_run[0].fitness;
	double temp_worst = qeaResult.best_in_run[0].fitness;
	double nRun = (double)QEA_MAXGENS;
	
	for(int i=0; i<QEA_MAXGENS; i++)
	{
		tmp = qeaResult.best_in_run[i].fitness;
		temp_sum += tmp;
		if(temp_best >= tmp)
			temp_best = tmp;
		if(temp_worst <= tmp)
			temp_worst = tmp;
	}
	qeaResult.mean_fitness = (double)(temp_sum/nRun);
	qeaResult.best_fitness = temp_best;
	qeaResult.worst_fitness = temp_worst;
	
	//standard deviation
	temp_sum = 0;
	for(int i=0; i<QEA_MAXGENS; i++)
	{
		temp_sum += pow((qeaResult.best_in_run[i].fitness - qeaResult.mean_fitness),2.);
	}
	
	qeaResult.standard_dev= sqrt(temp_sum/nRun);
	
}

int main()
{

	srand((unsigned)time(NULL));



	for (int i=0; i<QEA_NRUNS; i++) {

		QEArun(i);
	
		
		printf("run=%d ==> ", i);

		for (int v=0; v<NVARS; v++) 
			printf("x%d=%lf ", v, Binary2Double(qea.bb.sol, v));

		printf("==> f=%lf\n", qea.bb.fitness);
	}

	QEA_analysis();
//	Analysis_JH();

	std::cout << "Best Profit = " << qeaResult.best_fitness << std::endl;
	std::cout << "Mean Profit = " << qeaResult.mean_fitness << std::endl;
	std::cout << "Worst Profit = " << qeaResult.worst_fitness << std::endl;
	std::cout << "Standard dev = " << qeaResult.standard_dev << std::endl;
	std::cout << "# Feasible Sol = " << qeaResult.num_feasible_sol << std::endl;
	std::cout << "Elapsed time = " << qeaResult.mean_time << " msec" << std::endl;
	std::cout << "Average generation = " << qeaResult.mean_generation << std::endl;

return 0;

}


/* User defined functions for QEA */

#ifdef USER_INIT
// �  �ʱ�ȭ �Լ�
void USER_initialize(void)
{
}
#endif

#ifdef USER_REPAIR
// repair P(t) : constraint solution.
void USER_repair(void)
{

}
#endif

#ifdef USER_FEASIBILITY
// Binary solution feasibility user.
bool USER_bFeasible(unsigned char x[])
{
	return true;
}
#endif


#ifdef USER_OBJECTIVE
// user objective function
double USER_objective(unsigned char x[])
{
	double val[NVARS];

	for (int v=0; v<NVARS; v++) {
		val[v] = Binary2Double(x, v);
	}

	// De Jong Function...
#ifdef DEJONG1
	return Dejong_F1(val);
#endif
#ifdef DEJONG2
	return Dejong_F2(val);
#endif
#ifdef DEJONG3
	return Dejong_F3(val);
#endif
#ifdef DEJONG4
	return Dejong_F4(val);
#endif
#ifdef DEJONG5
	return Dejong_F5(val);
#endif

}
#endif


//#ifdef USER_TERMINATE
// termination condition user.
bool USER_isTerminate(double gamma)
{
#ifdef USER_TERMI_BEST
	return termi_best(gamma);
#endif
}
//#endif 
