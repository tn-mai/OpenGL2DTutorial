/**
* @file Character.cpp
*/
#include "Character.h"

namespace Character {

/**
* 衝突判定付きスプライトを作成する.
*/
CollidableSpritePtr CollidableSprite::create(const TexturePtr& tex, const glm::vec3& pos, const CollisionRect& body, int hp)
{
  struct Impl : public CollidableSprite {
    Impl(const TexturePtr& tex, const glm::vec3& pos, const CollisionRect& body, int hp = 1) : CollidableSprite(tex, pos, body, hp) {}
  };
  return std::make_shared<Impl>(tex, pos, body, hp);
}

/**
* コンストラクタ.
*/
CollidableSprite::CollidableSprite(const TexturePtr& tex, const glm::vec3 pos, const CollisionRect& body, int hp) :
  Sprite(tex),
  localBody(body),
  worldBody(body),
  health(hp)
{
  Position(pos);
}

/**
* 更新.
*/
void CollidableSprite::Update(glm::f32 dt)
{
  Sprite::Update(dt);
  const glm::vec2 pos = WorldPosition();
  worldBody.lt = localBody.lt + pos;
  worldBody.rb = localBody.rb + pos;
}

/**
* ヘルス値を減らす.
*
* @param n 減少させる数値.
*
* @return 減少後のヘルス値.
*/
int CollidableSprite::SubtractHealth(int n) {
  if (health > n) {
    health -= n;
  } else {
    Die();
  }
  return health;
}

/**
* ヘルス値を相殺する.
*
* @param e 相殺するオブジェクト.
*/
void CollidableSprite::CountervailingHealth(CollidableSprite& e)
{
  const auto h0 = health;
  SubtractHealth(e.health);
  e.SubtractHealth(h0);
}

} // namespace Character