void UpdateQbit(int gen, int subpop_index);

void UpdateQbit(int gen, int subpop_index){
	
	int i,j;

	double alpha, beta;

	for (i=0 ; i < SUBPOPSIZE; i++){// i : best groupÀÇ individual
		for (j=0; j <BITSTRING; j++){

			alpha = subpop[subpop_index].individual[i].alpha[j];
			beta = subpop[subpop_index].individual[i].beta[j];
		 	 
		//	if (ref_subpop[subpop_index].individual[i].alpha[j]==1 && subpop[subpop_index].individual[i].alpha[j] ==0){
			if (ref_subpop[subpop_index].individual[i].binary[j]==1 && subpop[subpop_index].individual[i].binary[j] ==0){
				if ((alpha >=0 && beta >=0) || (alpha <= 0 && beta <= 0)){
					subpop[subpop_index].individual[i].alpha[j] = cos(THETA)*alpha-sin(THETA)*beta;
					subpop[subpop_index].individual[i].beta[j] = sin(THETA)*alpha+cos(THETA)*beta;
				}
				else{
					subpop[subpop_index].individual[i].alpha[j]= cos(-THETA)*alpha-sin(-THETA)*beta;
					subpop[subpop_index].individual[i].beta[j] = sin(-THETA)*alpha+cos(-THETA)*beta;
				}
			}

		//	else if(ref_subpop[subpop_index].individual[i].alpha[j]==0 && subpop[subpop_index].individual[i].alpha[j] ==1){
			else if(ref_subpop[subpop_index].individual[i].binary[j]==0 && subpop[subpop_index].individual[i].binary[j] ==1){
				if ((alpha >= 0 && beta >= 0) || (alpha <= 0 && beta <= 0)){
					subpop[subpop_index].individual[i].alpha[j] = cos(-THETA)*alpha-sin(-THETA)*beta;
					subpop[subpop_index].individual[i].beta[j]  = sin(-THETA)*alpha+cos(-THETA)*beta;
				}
				else{
					subpop[subpop_index].individual[i].alpha[j] = cos(THETA)*alpha-sin(THETA)*beta;
					subpop[subpop_index].individual[i].beta[j]  = sin(THETA)*alpha+cos(THETA)*beta;

				}
			}
		}
	}
}