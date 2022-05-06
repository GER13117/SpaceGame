//
// Created by Okke on 06.05.2022.
//

#include "include/Game.h"

void Game::initWindow() {
    InitWindow(800, 450, "Space Game");
    SetTargetFPS(60);
}

void Game::initCelestialBodies() {
    celestialBodies.push_back(new CelestialBody(200.F, 20.F, {100, 100}, RED));
}


Game::Game() {
    initWindow();
    initCelestialBodies();
    posY = 200;
}

Game::~Game() {

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
    for (auto e : celestialBodies) {
        e->update(dt);
    }

    //Moving Text
    if (posY > 450)
        posY = -20;
    else
        posY += dt * 20;

}

void Game::render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (auto e : celestialBodies) {
        e->render();
    }

    DrawText("Congrats! You created your first window!", 190, (int) posY, 20, LIGHTGRAY);

    DrawFPS(0,0);
    EndDrawing();
}


void Game::run() {
    while (!WindowShouldClose()) {
        update(GetFrameTime());
        render();
    }

    CloseWindow();
}