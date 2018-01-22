/**
* @file Scene.cpp
*/
#include "Scene.h"

namespace Scene {

/**
* �R���X�g���N�^.
*/
Manager::Manager(const glm::vec2& screenSize)
{
  originNode.Name("originNode");
  sceneRootNode.Name("sceneRootNode");
  originNode.AddChild(&sceneRootNode);
  sprColorFilter.Scale(screenSize * (1.0f / 4.0f));
  sprColorFilter.Position(glm::vec3(0, 0, 500));
}

/**
* �Q�[�����J�n����.
*
* @param start �J�n����V�[��.
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
  nextScene = nullptr;
  if (!currentScene->Initialize(*this)) {
    currentScene = nullptr;
    return false;
  }
  originNode.AddChild(&sprColorFilter);
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

  originNode.UpdateRecursive(dt);

  if(fadeMode != FadeMode::None || !nextScene) {
    return true;
  }
  originNode.RemoveChild(&sprColorFilter);
  currentScene->Finalize(*this);
  currentScene = nullptr;
  if (!nextScene->Initialize(*this)) {
    nextScene = nullptr;
    return false;
  }
  currentScene = nextScene;
  nextScene = nullptr;
  fadeMode = FadeMode::In;
  fadeTimer = 0.5f;
  originNode.AddChild(&sprColorFilter);
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
  fadeMode = FadeMode::Out;
  fadeTimer = 0.5f;
  originNode.AddChild(&sprColorFilter);
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