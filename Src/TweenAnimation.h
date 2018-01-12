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

enum class EasingType {
  Linear,
  EaseIn,
  EaseOut,
  EaseInOut,
};

/**
* トウィーニング基本クラス.
*/
class Tween
{
public:
  Tween() = default;
  explicit Tween(glm::f32 d, glm::u32 t = 1) : duration(d), times(static_cast<float>(t)) {}
  Tween(const Tween&) = default;
  Tween& operator=(const Tween&) = default;
  virtual ~Tween() = default;

  glm::f32 TotalDuration() const { return duration * times; }
  glm::f32 UnitDuration() const { return duration; }
  void UnitDuration(glm::f32 d) { duration = d; }
  EasingType Easing() const { return easing; }
  void Easing(EasingType type) { easing = type; }

  virtual void Initialize(Node&) { total = 0; }
  void Step(Node& node, glm::f32 ratio);
  virtual void Update(Node&, glm::f32 ratio) = 0;
  virtual void Finalize(Node&) {}

private:
  glm::f32 duration = 1.0f; ///< 動作時間.
  glm::f32 times = 1.0f;
  EasingType easing = EasingType::Linear;
  glm::u32 total = 0;
};
typedef std::shared_ptr<Tween> TweenPtr;

/**
* ノードのトウィーンアニメーションを制御するクラス.
*/
class Animate
{
public:
  Animate() = default;
  ~Animate() = default;
  explicit Animate(const TweenPtr& p) { Tween(p); }
  Animate(const Animate&) = default;
  Animate& operator=(const Animate&) = default;

  void Tween(const TweenPtr& p) {
    tween = p;
    reciprocalDuration = 1.0f / p->TotalDuration();
    elapsed = 0.0f;
  }
  const TweenPtr& Tween() const { return tween; }

  void Pause() { isPause = true; }
  void Resume() { isPause = false; }
  void Loop(bool f) { isLoop = f; }
  bool IsLoop() const { return isLoop; }

  void Update(Node&, glm::f32);

private:
  glm::f32 reciprocalDuration = 0.0f;
  glm::f32 elapsed = 0.0f; ///< 経過時間.
  bool isInitialized = false;
  bool isPause = false; ///< 時間経過を一時停止するかどうか.
  bool isLoop = true; ///< ループ再生を行うかどうか.

  TweenPtr tween;
};
typedef std::shared_ptr<Animate> AnimatePtr;

/**
* ノードの移動アニメーション.
*/
class MoveBy : public Tween
{
public:
  MoveBy() = default;
  MoveBy(glm::f32 d, const glm::vec3& ofs);
  MoveBy(const MoveBy&) = default;
  MoveBy& operator=(const MoveBy&) = default;
  virtual ~MoveBy() = default;

  virtual void Initialize(Node&) override;
  virtual void Update(Node&, glm::f32) override;

private:
  glm::vec3 start; ///< 移動開始座標.
  glm::vec3 offset; ///< 移動終了座標.
};

class AccelBy : public Tween
{
public:
  AccelBy() = default;
  AccelBy(glm::f32 d, const glm::vec3& ofs);
  AccelBy(const AccelBy&) = default;
  AccelBy& operator=(const AccelBy&) = default;
  virtual ~AccelBy() = default;

  virtual void Initialize(Node&) override;
  virtual void Update(Node&, glm::f32) override;

private:
  glm::vec3 v0;
  glm::vec3 p0;
  glm::vec3 offset;
};

/**
* トウィーニングの列.
*/
class Sequence : public Tween
{
public:
  Sequence(glm::u32 t = 1) : Tween(0.0f, t) {}
  Sequence(const Sequence&) = default;
  Sequence& operator=(const Sequence&) = default;
  virtual ~Sequence() = default;

  void Add(const TweenPtr& p) {
    seq.push_back(p);
    UnitDuration(UnitDuration() + p->TotalDuration());
    reciprocalDuration = 1.0f / TotalDuration();
  }

  virtual void Initialize(Node&) override;
  virtual void Update(Node&, glm::f32) override;

private:
  bool NextTween(Node&);

  std::vector<TweenPtr> seq;
  int index = -1;
  glm::f32 currentDurationStart;
  glm::f32 currentDurationEnd;
  glm::f32 currentReciprocalDuration;
  glm::f32 reciprocalDuration;
};

} // namespace TweenAnimation

#endif // TWEENANIMATION_H_INCLUDED