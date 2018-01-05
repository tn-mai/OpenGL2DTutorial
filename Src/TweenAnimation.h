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

private:
  glm::f32 duration = 1.0f; ///< 動作時間.
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
    reciprocalDuration = 1.0f / p->Duration();
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
class Move : public Tween
{
public:
  Move() = default;
  Move(glm::f32 time, const glm::vec3& ofs);
  Move(const Move&) = default;
  Move& operator=(const Move&) = default;
  virtual ~Move() = default;

  virtual void Initialize(Node&) override;
  virtual void Update(Node&, glm::f32) override;

private:
  glm::vec3 start; ///< 移動開始座標.
  glm::vec3 offset; ///< 移動終了座標.
};

/**
* トウィーニングの列.
*/
class Sequence : public Tween
{
public:
  Sequence() = default;
  Sequence(const Sequence&) = default;
  Sequence& operator=(const Sequence&) = default;
  virtual ~Sequence() = default;

  void Add(const TweenPtr& p) {
    seq.push_back(p);
    Duration(Duration() + p->Duration());
    reciprocalDuration = 1.0f / Duration();
  }

  virtual void Initialize(Node&) override;
  virtual void Update(Node&, glm::f32) override;

private:
  bool NextTween(Node&);

  std::vector<TweenPtr> seq;
  int index = -1;
  glm::f32 currentStartRatio;
  glm::f32 currentEndRatio;
  glm::f32 currentReciprocalRange;
  glm::f32 reciprocalDuration;
};

} // namespace TweenAnimation

#endif // TWEENANIMATION_H_INCLUDED