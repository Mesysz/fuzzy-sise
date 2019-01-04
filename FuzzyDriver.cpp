//
// Created by an21 on 01.01.19.
//

#include "FuzzyDriver.h"
#include "tinyxml2-master/tinyxml2.h"

void FuzzyDriver::calculateState(double speedA, double speedB, double speedC, double distanceAB, double distanceAC,
                                 double distanceToEnd, bool rightLane) {
    tinyxml2::XMLDocument dupa;
    dupa.LoadFile("../ustaw.xml");
    const char *title = dupa.FirstChildElement("STER")->FirstChildElement("PARAMETR")->FirstChildElement(
            "NAZWA")->GetText();
    std::cout << title << std::endl;
}

double FuzzyDriver::getAcceleration() const {
    return acceleration;
}

bool FuzzyDriver::getLane() const {
    return lane;
}

FuzzyDriver::FuzzyDriver(double acceleration, bool lane) : acceleration(acceleration), lane(lane) {

}

void FuzzyDriver::fuzzyficate(double speedA, double speedB, double speedC, double distanceAB, double distanceAC,
                              double distanceToEnd, bool rightLane) {
//    if(speedA<=25 && speedA>20) fuzzyASpeed = Speed::very_fast;
//    if(speedA<=20 && speedA>15) fuzzyASpeed = Speed::fast;
//    if(speedA<=15 && speedA>10) fuzzyASpeed = Speed::medium;
//    if(speedA<=10 && speedA>5)  fuzzyASpeed = Speed::slow;
//    if(speedA<=5  && speedA>=0) fuzzyASpeed = Speed::very_slow;
//    if(speedB<=25 && speedB>20) fuzzyBSpeed = Speed::very_fast;
//    if(speedB<=20 && speedB>15) fuzzyBSpeed = Speed::fast;
//    if(speedB<=15 && speedB>10) fuzzyBSpeed = Speed::medium;
//    if(speedB<=10 && speedB>5)  fuzzyBSpeed = Speed::slow;
//    if(speedB<=5  && speedB>=0) fuzzyBSpeed = Speed::very_slow;
//    if(speedC<=25 && speedC>20) fuzzyCSpeed = Speed::very_fast;
//    if(speedC<=20 && speedC>15) fuzzyCSpeed = Speed::fast;
//    if(speedC<=15 && speedC>10) fuzzyCSpeed = Speed::medium;
//    if(speedC<=10 && speedC>5)  fuzzyCSpeed = Speed::slow;
//    if(speedC<=5  && speedC>=0) fuzzyCSpeed = Speed::very_slow;
}

void FuzzyDriver::decide() {

}

void FuzzyDriver::defuzzyficate() {

}
