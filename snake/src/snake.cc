#include <cmath>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <esat/window.h>
#include <esat/draw.h>
#include <esat/sprite.h>
#include <esat/input.h>
#include <esat/time.h>

struct Quad {
  int x1, x2, y1, y2;
};

struct Pos {
  float x, y;
};

Pos MoveSnake(Pos pos) {

  pos.x += 32.0f;



  
  return pos;
}

Quad GenerateRandomQuad() {
  int randx = (rand() % 40) * 40;
  int randy = (rand() % 30) * 30;
  Quad quad = { (float) randx, (float) (randx + 32), (float) randy, (float) (randy + 32)};
  return quad;
}

void DrawQuad(Quad quad) {    
  float p[] = {
    quad.x1, quad.y1,
    quad.x2, quad.y1,
    quad.x2, quad.y2,
    quad.x1, quad.y2,
    quad.x1, quad.y2,
  };
  esat::DrawSetFillColor(255, 0, 0, 255);
  esat::DrawSetStrokeColor(255,0, 0, 255);
  esat::DrawSolidPath(p, 5, true);
}

void Snake() {
  esat::WindowInit(640, 480);
  esat::WindowSetMouseVisibility(true);

  esat::SpriteHandle snake_sprite = esat::SpriteFromFile("../data/snake.png");

  Pos pos = { 0.0f, 0.0f };

  Quad quad = GenerateRandomQuad();

  while (esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
    esat::DrawBegin();
    esat::DrawClear(0, 0, 0);
    
    pos = MoveSnake(pos);

    esat::DrawSprite(snake_sprite, pos.x, pos.y);
    DrawQuad(quad);

    esat::DrawEnd();

    // End of current frame
    esat::WindowFrame();

  }

  esat::SpriteRelease(snake_sprite);

  esat::WindowDestroy();
}

int esat::main(int argc, char **argv) {
  Snake();
  return 0;
}
