#include <chrono>
#include <thread>

#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "button.hpp"

// Sounds
#include "ball.h"
#include "music.h"

Wave ball_wave = LoadWaveFromMemory(".mp3", ball_mp3, ball_mp3_len);
Sound ball_sfx;
Music bgmusic;

// Colors
const Color green = Color{38, 185, 154, 255};
const Color darkGreen = Color{20, 160, 133, 255};
const Color lightGreen = Color{129, 204, 184, 255};
const Color yellow = Color{243, 213, 91, 255};

// Declarations
void LimitMovementY(Vector2& pos, Vector2 size);
void Reset();
class Ball;
class Paddle;
class CPUPaddle;

const int screen_width = 1280; const int screen_height = 800;

int playerScore = 0;
int cpuScore = 0;

bool running = true;

void LimitMovementY(Vector2& pos, Vector2 size)
{
    if (pos.y < 0)
    {
        pos.y = 0;
    }

    if (pos.y + size.y > screen_height)
    {
        pos.y = screen_height - size.y;
    }
}

class Ball 
{
    public:
        Vector2 pos;
        Vector2 speed;
        int radius;

        void Draw()
        {
            Color color = (abs(speed.x) == 30) ? RED : yellow;
            DrawCircle(pos.x, pos.y, radius, color);
        }

        void Update()
        {
            if (!running)
            {
                return;
            }
            pos = Vector2Add(pos, speed);
            if (pos.y + radius  >= screen_height || pos.y - radius <= 0)
            {
                speed.y *= -1;
            }

            if (pos.x + radius  >= screen_width)
            {
                cpuScore++;
                BeginDrawing();
                    DrawRectangle(0, 0, screen_width, screen_height, Color{0, 255, 0, 100});
                EndDrawing();
                Reset();
            }

            if (pos.x - radius <= 0)
            {
                playerScore++;
                BeginDrawing();
                    DrawRectangle(0, 0, screen_width, screen_height, Color{255, 0, 0, 100});
                EndDrawing();
                Reset();
            }
        }
};

class Paddle
{
    public:
        Vector2 pos;
        Vector2 size;
        int speed;

        void Draw()
        {
            DrawRectangleRounded(Rectangle{pos.x, pos.y, size.x, size.y}, 1, 6, WHITE);
            DrawRectangle(pos.x, pos.y + 50, 25, 20, RED);
        }

        void Update()
        {
            if (IsKeyDown(KEY_UP))
            {
                running = true;
                pos.y -= speed;
            }

            if (IsKeyDown(KEY_DOWN))
            {
                running = true;
                pos.y += speed;
            }

            

            LimitMovementY(pos, size);
        }
};

class CPUPaddle: public Paddle
{
    public:
        void Draw()
        {
            DrawRectangleRounded(Rectangle{pos.x, pos.y, size.x, size.y}, 1, 6, WHITE);
        }

        void Update(int ball_y)
        {
            if (pos.y + size.y / 2 < ball_y)
            {
                pos.y += speed;
            }
            else if (pos.y + size.y / 2 > ball_y)
            {
                pos.y -= speed;
            }

            LimitMovementY(pos, size);
        }
};

Ball ball;
Paddle player;
CPUPaddle ai;

void Reset()
{
    running = false;
    ball.pos = Vector2{screen_width / 2, screen_height / 2};
    ball.speed = Vector2{15, 15};
    ai.pos.y = (screen_height - ai.size.y) / 2;
    player.pos.y = (screen_height - player.size.y) / 2;

    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void playMusic(Music music)
{
    while (true)
    {
        UpdateMusicStream(music);
    }
}

int main()
{
    InitWindow(screen_width, screen_height, "Pong");
    SetTargetFPS(60);

    InitAudioDevice();
    ball_sfx = LoadSoundFromWave(ball_wave);
    bgmusic = LoadMusicStreamFromMemory(".wav", music_wav, music_wav_len);
    bgmusic.looping = true;
    PlayMusicStream(bgmusic);
    std::thread musicThread(playMusic, bgmusic);

    Button playBut = Button(500, 400, 300, 140, "Play", Color{10, 100, 80, 255}, darkGreen);

    // Main Menu
    while (true)
    {
        if (WindowShouldClose())
        {
            CloseWindow();
            return 0;
        }
        BeginDrawing();
            ClearBackground(green);
            DrawRectangle(0, 0, screen_width / 2, screen_height, darkGreen);
            DrawText("Pong", 400, 150, 200, Color{255, 255, 255, 255});
            playBut.Draw();
            if (playBut.isPressed())
            {
                break;
            }

        EndDrawing();
        UpdateMusicStream(bgmusic);
    }

    // Tutorial
    while (true)
    {
        BeginDrawing();
            ClearBackground(green);
            DrawRectangle(0, 0, screen_width / 2, screen_height, darkGreen);
            DrawText("How to Play", 125, 75, 150, yellow);
            DrawText("Move with arrows", 200, 300, 50, BLACK);
            DrawText("Try to hit the ball to the other side", 200, 375, 50, BLACK);
            DrawText("To get extra speed for the ball,", 200, 450, 50, BLACK);
            DrawText("hit it with the center (red", 250, 525, 50, BLACK);
            DrawText("part) of the paddle", 250, 600, 50, BLACK);
            DrawText("Press space to continue", 200, 675, 50, BLACK);
        EndDrawing();
        if (WindowShouldClose())
        {
            CloseWindow();
            return 0;
        }
        if (IsKeyPressed(KEY_SPACE))
        {
            break;
        }
        UpdateMusicStream(bgmusic);
    }

    // Initialize ball
    ball.radius = 20;
    ball.pos = Vector2{screen_width / 2, screen_height / 2};
    ball.speed = Vector2{15, 15};

    // Initialize player
    player.size = Vector2{25, 120};
    player.pos = Vector2{10, (screen_height - player.size.y) / 2};
    player.speed = 10;

    ai.size = Vector2{25, 120};
    ai.pos = Vector2{screen_width - ai.size.x - 10, (screen_height - player.size.y) / 2};
    ai.speed = 15;

    while (true)
    {
        if (WindowShouldClose())
        {
            CloseWindow();
            return 0;
        }
        BeginDrawing();
            // Background
            ClearBackground(green);
            DrawRectangle(0, 0, screen_width / 2, screen_height, darkGreen);
            DrawCircle(screen_width / 2, screen_height / 2, 150, lightGreen);
            DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, Color{255, 255, 255, 255});

            // Updating
            ball.Update();
            player.Update();
            ai.Update(ball.pos.y);
            if (CheckCollisionCircleRec(ball.pos, ball.radius, Rectangle{player.pos.x, player.pos.y + 5, player.size.x, player.size.y - 10}))
            {
                PlaySound(ball_sfx);
                ball.speed.x *= -1;
                ball.pos.x += 10;
                if (CheckCollisionCircleRec(ball.pos, ball.radius, Rectangle{player.pos.x, player.pos.y + 50, player.size.x, player.size.y - 100}))
                {
                    ball.speed = Vector2Multiply(ball.speed, Vector2{2, 2});
                }
            }

            if (CheckCollisionCircleRec(ball.pos, ball.radius, Rectangle{ai.pos.x, ai.pos.y + 5, ai.size.x, ai.size.y - 10}))
            {
                PlaySound(ball_sfx);
                if (abs(ball.speed.x) == 30)
                {
                    ball.speed = Vector2Divide(ball.speed, Vector2{2, 2});
                }
                ball.pos.x -= 10;
                ball.speed.x *= -1;
            }

            // Drawing
            ball.Draw();
            ai.Draw();
            player.Draw();

            DrawText(TextFormat("%i", playerScore), 1000, 50, 100, Color{255, 255, 255, 255});
            DrawText(TextFormat("%i", cpuScore), 100, 50, 100, Color{255, 255, 255, 255});

            // Winning
            if (playerScore == 5 || cpuScore == 5)
            {
                BeginDrawing();
                    ClearBackground(lightGreen);
                    DrawText("Game End", 200, 400, 300, darkGreen);
                EndDrawing();

                while (!WindowShouldClose()) {;}
                CloseWindow();
                return 0;
            }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}