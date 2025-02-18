// Created 2025-02-17 by Christoffer Rozenbachs

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "raylib.h"
#include "raymath.h"

#define WIDTH 21  // Maze width (must be odd)
#define HEIGHT 21 // Maze height (must be odd)
#define CELL_SIZE 20
#define TILE_SIZE 20

typedef struct {
    int box;
    int goal;
    bool walkable;
} maze_t;

maze_t maze[HEIGHT][WIDTH];

void carve(int x, int y) {
    int dirs[] = {0, 1, 2, 3};
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4, temp = dirs[i];
        dirs[i] = dirs[r];
        dirs[r] = temp;
    }
    for (int i = 0; i < 4; i++) {
        int dx = (dirs[i] == 0) - (dirs[i] == 1);
        int dy = (dirs[i] == 2) - (dirs[i] == 3);
        int nx = x + dx * 2, ny = y + dy * 2;
        if (nx > 0 && nx < WIDTH - 1 && ny > 0 && ny < HEIGHT - 1 && maze[ny][nx].box == '#') {
            maze[ny - dy][nx - dx].box = ' ';
            maze[ny][nx].box = ' ';
            maze[ny - dy][nx - dx].walkable = true;
            maze[ny][nx].walkable = true;
            carve(nx, ny);
        }
    }
}

void generateMaze() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            maze[y][x].box = (x % 2 && y % 2) ? ' ' : '#';
            maze[y][x].walkable = (maze[y][x].box == '#'); 
        }
    }
    carve(1, 1);
}

bool isPositionWalkable(float x, float y) {
    // Convert pixel coordinates to maze array indices
    int mazeX = (int)(x / TILE_SIZE);  // Assuming TILE_SIZE is your tile width
    int mazeY = (int)(y / TILE_SIZE);  // Assuming TILE_SIZE is your tile height
    
    // Check bounds
    if (mazeX < 0 || mazeX >= WIDTH || mazeY < 0 || mazeY >= HEIGHT) {
        return false;
    }
    
    return maze[mazeY][mazeX].walkable;
}

typedef enum GameScreen{ 
    LOGO,
    TITLE,
    GAMEPLAY,
    ENDING
} GameScreen;

typedef enum Controls{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    OPEN
} Controls;

typedef struct player_t{
    Vector2 position;
    Vector2 size;
    Vector2 velocity;
    int hp;
    int key;
} player_t;


int key_bindings[5];

int main(){

    const int screen_width = 800;
    const int screen_height = 600;

    key_bindings[UP] = KEY_W;
    key_bindings[DOWN] = KEY_S;
    key_bindings[LEFT] = KEY_A;
    key_bindings[RIGHT] = KEY_D;
    key_bindings[OPEN] = KEY_E;

    player_t player = {200, 200, 20, 20, 5, 5, 100, 0};


    GameScreen current_screen = LOGO;

    int frames_counter = 0; 

    SetTargetFPS(60);
    InitWindow(screen_width, screen_height, "Endless Dungeon");
    // InitWindow(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, "Maze Generator");
    generateMaze();

    while(!WindowShouldClose()){

        switch (current_screen)
        {
            case LOGO:
            {
                // TODO: Update LOGO screen variables here!
                frames_counter++;    // Count frames
                // Wait for 2 seconds (120 frames) before jumping to TITLE screen
                if (frames_counter > 10)
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


        if (IsKeyPressed(key_bindings[UP]) || IsKeyPressedRepeat(key_bindings[UP])) {
            float newY = player.position.y - 20;
            if (isPositionWalkable(player.position.x, newY)) { 
                player.position.y = newY; 
            }
        }
        
        if (IsKeyPressed(key_bindings[DOWN]) || IsKeyPressedRepeat(key_bindings[DOWN])) {
            float newY = player.position.y + 20;
            if (isPositionWalkable(player.position.x, newY)) {
                player.position.y = newY;
            }
        }
        
        if (IsKeyPressed(key_bindings[LEFT]) || IsKeyPressedRepeat(key_bindings[LEFT])) {
            float newX = player.position.x - 20;
            if (isPositionWalkable(newX, player.position.y)) {
                player.position.x = newX;
            }
        }
        
        if (IsKeyPressed(key_bindings[RIGHT]) || IsKeyPressedRepeat(key_bindings[RIGHT])) {
            float newX = player.position.x + 20;
            if (isPositionWalkable(newX, player.position.y)) {
                player.position.x = newX;
            }
        }
        
        if(IsKeyDown(OPEN)) {

        }

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

                    Rectangle player_rec = {player.position.x, player.position.y, player.size.x, player.size.y};

                    for (int y = 0; y < HEIGHT; y++) {
                        for (int x = 0; x < WIDTH; x++) {
                            if (maze[y][x].box == '#')
                            DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
                        }
                        DrawRectangleRec(player_rec, GRAY);
                    }

                    // TODO: Draw GAMEPLAY screen here!
                    
                    DrawText("GAMEPLAY SCREEN", 430, 20, 30, MAROON);
                    DrawText("PRESS Q to ENDING SCREEN", 430, 220, 20, MAROON);

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