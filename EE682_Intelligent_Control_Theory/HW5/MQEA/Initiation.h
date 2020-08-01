void Initiation();

void Initiation(){
	int i,j,k;
	
	for(i=0; i<SUBPOPULATION; i++){
		for(j=0; j<SUBPOPSIZE; j++){
			for(k=0; k<BITSTRING; k++){
				subpop[i].individual[j].alpha[k] = sqrt(0.5);
				subpop[i].individual[j].beta[k] = sqrt(0.5);
				subpop[i].individual[j].binary[k]=0;
 
				subpop[i].individual[j].alpha[k] = sqrt(0.5);
				subpop[i].individual[j].beta[k] = sqrt(0.5);
				subpop[i].individual[j].binary[k]=0;
			}
			for(k=0; k<VarNum; k++){
				subpop[i].individual[j].real[k]=0;
			}
			for(k=0; k<OBJ; k++){
				subpop[i].individual[j].fitness[k]=0;
			}  
			subpop[i].individual[j].rank=0;  // 1부터 시작한다
			subpop[i].individual[j].cuboid=0;
		}
	}
}  