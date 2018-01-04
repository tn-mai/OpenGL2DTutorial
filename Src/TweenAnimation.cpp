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
*
*/
glm::f32 Sequence::TotalDuration() const
{
  glm::f32 duration = 0.0f;
  for (const auto& e : seq) {
    duration += e->Duration();
  }
  return duration;
}

/**
* �g�E�B�[�j���O����X�V����.
*
* @param sprite  �X�V����m�[�h.
* @param ratio   �n�_�E�I�_�Ԃ̔䗦.
*/
void Sequence::Update(Node& node, glm::f32 ratio)
{
  if (seq.empty()) {
    return;
  }
  if (index < 0) {
    index = 0;
    currentStartTime = 0.0f;
    currentEndTime = seq[0]->Duration();
    seq[0]->Initialize(node);
  }
  const glm::f32 elapsed = ratio * Duration();
  const int prevIndex = index;
  while (elapsed < currentStartTime && index > 0) {
    --index;
    currentEndTime = currentStartTime;
    currentStartTime -= seq[index]->Duration();
  }
  while (elapsed >= currentEndTime && index < static_cast<int>(seq.size() - 1)) {
    ++index;
    currentStartTime = currentEndTime;
    currentEndTime += seq[index]->Duration();
  }
  if (prevIndex != index) {
    seq[index]->Initialize(node);
  }
  const glm::f32 curElapsed = elapsed - currentStartTime;
  const glm::f32 curDuration = seq[index]->Duration();
  const glm::f32 curRatio = curElapsed / curDuration;
  seq[index]->Update(node, curRatio);
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