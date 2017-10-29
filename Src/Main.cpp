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

  SpriteRenderer spriteRenderer;
  spriteRenderer.Init(10000);

  TexturePtr tex = Texture::LoadFromFile("Res/Sample.bmp");
  Sprite sprite(tex);

  glEnable(GL_DEPTH_TEST);

  // メインループ.
  while (!window.ShouldClose()) {
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
