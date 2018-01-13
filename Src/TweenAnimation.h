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
  explicit Tween(glm::f32 d, glm::u32 t = 1) : duration(d), reciprocalDuration(1.0f / d), times(static_cast<float>(t)) {}
  Tween(const Tween&) = default;
  Tween& operator=(const Tween&) = default;
  virtual ~Tween() = default;

  glm::f32 TotalDuration() const { return duration * times; }
  glm::f32 UnitDuration() const { return duration; }
  glm::f32 ReciprocalUnitDuration() const { return reciprocalDuration; }
  void UnitDuration(glm::f32 d) {
    duration = d;
    reciprocalDuration = 1.0f / d;
  }
  EasingType Easing() const { return easing; }
  void Easing(EasingType type) { easing = type; }

  virtual void Initialize(Node&) { total = 0; }
  void Step(Node& node, glm::f32 ratio);
  virtual void Update(Node&, glm::f32 ratio) = 0;
  virtual void Finalize(Node&) {}

private:
  glm::f32 duration = 1.0f; ///< 動作時間.
  glm::f32 reciprocalDuration = 1.0f; ///< 動作時間の逆数.
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
    elapsed = 0.0f;
  }
  const TweenPtr& Tween() const { return tween; }

  void Pause() { isPause = true; }
  void Resume() { isPause = false; }
  void Loop(bool f) { isLoop = f; }
  bool IsLoop() const { return isLoop; }

  void Update(Node&, glm::f32);

private:
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

/**
* ノードの移動アニメーション.
*/
class MoveXBy : public Tween
{
public:
  MoveXBy() = default;
  MoveXBy(glm::f32 d, glm::f32 ofs);
  MoveXBy(const MoveXBy&) = default;
  MoveXBy& operator=(const MoveXBy&) = default;
  virtual ~MoveXBy() = default;

  virtual void Initialize(Node&) override;
  virtual void Update(Node&, glm::f32) override;

private:
  glm::f32 start; ///< 移動開始座標.
  glm::f32 offset; ///< 移動終了座標.
};

/**
* ノードの移動アニメーション.
*/
class MoveYBy : public Tween
{
public:
  MoveYBy() = default;
  MoveYBy(glm::f32 d, glm::f32 ofs);
  MoveYBy(const MoveYBy&) = default;
  MoveYBy& operator=(const MoveYBy&) = default;
  virtual ~MoveYBy() = default;

  virtual void Initialize(Node&) override;
  virtual void Update(Node&, glm::f32) override;

private:
  glm::f32 start; ///< 移動開始座標.
  glm::f32 offset; ///< 移動終了座標.
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
  }

  virtual void Initialize(Node&) override;
  virtual void Update(Node&, glm::f32) override;

private:
  bool NextTween(Node&);

  std::vector<TweenPtr> seq;
  int index = -1;
  glm::f32 currentDurationStart;
  glm::f32 currentDurationEnd;
};

/**
* ノードの移動アニメーション.
*/
class Parallelize : public Tween
{
public:
  Parallelize() = default;
  Parallelize(const Parallelize&) = default;
  Parallelize& operator=(const Parallelize&) = default;
  virtual ~Parallelize() = default;

  void Add(const TweenPtr& p) {
    tweens.push_back(p);
    const glm::f32 d = p->TotalDuration();
    if (d > UnitDuration()) {
      UnitDuration(d);
    }
  }

  virtual void Initialize(Node&) override;
  virtual void Update(Node&, glm::f32) override;

private:
  std::vector<TweenPtr> tweens;
};

} // namespace TweenAnimation

#endif // TWEENANIMATION_H_INCLUDED