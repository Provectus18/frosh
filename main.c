#include "raylib.h"

int main(void)
{
    const int W = 800, H = 450;
    InitWindow(W, H, "Frosh");
    SetTargetFPS(60);

    Vector2 pos = { W/2.0f, H/2.0f };
    float radius = 20.0f;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
	if (IsKeyPressed(KEY_F)){
	    ToggleFullscreen();
	  }
        
	// Move the ball with Key inputs
	
	if (IsKeyDown(KEY_RIGHT)) pos.x += 2.0f;
        if (IsKeyDown(KEY_LEFT)) pos.x -= 2.0f;
        if (IsKeyDown(KEY_UP)) pos.y -= 2.0f;
        if (IsKeyDown(KEY_DOWN)) pos.y += 2.0f;
	
        // Prevent the ball from going out of frame, cling to the side

	 if (pos.x - radius < 0) {
        pos.x = radius;
        }
        if (pos.x + radius > W) {
        pos.x = W - radius;
        }
        if (pos.y - radius < 0) {
        pos.y = radius;
        }
        if (pos.y + radius > H) {
        pos.y = H - radius;
        }

        if (pos.x - radius < W)
	
	// Draw the circle on the screen
	BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawCircleV(pos, radius, BLUE);
            DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
