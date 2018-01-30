/**
* @file Boss.cpp
*/
#include "Boss.h"
#include <glm/gtc/matrix_transform.hpp>
#include <functional>

namespace GameObject {

class Escort : public Character
{
public:
  Escort(const TexturePtr& tex, const glm::vec3& pos, std::function<void(const CharacterPtr&)> shotFunc) :
    Character(tex, pos, { {-12, 12}, {12, -12} }, 10),
    shotFunc(shotFunc)
  {}

  void AimingShot(const NodePtr& target, glm::f32 velocity, int attack)
  {
    const glm::vec3 pos = WorldPosition();
    auto shot = Character::create(Texture(), pos, { {-4, 4 },{ 4, -4 }  }, attack);
    const glm::vec3 direction = glm::normalize(target->WorldPosition() - pos);
    const glm::f32 rot = glm::acos(glm::dot(glm::vec3(-1, 0, 0), direction));
    auto tween = std::make_shared<TweenAnimation::Parallelize>();
    tween->Add(std::make_shared<TweenAnimation::MoveBy>(2000.0f / velocity, direction * 2000.0f));
    tween->Add(std::make_shared<RemoveIfOutOfArea>(Rect{ glm::vec2(-400, -300), glm::vec2(800, 600) }));
    shot->Tweener(std::make_shared<TweenAnimation::Animate>(tween));
    shot->Rectangle({ {512 - 32 - 16, 0},{16, 16} });
    shot->Rotation(rot);
    shot->Name("shot(e)");
    shotFunc(shot);
  }
private:
  std::function<void(const CharacterPtr&)> shotFunc;
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

class BossShot : public TweenAnimation::Tween
{
public:
  BossShot() : Tween(0, TweenAnimation::EasingType::Linear) {}
  virtual void Update(Node& node, glm::f32 elapsed) override
  {
    Boss& boss = static_cast<Boss&>(node);
    if (boss.Health() >= 50) {
      return;
    }
    const glm::vec3 pos = boss.WorldPosition();
    for (glm::f32 d = 0; d < 360.0f; d += 360.0f / 16.0f) {
      auto shot = Character::create(boss.Texture(), pos, { {-2, 2 },{ 2, -2 } }, 2);
      const glm::f32 rot = glm::radians(degree + d);
      const glm::vec3 direction = glm::rotate(glm::mat4(), rot, glm::vec3(0, 0, 1)) * glm::vec4(-1, 0, 0, 1);
      const glm::f32 velocity = 100.0f;
      auto tween = std::make_shared<TweenAnimation::Parallelize>();
      tween->Add(std::make_shared<TweenAnimation::MoveBy>(2000.0f / velocity, direction * 2000.0f));
      tween->Add(std::make_shared<RemoveIfOutOfArea>(Rect{ glm::vec2(-400, -300), glm::vec2(800, 600) }));
      shot->Tweener(std::make_shared<TweenAnimation::Animate>(tween));
      shot->Rectangle({ {512 - 32 - 16 - 16, 0},{16, 16} });
      shot->Name("shot(boss)");
      boss.Shot(shot);
    }
    degree += 360.0f / 64.0f;
    if (degree >= 360.0f) {
      degree -= 360.0f;
    }
  }
private:
  glm::f32 degree = 0;
};

/**
* コンストラクタ.
*/
Boss::Boss(
  const TexturePtr& tex,
  const NodePtr& player,
  std::vector<CharacterPtr>& enemyList,
  std::vector<CharacterPtr>& enemyShotList,
  std::vector<FrameAnimation::TimelinePtr>& timelineList
) :
  Character(tex, glm::vec3(256, 0, 0), { {-48, 112},{48, -112} }, 100),
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
    CharacterPtr escort = std::make_shared<Escort>(tex, pos, [this](const CharacterPtr& shot) { Shot(shot); });
    escort->Name("escort");
    escort->Animator(std::make_shared<FrameAnimation::Animate>(timelineList[0]));
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
    auto seqShot = std::make_shared<TA::Sequence>();
    seqShot->Add(std::make_shared<TA::Wait>(0.5f));
    seqShot->Add(std::make_shared<BossShot>());
    auto palBoss = std::make_shared<TA::Parallelize>();
    palBoss->Add(seqBoss);
    palBoss->Add(seqShot);
    TA::AnimatePtr tweenBoss = std::make_shared<TA::Animate>(palBoss);
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

} // namespace GameObject