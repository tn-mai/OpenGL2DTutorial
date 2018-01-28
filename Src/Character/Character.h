/**
* @file Character.h
*/
#ifndef CHARACTER_CHARACTER_H_INCLUDED
#define CHARACTER_CHARACTER_H_INCLUDED
#include "../Sprite.h"

namespace Character {

struct CollisionRect
{
  glm::vec2 lt;
  glm::vec2 rb;
};

/**
* 衝突判定付きスプライト.
*/
class CollidableSprite : public Sprite
{
public:
  static std::shared_ptr<CollidableSprite> create(const TexturePtr& tex, const glm::vec3& pos, const CollisionRect& body, int hp = 1);

  CollidableSprite(const TexturePtr& tex, const glm::vec3 pos, const CollisionRect& body, int hp = 1);
  virtual void Update(glm::f32 dt) override;
  virtual void Die() { health = 0; }
  const CollisionRect& Body() const { return worldBody; }
  int Health() const { return health; }
  void Health(int n) { health = n; }
  int SubtractHealth(int n);
  void CountervailingHealth(CollidableSprite& e);
  bool IsDead() const { return health <= 0; }

private:

  CollisionRect localBody;
  CollisionRect worldBody;
  int health;
};
using NodePtr = std::shared_ptr<Node>;
using CollidableSpritePtr = std::shared_ptr<CollidableSprite>;

/**
* 範囲外に出たら殺す.
*/
class RemoveIfOutOfArea : public TweenAnimation::Tween {
public:
  RemoveIfOutOfArea(const Rect& r);
  virtual void Update(Node& node, glm::f32 dt) override;
private:
  Rect area;
};

} // namespace Character

#endif // CHARACTER_CHARACTER_H_INCLUDED