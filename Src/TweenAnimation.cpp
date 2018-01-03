/**
* @file TweenAnimation.cpp
*/
#include "TweenAnimation.h"
#include "Node.h"
#include <algorithm>

namespace TweenAnimation {

/**
* �R���X�g���N�^.
*
* @param time  ���쎞��.
* @param ofs   �ړ���̑��΍��W.
*/
Move::Move(glm::f32 time, const glm::vec3& ofs)
  : Tween(time)
  , offset(ofs)
{
}

/**
* �ړ���Ԃ�����������.
*
* @param sprite �ΏۂƂȂ�m�[�h.
*/
void Move::Initialize(Node& node)
{
  start = node.Position();
}

/**
* �ړ���Ԃ��X�V����.
*
* @param sprite  �X�V����m�[�h.
* @param ratio   �n�_�E�I�_�Ԃ̔䗦.
*/
void Move::Update(Node& node, glm::f32 ratio)
{
  node.Position(start + glm::mix(glm::vec3(), offset, ratio));
}

/**
* ��Ԃ��X�V����.
*
* @param sprite  �X�V����m�[�h.
*/
void Animate::Initialize(Node& node)
{
  currentTween->Initialize(node);
}

/**
* ��Ԃ��X�V����.
*
* @param sprite  �X�V����m�[�h.
* @param dt      �O��̍X�V����̌o�ߎ���.
*/
void Animate::Update(Node& node, glm::f32 dt)
{
  if (!firstTween) {
    return;
  }
  if (!currentTween) {
    currentTween = firstTween;
    currentTween->Initialize(node);
  }
  currentElapsed += dt * speed;
  const glm::f32 ratio = glm::clamp(currentElapsed * reciprocalCurrentDuration, 0.0f, 1.0f);
  currentTween->Update(node, ratio);
}

} // namespace TweenAnimation