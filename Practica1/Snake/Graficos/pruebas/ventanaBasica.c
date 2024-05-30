#include "raylib.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeigth = 450;

    InitWindow(screenWidth, screenHeigth, "Ventana basica usando raylib");

    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Esta es tu primer partalla de raylib", 190, 200, 20, LIGHTGRAY);
        EndDrawing();        
    }

    CloseWindow();

    return 0;
}