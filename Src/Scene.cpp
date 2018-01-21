/**
* @file Scene.cpp
*/
#include "Scene.h"

namespace Scene {

/**
* コンストラクタ.
*/
Manager::Manager()
{
  rootNode.Name("rootNode");
}

/**
* ゲームを開始する.
*
* @param start 開始するシーン.
*/
bool Manager::Start(const ScenePtr& start)
{
  currentScene = start;
  nextScene = nullptr;
  if (!currentScene->Initialize(*this)) {
    currentScene = nullptr;
    return false;
  }
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
    return false;
  }
  currentScene->Update(*this, dt);
  rootNode.UpdateRecursive(dt);
  if (!nextScene) {
    return true;
  }
  currentScene->Finalize(*this);
  currentScene = nullptr;
  if (!nextScene->Initialize(*this)) {
    nextScene = nullptr;
    return false;
  }
  currentScene = nextScene;
  nextScene = nullptr;
  return true;
}

/**
* 次に実行するシーンを指定する.
*
* @param next 次に実行するシーン.
*/
void Manager::NextScene(const ScenePtr& next)
{
  nextScene = next;
}

/**
* ゲームを終了する.
*
* 次回のUpdate()でシーンのFinalizeが呼ばれ、Update()がfalseを返すことでゲームを終了させる.
*/
void Manager::End()
{
  nextScene = nullptr;
  isEnd = true;
}

} // namespace Scene