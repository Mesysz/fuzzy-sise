//
// Created by an21 on 20.12.18.
//



#include "Road.h"
#include <SFML/Graphics.hpp>
void Road::simulate() {
    bool notEnd = true;
    std::string source = "../ustaw.xml";
    fuzzyDriver.readRegs(source.c_str());
    sf::RenderWindow window(sf::VideoMode(100, 500), "SFML Works!");
    float divider = 2000 / window.getSize().y;
    sf::CircleShape carA(5.f);
    sf::CircleShape carB(5.f);
    sf::CircleShape carC(5.f);
    carA.setFillColor(sf::Color::Green);
    carB.setFillColor(sf::Color::Red);
    carC.setFillColor(sf::Color::Yellow);
    while (notEnd) {
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        if (distanceToEnd < 0) {
            window.close();
            notEnd = false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                notEnd = false;
            }
        }
        speedA += fuzzyDriver.getAcceleration();
        if(speedA > fuzzyDriver.getSpeedVector().begin()->B){
            speedA = fuzzyDriver.getSpeedVector().begin()->B;
        }
        lane = fuzzyDriver.getLane();
        distanceAB += speedB - speedA;
        distanceAC -= speedA + speedC;
        distanceToEnd -= speedA;
        fuzzyDriver.calculateState(speedA, speedB, speedC, distanceAB, distanceAC, distanceToEnd, lane);
        window.clear();
        //std::cout<<fuzzyDriver.getAcceleration()<<" "<<speedA<<" "<<distanceToEnd<<" "<<distanceAC<<" "<<distanceAB<<"\n";
        carA.setPosition(50 - lane * 25, distanceToEnd / divider);
        carB.setPosition(50, distanceToEnd / divider - distanceAB / divider);
        carC.setPosition(25, distanceToEnd / divider - distanceAC / divider);
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

