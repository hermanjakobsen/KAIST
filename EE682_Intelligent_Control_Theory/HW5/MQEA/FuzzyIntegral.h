double ChoquetIntegral(double weight[], double estimation[], double zeta);
double fuzzy_measure(double lambda, double weight[], int mask[]);
double func_PHI( double a, double b );


double ChoquetIntegral(double weight[], double estimation[], double zeta){
	
	double lambda = (((1-zeta)*(1-zeta)) / (zeta*zeta)) - 1;
	int i, j;
	int index[OBJ];
	 

	double sorted_weight[OBJ];
	double sorted_estimation[OBJ];
	double imsi;
	int imsi2;
	double pre_estimation = 0;
	int mask[OBJ];
	double choquet_integral = 0;
	double fm;
	double temp_sum=0;


	
	for( i=0 ; i<OBJ ; i++ ){
		index[i] = i;
	}
		
	  
	for( i=0 ; i<OBJ ; i++ )
	{
		sorted_weight[i] = weight[i];
		sorted_estimation[i] = estimation[i];
	}
	
	for( i=0 ; i<OBJ ; i++ )
	{
		for( j=i+1 ; j<OBJ ; j++ )
		{
			if( sorted_estimation[i] > sorted_estimation[j] )
			{
				
				imsi = sorted_weight[i];
				sorted_weight[i] = sorted_weight[j];
				sorted_weight[j] = imsi;
				
				imsi = sorted_estimation[i];
				sorted_estimation[i] = sorted_estimation[j];
				sorted_estimation[j] = imsi;
				
				
				imsi2 = index[i];
				index[i] = index[j];
				index[j] = imsi2;
			}
		}
	}
	

	
	for( i=0 ; i<OBJ ; i++ )
		mask[i] = 1;
	

	
	for( i=0 ; i<OBJ ; i++ )
	{
		fm = fuzzy_measure(lambda, sorted_weight, mask);

		choquet_integral += ( sorted_estimation[i] - pre_estimation ) * fm;
		
		mask[i] = 0;
		
		pre_estimation = sorted_estimation[i];
	}
	
	return choquet_integral;
}


double fuzzy_measure(double lambda, double weight[], int mask[])
{
	int i=0;
	double u = 0;
	
	for(i=0 ; i<OBJ ; i++ )
	{
		if( mask[i] == 1 )
			u += weight[i];
	}
	
	return func_PHI( lambda+1, u );
}

double func_PHI( double a, double b )
{
	if(a==1){
		return b;
	}
	else{
		return (pow(a, b) - 1 ) / (a - 1);
	}
}