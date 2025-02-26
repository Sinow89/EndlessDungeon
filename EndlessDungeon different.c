// Created 2025-02-19 by Christoffer Rozenbachs

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "raylib.h"
#include "raymath.h"

#define MAP_HEIGHT 45 // 900/20 = 45, files the whole screen
#define MAP_WIDTH 90 // 1800/20 = 90

int key_bindings[5];
const int screen_height = MAP_HEIGHT*20;
const int screen_width = MAP_WIDTH*20;
int n_rooms = 1; // Actual number of rooms
int key = 0;

typedef enum tiles_type{
    FLOOR,
    WALL,
    GOAL,
    DOOR,
    TREASURE,
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

typedef struct room_t{
  int height; 
  int width;
  Vector2 position;
  Vector2 center;
} room_t;

tiles_t tiles[MAP_HEIGHT][MAP_WIDTH];
player_t player = {960, 200, 20, 20, 5, 5, 100, 0};
room_t rooms[15]; // Maximum number of rooms

void create_tiles(){
    for (int y = 0; y < MAP_HEIGHT; y++){
        for (int x = 0; x < MAP_WIDTH; x++){
            tiles[y][x].position.y = y * 20;
            tiles[y][x].position.x = x * 20;
            tiles[y][x].type = WALL;
        }
    }
};

room_t create_room(int y, int x, int height, int width){
    room_t new_room;

    new_room.position.y = y;
    new_room.position.x = x;
    new_room.height = height;
    new_room.width = width;
    new_room.center.y = y + (int)(height * 0.5);
    new_room.center.x = x + (int)(width * 0.5);

    return new_room;
}

void add_room(room_t room) {
    for (int y = room.position.y; y < room.position.y + room.height; y++) {
        for (int x = room.position.x; x < room.position.x + room.width; x++) {
            DrawRectangle(tiles[y][x].position.x, tiles[y][x].position.y, 20, 20, GRAY);
            tiles[y][x].type = FLOOR;
        }
    }
}

void connect_room_centers(Vector2 centerOne, Vector2 centerTwo) {
    Vector2 temp = centerOne;

    while (true) {
        if ((int)temp.x != (int)centerTwo.x) {
            temp.x += (centerTwo.x > temp.x) ? 1 : -1;
        } else if ((int)temp.y != (int)centerTwo.y) {
            temp.y += (centerTwo.y > temp.y) ? 1 : -1;
        } else {
            break;
        }

        tiles[(int)temp.y][(int)temp.x].type = FLOOR;
        DrawRectangle(temp.x * 20, temp.y * 20, 20, 20, GREEN);

    }
}
bool room_overlaps(room_t* rooms, int rooms_counter, int y, int x, int height, int width) {
    for (int i = 0; i < rooms_counter; i++) {
      if (x >= rooms[i].position.x + rooms[i].width || rooms[i].position.x >= x + width) {
        continue;
      }
      if (y + height <= rooms[i].position.y || rooms[i].position.y + rooms[i].height <= y){
        continue;
      }
      return true;
    }
    return false;
  }

void create_random_room(){
    n_rooms = (rand() % 11) + 5; // 5 to 15 rooms
    int rooms_counter = 0;
    for (int i = 0; i < n_rooms; i++) {
        int y = (rand() % (MAP_HEIGHT - 10)) + 1;
        int x = (rand() % (MAP_WIDTH - 20)) + 1;
        int height = (rand() % 7) + 3;
        int width = (rand() % 15) + 5;

        rooms[i] = create_room(y, x, height, width);
        add_room(rooms[i]); // This sets tiles[y][x].type = FLOOR
        if (!room_overlaps(rooms, rooms_counter, y, x, height, width))
        {
          rooms[rooms_counter] = create_room(y, x, height, width);
          add_room(rooms[rooms_counter]);
          rooms_counter++;
        }
    }

    for (int i = 0; i < n_rooms - 1; i++) {
        connect_room_centers(rooms[i].center, rooms[i + 1].center);
    }
    player.position.y = rooms[0].center.y * 20;
    player.position.x = rooms[0].center.x * 20;
};

void move_player(Vector2 direction) {
    Vector2 new_position = {player.position.x + direction.x, player.position.y + direction.y };

    int tile_x = new_position.x * 0.05;
    int tile_y = new_position.y * 0.05;

    if (tile_x < 0 || tile_x >= MAP_WIDTH || tile_y < 0 || tile_y >= MAP_HEIGHT) {
        return;
    }
    if (tiles[tile_y][tile_x].type == FLOOR) {
        player.position = new_position;
    }

    if (tiles[tile_y][tile_x].type == GOAL) {
        player.position = new_position;
    }

    if (tiles[tile_y][tile_x].type == TREASURE) {
        player.position = new_position;
    }

    if (tiles[tile_y][tile_x].type == WALL) {
        return;
    }
    if (tiles[tile_y][tile_x].type == DOOR) {
        return;
    }
}

bool player_can_open() {
    int tile_x = player.position.x / 20;
    int tile_y = player.position.y / 20;

    if (tiles[tile_y][tile_x].type == GOAL) {
        return true;
    }
    return false;
}


bool player_can_collect() {
    int tile_x = player.position.x / 20;
    int tile_y = player.position.y / 20;

    if (tiles[tile_y][tile_x].type == TREASURE) {
        return true;
    }
    return false;
}

void draw_tiles(){
    for (int y = 0; y < MAP_HEIGHT; y++){
        for (int x = 0; x < MAP_WIDTH; x++){
            DrawRectangle(tiles[y][x].position.x, tiles[y][x].position.y, 20, 20, BLACK);
        }
    }
};

void draw_random_room(){
    for (int i = 0; i < n_rooms; i++) {
        add_room(rooms[i]);
    }
    
    for (int i = 0; i < n_rooms - 1; i++) {
        connect_room_centers(rooms[i].center, rooms[i + 1].center);
    }
    tiles[(int)rooms[1].center.y][(int)rooms[1].center.x].type = GOAL; 
    DrawRectangle((rooms[1].center.x * 20), (rooms[1].center.y * 20), 20, 20, RED);

    tiles[(int)rooms[2].position.y][(int)rooms[2].position.x].type = TREASURE; 
    DrawRectangle((rooms[2].position.x * 20), (rooms[2].position.y * 20), 20, 20, BLUE);
};


/*-------------------------------------------------------*/
/*----------------------MAIN-----------------------------*/
/*-------------------------------------------------------*/


int main(){

    create_tiles();
    srand(time(NULL));
    
    key_bindings[UP] = KEY_W;
    key_bindings[DOWN] = KEY_S;
    key_bindings[LEFT] = KEY_A;
    key_bindings[RIGHT] = KEY_D;
    key_bindings[OPEN] = KEY_E;

    GameScreen current_screen = GAMEPLAY;

    int frames_counter = 0; 

    SetTargetFPS(60);
    InitWindow(screen_width, screen_height, "Endless Dungeon");
    create_random_room();
    
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
            create_random_room();
            create_tiles();
        }

        if (IsKeyPressed(key_bindings[OPEN]) && player_can_collect) {
            key++;
        }

        if (IsKeyPressed(KEY_V)) {
            key++;
         }

        if (IsKeyPressed(KEY_R)) {
            create_tiles();
            create_random_room();
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
                    draw_random_room();

                    Rectangle player_rec = {player.position.x, player.position.y, player.size.x, player.size.y};
                    DrawRectangleRec(player_rec, WHITE);
                    // DrawText("GAMEPLAY SCREEN", 430, 20, 30, MAROON);
                    // DrawText("PRESS Q to ENDING SCREEN", 430, 220, 20, MAROON);

                    char keyText[20];
                    sprintf(keyText, "Keys: %d", key);
                    DrawText(keyText, 20, 20, 30, WHITE);

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