#pragma once

#include "random"

class Noise{

private:
    std::mt19937 gen; // Random number engine

public:
    Noise(){
        std::random_device rd;
        gen.seed(rd());
    }

    double uniform(double lb, double ub){
        std::uniform_real_distribution<> dis(lb, ub);
        return dis(gen);
    }

    double gaussian(double mean, double sd){
        std::normal_distribution<> dis(mean, sd);
        return dis(gen);
    }
};