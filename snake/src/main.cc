
#include <cmath>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <esat/window.h>
#include <esat/draw.h>
#include <esat/sprite.h>
#include <esat/input.h>
#include <esat/time.h>

#include "grid.h"
#include "agent.h"

const int fps = 5;
const int kQuadSize = 32;
const int kWindowWidth = 640;
const int kWindowHeight = 480;

double current_time, last_time;

struct Quad {
  int x1, y1, x2, y2;
};

struct Pos {
  int x, y;
};

void DrawTiles() {
  for (int x = kQuadSize; x < kWindowWidth; x += kQuadSize) {
    esat::DrawSetStrokeColor(255, 255, 255, 255);
    esat::DrawLine((float)x, 0, (float)x, kWindowHeight);
  }
  for (int y = kQuadSize; y < kWindowHeight; y += kQuadSize) {
    esat::DrawSetStrokeColor(255, 255, 255, 255);
    esat::DrawLine(0, (float)y, kWindowWidth, (float)y);
  }
}

Pos MoveSnake(Pos pos, Quad target) {
  if (pos.x > target.x1) pos.x += kQuadSize;
  if (pos.x < target.x1) pos.x -= kQuadSize;
  if (pos.y > target.y1) pos.y += kQuadSize;
  if (pos.y < target.y1) pos.y -= kQuadSize;
  return pos;
}

Quad GenerateRandomQuad() {
  int randx = (rand() % 20) * kQuadSize;
  int randy = (rand() % 15) * kQuadSize;
  printf("%d, %d\n", randx, randy);
  Quad quad = { randx, (randx + kQuadSize), randy, (randy + kQuadSize)};
  return quad;
}

void DrawQuad(Quad quad) {    
  float p[] = {
    (float)quad.x1, (float)quad.y1,
    (float)quad.x2, (float)quad.y1,
    (float)quad.x2, (float)quad.y2,
    (float)quad.x1, (float)quad.y2,
    (float)quad.x1, (float)quad.y2,
  };
  esat::DrawSetFillColor(255, 0, 0, 255);
  esat::DrawSetStrokeColor(255,0, 0, 255);
  esat::DrawSolidPath(p, 5, true);
}

void Play() {
  
  srand(time(0));

  esat::WindowInit(640, 480);
  esat::WindowSetMouseVisibility(true);

  esat::SpriteHandle snake_sprite = esat::SpriteFromFile("../data/snake.png");


  Pos pos = { 0, 0 };

  Quad quad = GenerateRandomQuad();

  //Grid* grid = Grid::CreateGrid(100, 100);

  while (esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
    last_time = esat::Time();
    esat::DrawBegin();
    esat::DrawClear(0, 0, 0);
    
    //Update    
    pos = MoveSnake(pos, quad);

    //Draw
    esat::DrawSprite(snake_sprite, (float)pos.x, (float)pos.y);
    DrawQuad(quad);
    DrawTiles();

    esat::DrawEnd();

    //Control fps
    do {
      current_time = esat::Time();
    } while ((current_time - last_time) <= 1000.0 / fps);
    // End of current frame
    esat::WindowFrame();

  }

  esat::SpriteRelease(snake_sprite);

  esat::WindowDestroy();
}

int esat::main(int argc, char** argv) {
  argc = 0;
  argv = nullptr;
  Play();  
  return 0;
}
