//
// Created by an21 on 20.12.18.
//

#include "Road.h"
#include <SFML/Graphics.hpp>
void Road::simulate() {
    bool notEnd = true;
    int i = 0;
    std::string source = "../ustaw.xml";
    fuzzyDriver.readRegs(source.c_str());
    sf::RenderWindow window(sf::VideoMode(200, 500), "SFML Works!");
    float divider = 2000 / window.getSize().y;
    sf::CircleShape carA(5.f);
    sf::CircleShape carB(5.f);
    sf::CircleShape carC(5.f);
    carA.setFillColor(sf::Color::Green);
    carB.setFillColor(sf::Color::Red);
    carC.setFillColor(sf::Color::Yellow);
    while (notEnd) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                notEnd = false;
            }
        }
        fuzzyDriver.calculateState(speedA, speedB, speedC, distanceAB, distanceAC, distanceToEnd, rightLane);
        speedA += fuzzyDriver.getAcceleration();
        rightLane = fuzzyDriver.getLane();
        distanceAB += speedB - speedA;
        distanceAC -= speedA + speedC;
        distanceToEnd -= speedA;
        window.clear();
        carA.setPosition(150, distanceToEnd / divider);
        carB.setPosition(150, distanceToEnd / divider - distanceAB / divider);
        carC.setPosition(100, distanceToEnd / divider - distanceAC / divider);
        window.draw(carA);
        window.draw(carB);
        window.draw(carC);
        std::cout << speedA << " " << speedB << " " << speedC << " " << distanceAB << " " << distanceAC << " "
                  << distanceToEnd << " " << rightLane << std::endl;
        window.display();
    }
}

Road::Road(double speedA, double speedB, double speedC, double distanceAB, double distanceAC, double distanceToEnd,
           bool rightLane) : speedA(speedA), speedB(speedB), speedC(speedC), distanceAB(distanceAB),
                             distanceAC(distanceAC), distanceToEnd(distanceToEnd), rightLane(rightLane),
                             fuzzyDriver(0.0f, Lane::right) {

}

