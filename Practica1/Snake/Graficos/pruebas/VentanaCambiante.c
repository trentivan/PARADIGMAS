#include "raylib.h"

int main(void)
{

    const int screenWidth = 800;
    const int screenHeight = 450;

    typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING} GameScreen;

    GameScreen currentScreen = TITLE;

    int framesCounter = 0;

    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        switch(currentScreen)
        {
            case LOGO:
            {
                framesCounter++;
                if(framesCounter > 120)
                    currentScreen = TITLE;
            } break;
            case TITLE:
            {
                if(KeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                    currentScreen = GAMEPLAY;
            } break;
            case GAMEPLAY:
            {
                if(IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                    currenstScreen = ENDING;
            } break;
            case ENDING:
            {
                if(IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                    currentScreen = ENDING;
            } break;
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            switch(currentScreen)
            {
                case LOGO:
                {
                    DrawTExt("Logo en pantalla", 20, 20, 40, LIGHTGRAY);
                    DrawText("Espera 2 segundos...", 290, 220, 20, GRAY);
                } break;
                case TITLE:
                {
                    DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
                    DrawText("Titulo de pantalla", 20, 20, 40, DARKGREEN);
                    DrawText("Presiona ENTER o TAP para cambiar pantalla", 1390, 220, 20, MAROON);
                } break;
                case GAMEPLAY:
                {
                    DrawRectangle(0, 0)
                }
            }
    }
    
}
