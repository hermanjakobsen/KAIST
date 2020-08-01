void Migration(int gen, int subpop_index);

void Migration(int gen, int subpop_index){

	int i,j,k, domination_count;
	int migration_no; // migration 할 해를 몇개중에 선택할 것이냐
	double d1,d2;
	individual_struct temp;
	
 
	//각 subpop 취합 -> global pop
	k=0;
	for(i=0; i<SUBPOPULATION; i++){
		for(j=0; j<SUBPOPSIZE; j++){
			globalpop.individual[k] = subpop[i].individual[j];
			k++;
		}
	}
		
	//globalpop(t) + archive(t-1) -> com_globalpop
	if(gen == 0){
		prev_archive = globalpop;
	}
	else{
		for(i=0; i<SUBPOPULATION*SUBPOPSIZE; i++){
			com_globalpop.individual[i] = prev_archive.individual[i];
			com_globalpop.individual[i+SUBPOPULATION*SUBPOPSIZE] = globalpop.individual[i];
		}
	}

#ifdef _PREFERENCE_BASED_SORTING
	//Sorting Pool을 Fuzzy Integral 에 따라 소팅 090929
	PreferenceSorting(gen, subpop_index);

#elif defined _DOMINANCE_BASED_SORTING	
	
	//globalpop(t) + archive(t-1) 를 nondominated 소팅
	
	for (i=0; i <2*SUBPOPULATION*SUBPOPSIZE; i++){ 
		for (j=i+1; j <2*SUBPOPULATION*SUBPOPSIZE; j++){
			//i가 j를 dominate 하면 더 좋은것이다. many-objective를 고려해서, 과반수
			domination_count=0;
			for(k=0; k < OBJ ; k++){ 
				if(com_globalpop.individual[i].fitness[k] >= com_globalpop.individual[j].fitness[k]){
					domination_count++;
				}
			}

			d1=0;
			d2=0;
			for(k=0 ; k<OBJ ; k++){
				d1 += pow(com_globalpop.individual[i].fitness[k],2);
				d2 += pow(com_globalpop.individual[j].fitness[k],2);
			}

			//	if(sqrt(d1) > sqrt(d2)){

			//DTLZ1, 3,4에서 가장 좋다 
			if(domination_count >= NondomCondition || (domination_count < NondomCondition && domination_count > 0 && sqrt(d1) > sqrt(d2))){ 

			//nearst distance넣은 건 최악, DTLZ 모두에서 효과 없다
			//if(domination_count >= NondomCondition || (domination_count < NondomCondition && domination_count > 0 && SortingPool[f].distance < SortingPool[l].distance)){ 

			//DTLZ6, 7에서 좋다
			//	if(domination_count >= OBJ){
				temp =com_globalpop.individual[i];
				com_globalpop.individual[i] = com_globalpop.individual[j];
				com_globalpop.individual[j] = temp;
			}
		}
	}

#endif
	
	//globalpop(t) + archive(t-1) 중 nondominated sol.만 archive(t)에 저장
	archive = com_globalpop; // 어차피 상위 SUBPOPULATION*SUBPOPSIZE 개만 사용할것이기 때문에 일단 전체를 카피

	//migration_no = BestArchiveSize; // 100개중에서 랜덤하게 선정 -> 주석처리하는게 훨씬 더 좋다!!!!
	migration_no = 10;//상위 10개중에서 선택해서 migration하겠다. -> DTLZ1,3,7에서 좋다.


	
	//상위 popsize만큼을 각 localpop[k]에 무조건 분배해준다
	for (i=0; i < SUBPOPULATION; i++){ 
		//분배
		for(j=0; j < SUBPOPSIZE ; j++){
			//archive 해 중 랜덤한 해를 reference sol.로 copy
			ref_subpop[i].individual[j] = archive.individual[rand()%migration_no];
			
		}
	}
	
}