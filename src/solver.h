#ifndef solver_h
#define solver_h

#include "vec2.h"
#include "spatialGrid.h"
#include "particle.h"
#include "raylib.h"
#include <vector>
#include <cmath>
#include <span>
#include <algorithm>
#include <iostream>

class Solver
{
private:
    std::vector<Particle> particles;
    SpatialGrid grid;
    Vec2 gravity;
    float screenWidth;
    float screenHeight;
    float particleRadius;

public:
    Solver(float width, float height, float radius, Vec2 gravity)
        : grid(width, height, radius),
          gravity(gravity),
          screenWidth(width),
          screenHeight(height),
          particleRadius(radius)
    {
        particles.reserve(10000);
        grid.clear();
    }

    void addParticle(Vec2 pos, float radius, Color color = WHITE)
    {
        particles.push_back(Particle{pos, pos, std::min(particleRadius, radius), Vec2{}, color});
    }

    void update(float dt, int subSteps = 8)
    {
        // add gravity
        for (size_t s = 0; s < subSteps; s++)
        {
            const float subDt = dt / static_cast<float>(subSteps);
            for (Particle &p : particles)
            {
                p.acceleration = gravity;
                p.update(subDt);
            }

            // grid init
            grid.clear();
            for (size_t i = 0; i < particles.size(); i++)
            {
                grid.insert(static_cast<int>(i), particles[i].pos);
            }

            for (size_t i = 0; i < particles.size(); i++)
            {
                std::pair<int, int> cpos = grid.getCellCoords(particles[i].pos);
                for (int dx = -1; dx <= 1; dx++)
                {
                    for (int dy = -1; dy <= 1; dy++)
                    {
                        int nx = cpos.first + dx;
                        int ny = cpos.second + dy;

                        if (nx < 0 || nx >= grid.cols || ny < 0 || ny >= grid.rows)
                            continue;

                        int neighbourCell = nx + ny * grid.cols;

                        for (int j : grid.getCellParticles(neighbourCell))
                        {
                            if (j <= static_cast<int>(i))
                                continue;
                            Vec2 axis = particles[i].pos - particles[j].pos; // center
                            float distSq = axis.length_squared();
                            // not using sqrt unless it's actually necessary
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
                }
            }
            for (Particle &p : particles)
            {
                checkWalls(p);
            }
        }
    }

    void checkWalls(Particle &p)
    {
        // optimise later and add damping
        if (p.pos.y > screenHeight - p.radius)
        {
            p.pos.y = screenHeight - p.radius;
        }
        if (p.pos.x > screenWidth - p.radius)
        {
            p.pos.x = screenWidth - p.radius;
        }
        if (p.pos.x < p.radius)
        {
            p.pos.x = p.radius;
        }
        if (p.pos.y < p.radius)
        {
            p.pos.y = p.radius;
        }
    }

    std::span<const Particle> getParticles() const { return particles; }
};

#endif