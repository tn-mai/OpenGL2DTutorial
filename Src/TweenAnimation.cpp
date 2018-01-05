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
* ���̃g�E�B�[�j���O�I�u�W�F�N�g��ݒ肷��.
*
* @param sprite  ����Ώۃm�[�h.
*/
bool Sequence::NextTween(Node& node)
{
  if (index >= static_cast<int>(seq.size() - 1)) {
    return false;
  }
  ++index;
  currentStartRatio = currentEndRatio;
  currentEndRatio += seq[index]->Duration() * reciprocalDuration;
  currentReciprocalRange = 1.0f / (currentEndRatio - currentStartRatio);
  seq[index]->Initialize(node);
  return true;
}

/**
* �g�E�B�[�j���O�������������.
*
* @param sprite  ����Ώۃm�[�h.
*/
void Sequence::Initialize(Node& node)
{
  index = -1;
  NextTween(node);
}

/**
* �g�E�B�[�j���O����X�V����.
*
* @param sprite  ����Ώۃm�[�h.
* @param ratio   �n�_�E�I�_�Ԃ̔䗦.
*/
void Sequence::Update(Node& node, glm::f32 ratio)
{
  if (seq.empty()) {
    return;
  }
  while (ratio >= currentEndRatio) {
    seq[index]->Update(node, 1.0f);
    if (!NextTween(node)) {
      break;
    }
  }
  const glm::f32 currentRatio = (ratio - currentStartRatio) * currentReciprocalRange;
  seq[index]->Update(node, currentRatio);
}

/**
* ��Ԃ��X�V����.
*
* @param sprite  �X�V����m�[�h.
* @param dt      �O��̍X�V����̌o�ߎ���.
*/
void Animate::Update(Node& node, glm::f32 dt)
{
  if (!tween) {
    return;
  }
  if (!isInitialized) {
    isInitialized = true;
    tween->Initialize(node);
  }
  elapsed += dt;
  const glm::f32 ratio = glm::clamp(elapsed * reciprocalDuration, 0.0f, 1.0f);
  tween->Update(node, ratio);
}

} // namespace TweenAnimation