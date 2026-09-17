#include "raylib.h"

int main(void)
{
    const int W = 800, H = 450;
    InitWindow(W, H, "Bouncing Ball");
    SetTargetFPS(60);

    Vector2 pos = { W/2.0f, H/2.0f };
    Vector2 vel = { 300.0f, 250.0f };
    float radius = 20.0f;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // Move
        pos.x += vel.x * dt;
        pos.y += vel.y * dt;

        // Bounce off edges
        if (pos.x - radius < 0 || pos.x + radius > W) vel.x *= -1;
        if (pos.y - radius < 0 || pos.y + radius > H) vel.y *= -1;

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawCircleV(pos, radius, RED);
            DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
