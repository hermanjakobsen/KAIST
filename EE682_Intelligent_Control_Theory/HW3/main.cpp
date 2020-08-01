#include<iostream>
#include<fstream>

#include"control.hpp"
#include"noise.hpp"

using namespace std;

int main(){
    // Simulation parameters
    double const timeStep = 0.05;
    int const maxTimeStep = 500;
    double const numOfControllers = 2;
    double const reference = 1;

    // Model parameters
    int const modelOrder = 4;
    std::vector<double> a = {1.0, -2.856 , 2.717, -0.861};
	std::vector<double> b = {0.0, 0.0, 2.3136e-4, 9.28e-7};

	// Initialize controllers
	std::vector<Control> controllers(numOfControllers, Control(modelOrder, timeStep));

	for(int i = 0; i < numOfControllers; i++){
		controllers[i].setA(a);
		controllers[i].setB(b);
		controllers[i].setTarget(reference);
	}

    // Control parameters
    double Ke = 1, Kce = 1, Ku = 1;

    // Output file for simulation data
    std::fstream simOut("output.txt", std::ios::out);
    simOut << "T1;T2" << std::endl;

    // Simulation
    for (int t=0; t<maxTimeStep; t++) {
		

		controllers[0].T1_fuzzy(Ke, Kce, Ku);
		controllers[1].T2_fuzzy(Ke, Kce, Ku);
		
		for(int i=0; i<numOfControllers; i++){
			// Add noise to motor parameters
			//controllers[i].noise_motor_parameters(a, b);

			double yout = controllers[i].motor();
			controllers[i].delay(yout);
			simOut << yout;

			if (i < numOfControllers - 1){
				simOut << ";";
			}
		}

		simOut << std::endl;

	}


    simOut.close();

    return 0;
}