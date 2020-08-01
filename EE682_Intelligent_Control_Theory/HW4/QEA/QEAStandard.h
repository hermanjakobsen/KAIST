//////////////////////////////////////////////////////////////////////////////
//
//	file: QEAStandard.h
//	Date: 2002.11.7
//	Copyright 2002 Kuk-Hyun Han. All rights reserved.
//	Description: QEA standard structure h (�ڻ� ������)
//
//////////////////////////////////////////////////////////////////////////////


#include <math.h>
#include <stdlib.h>
//#include <windows.h>

#include "QEAdef.h"
#include "HStopwatch.h"

#pragma warning(disable:4996)

#define QEA_POPSIZE_VAR		QEA_POPSIZE
#define QEA_NUMINGRP_VAR	QEA_NUMINGRP
#define QEA_TGLOBAL_VAR		QEA_TGLOBAL
#define QEA_NOBSERV_VAR		QEA_NOBSERV


// Binary solution struct.
typedef struct {
	// Binary solutions
	unsigned char sol[QEA_NBITS];				// Binary solutions
	double	fitness;							// Fitness value of sol
	bool	feasible;							// Feasibility of sol
} SBinarySol;


// QEA variables
typedef struct {

	// current generation
	int		t;

	// population
	double	Qbit[QEA_POPSIZE][QEA_NBITS][2];				// Q-bit individual

	// Binary solutions
	SBinarySol	X[QEA_POPSIZE][QEA_NOBSERV];				// Binary solutions (multiple observing)

	// Best solutions
	SBinarySol	B[QEA_POPSIZE];								// Best binary solution

	// Best of best
	SBinarySol	bb;											// best 

} SQEAVar;

SQEAVar	qea;

// QEA lookup table
double qeaLT[8] = QEA_INIT_LOOKUP;


typedef struct {
	SBinarySol	best_in_run[QEA_NRUNS];		// run best solution
	double best_fitness;					// run best fitness
	double mean_fitness;					// run best mean fitness
	double worst_fitness;					// run best�� ���� worst fitness
	double standard_dev;					// run best���� ǥ�� ����
	double mean_time;						// run�� ��� ���� �ҿ� �ð�
	int mean_generation;					// run�� ��� generation
	int num_feasible_sol;					// feasible solution ����
} SQEAResult;


HStopwatch QEAwatch;

// QEA
SQEAResult qeaResult;

// Q-bit individual
void QEA_initialize_Q(void);

#ifdef USER_INIT
void USER_initialize(void);
#endif

// make P(t) by observing the states of Q(t) : multiple observing
void QEA_make_P(void);

#ifdef USER_REPAIR
// repair P(t) : constraint solution.
void USER_repair(void);
#endif

// evaluate P(t) :USER_objective function.
void QEA_evaluate_P(void);

// Binary solution feasibilit
#ifdef USER_FEASIBILITY
bool USER_bFeasible(unsigned char x[]);
#endif

// user objective function
#ifdef USER_OBJECTIVE
double USER_objective(unsigned char x[]);
#endif


// store the best solutions among P(t) into B(t)
void QEA_storeBest0(void);		// Maximize problem

// termination condition user.
bool USER_isTerminate(double gamma);

// update Q(t) using Q-gates
void QEA_update_Q(void);		// Q update
void QEA_update0(void);			// multiple Q update

// store the best solutions among P(t) anb B(t-1) into B(t)
void QEA_storeBest(void);		// Maximize problem

// if (migration-condition) migrate globally or locally
void QEA_migrate(void);

// QEA
void QEA_analysis(void);

// Store solution in file
void FILE_sol(void);


// QEA main 
// 1. t = 0
// 2. initialize Q(t)
//		2.5 initialze_User()
// 3. make P(t) by observing the states of Q(t) : multiple observing
//		3.5 repair P(t) : constraint solution.
// 4. evaluate P(t)
// 5. store the best solutions among P(t) into B(t)
// 6. while (not termination-condition) do
// 7. t = t + 1
// 8. make P(t) by observing the states of Q(t-1) : multiple observing
//		8.5 repair P(t) : constraint solution.
// 9. evaluate P(t)
// 10. update Q(t) using Q-gates
// 11. store the best solutions among B(t-1) and P(t) into B(t)
// 12. store the best solution b among B(t)
// 13. if (migration-condition) migrate globally or locally

void QEArun(int run) 
{

	QEAwatch.start();
	// 1. t = 0
	qea.t = 0;

	// 2. initialize Q(t)
	QEA_initialize_Q();


#ifdef USER_INIT
	// 2.5 initialze_User()
	USER_initialize();
#endif

	// 3. make P(t) by observing the states of Q(t) : multiple observing
	QEA_make_P();

#ifdef USER_REPAIR
	// 3.5 repair P(t) : constraint solution.
	USER_repair();
#endif

	// 4. evaluate P(t) : USER_objective function
	QEA_evaluate_P();

	// 5. store the best solutions among P(t) into B(t)
	QEA_storeBest0();			
	
	// 6. while (not termination-condition) do
#ifndef USER_TERMINATE
	while (qea.t < QEA_MAXGENS)
#else
	#ifdef USER_TERMI_BEST
		while ( !USER_isTerminate(USER_TERMI_BEST) && qea.t < QEA_MAXGENS) 
	#endif
#endif
	{
		// 7. t = t + 1
		qea.t++;

		// 8. make P(t) by observing the states of Q(t-1) : multiple observing
		QEA_make_P();

#ifdef USER_REPAIR
		// 8.5 repair P(t) : constraint solution .
		USER_repair();
#endif

		// 9. evaluate P(t)
		QEA_evaluate_P();
	
		// 10. update Q(t) using Q-gates
		QEA_update_Q();

		// 11. store the best solutions among B(t-1) and P(t) into B(t)
		// 12. store the best solution b among B(t)
		QEA_storeBest();

		
#ifdef QEA_TGLOBAL
		// 13. if (migration-condition) migrate globally or locally
		QEA_migrate();
#endif

	}
	QEAwatch.stop();


	// KHHAN_ANALYSIS
	qeaResult.best_in_run[run] = qea.bb;
	qeaResult.mean_time = (qeaResult.mean_time*run + QEAwatch.getDT()/1000.)/(run+1);
	qeaResult.mean_generation = (qeaResult.mean_generation*run + qea.t)/(run+1);

}

#define QEA_INIT_ALPHA		(1./sqrt(2.))
#define QEA_INIT_BETA		(1./sqrt(2.))


// Q-bit individual
void QEA_initialize_Q(void)
{
	//Q-bit 1/root(2)
	for (int i=0; i<QEA_POPSIZE_VAR; i++) {
		for (int k=0; k<QEA_NBITS; k++) {
			qea.Qbit[i][k][0] = QEA_INIT_ALPHA;
			qea.Qbit[i][k][1] = QEA_INIT_BETA;
		}
	}
}

// make P(t) by observing the states of Q(t) : multiple observing 
void QEA_make_P(void)
{
	int i, j, k;
	double tmp, beta;

	// Binary solutions (multiple observing)
	for (i=0; i<QEA_POPSIZE_VAR; i++) {
		for (j=0; j<QEA_NOBSERV_VAR; j++) {
			for (k=0; k<QEA_NBITS; k++) {
				// random.
				tmp = (double)rand()/RAND_MAX;
				// Q-bit beta
				beta = qea.Qbit[i][k][1];
				if (tmp <= beta*beta) {
					qea.X[i][j].sol[k] = 1;
				}
				else {
					qea.X[i][j].sol[k] = 0;
				}
			}
		}
	}
}

// evaluate P(t) : USER_objective function.
void QEA_evaluate_P(void)
{
	int i, j;

	for (i=0; i<QEA_POPSIZE_VAR; i++) {
		for (j=0; j<QEA_NOBSERV_VAR; j++) {
			// fitness
#ifdef USER_OBJECTIVE
			// Fitness value of Xsol
			qea.X[i][j].fitness = USER_objective(qea.X[i][j].sol);
#else
			// basic objective value : number of 1s
			qea.X[i][j].fitness = 0.;
			for (int k=0; k<QEA_NBITS; k++) {
				qea.X[i][j].fitness += qea.X[i][j].sol[k];
			}
#endif

#ifdef USER_FEASIBILITY
	#ifdef QEA_NO_NEED_TO_FEASIBILITY_CHECK_EVERY_TIME
			qea.X[i][j].feasible = true;
	#else
			qea.X[i][j].feasible = USER_bFeasible(qea.X[i][j].sol);
	#endif
#else 
			qea.X[i][j].feasible = true;
#endif
		}
	}
}

// store the best solutions among P(t) into B(t)
void QEA_storeBest0(void)
{
	int i, j;
	double best, bbest;
	int index, bindex;

	bbest = qea.X[0][0].fitness;
	bindex = 0;

	for (i=0; i<QEA_POPSIZE_VAR; i++) {
		best = qea.X[i][0].fitness;
		index = 0;
		for (j=1; j<QEA_NOBSERV_VAR; j++) {
#ifndef QEA_MINIMIZE
			if (best <= qea.X[i][j].fitness)		// maximize problem
#else
			if (best >= qea.X[i][j].fitness)		// minimize problem
#endif
			{
				best = qea.X[i][j].fitness;
				index = j;
			}
		}

		// store into B(t)
		qea.B[i] = qea.X[i][index];

		// store best of best
#ifndef QEA_MINIMIZE
		if (bbest <= qea.B[i].fitness)		// maximize problem
#else
		if (bbest >= qea.B[i].fitness)		// minimize problem
#endif

		{
			bbest = qea.B[i].fitness;
			bindex = i;
		}
	}

	qea.bb = qea.B[bindex];
}


// update Q(t) using Q-gates
void QEA_update_Q(void)
{
	QEA_update0();
}

/* Update multiple x */
void QEA_update0(void)
{
	double current_bestscore;
	double x_score;
	unsigned char x, b, c;
	int index, i, j, k;
	double sign, alpha, beta;
	double dth;

	for (i=0; i<QEA_POPSIZE_VAR; i++) {
		current_bestscore = qea.B[i].fitness;
		for (j=0; j<QEA_NOBSERV_VAR; j++) {
			x_score = qea.X[i][j].fitness;
#ifndef QEA_MINIMIZE
			c = (x_score >= current_bestscore) ? 1 : 0; //maximize
#else 
			c = (x_score <= current_bestscore) ? 1 : 0;	//minimize
#endif


			if (c == 1)
				continue;

			for (k=0; k<QEA_NBITS; k++) {
				x = qea.X[i][j].sol[k];
				b = qea.B[i].sol[k];
				index = x*4 + b*2 + c;

				// lookup table
				if (qeaLT[index] != 0.) {
					alpha = qea.Qbit[i][k][0];
					beta = qea.Qbit[i][k][1];
					sign = alpha * beta;
					sign = sign/fabs(sign);

					dth = sign * qeaLT[index];

					// rotation gate
					qea.Qbit[i][k][0] = alpha*cos(dth) - beta*sin(dth);
					qea.Qbit[i][k][1] = alpha*sin(dth) + beta*cos(dth);

					// SRN gate
					//qea.Qbit[i][k][0] = alpha*(1/sqrt(2)) - beta*(1/sqrt(2));
					//qea.Qbit[i][k][1] = alpha*(1/sqrt(2)) + beta*(1/sqrt(2));
				}
			}
		}
	}
}



// store the best solutions among P(t) and B(t-1) into B(t)
// t >= 1 �� ��쿡�� P(t) and B(t-1) �߿��� B(t)�� ������.
void QEA_storeBest(void)
{
	int i, j;
	double best, bbest;
	int index, bindex;

	// best of best�� ã�� ����.
	bbest = qea.B[0].fitness;
	bindex = 0;

	for (i=0; i<QEA_POPSIZE_VAR; i++) {
		best = qea.B[i].fitness;
		index = -1;
		for (j=0; j<QEA_NOBSERV_VAR; j++) {
#ifndef QEA_MINIMIZE
			if (best <= qea.X[i][j].fitness)		// maximize problem
#else
			if (best >= qea.X[i][j].fitness)		// minimize problem
#endif
			{
				best = qea.X[i][j].fitness;
				index = j;
			}
		}

		// best of best.
#ifndef QEA_MINIMIZE
		if (bbest <= best)		// maximize problem
#else
		if (bbest >= best)		// minimize problem
#endif
		{
			bbest = best;
			bindex = i;
		}

		// best.
		if (index == -1)
			continue;

		// store into B(t)
		qea.B[i] = qea.X[i][index];
	}

	// best of best. store b among B(t)
	qea.bb = qea.B[bindex];
}


#ifdef QEA_TGLOBAL
// if (migration-condition) migrate globally or locally
void QEA_migrate(void)
{
	if (qea.t % QEA_TGLOBAL_VAR == 0) {
		// migrate globally
		// b B(t) copy
		for (int i=0; i<QEA_POPSIZE_VAR; i++) {
			qea.B[i] = qea.bb;
		}
	}

#ifndef QEA_LOCAL_MIGRATE_OFF
	else if (qea.t % QEA_TLOCAL == 0) {
		// migrate locally
		int numGrp = (QEA_POPSIZE_VAR/QEA_NUMINGRP_VAR) + ( (QEA_POPSIZE_VAR % QEA_NUMINGRP_VAR) ? 1 : 0 );
		// group best
		double best;
		int index;
		// pop index
		int i = 0;
		for (int g=0; g<numGrp; g++) {
			index = i;
			best = qea.B[index].fitness;
			for (int l=i+1; l<i+QEA_NUMINGRP_VAR && l<QEA_POPSIZE_VAR; l++) {
#ifndef QEA_MINIMIZE
				if (best <= qea.B[l].fitness)		// maximize problem
#else
				if (best >= qea.B[l].fitness)		// minimize problem
#endif
				{
					best = qea.B[l].fitness;
					index = l;
				}
			}

			// group best copy...
			for (int l=i; l<i+QEA_NUMINGRP_VAR && l<QEA_POPSIZE_VAR; l++) {
				if (l == index)
					continue;
				qea.B[l] = qea.B[index];
			}

			// i = ?
			i += QEA_NUMINGRP_VAR;
		}
	}
#endif // QEA_LOCAL_MIGRATE_OFF
}
#endif


// QEA 
void QEA_analysis(void)
{
	// best, mean, worst fitness
	double temp_sum = 0.;
	double tmp;
	double temp_best = qeaResult.best_in_run[0].fitness;
	double temp_worst = qeaResult.best_in_run[0].fitness;
	int num_feasible_sol = 0;

	for (int r=0; r<QEA_NRUNS; r++) {
		if (qeaResult.best_in_run[r].feasible == true) {
			num_feasible_sol++;
			tmp = qeaResult.best_in_run[r].fitness;
			temp_sum += tmp;

#ifndef QEA_MINIMIZE
			if (temp_best <= tmp)	//maximize
				temp_best = tmp;
			if (temp_worst >= tmp)
				temp_worst = tmp;
#else
			if (temp_best >= tmp)	//minimize
				temp_best = tmp;
			if (temp_worst <= tmp)
				temp_worst = tmp;
#endif
		}
	}
	qeaResult.mean_fitness = (double)(temp_sum/num_feasible_sol);
	qeaResult.best_fitness = temp_best;
	qeaResult.worst_fitness = temp_worst;
	qeaResult.num_feasible_sol = num_feasible_sol;

	/* Calculate standard deviation */
	/* sum {(x - mean)^2} / num */
	temp_sum = 0.;
	for (int r=0; r<QEA_NRUNS; r++) {
		if (qeaResult.best_in_run[r].feasible == true) {
			temp_sum += (qeaResult.best_in_run[r].fitness - qeaResult.mean_fitness)*(qeaResult.best_in_run[r].fitness - qeaResult.mean_fitness);
		}
	}
	qeaResult.standard_dev = sqrt(temp_sum / num_feasible_sol);


	#ifdef FILE_SOLUTION
	FILE_sol();
	#endif
}

/* Write solution and statistics to file */
void FILE_sol(void)
{
	char filename[300] = "QEA_output/D5_solution.txt";
	//sprintf(filename, "solution.txt");
	
	// file open
	FILE *fp = fopen(filename, "wt");

	for (int r=0; r<QEA_NRUNS; r++) {
		for (int k=0; k<QEA_NBITS; k++) {
			fprintf (fp, "%d", qeaResult.best_in_run[r].sol[k]);
		}
		fprintf (fp, " => %lf\n", qeaResult.best_in_run[r].fitness);
	}

	fprintf(fp, "Best Fitness = %10.9lf\n", qeaResult.best_fitness);
	fprintf(fp, "Average Fitness = %10.9lf\n", qeaResult.mean_fitness);
	fprintf(fp, "Worst Fitness = %10.9lf\n", qeaResult.worst_fitness);
	fprintf(fp, "Standard dev. = %10.9lf\n", qeaResult.standard_dev);
	fprintf(fp, "Feasible Best Sol. # = %d\n", qeaResult.num_feasible_sol);
	fprintf(fp, "Average E. Time = %lf\n", qeaResult.mean_time);
	fprintf(fp, "Average Generation = %d\n", qeaResult.mean_generation);
	fclose(fp);
}