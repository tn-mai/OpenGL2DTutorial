/**
* @file TweenAnimation.cpp
*/
#include "TweenAnimation.h"
#include "Node.h"
#include <algorithm>
#include <iostream>

namespace TweenAnimation {

/**
* 状態を更新する.
*
* @param node    更新対象のノード.
* @param elapsed 経過時間.
*/
void Tween::Step(Node& node, glm::f32 elapsed)
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
    /* 何もしない */
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
* コンストラクタ.
*
* @param time  動作時間.
* @param ofs   移動先の相対座標.
*/
MoveBy::MoveBy(glm::f32 time, const glm::vec3& ofs, Target t)
  : Tween(time)
  , offset(ofs)
  , target(t)
{
}

/**
* 移動状態を初期化する.
*
* @param node 制御対象のノード.
*/
void MoveBy::Initialize(Node& node)
{
  Tween::Initialize(node);
  start = node.Position();
}

/**
* 移動状態を更新する.
*
* @param node    更新対象のノード.
* @param elapsed 経過時間.
*/
void MoveBy::Update(Node& node, glm::f32 elapsed)
{
  const glm::f32 ratio = elapsed * ReciprocalUnitDuration();
  const glm::vec3 cur = start + glm::mix(glm::vec3(), offset, ratio);
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
* 次のトウィーニングオブジェクトを設定する.
*
* @param node 制御対象のノード.
*/
bool Sequence::NextTween(Node& node)
{
  if (index >= static_cast<int>(seq.size() - 1)) {
    return false;
  }
  ++index;
  currentDurationStart = currentDurationEnd;
  currentDurationEnd += seq[index]->TotalDuration();
  seq[index]->Initialize(node);
  return true;
}

/**
* トウィーニング列を初期化する.
*
* @param node 制御対象のノード.
*/
void Sequence::Initialize(Node& node)
{
  Tween::Initialize(node);
  index = -1;
  currentDurationEnd = 0.0f;
  NextTween(node);
}

/**
* トウィーニング列を更新する.
*
* @param node    更新対象のノード.
* @param elapsed 経過時間.
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
* 状態を更新する.
*
* @param node  更新対象のノード.
* @param dt    前回の更新からの経過時間.
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
* 移動状態を初期化する.
*
* @param node 制御対象のノード.
*/
void Parallelize::Initialize(Node& node)
{
  Tween::Initialize(node);
  for (auto& e : tweens) {
    e->Initialize(node);
  }
}

/**
* 移動状態を更新する.
*
* @param node    更新対象のノード.
* @param elapsed 経過時間.
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