/**
* @file TweenAnimation.cpp
*/
#include "TweenAnimation.h"
#include "Node.h"
#include <algorithm>
#include <iostream>

namespace TweenAnimation {

/**
* ��Ԃ��X�V����.
*
* @param node    �X�V�Ώۂ̃m�[�h.
* @param elapsed �o�ߎ���.
*/
void Tween::Step(Node& node, glm::f32 elapsed)
{
  const glm::u32 current = static_cast<glm::u32>(elapsed / UnitDuration());
  for (glm::u32 i = total; i < current; ++i) {
    Update(node, UnitDuration());
    Initialize(node);
  }
  total = current;
  glm::f32 ratio = std::fmod(elapsed, UnitDuration()) / UnitDuration();
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
  Update(node, UnitDuration() * ratio);
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
* @param node ����Ώۂ̃m�[�h.
*/
void MoveBy::Initialize(Node& node)
{
  Tween::Initialize(node);
  start = node.Position();
}

/**
* �ړ���Ԃ��X�V����.
*
* @param node    �X�V�Ώۂ̃m�[�h.
* @param elapsed �o�ߎ���.
*/
void MoveBy::Update(Node& node, glm::f32 elapsed)
{
  const glm::f32 ratio = elapsed / TotalDuration();
  node.Position(start + glm::mix(glm::vec3(), offset, ratio));
}

/**
* �R���X�g���N�^.
*
* @param time  ���쎞��.
* @param ofs   �ړ���̑��΍��W.
*/
MoveXBy::MoveXBy(glm::f32 time, glm::f32 ofs)
  : Tween(time)
  , offset(ofs)
{
}

/**
* �ړ���Ԃ�����������.
*
* @param node ����Ώۂ̃m�[�h.
*/
void MoveXBy::Initialize(Node& node)
{
  Tween::Initialize(node);
  start = node.Position().x;
}

/**
* �ړ���Ԃ��X�V����.
*
* @param node    �X�V�Ώۂ̃m�[�h.
* @param elapsed �o�ߎ���.
*/
void MoveXBy::Update(Node& node, glm::f32 elapsed)
{
  const glm::f32 ratio = elapsed / TotalDuration();
  glm::vec3 pos = node.Position();
  pos.x = start + glm::mix(0.0f, offset, ratio);
  node.Position(pos);
}

/**
* �R���X�g���N�^.
*
* @param time  ���쎞��.
* @param ofs   �ړ���̑��΍��W.
*/
MoveYBy::MoveYBy(glm::f32 time, glm::f32 ofs)
  : Tween(time)
  , offset(ofs)
{
}

/**
* �ړ���Ԃ�����������.
*
* @param node ����Ώۂ̃m�[�h.
*/
void MoveYBy::Initialize(Node& node)
{
  Tween::Initialize(node);
  start = node.Position().y;
}

/**
* �ړ���Ԃ��X�V����.
*
* @param node    �X�V�Ώۂ̃m�[�h.
* @param elapsed �o�ߎ���.
*/
void MoveYBy::Update(Node& node, glm::f32 elapsed)
{
  const glm::f32 ratio = elapsed / TotalDuration();
  glm::vec3 pos = node.Position();
  pos.y = start + glm::mix(0.0f, offset, ratio);
  node.Position(pos);
}

/**
* ���̃g�E�B�[�j���O�I�u�W�F�N�g��ݒ肷��.
*
* @param node ����Ώۂ̃m�[�h.
*/
bool Sequence::NextTween(Node& node)
{
  if (index >= static_cast<int>(seq.size() - 1)) {
    return false;
  }
  ++index;
  currentDurationStart = currentDurationEnd;
  currentDurationEnd += seq[index]->TotalDuration();
  currentReciprocalDuration = 1.0f / (currentDurationEnd - currentDurationStart);
  seq[index]->Initialize(node);
  return true;
}

/**
* �g�E�B�[�j���O�������������.
*
* @param node ����Ώۂ̃m�[�h.
*/
void Sequence::Initialize(Node& node)
{
  Tween::Initialize(node);
  index = -1;
  currentDurationEnd = 0.0f;
  NextTween(node);
}

/**
* �g�E�B�[�j���O����X�V����.
*
* @param node    �X�V�Ώۂ̃m�[�h.
* @param elapsed �o�ߎ���.
*/
void Sequence::Update(Node& node, glm::f32 elapsed)
{
  if (seq.empty()) {
    return;
  }
  while (elapsed >= currentDurationEnd) {
    seq[index]->Step(node, seq[index]->TotalDuration());
    if (!NextTween(node)) {
      return;
    }
  }
  seq[index]->Step(node, elapsed - currentDurationStart);
 }

/**
* ��Ԃ��X�V����.
*
* @param node  �X�V�Ώۂ̃m�[�h.
* @param dt    �O��̍X�V����̌o�ߎ���.
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
  if (elapsed >= tween->TotalDuration() && isLoop) {
    tween->Initialize(node);
    elapsed -= tween->TotalDuration();
  }
  tween->Step(node, elapsed);
}

/**
* �ړ���Ԃ�����������.
*
* @param node ����Ώۂ̃m�[�h.
*/
void Parallelize::Initialize(Node& node)
{
  Tween::Initialize(node);
  for (auto& e : tweens) {
    e->Initialize(node);
  }
}

/**
* �ړ���Ԃ��X�V����.
*
* @param node    �X�V�Ώۂ̃m�[�h.
* @param elapsed �o�ߎ���.
*/
void Parallelize::Update(Node& node, glm::f32 elapsed)
{
  if (tweens.empty()) {
    return;
  }
  for (auto& e : tweens) {
    e->Step(node, elapsed);
  }
}

} // namespace TweenAnimation