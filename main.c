#include "raylib.h"
#include "levels.h"
#include <stdio.h>
#include <math.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 450
#define TILE_PADDING 10

#define BASE_SPEED      200.0f
#define MAX_SPEED     800.0f
#define ACCEL_RATE     250.0f
#define DECEL_RATE    1200.0f

typedef struct Player {
    Vector2 pos;
    float radius;
} Player;


int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Frosh");
    InitAudioDevice();
    SetTargetFPS(60);

    Music music = LoadMusicStream("./Music/pond.wav");
    SetMusicVolume(music, 0.7f);
    PlayMusicStream(music);

    Texture2D Ice = LoadTexture("./Textures/ice.png");

    LevelData *level = &g_level;

    Camera2D camera = {
        .offset = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f },
        .rotation = 0.0f,
        .zoom = 1.0f
    };

    Player player = {
        .pos = { 0.0f, -20.0f },
        .radius = 15.0f
    };

    float currentSpeed = BASE_SPEED;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        camera.target = player.pos;

        if (IsKeyPressed(KEY_F)) ToggleFullscreen();

        // Read input direction
        Vector2 input = { 0.0f, 0.0f };
        if (IsKeyDown(KEY_RIGHT)) input.x += 1.0f;
        if (IsKeyDown(KEY_LEFT))  input.x -= 1.0f;
        if (IsKeyDown(KEY_UP))   input.y -= 1.0f;
        if (IsKeyDown(KEY_DOWN))  input.y += 1.0f;

        bool isMoving = (input.x != 0.0f || input.y != 0.0f);

        if (isMoving) {
            // Ramp up while held
            currentSpeed += ACCEL_RATE * dt;
            if (currentSpeed > MAX_SPEED) currentSpeed = MAX_SPEED;

            // Normalize diagonal input
            float len = sqrtf(input.x * input.x + input.y * input.y);
            input.x /= len;
            input.y /= len;
        } else {
            // Slow down when released
            currentSpeed -= DECEL_RATE * dt;
            if (currentSpeed < BASE_SPEED) currentSpeed = BASE_SPEED;
        }

        Vector2 velocity = {
            input.x * currentSpeed,
            input.y * currentSpeed
        };

        // Movement with collision
        if (velocity.x != 0.0f || velocity.y != 0.0f) {
            Vector2 oldPos = player.pos;

            // Block collision
            player.pos.x += velocity.x * dt;
	    player.pos.y += velocity.y * dt;
	    for (int i = 0; i < level->blockCount; i++) {
                if (CheckCollisionCircleRec(player.pos, player.radius, level->blocks[i])) {
                    player.pos.x = oldPos.x;
		    player.pos.y = oldPos.y;
		    break;
                }
            }

	    // Sponge collision (Slows down)
	    if (CheckCollisionCircles(player.pos, player.radius, level->sponge.pos, level->sponge.radius)) {
	      currentSpeed = 100.0f;
	    }
        }

        // Render
        BeginDrawing();
	    ClearBackground(DARKBLUE);  
	    float offsetX = fmod(player.pos.x * 0.05f, Ice.width);
	    float offsetY = fmod(player.pos.y * 0.05f, Ice.height);

	    
	    // Calculate how many tiles we need to cover the screen
	    int cols = (int)ceil((SCREEN_WIDTH + Ice.width * 2) / Ice.width);
	    int rows = (int)ceil((SCREEN_HEIGHT + Ice.height * 2) / Ice.height);

	    for (int row = -1; row < rows + 1; row++) {
	      for (int col = -1; col < cols + 1; col++) {
		int x = (int)(offsetX + col * Ice.width) - TILE_PADDING;
		int y = (int)(offsetY + row * Ice.height) - TILE_PADDING;
		  DrawTexture(Ice, x, y, WHITE);
                }
            }


	    BeginMode2D(camera);
                for (int i = 0; i < level->blockCount; i++) {
                    DrawRectangleRec(level->blocks[i], GREEN);
                }
		DrawCircleV(player.pos, player.radius, RED);
		DrawCircleV(level->sponge.pos, level->sponge.radius, YELLOW);
            EndMode2D();

            // Speedometer
            char speedText[32];
            snprintf(speedText, sizeof(speedText), "Speed: %.0f", currentSpeed);
            DrawText(speedText, 10, 10, 20, WHITE);
        EndDrawing();

        UpdateMusicStream(music);
    }
    
    UnloadTexture(Ice);
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
