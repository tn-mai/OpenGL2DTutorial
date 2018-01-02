/**
* @file TweenAnimation.cpp
*/
#include "TweenAnimation.h"
#include "Sprite.h"

namespace TweenAnimation {

/**
* コンストラクタ.
*
* @param time  動作時間.
* @param goal  移動先の座標.
*/
Move::Move(glm::f32 time, const glm::vec3& goal)
  : Tweening(time)
  , goal(goal)
{
}

/**
* 移動状態を初期化する.
*
* @param sprite 対象となるスプライト.
*/
void Move::Initialize(Sprite& sprite)
{
  start = sprite.Position();
}

/**
* 移動状態を更新する.
*
* @param sprite  更新するスプライト.
* @param ratio   始点・終点間の比率.
*/
void Move::Update(Sprite& sprite, glm::f32 ratio)
{
  sprite.Position(glm::mix(start, goal, ratio));
}

} // namespace TweenAnimation