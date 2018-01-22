/**
* @file Title.cpp
*/
#include "Title.h"
#include "MainGame.h"
#include "../GLFWEW.h"

namespace Scene {

/// 文字列の色.
static const glm::vec4 color[2] = {
  {0.25f, 1.0f, 0.5f, 1.0f }, // アクティブな文字列の色.
  {0.25f, 1.0f, 0.5f, 0.25f} // 非アクティブな文字列の色.
};

/**
* テキストをスプライトにする.
*
* @param v          スプライト化したテキストを格納するvector.
* @param offset     スプライト表示開始座標.
* @param text       スプライト化するテキスト.
* @param colorIndex 文字の色を示すインデックス.
*/
void Title::SetTextSprite(std::vector<Sprite>& v, glm::vec3 offset, const char* text, int colorIndex)
{
  static const glm::vec2 fontSize(32, 64);
  v.reserve(std::strlen(text));
  while (*text) {
    if (*text > ' ') {
      const int code = *text - ' ';
      const float x = static_cast<float>(code % 16);
      const float y = static_cast<float>(7 - code / 16);
      v.push_back(Sprite(texFont));
      v.back().Rectangle({ glm::vec2(x * fontSize.x, y * fontSize.y), fontSize });
      v.back().Position(offset);
      v.back().Color(color[colorIndex]);
      AddChild(&v.back());
    }
    offset.x += fontSize.x;
    ++text;
  }
}

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
  AddChild(&sprBg);

  texTitle = Texture::LoadFromFile("Res/Title.dds");
  if (!texTitle) {
    return false;
  }
  sprTitle.Texture(texTitle);
  sprTitle.Position(glm::vec3(0, 96, 0));
  AddChild(&sprTitle);

  texFont = Texture::LoadFromFile("Res/Font.dds");
  if (!texFont) {
    return false;
  }
  SetTextSprite(sprPushStart, glm::vec3(-32 * 5, -128, 0), "PUSH START", 0);
  SetTextSprite(sprExit, glm::vec3(-32 * 2, -192, 0), "EXIT", 1);

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
    if (select == 0) {
      manager.ReplaceScene(std::make_shared<MainGame>()); // メインゲーム画面へ.
    } else {
      manager.End(); // ゲーム終了.
    }
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
