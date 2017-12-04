/**
* @file FrameAnimation.h
*/
#ifndef FRAMEANIMATION_H_INCLUDED
#define FRAMEANIMATION_H_INCLUDED
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class Sprite;

namespace FrameAnimation {

/**
* ����u�ԂɃX�v���C�g�Ƃ��ĕ\������e�N�X�`�����͈̔�.
*/
struct KeyFrame
{
  glm::f32 time; ///< �K�p�J�n����.
  glm::vec2 origin; ///< ���_.
  glm::vec2 size; ///< �傫��.
};

/**
* �L�[�t���[�������Ԃɉ����ĕ��ׂ��f�[�^�^.
*/
struct Timeline
{
  std::vector<KeyFrame> data; ///< �������ɐ��񂳂�Ă���L�[�t���[���̔z��.
};

/// Timeline�|�C���^�^.
typedef std::shared_ptr<Timeline> TimelinePtr;

/**
* �X�v���C�g�̃A�j���[�V�����𐧌䂷��N���X.
*/
class Animate
{
public:
  Animate() = default;
  explicit Animate(const TimelinePtr&);
  ~Animate() = default;
  Animate(const Animate&) = default;
  Animate& operator=(const Animate&) = default;

  void Timeline(const TimelinePtr& list) { timeline = list; }
  const TimelinePtr& Timeline() const { return timeline; }
  void Speed(glm::f32 s) { speed = s; }
  glm::f32 Speed() const { return speed; }
  glm::f32 ElapsedTime() const { return elapsedTime; }
  glm::u32 KeyFrameIndex() const { return keyFrameIndex; }
  void Pause() { isPause = true; }
  void Resume() { isPause = false; }
  void Loop(bool f) { isLoop = f; }
  bool IsLoop() const { return isLoop; }

  void Update(Sprite&, glm::f32);

private:
  TimelinePtr timeline; ///< �������ɐ��񂳂�Ă���L�[�t���[���̔z��.
  glm::f32 speed = 1.0f; ///< �^�C�����C���̐i�s���x.
  glm::f32 elapsedTime = 0.0f; ///< �o�ߎ���.
  glm::u32 keyFrameIndex = 0; ///< ���ݓK�p�ΏۂƂȂ��Ă���^�C�����C�����̃L�[�t���[�����w���C���f�b�N�X.
  bool isPause = false; ///< ���Ԍo�߂��ꎞ��~���邩�ǂ���.
  bool isLoop = true; ///< ���[�v�Đ����s�����ǂ���.
};

/// Animate�|�C���^�^.
typedef std::shared_ptr<Animate> AnimatePtr;

} // namespace FrameAnimation

#endif // FRAMEANIMATION_H_INCLUDED