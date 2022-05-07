//
// Created by Okke on 06.05.2022.
//

#ifndef SPACEGAME_GAME_H
#define SPACEGAME_GAME_H

#include "CelestialBody.h"

class Game {

private:
    bool pauseGame = true;

    Camera2D camera;

    std::size_t planetIndex = 0;

    std::vector<CelestialBody *> celestialBodies;

    void initCelestialBodies();

    void initWindow();

    void guiUpdateRender();

    void updateInput(const float &dt);

    void update(const float &dt);

    void render();

public:
    Game();

    virtual ~Game();

    void run();
};


#endif //SPACEGAME_GAME_H
