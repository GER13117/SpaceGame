//
// Created by Okke on 06.05.2022.
//

//TODO: Easier possibilities / UI for placing moons around planets
//TODO: Cleanup (nested ifs etc)
//TODO: Tooltips
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"

#include "include/Game.h"

#define RAYGUI_IMPLEMENTATION

#include "include/cyber.h"

void Game::initWindow() {
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(1920, 1080, "Space Game");
    SetTargetFPS(60);
    int monitor = GetCurrentMonitor();

    SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
    ToggleFullscreen();
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


    setNewCelestialBodies();
}

void Game::initRenderElements() {
    camera = {0};
    camera.target = {0.f, 0.f};
    camera.offset = {(float) GetScreenWidth() / 2.0f, (float) GetScreenHeight() / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    bloomShader = LoadShader(0, "../resources/shader/bloom.glsl");
    target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
}

void Game::initVariables() {
    numSteps = 8000;
    physicsTime = 1.F / 60.F;
    sideGuiWidth = 180;
}


Game::Game() {
    initVariables();
    initWindow();
    initRenderElements();
    trajectories = new Trajectories(numSteps);
    initCelestialBodies();
    GuiLoadStyleCyber();


}

Game::~Game() {
    clearAllCelestialBodies();
}

float Game::startVel(float centralSurfaceGravity, float centralBodyRadius, float orbitDistance) {
    if (orbitDistance < 0)
        return -sqrt(centralSurfaceGravity * centralBodyRadius * centralBodyRadius / -orbitDistance);
    return sqrt(centralSurfaceGravity * centralBodyRadius * centralBodyRadius / orbitDistance);
}

void Game::resetSolarSystem() {
    clearAllCelestialBodies();

    initCelestialBodies();

    pauseGame = true;

    this->trajectories->update(physicsTime, planetIndex);
}

void Game::editSolarSystem() {
    pauseGame = true;
    //TODO: Implement CelestialBody Constructor that calculates the velocity using the angle of the planet
    if (IsMouseButtonPressed(0) //Left mouse
        && !CheckCollisionPointRec(GetMousePosition(),
                                   {0, 0, 300, static_cast<float>(GetScreenHeight())}) //Inside GUI ?
        && !std::any_of(celestialBodies.begin(), celestialBodies.end(),
                        [this](CelestialBody *body) {
                            return CheckCollisionPointCircle(GetMousePosition(),
                                                             GetWorldToScreen2D(body->getPosition(), camera),
                                                             body->getRadius() *
                                                             camera.zoom); //Cursor on top of other planet
                        })) {
        celestialBodies.push_back(
                new CelestialBody(10.F, 20.F,
                                  GetScreenToWorld2D(GetMousePosition(), camera),
                                  {0, 0}, GREEN,
                                  "Earth"));
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
                    {10, 300, sideGuiWidth, sideGuiWidth},
                    e->getColor()));

            e->setRadius(GuiSlider({10, 300 + 220, sideGuiWidth, 30},
                                   "", TextFormat("radius: %0.1f", e->getRadius()),
                                   e->getRadius(), 1, 200));
            e->setSurfaceGravity(GuiSlider({10, 300 + 220 + 40, sideGuiWidth, 30},
                                           "", TextFormat("gravity: %0.1f", e->getSurfaceGravity()),
                                           e->getSurfaceGravity(), 1, 1000));
            e->recalculateMass();

            e->setVVelocity(
                    {GuiSlider({10, 300 + 220 + 40 + 40 + 40, sideGuiWidth, 30},
                               "", TextFormat("x-velocity: %0.3F", e->getVVelocity().x),
                               e->getVVelocity().x, -200.F, 180),
                     GuiSlider({10, 300 + 220 + 40 + 40 + 40 + 40, sideGuiWidth, 30},
                               "", TextFormat("y-velocity: %0.3F", e->getVVelocity().y),
                               e->getVVelocity().y, -200.F, 200)});

            if (GuiButton({10, 300 + 220 + 40 + 40 + 40 + 40 + 40, sideGuiWidth / 2, 30}, "Delete Body")) {
                auto it = std::find(celestialBodies.begin(), celestialBodies.end(), e);
                celestialBodies.erase(it);
            }
        }
    }

    setNewCelestialBodies();
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
    if (GetMouseWheelMove() < 0 && camera.zoom > 0.1F)
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
        allowEdit = false;
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

    if (GuiButton({10, 10, 30, 30}, GuiIconText(RICON_PLAYER_PREVIOUS, "")) && timeWarp > 1)
        timeWarp--;
    if (GuiButton({10 + 40 + 40, 10, 30, 30}, GuiIconText(RICON_PLAYER_NEXT, "")))
        timeWarp++;

    if (GuiButton({10 + 40, 10, 30, 30},
                  pauseGame ? GuiIconText(RICON_PLAYER_PLAY, "") : GuiIconText(RICON_PLAYER_PAUSE,
                                                                               ""))) { //TODO: Better way?
        allowEdit = false;
        pauseGame = !pauseGame;
    }

    showPredictedTrajectories = GuiToggle({10, 50, 120, 30}, "show trajectories", showPredictedTrajectories);
    if (GuiButton({10, 50 + 40, 59, 30}, "default")) {
        resetSolarSystem();
        allowEdit = true;
    }
    if (GuiButton({72, 50 + 40, 59, 30}, "clear")) {
        clearAllCelestialBodies();
        celestialBodies.push_back(new CelestialBody(sunGravity, sunRadius, GOLD, "Sun"));
        allowEdit = true;
    }
    if (allowEdit) {
        editSystem = GuiToggle({10, 50 + 40 + 40, 120, 30}, "edit solar system", editSystem);
        if (editSystem)
            editSolarSystem();
    }
}

void Game::inWorldInfoText(Vector2 pos, float font_size) {
    //on planet
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

void Game::onScreenText() {
    //Top
    DrawText(celestialBodies[planetIndex]->getName(),
             GetScreenWidth() / 2 - GetTextWidth(celestialBodies[planetIndex]->getName()) * 2, 10, 40, WHITE);

    DrawFPS(GetScreenWidth() - 80, 10);
}


void Game::update(const float &dt) {

    frameCounter++;
    anyBodySelected = false;
    if (showPredictedTrajectories && frameCounter % 2 == 0) {
        this->trajectories->update(dt, planetIndex);
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
    //Rendertexture for shader
    BeginTextureMode(target);
    ClearBackground({0, 1, 23});
    //World viewed by the camera
    BeginMode2D(camera);
    if (showPredictedTrajectories)
        this->trajectories->render();

    for (auto e: celestialBodies) {
        e->render();
    }
    EndMode2D();
    EndTextureMode();

    BeginDrawing();
    ClearBackground({0, 1, 23});

    BeginShaderMode(bloomShader);
    DrawTextureRec(target.texture,
                   {0, 0, (float) target.texture.width, (float) -target.texture.height},
                   {0, 0}, WHITE);
    EndShaderMode();

    BeginMode2D(camera);
    if (anyBodySelected)
        inWorldInfoText(Vector2Add(posSelectedPlanet, {radiusSelectedPlanet, radiusSelectedPlanet}), 40);
    EndMode2D();

    guiUpdateRender();
    onScreenText();
    EndDrawing();
}

void Game::run() {
    while (!WindowShouldClose()) {
        update(physicsTime);
        render();
    }
    UnloadShader(bloomShader);
    UnloadRenderTexture(target);
    CloseWindow();
}

void Game::setNewCelestialBodies() {
    this->trajectories->setCelestialBodies(celestialBodies);
    for (auto e: celestialBodies) {
        e->setOtherCelestialBodies(celestialBodies);
    }
}

void Game::clearAllCelestialBodies() {
    for (auto e: celestialBodies) {
        delete e;
    }
    celestialBodies.clear();
}

#pragma clang diagnostic pop
