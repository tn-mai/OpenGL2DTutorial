/**
* @file MainGame.cpp
*/
#ifndef SCENE_MAINGAME_H_INCLUDED
#define SCENE_MAINGAME_H_INCLUDED
#include "../Scene.h"
#include "../Sprite.h"

namespace Scene {

/// �^�C�����C���I�u�W�F�N�g�̔z��.
using TimelineList = std::vector<FrameAnimation::TimelinePtr>;

/**
* ���C���Q�[�����.
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