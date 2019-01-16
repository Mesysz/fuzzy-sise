//
// Created by an21 on 01.01.19.
//

#include "FuzzyDriver.h"

void FuzzyDriver::calculateState(double speedA, double speedB, double speedC, double distanceAB, double distanceAC,
                                 double distanceToEnd, Lane rightLane) {
    fuzzyficate(speedA, speedB, speedC, distanceAB, distanceAC, distanceToEnd, rightLane);
    check();
}

double FuzzyDriver::getAcceleration() const {
    return acceleration;
}

Lane FuzzyDriver::getLane() const {
    return lane;
}

FuzzyDriver::FuzzyDriver(double acceleration, Lane lane) : acceleration(acceleration), lane(lane) {

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
                              double distanceToEnd, Lane currline) {
    t1 = get_membership(speedA, speedVector);
    t2 = get_membership(speedB, speedVector);
    t3 = get_membership(speedC, speedVector);
    t4 = get_membership(distanceAB, distanceVector);
    t5 = get_membership(distanceAC, distanceVector);
    t6 = get_membership(distanceToEnd, distanceVector);
    tMap["SPEED_A"] = t1;
    tMap["SPEED_B"] = t2;
    tMap["SPEED_C"] = t3;
    tMap["DISTANCE_AB"] = t4;
    tMap["DISTANCE_AC"] = t5;
    tMap["DISTANCE_END"] = t6;

//    DEBUGOWANIE BARDZO XD
//
#ifdef LOG_TO_CONSOLE
    std::cout << "##################\nCurrent line:  " << ((currline == left) ? "left" : "right")
              << "\nspeedA:        ";
    for (auto i : t1) {
        if (i.second > 0) std::cout << i.first << " " << i.second << " ";
    }
    std::cout << "\nspeedB:        ";
    for (auto i : t2) {
        if (i.second > 0) std::cout << i.first << " " << i.second << " ";
    }
    std::cout << "\nspeedC:        ";
    for (auto i : t3) {
        if (i.second > 0) std::cout << i.first << " " << i.second << " ";
    }
    std::cout << "\ndistanceAB:    ";
    for (auto i : t4) {
        if (i.second > 0) std::cout << i.first << " " << i.second << " ";
    }
    std::cout << "\ndistanceAC:    ";
    for (auto i : t5) {
        if (i.second > 0) std::cout << i.first << " " << i.second << " ";
    }
    std::cout << "\ndistanceToEnd: ";
    for (auto i : t6) {
        if (i.second > 0) std::cout << i.first << " " << i.second << " ";
    }
    std::cout << "\n";
#endif
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
    readRules("../reg.xml");
    calculateCenterValues();
}

void FuzzyDriver::readParam(std::string param, const char *source, std::vector<Parameters> &vector) {
    tinyxml2::XMLDocument xmlDocument;
    xmlDocument.LoadFile(source);
    tinyxml2::XMLElement *element = xmlDocument.FirstChildElement("PARAMETERS")->FirstChildElement(
            param.c_str())->FirstChildElement();
    std::string temp2(element->Value());

    for (tinyxml2::XMLElement *e = element; e != nullptr; e = e->NextSiblingElement()) {
        tinyxml2::XMLElement* element1 = e->FirstChildElement("NAME");
        Parameters parameters;
        parameters.name = element1->GetText();
        double temp = atof(element1->NextSiblingElement()->GetText());
        parameters.A = temp;
        element1 = element1->NextSiblingElement();
        temp = atof(element1->NextSiblingElement()->GetText());
        parameters.M = temp;
        if (temp2 == "TRIANGLE") {
            parameters.N = parameters.M;
        } else {
            element1 = element1->NextSiblingElement();
            temp = atof(element1->NextSiblingElement()->GetText());
            parameters.N = temp;
        }
        element1 = element1->NextSiblingElement();
        temp = atof(element1->NextSiblingElement()->GetText());
        parameters.B = temp;
        vector.push_back(parameters);
    }
}

void FuzzyDriver::readRules(const char *source) {
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
    std::vector<std::pair<std::string, double>> v;
    std::string currLine = lane == right ? "right" : "left";

    for (const auto &it : rulesMap) {
        if (it.first.back().second != currLine)
            continue;
        int conditions{1};
        double coefficient {2};
        for (const auto &i : it.first) {
            std::string temp1 = i.first;
            std::string temp2 = i.second;
            for (const auto &j : tMap[temp1]) {
                if (j.first == temp2 &&
                    j.second > 0) {//do tego ifa wejdzie jeśli pojedynczy warunek z reguly jest spelniony
                    ++conditions;// licze warunki, zeby potem sprawdzic czy wszystkie zostaly spelnione z reguly
                    if (j.second < coefficient) {// szukam minimum
                        coefficient = j.second;
                    }
                }
            }
        }
        if (coefficient != 2 && conditions ==
                                it.first.size()) {//jesli wszystkie warunki spelnione to dodaje do wektora rodzaj przyspieszenia i wartosc minimum
            std::pair<std::string, double> pairNewAcc(it.second.at(0).second, coefficient);
            v.emplace_back(pairNewAcc);
            if (it.second.back().second == "left")
                lane = left;
            if (it.second.back().second == "right")
                lane = right;
#ifdef LOG_TO_CONSOLE
            std::cout << "Nowa linia: " << it->second.back().second << std::endl;
#endif
        }
    }
    double weightSum{1};
    double sum {0};
    double newAcc {0};
#ifdef LOG_TO_CONSOLE
    std::cout << "Ilosc dobrych regul to : " << v.size() << std::endl;
#endif
    for(int i = 0; i < v.size(); ++i){
        for (auto iteratorCenterValuesAcc : centerValuesAcc) {
            if (v[i].first == iteratorCenterValuesAcc.first) {//sprawdzam czy rodzaj predkosci sie zgadza
                weightSum += v[i].second;//sumuje wagi do mianownika
                sum += v[i].second * iteratorCenterValuesAcc.second; // mnoze przez srodkowa wartosc do licznika
            }
        }
    }
    newAcc = sum/weightSum;//srednia wazona
#ifdef LOG_TO_CONSOLE
    std::cout << sum << ", " << weightSum << std::endl;
    std::cout << "Nowe przyspieszenie wynosi: " << newAcc << std::endl;//tutaj powinna byc i chyba jest nowa wartosc przyspieszenia
    // trzeba te wartosc przekazac do samochodu, nie wiem czemu ale w drugiej iteracji sie nie liczy :( blad jest chybaa w tym, ze nie zalicza reguly chuj wie czemu
#endif
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


