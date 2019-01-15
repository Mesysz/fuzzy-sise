//
// Created by an21 on 20.12.18.
//



#include "Road.h"
#include <SFML/Graphics.hpp>
#include <random>
#include <chrono>

void Road::simulate() {
    bool notEnd = true;
    std::string source = "../ustaw.xml";
    fuzzyDriver.readRegs(source.c_str());
    double maxSpeed = fuzzyDriver.getSpeedVector().begin()->B;
    sf::RenderWindow window(sf::VideoMode(100, 500), "SFML Works!");
    float divider = 2000 / window.getSize().y;
    sf::CircleShape carA(5.f);
    sf::CircleShape carB(5.f);
    sf::CircleShape carC(5.f);
    carA.setFillColor(sf::Color::Green);
    carB.setFillColor(sf::Color::Red);
    carC.setFillColor(sf::Color::Yellow);
    std::uniform_int_distribution<int> possibility(0, 10);
//    std::random_device r;
    std::default_random_engine re(std::chrono::system_clock::now().time_since_epoch().count());
    while (notEnd) {
        if (distanceToEnd < 0) {
            window.close();
            notEnd = false;
            break;
        }
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                notEnd = false;
                break;
            }
        }
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
        distanceAC -= speedA + speedC;
        distanceToEnd -= speedA;
        std::cout << "A " << speedA << " B " << speedB << " C " << speedC << " distance AB " << distanceAB
                  << " distance AC " << distanceAC << "\n";
        carA.setPosition(50 - lane * 25, distanceToEnd / divider);
        carB.setPosition(50, distanceToEnd / divider - distanceAB / divider);
        carC.setPosition(25, distanceToEnd / divider - distanceAC / divider);
        window.clear();
        window.draw(carA);
        window.draw(carB);
        window.draw(carC);
#ifdef LOG_TO_CONSOLE
        std::cout << speedA << " " << speedB << " " << speedC << " " << distanceAB << " " << distanceAC << " "
                  << distanceToEnd << " " << rightLane << " " << fuzzyDriver.getAcceleration() << std::endl;
#endif
        window.display();
    }
}

Road::Road(double speedA, double speedB, double speedC, double distanceAB, double distanceAC, double distanceToEnd,
           Lane lane) : speedA(speedA), speedB(speedB), speedC(speedC), distanceAB(distanceAB),
                        distanceAC(distanceAC), distanceToEnd(distanceToEnd), lane(lane),
                        fuzzyDriver(0.0f, Lane::right) {

}

