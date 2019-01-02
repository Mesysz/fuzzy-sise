//
// Created by an21 on 20.12.18.
//

#ifndef FUZZY_SISE_WORLD_H
#define FUZZY_SISE_WORLD_H

#include "FuzzyDriver.h"

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
