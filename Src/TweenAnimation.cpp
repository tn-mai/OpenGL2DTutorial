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
  ratio = ratio * times;
  const glm::u32 current = static_cast<glm::u32>(ratio);
  for (glm::u32 i = total; i < current; ++i) {
    Update(node, 1.0f);
    Initialize(node);
  }
  ratio = std::fmod(ratio, 1.0f);
  total = current;
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
  Update(node, ratio);
}

/**
* コンストラクタ.
*
* @param time  動作時間.
* @param ofs   移動先の相対座標.
*/
MoveBy::MoveBy(glm::f32 time, const glm::vec3& ofs)
  : Tween(time)
  , offset(ofs)
{
}

/**
* 移動状態を初期化する.
*
* @param sprite 対象となるノード.
*/
void MoveBy::Initialize(Node& node)
{
  Tween::Initialize(node);
  start = node.Position();
}

/**
* 移動状態を更新する.
*
* @param sprite  更新するノード.
* @param ratio   始点・終点間の比率.
*/
void MoveBy::Update(Node& node, glm::f32 ratio)
{
  node.Position(start + glm::mix(glm::vec3(), offset, ratio));
}

/**
* コンストラクタ.
*
* @param time  動作時間.
* @param ofs   加速度.
*/
AccelBy::AccelBy(glm::f32 time, const glm::vec3& ofs)
  : Tween(time)
  , offset(ofs)
{
}

/**
* 移動状態を初期化する.
*
* @param sprite 対象となるノード.
*/
void AccelBy::Initialize(Node& node)
{
  Tween::Initialize(node);
  v0 = node.Velocity();
  p0 = node.Position();
}

/**
* 加速を更新する.
*
* @param sprite  更新するノード.
* @param ratio   始点・終点間の比率.
*/
void AccelBy::Update(Node& node, glm::f32 ratio)
{
  const float t = TotalDuration() * ratio;
  const glm::vec3 at = offset * t;
  node.Velocity(v0 + at);
  node.Position(p0 + v0 * t + 0.5f * at * t);
}

/**
* 次のトウィーニングオブジェクトを設定する.
*
* @param sprite  操作対象ノード.
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
* トウィーニング列を初期化する.
*
* @param sprite  操作対象ノード.
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
* @param sprite  操作対象ノード.
* @param ratio   始点・終点間の比率.
*/
void Sequence::Update(Node& node, glm::f32 ratio)
{
  if (seq.empty()) {
    return;
  }
  const glm::f32 elapsed = ratio * UnitDuration();
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
* 状態を更新する.
*
* @param sprite  更新するノード.
* @param dt      前回の更新からの経過時間.
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
  const glm::f32 ratio = glm::clamp(elapsed * reciprocalDuration, 0.0f, 1.0f);
  tween->Step(node, ratio);
}

} // namespace TweenAnimation