/**
* @file TweenAnimation.h
*/
#ifndef TWEENANIMATION_H_INCLUDED
#define TWEENANIMATION_H_INCLUDED
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class Sprite;

namespace TweenAnimation {

/**
* �g�E�B�[�j���O��{�N���X.
*/
class Tweening
{
public:
  Tweening() = default;
  explicit Tweening(glm::f32 time) : time(time) {}
  Tweening(const Tweening&) = default;
  Tweening& operator=(const Tweening&) = default;
  virtual ~Tweening() = default;

  virtual void Initialize(Sprite&) {}
  virtual void Update(Sprite&, glm::f32 ratio) = 0;
  virtual void Finalize(Sprite&) {}
private:
  glm::f32 time = 1.0f; ///< ���쎞��.
};

/**
* �X�v���C�g�̈ړ��A�j���[�V����.
*/
class Move : public Tweening
{
public:
  Move() = default;
  Move(glm::f32 time, const glm::vec3& goal);
  Move(const Move&) = default;
  Move& operator=(const Move&) = default;
  ~Move() = default;

  virtual void Initialize(Sprite&) override;
  virtual void Update(Sprite&, glm::f32) override;

private:
  glm::vec3 start; ///< �ړ��J�n���W.
  glm::vec3 goal; ///< �ړ��I�����W.
};

} // namespace TweenAnimation

#endif // TWEENANIMATION_H_INCLUDED