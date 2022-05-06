//
// Created by Okke on 06.05.2022.
//

#ifndef SPACEGAME_CELESTIALBODY_H
#define SPACEGAME_CELESTIALBODY_H
#include <raylib.h>

class CelestialBody {
private:
    const float G = 0.00001;
    float calculateMass(float surfaceGravity, float fRadius);

    float mass;
    float radius;
    Vector2 pos;
    Vector2 velocity;
    Color color;
public:

    CelestialBody(float surfaceGravity, float radius, const Vector2 &pos, Color color);

    virtual ~CelestialBody();

    void update(const float &dt);

    void render();

};


#endif //SPACEGAME_CELESTIALBODY_H
