//
// Created by Okke on 06.05.2022.
//

#ifndef SPACEGAME_CELESTIALBODY_H
#define SPACEGAME_CELESTIALBODY_H

#include <raylib.h>
#include <raymath.h>

#include <vector>

class CelestialBody {
private:
    const float G = 0.00001;
    std::vector<CelestialBody *> celestialBodies;
    float calculateMass(float surfaceGravity, float fRadius) const;

    float mass;
    float radius;
    Vector2 pos;
    Vector2 velocity;
    Color color;
public:
    float getMass() const;

    Vector2 getPosition();

    void setOtherCelestialBodies(const std::vector<CelestialBody *> &celestial_bodies);

    CelestialBody(float surfaceGravity, float radius, const Vector2 pos, Vector2 vel, Color color);

    virtual ~CelestialBody();

    void update(const float &dt);

    void render();

};


#endif //SPACEGAME_CELESTIALBODY_H
