//
// Created by Okke on 06.05.2022.
//

#include "CelestialBody.h"

CelestialBody::CelestialBody(float surfaceGravity, float radius, const Vector2 pos, Vector2 vel, Color color)
        : mass(calculateMass(surfaceGravity, radius)), radius(radius), pos(pos), velocity(vel), color(color) {

}

CelestialBody::~CelestialBody() {

}

float CelestialBody::getMass() {
    return this->mass;
}

Vector2 CelestialBody::getPosition() {
    return this->pos;
}

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
    DrawCircle((int) this->pos.x, (int) this->pos.y, this->radius, this->color);
}

float CelestialBody::calculateMass(float surfaceGravity, float fRadius) {
    return surfaceGravity * fRadius * fRadius / G;
}
