#ifndef GA_DEJONG_H
#define GA_DEJONG_H


#define TRUE 		1
#define FALSE 		0
#define PHI			3.14159265

/* Tuning parameters */
#define	NRUNS		10         /* Number of Runs */
#define POPSIZE 	50		    /* Population size */
#define MAXGENS 	500	        /* Max. number of generations */
#define PXOVER 		0.25		/* Probability of crossover */
#define PMUTATION 	0.02	    /* Probability of mutation */
#define ERRORRANGE  0.0001

/* Problem specific parameters */
#define DEJONG      5           /* Dejong function */
#define METHOD      3           /* Selection method: 1)Select, 2)Ranking, 3)Tournament */
#define NX          2           /* No. of variables in function */
#define FROM        -65.536      /* start of interval for variable */
#define TO          65.536      /* end of interval for variable */
#define NVARS 		NX*18       /* Number of bits in a chromosome, 18 bits for each individual */
#define OFFSET		10000         /* To make a max. problem: 1:50,2:100, 3:100, 4:100 5:10000 */
#define OPTIMAL     1        /* Optimal value of function? */

/* File parameters */
#define FILE_LOG_NAME "GA_output/D5/galog_tournament.txt"
#define FILE_SOL_NAME "GA_output/D5/solution_tournament.txt"

struct genotype           /* Genotype (GT), a member of the population */
{
	int	gene[NVARS];	  /* A binary string of chromosome */
	double	fitness;	  /* GT's fitness */
	double	rfitness;	  /* Relative fitness */
	double	cfitness;	  /* Cumulative fitness */
};

/* Declaration of procedures used by this genetic algorithm */
void initialize(void);
void evaluate(void);
void keep_the_best(void);
void elitist(void);
void Select(void);
void RANKING(void);
void TOURNAMENT(void);
void crossover(void);
void Xover(int,int);
void swap(int *, int *);
void mutate(void);
void report(void);
double DEJONG1(double *x);
double DEJONG2(double *x);
double DEJONG3(double *x);
double DEJONG4(double *x);
double DEJONG5(double *x);

#endif /* GA_DEJONG_H */