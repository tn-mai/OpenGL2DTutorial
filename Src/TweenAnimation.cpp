/**
* @file TweenAnimation.cpp
*/
#include "TweenAnimation.h"
#include "Node.h"
#include <algorithm>

namespace TweenAnimation {

/**
*
*/
void Tween::Step(Node& node, glm::f32 ratio)
{
  switch (easing) {
  default:
  case EasingType::Linear:
    /* �������Ȃ� */
    break;
  case EasingType::EaseIn:
    ratio *= ratio;
    break;
  case EasingType::EaseOut:
    ratio *= (2.0f - ratio);
    break;
  case EasingType::EaseInOut:
    if (ratio < 0.5f) {
      ratio *= 2.0f;
      ratio *= ratio;
    } else {
      ratio = (ratio - 0.5f) * 2.0f;
      ratio *= (2.0f - ratio);
      ratio += 1.0f;
    }
    ratio *= 0.5f;
    break;
  }
  Update(node, ratio);
}

/**
* �R���X�g���N�^.
*
* @param time  ���쎞��.
* @param ofs   �ړ���̑��΍��W.
*/
MoveBy::MoveBy(glm::f32 time, const glm::vec3& ofs)
  : Tween(time)
  , offset(ofs)
{
}

/**
* �ړ���Ԃ�����������.
*
* @param sprite �ΏۂƂȂ�m�[�h.
*/
void MoveBy::Initialize(Node& node)
{
  start = node.Position();
}

/**
* �ړ���Ԃ��X�V����.
*
* @param sprite  �X�V����m�[�h.
* @param ratio   �n�_�E�I�_�Ԃ̔䗦.
*/
void MoveBy::Update(Node& node, glm::f32 ratio)
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
  currentDurationStart = currentDurationEnd;
  currentDurationEnd += seq[index]->Duration();
  currentReciprocalDuration = 1.0f / (currentDurationEnd - currentDurationStart);
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
  currentDurationEnd = 0.0f;
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
  const glm::f32 elapsed = ratio * Duration();
  while (elapsed >= currentDurationEnd) {
    seq[index]->Update(node, 1.0f);
    if (!NextTween(node)) {
      break;
    }
  }
  const glm::f32 currentRatio = glm::clamp((elapsed - currentDurationStart) * currentReciprocalDuration, 0.0f, 1.0f);
  seq[index]->Step(node, currentRatio);
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
  if (elapsed >= tween->Duration() && isLoop) {
    tween->Initialize(node);
    elapsed -= tween->Duration();
  }
  const glm::f32 ratio = glm::clamp(elapsed * reciprocalDuration, 0.0f, 1.0f);
  tween->Step(node, ratio);
}

} // namespace TweenAnimation