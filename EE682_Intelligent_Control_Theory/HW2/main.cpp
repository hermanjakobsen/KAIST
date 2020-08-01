#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <cmath>

#include "fuzzy.hpp"
#include "control.hpp"

int main()
{
	//////////////////////////////////////////////////////
	/* 
	Plausibility measure
	Fuzzy::xi = 0.9;
	Fuzzy::preference = {1,10,2}; // Preference1 -> flc11.txt
	Fuzzy::preference = {2,1,10}; // Preference2 -> flc12.txt
	Fuzzy::preference = {10,2,1}; // Preference3 -> flc13.txt

	Belief measure
 	Fuzzy::xi = 0.1;
	Fuzzy::preference = {1,10,2}; // Preference1 -> flc21.txt
	Fuzzy::preference = {2,1,10}; // Preference2 -> flc22.txt
	Fuzzy::preference = {10,2,1}; // Preference3 -> flc23.txt
	*/
	//////////////////////////////////////////////////////

	double timeStep=0.05;
	Control control(4, timeStep);
	std::vector<double> a = {1.0, -2.856 , 2.717, -0.861};
	std::vector<double> b = {0.0, 0.0, 2.3136e-4, 9.28e-7};

	//////////////////////////////////////////////////////
	// Use plausibility measure xi=0.9, belief measure xi=0.1, respectively. 
	Fuzzy::xi = 0.1;
//	Fuzzy::preference = {1,10,2}; // Preference1
	Fuzzy::preference = {2,1,10}; // Preference2
//	Fuzzy::preference = {10,2,1}; // Preference3
	//////////////////////////////////////////////////////
	
	control.setA(a);
	control.setB(b);

	std::fstream fileout("flc22.txt", std::ios::out);

	//////////////////////////////////////////////////////
	// Use the parameters you used in HW#1.
	double Ke = 1.6, Kce = 1.2, Ku = 3.6;
	//double Ke = 1.886226, Kce = 1.8875, Ku = 1.051013; // Parameters from source code
	/////////////////////////////////////////////////////

	double yout = 0.0;
	double target = 1.0;
	control.setTarget(target);

	int maxTimeStep = 500;
	for (int t=0; t<maxTimeStep; t++) {

		control.FLC(Ke, Kce, Ku);
		yout = control.motor();
		control.delay(yout);

		fileout << yout << std::endl;
	}

	fileout.close();
	return 0;
}
