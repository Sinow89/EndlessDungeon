#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "raylib.h"
#include "raymath.h"


// Created 2025-02-17 by Christoffer Rozenbachs

typedef enum GameScreen{ 
    LOGO,
    TITLE,
    GAMEPLAY,
    ENDING
} GameScreen;

int main(){

    const int screen_width = 800;
    const int screen_height = 600;

    GameScreen current_screen = LOGO;

    int frames_counter = 0; 

    SetTargetFPS(60);
    InitWindow(screen_width, screen_height, "Endless Dungeon");

    while(!WindowShouldClose()){

        switch (current_screen)
        {
            case LOGO:
            {
                // TODO: Update LOGO screen variables here!
                frames_counter++;    // Count frames
                // Wait for 2 seconds (120 frames) before jumping to TITLE screen
                if (frames_counter > 120)
                {
                    current_screen = TITLE;
                }
            } break;
            case TITLE:
            {
                // TODO: Update TITLE screen variables here!
                // Press enter to change to GAMEPLAY screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    current_screen = GAMEPLAY;
                }
            } break;
            case GAMEPLAY:
            {
                // TODO: Update GAMEPLAY screen variables here!

                // Press enter to change to ENDING screen
                if (IsKeyPressed(KEY_Q))
                {
                    current_screen = ENDING;
                }
            } break;
            case ENDING:
            {
                // TODO: Update ENDING screen variables here!
                // Press enter to return to TITLE screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    current_screen = TITLE;
                }
            } break;
            default: break;
        }

        /*-------------------------------------------------------*/
        /*----------------------Game-logic-----------------------*/
        /*-------------------------------------------------------*/



        /*-------------------------------------------------------*/
        /*----------------------Controls-------------------------*/
        /*-------------------------------------------------------*/



        /*-------------------------------------------------------*/
        /*----------------------Drawing--------------------------*/
        /*-------------------------------------------------------*/

        BeginDrawing();

            ClearBackground(RAYWHITE);

            switch(current_screen)
            {
                case LOGO:
                {
                    // TODO: Draw LOGO screen here!
                    DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
                    DrawText("WAIT for 2 SECONDS...", 290, 220, 20, GRAY);

                } break;
                case TITLE:
                {
                    // TODO: Draw TITLE screen here!
                    DrawRectangle(0, 0, screen_width, screen_height, GREEN);
                    DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
                    DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);

                } break;
                case GAMEPLAY:
                {
                    // TODO: Draw GAMEPLAY screen here!
                    DrawRectangle(0, 0, screen_width, screen_height, PURPLE);
                    DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
                    DrawText("PRESS Q to ENDING SCREEN", 130, 220, 20, MAROON);

                } break;
                case ENDING:
                {
                    // TODO: Draw ENDING screen here!
                    DrawRectangle(0, 0, screen_width, screen_height, BLUE);
                    DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                    DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);

                } break;
                default: break;
            }

        EndDrawing();

    }
    CloseWindow();
}