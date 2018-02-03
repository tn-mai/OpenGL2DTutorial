/**
* @file Character.cpp
*/
#include "Character.h"

/**
* 衝突判定付きスプライトを作成する.
*/
CharacterPtr Character::Create(const TexturePtr& tex, const glm::vec3& pos, const CollisionRect& body, int hp)
{
  return std::make_shared<Character>(tex, pos, body, hp);
}

/**
* コンストラクタ.
*/
Character::Character(const TexturePtr& tex, const glm::vec3 pos, const CollisionRect& body, int hp) :
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
void Character::Update(glm::f32 dt)
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
int Character::SubtractHealth(int n) {
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
void Character::CountervailingHealth(Character& e)
{
  const auto h0 = health;
  SubtractHealth(e.health);
  e.SubtractHealth(h0);
}

/**
* 衝突状態を調べる.
*
* @param lhs 左辺の衝突判定対象.
* @param rhs 右辺の衝突判定対象.
*
* @retval true 衝突している.
* @retval false 衝突していない.
*/
bool IsCollision(const CharacterPtr& lhs, const CharacterPtr& rhs)
{
  const auto bodyL = lhs->Body();
  const auto bodyR = rhs->Body();
  if (bodyL.lt.x < bodyR.rb.x && bodyL.rb.x > bodyR.lt.x) {
    if (bodyL.lt.y > bodyR.rb.y && bodyL.rb.y < bodyR.lt.y) {
      return true;
    }
  }
  return false;
}
