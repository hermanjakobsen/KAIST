void MultipleObservation(int gen, int subpop_index); //Qbit -> Binary bit 생성

void MultipleObservation(int gen, int subpop_index){
	int i,j,k, count_dominate;
	double d1,d2;
	individual_struct temp[SUBPOPSIZE];

	if (gen == 0 || OBSERVATION == 1){	//처음에는 observation 한번만 
		for(i=0; i<SUBPOPSIZE; i++){
			for(j=0; j<BITSTRING; j++){
				if((double)rand()/RAND_MAX < square(subpop[subpop_index].individual[i].alpha[j])){
					subpop[subpop_index].individual[i].binary[j] = 0;
				}
				else{
					subpop[subpop_index].individual[i].binary[j] = 1;
				}
			} 
		} 
	}
	else{ //Multiple observation
		
		for(i=0; i<SUBPOPSIZE; i++){
			for(j=0; j<BITSTRING; j++){
				if((double)rand()/RAND_MAX < square(subpop[subpop_index].individual[i].alpha[j])){
					subpop[subpop_index].individual[i].binary[j] = 0;
				}
				else{
					subpop[subpop_index].individual[i].binary[j] = 1;
				}
			}
			
			temp[i] = subpop[subpop_index].individual[i]; //temp에 좋은해만 모아놓는다.
		}
		

		for(i=0; i<OBSERVATION-1; i++){
			for(j=0; j<SUBPOPSIZE; j++){
				for(k=0; k<BITSTRING; k++){
					if((double)rand()/RAND_MAX < square(subpop[subpop_index].individual[j].alpha[k])){
						subpop[subpop_index].individual[j].binary[k] = 0;
					}
					else{
						subpop[subpop_index].individual[j].binary[k] = 1;
					}
				}
			}
			Decode(gen, subpop_index);
			Evaluation(gen, subpop_index);

			//더 좋은해가 나오면 temp[]에 넣는다
			for(j=0 ; j<SUBPOPSIZE ; j++){ 
				count_dominate=0;
				for(k=0 ; k<OBJ ; k++){
					if ( temp[j].fitness[k] >= subpop[subpop_index].individual[j].fitness[k]){ 
						//minimization문제로 가정하고 부등호 조심, fitness function에 -를 붙이므로써 조절한다 max 문제는,,,
						count_dominate++;
					}
				}

				d1=0;
				d2=0;
				for(k=0 ; k<OBJ ; k++){
					d1 += pow(temp[j].fitness[k],2);
					d2 += pow(subpop[subpop_index].individual[j].fitness[k],2);
				}

				//->DTLZ6 에서 좋다
				//	if(sqrt(d1) > sqrt(d2)){ 

				//DTLZ1,3,4,7에서 좋다
				if(count_dominate >= NondomCondition || (count_dominate < NondomCondition && count_dominate > 0 && sqrt(d1) > sqrt(d2))){ 
					//  if(count_dominate >= NondomCondition){
					//  과반수 이상의 obj에 좋은 해가 나오면 보다 dominate하는 해가 나오면 저장하는게 성능 더 좋다
					temp[j] = subpop[subpop_index].individual[j]; //temp에 좋은해만 모아놓는다.
				}
			}
		}
		//최종적으로 좋은해만 모인 temp를 subpop으로 복사
		for(i=0 ; i<SUBPOPSIZE ; i++){ 
			subpop[subpop_index].individual[i] = temp[i]; //temp에 좋은해만 모아놓는다.
		}	
	}

}