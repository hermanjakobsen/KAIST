// MQEA.cpp : Defines the entry point for the console application.

//#include "stdafx.h"

#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>//INT_MAX의 값을 알기 위해 
#include <windows.h>


#define PI 3.141593
#define square(x) ((x)*(x))
#define INFINITE 99999999

//////////////////////////////////////MQEA//////////////////////////////////////
//////////////////////////////////////MQEA//////////////////////////////////////
//////////////////////////////////////MQEA//////////////////////////////////////
//////////////////////////////////////MQEA//////////////////////////////////////

//////////////		Setting Parameters		//////////////////////
#define DTLZ2 // 문제 종류 DTLZ1 ~ DTLZ7

#define BITSTRING 1000 // 총 binary bit string 수 = DECODING_RESOLUTION * MAXVAR
#define DECODING_RESOLUTION 20 // Binary bit decoding resolution
#define MAXVAR 50 //최대 variable 수

#define RUN	1
#define OBSERVATION 10 // Observation 수
#define ITERATION 5 // 실행횟수
#define OBJ 3 // Objectives 수
#define GEN 300 // Generation 수
#define SUBPOPSIZE 25 // Population 크기 = Individual 수
#define SUBPOPULATION 4 // Subpopulation 갯수


double xi = 0.25; //크사이
//#define _PREFERENCE_BASED_SORTING //Fuzzy integral 적용
#define _DOMINANCE_BASED_SORTING // Fuzzy integral 적용 없이 Dominance 관계로 Archive 소팅

double weight[OBJ];		// fuzzy measure를 계산하기 위한 weight
/*
double pairwise_matrix[OBJ][OBJ]={
	{1, (double)0.1, 1, (double)0.1, 1, (double)0.1, 1},
 	{10, 1, 10, 1, 10, 1, 10},
	{1, (double)0.1, 1, (double)0.1, 1, (double)0.1, 1},
	{10, 1, 10, 1, 10, 1, 10},
	{1, (double)0.1, 1, (double)0.1, 1, (double)0.1, 1},
	{10, 1, 10, 1, 10, 1, 10},
	{1, (double)0.1, 1, (double)0.1, 1, (double)0.1, 1},
};
*/
/*
double pairwise_matrix[OBJ][OBJ]={
	{1, (double)0.1, 1},
	{10, 1, 10},
	{1, (double)0.1, 1},
};
*/

//안쓰는거.
double pairwise_matrix[OBJ][OBJ]={
	{1, (double)0.1},
	{10, 1},
};
/*
double pairwise_matrix[OBJ][OBJ]={
	{1, 1, 1, 1, 1, 1, (double)0.1},
	{1, 1, 1, 1, 1, 1, (double)0.1},
	{1, 1, 1, 1, 1, 1, (double)0.1},
	{1, 1, 1, 1, 1, 1, (double)0.1},
	{1, 1, 1, 1, 1, 1, (double)0.1},
	{1, 1, 1, 1, 1, 1, (double)0.1},
	{10, 10, 10, 10, 10, 10, 1},
};
*/
#define LOWER_BOUND 0 // Variable 도메인
#define UPPER_BOUND 1

int NondomCondition = OBJ;

#ifdef DTLZ1
double THETA = 0.20*PI; 
#elif defined DTLZ2
double THETA = 0.20*PI; 
#elif defined DTLZ3
double THETA = 0.20*PI; 
#elif defined DTLZ4
double THETA = 0.23*PI; 
#elif defined DTLZ5
double THETA = 0.20*PI; 
#elif defined DTLZ6
double THETA = 0.20*PI; 
#elif defined DTLZ7
double THETA = 0.23*PI; 
#endif

// Variable 수
#ifdef DTLZ1
int VarNum = OBJ+5-1;    
#elif defined DTLZ2
int VarNum = OBJ+10-1;
#elif defined DTLZ3
int VarNum = OBJ+10-1;   
#elif defined DTLZ4
int VarNum = OBJ+10-1;   
#elif defined DTLZ5
int VarNum = OBJ+10-1; 
#elif defined DTLZ6
int VarNum = OBJ+10-1;   
#elif defined DTLZ7
int VarNum = OBJ+20-1;  
#endif

typedef struct
{
	double alpha[BITSTRING];
	double beta[BITSTRING];
	int binary[BITSTRING];
	double real[MAXVAR];
	double fitness[OBJ];
	int rank;
	double cuboid;

	double fuzzy_integral;
	double partial_eval[OBJ];

}individual_struct;

typedef struct{
	individual_struct individual[2*SUBPOPSIZE];
}population;

typedef struct{
	individual_struct individual[2*SUBPOPULATION*SUBPOPSIZE];
	int rankno[200];
	int maxrank;
}globalpopulation;

typedef struct{
	individual_struct individual[ITERATION*SUBPOPULATION*SUBPOPSIZE];
}selectionpool;

population subpop[SUBPOPULATION];
population ref_subpop[SUBPOPULATION];
population prev_subpop[SUBPOPULATION+1];
population com_subpop;  // 소팅을 위해 부모와 자식을 합친 combined subpopulation
globalpopulation iteration_globalpop[ITERATION];
globalpopulation globalpop;
globalpopulation com_globalpop;
globalpopulation iteration_archive[ITERATION];
globalpopulation archive;
globalpopulation mig_globalpop;
globalpopulation temp_com_globalpop;
globalpopulation prev_archive;
selectionpool fi_selection_pool;

#include "Initiation.h"
#include "Decode.h"
#include "Evaluation.h"
#include "MultipleObservation.h"
#include "NondomSort.h"
#include "CrowdingDistance.h"
#include "CrowdingMigration.h"
#include "UpdateQbit.h"
#include "FuzzyIntegral.h"
#include "PreferenceSorting.h"
#include "Migration.h"
#include "PrintResult.h"


//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char *argv[])
{
	int i,j,k,m,n,runnum, FrontNum;

	srand((unsigned int)time(NULL)); // 랜덤 seed


	for (runnum=0; runnum<RUN; runnum++)
	{
	/////////////////////////	Iteration 시작~		///////////////////////////////////
		Initiation();
		for(i=0; i<ITERATION; i++){
			printf("\nIteration: %d \tGeneration: ", i+1);
			//Initiation();
			for(j=0; j<GEN; j++){
				if(j%50 == 0){
					printf("%d\t",j);
				}

				for(k=0; k<SUBPOPULATION; k++){
				
					if(j>0){ // Q-bit update
						UpdateQbit(j,k);
					}
					MultipleObservation(j, k); 
					Decode(j, k);
					Evaluation(j, k);
					if(j==0){ //첫 gen에는 소팅안해도 된다.
						//처음 gen에는 subpop == ref_subpop
						ref_subpop[k] = subpop[k];
					}
					else{
						FrontNum = NondomSort(j, k);
						CrowdingDistance(j, k, FrontNum);
						subpop[k] = com_subpop;
					}
					prev_subpop[k] = subpop[k];
				}
				Migration(j,k);
			}
			iteration_globalpop[i] = globalpop; 
			iteration_archive[i] = archive; 
			//}
		}
		PrintResult(runnum);
	}



	return 0;
}


