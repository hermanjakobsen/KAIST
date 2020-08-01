// File_A.txt 와 File_B.txt에서 데이터 읽어와서 Coverage, Diversity 계산
// DATA file 내의 서식은 F1\tF2\tF3 -> 탭으로 구분되어 있어야 한다
 
#include <stdio.h>
#include <stdlib.h>
//#include <iostream.h>
#include <math.h>
#include <limits.h>//INT_MAX의 값을 알기 위해 사용
#pragma warning(disable:4996)
//#define MAXIMIZATION	//Maximize 문제일 경우
#define MINIMIZATION

//#define MAX_RUN 1 //10번돌려도 하나의 nondom set으로 묶어서 다루므로 무조건 1이다.
#define MAX_OBJ 12 
#define RESULT_NUM 3000 // Max. popuation 갯수, 충분히 잡아준다, 100 pop을 30run 하면 최대 3000개
#define MAX_STR_LENGTH  256 // maximal length of strings in the files 
#define MAX_LINE_LENGTH  2048 // maximal length of lines in the files 

#define error(X,Y)  if (X) printf(Y "\n"), exit(1)

int NoObj=0; // Objectives의 갯수

int determine_dim(FILE  *fp); // 파일의 Objective 수 체크

 
void main(){

	int temp=0;
	int i=0, j=0, k=0, l=0, m=0, ll = 0, nn = 0;
	int count_A=0, count_B=0, count_AB=0;
	FILE *File_A, *File_B, *ResultFile;
	double temp_A;
	double temp_B;
	double mean_A=0;
	double mean_B=0;

	double mean2_A=0;
	double mean2_B=0;

	double mean3_A=0;
	double mean3_B=0;	

	double Result_A[MAX_OBJ][RESULT_NUM];	// 비교할 원래 알고리즘
	double Result_B[MAX_OBJ][RESULT_NUM];	// 제안하는 알고리즘
	
	char filename[40];

	for(ll=1;ll<8;ll++) {		
		for(nn=0;nn<50;nn++) {
			sprintf(filename, "CORE4_DIV_DTLZ%d.txt",ll);
			ResultFile = fopen(filename,"a");
			sprintf(filename, "CORE4_100_500\\DTLZ%d_%d.txt",ll,nn);
			File_A=fopen(filename,"r");		
			File_B=fopen(filename,"r");		

	
	//파일의 데이터가 몇 objective 인지 체크
	NoObj = determine_dim(File_A);
	temp = determine_dim(File_B);
	error(NoObj!=temp, "No. of objectives in data is different!!!\n");

	//초기화
	for(i=0; i<NoObj; i++){
		for(j=0; j<RESULT_NUM; j++){
			Result_A[i][j]=0;
			Result_B[i][j]=0;
		}
	}	


	//A 파일 읽기
	count_A=0;
	do{
		for(i=0; i<NoObj; i++){
			fscanf(File_A, "%lf", &Result_A[i][count_A]);
		}
		count_A++;
	}while(!feof(File_A));
	

	//B 파일 읽기
	count_B=0;
	do{
		for(i=0; i<NoObj; i++){
			fscanf(File_B, "%lf", &Result_B[i][count_B]);
		}
		count_B++;
	}while(!feof(File_B));

/*
	////////////////////////////////////////////////////////////////////////////////////////////////
	//////////		A, B의 데이터 출력		////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////

	printf("***********  DATA A  ***********\n\n");
	fprintf(ResultFile,"***********  DATA A  ***********\n\n");
	//for(i=0; i<count_A; i++){
	i=0;
		for(j=0; j<NoObj; j++){
			if(j == NoObj-1){
				printf("%f\n", Result_A[j][i]);
				fprintf(ResultFile,"%f\n", Result_A[j][i]);
			}
			else{
				printf("%f\t", Result_A[j][i]);
				fprintf(ResultFile,"%f\t", Result_A[j][i]);
			}
		}
	//}
	printf("\n\n");
	fprintf(ResultFile,"\n\n");

	printf("***********  DATA B  ***********\n\n");
	fprintf(ResultFile,"***********  DATA B  ***********\n\n");
//	for(i=0; i<count_B; i++){
	i=0;
		for(j=0; j<NoObj; j++){
			if(j == NoObj-1){
				printf("%f\n", Result_B[j][i]);
				fprintf(ResultFile,"%f\n", Result_B[j][i]);
			}
			else{
				printf("%f\t", Result_B[j][i]);
				fprintf(ResultFile,"%f\t", Result_B[j][i]);
			}
		}
//	}
*/
	////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////// F1 평균 계산   //////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////

	mean_A = 0;
	for(i=0; i<(int)(count_A/3); i++){
		mean_A += Result_A[1][i];
	}
	mean_A /= (int)(count_A/3); 


	mean_B = 0;
	for(i=0; i<(int)(count_B/3); i++){
		mean_B += Result_B[1][i];
	}
	mean_B /= (int)(count_B/3); 

	mean2_A = 0;
	for(i=0; i<(int)(count_A/3); i++){
		mean2_A += Result_A[3][i];
	}
	mean2_A /= (int)(count_A/3); 


	mean2_B = 0;
	for(i=0; i<(int)(count_B/3); i++){
		mean2_B += Result_B[3][i];
	}
	mean2_B /= (int)(count_B/3); 

	mean3_A = 0;
	for(i=0; i<(int)(count_A/3); i++){
		mean3_A += Result_A[5][i];
	}
	mean3_A /= (int)(count_A/3); 


	mean3_B = 0;
	for(i=0; i<(int)(count_B/3); i++){
		mean3_B += Result_B[5][i];
	}
	mean3_B /= (int)(count_B/3); 



	////////////////////////////////////////////////////////////////////////////////////////////////
	////////   Coverage 계산 C(A,B),   A 가 B를 얼마나 지배하는지  ///////
	////////////////////////////////////////////////////////////////////////////////////////////////

	double C_AB=0; //Coverage C(A,B)
	double C_BA=0; //Coverage C(B,A)

	int domination_count=0;
	int knap_domination_count=0;


	/// C(A,B) 계산
	for(i=0; i<count_B; i++){
		domination_count=0;
		for(j=0; j<count_A ; j++){
			knap_domination_count=0;
			for(k=0; k<NoObj ; k++){
#ifdef MAXIMIZATION
				if(Result_B[k][i] <= Result_A[k][j]){
#endif
#ifdef MINIMIZATION
				if(Result_B[k][i] >= Result_A[k][j]){
#endif
					knap_domination_count++;
				}
			}
			if(knap_domination_count == NoObj){ // dominated 되면
				domination_count++; // 현재 B를 dominate하는 A가 있다
			}
		}
		if(domination_count > 0){ // 현재 B를 dominate하는 A가 하나라도 있으면
			C_AB++; // A에 의해 dominated 되는 B의 개수
		}
	}	

	/// C(B,A) 계산
	domination_count=0;
	knap_domination_count=0;

	for(i=0; i<count_A; i++){
		domination_count=0;
		for(j=0; j<count_B ; j++){
			knap_domination_count=0;
			for(k=0; k<NoObj ; k++){
#ifdef MAXIMIZATION
				if(Result_A[k][i] <= Result_B[k][j]){
#endif
#ifdef MINIMIZATION
				if(Result_A[k][i] >= Result_B[k][j]){
#endif
					knap_domination_count++;
				}
			}
			if(knap_domination_count == NoObj){ // dominated 되면
				domination_count++; // 현재 B를 dominate하는 A가 있다
			}
		}
		if(domination_count > 0){ // 현재 B를 dominate하는 A가 하나라도 있으면
			C_BA++; // A에 의해 dominated 되는 B의 개수
		}
	}
/*
	printf("\n\nA의 갯수: %d\n", count_A);
	printf("B의 갯수: %d\n", count_B);
	printf("B가 Cover 당하는 갯수: %d \n", (int)C_AB);
	printf("A가 Cover 당하는 갯수: %d \n\n", (int)C_BA);

	fprintf(ResultFile,"\n\nA의 갯수: %d\n", count_A);
	fprintf(ResultFile,"B의 갯수: %d\n", count_B);
	fprintf(ResultFile,"B가 Cover 당하는 갯수: %d \n", (int)C_AB);
	fprintf(ResultFile,"A가 Cover 당하는 갯수: %d \n\n", (int)C_BA);

	C_AB = C_AB / count_B;	
	C_BA = C_BA / count_A;

	printf("C(A,B): %f \n", C_AB);
	printf("C(B,A): %f \n\n", C_BA);

	fprintf(ResultFile,"C(A,B): %f \n", C_AB);
	fprintf(ResultFile,"C(B,A): %f \n\n", C_BA);
*/
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////   Diversity by JHK 계산 D_JHK  /////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////

		double d[RESULT_NUM];
		double diversity_A=0;
		double diversity_B=0;
		double d_between=0;
		double d_mean=0;
		double d_sum=0;
		double d_min = INT_MAX;

		//초기화
		for(i=0; i<RESULT_NUM; i++){
			d[i]=0;
		}

		//먼저 A부터 distance_i를 계산
		for(i=0; i<count_A; i++){
			d_min = INT_MAX;
			for(j=0; j<count_A; j++){
				if(i != j){
					d_between=0;
					for(k=0; k<NoObj; k++){
						d_between += pow(Result_A[k][i] - Result_A[k][j],2);
					}
					d_between = sqrt(d_between);
					if (d_min > d_between){
						d_min = d_between;
					}
				}
			}
			d[i]=d_min;
		}

		d_mean=0;
		for(i=0; i<count_A; i++){
			d_mean += d[i];
		}
		d_mean = d_mean/count_A;

		diversity_A=0;
		d_sum=0;
		for(i=0; i<count_A; i++){
			diversity_A += pow(d[i]-d_mean,2);
			d_sum += d[i];
		}
		
		temp_A = diversity_A/count_A;  //Zharng diversity 계산을 위해
		diversity_A = diversity_A / d_sum;

		// B의 distance_i를 계산

		//초기화
		for(i=0; i<RESULT_NUM; i++){
			d[i]=0;
		}
		
		
		for(i=0; i<count_B; i++){
			d_min = INT_MAX;
			for(j=0; j<count_B; j++){
				if(i != j){
					d_between=0.0;
					for(k=0; k<NoObj; k++){
						d_between += pow(Result_B[k][i] - Result_B[k][j],2.0);
					}
					d_between = sqrt(d_between);
					if (d_min > d_between && d_between>0.0){					
						d_min = d_between;					
					}
				}
			}
			d[i]=d_min;
		}
		
		d_mean=0;
		for(i=0; i<count_B; i++){
			d_mean += d[i];
		}
		d_mean = d_mean/count_B;
		
		diversity_B=0;
		d_sum=0;
		for(i=0; i<count_B; i++){
			diversity_B += pow(d[i]-d_mean,2);
			d_sum += d[i];
		}
		temp_B = diversity_B/count_B;  //Zharng diversity 계산을 위해
		diversity_B = diversity_B / d_sum;

		////////////////////////////////////////////////////////////////////////////////////////////////
		//////////		A, B의 diversity 출력		////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		printf("Diversity A: %f\n", diversity_A);
		printf("Diversity B: %f\n\n", diversity_B);
		
		fprintf(ResultFile,"Diversity A: %f\n", diversity_A);
		fprintf(ResultFile,"Diversity B: %f\n\n", diversity_B);
	*/

		////////////////////////////////////////////////////////////////////////////////////////////////
		////////   Diversity by Zhang 계산   ////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////

		double Fmin_A[MAX_OBJ];
		double Fmax_A[MAX_OBJ];
		
		double Fmin_B[MAX_OBJ];
		double Fmax_B[MAX_OBJ];

		double parent; //분모
		double spring; //분자


		//초기화

		for(j=0; j<NoObj; j++){
			Fmin_A[j] = Result_A[j][0];
			Fmax_A[j] = Result_A[j][0];
			Fmin_B[j] = Result_B[j][0];
			Fmax_B[j] = Result_B[j][0];
		}

		
		//A,B의 fmax, fmin 찾기
		
			for(j=0; j<NoObj; j++){
				for(k=0; k<count_A; k++){
					if(Fmin_A[j] > Result_A[j][k]){
						Fmin_A[j] = Result_A[j][k];
					}
					if(Fmax_A[j] < Result_A[j][k]){
						Fmax_A[j] = Result_A[j][k];
					}
				}
			}

			for(j=0; j<NoObj; j++){
				for(k=0; k<count_B; k++){
					if(Fmin_B[j] > Result_B[j][k]){
						Fmin_B[j] = Result_B[j][k];
					}
					if(Fmax_B[j] < Result_B[j][k]){
						Fmax_B[j] = Result_B[j][k];
					}
				}
			}

		

		// A의 diversity 계산

		spring=0;
		for(j=0; j<NoObj; j++){
			spring += (Fmax_A[j] - Fmin_A[j]);
		}
		parent = sqrt(temp_A);

		if(parent == 0){
			diversity_A = 0;
		}
		else{
			diversity_A = spring/(parent);
			//diversity_A = parent;
		}

			
		// B의 diversity 계산
			
		spring=0;
		for(j=0; j<NoObj; j++){
			spring += (Fmax_B[j] - Fmin_B[j]);
		}

		parent = sqrt(temp_B);
		
		if(parent == 0){
			diversity_B = 0;
		}
		else{
			diversity_B = spring/(parent);
			//diversity_B = parent;
		}

		
		/*
		////////////////////////////////////////////////////////////////////////////////////////////////
		//////////		A, B의 diversity 출력		////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////
		
		printf("\n******** Zhang's Diversity ********\n\n");
		fprintf(ResultFile,"\n\n******** Zhang's Diversity ********\n\n");
		printf("Diversity A: %f\n",diversity_A);

		printf("");
		printf("Diversity B: %f\n", diversity_B);
	// 	for(i=0; i<run; i++){
	// 		printf("Diversity B: %f \n", diversity_B[i]);
	// 	}
		
		fprintf(ResultFile, "Diversity A: %f\n", diversity_A);

		fprintf(ResultFile,"\n");
		fprintf(ResultFile, "Diversity B: %f\n", diversity_B);

		printf("\n\n");
		fprintf(ResultFile,"\n\n");
	 */


		// 쉽게 엑셀에 붙이기 위한 출력 임시!!

		fprintf(ResultFile, "%lf\n", diversity_B);

		fclose(File_A); 
		fclose(File_B); 
		fclose(ResultFile); 
		}
		}
		

}


int  determine_dim(FILE  *fp){

	char  line[MAX_STR_LENGTH];
	int  i, no_obj;
	int  line_found, number_found;
	double  number;
	
	no_obj = 0;
	line_found = 0;
	while (fgets(line, MAX_LINE_LENGTH, fp) != NULL && !line_found)
		line_found = sscanf(line, "%lf", &number);
	if (line_found) {
		i = 0;
		do {
			no_obj++;
			while (line[i] != '\t' && line[i] != '\n' && line[i] != '\0')
				i++;
			number_found = sscanf(&(line[i]), "%lf", &number);
			while (line[i] == '\t' && line[i] != '\0')
				i++;
		} while (number_found == 1);
	}
	
	return no_obj;
}
