//
// Created by an21 on 01.01.19.
//

#ifndef FUZZY_SISE_FUZZYDRIVER_H
#define FUZZY_SISE_FUZZYDRIVER_H

#include "functions.h"
#include "tinyxml2-master/tinyxml2.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include "map"

class FuzzyDriver {
private:
    double acceleration;
    bool lane;

    std::vector<Parameters> speedVector;
    std::vector<Parameters> distanceVector;
    std::vector<Parameters> laneVector;
    std::vector<Parameters> accelerationVector;
    std::map<std::vector<std::pair<std::string, std::string>>, std::vector<std::pair<std::string, std::string>>> rulesMap;

    void fuzzyficate(double speedA, double speedB, double speedC, double distanceAB, double distanceAC,
                     double distanceToEnd, bool rightLane);

    void decide();

    void defuzzyficate();

    std::vector<std::pair<std::string, double>> get_membership(double value, std::vector<Parameters> params);

public:
    FuzzyDriver(double acceleration, bool lane);

    void calculateState(double speedA, double speedB, double speedC, double distanceAB, double distanceAC,
                        double distanceToEnd, bool rightLane);

    double getAcceleration() const;

    bool getLane() const;

    void readRegs(const char *source);

    void readParam(std::string param, const char *source, std::vector<Parameters> &vector);

    void readRules(std::string param, const char *source);
};


#endif //FUZZY_SISE_FUZZYDRIVER_H
