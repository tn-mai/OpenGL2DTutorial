/**
* @file Character.cpp
*/
#include "Character.h"

/**
* �R���X�g���N�^.
*/
RemoveIfOutOfArea::RemoveIfOutOfArea(const Rect& r) : Tween(100, TweenAnimation::EasingType::Linear), area(r)
{
}

/**
* �X�V.
*/
void RemoveIfOutOfArea::Update(Node& node, glm::f32 dt)
{
  const glm::vec3 pos = node.Position();
  if (pos.x < area.origin.x || pos.x > area.origin.x + area.size.x ||
    pos.y < area.origin.y || pos.y > area.origin.y + area.size.y) {
    static_cast<Character&>(node).Die();
  }
}

/**
* �Փ˔���t���X�v���C�g���쐬����.
*/
CollidableSpritePtr Character::create(const TexturePtr& tex, const glm::vec3& pos, const CollisionRect& body, int hp)
{
  struct Impl : public Character {
    Impl(const TexturePtr& tex, const glm::vec3& pos, const CollisionRect& body, int hp = 1) : Character(tex, pos, body, hp) {}
  };
  return std::make_shared<Impl>(tex, pos, body, hp);
}

/**
* �R���X�g���N�^.
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
* �X�V.
*/
void Character::Update(glm::f32 dt)
{
  Sprite::Update(dt);
  const glm::vec2 pos = WorldPosition();
  worldBody.lt = localBody.lt + pos;
  worldBody.rb = localBody.rb + pos;
}

/**
* �w���X�l�����炷.
*
* @param n ���������鐔�l.
*
* @return ������̃w���X�l.
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
* �w���X�l�𑊎E����.
*
* @param e ���E����I�u�W�F�N�g.
*/
void Character::CountervailingHealth(Character& e)
{
  const auto h0 = health;
  SubtractHealth(e.health);
  e.SubtractHealth(h0);
}
