/**
* @file MainGame.cpp
*/
#include "MainGame.h"
#include "Title.h"
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


void MainGame::EnemyShot(const Sprite& enemy, glm::f32 vel, int atk)
{
  const glm::vec3 pos = enemy.WorldPosition();
  auto shot = Character::CollidableSprite::create(tex, pos, { {-4, 4 },{ 4, -4 }  }, atk);
  const glm::vec3 direction = glm::normalize(sprite->WorldPosition() - pos);
  const glm::f32 rot = glm::acos(glm::dot(glm::vec3(-1, 0, 0), direction));
  auto tween = std::make_shared<TweenAnimation::Parallelize>();
  tween->Add(std::make_shared<TweenAnimation::MoveBy>(2000.0f / vel, direction * 2000.0f));
  tween->Add(std::make_shared<Character::RemoveIfOutOfArea>(Rect{ glm::vec2(-400, -300), glm::vec2(800, 600) }));
  shot->Tweener(std::make_shared<TweenAnimation::Animate>(tween));
  shot->Rectangle({ {512 - 32 - 16, 0},{16, 16} });
  shot->Rotation(rot);
  shot->Name("shot(e)");
  enemyShotList.push_back(shot);
  AddChild(shot.get());
}

void FreeDeadSprite(std::vector<Character::CollidableSpritePtr>& targetList)
{
  targetList.erase(
    std::remove_if(targetList.begin(), targetList.end(), [](const Character::CollidableSpritePtr& p) { return p->IsDead(); }), targetList.end()
  );
}
/**
*
*/
void MainGame::FreeAllDeadSprite()
{
  nodeList.erase(std::remove_if(nodeList.begin(), nodeList.end(), [](const NodePtr& p) {return !p->Parent(); }), nodeList.end());
  FreeDeadSprite(enemyShotList);
  FreeDeadSprite(enemyList);
}

/**
* 衝突状態を調べる.
*
* @param lhs 左辺の衝突判定対象.
* @param rhs 右辺の衝突判定対象.
*
* @retval true 衝突している.
* @retval false 衝突していない.
*/
bool IsCollision(const Character::CollidableSpritePtr& lhs, const Character::CollidableSpritePtr& rhs)
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
* 衝突判定.
*
* @param b0 左辺の衝突判定対象範囲の先頭.
* @param e0 左辺の衝突判定対象範囲の終端.
* @param b1 右辺の衝突判定対象範囲の先頭.
* @param e1 右辺の衝突判定対象範囲の終端.
* @param solver 衝突を解決する関数オブジェクト.
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

enum AnimeId {
  AnimeId_Enemy_Escort,
  AnimeId_Blast,
};

/**
* アニメーションデータの初期化.
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
  static const FrameAnimation::KeyFrame blastFrames[] = {
    { 0.000f, glm::vec2(416,  0), glm::vec2(32, 32) },
    { 0.125f, glm::vec2(416, 32), glm::vec2(32, 32) },
    { 0.250f, glm::vec2(416, 64), glm::vec2(32, 32) },
    { 0.375f, glm::vec2(416, 96), glm::vec2(32, 32) },
    { 0.500f, glm::vec2(416, 96), glm::vec2(0, 0) },
  };
  static const struct {
    const FrameAnimation::KeyFrame* begin;
    const FrameAnimation::KeyFrame* end;
  } timelines[] = {
    { std::begin(escortFrames), std::end(escortFrames) },
    { std::begin(blastFrames), std::end(blastFrames) },
  };
  TimelineList timelineList;
  for (auto& e : timelines) {
    FrameAnimation::TimelinePtr timeline = std::make_shared<FrameAnimation::Timeline>();
    timeline->data.assign(e.begin, e.end);
    timelineList.push_back(timeline);
  }

  return timelineList;
}

/**
* メインゲーム画面の初期化.
*
* @param manager シーン管理クラス.
*
* @retval true  成功.
* @retval false 失敗.
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

  sprite = Character::Player::Create(tex);

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
    CollidableSpritePtr escort = Character::CollidableSprite::create(tex, pos, { {-12, 12}, {12, -12} }, 10);
    escort->Name("escort");
    escortNode.AddChild(escort.get());

    auto animator = std::make_shared<FrameAnimation::Animate>(timelineList[AnimeId_Enemy_Escort]);
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
* メインゲーム画面の更新.
*
* @param manager シーン管理クラス.
* @param dt      前回の更新からの経過時間.
*
* @retval true  成功.
* @retval false 失敗.
*/
bool MainGame::Update(Manager& manager, float dt)
{
  GLFWEW::Window& window = GLFWEW::Window::Instance();
  const GamePad& gamepad = window.GetGamePad();
  if (sprite->GameOver()) {
    if (gameoverTimer > 0) {
      gameoverTimer -= dt;
      if (gameoverTimer <= 0) {
        gameoverTimer = 0;
        Font::SetTextSprite(*RootNode(), gameoverList, { -32 * 4.5f, 0, 0 }, "GAME OVER", glm::vec4(1, 0.25f, 0.125f, 1));
      }
    } else if (gamepad.buttonDown & GamePad::START) {
      manager.ReplaceScene(std::make_shared<Title>()); // タイトル画面へ.
    }
  }

  escortNode.Rotation(escortNode.Rotation() + glm::radians(25.0f * dt));
  for (auto e : escortNode.Children()) {
    e->Rotation(-escortNode.Rotation());
  }

  const auto AddBlastSprite = [this](const CollidableSpritePtr& target) {
    auto p = std::make_shared<Sprite>(tex);
    p->Position(target->WorldPosition());
    auto tween = std::make_shared<TweenAnimation::Sequence>();
    tween->Add(std::make_shared<Wait>(0.5f));
    tween->Add(std::make_shared<RemoveFromParent>());
    p->Tweener(std::make_shared<TweenAnimation::Animate>(tween));
    p->Animator(std::make_shared<FrameAnimation::Animate>(timelineList[AnimeId_Blast]));
    nodeList.push_back(p);
    AddChild(p.get());
  };

  DetectCollision(
    sprite->ShotList().begin(), sprite->ShotList().end(),
    enemyList.begin(), enemyList.end(),
    [&](const CollidableSpritePtr& lhs, const CollidableSpritePtr& rhs) {
    lhs->CountervailingHealth(*rhs.get());
    if (rhs->IsDead()) {
      score += 500;
      AddBlastSprite(rhs);
    } else {
      score += 10;
    }
  }
  );

  if (!sprite->Invinsible()) {
    const auto DestroyPlayer = [&](const CollidableSpritePtr& lhs, const CollidableSpritePtr& rhs) {
      lhs->CountervailingHealth(*rhs.get());
      if (lhs->IsDead()) {
        AddBlastSprite(lhs);
        if (restList.empty()) {
          sprite->GameOver(true);
          gameoverTimer = 2;
        } else {
          restList.pop_back();
        }
      }
    };

    DetectCollision(&sprite, &sprite + 1, enemyShotList.begin(), enemyShotList.end(), DestroyPlayer);
    DetectCollision(&sprite, &sprite + 1, enemyList.begin(), enemyList.end(), DestroyPlayer);
  }

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
* メインゲーム画面の後始末.
*
* @param manager シーン管理クラス.
*/
void MainGame::Finalize(Manager& manager)
{
}

} // namespace Scene
