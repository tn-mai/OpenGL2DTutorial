/**
* @file MainGame.cpp
*/
#ifndef SCENE_MAINGAME_H_INCLUDED
#define SCENE_MAINGAME_H_INCLUDED
#include "../Scene.h"
#include "../Sprite.h"
#include "../Character/Character.h"
#include <memory>

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

  void FreeAllDeadSprite();
  void PlayerShot(glm::f32 rot, glm::f32 vel, int atk);
  void EnemyShot(const Sprite& sprite, glm::f32 vel, int atk);

private:
  using NodePtr = Character::NodePtr;
  using CollidableSpritePtr = Character::CollidableSpritePtr;

  TexturePtr tex;
  TexturePtr texBg;
  TimelineList timelineList;
  Character::CollidableSpritePtr sprite;
  Sprite boss;
  Sprite background;
  Node escortNode;

  float invinsibleTimer = 2;
  bool controllable = true;
  bool gameover = false;
  float gameoverTimer = 2;
  std::vector<Sprite> gameoverList;
  int rest = 2;
  std::vector<Sprite> restList;
  int score = 0;
  std::vector<Sprite> scoreList;

  std::vector<NodePtr> nodeList;
  std::vector<CollidableSpritePtr> playerShotList;
  std::vector<CollidableSpritePtr> enemyShotList;
  std::vector<CollidableSpritePtr> enemyList;
};

} // namespace Scene

#endif // SCENE_MAINGAME_H_INCLUDED