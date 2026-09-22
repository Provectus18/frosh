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
      .pos = { 0.0f, H - 30.0f },
      .size = { W, 100.0f },
    };

    
    Vector2 velocity = { 0.0f, 0.0f };
    const float GRAVITY = 1500.0f;
    const float MOVE_SPEED = 500.0f;
    const float JUMP_FORCE = -600.0f;
    bool grounded = false;


    while (!WindowShouldClose()) {
      float dt = GetFrameTime();
      camera.target = player.pos;

      if (IsKeyPressed(KEY_F)) ToggleFullscreen();


      

      if (IsKeyDown(KEY_RIGHT)) velocity.x = MOVE_SPEED;
      else if (IsKeyDown(KEY_LEFT)) velocity.x = -MOVE_SPEED;
      else velocity.x = 0.0f;


      velocity.y += GRAVITY * dt;

      
      if (IsKeyPressed(KEY_SPACE) && grounded) {
        velocity.y = JUMP_FORCE;
        grounded = false;
      }

      // Store the old position for collision.
      Vector2 oldPos = player.pos;

      
      player.pos.x += velocity.x * dt;
      player.pos.y += velocity.y * dt;

      
      Rectangle platformRect = {
        platform.pos.x, platform.pos.y,
        platform.size.x, platform.size.y
      };

      if (CheckCollisionCircleRec(player.pos, player.radius, platformRect)) {
        // We hit the platform — figure out which side
        // Calculate where we'd be without collision
        Vector2 projectedPos = {
          oldPos.x + velocity.x * dt,
          oldPos.y + velocity.y * dt
        };

        // Check if we were falling onto the platform (landing)
        if (velocity.y > 0 && oldPos.y + player.radius <= platform.pos.y) {
          // Landing on top
          player.pos.y = platform.pos.y - player.radius;
          velocity.y = 0.0f;
          grounded = true;
        }
        // Otherwise, we hit a side or bottom — just revert position
        else {
          player.pos = oldPos;
          velocity.y = 0.0f;             // cancel vertical momentum on wall hit
        }
      } else {
        grounded = false;
      }

      // Keep player inside window bounds
      if (player.pos.x < player.radius) {
        player.pos.x = player.radius;
        velocity.x = 0.0f;
      }
      if (player.pos.x > W - player.radius) {
        player.pos.x = W - player.radius;
        velocity.x = 0.0f;
      }
      if (player.pos.y > H - player.radius) {
        player.pos.y = H - player.radius;
        velocity.y = 0.0f;
        grounded = true;
      }
      
      // Display the game
      BeginDrawing();
          ClearBackground(RAYWHITE);
          BeginMode2D(camera); 
          DrawCircleV(player.pos, player.radius, BLUE);
          DrawRectangleV(platform.pos, platform.size, GREEN);
          UpdateMusicStream(music);
      EndDrawing();
    }

    CloseWindow();
    return 0;
}
