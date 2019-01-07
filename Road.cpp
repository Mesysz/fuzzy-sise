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
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML Works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    while (notEnd) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.draw(shape);
        window.display();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        fuzzyDriver.calculateState(speedA, speedB, speedC, distanceAB, distanceAC, distanceToEnd, rightLane);
        speedA += fuzzyDriver.getAcceleration();
        rightLane = fuzzyDriver.getLane();
        distanceAB += speedB - speedA;
        distanceAC -= speedA + speedC;
        distanceToEnd -= speedA;
        std::cout << speedA << " " << speedB << " " << speedC << " " << distanceAB << " " << distanceAC << " "
                  << distanceToEnd << " " << rightLane << std::endl;
    }
}

Road::Road(double speedA, double speedB, double speedC, double distanceAB, double distanceAC, double distanceToEnd,
           bool rightLane) : speedA(speedA), speedB(speedB), speedC(speedC), distanceAB(distanceAB),
                             distanceAC(distanceAC), distanceToEnd(distanceToEnd), rightLane(rightLane),
                             fuzzyDriver(0.0f, Lane::right) {

}

