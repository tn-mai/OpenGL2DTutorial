/**
* @file Font.cpp
*/
#include "Font.h"

namespace Font {

TexturePtr texFont;
static const glm::vec2 fontSize(32, 64);

/**
* ������.
*/
bool Init()
{
  if (texFont) {
    return true;
  }
  texFont = Texture::LoadFromFile("Res/Font.dds");
  return texFont.get();
}

/**
* �\�����镶����ݒ肷��.
*
* @param sprite �\���p�X�v���C�g.
* @param c      �\�����镶��.
*/
void SetChar(Sprite& sprite, int c)
{
  if (c > ' ') {
    const int code = c - ' ';
    const float x = static_cast<float>(code % 16);
    const float y = static_cast<float>(7 - code / 16);
    sprite.Rectangle({ glm::vec2(x * fontSize.x, y * fontSize.y), fontSize });
  }
}

/**
* �e�L�X�g���X�v���C�g�ɂ���.
*
* @param v          �X�v���C�g�������e�L�X�g���i�[����vector.
* @param offset     �X�v���C�g�\���J�n���W.
* @param text       �X�v���C�g������e�L�X�g.
* @param colorIndex �����̐F�������C���f�b�N�X.
*/
void SetTextSprite(Node& node, std::vector<Sprite>& v, glm::vec3 offset, const char* text, const glm::vec4& color)
{
  v.reserve(std::strlen(text));
  while (*text) {
    if (*text > ' ') {
      v.push_back(Sprite(texFont));
      v.back().Position(offset);
      v.back().Color(color);
      SetChar(v.back(), *text);
      node.AddChild(&v.back());
    }
    offset.x += fontSize.x;
    ++text;
  }
}

} // namespace Font