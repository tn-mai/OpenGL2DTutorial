/**
* @file TweenAnimation.cpp
*/
#include "TweenAnimation.h"
#include "Node.h"
#include <algorithm>

namespace TweenAnimation {

/**
* コンストラクタ.
*
* @param time  動作時間.
* @param ofs   移動先の相対座標.
*/
Move::Move(glm::f32 time, const glm::vec3& ofs)
  : Tween(time)
  , offset(ofs)
{
}

/**
* 移動状態を初期化する.
*
* @param sprite 対象となるノード.
*/
void Move::Initialize(Node& node)
{
  start = node.Position();
}

/**
* 移動状態を更新する.
*
* @param sprite  更新するノード.
* @param ratio   始点・終点間の比率.
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
* トウィーニング列を更新する.
*
* @param sprite  更新するノード.
* @param ratio   始点・終点間の比率.
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
* 状態を更新する.
*
* @param sprite  更新するノード.
*/
void Animate::Initialize(Node& node)
{
  currentTween->Initialize(node);
}

/**
* 状態を更新する.
*
* @param sprite  更新するノード.
* @param dt      前回の更新からの経過時間.
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