#pragma warning(disable:4996)
void PrintResult(int runnum);

void PrintResult(int runnum){
	int i,j,k,l,m,f,q, domination_count, OBJ_domination_count, temp, temp1, nondom_no;
	FILE *globalpop_fitness;
	FILE *nondom_fitness;
	FILE *nondom_real;
	FILE *Variable;
	FILE *prefer_selected;
	char file_name[256];
	char file_name1[256];
	char file_name2[256];
	individual_struct ind_temp;
 
	double weight[OBJ];		// fuzzy measure를 계산하기 위한 weight
	double temp_sum=0; 
	double temp_sum2=0;
	double temp_max=0;
	double temp_min=0;
	/*
	sprintf(file_name, "DTLZ/%d/1_MQEA_globalpop_fitness.txt",runnum);
	globalpop_fitness = fopen(file_name,"w");

	sprintf(file_name1, "DTLZ/%d/2_MQEA_nondom_fitness.txt",runnum);
	nondom_fitness = fopen(file_name1,"w");

	nondom_real = fopen("2_MQEA_nondom_real.txt","w");
	Variable = fopen("3_MQEA_Variable.txt","w");

	sprintf(file_name2, "DTLZ/%d/4_Preferred_Solution.txt",runnum);
	prefer_selected = fopen(file_name2,"w");
	*/

	globalpop_fitness = fopen("1_MQEA_globalpop_fitness.txt","w");
	nondom_fitness = fopen("2_MQEA_nondom_fitness.txt","w");
	//Variable = fopen("3_MQEA_nondom_real.txt","w");
	prefer_selected = fopen("4_Preferred_Solution.txt","w");

	//globalpop의 해 찍기
	
	//archive 중 non-dominated solution만 저장
	nondom_no=0;
	temp=0;
	for(i=0; i<ITERATION ; i++){
		for(j=0; j<SUBPOPSIZE*SUBPOPULATION ; j++){
			domination_count=0;
			for(k=0; k<ITERATION ; k++){
				for(l=0; l<SUBPOPSIZE*SUBPOPULATION ; l++){
					OBJ_domination_count=0;
					for(m=0; m<OBJ ; m++){
						if(iteration_archive[i].individual[j].fitness[m] > iteration_archive[k].individual[l].fitness[m] ){
							OBJ_domination_count++;
						}
					}
					if(OBJ_domination_count == OBJ){ // dominated 되면
						domination_count++; // 현재 해를 dominate하는 해가 하나라도 있다
					}
				}
			}
			if(domination_count == 0){ //nondominated sol 이면
				fi_selection_pool.individual[nondom_no] = iteration_archive[i].individual[nondom_no];
				nondom_no++;
				for(k = 0;k < OBJ;k++){	
					if(temp==0){
						fprintf(nondom_fitness,"%f",iteration_archive[i].individual[j].fitness[k]);
						temp++;
					}
					else if(k==0){
						fprintf(nondom_fitness,"\n%f",iteration_archive[i].individual[j].fitness[k]);
					}
					else{
						fprintf(nondom_fitness,"\t%f",iteration_archive[i].individual[j].fitness[k]);
					}
				}
			}
		}
	}

	temp=0;
	for(i=0; i<ITERATION ; i++){
		for(j=0; j<SUBPOPSIZE*SUBPOPULATION ; j++){
			//global pop 찍기
			for(k = 0;k < OBJ;k++){	
				if(temp==0){
					fprintf(globalpop_fitness,"%f",iteration_globalpop[i].individual[j].fitness[k]);
					temp++;
				}
				else if(k==0){
					fprintf(globalpop_fitness,"\n%f",iteration_globalpop[i].individual[j].fitness[k]);
				}
				else{
					fprintf(globalpop_fitness,"\t%f",iteration_globalpop[i].individual[j].fitness[k]);
				}
			}
		}
	}

	//다 돌리고 마지막으로 archive의 nondom 중에서 F.I.로 해 선택하기
	
	//Archive의 해를 F.I 로 소팅하기
	//Fitness value를 normalization, 최대값 = 0, 최소값 = 1

	for(f=0 ; f<OBJ ; f++){
		temp_min=fi_selection_pool.individual[0].fitness[f];
		temp_max=fi_selection_pool.individual[0].fitness[f];

		for(l=0 ; l<nondom_no; l++){ //각 obj 별 최대값 찾기
			if(temp_min > fi_selection_pool.individual[l].fitness[f] ){
				temp_min = fi_selection_pool.individual[l].fitness[f];
			}
			if(temp_max < fi_selection_pool.individual[l].fitness[f] ){
				temp_max = fi_selection_pool.individual[l].fitness[f];
			}
		}

		//maximization 문제이면 (iteration_archive[k].individual[l].fitness[f] temp-min)/(temp_max-temp_min) 로만 수정하면 된다
		for(l=0 ; l<nondom_no ; l++){ //가장 큰 걸 찾았으면 normalization
			 fi_selection_pool.individual[l].partial_eval[f] = (temp_max - iteration_archive[k].individual[l].fitness[f] )/(temp_max-temp_min) ;
		}
		
	}


	//SoringPool의 fuzzy integral 계산
	for (f=0 ; f <nondom_no; f++){ 
		fi_selection_pool.individual[f].fuzzy_integral=ChoquetIntegral(weight, fi_selection_pool.individual[f].partial_eval, xi);
	}

	//fuzzy integral 큰 순서대로 소팅하자
		for (f=0; f <nondom_no; f++){ 
			for (l=f+1; l < nondom_no; l++){
				if( fi_selection_pool.individual[f].fuzzy_integral <  fi_selection_pool.individual[l].fuzzy_integral ){
					ind_temp = fi_selection_pool.individual[f];
					fi_selection_pool.individual[f] = fi_selection_pool.individual[l];
					fi_selection_pool.individual[l] = ind_temp;
				}
			}
		}	   


	//F.I값이 가장 큰 해를 파일에 출력
	temp=0;
	for(q = 0;q < OBJ;q++){	
		if(temp==0){
			fprintf(prefer_selected,"%f",fi_selection_pool.individual[0].fitness[q]);
			temp++;
		}
		else if(q==0){
			fprintf(prefer_selected,"\n%f",fi_selection_pool.individual[0].fitness[q]);
		}
		else{
			fprintf(prefer_selected,"\t%f",fi_selection_pool.individual[0].fitness[q]);
		}
	}

	fprintf(prefer_selected,"\n\n");

	//맨위의 Chrom_result 출력하기
	for(k=0; k<VarNum ; k++){
		fprintf(prefer_selected,"%f\t",fi_selection_pool.individual[0].real[k]);
	}




	fclose(globalpop_fitness);
	fclose(nondom_fitness);
	//fclose(nondom_real);
	//fclose(Variable);
	fclose(prefer_selected);
}