//
// Created by an21 on 20.12.18.
//



#include "Road.h"
#include <SFML/Graphics.hpp>
#include <random>
#include <chrono>


void Road::simulate() {
    bool notEnd = true;
    std::string source = "../trapez.xml";
    fuzzyDriver.readRegs(source.c_str());
    double maxSpeed = fuzzyDriver.getSpeedVector().begin()->B;
#ifndef NO_GRAPHICS
    sf::RenderWindow window(sf::VideoMode(100, 500), "SFML Works!");
    float divider = 2000 / window.getSize().y;
    sf::CircleShape carA(1.f);
    sf::CircleShape carB(1.f);
    sf::CircleShape carC(1.f);
    carA.setFillColor(sf::Color::Green);
    carB.setFillColor(sf::Color::Red);
    carC.setFillColor(sf::Color::Yellow);
#else
    float divider = 1;
#endif
    std::uniform_int_distribution<int> possibility(0, 10);
//    std::random_device r;
    std::default_random_engine re(std::chrono::system_clock::now().time_since_epoch().count());
    double prevDistAC{};
    std::string crashed{};
    while (notEnd) {
        if (distanceToEnd < 0) {
#ifndef NO_GRAPHICS
            window.close();
#endif
            notEnd = false;
            break;
        }
        //std::this_thread::sleep_for(std::chrono::seconds(1));
#ifndef NO_GRAPHICS
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                notEnd = false;
                break;
            }
        }
#endif
        fuzzyDriver.calculateState(speedA, speedB, speedC, distanceAB, distanceAC, distanceToEnd, lane);
        speedA += fuzzyDriver.getAcceleration();
        speedB += possibility(re) % 10 == 0 ? possibility(re) % 2 == 0 ? 0.5 : -0.5 : 0;
        speedC += possibility(re) % 10 == 0 ? possibility(re) % 2 == 0 ? 0.5 : -0.5 : 0;
        if (speedA > maxSpeed) speedA = maxSpeed;
        if (speedB > maxSpeed) speedB = maxSpeed;
        if (speedC > maxSpeed) speedC = maxSpeed;
        if (speedB < 0) speedB = 0;
        if (speedC < 0) speedC = 0;
        lane = fuzzyDriver.getLane();
        distanceAB += speedB - speedA;
        prevDistAC = distanceAC;
        distanceAC -= speedA + speedC;
        if (distanceAC < 0 && prevDistAC > 0 && lane == left) {
#ifndef NO_GRAPHICS
            window.close();
#endif
            notEnd = false;
            crashed = "AC";
            break;
        }
        distanceToEnd -= speedA;
        if (distanceAB > distanceToEnd) {
#ifndef NO_GRAPHICS
            window.close();
#endif
            notEnd = false;
            break;
        }
        if (distanceAB < 4 && distanceAB > -4 && lane == right) {
            notEnd = false;
            crashed = "AB";
            if (distanceAB > 0) crashed += " wjechal A";
            if (distanceAB < 0) crashed += " wjechal B";
            break;
        }
//        std::cout << "A " << speedA << " B " << speedB << " C " << speedC << " distance AB " << distanceAB
//                  << " distance AC " << distanceAC << "\n";
#ifdef LOG_TO_CONSOLE
        std::cout << speedA << " " << speedB << " " << speedC << " " << distanceAB << " " << distanceAC << " "
                  << distanceToEnd << " " << rightLane << " " << fuzzyDriver.getAcceleration() << std::endl;
#endif
#ifndef NO_GRAPHICS
        carA.setPosition(50 - lane * 25, distanceToEnd / divider);
        carB.setPosition(50, distanceToEnd / divider - distanceAB / divider);
        carC.setPosition(25, distanceToEnd / divider - distanceAC / divider);
        window.clear();
        window.draw(carA);
        window.draw(carB);
        window.draw(carC);
        window.display();
#endif
    }
    std::string result = (distanceAB < 0 && crashed == "" && lane == right && distanceToEnd < 0) ? "TAK"
                                                                                                 : "NIE";
    std::cout << result << " Wypadek: " << crashed << "\n";
}

Road::Road(double speedA, double speedB, double speedC, double distanceAB, double distanceAC, double distanceToEnd,
           Lane lane) : speedA(speedA), speedB(speedB), speedC(speedC), distanceAB(distanceAB),
                        distanceAC(distanceAC), distanceToEnd(distanceToEnd), lane(lane),
                        fuzzyDriver(0.0f, Lane::right) {

}

