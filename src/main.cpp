#include "raylib.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Testowanko");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        DrawFPS(10, 10);
        ClearBackground({28,28,28,255});

        EndDrawing();
    }

    CloseWindow();

    return 0;
}