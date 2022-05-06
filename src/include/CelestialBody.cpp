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

}

void CelestialBody::render() {
    DrawCircle((int) this->pos.x, (int) this->pos.y, this->radius, this->color);
}

float CelestialBody::calculateMass(float surfaceGravity, float fRadius) {
    return surfaceGravity * fRadius * fRadius / G;
}
