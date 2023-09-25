#include <cmath>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <esat/window.h>
#include <esat/draw.h>
#include <esat/sprite.h>
#include <esat/input.h>
#include <esat/time.h>

void RenderFormula() {
  static float offset = 0;
  offset += 0.01f;
  float points[2000];
  for (int i = 0; i < 1000; ++i) {
    points[i*2] = i*640.f/1000.f;
    points[i * 2 + 1] = sinf(-offset*2.f + i / 2.0f)*10.+sinf(offset + i / 20.0f)*40.f + 240.f;
  }
  esat::DrawSetStrokeColor(255, 0, 0);
  esat::DrawPath(points,1000);
}

void RenderQuad() {
  float p[] = {
    20,20,
    100,20,
    100,100,
    20,100,
    20,20, // last point connects with the first one
  };
  esat::DrawSetFillColor(128,64,64,200);
  esat::DrawSetStrokeColor(255,255,0,200);
  esat::DrawSolidPath(p, 5, true);
}

void RenderText() {
  esat::DrawSetFillColor(255, 255, 0, 255);
  esat::DrawSetTextSize(50);
  esat::DrawSetTextBlur(6);
  esat::DrawText(350, 200, "Hi there! esat :)");
  esat::DrawSetFillColor(200, 200, 200, 255);
  esat::DrawSetTextSize(50);
  esat::DrawSetTextBlur(0);
  esat::DrawText(350, 200, "Hi there! esat :)");
}

void RenderFPS() {
  static double last_time = esat::Time();
  double current_time = esat::Time();
  double fps = 1000.0 / (current_time - last_time);
  std::stringstream ss;
  ss << "FPS = " << std::setprecision(2) << std::fixed << fps;
  esat::DrawSetFillColor(0, 255, 255, 255);
  esat::DrawSetTextSize(40);
  esat::DrawSetTextBlur(0);
  esat::DrawText(10, 45, ss.str().c_str());
  last_time = current_time;
}

void RenderDebug() {
  std::stringstream ss;
  ss << "mouse = " << esat::MousePositionX() << "," << esat::MousePositionY();
  esat::DrawSetFillColor(255, 0, 255, 255);
  esat::DrawSetTextSize(18);
  esat::DrawSetTextBlur(0);
  esat::DrawText(10, 10, ss.str().c_str());
  // if F1 is pressed, show the content of the input buffer
  if (esat::IsSpecialKeyDown(esat::kSpecialKey_F1)) {
    std::cout << "INPUT BUFFER" << std::endl;
    while (char c = esat::GetNextPressedKey()) {
      std::cout << c;
    }
    std::cout << std::endl << "----------------" << std::endl;
  }
}

void UpdateProceduralTexture(esat::SpriteHandle s, float t) {
  unsigned char sprite_data[128 * 128 * 4];
  for (int i = 0; i < 128; ++i) {
    for (int j = 0; j < 128; ++j) {
      int p = (i * 128 + j) * 4;
      sprite_data[p+0] = 155*((i/8)%2);
      sprite_data[p+1] = 155 *((j / 8) % 2);
      sprite_data[p + 2] = sinf(t*2+i/8.f)*127+128;
      sprite_data[p + 3] = sinf(t/5.f + j / 10.f) * 100 + 150;
    }
  }
  esat::SpriteUpdateFromMemory(s, sprite_data);
}

void Test() {
  esat::WindowInit(640, 480);
  esat::DrawSetTextFont("../data/test.ttf");
  esat::SpriteHandle sprite = esat::SpriteFromFile("../data/texture.png");
  esat::SpriteHandle procedural_sprite = esat::SpriteFromMemory(128, 128, (const unsigned char*)0L);
  float f = 0.f;
  struct {
    bool capture;
    char buffer[100];
    int pos;
  } input = { false, "", 0 };
  esat::WindowSetMouseVisibility(true);
  while (esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
    esat::DrawBegin();
    esat::DrawClear(0, 0, 0);
    // single line...
    esat::DrawSetStrokeColor(255, 255, 255);
    esat::DrawLine(0, 0, 640, 480);
    // Draw a Sprite
    esat::DrawSprite(sprite, 150 + 60 * cosf(f), 20);
    UpdateProceduralTexture(procedural_sprite, f);
    esat::DrawSprite(procedural_sprite, 150 + -60 * cosf(f), 220);
    // Compute a transformation matrix:
    esat::Mat3 m = esat::Mat3Translate(esat::MousePositionX(), esat::MousePositionY());
    m = esat::Mat3Multiply(m, esat::Mat3Rotate(f));
    m = esat::Mat3Multiply(m, esat::Mat3Scale(0.2, 0.2));
    esat::DrawSpriteWithMatrix(sprite, m);
    if (esat::IsSpecialKeyDown(esat::kSpecialKey_F1)) {
      if (input.capture) {
        input.capture = false;
      }
      else {
        input.capture = true;
        input.pos = 0;
        input.buffer[0] = '\0';
        esat::ResetBufferdKeyInput();
      }
    }
    if (input.capture) {
      char k = esat::GetNextPressedKey();
      if (k && input.pos < 100) {
        input.buffer[input.pos] = k;
        input.buffer[input.pos + 1] = '\0';
        input.pos++;
      }
      esat::DrawText(10, 450, "CAPTURING TEXT");
      esat::DrawText(10, 480, input.buffer);
    }

    esat::SpriteTransform st;
    st.x = esat::MousePositionX();
    st.y = esat::MousePositionY();
    st.angle = f + 1.5;
    st.scale_x = 0.1;
    st.scale_y = 0.8;
    st.sprite_origin_x = esat::SpriteWidth(sprite) / 2;
    st.sprite_origin_y = esat::SpriteHeight(sprite) / 2;
    esat::DrawSprite(sprite, st);

    // more complex drawing
    RenderFormula();
    RenderQuad();
    RenderText();
    // debug
    RenderDebug();
    RenderFPS();
    esat::DrawEnd();
    // End of current frame
    esat::WindowFrame();
    f += 0.1;
    // Check key status
    if (esat::IsKeyDown('A'))    std::cout << "A -> DOWN     " << f << std::endl;
    if (esat::IsKeyPressed('A')) std::cout << "A -> PRESSED  " << f << std::endl;
    if (esat::IsKeyUp('A'))      std::cout << "A -> UP       " << f << std::endl;

    if (esat::IsSpecialKeyDown(esat::kSpecialKey_Shift))    std::cout << "shift -> DOWN     " << f << std::endl;
    if (esat::IsSpecialKeyPressed(esat::kSpecialKey_Shift)) std::cout << "shift -> PRESSED  " << f << std::endl;
    if (esat::IsSpecialKeyUp(esat::kSpecialKey_Shift))      std::cout << "shift -> UP       " << f << std::endl;
  }
  esat::SpriteRelease(sprite);
  esat::SpriteRelease(procedural_sprite);
  esat::WindowDestroy();
}

//int esat::main(int argc, char **argv) {
//  Test();
//  return 0;
//}
