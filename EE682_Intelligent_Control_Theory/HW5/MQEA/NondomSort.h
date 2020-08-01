int NondomSort(int gen, int subpop_index); //Nondominated sorting

int NondomSort(int gen, int subpop_index){
	int i,j,k, search_count, dom_count, compare_count, current_rank;
	
	//�ʱ�ȭ
	for (i=0 ; i<SUBPOPSIZE ; i++ ){
		subpop[subpop_index].individual[i].rank = 0; // ��ũ�� 1���� �����Ѵ�. 0���� �ʱ�ȭ
		prev_subpop[subpop_index].individual[i].rank = 0; 
	}

	//�ʱ�ȭ
	for (i=0 ; i<SUBPOPSIZE ; i++ ){
		com_subpop.individual[i] = prev_subpop[subpop_index].individual[i];
		com_subpop.individual[i+SUBPOPSIZE] = subpop[subpop_index].individual[i];
	}
 
	 
	//com_subpop[]�� ������ ��ŷ�� �ű���
	current_rank=1; //��ũ�� 1���� �ű���
	search_count=0;
	while(search_count < SUBPOPSIZE*2){ // �� ������ �����
		for(i=0 ; i < SUBPOPSIZE*2 ; i++){ // �ϳ��� individual�� ����
			if (com_subpop.individual[i].rank == 0){
				dom_count = 0;	//�ڽ��� dominate �ϴ°� �� ������ ����
				for (j=0 ; j < SUBPOPSIZE*2 ; j++){ //�ٸ� ��� �ؿ� ��
					if (i != j && (com_subpop.individual[j].rank == 0 || com_subpop.individual[j].rank == current_rank) ){ // �ڽ��� �ƴϰ� rank�� �� �Ű��� �ص���� ��
						compare_count=0;
						for(k=0; k<OBJ; k++){ //minimization �����϶�
							if(com_subpop.individual[i].fitness[k] > com_subpop.individual[j].fitness[k]){
								compare_count++;
							}
						}
						if (compare_count == OBJ){ // j��° �ؿ��� dominate ���ϸ�
							dom_count++;
						}
					}
				}
				if (dom_count == 0){ //dominate �ϴ� �ذ� �ϳ��� ������ -> nondom
					com_subpop.individual[i].rank = current_rank;
					search_count++;
				}
			}
		}
		current_rank++;
	}

	return current_rank;
}