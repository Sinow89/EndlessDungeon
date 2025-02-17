#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "raylib.h"
#include "raymath.h"

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

    SetTargetFPS(60);
    InitWindow(screen_width, screen_height, "Endless Dungeon");

    while(!WindowShouldClose()){

        BeginDrawing();
        ClearBackground(BLACK);

        EndDrawing();

    }
    CloseWindow();
}