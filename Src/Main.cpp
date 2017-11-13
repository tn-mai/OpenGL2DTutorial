/**
* @file Main.cpp
*/
#include "GLFWEW.h"
#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"

/// エントリーポイント.
int main()
{
  GLFWEW::Window& window = GLFWEW::Window::Instance();
  if (!window.Init(800, 600, "OpenGL Tutorial")) {
    return 1;
  }

  TexturePtr tex = Texture::LoadFromFile("Res/Objects.dds");
  TexturePtr texBg = Texture::LoadFromFile("Res/UnknownPlanet.dds");
  if (!tex || !texBg) {
    return 1;
  }

  SpriteRenderer spriteRenderer;
  spriteRenderer.Init(10000);
  Node rootNode;
  rootNode.name = "rootNode";

  Sprite sprite(tex);
  sprite.Rectangle({glm::vec2(0 ,0), glm::vec2(64, 32)});
  sprite.name = "player";

  Sprite boss(tex);
  boss.Rectangle({glm::vec2(320 ,128), glm::vec2(128, 256)});
  boss.Position(glm::vec3(256, 0, 0));
  boss.name = "boss";

  Sprite background(texBg);
  background.name = "bg";

  rootNode.AddChild(&background);
  rootNode.AddChild(&boss);
  rootNode.AddChild(&sprite);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glDepthFunc(GL_LEQUAL);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
      rootNode.Position(rootNode.Position() + vec);
    }

    glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    rootNode.Update(glm::mat4x4());
    spriteRenderer.Update(&rootNode);
    spriteRenderer.Draw(glm::vec2(800, 600));

    window.SwapBuffers();
  }

  return 0;
}
