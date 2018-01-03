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