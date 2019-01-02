//
// Created by an21 on 20.12.18.
//

#include "Road.h"
#include<unistd.h>
#include<iostream>
#include"FuzzyDriver.h"

void Road::simulate() {
    bool notEnd = true;
    int i = 0;

    while (notEnd) {
        sleep(1);
        fuzzydriver.calculateState(speedA, speedB, speedC, distanceAB, distanceAC, distanceToEnd, rightLane);
        speedA += fuzzydriver.getAcceleration();
        rightLane = fuzzydriver.getLane();
        distanceAB += speedB - speedA;
        distanceAC -= speedA + speedC;
        distanceToEnd -= speedA;
        std::cout << speedA << " " << speedB << " " << speedC << " " << distanceAB << " " << distanceAC << " "
                  << distanceToEnd << " " << rightLane << std::endl;
    }
}

Road::Road(double speedA, double speedB, double speedC, double distanceAB, double distanceAC, double distanceToEnd,
           bool rightLane) : speedA(speedA), speedB(speedB), speedC(speedC), distanceAB(distanceAB),
                             distanceAC(distanceAC), distanceToEnd(distanceToEnd), rightLane(rightLane) {

}

