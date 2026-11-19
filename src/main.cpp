#include "raylib.h"
#include "vec2.h"
#include "particle.h"
#include "solver.h"
#include <vector>
#include <cmath>
#include <chrono>

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 800;
    const int particleRadius = 2;
    Solver solver = Solver(screenWidth, screenHeight, particleRadius, Vec2{0, 1000.0f});

    InitWindow(screenWidth, screenHeight, "Testowanko");

    SetTargetFPS(60);

    constexpr float dt = 1.0f / 60.0f;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground({28, 28, 28, 255});

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePos = GetMousePosition();
            int r = GetRandomValue(-5, 5);
            solver.addParticle(Vec2{mousePos.x + r, mousePos.y + r}, 2, WHITE);
        }

        static bool autoSpawn = false;
        if (IsKeyPressed(KEY_SPACE))
            autoSpawn = !autoSpawn;

        if (autoSpawn)
        {
            float time = static_cast<float>(GetTime());
            float spawnX = screenWidth * 0.5f + std::sin(time * 2.0f) * 250.0f; // sin to clamp between -1;1
            for (int i = 0; i < 8; ++i)
            {
                float offsetX = (i - 3.5f) * (particleRadius * 2.2f);
                solver.addParticle(Vec2{spawnX + offsetX, 30.0f}, particleRadius, WHITE);
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

        if (IsKeyPressed(KEY_R)) // reset
        {
            solver.clearParticles();
            autoSpawn = false;
        }

        // Physics part
        auto t0 = std::chrono::steady_clock::now();
        solver.update(dt);
        auto t1 = std::chrono::steady_clock::now();

        // draw
        for (const Particle &p : solver.getParticles())
        {
            DrawCircleV({p.pos.x, p.pos.y}, p.radius, p.color);
        }
        auto t2 = std::chrono::steady_clock::now();

        // count time
        float physicsMs = std::chrono::duration<float, std::milli>(t1 - t0).count();
        float drawMs = std::chrono::duration<float, std::milli>(t2 - t1).count();

        DrawRectangle(5, 5, 290, 82, Color{15, 15, 15, 200});
        DrawRectangleLines(5, 5, 290, 82, Color{80, 80, 80, 255});
        DrawFPS(10, 10);
        DrawText(TextFormat("Particles: %zu", solver.getParticles().size()), 110, 10, 20, LIGHTGRAY);
        DrawText(TextFormat("Physics: %.2f ms", physicsMs), 10, 35, 20, SKYBLUE);
        DrawText(TextFormat("Draw:    %.2f ms", drawMs), 10, 60, 20, GOLD);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}