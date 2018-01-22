/**
* @file Title.h
*/
#ifndef SCENE_TITLE_H_INCLUDED
#define SCENE_TITLE_H_INCLUDED
#include "../Scene.h"
#include "../Sprite.h"

namespace Scene {

/**
* ƒ^ƒCƒgƒ‹‰æ–Ê.
*/
class Title : public Scene {
public:
  virtual bool Initialize(Manager& manager) override;
  virtual bool Update(Manager& manager, float dt) override;
  virtual void Finalize(Manager& manager) override;

private:
  void SetTextSprite(std::vector<Sprite>& v, glm::vec3 offset, const char* text, int colorIndex);

  TexturePtr texBg;
  TexturePtr texTitle;
  TexturePtr texFont;
  Sprite sprBg;
  Sprite sprTitle;
  std::vector<Sprite> sprPushStart;
  std::vector<Sprite> sprExit;
  int select = 0;
};

} // namespace Scene

#endif // SCENE_TITLE_H_INCLUDED