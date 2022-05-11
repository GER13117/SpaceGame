//
// Created by Okke on 06.05.2022.
//

#include "include/CelestialBody.h"

CelestialBody::CelestialBody(float surfaceGravity, float radius, const Vector2 pos, Vector2 vel, Color color, const char *name)
        : mass(calculateMass(surfaceGravity, radius)), surfaceGravity(surfaceGravity), radius(radius), pos(pos), velocity(vel), color(color), name(name) {}

CelestialBody::CelestialBody(float surfaceGravity, float pRadius, float dist_to_surface, float ring_width, Vector2 pos, Vector2 vel, Color color, Color ring_color, const char *name)
        : mass(calculateMass(surfaceGravity, pRadius)), surfaceGravity(surfaceGravity), radius(pRadius),
          innerRingRadius(dist_to_surface + pRadius), outerRingRadius(dist_to_surface + pRadius + ring_width),
          pos(pos), velocity(vel), color(color), ringColor(ring_color), name(name), hasRing(true) {}

CelestialBody::CelestialBody(float surfaceGravity, float pRadius, float dist_to_surface, float ring_width, float dist_to_sun, float sun_radius, float sun_surface_gravity, Color color,
                             Color ring_color, const char *name)
        : mass(calculateMass(surfaceGravity, pRadius)), surfaceGravity(surfaceGravity), radius(pRadius),
          innerRingRadius(dist_to_surface + pRadius), outerRingRadius(dist_to_surface + pRadius + ring_width),
          pos({dist_to_sun, 0}), velocity({0, startVel(sun_surface_gravity, sun_radius, dist_to_sun)}),
          color(color), ringColor(ring_color), name(name), hasRing(true) {
}

CelestialBody::CelestialBody(float surfaceGravity, float radius, float dist_to_sun, float sun_radius, float sun_surface_gravity, Color color, const char *name)
        : mass(calculateMass(surfaceGravity, radius)), surfaceGravity(surfaceGravity), radius(radius), pos({dist_to_sun, 0}), velocity({0, startVel(sun_surface_gravity, sun_radius, dist_to_sun)}),
          color(color), name(name) {}

CelestialBody::CelestialBody(float surfaceGravity, float radius, Color color, const char *name)
        : mass(calculateMass(surfaceGravity, radius)), surfaceGravity(surfaceGravity), radius(radius), pos({0, 0}), velocity({0, 0}), color(color), name(name) {}

CelestialBody::~CelestialBody() = default;

//Getter
float CelestialBody::getMass() const {
    return this->mass;
}

Vector2 CelestialBody::getPosition() {
    return this->pos;
}

float CelestialBody::getRadius() const {
    return radius;
}

float CelestialBody::getVelocity() const {
    return Vector2Length(velocity);
}

const char *CelestialBody::getName() {
    return name;
}

Vector2 CelestialBody::getVVelocity() {
    return this->velocity;
}


//Setter
void CelestialBody::setOtherCelestialBodies(const std::vector<CelestialBody *> &celestial_bodies) {
    this->celestialBodies = celestial_bodies;
}


void CelestialBody::update(const float &dt) {
    for (auto e: celestialBodies) {
        if (e != this) {
            Vector2 connectingVec = Vector2Subtract(e->getPosition(), this->pos);
            float sqrDist = Vector2LengthSqr(connectingVec);
            Vector2 forceDir = Vector2Normalize(connectingVec);

            float accelerationX = forceDir.x * G * (e->getMass() / sqrDist);
            float accelerationY = forceDir.y * G * (e->getMass() / sqrDist);

            velocity.x += accelerationX * dt;
            velocity.y += accelerationY * dt;
            /* Collision
             * if (this->velocity.dot(forceDir) > 0.F && sqrDist <= (this->getRadius() + e->radius) * (this->getRadius() + e->radius) + 1) {
                this->velocity -= forceDir * this->velocity.dot(forceDir); //TODO: Implement
            }*/
        }
    }

    this->pos.x += velocity.x * dt;
    this->pos.y += velocity.y * dt;
}

void CelestialBody::render() {
    if (hasRing) {
        DrawRing(pos, this->innerRingRadius, this->outerRingRadius, 0, 360, 90, ringColor);
    }

    if (selected || getsModified) {
        DrawCircleV(pos, this->radius, RED);
        DrawCircleV(pos, this->radius - 5, this->color);
    } else {
        DrawCircleV(pos, this->radius, this->color);

    }
}

void CelestialBody::recalculateMass() {
    this->mass = calculateMass(this->surfaceGravity, this->radius);
}

float CelestialBody::calculateMass(float surface_gravity, float fRadius) const {
    return surface_gravity * fRadius * fRadius / G;
}

float CelestialBody::startVel(float centralSurfaceGravity, float centralBodyRadius, float orbitDistance) {
    if (orbitDistance < 0)
        return -sqrt(centralSurfaceGravity * centralBodyRadius * centralBodyRadius / -orbitDistance);
    return sqrt(centralSurfaceGravity * centralBodyRadius * centralBodyRadius / orbitDistance);
}

Color CelestialBody::getColor() {
    return this->color;
}

void CelestialBody::setColor(Color newColor) {
    this->color = newColor;
}

void CelestialBody::setRadius(float newRadius) {
    this->radius = newRadius;
}

void CelestialBody::setSurfaceGravity(float newSurfaceGravity) {
    this->surfaceGravity = newSurfaceGravity;
}

float CelestialBody::getSurfaceGravity() const {
    return this->surfaceGravity;
}

void CelestialBody::setVVelocity(float newVelocity) {
    //TODO: Fancy Maths by using the position and new Velocity to get new VVelocity;
}
