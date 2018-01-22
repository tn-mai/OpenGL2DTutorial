/**
* @file Title.cpp
*/
#include "Title.h"
#include "MainGame.h"
#include "../GLFWEW.h"

namespace Scene {

/// ������̐F.
static const glm::vec4 color[2] = {
  {0.25f, 1.0f, 0.5f, 1.0f }, // �A�N�e�B�u�ȕ�����̐F.
  {0.25f, 1.0f, 0.5f, 0.25f} // ��A�N�e�B�u�ȕ�����̐F.
};

/**
* �e�L�X�g���X�v���C�g�ɂ���.
*
* @param v          �X�v���C�g�������e�L�X�g���i�[����vector.
* @param offset     �X�v���C�g�\���J�n���W.
* @param text       �X�v���C�g������e�L�X�g.
* @param colorIndex �����̐F�������C���f�b�N�X.
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
    if (select == 0) {
      manager.ReplaceScene(std::make_shared<MainGame>()); // ���C���Q�[����ʂ�.
    } else {
      manager.End(); // �Q�[���I��.
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
* �^�C�g����ʂ̏I��.
*
* @param manager �V�[���Ǘ��N���X.
*/
void Title::Finalize(Manager& manager)
{
}

} // namespace Scene
