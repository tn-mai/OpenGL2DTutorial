/**
* @file Main.cpp
*/
#include "GLFWEW.h"
#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

/// エントリーポイント.
int main()
{
  GLFWEW::Window& window = GLFWEW::Window::Instance();
  if (!window.Init(800, 600, "OpenGL Tutorial")) {
    return 1;
  }

  TexturePtr tex = Texture::LoadFromFile("Res/Sample.bmp");
  if (!tex) {
    return 1;
  }
  SpriteRenderer spriteRenderer;
  spriteRenderer.Init(10000);
  Sprite sprite(tex);
  sprite.Scale(glm::vec2(800.0f / tex->Width(), 600.0f / tex->Height()));

  glEnable(GL_DEPTH_TEST);

  // メインループ.
  while (!window.ShouldClose()) {
    window.UpdateGamePad();
    const GamePad& gamepad = window.GetGamePad();
    glm::vec3 vec;
    if (gamepad.buttons & GamePad::DPAD_LEFT) {
      vec.x = -1;
    } else if (gamepad.buttons & GamePad::DPAD_RIGHT) {
      vec.x = 1;
    }
    if (gamepad.buttons & GamePad::DPAD_UP) {
      vec.y = 1;
    } else if (gamepad.buttons & GamePad::DPAD_DOWN) {
      vec.y = -1;
    }
    if (vec.x || vec.y) {
      vec = glm::normalize(vec);
      sprite.Position(sprite.Position() + vec);
    }

    glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    spriteRenderer.BeginUpdate();
    spriteRenderer.AddVertices(sprite);
    spriteRenderer.EndUpdate();
    spriteRenderer.Draw(tex, glm::vec2(800, 600));

    window.SwapBuffers();
  }

  return 0;
}
