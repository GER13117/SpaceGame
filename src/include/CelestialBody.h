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

    [[nodiscard]] float calculateMass(float surfaceGravity, float fRadius) const;

    static float startVel(float centralSurfaceGravity, float centralBodyRadius, float orbitDistance);

    float mass;

    float radius;

    bool hasRing = false;

    float outerRingRadius = 0.F;

    float innerRingRadius = 0.F;

    Vector2 pos;

    Vector2 velocity;

    Color color;

    Color ringColor = BLACK;
public:
    //Flags
    bool selected = false;

    bool getsModified = false;

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
    /**
     * Sun / Central body of the planet-system
     * @param surfaceGravity
     * @param radius
     * @param color
     * @param name
     */
    CelestialBody(float surfaceGravity, float radius, Color color, const char *name);

    /**
     * Celestial body by x/y-position and velocity
     * @param surfaceGravity
     * @param radius
     * @param pos
     * @param vel
     * @param color
     * @param name
     */
    CelestialBody(float surfaceGravity, float radius, Vector2 pos, Vector2 vel, Color color, const char *name);

    /**
     * Sun orbiting planet by distance to sun
     * @param surfaceGravity
     * @param radius
     * @param dist_to_sun
     * @param sun_radius
     * @param sun_surface_gravity
     * @param color
     * @param name
     */
    CelestialBody(float surfaceGravity, float radius, float dist_to_sun, float sun_radius, float sun_surface_gravity, Color color, const char *name);

    /**
     * Planet with a ring around him, by x/y-position and velocity
     * @param surfaceGravity
     * @param pRadius
     * @param dist_to_surface
     * @param ring_width
     * @param pos
     * @param vel
     * @param color
     * @param ring_color
     * @param name
     */
    CelestialBody(float surfaceGravity, float pRadius, float dist_to_surface, float ring_width, Vector2 pos, Vector2 vel, Color color, Color ring_color, const char *name);

    CelestialBody(float surfaceGravity, float pRadius, float dist_to_surface, float ring_width, float dist_to_sun, float sun_radius, float sun_surface_gravity, Color color, Color ring_color, const char *name);

    virtual ~CelestialBody();

    void update(const float &dt);

    void render();
};


#endif //SPACEGAME_CELESTIALBODY_H
