//
// Created by an21 on 20.12.18.
//

#ifndef FUZZY_SISE_WORLD_H
#define FUZZY_SISE_WORLD_H

#include "FuzzyDriver.h"
#include <unistd.h>
#include <iostream>
#include "FuzzyDriver.h"
#include "functions.h"
#include <chrono>
#include <thread>

class Road {
    FuzzyDriver fuzzyDriver;
    double speedA;
    double speedB;
    double speedC;
    double distanceAB;
    double distanceAC;
    double distanceToEnd;
    bool rightLane;
public:
    void simulate();

    Road(double speedA, double speedB, double speedC, double distanceAB, double distanceAC, double distanceToEnd,
         bool rightLane);
};


#endif //FUZZY_SISE_WORLD_H
