/**
* @file Character.cpp
*/
#include "Character.h"

/**
* �Փ˔���t���X�v���C�g���쐬����.
*/
CharacterPtr Character::Create(const TexturePtr& tex, const glm::vec3& pos, const CollisionRect& body, int hp)
{
  return std::make_shared<Character>(tex, pos, body, hp);
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

/**
* �Փˏ�Ԃ𒲂ׂ�.
*
* @param lhs ���ӂ̏Փ˔���Ώ�.
* @param rhs �E�ӂ̏Փ˔���Ώ�.
*
* @retval true �Փ˂��Ă���.
* @retval false �Փ˂��Ă��Ȃ�.
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
