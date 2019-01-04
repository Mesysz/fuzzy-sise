//
// Created by an21 on 20.12.18.
//

#include "Road.h"
#include <unistd.h>
#include <iostream>
#include "FuzzyDriver.h"
#include "functions.h"
#include <chrono>
#include <thread>

void Road::simulate() {
    bool notEnd = true;
    int i = 0;
    fuzzyDriver.readRegs("../ustaw.xml");
    while (notEnd) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        fuzzyDriver.calculateState(speedA, speedB, speedC, distanceAB, distanceAC, distanceToEnd, rightLane);
        speedA += fuzzyDriver.getAcceleration();
        rightLane = fuzzyDriver.getLane();
        distanceAB += speedB - speedA;
        distanceAC -= speedA + speedC;
        distanceToEnd -= speedA;
        std::cout << speedA << " " << speedB << " " << speedC << " " << distanceAB << " " << distanceAC << " "
                  << distanceToEnd << " " << rightLane << std::endl;
    }
}

Road::Road(double speedA, double speedB, double speedC, double distanceAB, double distanceAC, double distanceToEnd,
           bool rightLane) : speedA(speedA), speedB(speedB), speedC(speedC), distanceAB(distanceAB),
                             distanceAC(distanceAC), distanceToEnd(distanceToEnd), rightLane(rightLane),
                             fuzzyDriver(0.0f, Lane::right) {

}

