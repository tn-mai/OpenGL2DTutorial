/**
* @file SpriteAnimation.cpp
*/
#include "FrameAnimation.h"
#include "Sprite.h"
#include <algorithm>
#include <math.h>

namespace FrameAnimation {

/**
* �R���X�g���N�^.
*
* tl
*/
Animate::Animate(const TimelinePtr& tl)
  : timeline(tl)
{
}

/**
* �A�j���[�V�������X�V����.
*
* @param targe �A�j���[�V�����̓K�p�ΏۂƂȂ�X�v���C�g.
* @param delta �O��̍X�V����̌o�ߎ���(�b).
*/
void Animate::Update(Sprite& target, glm::f32 delta)
{
  if (!timeline) {
    return;
  }
  if (!isPause) {
    elapsedTime += delta * speed;
    const glm::f32 totalTime = timeline->data.back().time;
    if (elapsedTime < 0 || elapsedTime >= totalTime) {
      if (!isLoop) {
        elapsedTime = std::min(std::max(elapsedTime, 0.0f), totalTime);
      } else {
        elapsedTime -= std::floor(elapsedTime / totalTime) * totalTime;
      }
    }
    auto itr = std::partition_point(timeline->data.begin(), timeline->data.end(), [this](const KeyFrame& e) { return e.time <= elapsedTime; });
    keyFrameIndex = std::max(itr - timeline->data.begin() - 1, 0);
  }

  const KeyFrame& keyframe = timeline->data[keyFrameIndex];
  target.Rectangle({ keyframe.origin, keyframe.size });
}

} // namespace FrameAnimation
