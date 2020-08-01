void MultipleObservation(int gen, int subpop_index); //Qbit -> Binary bit ����

void MultipleObservation(int gen, int subpop_index){
	int i,j,k, count_dominate;
	double d1,d2;
	individual_struct temp[SUBPOPSIZE];

	if (gen == 0 || OBSERVATION == 1){	//ó������ observation �ѹ��� 
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
			
			temp[i] = subpop[subpop_index].individual[i]; //temp�� �����ظ� ��Ƴ��´�.
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

			//�� �����ذ� ������ temp[]�� �ִ´�
			for(j=0 ; j<SUBPOPSIZE ; j++){ 
				count_dominate=0;
				for(k=0 ; k<OBJ ; k++){
					if ( temp[j].fitness[k] >= subpop[subpop_index].individual[j].fitness[k]){ 
						//minimization������ �����ϰ� �ε�ȣ ����, fitness function�� -�� ���̹Ƿν� �����Ѵ� max ������,,,
						count_dominate++;
					}
				}

				d1=0;
				d2=0;
				for(k=0 ; k<OBJ ; k++){
					d1 += pow(temp[j].fitness[k],2);
					d2 += pow(subpop[subpop_index].individual[j].fitness[k],2);
				}

				//->DTLZ6 ���� ����
				//	if(sqrt(d1) > sqrt(d2)){ 

				//DTLZ1,3,4,7���� ����
				if(count_dominate >= NondomCondition || (count_dominate < NondomCondition && count_dominate > 0 && sqrt(d1) > sqrt(d2))){ 
					//  if(count_dominate >= NondomCondition){
					//  ���ݼ� �̻��� obj�� ���� �ذ� ������ ���� dominate�ϴ� �ذ� ������ �����ϴ°� ���� �� ����
					temp[j] = subpop[subpop_index].individual[j]; //temp�� �����ظ� ��Ƴ��´�.
				}
			}
		}
		//���������� �����ظ� ���� temp�� subpop���� ����
		for(i=0 ; i<SUBPOPSIZE ; i++){ 
			subpop[subpop_index].individual[i] = temp[i]; //temp�� �����ظ� ��Ƴ��´�.
		}	
	}

}