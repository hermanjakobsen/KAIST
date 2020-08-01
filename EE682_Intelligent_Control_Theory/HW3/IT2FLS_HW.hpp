#pragma once

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

typedef struct in_mem {
	double *width;         // Input membership function width (1/2 of triangle base).  
	double *center;       // Center of each input membership function.                
	double *FoU;			// FoU values for each input membership function			
	double *dom_U;          // Degree of membership for each upper membership function. 
	double *dom_L;          // Degree of membership for each lower membership function. 
} IN_MEM;

typedef struct out_mem {
	double *width;         /* Output membership function width (1/2 of triangle base). */
	double *center;       /* Center of each output membership function.   */
	double *FoU;			// FoU values for each output membership function
} OUT_MEM;

typedef struct fuz_sys {
	IN_MEM  *emem;        /* Groups all fuzzy system parameters in a single variable. */
	IN_MEM  *edotmem;
	OUT_MEM *outmem_LM;
	OUT_MEM *outmem_RM;
} FUZ_SYS;

void fuzzy_init(FUZ_SYS *fuzzy_system, int type = 2);
void fuzzy_free(FUZ_SYS *fuzzy_system);
double fuzzy_control(double e, double edot, FUZ_SYS *fuzzy_system);
void fuzzyify(double u, IN_MEM *mem);
double leftall(double u, double w, double c, double f);
double rightall(double u, double w, double c, double f);
double triangle(double u, double w, double c, double f);
double gaussian(double u, double w, double c, double f);
void match(const IN_MEM *emem, const IN_MEM *edotmem, int *pos_U, int *pos_L);
double inf_defuzz(IN_MEM *emem, IN_MEM *edotmem, OUT_MEM *outmem_LM, OUT_MEM *outmem_RM, int *pos, int flg);