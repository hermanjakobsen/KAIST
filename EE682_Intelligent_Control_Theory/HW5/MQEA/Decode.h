void Decode(int gen, int subpop_index);

void Decode(int gen, int subpop_index){
	int i, sum, b, k, c, d, m, x, binary_index, real_index;
	double coef[MAXVAR];

	for(i=0; i<SUBPOPSIZE; i++){
		binary_index=0;
		real_index=0;
		for(m=0; m < VarNum; m++){
			sum = 0;
			for(k = 0; k < DECODING_RESOLUTION; k++)
			{
			 	b = subpop[subpop_index].individual[i].binary[binary_index];
				d = DECODING_RESOLUTION - k - 1;
				c = (int)pow((double)2,d);
				sum =sum + c * b;
				binary_index++;
			}

			x = DECODING_RESOLUTION;
			coef[m] = pow((double)2,x) - 1;
			subpop[subpop_index].individual[i].real[real_index] =(double)(LOWER_BOUND + (sum/coef[m])*(UPPER_BOUND-LOWER_BOUND));
			real_index++;
		}
	}

}  