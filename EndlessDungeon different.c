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
#define TILE_HEIGHT 8
#define TILE_WIDTH 8
#define MAX_TEXTURES 1

int key_bindings[5];
const int screen_height = MAP_HEIGHT*20;
const int screen_width = MAP_WIDTH*20;
int n_rooms = 1; // Actual number of rooms
int key = 0;
int amount_key = 1;

typedef enum tiles_type{
    FLOOR,
    WALL,
    GOAL,
    DOOR,
    TREASURE,
} tiles_type;

typedef enum{
    TEXTURE_TILE_MAP = 0,
} texture_asset;

Texture2D textures[MAX_TEXTURES];

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
        add_room(rooms[i]);
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

void draw_random_room(){
        for (int i = 0; i < n_rooms; i++) {
        add_room(rooms[i]);
    }
    
    for (int i = 0; i < n_rooms - 1; i++) {
        connect_room_centers(rooms[i].center, rooms[i + 1].center);
    }
    tiles[(int)rooms[1].center.y][(int)rooms[1].center.x].type = GOAL; 
    DrawRectangle((rooms[1].center.x * 20), (rooms[1].center.y * 20), 20, 20, RED);

    if(amount_key > 0){
        tiles[(int)rooms[2].position.y][(int)rooms[2].position.x].type = TREASURE; 
        DrawRectangle((rooms[2].position.x * 20), (rooms[2].position.y * 20), 20, 20, BLUE);
    }
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

    GameScreen current_screen = LOGO;

    int frames_counter = 0; 

    SetTargetFPS(60);
    InitWindow(screen_width, screen_height, "Endless Dungeon");
    create_random_room();

    Image image = LoadImage("colored_tilemap_packed.png");
    textures[TEXTURE_TILE_MAP] = LoadTextureFromImage(image);
    
    while(!WindowShouldClose()){

        switch (current_screen)
        {
            case LOGO:
            {
                frames_counter++;    // Count frames
                // Wait for 2 seconds (120 frames) before jumping to TITLE screen
                if (frames_counter > 240)
                {
                    current_screen = TITLE;
                }
            } break;
            case TITLE:
            {
                if (IsKeyPressed(KEY_SPACE))
                {
                    current_screen = GAMEPLAY;
                }
            } break;
            case GAMEPLAY:
            {
                if (IsKeyPressed(KEY_Q))
                {
                    current_screen = ENDING;
                }
            } break;
            case ENDING:
            {
                if (IsKeyPressed(KEY_ENTER))
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

        if (IsKeyPressed(key_bindings[OPEN]) && player_can_open() && key > 0) {
            create_random_room();
            create_tiles();
            amount_key++;
            key--;
        }

        if (IsKeyPressed(key_bindings[OPEN]) && player_can_collect()) {
            key++;
            amount_key = 0;
            tiles[(int)rooms[2].position.y][(int)rooms[2].position.x].type = FLOOR; 
        }

        if (IsKeyPressed(KEY_R)) {
            create_tiles();
            create_random_room();
            amount_key = 0;
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
                    DrawRectangle(0, 0, screen_width, screen_height, BLACK);
                    DrawText("The Endless Dungeon", 400, 200, 100, WHITE);
                    DrawText("by Christoffer Rozenbachs", 800, 350, 20, GRAY);

                } break;
                case TITLE:
                {
                    DrawRectangle(0, 0, screen_width, screen_height, BLACK);
                    DrawText("How to play the game", 400, 150, 50, WHITE);
                    DrawText("You walk with WASD", 400, 250, 20, GRAY);
                    DrawText("To pick up things you press E", 400, 300, 20, GRAY);
                    DrawText("In order the progress you have to pick up the key before", 400, 350, 20, GRAY);
                    DrawText("Press Space to start the game", 600, 600, 20, WHITE);

                } break;
                case GAMEPLAY:
                {
                    player_can_open();
                    draw_random_room();
                    
                    int texture_index_x = 0;
                    int texture_index_y = 0;

                    for (int y = 0; y < MAP_HEIGHT; y++) {
                        for (int x = 0; x < MAP_WIDTH; x++) {
                
                            switch (tiles[y][x].type) {
                                case FLOOR:
                                    texture_index_x = 4;
                                    texture_index_y = 4;
                                    break;
                                case WALL:
                                    texture_index_x = 1;
                                    texture_index_y = 1;
                                    break;
                                case GOAL:
                                    texture_index_x = 4;
                                    texture_index_y = 2;
                                    break;
                                case DOOR:
                                    texture_index_x = 2;
                                    texture_index_y = 4;
                                    break;
                                case TREASURE:
                                    texture_index_x = 10;
                                    texture_index_y = 5;
                                    break;
                                default:
                                    texture_index_x = 4;
                                    texture_index_y = 3;
                                    break;
                            }
                
                            Rectangle source = {
                                (float)texture_index_x * TILE_HEIGHT,
                                (float)texture_index_y * TILE_WIDTH,
                                (float)TILE_WIDTH,
                                (float)TILE_HEIGHT
                            };
                
                            Rectangle dest = {
                                (float)(tiles[y][x].position.x),
                                (float)(tiles[y][x].position.y),
                                20,
                                20
                            };
                
                            Vector2 origin = {0, 0};
                            DrawTexturePro(textures[TEXTURE_TILE_MAP], source, dest, origin, 0.0f, WHITE);
                        }
                    }

                    int player_texture_index_x = 4;
                    int player_texture_index_y = 0;
                    
                    Rectangle player_source = {
                        (float)player_texture_index_x * TILE_WIDTH,
                        (float)player_texture_index_y * TILE_HEIGHT,
                        (float)TILE_WIDTH,
                        (float)TILE_HEIGHT
                    };
                    
                    Rectangle player_dest = {
                        player.position.x,
                        player.position.y,
                        21,
                        20
                    };
                    
                    Vector2 player_origin = {0, 0};
                    DrawTexturePro(textures[TEXTURE_TILE_MAP], player_source, player_dest, player_origin, 0.0f, WHITE);

                    char keyText[20];
                    sprintf(keyText, "Keys: %d", key);
                    DrawText(keyText, 20, 20, 20, WHITE);

                } break;
                case ENDING:
                {
                    DrawRectangle(0, 0, screen_width, screen_height, BLACK);
                    DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                    DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);
                } break;
                default: break;
            }

        EndDrawing();

    }
    for(int i = 0; i < MAX_TEXTURES; i++){
        UnloadTexture(textures[i]);
    }
    CloseWindow();
}