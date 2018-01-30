/**
* @file MainGame.cpp
*/
#ifndef SCENE_MAINGAME_H_INCLUDED
#define SCENE_MAINGAME_H_INCLUDED
#include "../Scene.h"
#include "../Sprite.h"
#include "../Character/Player.h"
#include "../Character/Boss.h"
#include "../Font.h"
#include <memory>
#include <random>

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
  void EnemyShot(const Sprite& sprite, glm::f32 vel, int atk);

private:
  TexturePtr tex;
  TexturePtr texBg;
  TimelineList timelineList;
  GameObject::PlayerPtr sprite;
  GameObject::BossPtr boss;
  Sprite background;
  std::vector<Sprite> bgStarList;

  float eventTimer = 2;
  Font::SpriteList eventTextList;
  int rest = 2;
  std::vector<Sprite> restList;
  int score = 0;
  Font::SpriteList scoreList;

  std::vector<NodePtr> nodeList;
  std::vector<CharacterPtr> enemyShotList;
  std::vector<CharacterPtr> enemyList;

  std::mt19937 random;
};

} // namespace Scene

#endif // SCENE_MAINGAME_H_INCLUDED