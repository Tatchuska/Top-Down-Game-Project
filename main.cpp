#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>

int main()
{
    // ARRAY FOR THE WINDOWS DIMENSIONS
    const int windowWidht{384};
    const int windowHeight{384};
    // INITIALIZE THE WINDOW
    InitWindow(windowWidht, windowHeight, "Tatchuska's Top Down");

    Texture2D map = LoadTexture("nature_tileset/map.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0f};

    Character knight{windowWidht, windowHeight};

    Prop props[6]{
        Prop{Vector2{800.f, 300.f}, LoadTexture("nature_tileset/Rock.png")},
        Prop{Vector2{2300.f, 300.f}, LoadTexture("nature_tileset/Bush.png")},
        Prop{Vector2{2600.f, 2300.f}, LoadTexture("nature_tileset/Rock.png")},
        Prop{Vector2{600.f, 2700.f}, LoadTexture("nature_tileset/bush.png")},
        Prop{Vector2{1500.f, 1600.f}, LoadTexture("nature_tileset/Sign.png")},
        Prop{Vector2{400.f, 700.f}, LoadTexture("nature_tileset/Log.png")}};

    Enemy goblin{
        Vector2{1500.f, 1300.f},
        LoadTexture("characters/goblin_idle_spritesheet.png"),
        LoadTexture("characters/goblin_run_spritesheet.png")};

    Enemy slime{
        Vector2{2500.f, 2700.f},
        LoadTexture("characters/slime_idle_spritesheet.png"),
        LoadTexture("characters/slime_run_spritesheet.png")};

    Enemy *enemies[]{
        &goblin,
        &slime};

    for (auto enemy : enemies)
    {
        enemy->setTarget(&knight);
    }

    goblin.setTarget(&knight);

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);

        // DRAW THE MAP
        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);

        // DRAW THE PROPS
        for (auto prop : props)
        {
            prop.Render(knight.getWorldPos());
        }

        if (!knight.getAlive()) // the character is not alive
        {
            DrawText("Game Over!", 55.f, 45.f, 40, RED);
            EndDrawing();
            continue;
        }
        else // character is alive
        {
            std::string knightsHealth = "Health: ";
            knightsHealth.append(std::to_string(knight.getHealth()), 0, 5);
            DrawText(knightsHealth.c_str(), 55.f, 45.f, 40, RED);
        }

        knight.tick(GetFrameTime());
        // CHECK MAP BOUNDS
        if (knight.getWorldPos().x < 0.f ||
            knight.getWorldPos().y < 0.f ||
            knight.getWorldPos().x + windowWidht > map.width * mapScale ||
            knight.getWorldPos().y + windowHeight > map.height * mapScale)
        {
            knight.undoMovement();
        }

        // CHECK PROP COLLISIONS
        for (auto prop : props)
        {
            if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.getCollisionRec()))
            {
                knight.undoMovement();
            }
        }

        for (auto enemy : enemies)
        {
            enemy->tick(GetFrameTime());
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            for (auto enemy : enemies)
            {
                if (CheckCollisionRecs(enemy->getCollisionRec(), knight.getWeaponCollisionRec()))
                {
                    enemy->setAlive(false);
                }
            }
        }

        EndDrawing();
    }

    UnloadTexture(map);
    CloseWindow();
}