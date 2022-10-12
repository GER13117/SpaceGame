//
// Created by Okke on 06.05.2022.
//

#ifndef SPACEGAME_GAME_H
#define SPACEGAME_GAME_H

#include <algorithm>
#include "Trajectories.h"
class Game {

private:

    float sideGuiWidth;

    Shader bloomShader;

    RenderTexture2D target;

    bool allowEdit = true;

    Trajectories *trajectories;

    float physicsTime;

    int timeWarp = 1;

    int numSteps;

    uint8_t frameCounter = 0;

    const float sunGravity = 500.F;

    const float sunRadius = 50.F;

    bool showPredictedTrajectories = false;

    bool pauseGame = true;

    bool editSystem = false;

    Camera2D camera;

    Vector2 posSelectedPlanet;

    bool anyBodySelected = false;

    Vector2 posSun;

    float distToSun = 0.F;

    float relativeVelSelectedPlanet;

    float radiusSelectedPlanet;

    const char *nameSelectedPlanet;

    int planetIndex = 0;

    std::vector<CelestialBody *> celestialBodies;

    void clearAllCelestialBodies();

    void initCelestialBodies();

    void initVariables();

    void resetSolarSystem();

    float startVel(float centralSurfaceGravity, float centralBodyRadius, float orbitDistance);

    void initWindow();

    void guiUpdateRender();

    void updateInput(const float &dt);

    void setNewCelestialBodies();

    void inWorldInfoText(Vector2 pos, float font_size);

    void initRenderElements();

    void update(const float &dt);

    void render();

public:
    Game();

    virtual ~Game();

    void run();

    void editSolarSystem();

    void onScreenText();
};


#endif //SPACEGAME_GAME_H
