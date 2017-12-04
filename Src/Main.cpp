/**
* @file Main.cpp
*/
#include "GLFWEW.h"
#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"
#include <glm/gtc/matrix_transform.hpp>

/// タイムラインオブジェクトの配列.
using TimelineList = std::vector<FrameAnimation::TimelinePtr>;

/**
* アニメーションデータの初期化.
*/
TimelineList InitAnimationData()
{
  static const FrameAnimation::KeyFrame escortFrames[] = {
    { 0.000f, glm::vec2(480, 0), glm::vec2(32, 32) },
    { 0.125f, glm::vec2(480, 96), glm::vec2(32, 32) },
    { 0.250f, glm::vec2(480, 64), glm::vec2(32, 32) },
    { 0.375f, glm::vec2(480, 32), glm::vec2(32, 32) },
    { 0.500f, glm::vec2(480, 0), glm::vec2(32, 32) },
  };
  TimelineList timelineList;
  FrameAnimation::TimelinePtr timeline = std::make_shared<FrameAnimation::Timeline>();
  timeline->data.assign(std::begin(escortFrames), std::end(escortFrames));
  timelineList.push_back(timeline);

  return timelineList;
}

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
  const TimelineList timelineList = InitAnimationData();

  Sprite sprite(tex);
  sprite.Rectangle({glm::vec2(0 ,0), glm::vec2(64, 32)});
  sprite.Name("player");

  Sprite boss(tex);
  boss.Rectangle({glm::vec2(320 ,128), glm::vec2(128, 256)});
  boss.Position(glm::vec3(256, 0, 0));
  boss.Name("boss");

  Sprite background(texBg);
  background.Name("bg");

  Node rootNode;
  rootNode.Name("rootNode");
  rootNode.AddChild(&background);
  rootNode.AddChild(&boss);
  rootNode.AddChild(&sprite);

  Node escortNode;
  escortNode.Position(glm::vec3(-16, 0, 0));
  boss.AddChild(&escortNode);

  std::vector<Sprite> escortList(16, Sprite(tex));
  for (size_t i = 0; i < escortList.size(); ++i) {
    const auto m = glm::rotate(glm::mat4(), glm::radians(static_cast<float>(i * 360) / static_cast<float>(escortList.size())), glm::vec3(0, 0, 1));
    const glm::vec4 pos = m * glm::vec4(0, 144, 0, 1);
    escortList[i].Position(pos);
    escortList[i].Rectangle({glm::vec2(480, 0), glm::vec2(32, 32)});
    escortNode.AddChild(&escortList[i]);

    auto animator = std::make_shared<FrameAnimation::Animate>(timelineList[0]);
    escortList[i].Animator(animator);
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glDepthFunc(GL_LEQUAL);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // メインループ.
  while (!window.ShouldClose()) {
    window.Update();
    const float deltaTime = window.DeltaTime();
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
      vec = glm::normalize(vec) * 400.0f * deltaTime;
      sprite.Position(sprite.Position() + vec);
    }

    escortNode.Rotation(escortNode.Rotation() + glm::radians(25.0f * deltaTime));
    for (auto e : escortNode.Children()) {
      e->Rotation(-escortNode.Rotation());
    }

    rootNode.UpdateRecursive(deltaTime);
    spriteRenderer.Update(rootNode);

    glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    spriteRenderer.Draw(glm::vec2(800, 600));

    window.SwapBuffers();
  }

  return 0;
}
