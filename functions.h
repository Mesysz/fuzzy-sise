//
// Created by an21 on 01.01.19.
//

#ifndef FUZZY_SISE_FUNCTIONS_H
#define FUZZY_SISE_FUNCTIONS_H

#include <iostream>

enum Lane {
    right = false,
    left = true
};

struct Parameters {
    std::string name;
    double A;
    double M;
    double N;
    double B;
};

#endif //FUZZY_SISE_FUNCTIONS_H
