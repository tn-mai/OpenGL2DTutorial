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

  virtual void Initialize(Node&) {}
  virtual void Update(Node&, glm::f32 ratio) = 0;
  virtual void Finalize(Node&) {}
  virtual glm::f32 TotalDuration() const { return duration; }

private:
  glm::f32 duration = 1.0f; ///< ���쎞��.
};
typedef std::shared_ptr<Tween> TweenPtr;

/**
* �X�v���C�g�̃g�E�B�[���A�j���[�V�����𐧌䂷��N���X.
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
  glm::f32 speed = 1.0f; ///< �^�C�����C���̐i�s���x.
  glm::f32 totalDuration = 0.0f; ///< ������.
  glm::f32 totalElapsed = 0.0f;
  glm::f32 reciprocalCurrentDuration = 0.0f;
  glm::f32 currentElapsed = 0.0f; ///< �o�ߎ���.
  bool isPause = false; ///< ���Ԍo�߂��ꎞ��~���邩�ǂ���.
  bool isLoop = true; ///< ���[�v�Đ����s�����ǂ���.

  TweenPtr firstTween;
  TweenPtr currentTween;
};
typedef std::shared_ptr<Animate> AnimatePtr;

/**
* �X�v���C�g�̈ړ��A�j���[�V����.
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
  glm::vec3 start; ///< �ړ��J�n���W.
  glm::vec3 goal; ///< �ړ��I�����W.
};

} // namespace TweenAnimation

#endif // TWEENANIMATION_H_INCLUDED