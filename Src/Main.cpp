/**
* @file Main.cpp
*/
#include "GLFWEW.h"
#include "Scene/Title.h"

/// エントリーポイント.
int main()
{
  GLFWEW::Window& window = GLFWEW::Window::Instance();
  if (!window.Init(800, 600, "OpenGL Tutorial")) {
    return 1;
  }
  SpriteRenderer spriteRenderer;
  spriteRenderer.Init(10000);
  Scene::Manager sceneManager(glm::vec2(800, 600));
  sceneManager.Start(std::make_shared<Scene::Title>());

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glDepthFunc(GL_LEQUAL);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // メインループ.
  while (!window.ShouldClose()) {
    window.Update();
    const float deltaTime = window.DeltaTime();

    if (!sceneManager.Update(deltaTime)) {
      break;
    }
    spriteRenderer.Update(sceneManager.OriginNode());

    glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    spriteRenderer.Draw(glm::vec2(800, 600));

    window.SwapBuffers();
  }

  return 0;
}
