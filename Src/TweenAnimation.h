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
* �g�E�B�[�j���O��{�N���X.
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
  EasingType Easing() const { return easing; }
  void Easing(EasingType type) { easing = type; }

  virtual void Initialize(Node&) {}
  void Step(Node& node, glm::f32 ratio);
  virtual void Update(Node&, glm::f32 ratio) = 0;
  virtual void Finalize(Node&) {}

private:
  glm::f32 duration = 1.0f; ///< ���쎞��.
  EasingType easing = EasingType::Linear;
};
typedef std::shared_ptr<Tween> TweenPtr;

/**
* �m�[�h�̃g�E�B�[���A�j���[�V�����𐧌䂷��N���X.
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
  glm::f32 elapsed = 0.0f; ///< �o�ߎ���.
  bool isInitialized = false;
  bool isPause = false; ///< ���Ԍo�߂��ꎞ��~���邩�ǂ���.
  bool isLoop = true; ///< ���[�v�Đ����s�����ǂ���.

  TweenPtr tween;
};
typedef std::shared_ptr<Animate> AnimatePtr;

/**
* �m�[�h�̈ړ��A�j���[�V����.
*/
class MoveBy : public Tween
{
public:
  MoveBy() = default;
  MoveBy(glm::f32 time, const glm::vec3& ofs);
  MoveBy(const MoveBy&) = default;
  MoveBy& operator=(const MoveBy&) = default;
  virtual ~MoveBy() = default;

  virtual void Initialize(Node&) override;
  virtual void Update(Node&, glm::f32) override;

private:
  glm::vec3 start; ///< �ړ��J�n���W.
  glm::vec3 offset; ///< �ړ��I�����W.
};

/**
* �g�E�B�[�j���O�̗�.
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
  glm::f32 currentDurationStart;
  glm::f32 currentDurationEnd;
  glm::f32 currentReciprocalDuration;
  glm::f32 reciprocalDuration;
};

} // namespace TweenAnimation

#endif // TWEENANIMATION_H_INCLUDED