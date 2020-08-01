void CrowdingMigration(int gen, int subpop_index, int FrontNum);

void CrowdingMigration(int gen, int subpop_index, int FrontNum){

	int i,j, Front;
	double migration_rate;
	int mig_index, mig_index1, mig_index2;


		 
	struct Individual{
		int index; // Individual 번호
		double f[OBJ]; // F1
	};
	
	Individual In[SUBPOPSIZE*2];
	Individual temp_In;

	//cuboid 초기화
	for (i=0 ; i < SUBPOPSIZE*SUBPOPULATION*2 ; i++ ){
		com_globalpop.individual[i].cuboid=0; 
	}

	  
	int temp_num = 0;
	int l;
	individual_struct temp;

	for (Front=0 ; Front <FrontNum ; Front++){ // Rank 0부터 마지막 Rank까지
		for (j=0 ; j<com_globalpop.rankno[Front] ; j++){ // Crowding Distance 계산하기
			if (com_globalpop.rankno[Front] <= 1){ // sol이 하나일 때
				com_globalpop.individual[j+temp_num].cuboid = 0;
			}
			else if (j == 0){ // 맨 왼쪽 sol
				com_globalpop.individual[j+temp_num].cuboid = INFINITE; // Boundary 는 무한대
			}
			else if (j == com_globalpop.rankno[Front]-1){ // 맨 오른쪽 sol
				com_globalpop.individual[j+temp_num].cuboid = INFINITE;
			}
			else{ // sol 이 세개이상일 때
				com_globalpop.individual[j+temp_num].cuboid = 0;
				for (i=0; i<OBJ ; i++ ){ 
					com_globalpop.individual[j+temp_num].cuboid +=
						labs((long)(com_globalpop.individual[j+temp_num+1].fitness[i] - com_globalpop.individual[j+temp_num-1].fitness[i]));
				}
			}
		}

		for (j=0 ; j<com_globalpop.rankno[Front]-1 ; j++){
			for (i=j+1; i < com_globalpop.rankno[Front]; i++){ 
				if( com_globalpop.individual[j+temp_num].cuboid <  com_globalpop.individual[i+temp_num].cuboid )
				{
					temp = com_globalpop.individual[j+temp_num];
					com_globalpop.individual[j+temp_num] = com_globalpop.individual[i+temp_num];
					com_globalpop.individual[i+temp_num] = temp;
				}		
			}
		}
		if (Front == 0);
		else
			temp_num += com_globalpop.rankno[Front-1];
	}

	///////////   각 com_globalpop.individual에서 상위 migration_rate*NumSol개 만큼을 migration 시킴 ///////
	
	migration_rate = 0.5;

	mig_index = 0;
	mig_index1 = 0;
	mig_index2 = 0;         ///// 전체 population이 짝수 인지 홀수 인지 확인. 짝수

		
	for (Front=0 ; Front<FrontNum ; Front++){ 
		for (j=0 ; j<com_globalpop.rankno[Front] ; j++){
			if (com_globalpop.rankno[Front]%2 == 1)
				if ( j < (int)((com_globalpop.rankno[Front]+1)*migration_rate) ){
					temp_com_globalpop.individual[mig_index1] = com_globalpop.individual[mig_index];
					mig_index1++;
				}
				else{
					temp_com_globalpop.individual[(int)(2*SUBPOPULATION*SUBPOPSIZE*migration_rate)+mig_index2] 
					= com_globalpop.individual[mig_index];
					mig_index2++;
				}
			else{
				if ( j < (int)(com_globalpop.rankno[Front]*migration_rate) ){
					temp_com_globalpop.individual[mig_index1] = com_globalpop.individual[mig_index];
					mig_index1++;
				}
				else{
					temp_com_globalpop.individual[(int)(2*SUBPOPULATION*SUBPOPSIZE*migration_rate)+mig_index2] 
					= com_globalpop.individual[mig_index];
					mig_index2++;
				}
			}
			mig_index++;
		}
	}
	
	

	mig_globalpop = temp_com_globalpop;
	com_globalpop = temp_com_globalpop;
	archive = temp_com_globalpop;
	
}


