/**
* @file MainGame.cpp
*/
#ifndef SCENE_MAINGAME_H_INCLUDED
#define SCENE_MAINGAME_H_INCLUDED
#include "../Scene.h"
#include "../Sprite.h"

namespace Scene {

/// タイムラインオブジェクトの配列.
using TimelineList = std::vector<FrameAnimation::TimelinePtr>;

/**
* メインゲーム画面.
*/
class MainGame : public Scene
{
public:
  virtual bool Initialize(Manager& manager) override;
  virtual bool Update(Manager& manager, float dt) override;
  virtual void Finalize(Manager& manager) override;

private:
  TimelineList timelineList;
  Sprite sprite;
  Sprite boss;
  Sprite background;
  Node escortNode;
  std::vector<Sprite> escortList;
};

} // namespace Scene

#endif // SCENE_MAINGAME_H_INCLUDED