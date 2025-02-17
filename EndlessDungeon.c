#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "raylib.h"
#include "raymath.h"

int main(){

    SetTargetFPS(60);
    InitWindow(800, 600, "Endless Dungeon");

    while(!WindowShouldClose()){

        BeginDrawing();
        ClearBackground(BLACK);

        EndDrawing();

    }
    CloseWindow();
}