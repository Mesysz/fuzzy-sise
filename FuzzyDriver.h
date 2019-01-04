//
// Created by an21 on 01.01.19.
//

#ifndef FUZZY_SISE_FUZZYDRIVER_H
#define FUZZY_SISE_FUZZYDRIVER_H

#include "functions.h"
#include "tinyxml2-master/tinyxml2.h"
#include <iostream>
#include <vector>

class FuzzyDriver {
private:
    double acceleration;
    bool lane;
    //TODO: Ustaw mi tu jakas zmienna do mapy ;3
    std::vector<Parameters> speedVector;
    std::vector<Parameters> distanceVector;
    std::vector<Parameters> laneVector;
    std::vector<Parameters> accelerationVector;

    void fuzzyficate(double speedA, double speedB, double speedC, double distanceAB, double distanceAC,
                     double distanceToEnd, bool rightLane);

    void decide();

    void defuzzyficate();

public:
    FuzzyDriver(double acceleration, bool lane);

    void calculateState(double speedA, double speedB, double speedC, double distanceAB, double distanceAC,
                        double distanceToEnd, bool rightLane);

    double getAcceleration() const;

    bool getLane() const;

    void readRegs(char *source);

    void readParam(std::string param, char *source, std::vector<Parameters> &vector);
};


#endif //FUZZY_SISE_FUZZYDRIVER_H
