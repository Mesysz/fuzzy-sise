//
// Created by an21 on 20.12.18.
//

#include <iostream>
#include "Road.h"
#include "functions.h"

int main()
{
    std::cout<<"Authors: Antoni Messyasz, Barlomiej Szewczyk"<<std::endl;
    std::cout << "Hello Road!" << std::endl;
    //speedA, speedB, speedC, distanceAB, distanceAC, distanceToEnd, rightLane
    Road road(2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, true);
    double predkoscA;
    double predkoscB;
    double predkoscC;
    double distanceAB;
    double distanceAC;
    double distanceToEnd;
    bool przeciwnyPas;
    road.simulate();
}