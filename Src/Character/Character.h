/**
* @file Character.h
*/
#ifndef CHARACTER_CHARACTER_H_INCLUDED
#define CHARACTER_CHARACTER_H_INCLUDED
#include "../Sprite.h"

/**
* 衝突判定用矩形.
*/
struct CollisionRect
{
  glm::vec2 lt; ///< 左上座標.
  glm::vec2 rb; ///< 右下座標.
};

/**
* 衝突判定付きスプライト.
*/
class Character : public Sprite
{
public:
  static std::shared_ptr<Character> Create(const TexturePtr& tex, const glm::vec3& pos, const CollisionRect& body, int hp = 1);

  Character(const TexturePtr& tex, const glm::vec3 pos, const CollisionRect& body, int hp = 1);
  virtual void Update(glm::f32 dt) override;
  virtual void Die() { health = 0; }
  const CollisionRect& Body() const { return worldBody; }
  int Health() const { return health; }
  void Health(int n) { health = n; }
  int SubtractHealth(int n);
  void CountervailingHealth(Character& e);
  bool IsDead() const { return health <= 0; }

private:

  CollisionRect localBody;
  CollisionRect worldBody;
  int health;
};
using NodePtr = std::shared_ptr<Node>;
using CharacterPtr = std::shared_ptr<Character>;

#endif // CHARACTER_CHARACTER_H_INCLUDED