//
// Created by Okke on 06.05.2022.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"

#include "include/Game.h"

#define RAYGUI_IMPLEMENTATION

#include "extras/raygui.h"

void Game::initWindow() {
    InitWindow(1920, 1080, "Space Game");
    SetTargetFPS(60);

    //Start Window in FullScreen
    int display = GetCurrentMonitor();
    //SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
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
    celestialBodies.push_back(new CelestialBody(sunGravity, sunRadius, GOLD, "Sun"));

    //Planets
    celestialBodies.push_back(new CelestialBody(10.F, 20.F, 200, sunRadius, sunGravity, GREEN, "Earth"));

    celestialBodies.push_back(new CelestialBody(10.F, 30.F, 10.F, 7.F, -300,
                                                sunRadius, sunGravity,
                                                PURPLE, {245, 245, 245, 200}, "Pluto"));

    celestialBodies.push_back(new CelestialBody(15.F, 10.F, 600, sunRadius, sunGravity, BROWN, "Brownie"));
    celestialBodies.push_back(new CelestialBody(2.F, 5.F, {600, 25},
                                                {startVel(15.F, 10.F, 25.F),
                                                 startVel(sunGravity, sunRadius, 600.F)},
                                                LIGHTGRAY, "Moon"));


    celestialBodies.push_back(new CelestialBody(9.F, 30.F, 1000, sunRadius, sunGravity, BLUE, "Poseidon"));
    celestialBodies.push_back(new CelestialBody(2.F, 5.F, {1000, 50},
                                                {startVel(9.F, 30.F, 50.F),
                                                 startVel(sunGravity, sunRadius, 1000.F)},
                                                PINK, "Europa"));
    celestialBodies.push_back(new CelestialBody(1.F, 7.F, {1000, -70},
                                                {startVel(9.F, 30.F, -70.F),
                                                 startVel(sunGravity, sunRadius, 1000.F)},
                                                RAYWHITE, "Rasmus"));


    for (auto e: celestialBodies) {
        e->setOtherCelestialBodies(celestialBodies);
    }
}

void Game::resetSolarSystem() {
    for (auto e: celestialBodies) {
        delete e;
    }
    celestialBodies.clear();

    initCelestialBodies();

    pauseGame = true;

    updateTrajectories();
}

void Game::editSolarSystem() {
    pauseGame = true;
    //TODO: Implement CelestialBody Constructor that calculates the velocity using the angle of the planet

    if (IsMouseButtonPressed(0) &&
        !CheckCollisionPointRec(GetMousePosition(), {0, 0, 300, static_cast<float>(GetScreenHeight())})) { //TODO: Planet add modus?
        float fDistToSun = Vector2Distance(celestialBodies[0]->getPosition(),
                                           GetScreenToWorld2D(GetMousePosition(), camera));
        celestialBodies.push_back(new CelestialBody(10.F, 20.F, fDistToSun, sunRadius, sunGravity, GREEN, "Earth"));
    }

    if (IsMouseButtonPressed(1)) {
        for (auto e: celestialBodies) {
            e->selected = false;
            if (CheckCollisionPointCircle(GetMousePosition(), GetWorldToScreen2D(e->getPosition(), camera),
                                          e->getRadius() * camera.zoom)) {
                e->getsModified = true;
            } else
                e->getsModified = false;
        }
    }

    for (auto e: celestialBodies) {
        if (e->getsModified) {
            e->setColor(GuiColorPicker(
                    {10, 300, 200, 200},
                    e->getColor()));

            e->setRadius(GuiSlider({10, 300 + 220, 120, 30}, "", "", e->getRadius(), 1, 200));
            e->setSurfaceGravity(GuiSlider({10, 300 + 220 + 40, 120, 30}, "", "", e->getSurfaceGravity(), 1, 200));
            if (GuiButton({10, 300 + 220 + 40 + 40, 120, 30}, "Recalculate Mass")) {
                e->recalculateMass();
                if (e == celestialBodies[0]) {
                    //Update Velocity for other Planets
                }
            }

            e->setVVelocity(GuiSlider({10, 300 + 220, 120, 30}, "", "", e->getVelocity(), 1, 200));

            if (GuiButton({10, 300 + 220 + 40 + 40 + 40, 120, 30}, "Delete Body")) {
                std::cout << "delete e;" << std::endl;
            } //TODO: Fix
        }
    }

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

    numSteps = 8000;
    timeStep = 1.F / 60.F;
}

Game::~Game() {
    for (auto e: celestialBodies) {
        delete e;
    }
    celestialBodies.clear();
}

void Game::updateTrajectories() {
    std::vector<CelestialBody *> virtualBodies;

    for (int i = 0; i < celestialBodies.size(); i++) {
        if (celestialBodies[i]->hasRing) {
            float distToSurface = celestialBodies[i]->getInnerRadius() - celestialBodies[i]->getRadius();
            float ringWidth = celestialBodies[i]->getOuterRadius() - celestialBodies[i]->getInnerRadius();
            virtualBodies.push_back(new CelestialBody(celestialBodies[i]->getSurfaceGravity(), celestialBodies[i]->getRadius(), distToSurface, ringWidth, celestialBodies[i]->getPosition(),
                                                      celestialBodies[i]->getVVelocity(), celestialBodies[i]->getColor(), celestialBodies[i]->getRingColor(), celestialBodies[i]->getName()));
        } else {
            virtualBodies.push_back(new CelestialBody(celestialBodies[i]->getSurfaceGravity(), celestialBodies[i]->getRadius(), celestialBodies[i]->getPosition(), celestialBodies[i]->getVVelocity(),
                                                      celestialBodies[i]->getColor(), celestialBodies[i]->getName()));
        }
    }

    numPlanets = virtualBodies.size();
    for (auto e: virtualBodies) {
        e->setOtherCelestialBodies(virtualBodies);
    }

    linePoints = new Vector2 *[numPlanets];
    for (int i = 0; i < numPlanets; i++) {
        linePoints[i] = new Vector2[numSteps];
    }

    for (int step = 0; step < numSteps; step++) {
        for (int planet = 0; planet < numPlanets; planet++) {
            virtualBodies[planet]->update(timeStep);
            linePoints[planet][step] = virtualBodies[planet]->getPosition();
        }
    }

    for (auto e: virtualBodies) {
        delete e;
    }
    virtualBodies.clear();
}

void Game::drawTrajectories() {
    for (int body = 0; body < numPlanets; body++) {
        DrawLineStrip(linePoints[body], numSteps, celestialBodies[body]->getColor());
    }
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
            if (CheckCollisionPointCircle(GetMousePosition(),
                                          GetWorldToScreen2D(e->getPosition(), camera),
                                          e->getRadius() * camera.zoom)) {
                e->selected = true;
            } else
                e->selected = false;
        }
    }

    if (IsKeyPressed(KEY_SPACE)) {
        for (auto e: celestialBodies) {
            e->getsModified = false;
        }
        pauseGame = !pauseGame;
    }

    if (IsKeyPressed(KEY_R)) {
        resetSolarSystem();
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
    if (GuiButton({10, 50, 120, 30}, pauseGame ? "continue" : "pause")) //False clang-tidy
        pauseGame = !pauseGame;
    if (GuiButton({10, 90, 120, 30}, "reset solar system")) //False clang-tidy
        resetSolarSystem();
    editSystem = GuiToggle({10, 130, 120, 30}, "edit solar system", editSystem);
    if (editSystem)
        editSolarSystem();
    showPredictedTrajectories = GuiToggle({10, 170, 120, 30}, "show trajectories", showPredictedTrajectories);
}

void Game::infoText(Vector2 pos, float font_size) {
    DrawTextEx(GetFontDefault(), nameSelectedPlanet,
               pos,
               font_size + 10.F, (font_size + 10.F) / 10.F, WHITE);
    DrawTextEx(GetFontDefault(), TextFormat("x: %0.3f", posSelectedPlanet.x),
               Vector2Add(pos, {0.F, 1.5F * font_size + 10}),
               font_size, font_size / 10.F, WHITE);
    DrawTextEx(GetFontDefault(), TextFormat("y: %0.3f", posSelectedPlanet.y),
               Vector2Add(pos, {0.F, 2.5F * font_size + 10}),
               font_size, font_size / 10.F, WHITE);
    DrawTextEx(GetFontDefault(), TextFormat("relative speed: %0.3f", relativeVelSelectedPlanet),
               Vector2Add(pos, {0.F, 3.5F * font_size + 10}),
               font_size, font_size / 10.F, WHITE);
    DrawTextEx(GetFontDefault(), TextFormat("dist to sun: %0.3f", distToSun),
               Vector2Add(pos, {0.F, 4.5F * font_size + 10}),
               font_size, font_size / 10.F, WHITE);
}


void Game::update(const float &dt) {
    frameCounter++;
    anyBodySelected = false;
    if (showPredictedTrajectories && frameCounter % 2 == 0) {
        updateTrajectories();
    }

    updateInput(dt);
    if (!pauseGame) { //False clang-tidy
        for (int i = 0; i < timeWarp; ++i) {
            for (auto e: celestialBodies) {
                e->update(dt);
            }
        }
    }
    posSun = celestialBodies[0]->getPosition();
    for (auto e: celestialBodies) {
        if (e->selected) {
            posSelectedPlanet = e->getPosition();
            relativeVelSelectedPlanet = Vector2Distance(celestialBodies[planetIndex]->getVVelocity(),
                                                        e->getVVelocity());
            nameSelectedPlanet = e->getName();
            radiusSelectedPlanet = e->getRadius();
            anyBodySelected = true;
        }
    }
    distToSun = Vector2Distance(posSun, posSelectedPlanet);

    camera.target = celestialBodies.at(planetIndex)->getPosition();
}

void Game::render() {
    BeginDrawing();
    ClearBackground({0, 1, 23});

    //World viewed by the camera
    BeginMode2D(camera);

    if (showPredictedTrajectories)
        drawTrajectories();

    for (auto e: celestialBodies) {
        e->render();
    }
    if (anyBodySelected)
        infoText(Vector2Add(posSelectedPlanet, {radiusSelectedPlanet, radiusSelectedPlanet}), 20);

    EndMode2D();

    guiUpdateRender();

    DrawText(celestialBodies[planetIndex]->getName(),
             GetScreenWidth() / 2 - GetTextWidth(celestialBodies[planetIndex]->getName()) * 2, 10, 40, WHITE);

    DrawFPS(10, 10);
    EndDrawing();
}

void Game::run() {
    while (!WindowShouldClose()) {
        update(GetFrameTime());
        render();
    }
    CloseWindow();
}

#pragma clang diagnostic pop