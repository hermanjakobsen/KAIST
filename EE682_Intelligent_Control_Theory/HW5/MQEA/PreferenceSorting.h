void PreferenceSorting(int gen, int subpop_index);

void PreferenceSorting(int gen, int subpop_index){

	double temp_sum=0;
	double temp_sum2=0;
	double temp_max=0;
	double temp_min=0;
	double group_index;

	int f,l;
	int mmax, rnk;
	individual_struct temp;

	//MQEA.cpp에 아래 중 골라서 넣기
	 

	/* 
	double pairwise_matrix[OBJ][OBJ]={ //3개중 1개 강조
 	 	{1, 5, 10},
 	 	{(double)0.2, 1, 2},
 	 	{(double)0.1, (double)0.5, 1},
 	};
	*/

	// double pairwise_matrix[OBJ][OBJ]={ //1만 강조
	// 	{1, 10, 10, 10, 10, 10, 10, 10, 10, 10},
	// 	{(double)0.1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	// };

	// double pairwise_matrix[OBJ][OBJ]={ //1~3만 강조
	// {1, 1, 1, 100, 100, 100, 100, 100, 100, 100},
	// {1, 1, 1, 100, 100, 100, 100, 100, 100, 100},
	// {1, 1, 1, 100, 100, 100, 100, 100, 100, 100},
	// {(double)0.01, (double)0.01, (double)0.01, 1, 1, 1, 1, 1, 1, 1},
	// {(double)0.01, (double)0.01, (double)0.01, 1, 1, 1, 1, 1, 1, 1},
	// {(double)0.01, (double)0.01, (double)0.01, 1, 1, 1, 1, 1, 1, 1},
	// {(double)0.01, (double)0.01, (double)0.01, 1, 1, 1, 1, 1, 1, 1},
	// {(double)0.01, (double)0.01, (double)0.01, 1, 1, 1, 1, 1, 1, 1},
	// {(double)0.01, (double)0.01, (double)0.01, 1, 1, 1, 1, 1, 1, 1},
	// {(double)0.01, (double)0.01, (double)0.01, 1, 1, 1, 1, 1, 1, 1},
	// };


// 	double pairwise_matrix[OBJ][OBJ]={ //1~3만 강조
// 	{1, 1, 1, 10, 10, 10, 10, 10, 10, 10},
// 	{1, 1, 1, 10, 10, 10, 10, 10, 10, 10},
// 	{1, 1, 1, 10, 10, 10, 10, 10, 10, 10},
// 	{(double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1, 1, 1},
// 	{(double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1, 1, 1},
// 	{(double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1, 1, 1},
// 	{(double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1, 1, 1},
// 	{(double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1, 1, 1},
// 	{(double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1, 1, 1},
// 	{(double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1, 1, 1},
// 	};


// 	double pairwise_matrix[OBJ][OBJ]={ //7 개 중 1~2만 강조 //TEC가장 최근 revision 제출했을시, 2010.7.
// 		{1, 1, 10, 10, 10, 10, 10},
// 		{1, 1, 10, 10, 10, 10, 10},
// 		{(double)0.1, (double)0.1, 1, 1, 1, 1, 1},
// 		{(double)0.1, (double)0.1, 1, 1, 1, 1, 1},
// 		{(double)0.1, (double)0.1, 1, 1, 1, 1, 1},
// 		{(double)0.1, (double)0.1, 1, 1, 1, 1, 1},
// 		{(double)0.1, (double)0.1, 1, 1, 1, 1, 1},
// 	};


	// double pairwise_matrix[OBJ][OBJ]={ //weight 동일
	// 	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	// 	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	// 	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	// 	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	// 	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	// 	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	// 	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	// 	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	// 	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	// 	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	// };


	// double pairwise_matrix[OBJ][OBJ]={ //1~5만 강조
	// 	{1, 1, 1, 1, 1, 10, 10, 10, 10, 10},
	// 	{1, 1, 1, 1, 1, 10, 10, 10, 10, 10},
	// 	{1, 1, 1, 1, 1, 10, 10, 10, 10, 10},
	// 	{1, 1, 1, 1, 1, 10, 10, 10, 10, 10},
	// 	{1, 1, 1, 1, 1, 10, 10, 10, 10, 10},
	// 	{(double)0.1, (double)0.1, (double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, (double)0.1, (double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, (double)0.1, (double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, (double)0.1, (double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, (double)0.1, (double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1},
	// };


	// double pairwise_matrix[OBJ][OBJ]={ //5개중 2개 강조
	// 	{1, 1, 10, 10, 10},
	// 	{1, 1, 10, 10, 10},
	// 	{(double)0.1, (double)0.1, 1, 1, 1},
	// 	{(double)0.1, (double)0.1, 1, 1, 1},
	// 	{(double)0.1, (double)0.1, 1, 1, 1},
	// };


	// double pairwise_matrix[OBJ][OBJ]={ //20개중 5개 강조
	// 	{1, 1, 1, 1, 1, 10, 10, 10, 10, 10},
	// 	{1, 1, 1, 1, 1, 10, 10, 10, 10, 10},
	// 	{1, 1, 1, 1, 1, 10, 10, 10, 10, 10},
	// 	{1, 1, 1, 1, 1, 10, 10, 10, 10, 10},
	// 	{1, 1, 1, 1, 1, 10, 10, 10, 10, 10},
	// 	{(double)0.1, (double)0.1, (double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, (double)0.1, (double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, (double)0.1, (double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, (double)0.1, (double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, (double)0.1, (double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, (double)0.1, (double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, (double)0.1, (double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, (double)0.1, (double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, (double)0.1, (double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, (double)0.1, (double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, (double)0.1, (double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, (double)0.1, (double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, (double)0.1, (double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, (double)0.1, (double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1},
	// 	{(double)0.1, (double)0.1, (double)0.1, (double)0.1, (double)0.1, 1, 1, 1, 1, 1},
	// };


	//weight 초기화
	for(f=0 ; f<OBJ ; f++){
		weight[f]=0;
	}

	temp_sum=0;
	for(f=0 ; f<OBJ ; f++){
		for(l=0 ; l<OBJ ; l++){
			temp_sum += pairwise_matrix[f][l];
		}
	}

	//weight 계산 
	for(f=0 ; f<OBJ ; f++){
		temp_sum2=0;
		for(l=0 ; l<OBJ ; l++){
			temp_sum2 += pairwise_matrix[f][l];
		}
		weight[f]=temp_sum2/temp_sum;
	}


	//Fitness value를 normalization, 최대값 = 0, 최소값 = 1


	for(f=0 ; f<OBJ ; f++){
		temp_max = com_globalpop.individual[0].fitness[f];
		temp_min = com_globalpop.individual[0].fitness[f];

		for(l=0 ; l<2*SUBPOPULATION*SUBPOPSIZE ; l++){ //각 obj 별 최소, 최대값 찾기
			if(temp_min > com_globalpop.individual[l].fitness[f] ){
				temp_min = com_globalpop.individual[l].fitness[f];
			}
			if(temp_max < com_globalpop.individual[l].fitness[f] ){
				temp_max = com_globalpop.individual[l].fitness[f];
			}
		}
		//maximization 문제이면 (com_globalpop.individual[l].fitness[f] temp-min)/(temp_max-temp_min) 로만 수정하면 된다
		for(l=0 ; l<2*SUBPOPULATION*SUBPOPSIZE ; l++){ //가장 큰 걸 찾았으면 normalization
			com_globalpop.individual[l].partial_eval[f] = (temp_max - com_globalpop.individual[l].fitness[f] )/(temp_max-temp_min) ;
		}
	}

	//SoringPool의 fuzzy integral 계산
	for (f=0 ; f < 2*SUBPOPULATION*SUBPOPSIZE ; f++){ 
		com_globalpop.individual[f].fuzzy_integral=ChoquetIntegral(weight, com_globalpop.individual[f].partial_eval, xi);
	}



	//fuzzy integral 큰 순서대로 소팅하자

	for (f=0; f < 2*SUBPOPULATION*SUBPOPSIZE; f++){ 
		for (l=f+1; l <  2*SUBPOPULATION*SUBPOPSIZE; l++){
			if( com_globalpop.individual[f].fuzzy_integral <  com_globalpop.individual[l].fuzzy_integral ){
				temp = com_globalpop.individual[f];
				com_globalpop.individual[f] = com_globalpop.individual[l];
				com_globalpop.individual[l] = temp;
			}
		}
	}	



	for (f=0 ; f < 2*SUBPOPULATION*SUBPOPSIZE ; f++){
		com_globalpop.rankno[f] = 0;
	}
//	mmax = 0;

	int numgroup = 10;
	for (f=0 ; f < 2*SUBPOPULATION*SUBPOPSIZE ; f++){
		group_index = numgroup - com_globalpop.individual[f].fuzzy_integral*numgroup;
//		com_globalpop.individual[f].rank = com_globalpop.individual[f].rank + (int)group_index;
		com_globalpop.individual[f].rank = (int)group_index;
//		if (com_globalpop.individual[f].rank>mmax)
//			mmax = com_globalpop.individual[f].rank;
		com_globalpop.rankno[com_globalpop.individual[f].rank]++;
	}
//	rnk = mmax+1;
//	globalpop.maxrank = rnk;


	CrowdingMigration(gen,subpop_index,numgroup);
	
	//
	//globalpop = com_globalpop;

}