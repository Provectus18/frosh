#include "raylib.h"
#include "levels.h"
#include <stdio.h>
#include <math.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define TILE_PADDING 10

#define BASE_SPEED      800.0f
#define MAX_SPEED        800.0f
#define ACCELERATION     1600.0f
#define DECEL_NORMAL     0.97f
#define DECEL_SLOW       0.92f
#define DECEL_SLOW_SPEED 0.85f

typedef struct Player {
  Vector2 pos;
  float radius;
  float expandHitBox;
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
  GameState currentState = STATE_MENU;
  double startTime = 0.0;
  double endTime = 0.0;
  int healthCount = 3;
  double cooldownTimer = 0.0;
  const float COOLDOWN_DURATION = 1.0f;

  bool inCollision = false;
  Vector2 velocity = { 0.0f, 0.0f };

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Frosh");
  startTime = GetTime();
  InitAudioDevice();
  SetTargetFPS(60);

  Music pondTheme = LoadMusicStream("../Music/pond.wav");
  Music woodTheme = LoadMusicStream("../Music/wood.wav");
  SetMusicVolume(pondTheme, 0.7f);
  SetMusicVolume(pondTheme, 0.7f);
  PlayMusicStream(pondTheme);
  PlayMusicStream(woodTheme);
  
  Texture2D PlayerTex = LoadTexture("../Textures/slingdo.png");
  Texture2D IceTex = LoadTexture("../Textures/ice.png");
  Texture2D SpongeTex = LoadTexture("../Textures/sponge.png");
  Texture2D DryMudTex = LoadTexture("../Textures/dried-mud.png");
  Texture2D FinishLineTex = LoadTexture("../Textures/flag.png");
  
  Camera2D camera = {
    .offset = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f },
    .rotation = 0.0f,
    .zoom = 1.0f
  };

  Player player = {
    .pos = level->startPos,
    .radius = 20.0f,
    .expandHitBox = 20.0f
  };

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    camera.target = player.pos;
    if (IsKeyPressed(KEY_F)) ToggleFullscreen();

    // Change the state of the game
    if (IsKeyPressed(KEY_P)) currentState = STATE_PAUSED;
    if (IsKeyPressed(KEY_S)) {
      currentState = STATE_PLAYING;
      startTime = GetTime();
      velocity = (Vector2){ 0.0f, 0.0f };
    }
    if (IsKeyPressed(KEY_R)) {
      currentState = STATE_MENU;
      player.pos = level->startPos;
      endTime = 0.0;
      healthCount = 3;
    }
		
    if (currentState == STATE_PLAYING) {
      Vector2 input = { 0.0f, 0.0f };
      if (IsKeyDown(KEY_L)) input.x += 1.0f;
      if (IsKeyDown(KEY_H))  input.x -= 1.0f;
      if (IsKeyDown(KEY_K))   input.y -= 1.0f;
      if (IsKeyDown(KEY_J))  input.y += 1.0f;

      bool holdingG = IsKeyDown(KEY_G);
      
      float decel = DECEL_NORMAL;
      float accelMult = 1.5f;
      float maxSpeed = MAX_SPEED;

      if (holdingG) {
        player.radius = 10.0f;
        accelMult = 1.0f;
        maxSpeed = MAX_SPEED / 3.0f * 2.0f;
      } else if (!holdingG && inCollision == false) {  
        player.radius = 20.0f;
      }

      velocity.x += input.x * ACCELERATION * accelMult * dt;
      velocity.y += input.y * ACCELERATION * accelMult * dt;

      Vector2 oldPos = player.pos;
      inCollision = false;

      // We are going to do per axis collision so the player can slide against walls
      player.pos.x += velocity.x * dt;
      for (int i = 0; i < level->blockCount; i++) {
        if (CheckCollisionCircleRec(player.pos, player.radius, level->blocks[i])) {
          player.pos.x = oldPos.x;
          velocity.x = 0.0f;
        } else if (CheckCollisionCircleRec(player.pos, player.expandHitBox, level->blocks[i])) inCollision = true;
      }

      player.pos.y += velocity.y * dt;
      for (int i = 0; i < level->blockCount; i++) {
        if (CheckCollisionCircleRec(player.pos, player.radius, level->blocks[i])) {
          player.pos.y = oldPos.y;
          velocity.y = 0.0f;
	  inCollision = true;
        } else if (CheckCollisionCircleRec(player.pos, player.expandHitBox, level->blocks[i])) inCollision = true;
      }

      // Sponge collision (Slow down)
      for (int i = 0; i < level->spongeCount; i++) {
	if (CheckCollisionCircles(player.pos, player.radius, level->sponge[i].pos, level->sponge[i].radius)) decel = DECEL_SLOW_SPEED;
      }

      velocity.x *= decel;
      velocity.y *= decel;

      float speed = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);
      if (speed > maxSpeed && speed > 0.0f) {
        velocity.x = velocity.x / speed * maxSpeed;
        velocity.y = velocity.y / speed * maxSpeed;
      }

      // Spike collision with cooldown and pushback
      if (cooldownTimer > 0.0) {
        cooldownTimer -= dt;
      }

      bool touchingSpike = false;
      for (int i = 0; i < level->spikeCount; i++) {
        if (CheckCollisionCircleRec(player.pos, player.radius, level->spikes[i])) {
          float pushBack = 20.0f;
          if (fabsf(velocity.x) > fabsf(velocity.y)) {
            player.pos.x += (velocity.x > 0.0f ? -pushBack : pushBack);
          } else {
            player.pos.y += (velocity.y > 0.0f ? -pushBack : pushBack);
          }
          touchingSpike = true;
	  inCollision = true;
          break;
        }
      }

      if (touchingSpike && cooldownTimer <= 0.0) {
        healthCount--;
        if (healthCount < 0) healthCount = 0;
        cooldownTimer = COOLDOWN_DURATION;
        velocity.x *= -1.0f;
        velocity.y *= -1.0f;
      }

      // The Finish Line!
      if (currentState != STATE_FINISHED && CheckCollisionCircleRec(player.pos, player.radius, level->finishLine)) {
        currentState = STATE_FINISHED;
        endTime = GetTime();
      }
    }
	

    // Render
    BeginDrawing();
    switch (currentState) {

    case STATE_MENU:
      ClearBackground(WHITE);
      DrawText("Welcome to Frosh!\n\nPress S to start", 100, 200, 30, BLACK);
      UpdateMusicStream(pondTheme);
      break;

    case STATE_PLAYING:
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

      float playerDiameter = player.radius * 2;
      float scale = (playerDiameter / PlayerTex.width) * 2;
      Rectangle playerRec = {
	player.pos.x - (PlayerTex.width  * scale) / 2,
	player.pos.y - (PlayerTex.height * scale) / 2,
	PlayerTex.width  * scale,
	PlayerTex.height * scale
      };
      Rectangle playerSrcRec = {0, 0, (float)PlayerTex.width, (float)PlayerTex.height};
      DrawTexturePro(PlayerTex, playerSrcRec, playerRec, (Vector2){0, 0}, 0.0f, WHITE);

      Rectangle finishLineSrcRec = {0, 0,  (float)FinishLineTex.width, (float)FinishLineTex.height};
      DrawTexturePro(FinishLineTex, finishLineSrcRec, level->finishLine, (Vector2){ 0, 0 }, 0.0f, WHITE);
      
      for (int i = 0; i < level->spongeCount; i++) {
	float spongeDiameter = level->sponge[i].radius * 2;
	Rectangle spongeRec = {
	  level->sponge[i].pos.x - level->sponge[i].radius,
	  level->sponge[i].pos.y - level->sponge[i].radius,
	  spongeDiameter, spongeDiameter
	};
	Rectangle spongeSrcRec = {0, 0, (float)SpongeTex.width, (float)SpongeTex.height};
	DrawTexturePro(SpongeTex, spongeSrcRec, spongeRec, (Vector2){0, 0}, 0.0f, WHITE);
      }
			
      Rectangle DryMudTextureSize = {0, 0, (float)DryMudTex.width, (float)DryMudTex.height};
      for (int i = 0; i < level->blockCount; i++) {
	DrawTexturePro(DryMudTex, DryMudTextureSize, level->blocks[i], (Vector2){ 0, 0 },  0.0f, WHITE);
      }

      for (int i = 0; i < level->spikeCount; i++) {
	DrawRectangleRec(level->spikes[i], BROWN);
      }
      EndMode2D();
      float actualSpeed = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);
      DrawText(TextFormat("Speed: %.0f", actualSpeed), 10, 10, 20, WHITE);

      Rectangle healthBar = { 150.0f, 10.0f, 40.0f, 20.0f };
      switch(healthCount) {
      case 3:
	DrawRectangleRec(healthBar, GREEN);
	break;
      case 2:
	DrawRectangleRec(healthBar, YELLOW);
	break;
      case 1:
	DrawRectangleRec(healthBar, RED);
	break;
      case 0:
	currentState = STATE_FINISHED;
	break;
      }
      break;

    case STATE_PAUSED:
      DrawRectangle( 100.0f, 100.0f, 600.0f, 400.0f, WHITE);
      DrawText("S to unpause\nR to return to menu", 200, 200, 30, BLACK);
      break;
		  
    case STATE_FINISHED:
      double timeTaken = endTime - startTime;
      DrawRectangle(100.0f, 100.0f, 600.0f, 400.0f, WHITE);
      if (healthCount == 0) DrawText("You have gone too far...\n\nPress R to return to menu", 200, 200, 30, BLACK); // Every game needs an ominous message when you die
      else {
	DrawText(TextFormat("Time taken: %.2f seconds\n\nPress R to return to menu", timeTaken), 200, 200, 30, BLACK);
	UpdateMusicStream(woodTheme);
      }
      break;
    }	  
    EndDrawing();
  }
  UnloadTexture(PlayerTex);
  UnloadTexture(IceTex);
  UnloadTexture(SpongeTex);
  UnloadTexture(DryMudTex);
  UnloadTexture(FinishLineTex);
  UnloadMusicStream(pondTheme);
  UnloadMusicStream(woodTheme);
  CloseAudioDevice();
  CloseWindow();
  return 0;
}
