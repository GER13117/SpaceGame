//
// Created by Okke on 06.05.2022.
//

#include "CelestialBody.h"

CelestialBody::CelestialBody(float surfaceGravity, float radius, const Vector2 &pos, Color color)
        : mass(calculateMass(surfaceGravity, radius)), radius(radius), pos(pos), color(color), velocity({0.F, 0.F}) {

}

CelestialBody::~CelestialBody() {

}

void CelestialBody::update(const float &dt) {

    Vector2 connectingVec; //Von Planet 1 zu Planet 2
    float sqrDist; //von connectingVec
    Vector2 forceDir = {0, 0}; //Normalenvektor von sqrDist oder ConnectingVec

    Vector2 acceleration = {100, 100}; //forceDir * G * (e->getMass() / sqrDist);

    velocity.x += acceleration.x * dt;
    velocity.y += acceleration.y * dt;

    /*
     * Collision
     * if (this->velocity.dot(forceDir) > 0.F && sqrDist <= (this->getRadius() + e->radius) * (this->getRadius() + e->radius) + 1) {
                this->velocity -= forceDir * this->velocity.dot(forceDir); //TODO: Maybe merge both planets into a bigger one
            }
     */

    pos.x += acceleration.x * dt;
    pos.y += acceleration.y * dt;
}

void CelestialBody::render() {
    DrawCircle((int) this->pos.x, (int) this->pos.y, this->radius, this->color);
}

float CelestialBody::calculateMass(float surfaceGravity, float fRadius) {
    return surfaceGravity * fRadius * fRadius / G;
}
