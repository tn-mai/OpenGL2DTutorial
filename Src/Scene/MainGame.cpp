/**
* @file MainGame.cpp
*/
#include "MainGame.h"
#include "Title.h"
#include "../Character/Boss.h"
#include "../Font.h"
#include "../GLFWEW.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <iostream>

namespace Scene {

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
  boss = std::make_shared<Character::Boss>(tex, sprite, enemyList, enemyShotList, timelineList);
  enemyList.push_back(boss);

  background.Texture(texBg);
  background.Name("bg");

  AddChild(&background);
  AddChild(boss.get());
  AddChild(sprite.get());
  Font::SetTextSprite(*RootNode(), scoreList, glm::vec3(-32 * 3, 300 - 32, 0), "000000", glm::vec4(1, 1, 1, 1));
  restList.resize(rest, Sprite(tex));
  for (size_t i = 0; i < restList.size(); ++i) {
    restList[i].Position(glm::vec3(-(400 - 16 - 16 * static_cast<int>(i)), 300 - 16, 0));
    restList[i].Rectangle({ {0, 0}, {64, 32} });
    restList[i].Scale({ 0.25f, 0.25f });
    restList[i].Name("rest");
    AddChild(&restList[i]);
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
  if (sprite->GameClear()) {
    if (eventTimer > 0) {
      eventTimer -= dt;
      if (eventTimer <= 0) {
        eventTimer = 0;
        static const char* const text[] = {
          "CONGRATURATION",
          "",
          "THANKS TO YOUR BRAVE",
          "FIGHTING. THE SOLAR",
          "SYSTEM RESTORED PEACE.",
          "YOUR NAME WILL REMAIN",
          "FOREVER..."
        };
        float y = 192;
        eventTextList.reserve(256);
        for (auto e : text) {
          const float halfLen = static_cast<float>(std::strlen(e)) * 0.5f;
          Font::SetTextSprite(*RootNode(), eventTextList, { -32 * halfLen + 3, y - 4, 0 }, e, glm::vec4(0, 0, 0, 0.75f));
          Font::SetTextSprite(*RootNode(), eventTextList, { -32 * halfLen, y, 0 }, e, glm::vec4(0.125f, 0.25f, 1, 1));
          y -= 64;
        }
      }
    } else if (gamepad.buttonDown & GamePad::START) {
      manager.ReplaceScene(std::make_shared<Title>()); // タイトル画面へ.
    }
  } else if (sprite->GameOver()) {
    if (eventTimer > 0) {
      eventTimer -= dt;
      if (eventTimer <= 0) {
        eventTimer = 0;
        Font::SetTextSprite(*RootNode(), eventTextList, { -32 * 4.5f, 0, 0 }, "GAME OVER", glm::vec4(1, 0.25f, 0.125f, 1));
      }
    } else if (gamepad.buttonDown & GamePad::START) {
      manager.ReplaceScene(std::make_shared<Title>()); // タイトル画面へ.
    }
  }
 
  const auto AddBlastSprite = [this](const glm::vec3& worldPosition, glm::f32 scale = 1) {
    auto p = std::make_shared<Sprite>(tex);
    p->Position(worldPosition);
    p->Scale(glm::vec2(scale));
    auto tween = std::make_shared<TweenAnimation::Sequence>();
    tween->Add(std::make_shared<TweenAnimation::Wait>(0.5f));
    tween->Add(std::make_shared<TweenAnimation::RemoveFromParent>());
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
      AddBlastSprite(rhs->WorldPosition());
      std::cout << "Destroy: " << rhs->Name() << std::endl;
    } else {
      score += 10;
    }
  }
  );

  if (!sprite->GameClear() && boss->IsDead()) {
    sprite->GameClear(true);
    for (auto e : boss->EscortList()) {
      if (e && !static_cast<Character::CollidableSprite*>(e)->IsDead()) {
        AddBlastSprite(e->WorldPosition());
      }
    }
    AddBlastSprite(boss->WorldPosition(), 5);
    boss.reset();
    eventTimer = 2;
  }

  if (!sprite->GameClear() && !sprite->Invinsible()) {
    const auto DestroyPlayer = [&](const CollidableSpritePtr& lhs, const CollidableSpritePtr& rhs) {
      lhs->CountervailingHealth(*rhs.get());
      if (lhs->IsDead()) {
        AddBlastSprite(lhs->WorldPosition(), 2);
        if (restList.empty()) {
          sprite->GameOver(true);
          eventTimer = 2;
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
