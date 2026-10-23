#include "raylib.h"
#include "levels.h"
#include <stdio.h>
#include <math.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define TILE_PADDING 10

#define BASE_SPEED      800.0f

typedef struct Player {
    Vector2 pos;
    float radius;
} Player;

typedef enum GameState {
	STATE_MENU,
	STATE_PLAYING,
	STATE_PAUSED,
	STATE_FINISHED
} GameState;

int main(void) {
    LevelData *level = &g_level;
	float currentSpeed = BASE_SPEED;
	GameState currentState = STATE_PLAYING;
	double startTime = 0.0;
    double endTime   = 0.0;

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Frosh");
    startTime = GetTime();
	InitAudioDevice();
    SetTargetFPS(60);

    Music music = LoadMusicStream("../Music/pond.wav");
    SetMusicVolume(music, 0.7f);
    PlayMusicStream(music);

    Texture2D IceTex = LoadTexture("../Textures/ice.png");
    Texture2D SpongeTex = LoadTexture("../Textures/sponge.png");
    Texture2D DryMudTex = LoadTexture("../Textures/dried-mud.png");

    Camera2D camera = {
        .offset = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f },
        .rotation = 0.0f,
        .zoom = 1.0f
    };

    Player player = {
        .pos = { 0.0f, -20.0f },
        .radius = 20.0f
    };

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        camera.target = player.pos;
		if (IsKeyPressed(KEY_F)) ToggleFullscreen();

		// Change the state of the game
		if (IsKeyPressed(KEY_P)) currentState = STATE_PAUSED;
		if (IsKeyPressed(KEY_U)) currentState = STATE_PLAYING;
		if (IsKeyPressed(KEY_R)) currentState = STATE_MENU;
		
		if (currentState == STATE_PLAYING) {
			Vector2 input = { 0.0f, 0.0f };
			if (IsKeyDown(KEY_L)) input.x += 1.0f;
			if (IsKeyDown(KEY_H))  input.x -= 1.0f;
			if (IsKeyDown(KEY_K))   input.y -= 1.0f;
			if (IsKeyDown(KEY_J))  input.y += 1.0f;
			
		bool holdingG = IsKeyDown(KEY_G);
		bool releasedG = IsKeyReleased(KEY_G);

		if (holdingG) {
		currentSpeed = BASE_SPEED / 2;
		player.radius = 10.0f;
		} else {  
		if (releasedG) currentSpeed = BASE_SPEED;
		if (player.radius < 20.0f) {
			player.radius = 20.0f;
			bool nowColliding = false;
			for (int i = 0; i < level->blockCount; i++) {
			if (CheckCollisionCircleRec(player.pos, player.radius, level->blocks[i])) {
			nowColliding = true;
			break;
			}
			}
			if (nowColliding) {
			player.radius = 10.0f;
			}
		}
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
			currentSpeed -= 50.0f;
			}

			// The Finish Line!
			if (currentState != STATE_FINISHED && CheckCollisionCircleRec(player.pos, player.radius, level->finishLine)) {
				currentState = STATE_FINISHED;
				endTime = GetTime();
			}
		}
	}
	

	// Render
        BeginDrawing();
	    if (currentState == STATE_MENU) {
			ClearBackground(WHITE);
			DrawText("Press U to start", 100, 200, 30, BLACK);
		}
		
		if (currentState == STATE_PLAYING) {
		ClearBackground(DARKBLUE);  
	    float offsetX = fmod(player.pos.x * 0.05f, IceTex.width);
	    float offsetY = fmod(player.pos.y * 0.05f, IceTex.height);
	    int cols = (int)ceil((SCREEN_WIDTH + IceTex.width * 2) / IceTex.width);
	    int rows = (int)ceil((SCREEN_HEIGHT + IceTex.height * 2) / IceTex.height);

	    for (int row = -1; row < rows + 1; row++) {
	      for (int col = -1; col < cols + 1; col++) {
		int x = (int)(offsetX + col * IceTex.width) - TILE_PADDING;
		int y = (int)(offsetY + row * IceTex.height) - TILE_PADDING;
		  DrawTexture(IceTex, x, y, WHITE);
                }
            }

	    BeginMode2D(camera);

	        DrawCircleV(player.pos, player.radius, RED);
			DrawRectangle(level->finishLine.x, level->finishLine.y, level->finishLine.width, level->finishLine.height, ORANGE);
			
			float diameter = level->sponge.radius * 2;
			Rectangle spongeRec = {level->sponge.pos.x - level->sponge.radius, level->sponge.pos.y - level->sponge.radius, diameter, diameter};
			Rectangle spongeTextureSize = {0, 0, (float)SpongeTex.width, (float)SpongeTex.height};
			DrawTexturePro(SpongeTex, spongeTextureSize, spongeRec, (Vector2){ 0, 0 }, 0.0f, WHITE);
			
			Rectangle DryMudTextureSize = {0, 0, (float)DryMudTex.width, (float)DryMudTex.height};
			for (int i = 0; i < level->blockCount; i++) {
		  	DrawTexturePro(DryMudTex, DryMudTextureSize, level->blocks[i], (Vector2){ 0, 0 },  0.0f, WHITE);
            }
    	EndMode2D();
	    
            // Speedometer
            char speedText[32];
            snprintf(speedText, sizeof(speedText), "Speed: %.0f", currentSpeed);
            DrawText(speedText, 10, 10, 20, WHITE);
	    }

		if (currentState == STATE_PAUSED) {
	      DrawRectangle( 100.0f, 100.0f, 600.0f, 400.0f, WHITE);
	      DrawText("Press U to unpause", 300, 200, 20, BLACK);
	    }
		
		// Print the time when hitting the finish line
		if (currentState == STATE_FINISHED) {
		double timeTaken = endTime - startTime;
		char timePrint[64];
		snprintf(timePrint, sizeof(timePrint), "TIME: %.2f seconds", timeTaken);

		int textWidth = MeasureText(timePrint, 40);
		DrawRectangle(0, SCREEN_HEIGHT / 2 - 55, SCREEN_WIDTH, 110, Fade(BLACK, 0.65f));
		DrawText(timePrint, SCREEN_WIDTH / 2 - textWidth / 2, SCREEN_HEIGHT / 2 - 45, 40, WHITE);
		}
	EndDrawing();
        UpdateMusicStream(music);	
    }
    
    UnloadTexture(IceTex);
    UnloadTexture(SpongeTex);
    UnloadTexture(DryMudTex);
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}