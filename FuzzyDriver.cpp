//
// Created by an21 on 01.01.19.
//

#include "FuzzyDriver.h"

void FuzzyDriver::calculateState(double speedA, double speedB, double speedC, double distanceAB, double distanceAC,
                                 double distanceToEnd, bool rightLane) {
    fuzzyficate(speedA, speedB, speedC, distanceAB, distanceAC, distanceToEnd, rightLane);
    check();
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
                value < i.A ? 0 : value < i.M ? (value - i.A) / (i.M - i.A) : value <= i.N ? 1 : value < i.B ?
                                                                                                (i.B - value) /
                                                                                                (i.B - i.N) : 0;
        membership.emplace_back(std::make_pair(i.name, result));
    }
    return membership;
}

void FuzzyDriver::fuzzyficate(double speedA, double speedB, double speedC, double distanceAB, double distanceAC,
                              double distanceToEnd, bool rightLane) {
    t1 = get_membership(speedA, speedVector);
    t2 = get_membership(speedB, speedVector);
    t3 = get_membership(speedC, speedVector);
    t4 = get_membership(distanceAB, distanceVector);
    t5 = get_membership(distanceAC, distanceVector);
    t6 = get_membership(distanceToEnd, distanceVector);
    if(!rightLane){
        lane2 = "right";
    }
    else{
        lane2 = "left";
    }
    tMap["SPEED_A"] = t1;
    tMap["SPEED_B"] = t2;
    tMap["SPEED_C"] = t3;
    tMap["DISTANCE_AB"] = t4;
    tMap["DISTANCE_AC"] = t5;

//    DEBUGOWANIE BARDZO XD
//
    std::cout << "speedA:        ";
    for (auto i : t1) {
        if (i.second > 0) std::cout << i.first << " " << i.second << " ";
    }
    std::cout << std::endl << "speedB:        ";
    for (auto i : t2) {
        std::cout << i.first << " " << i.second << " ";
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
    readParam("ACCELERATION", source, accelerationVector);
    readRules("COS", "../reg.xml");
    calculateCenterValues();
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
            vectorValue.emplace_back(pairValue);
        }
        rulesMap[vectorKey] = vectorValue;
    }
}

void FuzzyDriver::check() {
    std::map<std::vector<std::pair<std::string, std::string>>, std::vector<std::pair<std::string, std::string>>>::iterator it;
    it = rulesMap.begin();
    std::vector<std::pair<std::string, double>> v;
    std::string newLane = "right";
    for(; it != rulesMap.end(); ++it){
        int conditions {0};
        double coefficient {2};
        for(int i = 0; i < it->first.size(); ++i){
            std::string temp1 = it->first[i].first;
            std::string temp2 = it->first[i].second;
            for(int j = 0; j < tMap[temp1].size(); ++j){
                if(tMap[temp1].at(j).first == temp2 && tMap[temp1].at(j).second > 0){//do tego ifa wejdzie jeśli pojedynczy warunek z reguly jest spelniony
                    ++conditions;// licze warunki, zeby potem sprawdzic czy wszystkie zostaly spelnione z reguly
                    if(tMap[temp1].at(j).second < coefficient){// szukam minimum
                        coefficient = tMap[temp1].at(j).second;
                    }
                }
            }
        }
        if(it->first.back().second == lane2){//sprawdzam warunek na tor jazdy
            ++conditions;
        }
        if(coefficient != 2 && conditions == it->first.size()){//jesli wszystkie warunki spelnione to dodaje do wektora rodzaj przyspieszenia i wartosc minimum
            std::pair<std::string, double> pairNewAcc (it->second.at(0).second, coefficient);
            v.emplace_back(pairNewAcc);
            if(lane2 == "right" && it->second.back().second == "left"){
                newLane = "left";
            }
            if(lane2 == "left" && it->second.back().second == "right"){
                newLane = "right";
            }
            std::cout << "Nowa linia: " << it->second.back().second << std::endl;
        }
    }
    if(newLane == "right"){
        lane = 0;
    }
    else{
        lane = 1;
    }
    std::cout <<  std::endl;
    std::cout <<  std::endl;
    std::cout <<  std::endl;
    std::cout <<  std::endl;
    double weightSum {0};
    double sum {0};
    double newAcc {0};
    std::map<std::string, double>::iterator iteratorCenterValuesAcc = centerValuesAcc.begin();
    std::cout << "Ilosc dobrych regul to : " << v.size() << std::endl;
    for(int i = 0; i < v.size(); ++i){
        for(; iteratorCenterValuesAcc != centerValuesAcc.end(); ++iteratorCenterValuesAcc){
            if(v[i].first == iteratorCenterValuesAcc->first){//sprawdzam czy rodzaj predkosci sie zgadza
                weightSum += v[i].second;//sumuje wagi do mianownika
                sum += v[i].second * iteratorCenterValuesAcc->second; // mnoze przez srodkowa wartosc do licznika
            }
        }
    }
    newAcc = sum/weightSum;//srednia wazona
    std::cout << sum << ", " << weightSum << std::endl;
    std::cout << "Nowe przyspieszenie wynosi: " << newAcc << std::endl;//tutaj powinna byc i chyba jest nowa wartosc przyspieszenia
    // trzeba te wartosc przekazac do samochodu, nie wiem czemu ale w drugiej iteracji sie nie liczy :( blad jest chybaa w tym, ze nie zalicza reguly chuj wie czemu
    acceleration = newAcc;
}

void FuzzyDriver::calculateCenterValues() {
    for(int i = 0; i < accelerationVector.size(); ++i){
        centerValuesAcc[accelerationVector[i].name] = (accelerationVector[i].M + accelerationVector[i].N)/2;
    }
}

const std::vector<Parameters> &FuzzyDriver::getSpeedVector() const {
    return speedVector;
}


