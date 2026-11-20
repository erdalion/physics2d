#include "raylib.h"
#include "vec2.h"
#include "particle.h"
#include "solver.h"
#include "videoHandler.h"
#include <vector>
#include <cmath>
#include <chrono>

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 400;
    const int particleRadius = 2;
    Solver solver = Solver(screenWidth, screenHeight, particleRadius, Vec2{0, 1000.0f});

    InitWindow(screenWidth, screenHeight, "Testowanko");
    SetTargetFPS(60);

    //Particle texture, cause DrawCircel is slow as hell
    int texSize = particleRadius * 2;
    RenderTexture2D circleTex = LoadRenderTexture(texSize, texSize);
    BeginTextureMode(circleTex);
    ClearBackground(BLANK);
    DrawCircle(particleRadius, particleRadius, static_cast<float>(particleRadius), WHITE);
    EndTextureMode();

    constexpr float dt = 1.0f / 60.0f;
    VideoHandler videoHandler;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground({40, 40, 40, 255});

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePos = GetMousePosition();
            int r = GetRandomValue(-5, 5);
            solver.addParticle(Vec2{mousePos.x + r, mousePos.y + r}, particleRadius, WHITE);
        }

        static bool autoSpawn = false;
        if (IsKeyPressed(KEY_SPACE))
            autoSpawn = !autoSpawn;

        if (autoSpawn)
        {
            for (int i = 0; i < 8 && solver.getParticles().size() < 10000; ++i)
            {
                float spawnX = static_cast<float>(GetRandomValue(particleRadius, screenWidth - particleRadius));
                solver.addParticle(Vec2{spawnX, 10.0f}, particleRadius, WHITE);
            }
        }

        if (IsKeyPressed(KEY_B)) // Stress test: 10k Particles
        {
            float spacing = particleRadius * 2.1f;
            for (float y = 20.0f; y < screenHeight - 20.0f; y += spacing)
            {
                for (float x = 20.0f; x < screenWidth - 20.0f; x += spacing)
                {
                    if (solver.getParticles().size() >= 10000)
                        break;
                    solver.addParticle(Vec2{x, y}, particleRadius, WHITE);
                }
            }
        }

        if (IsKeyPressed(KEY_V)) // Toggle Video Playback
        {
            videoHandler.toggle(dt);
        }

        if (IsKeyPressed(KEY_R)) // reset
        {
            solver.clearParticles();
            autoSpawn = false;
            videoHandler.reset();
        }

        // Physics part
        auto t0 = std::chrono::steady_clock::now();
        solver.update(dt);
        auto t1 = std::chrono::steady_clock::now();

        // Easter Egg
        videoHandler.update(dt);

        // Draw
        for (const Particle &p : solver.getParticles())
        {
            Color color = p.color;
            color = videoHandler.getColor(p.pos.x, p.pos.y, screenWidth, screenHeight, color);
            // DrawCircleV({p.pos.x, p.pos.y}, p.radius, color);
            DrawTextureV(circleTex.texture, {p.pos.x - p.radius, p.pos.y - p.radius}, color);
        }

        auto t2 = std::chrono::steady_clock::now();

        // count time
        float physicsMs = std::chrono::duration<float, std::milli>(t1 - t0).count();
        float drawMs = std::chrono::duration<float, std::milli>(t2 - t1).count();

        DrawRectangle(5, 5, 470, 24, Color{15, 15, 15, 200});
        DrawRectangleLines(5, 5, 470, 24, Color{80, 80, 80, 255});
        DrawText(TextFormat("FPS: %d", GetFPS()), 10, 11, 12, LIGHTGRAY);
        DrawText(TextFormat("Particles: %zu", solver.getParticles().size()), 70, 11, 12, LIGHTGRAY);
        DrawText(TextFormat("Physics: %.2f ms", physicsMs), 175, 11, 12, SKYBLUE);
        DrawText(TextFormat("Draw: %.2f ms", drawMs), 310, 11, 12, GOLD);

        EndDrawing();
    }

    UnloadRenderTexture(circleTex);
    CloseWindow();

    return 0;
}