void Evaluation(int gen, int subpop_index);

void Evaluation(int gen, int subpop_index){
	
	int i,j,q;
	double x[MAXVAR*2];
	double f[OBJ];
	 
	/////////////////   DTLZ 문제 변수  //////////////////////////
	int n = VarNum;
	int dimension = OBJ;
	double g = 0;
	double h = 0;
	double f1 = 0;
	double alpha = 100;
	int k = n - dimension + 1;
	double t = 0;
	double *theta1 = (double *)malloc(dimension * sizeof(double));
	/////////////////////////////////////////////////////////

	for(i=0 ; i<SUBPOPSIZE;i++){
		for(j=0 ; j<VarNum ; j++){
			x[j] = subpop[subpop_index].individual[i].real[j];
		}
		  
     
#ifdef DTLZ1

	q=0;
	j=0;
	k = n - dimension + 1;

	g=0;
	for (q = n - k + 1; q <= n; q++){
		g += pow(x[q-1]-0.5,2) - cos(20 * PI * (x[q-1]-0.5));
	}

	g = 100 * (k + g);
	for (q = 1; q <= dimension; q++){
		f1 = 0.5 * (1 + g);
		for (j = dimension - q; j >= 1; j--){
			f1 *= x[j-1];
		}
		if (q > 1){
			f1 *= 1 - x[(dimension - q + 1) - 1];
		}
		f[q-1] = f1;
	}

#elif defined DTLZ2

	q=0;
	j=0;
	k = n - dimension + 1;

	g = 0;
	
	/*
	for (q = n - k + 1; q <= n; q++)
	{
		g += pow(x[q-1]-0.5,2);
	}
	*/
	for (q = n - k + 1; q <= n; q++)
	{
		g += pow(x[q-1]-0.5,2);
	}

	for (q = 1; q <= dimension; q++)
	{
		f1 = (1 + g);
		for (j = dimension - q; j >= 1; j--)
		{
			f1 *= cos(x[j-1] * PI / 2);
		}
		if (q > 1)
		{
			f1 *= sin(x[(dimension - q + 1) - 1] * PI / 2);
		}

		f[q-1] = f1;
	}

#elif defined DTLZ3
	q=0;
	j=0;
	k = n - dimension + 1;
	//f1 = 0;

	g = 0;
	for (q = n - k + 1; q <= n; q++)
	{
		g += pow(x[q-1]-0.5,2) - cos(20 * PI * (x[q-1]-0.5));
	}
	g = 100 * (k + g);

	for (q = 1; q <= dimension; q++)
	{
		f1 = (1 + g);
		for (j = dimension - q; j >= 1; j--)
		{
			f1 *= cos(x[j-1] * PI / 2);
		}
		if (q > 1)
		{
			f1 *= sin(x[(dimension - q + 1) - 1] * PI / 2);
		}

		f[q-1] = f1;
	}


#elif defined DTLZ4

	q=0;
	j=0;
	alpha =100;
	k = n - dimension + 1;	

	g = 0;

	for (q = n - k + 1; q <= n; q++){
		g += pow(x[q-1]-0.5,2);
	}

	for (q = 1; q <= dimension; q++){
		f1 = (1 + g);
		for (j = dimension - q; j >= 1; j--){
			f1 *= cos(pow(x[j-1],alpha) * PI / 2);
		}
		if (q > 1){
			f1 *= sin(pow(x[(dimension - q + 1) - 1],alpha) * PI / 2);
		}
		f[q-1] = f1;
	}

#elif defined DTLZ5

	q = 0;
	j = 0;
	k = n - dimension + 1;

	t = 0;
	g = 0;

	for (q = n - k + 1; q <= n; q++)
	{
		g += pow(x[q-1] - 0.5, 2);
	}

	t = PI / (4 * (1 + g));
	theta1[0] = x[0] * PI / 2;

	for (q = 2; q <= dimension - 1; q++)
	{
		theta1[q-1] = t * (1 + 2 * g * x[q-1]);
	}

	for (q = 1; q <= dimension; q++)
	{
		f1 = (1 + g);
		for (j = dimension - q; j >= 1; j--)
		{
			f1 *= cos(theta1[j-1]);
		}
		if (q > 1)
		{
			f1 *= sin(theta1[(dimension - q + 1) - 1]);
		}

		f[q-1] = f1;
	}



#elif defined DTLZ6

	q = 0;
	j = 0;

	k = n - dimension + 1;
	t = 0;
	g = 0;

	for (q = n - k + 1; q <= n; q++)
	{
		g += pow(x[q-1], 0.1);
	}

	t = PI / (4 * (1 + g));
	theta1[0] = x[0] * PI / 2;
	for (q = 2; q <= dimension - 1; q++)
	{
		theta1[q-1] = t * (1 + 2 * g * x[q-1]);
	}

	for (q = 1; q <= dimension; q++)
	{
		f1 = (1 + g);
		for (j = dimension - q; j >= 1; j--)
		{
			f1 *= cos(theta1[j-1]);
		}
		if (q > 1)
		{
			f1 *= sin(theta1[(dimension - q + 1) - 1]);
		}

		f[q-1] = f1;
	}

#elif defined DTLZ7

	q=0;
	j=0;
	k = n - dimension + 1; //초기화 꼭 해야함 주의!!!
	//!!!초기화 잘못하면 f가 무한대 값이 나온다, keepalive.h에서 에러 리포팅 함

	g = 0;
	h = 0; //초기화 꼭 해야함 주의!!!


	for (q = n - k + 1; q <= n; q++)
	{
		g += x[q-1];
	}
	g = 1 + 9 * g / k;

	for (q = 1; q <= dimension - 1; q++)
	{
		f[q-1] = x[q-1];
	}

	for (j = 1; j <= dimension - 1; j++)
	{
		h += x[j-1] / (1 + g) * (1 + sin(3 * PI * x[j-1]));
	}
	h = dimension - h;
	f[dimension - 1] = (1 + g) * h;

#endif

		for(k=0 ; k < OBJ ;k++){
			subpop[subpop_index].individual[i].fitness[k] = f[k];
		}
	}
}