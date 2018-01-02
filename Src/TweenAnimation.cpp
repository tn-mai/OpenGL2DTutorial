/**
* @file TweenAnimation.cpp
*/
#include "TweenAnimation.h"
#include "Sprite.h"

namespace TweenAnimation {

/**
* �R���X�g���N�^.
*
* @param time  ���쎞��.
* @param goal  �ړ���̍��W.
*/
Move::Move(glm::f32 time, const glm::vec3& goal)
  : Tweening(time)
  , goal(goal)
{
}

/**
* �ړ���Ԃ�����������.
*
* @param sprite �ΏۂƂȂ�X�v���C�g.
*/
void Move::Initialize(Sprite& sprite)
{
  start = sprite.Position();
}

/**
* �ړ���Ԃ��X�V����.
*
* @param sprite  �X�V����X�v���C�g.
* @param ratio   �n�_�E�I�_�Ԃ̔䗦.
*/
void Move::Update(Sprite& sprite, glm::f32 ratio)
{
  sprite.Position(glm::mix(start, goal, ratio));
}

} // namespace TweenAnimation