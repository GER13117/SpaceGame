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

    const char *name;

    [[nodiscard]] float calculateMass(float surface_gravity, float fRadius) const;

    static float startVel(float centralSurfaceGravity, float centralBodyRadius, float orbitDistance);

    float mass;

    float radius;

    float surfaceGravity;

    float outerRingRadius = 0.F;

    float innerRingRadius = 0.F;

    Vector2 pos;

    Vector2 initialPos;

    Vector2 velocity;

    Color color;

    Color ringColor = BLACK;
public:
    //Flags
    bool selected = false;

    bool getsModified = false;

    bool hasRing = false;

    //Getter
    [[nodiscard]] float getMass() const;

    Vector2 getPosition();

    [[nodiscard]] float getRadius() const;

    [[nodiscard]] float getVelocity() const;

    Vector2 getVVelocity();

    const char *getName();

    Color getColor();

    //Setter
    void setColor(Color newColor);

    void setRadius(float newRadius);

    void setOtherCelestialBodies(const std::vector<CelestialBody *> &celestial_bodies);

    //Constructors
    CelestialBody(float surfaceGravity, float radius, Color color, const char *name);

    CelestialBody(float surfaceGravity, float radius, Vector2 pos, Vector2 vel, Color color, const char *name);

    CelestialBody(float surfaceGravity, float radius, float dist_to_sun, float sun_radius, float sun_surface_gravity, Color color, const char *name);

    CelestialBody(float surfaceGravity, float pRadius, float dist_to_surface, float ring_width, Vector2 pos, Vector2 vel, Color color, Color ring_color, const char *name);

    CelestialBody(float surfaceGravity, float pRadius, float dist_to_surface, float ring_width, float dist_to_sun, float sun_radius, float sun_surface_gravity, Color color, Color ring_color, const char *name);

    virtual ~CelestialBody();

    void update(const float &dt);

    void render();

    void recalculateMass();

    void setSurfaceGravity(float newSurfaceGravity);

    float getSurfaceGravity() const;

    void setVVelocity(Vector2 newVVelocity);

    Color getRingColor();

    float getInnerRadius() const;

    float getOuterRadius() const;

    Vector2 getInitialPosition();
};


#endif //SPACEGAME_CELESTIALBODY_H
