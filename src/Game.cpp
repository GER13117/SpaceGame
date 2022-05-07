//
// Created by Okke on 06.05.2022.
//

#include "include/Game.h"

#define RAYGUI_IMPLEMENTATION

#include "extras/raygui.h"

void Game::initWindow() {
    InitWindow(1500, 840, "Space Game");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    //Start Window in FullScreen
    int display = GetCurrentMonitor();
    SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
    ToggleFullscreen();

}

float Game::startVel(float centralSurfaceGravity, float centralBodyRadius, float orbitDistance) {
    if (orbitDistance < 0)
        return -sqrt(centralSurfaceGravity * centralBodyRadius * centralBodyRadius / -orbitDistance);
    return sqrt(centralSurfaceGravity * centralBodyRadius * centralBodyRadius / orbitDistance);
}

void Game::initCelestialBodies() {

    //Starting velocity = sqr(G * M_central / R)
    //Sun
    const float sunGravity = 500.F;
    const float sunRadius = 50.F;
    celestialBodies.push_back(new CelestialBody(sunGravity, sunRadius, GOLD));

    //Planets
    celestialBodies.push_back(new CelestialBody(10.F, 20.F, {200, 0}, {0, startVel(sunGravity, sunRadius, 200.F)}, GREEN));
    celestialBodies.push_back(new CelestialBody(10.F, 30.F, {-300, 0}, {0, startVel(sunGravity, sunRadius, -300.F)}, PURPLE));


    for (auto e: celestialBodies) {
        e->setOtherCelestialBodies(celestialBodies);
    }
}


Game::Game() {
    initWindow();
    camera = {0};
    camera.target = {0.f, 0.f};
    camera.offset = {(float) GetScreenWidth() / 2.0f, (float) GetScreenHeight() / 2.0f};
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
    //Toggle Fullscreen
    //TODO: Fix resize to windowed window
    if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))) {
        int display = GetCurrentMonitor();
        pauseGame = true;
        if (IsWindowFullscreen()) {
            SetWindowSize(1500, 840);
        } else {
            SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
        }
        ToggleFullscreen();
    }

    //Zoom in and out
    if (GetMouseWheelMove() < 0 && camera.zoom >= 0.1F)
        camera.zoom -= 0.1F;
    if (GetMouseWheelMove() > 0)
        camera.zoom += 0.1F;


    if (IsMouseButtonPressed(0)) {
        for (auto e: celestialBodies) {
            if (CheckCollisionPointCircle(GetMousePosition(), GetWorldToScreen2D(e->getPosition(), camera), e->getRadius())) {
                e->selected = true;
            } else
                e->selected = false;
        }
    }

    if (IsKeyPressed(KEY_SPACE)) {
        pauseGame = !pauseGame;
    }

    if (IsKeyPressed(KEY_LEFT)) {
        if (planetIndex > 0)
            planetIndex--;
        else
            planetIndex = celestialBodies.size() - 1;
    } else if (IsKeyPressed(KEY_RIGHT)) {
        if (planetIndex + 1 < celestialBodies.size())
            planetIndex++;
        else
            planetIndex = 0;
    }

}

void Game::guiUpdateRender() {
    if (GuiButton({10, 50, 80, 20}, pauseGame ? "continue" : "pause")) //False clang-tidy
        pauseGame = !pauseGame;
}

void Game::update(const float &dt) {
    updateInput(dt);
    if (!pauseGame) { //False clang-tidy
        for (auto e: celestialBodies) {
            e->update(dt);
        }
    }

    if (counter % 10 == 0) {
        for (auto e: celestialBodies) {
            if (e->selected) {
                posSelectedPlanet = e->getPosition();
                velSelectedPlanet = e->getVelocity();
            }
        }
    }
    counter++;

    camera.target = celestialBodies.at(planetIndex)->getPosition();
}

void Game::render() {
    BeginDrawing();
    ClearBackground({31, 33, 54});

    //World viewed by the camera
    BeginMode2D(camera);
    for (auto e: celestialBodies) {
        e->render();
    }
    EndMode2D();

    guiUpdateRender();

    DrawText(TextFormat("x: %2.3f", posSelectedPlanet.x), 10, 100, 20, WHITE);
    DrawText(TextFormat("y: %2.3f", posSelectedPlanet.y), 10, 130, 20, WHITE);
    DrawText(TextFormat("speed: %2.3f", velSelectedPlanet), 10, 160, 20, WHITE);

    DrawFPS(10, 10);
    EndDrawing();
}


void Game::run() {
    while (!WindowShouldClose()) {
        update(GetFrameTime()); //TODO: Fix stutter frame in the beginning
        render();
    }

    CloseWindow();
}