//
// Created by Okke on 12.05.2022.
//

#include "include/Trajectories.h"

#include <utility>

void Trajectories::setCelestialBodies(std::vector<CelestialBody *> &celestial_bodies) {
    this->celestialBodies = celestial_bodies;

}

void Trajectories::update(const float &dt) {
    for (int i = 0; i < celestialBodies.size(); i++) {
        if (celestialBodies[i]->hasRing) {
            float distToSurface = celestialBodies[i]->getInnerRadius() - celestialBodies[i]->getRadius();
            float ringWidth = celestialBodies[i]->getOuterRadius() - celestialBodies[i]->getInnerRadius();
            virtualBodies.push_back(new CelestialBody(celestialBodies[i]->getSurfaceGravity(), celestialBodies[i]->getRadius(), distToSurface, ringWidth, celestialBodies[i]->getPosition(),
                                                      celestialBodies[i]->getVVelocity(), celestialBodies[i]->getColor(), celestialBodies[i]->getRingColor(), celestialBodies[i]->getName()));
        } else {
            virtualBodies.push_back(new CelestialBody(celestialBodies[i]->getSurfaceGravity(), celestialBodies[i]->getRadius(), celestialBodies[i]->getPosition(), celestialBodies[i]->getVVelocity(),
                                                      celestialBodies[i]->getColor(), celestialBodies[i]->getName()));
        }
    }

    numPlanets = virtualBodies.size();
    for (auto e: virtualBodies) {
        e->setOtherCelestialBodies(virtualBodies);
    }

    linePoints = new Vector2 *[numPlanets];
    for (int i = 0; i < numPlanets; i++) {
        linePoints[i] = new Vector2[numSteps];
    }

    for (int step = 0; step < numSteps; step++) {
        for (int planet = 0; planet < numPlanets; planet++) {
            virtualBodies[planet]->update(dt);
            linePoints[planet][step] = virtualBodies[planet]->getPosition();
        }
    }

    for (auto e: virtualBodies) {
        delete e;
    }
    virtualBodies.clear();
}

Trajectories::Trajectories(int numSteps)
    : numSteps(numSteps) {
}

void Trajectories::render() {
    for (int body = 0; body < numPlanets; body++) {
        DrawLineStrip(linePoints[body], numSteps, celestialBodies[body]->getColor());
    }
}
