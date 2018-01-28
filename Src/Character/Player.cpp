/**
* @file Player.cpp
*/
#include "Player.h"
#include "../GLFWEW.h"
#include "glm/gtc/matrix_transform.hpp"
#include <algorithm>

namespace Character {

/**
* 自機を作成する.
*/
PlayerPtr Player::Create(const TexturePtr& tex)
{
  return std::make_shared<Player>(tex);
}

/**
* コンストラクタ.
*/
Player::Player(const TexturePtr& tex) : CollidableSprite(tex, glm::vec3(-300, 0, 0), { {-16, 8},{ 16, -8} })
{
  Rectangle({ glm::vec2(0 ,0), glm::vec2(64, 32) });
  Name("player");
}

/**
* 弾を発射する.
*/
void Player::Shot(glm::f32 rot, glm::f32 vel, int atk)
{
  auto shot = Character::CollidableSprite::create(Texture(), Position(), { {-32, 8 },{ 32, -8 }  }, 1);
  const auto m = glm::rotate(glm::mat4(), glm::radians(rot), glm::vec3(0, 0, 1));
  glm::vec3 velocity = glm::vec4(vel, 0, 0, 1) * m;
  auto tween = std::make_shared<TweenAnimation::Parallelize>();
  tween->Add(std::make_shared<TweenAnimation::MoveBy>(0.5f, glm::vec3(800, 0, 0)));
  tween->Add(std::make_shared<RemoveIfOutOfArea>(Rect{ glm::vec2(-400, -300), glm::vec2(800, 600) }));
  shot->Tweener(std::make_shared<TweenAnimation::Animate>(tween));
  shot->Rectangle({ {64, 0},{64, 16} });
  shot->Name("shot(p)");
  shotList.push_back(shot);
  Parent()->AddChild(shot.get());
}

/**
* 死んだ弾を除去する.
*/
void Player::RemoveDeadShot()
{
  shotList.erase(
    std::remove_if(shotList.begin(), shotList.end(), [](const CollidableSpritePtr& p) { return p->IsDead(); }),
    shotList.end()
  );
}

/**
* 自機の更新.
*/
void Player::Update(glm::f32 dt)
{
  CollidableSprite::Update(dt);
  if (gameover || gameclear) {
    return;
  }

  if (invinsibleTimer > 2) {
    invinsibleTimer -= dt;
    if (invinsibleTimer <= 2) {
      Position(glm::vec3(-300, 0, 0));
      Color({ 1, 1, 1, 0.5f });
      Health(1);
      controllable = true;
    }
  } else if (invinsibleTimer > 0) {
    invinsibleTimer -= dt;
    if (invinsibleTimer <= 0) {
      invinsibleTimer = 0;
      Color({ 1, 1, 1, 1 });
      controllable = true;
    }
  }

  GLFWEW::Window& window = GLFWEW::Window::Instance();
  const GamePad& gamepad = window.GetGamePad();
  if (controllable) {
    if(gamepad.buttonDown & GamePad::A) {
      Shot(0, 100, 1);
      shotInterval = 1.0f / 8.0f;
    } else if (gamepad.buttons & GamePad::A) {
      shotInterval -= dt;
      if (shotInterval <= 0) {
        Shot(0, 100, 1);
        shotInterval += 1.0f / 8.0f;
      }
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
      glm::vec3 pos = Position() + vec;
      pos.x = glm::clamp(pos.x, -400.0f + 32.0f, 400.0f - 32.0f);
      pos.y = glm::clamp(pos.y, -300.0f + 16.0f, 300.0f - 16.0f);
      Position(pos);
    }
  }
  RemoveDeadShot();
}

/**
* 死亡処理.
*/
void Player::Die()
{
  CollidableSprite::Die();
  controllable = false;
  invinsibleTimer = 3;
  Color({ 1, 1, 1, 0 });
}


} // namespace Character