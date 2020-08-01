void Migration(int gen, int subpop_index);

void Migration(int gen, int subpop_index){

	int i,j,k, domination_count;
	int migration_no; // migration �� �ظ� ��߿� ������ ���̳�
	double d1,d2;
	individual_struct temp;
	
 
	//�� subpop ���� -> global pop
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
	//Sorting Pool�� Fuzzy Integral �� ���� ���� 090929
	PreferenceSorting(gen, subpop_index);

#elif defined _DOMINANCE_BASED_SORTING	
	
	//globalpop(t) + archive(t-1) �� nondominated ����
	
	for (i=0; i <2*SUBPOPULATION*SUBPOPSIZE; i++){ 
		for (j=i+1; j <2*SUBPOPULATION*SUBPOPSIZE; j++){
			//i�� j�� dominate �ϸ� �� �������̴�. many-objective�� ����ؼ�, ���ݼ�
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

			//DTLZ1, 3,4���� ���� ���� 
			if(domination_count >= NondomCondition || (domination_count < NondomCondition && domination_count > 0 && sqrt(d1) > sqrt(d2))){ 

			//nearst distance���� �� �־�, DTLZ ��ο��� ȿ�� ����
			//if(domination_count >= NondomCondition || (domination_count < NondomCondition && domination_count > 0 && SortingPool[f].distance < SortingPool[l].distance)){ 

			//DTLZ6, 7���� ����
			//	if(domination_count >= OBJ){
				temp =com_globalpop.individual[i];
				com_globalpop.individual[i] = com_globalpop.individual[j];
				com_globalpop.individual[j] = temp;
			}
		}
	}

#endif
	
	//globalpop(t) + archive(t-1) �� nondominated sol.�� archive(t)�� ����
	archive = com_globalpop; // ������ ���� SUBPOPULATION*SUBPOPSIZE ���� ����Ұ��̱� ������ �ϴ� ��ü�� ī��

	//migration_no = BestArchiveSize; // 100���߿��� �����ϰ� ���� -> �ּ�ó���ϴ°� �ξ� �� ����!!!!
	migration_no = 10;//���� 10���߿��� �����ؼ� migration�ϰڴ�. -> DTLZ1,3,7���� ����.


	
	//���� popsize��ŭ�� �� localpop[k]�� ������ �й����ش�
	for (i=0; i < SUBPOPULATION; i++){ 
		//�й�
		for(j=0; j < SUBPOPSIZE ; j++){
			//archive �� �� ������ �ظ� reference sol.�� copy
			ref_subpop[i].individual[j] = archive.individual[rand()%migration_no];
			
		}
	}
	
}