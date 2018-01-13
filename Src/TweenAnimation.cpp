/**
* @file TweenAnimation.cpp
*/
#include "TweenAnimation.h"
#include "Node.h"
#include <algorithm>
#include <iostream>

namespace TweenAnimation {

/**
*
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
void MoveBy::Update(Node& node, glm::f32 elapsed)
{
  const glm::f32 ratio = elapsed / TotalDuration();
  node.Position(start + glm::mix(glm::vec3(), offset, ratio));
}

/**
* コンストラクタ.
*
* @param time  動作時間.
* @param ofs   移動先の相対座標.
*/
MoveXBy::MoveXBy(glm::f32 time, glm::f32 ofs)
  : Tween(time)
  , offset(ofs)
{
}

/**
* 移動状態を初期化する.
*
* @param sprite 対象となるノード.
*/
void MoveXBy::Initialize(Node& node)
{
  Tween::Initialize(node);
  start = node.Position().x;
}

/**
* 移動状態を更新する.
*
* @param sprite  更新するノード.
* @param ratio   始点・終点間の比率.
*/
void MoveXBy::Update(Node& node, glm::f32 elapsed)
{
  const glm::f32 ratio = elapsed / TotalDuration();
  glm::vec3 pos = node.Position();
  pos.x = start + glm::mix(0.0f, offset, ratio);
  node.Position(pos);
}

/**
* コンストラクタ.
*
* @param time  動作時間.
* @param ofs   移動先の相対座標.
*/
MoveYBy::MoveYBy(glm::f32 time, glm::f32 ofs)
  : Tween(time)
  , offset(ofs)
{
}

/**
* 移動状態を初期化する.
*
* @param sprite 対象となるノード.
*/
void MoveYBy::Initialize(Node& node)
{
  Tween::Initialize(node);
  start = node.Position().y;
}

/**
* 移動状態を更新する.
*
* @param sprite  更新するノード.
* @param ratio   始点・終点間の比率.
*/
void MoveYBy::Update(Node& node, glm::f32 elapsed)
{
  const glm::f32 ratio = elapsed / TotalDuration();
  glm::vec3 pos = node.Position();
  pos.y = start + glm::mix(0.0f, offset, ratio);
  node.Position(pos);
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

  std::cout << "Animation sequence: " << index << "/" << seq.size() << std::endl;
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
  tween->Step(node, elapsed);
}

/**
* 移動状態を初期化する.
*
* @param sprite 対象となるノード.
*/
void Parallelize::Initialize(Node& node)
{
  Tween::Initialize(node);
}

/**
* 移動状態を更新する.
*
* @param sprite  更新するノード.
* @param ratio   始点・終点間の比率.
*/
void Parallelize::Update(Node& node, glm::f32 ratio)
{
}

} // namespace TweenAnimation