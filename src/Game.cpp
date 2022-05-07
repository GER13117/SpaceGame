//
// Created by Okke on 06.05.2022.
//

#include "include/Game.h"

void Game::initWindow() {
    InitWindow(1500, 840, "Space Game");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    /*int display = GetCurrentMonitor();
    SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
    ToggleFullscreen();*/
}

void Game::initCelestialBodies() {

    //Starting velocity = sqr(G * M_central / R)

    celestialBodies.push_back(new CelestialBody(500.F, 50.F, {0, 0}, {0, 0}, {255, 203, 0, 255}));
    celestialBodies.push_back(new CelestialBody(10.F, 20.F, {200, 0}, {0, 79}, GREEN));
    celestialBodies.push_back(new CelestialBody(10.F, 30.F, {-300, 0}, {0, -65}, PURPLE));


    for (auto e: celestialBodies) {
        e->setOtherCelestialBodies(celestialBodies);
    }
}


Game::Game() {
    initWindow();
    camera = {0};
    camera.target = (Vector2) {0.f, 0.f};
    camera.offset = (Vector2) {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    initCelestialBodies();
}

Game::~Game() {
    for (auto e: celestialBodies) {
        delete e;
    }
    celestialBodies.clear();
}

void Game::updateInput(const float &dt) {
    if (IsKeyPressed(KEY_LEFT) && planetIndex > 0)
        planetIndex--;
    else if (IsKeyPressed(KEY_RIGHT) && planetIndex + 1 < celestialBodies.size())
        planetIndex++;
}

void Game::update(const float &dt) {
    updateInput(dt);
    for (auto e: celestialBodies) {
        e->update(dt);
    }
    camera.target = celestialBodies.at(planetIndex)->getPosition();
}

void Game::render() {
    BeginDrawing();
    ClearBackground({31, 33, 54});

    BeginMode2D(camera);
    for (auto e: celestialBodies) {
        e->render();
    }
    EndMode2D();

    DrawFPS(0, 0);
    EndDrawing();
}


void Game::run() {
    while (!WindowShouldClose()) {
        update(GetFrameTime()); //TODO: Fix stutter frame in the beginning
        render();
    }

    CloseWindow();
}