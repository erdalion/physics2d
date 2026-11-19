#include "raylib.h"
#include "vec2.h"
#include "particle.h"
#include "solver.h"
#include <vector>
#include <cmath>

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 800;
    const int particleRadius = 2;
    Solver solver = Solver(screenWidth, screenHeight, particleRadius, Vec2{0,1000.0f});

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
            for (size_t i = 0; i < 10; i++)
            {
                int r =GetRandomValue(-5, 5);
                solver.addParticle(Vec2{mousePos.x * r, mousePos.y * r}, 2, WHITE);
            }
            
        }

        // Physics part
        solver.update(dt);

        // draw
        for (const Particle& p : solver.getParticles())
        {
            DrawCircleV({p.pos.x, p.pos.y}, p.radius, p.color);
        }
        
        DrawFPS(10, 10);
        DrawText(TextFormat("Particles: %zu", solver.getParticles().size()), 10, 35, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}