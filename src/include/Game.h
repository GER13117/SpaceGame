//
// Created by Okke on 06.05.2022.
//

#ifndef SPACEGAME_GAME_H
#define SPACEGAME_GAME_H

#include "CelestialBody.h"

class Game {

private:
    Camera2D camera;

    int planetIndex = 0;

    std::vector<CelestialBody *> celestialBodies;

    void initCelestialBodies();

    void initWindow();

    void updateInput(const float &dt);

    void update(const float &dt);

    void render();

public:
    Game();

    virtual ~Game();

    void run();
};


#endif //SPACEGAME_GAME_H
