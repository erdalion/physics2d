#include "raylib.h"
#include "vec2.h"
#include "particle.h"
#include <vector>

int main(void)
{
    std::vector<Particle> particles;
    particles.reserve(10000);
    const int screenWidth = 800;
    const int screenHeight = 450;

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
            for (size_t i = 0; i < 100; i++)
            {
                particles.push_back(Particle{Vec2{mousePos.x, mousePos.y}, Vec2{mousePos.x, mousePos.y}, 2, Vec2{0, 0}});
            }
            
        }

        // Physics part
        constexpr int subSteps = 8;
        constexpr float subDt = dt / static_cast<float>(subSteps);
        // check whether use size_t or normaln int
        for (size_t s = 0; s < subSteps; s++)
        {
            for (size_t i = 0; i < particles.size(); i++)
            {
                particles[i].acceleration = Vec2{0.0f, 1000.0f};
                particles[i].update(subDt);
            }

            for (size_t i = 0; i < particles.size(); i++)
            {
                for (size_t j = i + 1; j < particles.size(); j++)
                {
                    Vec2 axis = particles[i].pos - particles[j].pos; //center
                    float distSq = axis.length_squared(); 
                    //not using sqrt unless it's actually necessary
                    float minDist = particles[i].radius + particles[j].radius;
                    if (distSq < minDist * minDist && distSq > 0.0001f) 
                    {
                        float dist = std::sqrt(distSq);
                        Vec2 n = axis / dist;
                        float overlap = minDist - dist;
                        particles[i].pos += n * (0.5f * overlap);
                        particles[j].pos -= n * (0.5f * overlap);
                    }
                }
            }
            

            for (size_t i = 0; i < particles.size(); i++)
            {
                // particles[i].acceleration = Vec2{0.0f, 1000.0f};
                // particles[i].update(dt);
                if (particles[i].pos.y > screenHeight - particles[i].radius)
                {
                    particles[i].pos.y = screenHeight - particles[i].radius;
                }
                if (particles[i].pos.x > screenWidth - particles[i].radius)
                {
                    particles[i].pos.x = screenWidth - particles[i].radius;
                }
                if (particles[i].pos.x < particles[i].radius)
                {
                    particles[i].pos.x = particles[i].radius;
                }
                if (particles[i].pos.y < particles[i].radius)
                {
                    particles[i].pos.y = particles[i].radius;
                }
            }
        }


        // draw

        for (size_t i = 0; i < particles.size(); i++)
        {
            DrawCircleV({particles[i].pos.x, particles[i].pos.y}, particles[i].radius, particles[i].color);
        }
        
        DrawFPS(10, 10);
        DrawText(TextFormat("Particles: %zu", particles.size()), 10, 35, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}