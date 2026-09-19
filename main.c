#include "raylib.h"

int main(void)
{
    const int W = 800, H = 450;
    InitWindow(W, H, "Frosh");
    SetTargetFPS(60);

    Vector2 pos = { W/2.0f, H/2.0f };
    float radius = 20.0f;

    Camera2D camera = {
    .offset = (Vector2){ W/2.0f, H/2.0f },
    .rotation = 0.0f,
    .zoom = 1.0f
    };

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
	camera.target = pos;

	if (IsKeyPressed(KEY_F)) ToggleFullscreen();
	if (IsKeyDown(KEY_RIGHT)) pos.x += 2.0f;
        if (IsKeyDown(KEY_LEFT)) pos.x -= 2.0f;
        if (IsKeyDown(KEY_UP)) pos.y -= 2.0f;
        if (IsKeyDown(KEY_DOWN)) pos.y += 2.0f;


        // Display the game

        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode2D(camera); 
            DrawCircleV(pos, radius, BLUE);
	    DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
