//
// Created by Okke on 12.05.2022.
//

#ifndef SPACEGAME_TRAJECTORIES_H
#define SPACEGAME_TRAJECTORIES_H
#include "CelestialBody.h"

class Trajectories {
private:
    std::vector<CelestialBody * > celestialBodies;
    std::vector<CelestialBody *> virtualBodies;

    Vector2 **linePoints{};

    int numPlanets{};

    int numSteps;

public:
    Trajectories(int numSteps);

    void setCelestialBodies(std::vector<CelestialBody *> &celestial_bodies);

    void update(const float &dt, int centralPlanet);

    void render();

};


#endif //SPACEGAME_TRAJECTORIES_H
