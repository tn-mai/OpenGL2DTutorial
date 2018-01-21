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
* �^�C�g����ʂ̏�����.
*
* @param manager �V�[���Ǘ��N���X.
*
* @retval true  ����������.
* @retval false ���������s(���������ɂȂ�炩�̃G���[����������).
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
* �^�C�g����ʂ̍X�V.
*
* @param manager �V�[���Ǘ��N���X.
* @param dt      �O��̍X�V����̌o�ߎ���.
*
* @retval true  �X�V����.
* @retval false �X�V���s(�X�V���ɂȂ�炩�̃G���[����������).
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
    // ���C���Q�[����ʂ�.
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
* �^�C�g����ʂ̏I��.
*
* @param manager �V�[���Ǘ��N���X.
*/
void Title::Finalize(Manager& manager)
{
}

} // namespace Scene
