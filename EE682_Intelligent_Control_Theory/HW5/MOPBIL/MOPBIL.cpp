//adaptive grid�� �������� �� ����� �� ������ ������ archive�� 
//nondom�� ���ڰ� ������ diversity�� ������ ���ż��� ���� ������
//MOPBIL with adaptive grid�� ����� ����
 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <time.h>
#include <limits.h>//INT_MAX�� ���� �˱� ���� ���
#pragma warning(disable:4996)
//#include <malloc.h>


// Problem �Ķ���͵�
#define PI 3.141592
#define OBJ 3			// �����
#define DTLZ2 // ���� ���� DTLZ1, DTLZ2, DTLZ4
#define VAR_NUM 12 // variable ����
//DTLZ1=7, 
//DTLZ2=DTLZ3=DTLZ4=DTLZ5=DTLZ6=12, 
//DTLZ7 = 22
//OBJ�� 3�� �϶���....
#define BIT_NUM 20 //�ϳ��� variable�� 2������ ǥ���ϱ� ���� bit ��
#define POP 100				// Population ��
#define PV_NUM 100		    // Probability Vector ����, PV�� ������ POPó�� 100���� �� �ִ�.
#define EXTERNAL 100		// External Pareto Set �ִ� ũ��

#define ITERATION 5		// �ݺ� run ��
#define GENERATION 300		// �����

double LR=0.4;				// Learning Rate: 0.5, 0.1~0.3 ���� 0.4�� �ξ� ����. �ű��ϳ�

#define PV_UPDATE 2
// PV_update ���
//1. PBIL for MOP -> weighted sum
//2. MOPBIL -> randomly select in archive

#define MINIMIZATION
//#define MAXIMIZATION

#define L_BOUND 0
#define U_BOUND 1
#define Pm 0.02				// Mutation Ȯ�� 0.02(���� ���) vs 1/ITEM = 0.004(�Ϲ���), �������� ����
#define MS 0.02				// Mutaiton Shift!! 0.2�� ���� ������ 0.02�� �ϸ� ���� �ξ� ����


//#define Variation_LR 

#define INFINITE 9999999

int temp=0;
double dtemp=0;

//Chromosome
double BitString[POP][VAR_NUM];
int Chrom[POP][VAR_NUM][BIT_NUM];
int TempChrom[POP][VAR_NUM][BIT_NUM];
double ProbVector[PV_NUM][VAR_NUM][BIT_NUM];

//Fitness
int GenerationCount=0;
double Fitness[OBJ][POP];
double Fitness_Strength[POP]; // strength�� ���� Fitness �� �� solution�� �ϳ� ���� 


//External Set
int ChromArchive[EXTERNAL][VAR_NUM][BIT_NUM];
double FitnessArchive[OBJ][EXTERNAL];
int Current_External_Size=0; // External�� �� ���� �ʾ����� ����� sol ��

//Diversity ����
double DiversityIteration[ITERATION];

//Volume ����
double VolumeIteration[ITERATION];

//������ ����
double FitnessArchiveIteration[ITERATION][OBJ][EXTERNAL];
double avgFitnessArchiveIteration[OBJ][EXTERNAL];

void Initialize();
void CreateGene();  // PV�κ��� Binary String ����
void FitnessOBJ(); // Binary String ���κ��� Fitness ���
void UpdateArchive(); // Population���� nondominated solution���� ã�Ƽ� External�� ����
void ParetoRanking(); // strenth�� cover ����Ͽ� ��ŷ�� �ű��
void UpdatePV(); // PV�� ������Ʈ, Weighted sum ���
void Decode();

typedef struct solution{
	int chrom[VAR_NUM][BIT_NUM];
	double BitString[VAR_NUM];
	double obj[OBJ];
	int grid_loc;
	bool check;
}sol;

//Adaptive grid �ҽ��ڵ带 ���� ���� 

#define depth 5 //3obj 5�϶� MAX_LOC�� 32768�� �����Ǿ��ִ�
# define MAX_LOC 32768  // number of locations in grid (set for a three-objective problem using depth 5)
# define LARGE 200000 // should be about the maximum size of an integer for your compiler 

int arclength=0;
void update_grid(sol *s);

double gl_offset[OBJ];   // the range, offset etc of the grid 
double gl_range[OBJ];
double gl_largest[OBJ];
int grid_pop[MAX_LOC];   // the array holding the population residing in each grid location

sol *arc; 
int objectives=OBJ;
int find_loc(double *eval);


void main(){
	
	FILE *fp, *fp2, *fp3;
	fp = fopen("1_All_Result_PBIL.txt", "w"); // archive �����
	fp2 = fopen("2_Nondom_Result_PBIL.txt", "w"); // archive ���� nondom��
	fp3 = fopen("3_population_PBIL.txt", "w"); // ������ population
	
	int i=0, j=0, k=0, l=0, m=0;
	
	
	int domination_count=0;
	int OBJ_domination_count=0;
	
	arc = (sol *)malloc(EXTERNAL*sizeof(sol));

	srand(time(NULL)); // ���� seed
	
	//Iteration �迭 �ʱ�ȭ
	for(i=0; i<ITERATION ; i++){
		for(j=0; j<OBJ ; j++){
			for(k=0; k<EXTERNAL ; k++){
				FitnessArchiveIteration[i][j][k]=0;
			}
		}
	}
	//�ʱ�ȭ
	for(j=0; j<OBJ ; j++){
		for(k=0; k<EXTERNAL ; k++){
			avgFitnessArchiveIteration[j][k]=0;
		}
	}
	
	for(i=0; i<ITERATION ; i++){
		
#ifdef Variation_LR
		LR = 0.05*(i+1);
		printf("LR = %.3f\t\t",LR);
#endif
		printf("Iteration: %d,     \t", i);
		
		Initialize();
		Current_External_Size=0; // External�� �� ���� �ʾ����� ����� sol ��
	
		for(GenerationCount=0; GenerationCount<GENERATION; GenerationCount++){
			
			CreateGene();	
			Decode();
			FitnessOBJ();
			UpdateArchive();
			UpdatePV();
		}
		
		printf("Current archive size = %d\n",Current_External_Size);
		
		//Archive �� ����� ���
		temp=0;
		for(k=0; k<Current_External_Size ; k++){
			for(j=0; j<OBJ ; j++){
				if(temp==0){
					fprintf(fp,"%f",FitnessArchive[j][k]);
					temp++;
				}
				else if(j == 0){
					fprintf(fp,"\n%f",FitnessArchive[j][k]);
				}
				else{
					fprintf(fp,"\t%f",FitnessArchive[j][k]);
				}
			}
		}
		
		//pop �� ����� ���
		temp=0;
		for(k=0; k<POP ; k++){
			for(j=0; j<OBJ ; j++){
				if(temp==0){
					fprintf(fp3,"%f",Fitness[j][k]);
					temp++;
				}
				else if(j == 0){
					fprintf(fp3,"\n%f",Fitness[j][k]);
				}
				else{
					fprintf(fp3,"\t%f",Fitness[j][k]);
				}
			}
		}
		
		//��� iteration ����� ����
		for(j=0; j<OBJ ; j++){
			for(k=0; k<EXTERNAL ; k++){
				FitnessArchiveIteration[i][j][k]=FitnessArchive[j][k];
			}
		}
		
	}//iteration i �ε��� �ݱ�
	
	//archive�� ��� ��  �� non-dominated solution�� ����
	
	domination_count=0;
	OBJ_domination_count=0;
	temp=0;
	for(i=0; i<ITERATION ; i++){
		for(j=0; j<EXTERNAL ; j++){
			domination_count=0;
			for(k=0; k<ITERATION ; k++){
				for(l=0; l<EXTERNAL ; l++){
					OBJ_domination_count=0;
					for(m=0; m<OBJ ; m++){
#ifdef MINIMIZATION
						if(FitnessArchiveIteration[i][m][j] > FitnessArchiveIteration[k][m][l]){
#endif
#ifdef MAXIMIZATION						
						if(FitnessArchiveIteration[i][m][j] < FitnessArchiveIteration[k][m][l]){
#endif
							OBJ_domination_count++;
						}
					}
					if(OBJ_domination_count == OBJ){ // dominated �Ǹ�
						domination_count++; // ���� �ظ� dominate�ϴ� ���� ����
					}
				}
			}
			if(domination_count == 0){ //nondominated sol �̸�
				//fprintf(fp,"%f\t%f\n",FitnessArchiveIteration[i][0][j], FitnessArchiveIteration[i][1][j]);
				for(k=0; k<OBJ ; k++){
					if(temp==0){
						fprintf(fp2,"%f",FitnessArchiveIteration[i][k][j]);
						temp++;
					}
					else if(k == 0){
						fprintf(fp2,"\n%f",FitnessArchiveIteration[i][k][j]);
					}
					else{
						fprintf(fp2,"\t%f",FitnessArchiveIteration[i][k][j]);
					}
				}
			}
		}
	}
	
	
	fclose(fp);
	fclose(fp2);
	fclose(fp3);
	
}

void Initialize(){
	
	int i,j,k=0;
	
	// Fitness �ʱ�ȭ
	for(i=0 ; i<OBJ ; i++){
		for(j=0 ; j<POP ; j++){
			Fitness[i][j]=0;
			Fitness_Strength[j] = 0;
		}
	}
	
	// Chromosome, Probability Vector �ʱ�ȭ
	for(i=0 ; i<POP ; i++){
		for(j=0 ; j<VAR_NUM ; j++){		
			for(k=0 ; k<BIT_NUM ; k++){		
				Chrom[i][j][k]=0;
			}
		}
	}
	
	//PV �ʱ�ȭ
	for(i=0 ; i<PV_NUM ; i++){
		for(j=0 ; j<VAR_NUM ; j++){		
			for(k=0 ; k<BIT_NUM ; k++){			
				ProbVector[i][j][k]=0.5;  // PV = 0.5�� �ʱ�ȭ
			}
		}
	}
	
	
	//External set �ʱ�ȭ
	for(i=0 ; i<OBJ ; i++){
		for(j=0 ; j<EXTERNAL ; j++){
			FitnessArchive[i][j]=0;
		}
	}
	
	for(i=0 ; i<EXTERNAL ; i++){
		for(j=0 ; j<VAR_NUM ; j++){		
			for(k=0 ; k<BIT_NUM ; k++){	
				ChromArchive[i][j][k]=0;
			}
		}
	}
}

void CreateGene(){
	
	srand(time(NULL)); // ���� seed
	int i,j,k=0;
	
	// PV �� ���� Chrom[][] ������Ʈ
	// PV �� Binary sol �� ������ �ٸ���
	for(i=0 ; i<POP ; i++){
		for(j=0 ; j<VAR_NUM ; j++){		
			for(k=0 ; k<BIT_NUM ; k++){	
				if ( (double)rand()/RAND_MAX > ProbVector[i%PV_NUM][j][k]){  // PV�� ���� ����!!!!!!!! , POP ���� PV_NUM�� �� ���� �� �����Ƿ�
					Chrom[i][j][k]=0;
				}
				else {
					Chrom[i][j][k]=1;
				}
			}
		}
	}
}

void Decode(){
	int i,k;
	int sum,b,c,d,m;
	
	double coef_const = (pow(2.0,BIT_NUM) - 1);

	for(i = 0; i < POP; i++){
		for(m = 0; m < VAR_NUM ; m++){
			//finding out the co-efficient 2 to the power of (l-1) where l is the no of bits assigned to this variable For More Info Study DEB's Book
			sum = 0;
			for(k = 0; k < BIT_NUM; k++){
				b = Chrom[i][m][k];
				d = BIT_NUM - k - 1;
				c = (int)pow(2.0,d);
				sum =sum + c * b;
			}
			
			//x = BIT_NUM;
			//coef[m] = pow(2,x) - 1;
			//Parent[i].Chrom[m] =(float)(L_BOUND + (sum/coef[m])*(U_BOUND-L_BOUND));
			BitString[i][m] =(float)(L_BOUND + (sum/coef_const)*(U_BOUND-L_BOUND));
			
		}
	}
	
}


void FitnessOBJ(){
	
	//Chromosome
	//	int Chrom[POP][VAR_NUM][BIT_NUM];
	//Fitness
	//	double Fitness[OBJ][POP];
	
	int i,j,q; 
	double x[VAR_NUM];     /* problem variables */
	double f[OBJ];     /*array of fitness values*/
	
	int n = VAR_NUM;
	
	int dimension = OBJ;
	double g = 0;
	double h = 0;
	double f1 = 0;
    double alpha = 100;
	int k = n - dimension + 1;
	double t = 0;
	double theta1[10];// = malloc(dimension * sizeof(double));
	
	/////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////
	
	for(i = 0; i < POP ; i++){
		for(j = 0; j < VAR_NUM; j++){ // Binary-coded variables
			x[j] = BitString[i][j];
		}
		
		
		/*   DO NOT CHANGE ANYTHING ABOVE   */
		/*----------------------CODE YOUR OBJECTIVE FUNCTIONS HERE------------*/
		/*All functions must be of minimization type, negate maximization
		functions */
		/*============Start Coding Your Function From This Point=============*/
		
		
#ifdef DTLZ1
		
		q=0;
		j=0;
		k = n - dimension + 1;
		
		g=0;
		for (q = n - k + 1; q <= n; q++){
			g += pow(x[q-1]-0.5,2) - cos(20 * PI * (x[q-1]-0.5));
		}
		
		g = 100 * (k + g);
		for (q = 1; q <= dimension; q++){
			f1 = 0.5 * (1 + g);
			for (j = dimension - q; j >= 1; j--){
				f1 *= x[j-1];
			}
			if (q > 1){
				f1 *= 1 - x[(dimension - q + 1) - 1];
			}
			f[q-1] = f1;
		}
#endif
		
#ifdef DTLZ2
		
		q=0;
		j=0;
		k = n - dimension + 1;
		
		g = 0;
		for (q = n - k + 1; q <= n; q++)
		{
			g += pow(x[q-1]-0.5,2);
		}
		
		for (q = 1; q <= dimension; q++)
		{
			f1 = (1 + g);
			for (j = dimension - q; j >= 1; j--)
			{
				f1 *= cos(x[j-1] * PI / 2);
			}
			if (q > 1)
			{
				f1 *= sin(x[(dimension - q + 1) - 1] * PI / 2);
			}
			
			f[q-1] = f1;
		}
		
#endif
		
#ifdef DTLZ3
		
		q=0;
		j=0;
		k = n - dimension + 1;
		//f1 = 0;
		
		g = 0;
		for (q = n - k + 1; q <= n; q++)
		{
			g += pow(x[q-1]-0.5,2) - cos(20 * PI * (x[q-1]-0.5));
		}
		g = 100 * (k + g);
		
		for (q = 1; q <= dimension; q++)
		{
			f1 = (1 + g);
			for (j = dimension - q; j >= 1; j--)
			{
				f1 *= cos(x[j-1] * PI / 2);
			}
			if (q > 1)
			{
				f1 *= sin(x[(dimension - q + 1) - 1] * PI / 2);
			}
			
			f[q-1] = f1;
		}
		
		
#endif
		
#ifdef DTLZ4
		
		q=0;
		j=0;
		alpha =100;
		k = n - dimension + 1;	
		
		g = 0;
		
		for (q = n - k + 1; q <= n; q++){
			g += pow(x[q-1]-0.5,2);
		}
		
		for (q = 1; q <= dimension; q++){
			f1 = (1 + g);
			for (j = dimension - q; j >= 1; j--){
				f1 *= cos(pow(x[j-1],alpha) * PI / 2);
			}
			if (q > 1){
				f1 *= sin(pow(x[(dimension - q + 1) - 1],alpha) * PI / 2);
			}
			f[q-1] = f1;
		}
		
#endif
		
#ifdef DTLZ5
		
		q = 0;
		j = 0;
		k = n - dimension + 1;
		
		t = 0;
		g = 0;
		
		for (q = n - k + 1; q <= n; q++)
		{
			g += pow(x[q-1] - 0.5, 2);
		}
		
		t = PI / (4 * (1 + g));
		theta1[0] = x[0] * PI / 2;
		
		for (q = 2; q <= dimension - 1; q++)
		{
			theta1[q-1] = t * (1 + 2 * g * x[q-1]);
		}
		
		for (q = 1; q <= dimension; q++)
		{
			f1 = (1 + g);
			for (j = dimension - q; j >= 1; j--)
			{
				f1 *= cos(theta1[j-1]);
			}
			if (q > 1)
			{
				f1 *= sin(theta1[(dimension - q + 1) - 1]);
			}
			
			f[q-1] = f1;
		}
		
		
		
#endif
		
#ifdef DTLZ6
		q = 0;
		j = 0;
		
		k = n - dimension + 1;
		t = 0;
		g = 0;
		
		for (q = n - k + 1; q <= n; q++)
		{
			g += pow(x[q-1], 0.1);
		}
		
		t = PI / (4 * (1 + g));
		theta1[0] = x[0] * PI / 2;
		for (q = 2; q <= dimension - 1; q++)
		{
			theta1[q-1] = t * (1 + 2 * g * x[q-1]);
		}
		
		for (q = 1; q <= dimension; q++)
		{
			f1 = (1 + g);
			for (j = dimension - q; j >= 1; j--)
			{
				f1 *= cos(theta1[j-1]);
			}
			if (q > 1)
			{
				f1 *= sin(theta1[(dimension - q + 1) - 1]);
			}
			
			f[q-1] = f1;
		}
		
#endif
		
#ifdef DTLZ7
		
		q=0;
		j=0;
		k = n - dimension + 1; //�ʱ�ȭ �� �ؾ��� ����!!!
		//!!!�ʱ�ȭ �߸��ϸ� f�� ���Ѵ� ���� ���´�, keepalive.h���� ���� ������ ��
		
		g = 0;
		h = 0; //�ʱ�ȭ �� �ؾ��� ����!!!
		
		
		for (q = n - k + 1; q <= n; q++)
		{
			g += x[q-1];
		}
		g = 1 + 9 * g / k;
		
		for (q = 1; q <= dimension - 1; q++)
		{
			f[q-1] = x[q-1];
		}
		
		for (j = 1; j <= dimension - 1; j++)
		{
			h += x[j-1] / (1 + g) * (1 + sin(3 * PI * x[j-1]));
		}
		h = dimension - h;
		f[dimension - 1] = (1 + g) * h;
		
#endif
		
		
		for(k = 0 ; k < OBJ ;k++){
			Fitness[k][i]  = f[k];
		}
	  }
 }
 

 //Adaptive grid  ����
 void UpdateArchive(){
	 
	 int i,j,k,m;
	 int domination_count=0;
	 int OBJ_domination_count=0;
	 int no_nondom_POP_Archive=0;
	 int delete_count=0;
	 int most_crowd_grid=0;
	 int sol_num_grid=0;
	 
	 sol Population[POP];
	 sol Archive[EXTERNAL];
	 sol ArchiveTemp[EXTERNAL];
	 
	 
	 //ù ���뿡�� POP -> Archive�׳� ����
	 if(GenerationCount==0){
		 for(i=0 ; i<EXTERNAL ; i++){
			 for(j=0 ; j<OBJ ; j++){
				 FitnessArchive[j][i] = Fitness[j][i];
			 }
			 for(j=0 ; j<VAR_NUM ; j++){		
				 for(k=0 ; k<BIT_NUM ; k++){	
					 ChromArchive[i][j][k] = Chrom[i][j][k];
				 }
			 }
		 }
		 Current_External_Size = EXTERNAL;
		 
	 }
	 else{// �ι��� ������ʹ� Adaptive grid�� archive ������Ʈ
		 
		 //POP ����ü�� �ӽ� ����, adaptive grid �Լ� Ȱ���� ����
		 for(i=0 ; i<POP ; i++){
			 for(j=0 ; j<OBJ ; j++){
				 Population[i].obj[j] = Fitness[j][i];
			 }
			 for(j=0 ; j<VAR_NUM ; j++){		
				 for(k=0 ; k<BIT_NUM ; k++){	
					 Population[i].chrom[j][k] = Chrom[i][j][k];
				 }
			 }
		 }
		 
		 //Archive����ü�� �ӽ� ����, adaptive grid �Լ� Ȱ���� ����
		 for(i=0 ; i<EXTERNAL ; i++){
			 for(j=0 ; j<OBJ ; j++){
				 Archive[i].obj[j] = FitnessArchive[j][i];
			 }
			 for(j=0 ; j<VAR_NUM ; j++){		
				 for(k=0 ; k<BIT_NUM ; k++){	
					 Archive[i].chrom[j][k] = ChromArchive[i][j][k];
				 }
			 }
		 }
		 
		 //���ο� solution �ϳ� �ϳ��� archive�� ���� ���� �˻��Ѵ�
		 for(i=0 ; i<POP ; i++){
			 delete_count=0;
			 //�켱 ���ο� �ذ� archive�� �ص��� dominate�ϴ��� �˻�
			 for(j=0 ; j<Current_External_Size ; j++){
				 OBJ_domination_count=0;
				 for(m=0; m<OBJ ; m++){
#ifdef MINIMIZATION
					 if(Population[i].obj[m] < Archive[j].obj[m]){ //min ���� 
#endif
#ifdef MAXIMIZATION
					 if(Population[i].obj[m] > Archive[j].obj[m]){ //max ���� 
#endif
						 OBJ_domination_count++;
					 }
				 }
				 if(OBJ_domination_count == OBJ){ // ���� �ذ� archive�ظ� dominate �ϸ�
					 // archive���� ���ŵǾ��� �ظ� üũ�Ѵ�. 
					 Archive[j].check = true;
					 delete_count++;
					 
				 }
				 else{
					 Archive[j].check = false;
				 }
			 }
			 
			 if (delete_count > 0){ //����� �ϳ��� ������, �� �����ذ� archive�� �ظ� dominate �ϸ�
				 //üũ�� �ص��� ��� ����� ���ο� �ظ� �ִ´�
				 k=0;
				 for(j=0 ; j<Current_External_Size ; j++){//üũ�� �ظ� ������ �͵鸸 �ӽ�����
					 if (Archive[j].check == false){
						 ArchiveTemp[k] = Archive[j];
						 k++;
					 }
				 }
				 ArchiveTemp[k] = Population[i]; //���� �ظ� ����
				 k++;
				 Current_External_Size = k;
				 for(j=0 ; j<Current_External_Size ; j++){//�ӽ����� �޴� ���� ����
					 Archive[j] = ArchiveTemp[j] ;
				 }
			 }
			 else{ // �����ذ� archive�� �ظ� dominate���� ������
				 
				 if (Current_External_Size == EXTERNAL){//archive�� ��á��?
					 // ���� �ذ� archive�� diversity�� ������Ű���� �˻� 
					 // -> diversity�� ������Ų�ٴ� �ǹ̸� �Ʒ��� ���� �ؼ��޴� -����
					 // -> �ƹ��ذ� ���� grid�� �����ذ� ��ġ�Ҷ� archive�� �߰��Ѵ�
					 
					 //�ϴ� Archive[]�� *arc�� ����
					 arclength = Current_External_Size;
					 for(j=0 ; j<Current_External_Size ; j++){
						 arc[j] = Archive[j];
					 }
					 
					 //grid_pop �ʱ�ȭ
					 for(j=0 ; j<MAX_LOC; j++){
						 grid_pop[j]=0;
					 }
					 
					 //���ο� �ظ� ���� arc�� grid ������ ������Ʈ
					 //archive�� �����, ���� ���� grid location�� ã�´�
					 //grid_pop�� �� grid�� ��ġ�� ���� ������ ����Ǿ� �ִ�. Archive ����� + ���� �� 1��
					 update_grid(&Population[i]);
					 
					 for(j=0 ; j<Current_External_Size ; j++){ //Archive[] �ٽ� ����
						 Archive[j] = arc[j];
					 }
					 
					 // ���� �ذ� �ִ� grid ��ġ�� Archive �ذ� �ϳ��� ������ �ȳְ�
					 // Archive �ذ� �ϳ��� ������ ���� crowd�Ѱ��� archive�ظ� ����� �ִ´�
					 
					 //�������� grid ��ġ�� ���������
					 //0���� �ʱ�ȭ�߾����ϱ� 1�̸� ������ �ڱ��ڽŹۿ� ���ٴ� ��
					 
					 if (grid_pop[Population[i].grid_loc] == 1){ 
						 //���� crowd�� grid ��ġ�� ã�´�
						 most_crowd_grid=0;
						 sol_num_grid=0;
						 for(j=0 ; j<MAX_LOC; j++){ 
							 if (sol_num_grid < grid_pop[j]){
								 sol_num_grid = grid_pop[j];
								 most_crowd_grid=j;
							 }
						 }
						 //most_crowd_grid �� �ִ� �� �ؿ� ��ü
						 for(j=0 ; j<Current_External_Size; j++){ 
							 if (Archive[j].grid_loc == most_crowd_grid){
								 Archive[j] = Population[i];
								 break; // �ϳ��� ��ü�ؾ� �ϹǷ� break
							 }
						 }
					 }
				 } 
				 else{//archive�� ������ �ʾҴ�
					 Archive[Current_External_Size] = Population[i]; //���� �ظ� ����
					 Current_External_Size++;
				 }
			 }
		 }
		 
		 //Archive����ü�� �ӽ� �����ߴ� ���� �ٽ� ����
		 for(i=0 ; i<EXTERNAL ; i++){
			 for(j=0 ; j<OBJ ; j++){
				 FitnessArchive[j][i] = Archive[i].obj[j];
			 }
			 for(j=0 ; j<VAR_NUM ; j++){		
				 for(k=0 ; k<BIT_NUM ; k++){	
					 ChromArchive[i][j][k] = Archive[i].chrom[j][k];
				 }
			 }
		 }
	}
}


void UpdatePV(){
	
	srand(time(0)); // ���� seed	
	
	int i,j,q,k;
	int best_sol=0; // best solution�� ��ȣ
	double sum=0;
	double RandomWeight[OBJ];
	double WeightedFitness[EXTERNAL]; //External �ظ�
	
	// PV ������Ʈ
	for(q=0 ; q<PV_NUM ; q++){
		if(PV_UPDATE == 1){ //PBIL for MOP
			// Original PBIL for MOP	
			// sum=1 �� weight �����ϰ� ����
			for(i=0 ; i<OBJ ; i++){
				RandomWeight[i] = (double)rand()/RAND_MAX;
				sum = sum + RandomWeight[i];
			}
			
			for(i=0 ; i<OBJ ; i++){
				RandomWeight[i] = RandomWeight[i]/sum;
			}
			
			// weight ���ϱ� FitnessArchive
			// ���� pop + archive ���� best�� �̾ƾ� ������ �ϴ� archive������ best b�� ã�ƺ���
			
			//�ʱ�ȭ
			for(i=0 ; i<EXTERNAL ; i++){
				WeightedFitness[i]=0; 
			}
			
			// Weighted Fitness ���
			for(i=0 ; i<Current_External_Size ; i++){
				for(j=0 ; j<OBJ ; j++){
					WeightedFitness[i] += (FitnessArchive[j][i]*RandomWeight[j]);
					
				}
			}
			
			best_sol=0;
			for(i=0 ; i<Current_External_Size ; i++){
				if(WeightedFitness[best_sol] < WeightedFitness[i]){
					best_sol = i;
				}
			}
		}
		for(j=0 ; j<VAR_NUM ; j++){		
			for(k=0 ; k<BIT_NUM ; k++){	
				//weighted sum �ؼ� ����Ʈ �ϳ��� �����ϴ� �ͺ��� �Ź� nondom �� �����ϰ� �����ϴ°� �� ����.
				if(PV_UPDATE==2){ //MOPBIL
					best_sol = rand()%Current_External_Size; // �����ϴ� ���: best�� �����ϰ� �����Ѵ�.
				}
				ProbVector[q][j][k] = (ProbVector[q][j][k]*(1-LR)) + (ChromArchive[best_sol][j][k]*LR); //������Ʈ
				
				if(((double)rand()/RAND_MAX) < Pm){ //!!!!!!double �Ⱥ��̸� 0�̴�
					ProbVector[q][j][k] = (ProbVector[q][j][k]*(1-MS)) + ((rand()%2)*MS); //Mutation
				}
			}
		}
	}
}


void update_grid(sol *s)
{
	// recalculate ranges for grid in the light of a new solution s
	static int change = 0;
	int a, b;
	int square;
	double offset[OBJ];
	double largest[OBJ];
	double sse;
	double product;
	
	for (a = 0; a < objectives; a++)
    {
		offset[a] = LARGE;
		largest[a] = -LARGE;
    }
	
	for (b = 0; b < objectives; b++){
		for (a = 0; a < arclength; a++){
			if ((&arc[a])->obj[b] < offset[b])
				offset[b] = (&arc[a])->obj[b];
			if ((&arc[a])->obj[b] > largest[b])
				largest[b] = (&arc[a])->obj[b];		   
		}
    }
	//    printf("oldCURENT:largest = %f, offset = %f\n", largest[0], offset[0]); 
	//    printf("oldCURENT:largest = %f, offset = %f\n", largest[1], offset[1]); 
	
	
	for (b = 0; b < objectives; b++)
    {
		if (s->obj[b] < offset[b])
			offset[b] = s->obj[b];
		if (s->obj[b] > largest[b])
			largest[b] = s->obj[b];		   
    }
	
	sse = 0;
	product = 1;
    
	for (a = 0; a < objectives; a++)
    {
		
		sse += ((gl_offset[a] - offset[a])*(gl_offset[a] - offset[a]));
		sse += ((gl_largest[a] - largest[a])*(gl_largest[a] - largest[a]));
		product *= gl_range[a]; 
    }
	
	// printf("sse = %f\n", sse);
	if (sse > (0.1 * product * product)){	//if the summed squared error (difference) between old and new
		//minima and maxima in each of the objectives
		//is bigger than 10 percent of the square of the size of the space
		change++;                         // then renormalise the space and recalculte grid locations
	 	
		for (a = 0; a < objectives; a++){
			gl_largest[a] = largest[a]+0.2*largest[a];
			gl_offset[a] = offset[a]-0.2*offset[a];
			gl_range[a] = gl_largest[a] - gl_offset[a];
			
			//			printf("%.2f\t%.2f\n", gl_offset[a], gl_largest[a]);
		}
		
		for (a = 0; a < pow(2.0, (objectives*depth)); a++){
			grid_pop[a] = 0;
		}
		
		for (a = 0; a < arclength; a++){
			square = find_loc((&arc[a])->obj);
			(&arc[a])->grid_loc = square;
			grid_pop[square]++;
			
		}
    }
	square = find_loc(s->obj);
	s->grid_loc = square;
	grid_pop[(int)pow(2.0,(OBJ*depth))] = -5;
	grid_pop[square]++;   
	
}

int find_loc(double *eval)
{
	// find the grid location of a solution given a vector of its objective values
	
	int loc = 0;
	int d;
	int n = 1;
	
	int i;
	
	int inc[OBJ];
	double width[OBJ];
	double offset[OBJ];
	
	// printf("obj = %d, depth = %d\n", objectives, depth);
	
	// if the solution is out of range on any objective, return 1 more than the maximum possible grid location number
	for (i = 0; i < objectives; i++){
		if ((eval[i] < gl_offset[i])||(eval[i] > gl_offset[i] + gl_range[i]))
			return((int)pow(2.0,(objectives*depth)));
    }
	
	for (i = 0; i < objectives; i++){
		inc[i] = n;
		n *=2;
		width[i] = gl_range[i];
		offset[i]=gl_offset[i];
    }
	
	for (d = 1; d <= depth; d++){
		for (i = 0; i < objectives; i++){
			if(eval[i] < width[i]/2+offset[i])
				loc += inc[i];
			else
				//gl_offset[i] += width[i]/2;
				offset[i] += width[i]/2;
		}
		for (i = 0; i < objectives; i++){
			inc[i] *= (objectives *2);
			width[i] /= 2;
		}
    }
	return(loc);
} 
