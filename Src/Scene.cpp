/**
* @file Scene.cpp
*/
#include "Scene.h"

namespace Scene {

/**
* コンストラクタ.
*/
Scene::Scene()
{
  rootNode.Name("rootNode");
}

/**
* コンストラクタ.
*/
Manager::Manager(const glm::vec2& screenSize)
{
  originNode.Name("originNode");
  sprColorFilter.Scale(screenSize * (1.0f / 4.0f));
  sprColorFilter.Position(glm::vec3(0, 0, 500));
}

/**
* ゲームを開始する.
*
* @param start 開始するシーン.
*/
bool Manager::Start(const ScenePtr& start)
{
  if (!texColorFilter) {
    texColorFilter = Texture::LoadFromFile("Res/ColorFilter.dds");
    if (!texColorFilter) {
      return false;
    }
    sprColorFilter.Texture(texColorFilter);
  }
  currentScene = start;
  originNode.AddChild(currentScene->RootNode());
  nextScene = nullptr;
  if (!currentScene->Initialize(*this)) {
    currentScene = nullptr;
    return false;
  }
  originNode.AddChild(&sprColorFilter);
  return true;
}

/**
* シーンを更新する.
*
* @param dt 前回の更新からの経過時間.
*/
bool Manager::Update(float dt)
{
  if (!currentScene) {
    return false;
  }
  if (isEnd) {
    currentScene->Finalize(*this);
    currentScene = nullptr;
    nextScene = nullptr;
    return false;
  }
  currentScene->Update(*this, dt);

  switch (fadeMode) {
  case FadeMode::None:
    break;
  case FadeMode::Out:
    fadeTimer -= dt;
    if (fadeTimer <= 0) {
      fadeMode = FadeMode::None;
      fadeTimer = 0;
    }
    sprColorFilter.Color(glm::vec4(0, 0, 0, 1.0f - fadeTimer / 0.5f));
    break;
  case FadeMode::In:
    fadeTimer -= dt;
    if (fadeTimer <= 0) {
      fadeMode = FadeMode::None;
      fadeTimer = 0;
      originNode.RemoveChild(&sprColorFilter);
    }
    sprColorFilter.Color(glm::vec4(0, 0, 0, fadeTimer / 0.5f));
    break;
  }

  if (fadeMode == FadeMode::None && nextScene) {
    originNode.RemoveChild(&sprColorFilter);
    currentScene->Finalize(*this);
    currentScene = nullptr;
    originNode.AddChild(nextScene->RootNode());
    if (!nextScene->Initialize(*this)) {
      nextScene = nullptr;
      return false;
    }
    currentScene = nextScene;
    nextScene = nullptr;
    fadeMode = FadeMode::In;
    fadeTimer = 0.5f;
    originNode.AddChild(&sprColorFilter);
    dt = 0.0f;
  }
  originNode.UpdateTransform();
  originNode.UpdateRecursive(dt);
  return true;
}

/**
* 次に実行するシーンを指定する.
*
* @param next 次に実行するシーン.
*/
void Manager::ReplaceScene(const ScenePtr& next)
{
  nextScene = next;
  fadeMode = FadeMode::Out;
  fadeTimer = 0.5f;
  originNode.AddChild(&sprColorFilter);
}

/**
* ゲームを終了する.
*
* 次回のUpdate()でシーンのFinalizeが呼ばれ、Update()がfalseを返すことでゲームを終了させる.
*/
void Manager::End()
{
  isEnd = true;
}

} // namespace Scene