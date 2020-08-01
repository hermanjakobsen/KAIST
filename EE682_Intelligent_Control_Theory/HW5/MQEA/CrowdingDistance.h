void CrowdingDistance(int gen, int subpop_index, int FrontNum);

void CrowdingDistance(int gen, int subpop_index, int FrontNum){

	int i,j, Front, NumSol;

	struct Individual{
		int index; // Individual 번호
		double f[OBJ]; // F1
	};
 
	Individual In[SUBPOPSIZE*2];
	Individual temp_In;
	individual_struct  tempIn;
	  

	//cuboid 초기화
	for (i=0 ; i < SUBPOPSIZE*2 ; i++ ){
		com_subpop.individual[i].cuboid=0; 
	}

	
	//임시 구조체 초기화
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
	for (Front=0 ; Front <FrontNum ; Front++){ // Rank 0부터 마지막 Rank까지
		NumSol = 0;
		/*
		for (i=0; i<SUBPOPSIZE*2 ;i++ ){ // 초기화
			In[i].index = 0 ;
			for (j=0; j<OBJ ; j++ ){ // 초기화
				In[i].f[j] = 0 ;
			}
		}

		for (i=0 ; i<SUBPOPSIZE*2 ; i++ ){
			if (com_subpop.individual[i].rank == Front){ //현재 랭크의 individual 정보만 들어가고 나머진 0이다
				In[NumSol].index = i; //Individual 번호
				for (j=0; j<OBJ ; j++ ){ 
					In[NumSol].f[j] = com_subpop.individual[i].fitness[j]; // fitness 복사
				}
				NumSol++; //현재 랭크의 individual 갯수
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
		for (i=0; i < NumSol-1; i++){ // 0부터 NumSol까지 배열만 정렬한다.
			for (j=i+1; j < NumSol; j++){
				if (In[i].f[0] > In[j].f[0]){ //f1으로 비교, 점점 작아지게
					temp_In = In[i];   
					In[i] = In[j];
					In[j] = temp_In;
				}
			}
		}
	*/
		for (j=0 ; j<NumSol ; j++){ // Crowding Distance 계산하기

			if (NumSol <= 1){ // sol이 하나일 때
				com_subpop.individual[temp_NumSol+j].cuboid = 0;
			}
			else if (j == 0){ // 맨 왼쪽 sol
				com_subpop.individual[temp_NumSol+j].cuboid = INFINITE; // Boundary 는 무한대
			}
			else if (j == NumSol-1){ // 맨 오른쪽 sol
				com_subpop.individual[temp_NumSol+j].cuboid = INFINITE;
			}
			else{ // sol 이 세개이상일 때
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