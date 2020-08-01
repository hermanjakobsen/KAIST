int NondomSort(int gen, int subpop_index); //Nondominated sorting

int NondomSort(int gen, int subpop_index){
	int i,j,k, search_count, dom_count, compare_count, current_rank;
	
	//초기화
	for (i=0 ; i<SUBPOPSIZE ; i++ ){
		subpop[subpop_index].individual[i].rank = 0; // 랭크는 1부터 시작한다. 0으로 초기화
		prev_subpop[subpop_index].individual[i].rank = 0; 
	}

	//초기화
	for (i=0 ; i<SUBPOPSIZE ; i++ ){
		com_subpop.individual[i] = prev_subpop[subpop_index].individual[i];
		com_subpop.individual[i+SUBPOPSIZE] = subpop[subpop_index].individual[i];
	}
 
	 
	//com_subpop[]을 가지고 랭킹을 매기자
	current_rank=1; //랭크는 1부터 매기자
	search_count=0;
	while(search_count < SUBPOPSIZE*2){ // 한 껍질씩 벗기기
		for(i=0 ; i < SUBPOPSIZE*2 ; i++){ // 하나의 individual에 대해
			if (com_subpop.individual[i].rank == 0){
				dom_count = 0;	//자신을 dominate 하는게 몇 개인지 세기
				for (j=0 ; j < SUBPOPSIZE*2 ; j++){ //다른 모든 해와 비교
					if (i != j && (com_subpop.individual[j].rank == 0 || com_subpop.individual[j].rank == current_rank) ){ // 자신이 아니고 rank가 안 매겨진 해들과만 비교
						compare_count=0;
						for(k=0; k<OBJ; k++){ //minimization 문제일때
							if(com_subpop.individual[i].fitness[k] > com_subpop.individual[j].fitness[k]){
								compare_count++;
							}
						}
						if (compare_count == OBJ){ // j번째 해에게 dominate 당하면
							dom_count++;
						}
					}
				}
				if (dom_count == 0){ //dominate 하는 해가 하나도 없으면 -> nondom
					com_subpop.individual[i].rank = current_rank;
					search_count++;
				}
			}
		}
		current_rank++;
	}

	return current_rank;
}