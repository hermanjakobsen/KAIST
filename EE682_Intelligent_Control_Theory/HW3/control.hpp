#pragma once

#include<iostream>
#include<vector>

#include"IT2FLS_HW.hpp"
#include"noise.hpp"

int const noiseType = 0; // 1-uniform, 2-gaussian
double const ub = 0.01;
double const lb = -0.01;
double const mu = 0;
double const sigma = 0.01;

class Control {

public:
	Control(int _order, double _timeStep=0.5): order(_order), timeStep(_timeStep) {

		a.resize(order);
		b.resize(order);

		e.resize(order);
		m.resize(order);
		y.resize(order);

		initialize();
	}

	~Control(){
		fuzzy_free(&fuzzySystemT2);
	}

	void setA(std::vector<double>& _a) {
		a = _a;
	}
	void setB(std::vector<double>& _b) {
		b = _b;
	}
	void setTarget(int _target) {
		target = _target;
		e[0] = target-y[0];
	}

	void initialize() {

		// T2_fuzzy
		fuzzy_init(&fuzzySystemT2);	// Fuzzy initialization

		// T1_fuzzy
		fuzzy_init(&fuzzySystemT1, 1);	// This system has FoU equal to zero
	}

	double motor() {
		double mtotal=0.0,ytotal=0.0;

		for(int i=0;i <order;i++){
			mtotal += b[i]*m[i];
			if(i > 0) ytotal += a[i]*y[i];
		}
		y[0] = mtotal-ytotal;
		
		if  (noiseType == 1){ // Uniform noise
			return y[0] + noise.uniform(lb, ub)*y[0];
		}

		else if  (noiseType == 2){ // Gaussian noise
			return y[0] + noise.gaussian(mu, sigma)*y[0];
		}

		return y[0];
	}

	void delay(double yout) {
		int i;
		for (i=y.size()-1; i>0; i--) {
			e[i] = e[i-1];
			m[i] = m[i-1];
			y[i] = y[i-1];
		}

		e[0] = target-yout;
	}

	void T2_fuzzy(double Ke, double Kce, double Ku){
		double error = Ke*e[0];
		double dError = Kce*(e[0]-e[1])/timeStep;

		m[0] = Ku*fuzzy_control(error, dError, &fuzzySystemT2);
	}

	void T1_fuzzy(double Ke, double Kce, double Ku){
		double error = Ke*e[0];
		double dError = Kce*(e[0]-e[1])/timeStep;

		m[0] = Ku*fuzzy_control(error, dError, &fuzzySystemT1);
	}

	void noise_motor_parameters(std::vector<double> _a, std::vector<double> _b){
		for (int i=0; i<a.size(); i++){
			_a[i] += noise.uniform(lb, ub)*_a[i];
			_b[i] += noise.uniform(lb, ub)*_b[i];
		}
		setA(_a);
		setB(_b);
	}

	void print_a(){
		for(int i =0; i< a.size(); i++){
			std::cout << a[i] << std::endl; 
		}
	}

private:
	int order;
	double timeStep;
	double target;
	std::vector<double> a; // Define model of motor
	std::vector<double> b; // Define model of motor
	std::vector<double> e; // Error
	std::vector<double> m; // Output from controller
	std::vector<double> y; // Output from plant

	FUZ_SYS fuzzySystemT1; // Declaring the fuzzy structure for Type-1 FLS
	FUZ_SYS fuzzySystemT2; // Declaring the fuzzy structure for Type-2 FLS

	Noise noise; // Noise generator
};
