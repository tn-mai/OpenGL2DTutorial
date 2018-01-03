/**
* @file TweenAnimation.h
*/
#ifndef TWEENANIMATION_H_INCLUDED
#define TWEENANIMATION_H_INCLUDED
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class Node;

namespace TweenAnimation {

/**
* トウィーニング基本クラス.
*/
class Tween
{
public:
  Tween() = default;
  explicit Tween(glm::f32 d) : duration(d) {}
  Tween(const Tween&) = default;
  Tween& operator=(const Tween&) = default;
  virtual ~Tween() = default;

  glm::f32 Duration() const { return duration; }
  void Duration(glm::f32 d) { duration = d; }

  virtual void Initialize(Node&) {}
  virtual void Update(Node&, glm::f32 ratio) = 0;
  virtual void Finalize(Node&) {}
  virtual glm::f32 TotalDuration() const { return duration; }

private:
  glm::f32 duration = 1.0f; ///< 動作時間.
};
typedef std::shared_ptr<Tween> TweenPtr;

/**
* スプライトのトウィーンアニメーションを制御するクラス.
*/
class Animate
{
public:
  Animate() = default;
  ~Animate() = default;
  explicit Animate(const TweenPtr& p) { FirstTween(p); }
  Animate(const Animate&) = default;
  Animate& operator=(const Animate&) = default;

  void FirstTween(const TweenPtr& p) {
    firstTween = p;
    currentTween = nullptr;
    totalDuration = p->TotalDuration();
    reciprocalCurrentDuration = 1.0f / p->Duration();
    totalElapsed = 0.0f;
    currentElapsed = 0.0f;
  }
  const TweenPtr& FirstTween() const { return firstTween; }
  const TweenPtr& CurrentTween() const { return currentTween; }

  void Speed(glm::f32 s) { speed = s; }
  glm::f32 Speed() const { return speed; }
  glm::f32 ElapsedTime() const { return totalElapsed; }
  void Pause() { isPause = true; }
  void Resume() { isPause = false; }
  void Loop(bool f) { isLoop = f; }
  bool IsLoop() const { return isLoop; }

  void Initialize(Node&);
  void Update(Node&, glm::f32);

private:
  glm::f32 speed = 1.0f; ///< タイムラインの進行速度.
  glm::f32 totalDuration = 0.0f; ///< 総時間.
  glm::f32 totalElapsed = 0.0f;
  glm::f32 reciprocalCurrentDuration = 0.0f;
  glm::f32 currentElapsed = 0.0f; ///< 経過時間.
  bool isPause = false; ///< 時間経過を一時停止するかどうか.
  bool isLoop = true; ///< ループ再生を行うかどうか.

  TweenPtr firstTween;
  TweenPtr currentTween;
};
typedef std::shared_ptr<Animate> AnimatePtr;

/**
* スプライトの移動アニメーション.
*/
class Move : public Tween
{
public:
  Move() = default;
  Move(glm::f32 time, const glm::vec3& goal);
  Move(const Move&) = default;
  Move& operator=(const Move&) = default;
  ~Move() = default;

  virtual void Initialize(Node&) override;
  virtual void Update(Node&, glm::f32) override;

private:
  glm::vec3 start; ///< 移動開始座標.
  glm::vec3 goal; ///< 移動終了座標.
};

} // namespace TweenAnimation

#endif // TWEENANIMATION_H_INCLUDED