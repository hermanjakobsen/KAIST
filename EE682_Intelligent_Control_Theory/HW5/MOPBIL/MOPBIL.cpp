//adaptive grid를 적용했을 때 결과가 더 않조은 이유는 archive에 
//nondom의 숫자가 많아져 diversity는 좋으나 수렴성은 조금 떨어짐
//MOPBIL with adaptive grid의 결과는 좋다
 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <time.h>
#include <limits.h>//INT_MAX의 값을 알기 위해 사용
#pragma warning(disable:4996)
//#include <malloc.h>


// Problem 파라미터들
#define PI 3.141592
#define OBJ 3			// 가방수
#define DTLZ2 // 문제 종류 DTLZ1, DTLZ2, DTLZ4
#define VAR_NUM 12 // variable 갯수
//DTLZ1=7, 
//DTLZ2=DTLZ3=DTLZ4=DTLZ5=DTLZ6=12, 
//DTLZ7 = 22
//OBJ가 3개 일때임....
#define BIT_NUM 20 //하나의 variable을 2진수로 표현하기 위한 bit 수
#define POP 100				// Population 수
#define PV_NUM 100		    // Probability Vector 갯수, PV의 갯수가 POP처럼 100개일 수 있다.
#define EXTERNAL 100		// External Pareto Set 최대 크기

#define ITERATION 5		// 반복 run 수
#define GENERATION 300		// 세대수

double LR=0.4;				// Learning Rate: 0.5, 0.1~0.3 보다 0.4가 훨씬 좋다. 신기하네

#define PV_UPDATE 2
// PV_update 방식
//1. PBIL for MOP -> weighted sum
//2. MOPBIL -> randomly select in archive

#define MINIMIZATION
//#define MAXIMIZATION

#define L_BOUND 0
#define U_BOUND 1
#define Pm 0.02				// Mutation 확률 0.02(논문에 명시) vs 1/ITEM = 0.004(일반적), 성능차이 없다
#define MS 0.02				// Mutaiton Shift!! 0.2로 했을 때보다 0.02로 하면 성능 훨씬 좋다


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
double Fitness_Strength[POP]; // strength를 더한 Fitness 값 한 solution당 하나 존재 


//External Set
int ChromArchive[EXTERNAL][VAR_NUM][BIT_NUM];
double FitnessArchive[OBJ][EXTERNAL];
int Current_External_Size=0; // External이 꽉 차지 않았을때 저장된 sol 수

//Diversity 측정
double DiversityIteration[ITERATION];

//Volume 측정
double VolumeIteration[ITERATION];

//실험결과 정리
double FitnessArchiveIteration[ITERATION][OBJ][EXTERNAL];
double avgFitnessArchiveIteration[OBJ][EXTERNAL];

void Initialize();
void CreateGene();  // PV로부터 Binary String 생성
void FitnessOBJ(); // Binary String 으로부터 Fitness 계산
void UpdateArchive(); // Population에서 nondominated solution들을 찾아서 External에 저장
void ParetoRanking(); // strenth와 cover 계산하여 랭킹을 매긴다
void UpdatePV(); // PV를 업데이트, Weighted sum 방식
void Decode();

typedef struct solution{
	int chrom[VAR_NUM][BIT_NUM];
	double BitString[VAR_NUM];
	double obj[OBJ];
	int grid_loc;
	bool check;
}sol;

//Adaptive grid 소스코드를 위한 정의 

#define depth 5 //3obj 5일때 MAX_LOC가 32768로 설정되어있다
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
	fp = fopen("1_All_Result_PBIL.txt", "w"); // archive 모든해
	fp2 = fopen("2_Nondom_Result_PBIL.txt", "w"); // archive 에서 nondom만
	fp3 = fopen("3_population_PBIL.txt", "w"); // 마지막 population
	
	int i=0, j=0, k=0, l=0, m=0;
	
	
	int domination_count=0;
	int OBJ_domination_count=0;
	
	arc = (sol *)malloc(EXTERNAL*sizeof(sol));

	srand(time(NULL)); // 랜덤 seed
	
	//Iteration 배열 초기화
	for(i=0; i<ITERATION ; i++){
		for(j=0; j<OBJ ; j++){
			for(k=0; k<EXTERNAL ; k++){
				FitnessArchiveIteration[i][j][k]=0;
			}
		}
	}
	//초기화
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
		Current_External_Size=0; // External이 꽉 차지 않았을때 저장된 sol 수
	
		for(GenerationCount=0; GenerationCount<GENERATION; GenerationCount++){
			
			CreateGene();	
			Decode();
			FitnessOBJ();
			UpdateArchive();
			UpdatePV();
		}
		
		printf("Current archive size = %d\n",Current_External_Size);
		
		//Archive 의 모든해 출력
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
		
		//pop 의 모든해 출력
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
		
		//모든 iteration 결과를 저장
		for(j=0; j<OBJ ; j++){
			for(k=0; k<EXTERNAL ; k++){
				FitnessArchiveIteration[i][j][k]=FitnessArchive[j][k];
			}
		}
		
	}//iteration i 인덱스 닫기
	
	//archive의 모든 해  중 non-dominated solution만 저장
	
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
					if(OBJ_domination_count == OBJ){ // dominated 되면
						domination_count++; // 현재 해를 dominate하는 해의 갯수
					}
				}
			}
			if(domination_count == 0){ //nondominated sol 이면
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
	
	// Fitness 초기화
	for(i=0 ; i<OBJ ; i++){
		for(j=0 ; j<POP ; j++){
			Fitness[i][j]=0;
			Fitness_Strength[j] = 0;
		}
	}
	
	// Chromosome, Probability Vector 초기화
	for(i=0 ; i<POP ; i++){
		for(j=0 ; j<VAR_NUM ; j++){		
			for(k=0 ; k<BIT_NUM ; k++){		
				Chrom[i][j][k]=0;
			}
		}
	}
	
	//PV 초기화
	for(i=0 ; i<PV_NUM ; i++){
		for(j=0 ; j<VAR_NUM ; j++){		
			for(k=0 ; k<BIT_NUM ; k++){			
				ProbVector[i][j][k]=0.5;  // PV = 0.5로 초기화
			}
		}
	}
	
	
	//External set 초기화
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
	
	srand(time(NULL)); // 랜덤 seed
	int i,j,k=0;
	
	// PV 를 보고 Chrom[][] 업데이트
	// PV 와 Binary sol 의 갯수는 다르다
	for(i=0 ; i<POP ; i++){
		for(j=0 ; j<VAR_NUM ; j++){		
			for(k=0 ; k<BIT_NUM ; k++){	
				if ( (double)rand()/RAND_MAX > ProbVector[i%PV_NUM][j][k]){  // PV의 갯수 주의!!!!!!!! , POP 보다 PV_NUM이 더 작을 수 있으므로
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
		k = n - dimension + 1; //초기화 꼭 해야함 주의!!!
		//!!!초기화 잘못하면 f가 무한대 값이 나온다, keepalive.h에서 에러 리포팅 함
		
		g = 0;
		h = 0; //초기화 꼭 해야함 주의!!!
		
		
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
 

 //Adaptive grid  버전
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
	 
	 
	 //첫 세대에는 POP -> Archive그냥 복사
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
	 else{// 두번재 세대부터는 Adaptive grid로 archive 업데이트
		 
		 //POP 구조체에 임시 저장, adaptive grid 함수 활용을 위해
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
		 
		 //Archive구조체에 임시 저장, adaptive grid 함수 활용을 위해
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
		 
		 //새로운 solution 하나 하나를 archive에 넣을 건지 검사한다
		 for(i=0 ; i<POP ; i++){
			 delete_count=0;
			 //우선 새로운 해가 archive의 해들을 dominate하는지 검사
			 for(j=0 ; j<Current_External_Size ; j++){
				 OBJ_domination_count=0;
				 for(m=0; m<OBJ ; m++){
#ifdef MINIMIZATION
					 if(Population[i].obj[m] < Archive[j].obj[m]){ //min 문제 
#endif
#ifdef MAXIMIZATION
					 if(Population[i].obj[m] > Archive[j].obj[m]){ //max 문제 
#endif
						 OBJ_domination_count++;
					 }
				 }
				 if(OBJ_domination_count == OBJ){ // 현재 해가 archive해를 dominate 하면
					 // archive에서 제거되야할 해를 체크한다. 
					 Archive[j].check = true;
					 delete_count++;
					 
				 }
				 else{
					 Archive[j].check = false;
				 }
			 }
			 
			 if (delete_count > 0){ //지울게 하나라도 있으면, 즉 현재해가 archive의 해를 dominate 하면
				 //체크된 해들을 모두 지우고 새로운 해를 넣는다
				 k=0;
				 for(j=0 ; j<Current_External_Size ; j++){//체크된 해를 제외한 것들만 임시저장
					 if (Archive[j].check == false){
						 ArchiveTemp[k] = Archive[j];
						 k++;
					 }
				 }
				 ArchiveTemp[k] = Population[i]; //현재 해를 삽입
				 k++;
				 Current_External_Size = k;
				 for(j=0 ; j<Current_External_Size ; j++){//임시저장 햇던 것을 복구
					 Archive[j] = ArchiveTemp[j] ;
				 }
			 }
			 else{ // 현재해가 archive의 해를 dominate하지 않으면
				 
				 if (Current_External_Size == EXTERNAL){//archive가 꽉찼냐?
					 // 현재 해가 archive의 diversity를 증가시키는지 검사 
					 // -> diversity를 증가시킨다는 의미를 아래와 같이 해석햇다 -예훈
					 // -> 아무해가 없는 grid에 현재해가 위치할때 archive에 추가한다
					 
					 //일단 Archive[]를 *arc에 복사
					 arclength = Current_External_Size;
					 for(j=0 ; j<Current_External_Size ; j++){
						 arc[j] = Archive[j];
					 }
					 
					 //grid_pop 초기화
					 for(j=0 ; j<MAX_LOC; j++){
						 grid_pop[j]=0;
					 }
					 
					 //새로운 해를 보고 arc의 grid 정보를 업데이트
					 //archive의 모든해, 현재 해의 grid location을 찾는다
					 //grid_pop에 각 grid에 위치한 해의 갯수가 저장되어 있다. Archive 모든해 + 현재 해 1개
					 update_grid(&Population[i]);
					 
					 for(j=0 ; j<Current_External_Size ; j++){ //Archive[] 다시 복구
						 Archive[j] = arc[j];
					 }
					 
					 // 현재 해가 있는 grid 위치에 Archive 해가 하나라도 있으면 안넣고
					 // Archive 해가 하나도 없으면 제일 crowd한곳의 archive해를 지우고 넣는다
					 
					 //현재해의 grid 위치가 비어있으면
					 //0으로 초기화했엇으니까 1이면 현재해 자기자신밖에 없다는 뜻
					 
					 if (grid_pop[Population[i].grid_loc] == 1){ 
						 //가장 crowd한 grid 위치를 찾는다
						 most_crowd_grid=0;
						 sol_num_grid=0;
						 for(j=0 ; j<MAX_LOC; j++){ 
							 if (sol_num_grid < grid_pop[j]){
								 sol_num_grid = grid_pop[j];
								 most_crowd_grid=j;
							 }
						 }
						 //most_crowd_grid 에 있는 한 해와 교체
						 for(j=0 ; j<Current_External_Size; j++){ 
							 if (Archive[j].grid_loc == most_crowd_grid){
								 Archive[j] = Population[i];
								 break; // 하나만 교체해야 하므로 break
							 }
						 }
					 }
				 } 
				 else{//archive가 꽉차지 않았다
					 Archive[Current_External_Size] = Population[i]; //현재 해를 삽입
					 Current_External_Size++;
				 }
			 }
		 }
		 
		 //Archive구조체에 임시 저장했던 것을 다시 복구
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
	
	srand(time(0)); // 랜덤 seed	
	
	int i,j,q,k;
	int best_sol=0; // best solution의 번호
	double sum=0;
	double RandomWeight[OBJ];
	double WeightedFitness[EXTERNAL]; //External 해만
	
	// PV 업데이트
	for(q=0 ; q<PV_NUM ; q++){
		if(PV_UPDATE == 1){ //PBIL for MOP
			// Original PBIL for MOP	
			// sum=1 인 weight 랜덤하게 생성
			for(i=0 ; i<OBJ ; i++){
				RandomWeight[i] = (double)rand()/RAND_MAX;
				sum = sum + RandomWeight[i];
			}
			
			for(i=0 ; i<OBJ ; i++){
				RandomWeight[i] = RandomWeight[i]/sum;
			}
			
			// weight 곱하기 FitnessArchive
			// 원래 pop + archive 에서 best를 뽑아야 하지만 일단 archive에서만 best b를 찾아본다
			
			//초기화
			for(i=0 ; i<EXTERNAL ; i++){
				WeightedFitness[i]=0; 
			}
			
			// Weighted Fitness 계산
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
				//weighted sum 해서 베스트 하나를 선정하는 것보다 매번 nondom 중 랜덤하게 선택하는게 더 낫다.
				if(PV_UPDATE==2){ //MOPBIL
					best_sol = rand()%Current_External_Size; // 제안하는 방법: best를 랜덤하게 선정한다.
				}
				ProbVector[q][j][k] = (ProbVector[q][j][k]*(1-LR)) + (ChromArchive[best_sol][j][k]*LR); //업데이트
				
				if(((double)rand()/RAND_MAX) < Pm){ //!!!!!!double 안붙이면 0이다
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
