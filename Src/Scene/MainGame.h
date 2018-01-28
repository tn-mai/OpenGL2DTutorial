/**
* @file MainGame.cpp
*/
#ifndef SCENE_MAINGAME_H_INCLUDED
#define SCENE_MAINGAME_H_INCLUDED
#include "../Scene.h"
#include "../Sprite.h"
#include <memory>

namespace Scene {

struct CollisionRect
{
  glm::vec2 lt;
  glm::vec2 rb;
};

class CollidableSprite : public Sprite
{
public:
  static std::shared_ptr<CollidableSprite> create(const TexturePtr& tex, const glm::vec3& pos, const CollisionRect& body, int hp = 1);

  virtual void Update(glm::f32 dt) override {
    Sprite::Update(dt);
    const glm::vec2 pos = WorldPosition();
    worldBody.lt = localBody.lt + pos;
    worldBody.rb = localBody.rb + pos;
  }
  const CollisionRect& Body() const { return worldBody; }
  int Health() const { return health; }
  void Health(int n) { health = n; }
  int SubtractHealth(int n) {
    if (health >= n) {
      health -= n;
    } else {
      health = 0;
    }
    return health;
  }
  void Die() { health = 0; }
  void CountervailingHealth(CollidableSprite& e) {
    const auto h0 = health;
    SubtractHealth(e.health);
    e.SubtractHealth(h0);
  }
  bool IsDead() const { return health <= 0; }

private:
  CollidableSprite(const TexturePtr& tex, const glm::vec3 pos, const CollisionRect& body, int hp = 1) :
    Sprite(tex),
    localBody(body),
    worldBody(body),
    health(hp)
  {
    Position(pos);
  }

  CollisionRect localBody;
  CollisionRect worldBody;
  int health;
};
using NodePtr = std::shared_ptr<Node>;
using CollidableSpritePtr = std::shared_ptr<CollidableSprite>;

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
  TexturePtr tex;
  TexturePtr texBg;
  TimelineList timelineList;
  CollidableSpritePtr sprite;
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