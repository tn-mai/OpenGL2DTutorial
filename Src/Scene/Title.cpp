/**
* @file Title.cpp
*/
#include "Title.h"
#include "../GLFWEW.h"

namespace Scene {

static const glm::vec4 color[2] = {
  {0.25f, 1.0f, 0.5f, 1.0f },
  {0.25f, 1.0f, 0.5f, 0.25f}
};

/**
* タイトル画面の初期化.
*
* @param manager シーン管理クラス.
*
* @retval true  初期化成功.
* @retval false 初期化失敗(初期化中になんらかのエラーが発生した).
*/
bool Title::Initialize(Manager& manager)
{
  texBg = Texture::LoadFromFile("Res/UnknownPlanet.dds");
  if (!texBg) {
    return false;
  }
  sprBg.Texture(texBg);
  manager.AddChild(&sprBg);

  texTitle = Texture::LoadFromFile("Res/Title.dds");
  if (!texTitle) {
    return false;
  }
  sprTitle.Texture(texTitle);
  sprTitle.Position(glm::vec3(0, 96, 0));
  manager.AddChild(&sprTitle);

  texFont = Texture::LoadFromFile("Res/Font.dds");
  if (!texFont) {
    return false;
  }
  {
    glm::vec3 offset(-32 * 5, -128, 0);
    sprPushStart.reserve(32);
    for (auto e : "PUSH START") {
      const int code = e - ' ';
      const int x = code % 16;
      const int y = 7 - code / 16;
      sprPushStart.push_back(Sprite(texFont));
      sprPushStart.back().Rectangle({ glm::vec2(x * 512 / 16, y * 512 / 8), glm::vec2(32, 64) });
      sprPushStart.back().Position(offset);
      sprPushStart.back().Color(color[0]);
      manager.AddChild(&sprPushStart.back());
      offset.x += 32.0f;
    }
  }
  {
    glm::vec3 offset(-32 * 2, -128 - 64, 0);
    sprExit.reserve(32);
    for (auto e : "EXIT") {
      const int code = e - ' ';
      const int x = code % 16;
      const int y = 7 - code / 16;
      sprExit.push_back(Sprite(texFont));
      sprExit.back().Rectangle({glm::vec2(x * 512 / 16, y * 512 / 8), glm::vec2(32, 64)});
      sprExit.back().Position(offset);
      sprExit.back().Color(color[1]);
      manager.AddChild(&sprExit.back());
      offset.x += 32.0f;
    }
  }

  return true;
}

/**
* タイトル画面の更新.
*
* @param manager シーン管理クラス.
* @param dt      前回の更新からの経過時間.
*
* @retval true  更新成功.
* @retval false 更新失敗(更新中になんらかのエラーが発生した).
*/
bool Title::Update(Manager& manager, float dt)
{
  GLFWEW::Window& window = GLFWEW::Window::Instance();
  const auto gamepad = window.GetGamePad();
  if (gamepad.buttonDown & GamePad::DPAD_UP) {
    if (--select < 0) {
      select = 1;
    }
  } else  if (gamepad.buttonDown & GamePad::DPAD_DOWN) {
    if (++select > 1) {
      select = 0;
    }
  }
  if (gamepad.buttonDown & GamePad::START) {
    // メインゲーム画面へ.
  }

  for (auto& e : sprPushStart) {
    e.Color(color[select]);
  }
  for (auto& e : sprExit) {
    e.Color(color[!select]);
  }

  return true;
}

/**
* タイトル画面の終了.
*
* @param manager シーン管理クラス.
*/
void Title::Finalize(Manager& manager)
{
}

} // namespace Scene
