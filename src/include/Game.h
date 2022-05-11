//
// Created by Okke on 06.05.2022.
//

#ifndef SPACEGAME_GAME_H
#define SPACEGAME_GAME_H

#include "CelestialBody.h"

class Game {

private:
    bool modifyPlanet;

    const float sunGravity = 500.F;

    const float sunRadius = 50.F;

    bool showPredictedTrajectories;

    bool pauseGame = true;

    bool editSystem = false;

    Camera2D camera;

    Vector2 posSelectedPlanet;

    bool anyBodySelected = false;

    Vector2 posSun;

    float distToSun = 0.F;

    float relativeVelSelectedPlanet;

    float radiusSelectedPlanet;

    const char* nameSelectedPlanet;

    std::size_t planetIndex = 0;

    std::vector<CelestialBody *> celestialBodies;

    void initCelestialBodies();

    void resetSolarSystem();

    float startVel(float centralSurfaceGravity, float centralBodyRadius, float orbitDistance);

    void initWindow();

    void guiUpdateRender();

    void updateInput(const float &dt);

    void infoText(Vector2 pos, float font_size);

    void update(const float &dt);

    void render();

public:
    Game();

    virtual ~Game();

    void run();

    void editSolarSystem();

    void updateTrajectories();
};


#endif //SPACEGAME_GAME_H
