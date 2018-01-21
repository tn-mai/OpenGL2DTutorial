/**
* @file Scene.cpp
*/
#include "Scene.h"

namespace Scene {

/**
* �R���X�g���N�^.
*/
Manager::Manager()
{
  rootNode.Name("rootNode");
}

/**
* �Q�[�����J�n����.
*
* @param start �J�n����V�[��.
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
* �V�[�����X�V����.
*
* @param dt �O��̍X�V����̌o�ߎ���.
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
* ���Ɏ��s����V�[�����w�肷��.
*
* @param next ���Ɏ��s����V�[��.
*/
void Manager::NextScene(const ScenePtr& next)
{
  nextScene = next;
}

/**
* �Q�[�����I������.
*
* �����Update()�ŃV�[����Finalize���Ă΂�AUpdate()��false��Ԃ����ƂŃQ�[�����I��������.
*/
void Manager::End()
{
  nextScene = nullptr;
  isEnd = true;
}

} // namespace Scene