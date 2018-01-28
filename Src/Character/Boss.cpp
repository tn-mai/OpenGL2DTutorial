/**
* @file Boss.cpp
*/
#include "Boss.h"
#include <glm/gtc/matrix_transform.hpp>
#include <functional>

namespace Character {

class Escort : public CollidableSprite
{
public:
  Escort(const TexturePtr& tex, const glm::vec3& pos, std::function<void(const CollidableSpritePtr&)> shotFunc) :
    CollidableSprite(tex, pos, { {-12, 12}, {12, -12} }, 10),
    shotFunc(shotFunc)
  {}

  void AimingShot(const NodePtr& target, glm::f32 velocity, int attack)
  {
    const glm::vec3 pos = WorldPosition();
    auto shot = Character::CollidableSprite::create(Texture(), pos, { {-4, 4 },{ 4, -4 }  }, attack);
    const glm::vec3 direction = glm::normalize(target->WorldPosition() - pos);
    const glm::f32 rot = glm::acos(glm::dot(glm::vec3(-1, 0, 0), direction));
    auto tween = std::make_shared<TweenAnimation::Parallelize>();
    tween->Add(std::make_shared<TweenAnimation::MoveBy>(2000.0f / velocity, direction * 2000.0f));
    tween->Add(std::make_shared<Character::RemoveIfOutOfArea>(Rect{ glm::vec2(-400, -300), glm::vec2(800, 600) }));
    shot->Tweener(std::make_shared<TweenAnimation::Animate>(tween));
    shot->Rectangle({ {512 - 32 - 16, 0},{16, 16} });
    shot->Rotation(rot);
    shot->Name("shot(e)");
    shotFunc(shot);
  }
private:
  std::function<void(const CollidableSpritePtr&)> shotFunc;
};

class AimingShot : public TweenAnimation::Tween
{
public:
  AimingShot(const NodePtr& target) : Tween(0, TweenAnimation::EasingType::Linear), target(target) {}
  virtual void Update(Node& node, glm::f32 elapsed) override
  {
    static_cast<Escort&>(node).AimingShot(target, 400, 1);
  }

private:
  NodePtr target;
};

void EnemyShot(const Sprite& enemy, glm::f32 vel, int atk)
{
}

/**
* コンストラクタ.
*/
Boss::Boss(
  const TexturePtr& tex,
  const NodePtr& player,
  std::vector<CollidableSpritePtr>& enemyList,
  std::vector<CollidableSpritePtr>& enemyShotList,
  std::vector<FrameAnimation::TimelinePtr>& timelineList
) :
  CollidableSprite(tex, glm::vec3(256, 0, 0), { {-48, 112},{48, -112} }, 100),
  player(player),
  enemyList(enemyList),
  enemyShotList(enemyShotList)
{
  Rectangle({ glm::vec2(320 ,128), glm::vec2(128, 256) });
  Name("boss");

  {
    escortNode.Position(glm::vec3(-16, 0, 0));
    escortNode.Name("escortNode");
    auto tween = std::make_shared<TweenAnimation::Rotation>(1.0f, glm::radians(25.0f));
    escortNode.Tweener(std::make_shared<TweenAnimation::Animate>(tween));
    AddChild(&escortNode);
  }

  for (size_t i = 0; i < 16; ++i) {
    const auto m = glm::rotate(glm::mat4(), glm::radians(static_cast<float>(i * 360) / 16.0f), glm::vec3(0, 0, 1));
    const glm::vec4 pos = m * glm::vec4(0, 144, 0, 1);
    CollidableSpritePtr escort = std::make_shared<Escort>(tex, pos, [&](const CollidableSpritePtr& shot) {
      enemyShotList.push_back(shot);
      Parent()->AddChild(shot.get());
    });
    escort->Name("escort");
    auto animator = std::make_shared<FrameAnimation::Animate>(timelineList[0]);
    escort->Animator(animator);
    auto tween0 = std::make_shared<TweenAnimation::Parallelize>();
    auto tween1 = std::make_shared<TweenAnimation::Sequence>();
    tween1->Add(std::make_shared<TweenAnimation::Wait>(1.0f));
    tween1->Add(std::make_shared<AimingShot>(player));
    tween0->Add(std::make_shared<TweenAnimation::Rotation>(1.0f, glm::radians(-25.0f)));
    tween0->Add(tween1);
    escort->Tweener(std::make_shared<TweenAnimation::Animate>(tween0));
    enemyList.push_back(escort);
    escortNode.AddChild(escort.get());
  }

  {
    namespace TA = TweenAnimation;
#if 1
    auto moveBoss0 = std::make_shared<TA::MoveBy>(1.5f, glm::vec3(0, -150, 0));
    moveBoss0->Easing(TA::EasingType::EaseOut);
    auto moveBoss1 = std::make_shared<TA::MoveBy>(3.0f, glm::vec3(0, 300, 0));
    moveBoss1->Easing(TA::EasingType::EaseInOut);
    auto moveBoss2 = std::make_shared<TA::MoveBy>(1.5f, glm::vec3(0, -150, 0));
    moveBoss2->Easing(TA::EasingType::EaseIn);
    auto seqBoss = std::make_shared<TA::Sequence>();
    seqBoss->Add(moveBoss0);
    seqBoss->Add(moveBoss1);
    seqBoss->Add(moveBoss2);
    TA::AnimatePtr tweenBoss = std::make_shared<TA::Animate>(seqBoss);
#else
    auto moveBossY0 = std::make_shared<TA::MoveBy>(1.5f, glm::vec3(0, 150, 0), TA::EasingType::EaseOut, TA::Target::Y);
    auto moveBossY1 = std::make_shared<TA::MoveBy>(3.0f, glm::vec3(0, -300, 0), TA::EasingType::EaseInOut, TA::Target::Y);
    auto moveBossY2 = std::make_shared<TA::MoveBy>(1.5f, glm::vec3(0, 150, 0), TA::EasingType::EaseIn, TA::Target::Y);

    auto moveBossX0 = std::make_shared<TA::MoveBy>(0.75f, glm::vec3(-50, 0, 0), TA::EasingType::EaseOut, TA::Target::X);
    auto moveBossX1 = std::make_shared<TA::MoveBy>(1.5f, glm::vec3(100, 0, 0), TA::EasingType::EaseInOut, TA::Target::X);
    auto moveBossX2 = std::make_shared<TA::MoveBy>(0.75f, glm::vec3(-50, 0, 0), TA::EasingType::EaseIn, TA::Target::X);

    auto seqBossY = std::make_shared<TA::Sequence>(50);
    seqBossY->Add(moveBossY0);
    seqBossY->Add(moveBossY1);
    seqBossY->Add(moveBossY2);
    auto seqBossX = std::make_shared<TA::Sequence>(100);
    seqBossX->Add(moveBossX0);
    seqBossX->Add(moveBossX1);
    seqBossX->Add(moveBossX2);
    auto palBoss = std::make_shared<TA::Parallelize>();
    palBoss->Add(seqBossX);
    palBoss->Add(seqBossY);
    TA::AnimatePtr tweenBoss = std::make_shared<TA::Animate>(palBoss);
#endif
    Tweener(tweenBoss);
  }
}

} // namespace Character