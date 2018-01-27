/**
* @file Font.h
*/
#include "Sprite.h"
#include <vector>

namespace Font {

bool Init();
void SetChar(Sprite& sprite, int c);
void SetTextSprite(Node& node, std::vector<Sprite>& v, glm::vec3 offset, const char* text, const glm::vec4& color);

} // namespace Font