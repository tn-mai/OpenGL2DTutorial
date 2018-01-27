/**
* @file Font.cpp
*/
#include "Font.h"

namespace Font {

TexturePtr texFont;
static const glm::vec2 fontSize(32, 64);

/**
* 初期化.
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
* 表示する文字を設定する.
*
* @param sprite 表示用スプライト.
* @param c      表示する文字.
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
* テキストをスプライトにする.
*
* @param v          スプライト化したテキストを格納するvector.
* @param offset     スプライト表示開始座標.
* @param text       スプライト化するテキスト.
* @param colorIndex 文字の色を示すインデックス.
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