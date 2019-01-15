//
// Created by an21 on 20.12.18.
//

#include <iostream>
#include "Road.h"
#include "functions.h"
#include <random>
#include <chrono>

int main()
{
    std::cout<<"Authors: Antoni Messyasz, Barlomiej Szewczyk"<<std::endl;
    std::cout << "Hello Road!" << std::endl;
    std::uniform_real_distribution<double> random_speed(6.0, 25.0);
    std::uniform_real_distribution<double> random_distance(2.0, 2000.0);
    std::default_random_engine re(std::chrono::system_clock::now().time_since_epoch().count());
#ifdef NO_GRAPHICS
    for (int i = 0; i < 100; ++i) {
#endif
        //std::random_device r("mt19937");
        double speedA = random_speed(re);
        double speedB = random_speed(re);
        double speedC = random_speed(re);
        double distanceAB = random_distance(re);
        double distanceAC = random_distance(re);
        double distanceToEnd = 2000.0;
        //speedA, speedB, speedC, distanceAB, distanceAC, distanceToEnd, rightLane
        Road road(speedA, speedB, speedC, distanceAB, distanceAC, distanceToEnd, right);
        road.simulate();
#ifdef NO_GRAPHICS
    }
#endif
}
