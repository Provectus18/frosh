#include "raylib.h"

struct Player {
  Vector2 pos;
  float radius;
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
      .pos = { 0.0f, -20.0f },
      .radius = 20.0f
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

      
      if (IsKeyPressed(KEY_SPACE)) {
        velocity.y = JUMP_FORCE;
      }

      // Store the old position for collision.
      Vector2 oldPos = player.pos;
      
      player.pos.x += velocity.x * dt;
      player.pos.y += velocity.y * dt;

      
      Rectangle greenBlock = {
        0.0f, 0.0f,
        100.0f, 100.0f
      };
      
      // Collision
      player.pos.x += velocity.x * dt;
      if (CheckCollisionCircleRec(player.pos, player.radius, greenBlock)) {
	player.pos.x = oldPos.x;  // Revert the X position when colliding
	velocity.x = 0.0f;
      }

      player.pos.y += velocity.y * dt;
      if (CheckCollisionCircleRec(player.pos, player.radius, greenBlock)) {
	if (velocity.y > 0) {
	  player.pos.y = greenBlock.y - player.radius;
	  grounded = true;
	} else {
	  player.pos.y = greenBlock.y + greenBlock.height + player.radius;
	}
	velocity.y = 0.0f;
      }

      // Display the game
      BeginDrawing();
          ClearBackground(RAYWHITE);
          BeginMode2D(camera); 
          DrawCircleV(player.pos, player.radius, BLUE);
          DrawRectangleRec(greenBlock, GREEN);
          UpdateMusicStream(music);
      EndDrawing();
    }

    CloseWindow();
    return 0;
}
