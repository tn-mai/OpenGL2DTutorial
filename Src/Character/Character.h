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

bool IsCollision(const CharacterPtr& lhs, const CharacterPtr& rhs);

/**
* 衝突判定.
*
* @param b0 左辺の衝突判定対象範囲の先頭.
* @param e0 左辺の衝突判定対象範囲の終端.
* @param b1 右辺の衝突判定対象範囲の先頭.
* @param e1 右辺の衝突判定対象範囲の終端.
* @param solver 衝突を解決する関数オブジェクト.
*/
template<typename Itr0, typename Itr1, typename Func>
void DetectCollision(Itr0 b0, Itr0 e0, Itr1 b1, Itr1 e1, Func solver)
{
  for (Itr0 itr0 = b0; itr0 != e0; ++itr0) {
    if ((*itr0)->IsDead()) {
      continue;
    }
    for (Itr1 itr1 = b1; itr1 != e1; ++itr1) {
      if ((*itr1)->IsDead()) {
        continue;
      }
      if (IsCollision(*itr0, *itr1)) {
        solver(*itr0, *itr1);
        if ((*itr0)->IsDead()) {
          break;
        }
      }
    }
  }
}

#endif // CHARACTER_CHARACTER_H_INCLUDED