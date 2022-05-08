//
// Created by Okke on 06.05.2022.
//

#ifndef SPACEGAME_CELESTIALBODY_H
#define SPACEGAME_CELESTIALBODY_H

#include <raylib.h>
#include <raymath.h>

#include <vector>
#include <iostream>

class CelestialBody {
private:
    const float G = 0.00001;
    std::vector<CelestialBody *> celestialBodies;

    const char* name;

    [[nodiscard]] float calculateMass(float surfaceGravity, float fRadius) const;

    float mass;

    float radius;

    bool hasRing = false;

    float outerRingRadius;

    float innerRingRadius;

    Vector2 pos;

    Vector2 velocity;

    Color color;

    Color ringColor;
public:
    bool selected = false;

    [[nodiscard]] float getMass() const;

    Vector2 getPosition();

    [[nodiscard]] float getRadius() const;

    [[nodiscard]] float getVelocity() const;

    const char* getName();

    void setOtherCelestialBodies(const std::vector<CelestialBody *> &celestial_bodies);

    CelestialBody(float surfaceGravity, float radius, Vector2 pos, Vector2 vel, Color color, const char *name);

    CelestialBody(float surfaceGravity, float pRadius, float inner_ring_radius, float outer_ring_radius, Vector2 pos, Vector2 vel, Color color, Color ring_color, const char *name);

    CelestialBody(float surfaceGravity, float radius, Color color, const char *name);

    CelestialBody(float surfaceGravity, float radius, float degrees, float vel, Color color, const char *name);

    virtual ~CelestialBody();

    void update(const float &dt);

    void render();

};


#endif //SPACEGAME_CELESTIALBODY_H
