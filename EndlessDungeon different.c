// Created 2025-02-19 by Christoffer Rozenbachs

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "raylib.h"
#include "raymath.h"

#define MAP_HEIGHT 25
#define MAP_WIDTH 100

int key_bindings[5];
const int screen_width = 1024;
const int screen_height = 768;

typedef enum tiles_type{
    FLOOR,
    WALL,
    DOOR,
} tiles_type;

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

typedef struct tiles_t{
    Vector2 position;
    Vector2 size;
    int type;
} tiles_t;

tiles_t tiles[MAP_HEIGHT][MAP_WIDTH];

void create_tiles(){
    for (int y = 0; y < MAP_HEIGHT; y++){
        for (int x = 0; x < MAP_WIDTH; x++){
            tiles[y][x].position.y = y * 20;
            tiles[y][x].position.x = x * 20;
            tiles[y][x].type = WALL;
        }
    }
    tiles[10][0].type = DOOR;
    tiles[0][10].type = DOOR;
    tiles[19][10].type = DOOR;
    tiles[10][19].type = DOOR;
    tiles[5][5].type = WALL;
};

void draw_tiles(){
    for (int y = 0; y < MAP_HEIGHT; y++){
        for (int x = 0; x < MAP_WIDTH; x++){
            DrawRectangle(tiles[y][x].position.x, tiles[y][x].position.y, 20, 20, BLACK);
        }
    }
};

//Redo so this draws squares that are type walls.
void draw_floor(){
    for (int y = 5; y < 15; y++){
        for (int x = 40; x < 50; x++){
            DrawRectangle(tiles[y][x].position.x, tiles[y][x].position.y, 20, 20, GRAY);
            tiles[y][x].type = FLOOR;
        }
    }
};

void draw_door(){

    DrawRectangle(0, 200, 20, 20, RED);
    DrawRectangle(200, 0, 20, 20, RED);
    DrawRectangle(200, 380, 20, 20, RED);
    DrawRectangle(380, 200, 20, 20, RED);
};

player_t player = {960, 200, 20, 20, 5, 5, 100, 0};

void move_player(Vector2 direction) {
    Vector2 new_position = {player.position.x + direction.x, player.position.y + direction.y };

    int tile_x = new_position.x / 20;
    int tile_y = new_position.y / 20;

    if (tile_x < 0 || tile_x >= MAP_WIDTH || tile_y < 0 || tile_y >= MAP_HEIGHT) {
        return;
    }
    if (tiles[tile_y][tile_x].type == FLOOR) {
        player.position = new_position;
    }

    if (tiles[tile_y][tile_x].type == DOOR) {
        player.position = new_position;
    }

    if (tiles[tile_y][tile_x].type == WALL) {
        return;
    }
}

bool player_can_open() {
    int tile_x = player.position.x / 20;
    int tile_y = player.position.y / 20;

    if (tiles[tile_y][tile_x].type == DOOR) {
        return true;
    }
    return false;
}

int main(){

    create_tiles();
    
    key_bindings[UP] = KEY_W;
    key_bindings[DOWN] = KEY_S;
    key_bindings[LEFT] = KEY_A;
    key_bindings[RIGHT] = KEY_D;
    key_bindings[OPEN] = KEY_E;

    GameScreen current_screen = GAMEPLAY;

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


        if (IsKeyPressed(key_bindings[UP]) || IsKeyPressedRepeat(key_bindings[UP])){
            move_player((Vector2){0, -20});
        } 
    
        if (IsKeyPressed(key_bindings[DOWN]) || IsKeyPressedRepeat(key_bindings[DOWN])) {
            move_player((Vector2){0, 20});
        }
    
        if (IsKeyPressed(key_bindings[LEFT]) || IsKeyPressedRepeat(key_bindings[LEFT])) {
            move_player((Vector2){-20, 0});
        }
    
        if (IsKeyPressed(key_bindings[RIGHT]) || IsKeyPressedRepeat(key_bindings[RIGHT])) {
            move_player((Vector2){20, 0});
        }

        if (IsKeyPressed(key_bindings[OPEN]) && player_can_open()) {
            player.position.x = 200;
            player.position.y = 200;
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
                    player_can_open();
                    draw_tiles();
                    draw_door();
                    draw_floor();
                    DrawRectangle(tiles[5][5].position.x, tiles[5][5].position.y, 20,20, YELLOW);
                    
                    Rectangle player_rec = {player.position.x, player.position.y, player.size.x, player.size.y};
                    DrawRectangleRec(player_rec, WHITE);
                    
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