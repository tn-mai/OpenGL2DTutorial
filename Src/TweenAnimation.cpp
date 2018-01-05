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
  currentStartRatio = currentEndRatio;
  currentEndRatio += seq[index]->Duration() * reciprocalDuration;
  currentReciprocalRange = 1.0f / (currentEndRatio - currentStartRatio);
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
  index = -1;
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
  const glm::f32 ratio = glm::clamp(elapsed * reciprocalDuration, 0.0f, 1.0f);
  tween->Update(node, ratio);
}

} // namespace TweenAnimation