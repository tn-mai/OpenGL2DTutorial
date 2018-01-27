/**
* @file MainGame.cpp
*/
#include "MainGame.h"
#include "../Font.h"
#include "../GLFWEW.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <iostream>

namespace Scene {

class MoveByVelocity : public TweenAnimation::Tween {
public:
  MoveByVelocity(const glm::vec3& v) : Tween(100, TweenAnimation::EasingType::Linear), velocity(v) {}
  virtual void Update(Node& node, glm::f32 dt) override {
    node.Position(node.Position() + velocity * dt);
  }
private:
  glm::vec3 velocity;
};

class RemoveIfOutOfAraa : public TweenAnimation::Tween {
public:
  RemoveIfOutOfAraa(MainGame* scene, const Rect& r) : Tween(100, TweenAnimation::EasingType::Linear), scene(scene), area(r) {}
  virtual void Update(Node& node, glm::f32 dt) override {
    const glm::vec3 pos = node.Position();
    if (pos.x < area.origin.x || pos.x > area.origin.x + area.size.x ||
      pos.y < area.origin.y || pos.y > area.origin.y + area.size.y) {
      static_cast<CollidableSprite&>(node).Die();
    }
  }

private:
  MainGame *scene;
  Rect area;
};

class AimingShot : public TweenAnimation::Tween
{
public:
  AimingShot(MainGame* scene, const Node* target) : Tween(0, TweenAnimation::EasingType::Linear), scene(scene), target(target) {}
  virtual void Update(Node& node, glm::f32 elapsed) override
  {
    scene->EnemyShot(static_cast<Sprite&>(node), 400, 1);
  }

private:
  MainGame* scene;
  const Node* target = nullptr;
};

class Wait : public TweenAnimation::Tween
{
public:
  Wait(glm::f32 d) : Tween(d, TweenAnimation::EasingType::Linear) {}
  virtual void Update(Node&, glm::f32) override {}
};

class RemoveFromParent : public TweenAnimation::Tween
{
public:
  virtual void Update(Node& node, glm::f32 elapsed) override
  {
    if (node.Parent()) {
      node.Parent()->RemoveChild(&node);
    }
  }
};


std::shared_ptr<CollidableSprite> CollidableSprite::create(const TexturePtr& tex, const glm::vec3& pos, const CollisionRect& body, int hp)
{
  struct Impl : public CollidableSprite {
    Impl(const TexturePtr& tex, const glm::vec3& pos, const CollisionRect& body, int hp = 1) : CollidableSprite(tex, pos, body, hp) {}
  };
  return std::make_unique<Impl>(tex, pos, body, hp);
}

void MainGame::PlayerShot(glm::f32 rot, glm::f32 vel, int atk)
{
  auto shot = CollidableSprite::create(tex, sprite->Position(), { {-32, 8 },{ 32, -8 }  }, 1);
  const auto m = glm::rotate(glm::mat4(), glm::radians(rot), glm::vec3(0, 0, 1));
  glm::vec3 velocity = glm::vec4(vel, 0, 0, 1) * m;
  auto tween = std::make_shared<TweenAnimation::Parallelize>();
  tween->Add(std::make_shared<TweenAnimation::MoveBy>(0.5f, glm::vec3(800, 0, 0)));
  tween->Add(std::make_shared<RemoveIfOutOfAraa>(this, Rect{ glm::vec2(-400, -300), glm::vec2(800, 600) }));
  shot->Tweener(std::make_shared<TweenAnimation::Animate>(tween));
  shot->Rectangle({ {64, 0},{64, 16} });
  shot->Name("shot(p)");
  playerShotList.push_back(shot);
  AddChild(shot.get());
}

void MainGame::EnemyShot(const Sprite& enemy, glm::f32 vel, int atk)
{
  const glm::vec3 pos = enemy.WorldPosition();
  auto shot = CollidableSprite::create(tex, pos, { {-4, 4 },{ 4, -4 }  }, atk);
  const glm::vec3 direction = glm::normalize(sprite->WorldPosition() - pos);
  const glm::f32 rot = glm::acos(glm::dot(glm::vec3(-1, 0, 0), direction));
  auto tween = std::make_shared<TweenAnimation::Parallelize>();
  tween->Add(std::make_shared<TweenAnimation::MoveBy>(2000.0f / vel, direction * 2000.0f));
  tween->Add(std::make_shared<RemoveIfOutOfAraa>(this, Rect{ glm::vec2(-400, -300), glm::vec2(800, 600) }));
  shot->Tweener(std::make_shared<TweenAnimation::Animate>(tween));
  shot->Rectangle({ {512 - 32 - 16, 0},{16, 16} });
  shot->Rotation(rot);
  shot->Name("shot(e)");
  enemyShotList.push_back(shot);
  AddChild(shot.get());
}

void FreeDeadSprite(std::vector<CollidableSpritePtr>& targetList)
{
  targetList.erase(
    std::remove_if(targetList.begin(), targetList.end(), [](const CollidableSpritePtr& p) { return p->IsDead(); }), targetList.end()
  );
}
/**
*
*/
void MainGame::FreeAllDeadSprite()
{
  nodeList.erase(std::remove_if(nodeList.begin(), nodeList.end(), [](const NodePtr& p) {return !p->Parent(); }), nodeList.end());
  FreeDeadSprite(playerShotList);
  FreeDeadSprite(enemyShotList);
  FreeDeadSprite(enemyList);
}

/**
* �Փˏ�Ԃ𒲂ׂ�.
*
* @param lhs ���ӂ̏Փ˔���Ώ�.
* @param rhs �E�ӂ̏Փ˔���Ώ�.
*
* @retval true �Փ˂��Ă���.
* @retval false �Փ˂��Ă��Ȃ�.
*/
bool IsCollision(const CollidableSpritePtr& lhs, const CollidableSpritePtr& rhs)
{
  const auto bodyL = lhs->Body();
  const auto bodyR = rhs->Body();
  if (bodyL.lt.x < bodyR.rb.x && bodyL.rb.x > bodyR.lt.x) {
    if (bodyL.lt.y > bodyR.rb.y && bodyL.rb.y < bodyR.lt.y) {
      return true;
    }
  }
  return false;
}

/**
* �Փ˔���.
*
* @param b0 ���ӂ̏Փ˔���Ώ۔͈͂̐擪.
* @param e0 ���ӂ̏Փ˔���Ώ۔͈͂̏I�[.
* @param b1 �E�ӂ̏Փ˔���Ώ۔͈͂̐擪.
* @param e1 �E�ӂ̏Փ˔���Ώ۔͈͂̏I�[.
* @param solver �Փ˂���������֐��I�u�W�F�N�g.
*/
template<typename Itr0, typename Itr1, typename Func>
void DetectCollision(Itr0 b0, Itr0 e0, Itr1 b1, Itr1 e1, Func solver)
{
  for (; b0 != e0; ++b0) {
    if ((*b0)->IsDead()) {
      continue;
    }
    for (; b1 != e1; ++b1) {
      if ((*b1)->IsDead()) {
        continue;
      }
      if (IsCollision(*b0, *b1)) {
        solver(*b0, *b1);
        if ((*b0)->IsDead()) {
          break;
        }
      }
    }
  }
}

/**
* �A�j���[�V�����f�[�^�̏�����.
*/
TimelineList InitAnimationData()
{
  static const FrameAnimation::KeyFrame escortFrames[] = {
    { 0.000f, glm::vec2(480, 0), glm::vec2(32, 32) },
  { 0.125f, glm::vec2(480, 96), glm::vec2(32, 32) },
  { 0.250f, glm::vec2(480, 64), glm::vec2(32, 32) },
  { 0.375f, glm::vec2(480, 32), glm::vec2(32, 32) },
  { 0.500f, glm::vec2(480, 0), glm::vec2(32, 32) },
  };
  TimelineList timelineList;
  FrameAnimation::TimelinePtr timeline = std::make_shared<FrameAnimation::Timeline>();
  timeline->data.assign(std::begin(escortFrames), std::end(escortFrames));
  timelineList.push_back(timeline);

  return timelineList;
}

/**
* ���C���Q�[����ʂ̏�����.
*
* @param manager �V�[���Ǘ��N���X.
*
* @retval true  ����.
* @retval false ���s.
*/
bool MainGame::Initialize(Manager& manager)
{
  if (!Font::Init()) {
    return false;
  }

  tex = Texture::LoadFromFile("Res/Objects.dds");
  texBg = Texture::LoadFromFile("Res/UnknownPlanet.dds");
  if (!tex || !texBg) {
    return false;
  }

  timelineList = InitAnimationData();

  sprite = CollidableSprite::create(tex, glm::vec3(), { {-16, 8},{ 16, -8} });
  sprite->Texture(tex);
  sprite->Rectangle({ glm::vec2(0 ,0), glm::vec2(64, 32) });
  sprite->Name("player");

  boss.Texture(tex);
  boss.Rectangle({ glm::vec2(320 ,128), glm::vec2(128, 256) });
  boss.Position(glm::vec3(256, 0, 0));
  boss.Name("boss");

  background.Texture(texBg);
  background.Name("bg");

  AddChild(&background);
  AddChild(&boss);
  AddChild(sprite.get());
  Font::SetTextSprite(*RootNode(), scoreList, glm::vec3(-32 * 3, 300 - 32, 0), "000000", glm::vec4(1, 1, 1, 1));
  restList.resize(rest, Sprite(tex));
  for (size_t i = 0; i < restList.size(); ++i) {
    restList[i].Position(glm::vec3(-(400 - 16 - 16 * static_cast<int>(i)), 300 - 16, 0));
    restList[i].Rectangle({ {0, 0}, {64, 32} });
    restList[i].Scale({ 0.25f, 0.25f });
    AddChild(&restList[i]);
  }

  escortNode.Position(glm::vec3(-16, 0, 0));
  escortNode.Name("escortNode");
  boss.AddChild(&escortNode);

  for (size_t i = 0; i < 16; ++i) {
    const auto m = glm::rotate(glm::mat4(), glm::radians(static_cast<float>(i * 360) / 16.0f), glm::vec3(0, 0, 1));
    const glm::vec4 pos = m * glm::vec4(0, 144, 0, 1);
    CollidableSpritePtr escort = CollidableSprite::create(tex, pos, { {-12, 12}, {12, -12} }, 10);
    escort->Name("escort");
    escortNode.AddChild(escort.get());

    auto animator = std::make_shared<FrameAnimation::Animate>(timelineList[0]);
    escort->Animator(animator);
    auto tween = std::make_shared<TweenAnimation::Sequence>();
    tween->Add(std::make_shared<Wait>(1.0f));
    tween->Add(std::make_shared<AimingShot>(this, sprite.get()));
    escort->Tweener(std::make_shared<TweenAnimation::Animate>(tween));
    enemyList.push_back(escort);
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
    boss.Tweener(tweenBoss);
  }
  return true;
}

/**
* ���C���Q�[����ʂ̍X�V.
*
* @param manager �V�[���Ǘ��N���X.
* @param dt      �O��̍X�V����̌o�ߎ���.
*
* @retval true  ����.
* @retval false ���s.
*/
bool MainGame::Update(Manager& manager, float dt)
{
  GLFWEW::Window& window = GLFWEW::Window::Instance();
  const GamePad& gamepad = window.GetGamePad();
  if (gamepad.buttonDown & GamePad::A) {
    PlayerShot(0, 100, 1);
  }
  glm::vec3 vec;
  if (gamepad.buttons & GamePad::DPAD_LEFT) {
    vec.x = -1;
  } else if (gamepad.buttons & GamePad::DPAD_RIGHT) {
    vec.x = 1;
  }
  if (gamepad.buttons & GamePad::DPAD_UP) {
    vec.y = 1;
  } else if (gamepad.buttons & GamePad::DPAD_DOWN) {
    vec.y = -1;
  }
  if (vec.x || vec.y) {
    vec = glm::normalize(vec) * 400.0f * dt;
    sprite->Position(sprite->Position() + vec);
  }

  escortNode.Rotation(escortNode.Rotation() + glm::radians(25.0f * dt));
  for (auto e : escortNode.Children()) {
    e->Rotation(-escortNode.Rotation());
  }

  DetectCollision(
    playerShotList.begin(), playerShotList.end(),
    enemyList.begin(), enemyList.end(),
    [this](const CollidableSpritePtr& lhs, const CollidableSpritePtr& rhs) {
    lhs->CountervailingHealth(*rhs.get());
    if (rhs->IsDead()) {
      score += 500;
      auto p = std::make_shared<Sprite>(tex);
      p->Rectangle({ {512 - 96, 64 }, { 32, 32} });
      p->Position(rhs->WorldPosition());
      auto tween = std::make_shared<TweenAnimation::Sequence>();
      tween->Add(std::make_shared<Wait>(0.5f));
      tween->Add(std::make_shared<RemoveFromParent>());
      p->Tweener(std::make_shared<TweenAnimation::Animate>(tween));
      nodeList.push_back(p);
      AddChild(p.get());
    } else {
      score += 10;
    }
  }
  );
  DetectCollision(
    &sprite, &sprite + 1,
    enemyShotList.begin(), enemyShotList.end(),
    [this](const CollidableSpritePtr& lhs, const CollidableSpritePtr& rhs) {
    lhs->CountervailingHealth(*rhs.get());
    if (lhs->IsDead()) {
      if (restList.empty()) {
        gameover = true;
      } else {
        restList.pop_back();
        lhs->Position(glm::vec3(-300, 0, 0));
        lhs->Health(1);
      }
    }
  }
  );

  int scoreTmp = score;
  for (size_t i = scoreList.size(); i > 0;) {
    --i;
    Font::SetChar(scoreList[i], (scoreTmp % 10) + '0');
    scoreTmp /= 10;
  }

  FreeAllDeadSprite();

  return true;
}

/**
* ���C���Q�[����ʂ̌�n��.
*
* @param manager �V�[���Ǘ��N���X.
*/
void MainGame::Finalize(Manager& manager)
{
}

} // namespace Scene
