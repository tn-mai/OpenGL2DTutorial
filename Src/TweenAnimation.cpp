/**
* @file TweenAnimation.cpp
*/
#include "TweenAnimation.h"
#include "Node.h"
#include <algorithm>
#include <iostream>

namespace TweenAnimation {

/**
* �R���X�g���N�^.
*
* @param d  ���쎞��.
* @param e  ��ԕ��@.
* @param t  ���[�v��.
*/
Tween::Tween(glm::f32 d, EasingType e, glm::u32 t) :
  duration(d),
  reciprocalDuration(1.0f / d),
  times(static_cast<float>(t)),
  easing(e)
{
}

/**
* ��Ԃ��X�V����.
*
* @param node    �X�V�Ώۂ̃m�[�h.
* @param elapsed �o�ߎ���.
*/
void Tween::UpdateWithEasing(Node& node, glm::f32 elapsed)
{
  const glm::u32 current = static_cast<glm::u32>(elapsed * ReciprocalUnitDuration());
  for (glm::u32 i = total; i < current; ++i) {
    Update(node, UnitDuration());
    Initialize(node);
  }
  total = current;
  glm::f32 ratio = std::fmod(elapsed, UnitDuration()) * ReciprocalUnitDuration();
  switch (easing) {
  default:
  case EasingType::Linear:
    /* �������Ȃ� */
    break;
  case EasingType::EaseIn:
    ratio *= ratio;
    break;
  case EasingType::EaseOut:
    ratio = 2.0f * ratio - ratio * ratio;
    break;
  case EasingType::EaseInOut:
    ratio *= 2.0f;
    if (ratio < 1.0f) {
      ratio *= ratio;
    } else {
      ratio -= 1.0f;
      ratio = 2.0f * ratio - ratio * ratio;
      ratio += 1.0f;
    }
    ratio *= 0.5f;
    break;
  }
  Update(node, UnitDuration() * ratio);
}

/**
* ���䂷��g�E�B�[�j���O�I�u�W�F�N�g��ݒ肷��.
*
* @param p �g�E�B�[�j���O�I�u�W�F�N�g�ւ̃|�C���^.
*/
void Animate::Tween(const TweenPtr& p)
{
  tween = p;
  elapsed = 0.0f;
  isInitialized = false;
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
    tween->UpdateWithEasing(node, tween->TotalDuration());
    tween->Initialize(node);
    elapsed -= tween->TotalDuration();
  }
  tween->UpdateWithEasing(node, elapsed);
}

/**
* �R���X�g���N�^.
*
* @param d  ���쎞��.
* @param v  �ړ����鋗��.
*/
MoveBy::MoveBy(glm::f32 d, const glm::vec3& v, EasingType e, Target t)
  : Tween(d, e)
  , vector(v)
  , target(t)
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
  const glm::f32 ratio = elapsed * ReciprocalUnitDuration();
  const glm::vec3 cur = start + vector * ratio;
  glm::vec3 pos = node.Position();
  if (static_cast<int>(target) & static_cast<int>(Target::X)) {
    pos.x = cur.x;
  }
  if (static_cast<int>(target) & static_cast<int>(Target::Y)) {
    pos.y = cur.y;
  }
  if (static_cast<int>(target) & static_cast<int>(Target::Z)) {
    pos.z = cur.z;
  }
  node.Position(pos);
}

/**
* �g�E�B�[�j���O�I�u�W�F�N�g��ǉ�����.
*
* @param p �ǉ�����g�E�B�[�j���O�I�u�W�F�N�g.
*/
void Sequence::Add(const TweenPtr& p)
{
  seq.push_back(p);
  UnitDuration(UnitDuration() + p->TotalDuration());
}

/**
* ���̃g�E�B�[�j���O�I�u�W�F�N�g��ݒ肷��.
*
* @param node ����Ώۂ̃m�[�h.
*/
bool Sequence::NextTween(Node& node)
{
  if (index >= static_cast<int>(seq.size()) - 1) {
    return false;
  }
  ++index;
  currentDurationBegin = currentDurationEnd;
  currentDurationEnd += seq[index]->TotalDuration();
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
    seq[index]->UpdateWithEasing(node, seq[index]->TotalDuration());
    if (!NextTween(node)) {
      return;
    }
  }
  seq[index]->UpdateWithEasing(node, elapsed - currentDurationBegin);
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
    e->UpdateWithEasing(node, elapsed);
  }
}

} // namespace TweenAnimation