#ifndef particle_h
#define particle_h

#include "vec2.h"
#include "raylib.h"

class Particle
{
public:
    Vec2 pos;
    Vec2 oldPos;
    float radius;
    Vec2 acceleration;
    Color color = WHITE;

    void update(float dt){
        Vec2 velocity = pos - oldPos;
        oldPos = pos;
        pos = pos + velocity + acceleration * (dt * dt);
        //using Verlet equation
        acceleration = Vec2{0.0f, 0.0f};
    }
};

#endif