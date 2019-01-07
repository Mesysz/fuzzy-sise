//
// Created by an21 on 01.01.19.
//

#include "FuzzyDriver.h"

void FuzzyDriver::calculateState(double speedA, double speedB, double speedC, double distanceAB, double distanceAC,
                                 double distanceToEnd, bool rightLane) {
    //fuzzyficate(speedA, speedB, speedC, distanceAB, distanceAC, distanceToEnd, rightLane);
}

double FuzzyDriver::getAcceleration() const {
    return acceleration;
}

bool FuzzyDriver::getLane() const {
    return lane;
}

FuzzyDriver::FuzzyDriver(double acceleration, bool lane) : acceleration(acceleration), lane(lane) {

}

std::vector<std::pair<std::string, double>> FuzzyDriver::get_membership(double value, std::vector<Parameters> params) {
    std::vector<std::pair<std::string, double>> membership;
    for (auto i : params) {
        double result =
                value < i.A ? 0 : value < i.M ? (value - i.A) / (i.M - i.A) : value < i.N ? 1 : value < i.B ?
                                                                                                (i.B - value) /
                                                                                                (i.B - i.N) : 0;
        membership.emplace_back(std::make_pair(i.name, result));
    }
    return membership;
}

void FuzzyDriver::fuzzyficate(double speedA, double speedB, double speedC, double distanceAB, double distanceAC,
                              double distanceToEnd, bool rightLane) {
    auto t1 = get_membership(speedA, speedVector);
    auto t2 = get_membership(speedB, speedVector);
    auto t3 = get_membership(speedC, speedVector);
    auto t4 = get_membership(distanceAB, distanceVector);
    auto t5 = get_membership(distanceAC, distanceVector);
    auto t6 = get_membership(distanceToEnd, distanceVector);

//    DEBUGOWANIE BARDZO XD
//
    std::cout << "speedA:        ";
    for (auto i : t1) {
        if (i.second > 0) std::cout << i.first << " " << i.second << " ";
    }
    std::cout << std::endl << "speedB:        ";
    for (auto i : t2) {
        if (i.second > 0) std::cout << i.first << " " << i.second << " ";
    }
    std::cout << std::endl << "speedC:        ";
    for (auto i : t3) {
        if (i.second > 0) std::cout << i.first << " " << i.second << " ";
    }
    std::cout << std::endl << "distanceAB:    ";
    for (auto i : t4) {
        if (i.second > 0) std::cout << i.first << " " << i.second << " ";
    }
    std::cout << std::endl << "distanceAC:    ";
    for (auto i : t5) {
        if (i.second > 0) std::cout << i.first << " " << i.second << " ";
    }
    std::cout << std::endl << "distanceToEnd: ";
    for (auto i : t6) {
        if (i.second > 0) std::cout << i.first << " " << i.second << " ";
    }
    std::cout << std::endl;
}

void FuzzyDriver::decide() {

}

void FuzzyDriver::defuzzyficate() {

}

void FuzzyDriver::readRegs(const char *source) {
    readParam("SPEED", source, speedVector);
    readParam("DISTANCE", source, distanceVector);
    readParam("LANE", source, laneVector);
    //readParam("ACCELERATION", source, accelerationVector);
    readRules("COS", "../reg.xml");
}

void FuzzyDriver::readParam(std::string param, const char *source, std::vector<Parameters> &vector) {
    tinyxml2::XMLDocument xmlDocument;
    xmlDocument.LoadFile(source);
    tinyxml2::XMLElement* element = xmlDocument.FirstChildElement("PARAMETERS")->FirstChildElement(param.c_str())->FirstChildElement("PARAM");

    for (tinyxml2::XMLElement *e = element; e != nullptr; e = e->NextSiblingElement()) {
        tinyxml2::XMLElement* element1 = e->FirstChildElement("NAME");
        Parameters parameters;
        parameters.name = element1->GetText();
        double temp = atof(element1->NextSiblingElement()->GetText());
        parameters.A = temp;
        element1 = element1->NextSiblingElement();
        temp = atof(element1->NextSiblingElement()->GetText());
        parameters.M = temp;
        element1 = element1->NextSiblingElement();
        temp = atof(element1->NextSiblingElement()->GetText());
        parameters.N = temp;
        element1 = element1->NextSiblingElement();
        temp = atof(element1->NextSiblingElement()->GetText());
        parameters.B = temp;
        vector.push_back(parameters);
    }
}

void FuzzyDriver::readRules(std::string param, const char *source) {
    tinyxml2::XMLDocument xmlDocument;
    xmlDocument.LoadFile(source);
    tinyxml2::XMLElement* element = xmlDocument.FirstChildElement("BASE")->FirstChildElement("RULE");
    for(tinyxml2::XMLElement *e = element; e != nullptr; e = e->NextSiblingElement()){
        std::vector<std::pair<std::string, std::string>> vectorKey;
        std::vector<std::pair<std::string, std::string>> vectorValue;
        tinyxml2::XMLElement* element1 = e->FirstChildElement("IF");
        for(tinyxml2::XMLElement* e2 = element1->FirstChildElement(); e2 != nullptr; e2 = e2->NextSiblingElement()){
            std::pair <std::string, std::string> pairKey (e2->Value(), e2->GetText());
            vectorKey.emplace_back(pairKey);
        }
        element1 = e->FirstChildElement("THEN");
        for(tinyxml2::XMLElement* e3  = element1->FirstChildElement(); e3 != nullptr; e3 = e3->NextSiblingElement()){
            std::pair <std::string, std::string> pairValue (e3->Value(), e3->GetText());
        }
        rulesMap[vectorKey] = vectorValue;
    }
}
