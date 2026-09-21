#include "raylib.h"

struct Player {
  Vector2 pos;
  float radius;
};

struct Platform {
  Vector2 pos;
  Vector2 size;
};
  
int main(void) {
  const int W = 800, H = 450;
  InitWindow(W, H, "Frosh");
  InitAudioDevice();  
  SetTargetFPS(60);

    Music music = LoadMusicStream("pond.wav");
    SetMusicVolume(music, 0.7f);
    PlayMusicStream(music);
    
    Camera2D camera = {
      .offset = (Vector2){ W/2.0f, H/2.0f },
      .rotation = 0.0f,
      .zoom = 1.0f
    };

    struct Player player = {
      .pos = { W/2.0f, H/2.0f },
      .radius = 20.0f
    };

    struct Platform platform = {
      .pos = { 0.0f, H - 20.0f },
      .size = { W, 100.0f },
    };

    while (!WindowShouldClose()) {
      float dt = GetFrameTime();
      camera.target = player.pos;

      if (IsKeyPressed(KEY_F)) ToggleFullscreen();
      
      // Store old position before movement, so that the player clamps
      Vector2 oldPos = player.pos;

      if (IsKeyDown(KEY_RIGHT)) player.pos.x += 5.0f;
      if (IsKeyDown(KEY_LEFT)) player.pos.x -= 5.0f;
      if (IsKeyDown(KEY_SPACE)) player.pos.y -= 8.0f;
      if (IsKeyDown(KEY_DOWN)) player.pos.y += 2.0f;


      Rectangle platformRect = {
        platform.pos.x, platform.pos.y,
        platform.size.x, platform.size.y
      };

      // Check if player collides with platform
      if (CheckCollisionCircleRec(player.pos, player.radius, platformRect)) {
        // Reverts to old position
        player.pos = oldPos;
      }


      // Display the game
      BeginDrawing();
          ClearBackground(RAYWHITE);
          BeginMode2D(camera); 
          DrawCircleV(player.pos, player.radius, BLUE);
          DrawRectangleV(platform.pos, platform.size, DARKGREEN);
          DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, DARKGRAY);
          UpdateMusicStream(music);
      EndDrawing();
    }

    CloseWindow();
    return 0;
}
