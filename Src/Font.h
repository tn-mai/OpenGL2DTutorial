/**
* @file Font.h
*/
#include "Sprite.h"
#include <deque>

namespace Font {

using SpriteList = std::deque<Sprite>;

bool Init();
void SetChar(Sprite& sprite, int c);
void SetTextSprite(Node& node, SpriteList& v, glm::vec3 offset, const char* text, const glm::vec4& color);

} // namespace Font