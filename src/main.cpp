#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>

struct Size {
    int width;
    int height;
};

struct Box {
    Vector2 pos;
    Size dimens;
};

struct Bullet {
    Vector2 pos;
    Size dimens;
};

struct Enemy {
    Vector2 pos;
    Size dimens;
    Texture2D texture;
};

void PollInputs(struct Box* box, std::vector<Bullet>* bullets) {
    if (IsKeyDown(KEY_RIGHT)) {
        box->pos.x = box->pos.x + 10;
    }
    if (IsKeyDown(KEY_LEFT)) {
        box->pos.x = box->pos.x - 10;
    }
    if (IsKeyReleased(KEY_SPACE)) {
        struct Bullet newbullet;
        newbullet.pos.x = box->pos.x + box->dimens.width / 2;
        newbullet.pos.y = box->pos.y - 20;
        newbullet.dimens = {2, 5};
        bullets->push_back(newbullet);
    }
}

int isCollision(Vector2 pos1, Vector2 pos2, Size size2) {
    if (pos1.x > pos2.x && pos1.x < pos2.x + size2.width) {
        if (pos1.y < pos2.y + size2.height) {
            return 1;
        }
    }
    return 0;
}

void CollisionDetection(std::vector<Enemy>* enemies, std::vector<Bullet> bullets) {
    for (int i = 0; i < enemies->size(); i++) {
        for (Bullet bullet : bullets) {
            if (isCollision(bullet.pos, (*enemies)[i].pos, (*enemies)[i].dimens)) {
                enemies->erase(enemies->begin() + i);
            }
        }
    }
}

void RenderBullets(std::vector<Bullet>* bullets) {
    for (int i = 0; i < bullets->size(); i++) {
        Bullet bullet = (*bullets)[i];
        if (bullet.pos.y < 0) {
            bullets->erase(bullets->begin() + i);
            continue;
        }
        DrawRectangle(bullet.pos.x, bullet.pos.y, bullet.dimens.width, bullet.dimens.height, RED);
        (*bullets)[i].pos.y -= 3;
    }
}

void RenderEnemies(std::vector<Enemy> enemies) {
    for (Enemy enemy : enemies) {
        DrawTextureEx(enemy.texture, enemy.pos, 0.0, 0.08, WHITE);
    }
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Space Invaders");

    std::vector<Bullet> bullets;

    Texture2D enemyTexture = LoadTexture("assets/enemy.png");
    std::vector<Enemy> enemies;

    for (int i = 0; i < 10; i++) {
        float x = 10 + (i * 80);
        Enemy newen = { {x, 20}, {80,80}, enemyTexture };
        enemies.push_back(newen);
    }

    struct Box box;
    box.dimens = {100, 20};
    box.pos.x = GetScreenWidth() / 2 - box.dimens.width / 2;
    box.pos.y = GetScreenHeight() - 50 - box.dimens.height / 2;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
       BeginDrawing();

            ClearBackground(WHITE);

            if (enemies.size() <= 0) {
                Vector2 dimens = MeasureTextEx(GetFontDefault(), "YOU WIN", 50, 50.0/10.0);
                DrawText("YOU WIN", (GetScreenWidth() / 2) - dimens.x / 2, (GetScreenHeight() / 2) - dimens.y / 2, 50, PINK);
                EndDrawing();
                continue;
            }

            PollInputs(&box, &bullets);

            CollisionDetection(&enemies, bullets);

            // draw player
            DrawRectangle(box.pos.x, box.pos.y, box.dimens.width, box.dimens.height, PINK);

            RenderBullets(&bullets);

            RenderEnemies(enemies);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
