#include "raylib.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 450
#define MOVE_SPEED    500.0f

typedef struct Player {
    Vector2 pos;
    float radius;
} Player;

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Frosh");
    InitAudioDevice();
    SetTargetFPS(60);

    Music music = LoadMusicStream("pond.wav");
    SetMusicVolume(music, 0.7f);
    PlayMusicStream(music);

    Camera2D camera = {
        .offset = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f },
        .rotation = 0.0f,
        .zoom = 1.0f
    };

    Player player = {
        .pos = { 0.0f, -20.0f },
        .radius = 15.0f
    };

    Vector2 velocity = { 0.0f, 0.0f };

    Rectangle greenBlock = { 0.0f, 0.0f, 100.0f, 100.0f };

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        camera.target = player.pos;

        // Input keys
        if (IsKeyPressed(KEY_F)) ToggleFullscreen();

        velocity.x = 0.0f;
        velocity.y = 0.0f;
        if (IsKeyDown(KEY_RIGHT)) velocity.x =  MOVE_SPEED;
        if (IsKeyDown(KEY_LEFT))  velocity.x = -MOVE_SPEED;
        if (IsKeyDown(KEY_UP))    velocity.y = -MOVE_SPEED;
        if (IsKeyDown(KEY_DOWN))  velocity.y =  MOVE_SPEED;

        // Movement and collision
        Vector2 oldPos = player.pos;

        // X axis
        player.pos.x += velocity.x * dt;
        if (CheckCollisionCircleRec(player.pos, player.radius, greenBlock)) {
            player.pos.x = oldPos.x;
            velocity.x = 0.0f;
        }

        // Y axis
        player.pos.y += velocity.y * dt;
        if (CheckCollisionCircleRec(player.pos, player.radius, greenBlock)) {
            player.pos.y = oldPos.y;
            velocity.y = 0.0f;
        }

        // Render the game
        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode2D(camera);
                DrawCircleV(player.pos, player.radius, BLUE);
                DrawRectangleRec(greenBlock, GREEN);
            EndMode2D();
        EndDrawing();

        UpdateMusicStream(music);
    }

    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
