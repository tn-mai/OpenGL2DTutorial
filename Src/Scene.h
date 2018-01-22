/**
* @file Scene.h
*
*/
#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED
#include <memory>
#include "Sprite.h"
#include "Texture.h"

namespace Scene {

// ��s�錾.
class Manager;

/**
* �V�[���̊��N���X.
*/
class Scene
{
public:
  Scene() = default;
  virtual ~Scene() = default;
  Scene(const Scene&) = delete;
  Scene& operator=(const Scene&) = delete;

  virtual bool Initialize(Manager& manager) = 0;
  virtual bool Update(Manager& manager, float dt) = 0;
  virtual void Finalize(Manager& manager) = 0;
};

/// �V�[���p�|�C���^�^.
using ScenePtr = std::shared_ptr<Scene>;

/**
* �V�[���Ǘ��N���X.
*/
class Manager
{
public:
  Manager(const glm::vec2& screenSize);
  bool Start(const ScenePtr& start);
  bool Update(float dt);
  void NextScene(const ScenePtr& next);
  void End();

  void AddChild(Node* child) { sceneRootNode.AddChild(child); }
  void RemoveChild(Node* child) { sceneRootNode.RemoveChild(child); }
  const Node& OriginNode() const { return originNode; }
  Node& RootNode() { return sceneRootNode; }
  const Node& RootNode() const { return sceneRootNode; }

private:
  enum class FadeMode {
    None,
    Out,
    In,
  };
  TexturePtr texColorFilter;
  Sprite sprColorFilter;
  FadeMode fadeMode = FadeMode::In;
  float fadeTimer = 0.5f;
  ScenePtr currentScene;
  ScenePtr nextScene;
  Node originNode;
  Node sceneRootNode;
  bool isEnd = false;
};

} // namespace Scene

#endif // SCENE_H_INCLUDED