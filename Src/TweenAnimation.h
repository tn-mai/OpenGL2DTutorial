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
* �C�[�W���O�̎��.
*/
enum class EasingType {
  Linear, ///< ����.
  EaseIn, ///< ����.
  EaseOut, ///< ����.
  EaseInOut, ///< ����+����.
};

/**
* �A�j���[�V������K�p����v�f.
*/
enum class Target {
  X = 1, ///< X�v�f�̂�.
  Y = 2, ///< Y�v�f�̂�.
  XY = 3, ///< X�v�f��Y�v�f.
  Z = 4, ///< Z�v�f�̂�.
  XZ = 5, ///< X�v�f��Z�v�f.
  YZ = 6, ///< Y�v�f��Z�v�f.
  XYZ = 7 ///< �S�Ă̗v�f.
};

/**
* �g�E�B�[�j���O��{�N���X.
*/
class Tween
{
public:
  Tween() = default;
  explicit Tween(glm::f32 d, EasingType e = EasingType::Linear, glm::u32 t = 1);
  Tween(const Tween&) = delete;
  Tween& operator=(const Tween&) = delete;
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

private:
  glm::f32 duration = 1.0f; ///< ���쎞��.
  glm::f32 reciprocalDuration = 1.0f; ///< ���쎞�Ԃ̋t��.
  glm::f32 times = 1.0f;
  EasingType easing = EasingType::Linear;
  glm::u32 total = 0;
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
  Animate(const Animate&) = delete;
  Animate& operator=(const Animate&) = delete;

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
  glm::f32 elapsed = 0.0f; ///< �o�ߎ���.
  bool isInitialized = false;
  bool isPause = false; ///< ���Ԍo�߂��ꎞ��~���邩�ǂ���.
  bool isLoop = true; ///< ���[�v�Đ����s�����ǂ���.

  TweenPtr tween;
};
typedef std::shared_ptr<Animate> AnimatePtr;

/**
* �ړ��A�j���[�V����.
*/
class MoveBy : public Tween
{
public:
  MoveBy() = default;
  MoveBy(glm::f32 d, const glm::vec3& v, EasingType e = EasingType::Linear, Target t = Target::XYZ);
  MoveBy(const MoveBy&) = delete;
  MoveBy& operator=(const MoveBy&) = delete;
  virtual ~MoveBy() = default;

  virtual void Initialize(Node&) override;
  virtual void Update(Node&, glm::f32) override;

private:
  glm::vec3 start; ///< �ړ��J�n���W.
  glm::vec3 vector; ///< �ړ����鋗��.
  Target target = Target::XYZ; ///< ����Ώ�.
};

/**
* �g�E�B�[�j���O�̗�.
*/
class Sequence : public Tween
{
public:
  explicit Sequence(glm::u32 t = 1) : Tween(0.0f, EasingType::Linear, t) {}
  Sequence(const Sequence&) = delete;
  Sequence& operator=(const Sequence&) = delete;
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
* �m�[�h�̈ړ��A�j���[�V����.
*/
class Parallelize : public Tween
{
public:
  explicit Parallelize(glm::u32 t = 1) : Tween(0.0f, EasingType::Linear, t) {}
  Parallelize(const Parallelize&) = delete;
  Parallelize& operator=(const Parallelize&) = delete;
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