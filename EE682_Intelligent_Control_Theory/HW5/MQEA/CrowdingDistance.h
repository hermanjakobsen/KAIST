void CrowdingDistance(int gen, int subpop_index, int FrontNum);

void CrowdingDistance(int gen, int subpop_index, int FrontNum){

	int i,j, Front, NumSol;

	struct Individual{
		int index; // Individual ��ȣ
		double f[OBJ]; // F1
	};
 
	Individual In[SUBPOPSIZE*2];
	Individual temp_In;
	individual_struct  tempIn;
	  

	//cuboid �ʱ�ȭ
	for (i=0 ; i < SUBPOPSIZE*2 ; i++ ){
		com_subpop.individual[i].cuboid=0; 
	}

	
	//�ӽ� ����ü �ʱ�ȭ
	for (i=0; i<SUBPOPSIZE*2 ; i++ ){ 
		In[i].index = 0 ;
		for (j=0; j<OBJ ; j++ ){ 
			In[i].f[j] = 0 ;
		}
	}

	for (i=0 ; i < SUBPOPSIZE*2-1 ; i++ ){
		for (j=i+1 ; j < SUBPOPSIZE*2 ; j++ ){		
			if (com_subpop.individual[i].rank > com_subpop.individual[j].rank){
				tempIn = com_subpop.individual[i];
				com_subpop.individual[i] = com_subpop.individual[j];
				com_subpop.individual[j] = tempIn;
			}
		}
	}

	int temp_NumSol = 0;
	for (Front=0 ; Front <FrontNum ; Front++){ // Rank 0���� ������ Rank����
		NumSol = 0;
		/*
		for (i=0; i<SUBPOPSIZE*2 ;i++ ){ // �ʱ�ȭ
			In[i].index = 0 ;
			for (j=0; j<OBJ ; j++ ){ // �ʱ�ȭ
				In[i].f[j] = 0 ;
			}
		}

		for (i=0 ; i<SUBPOPSIZE*2 ; i++ ){
			if (com_subpop.individual[i].rank == Front){ //���� ��ũ�� individual ������ ���� ������ 0�̴�
				In[NumSol].index = i; //Individual ��ȣ
				for (j=0; j<OBJ ; j++ ){ 
					In[NumSol].f[j] = com_subpop.individual[i].fitness[j]; // fitness ����
				}
				NumSol++; //���� ��ũ�� individual ����
			}
		}
		*/
		for (i=0 ; i<SUBPOPSIZE*2 ; i++ ){
			if (com_subpop.individual[i].rank == Front){
				NumSol++;
			}
		}


		int temp=0;
		double temp2=0;


	/*
		for (i=0; i < NumSol-1; i++){ // 0���� NumSol���� �迭�� �����Ѵ�.
			for (j=i+1; j < NumSol; j++){
				if (In[i].f[0] > In[j].f[0]){ //f1���� ��, ���� �۾�����
					temp_In = In[i];   
					In[i] = In[j];
					In[j] = temp_In;
				}
			}
		}
	*/
		for (j=0 ; j<NumSol ; j++){ // Crowding Distance ����ϱ�

			if (NumSol <= 1){ // sol�� �ϳ��� ��
				com_subpop.individual[temp_NumSol+j].cuboid = 0;
			}
			else if (j == 0){ // �� ���� sol
				com_subpop.individual[temp_NumSol+j].cuboid = INFINITE; // Boundary �� ���Ѵ�
			}
			else if (j == NumSol-1){ // �� ������ sol
				com_subpop.individual[temp_NumSol+j].cuboid = INFINITE;
			}
			else{ // sol �� �����̻��� ��
				com_subpop.individual[temp_NumSol+j].cuboid = 0;
				for (i=0; i<OBJ ; i++ ){ 
					com_subpop.individual[temp_NumSol+j].cuboid += labs((long)(com_subpop.individual[temp_NumSol+j+1].fitness[i] - com_subpop.individual[temp_NumSol+j-1].fitness[i]));
				}
			}
		}
	
		

		for (j=temp_NumSol; j < temp_NumSol + NumSol-1; j++){
			for (i=j+1; i<temp_NumSol + NumSol; i++){
				if ( com_subpop.individual[i].cuboid  > com_subpop.individual[j].cuboid ){
					tempIn = com_subpop.individual[j];
					com_subpop.individual[j] = com_subpop.individual[i];
					com_subpop.individual[i] = tempIn;
				}
			}
		}
		
		temp_NumSol += NumSol;
	}


}