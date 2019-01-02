//
// Created by an21 on 01.01.19.
//

#ifndef FUZZY_SISE_FUZZYDRIVER_H
#define FUZZY_SISE_FUZZYDRIVER_H


class FuzzyDriver {
private:
    double acceleration = 0.0f;
    bool lane = false;
public:
    void calculateState(double speedA, double speedB, double speedC, double distanceAB, double distanceAC,
                        double distanceToEnd, bool rightLane);

    double getAcceleration();

    bool getLane();
};


#endif //FUZZY_SISE_FUZZYDRIVER_H
