//
// Created by Okke on 06.05.2022.
//

#include "include/Game.h"

void Game::initWindow() {
    InitWindow(800, 450, "Space Game");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
}

void Game::initCelestialBodies() {
    celestialBodies.push_back(new CelestialBody(500.F, 50.F, {400, 200}, {0, 0}, {255, 203, 0, 255}));
    celestialBodies.push_back(new CelestialBody(10.F, 20.F, {600, 200}, {0, 79}, GREEN));


    for (auto e: celestialBodies) {
        e->setOtherCelestialBodies(celestialBodies);
    }
}


Game::Game() {
    initWindow();
    initCelestialBodies();
}

Game::~Game() {
    for (auto e: celestialBodies) {
        delete e;
    }
    celestialBodies.clear();
}

void Game::updateInput(const float &dt) {
    if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))) {
        int display = GetCurrentMonitor();

        if (IsWindowFullscreen()) {
            SetWindowSize(800, 450);
        } else {
            SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
        }

        ToggleFullscreen();
    }
}

void Game::update(const float &dt) {
    updateInput(dt);
    for (auto e: celestialBodies) {
        e->update(dt);
    }
}

void Game::render() {
    BeginDrawing();
    ClearBackground({31, 33, 54});

    for (auto e: celestialBodies) {
        e->render();
    }

    DrawFPS(0, 0);
    EndDrawing();
}


void Game::run() {
    while (!WindowShouldClose()) {
        update(GetFrameTime());
        render();
    }

    CloseWindow();
}