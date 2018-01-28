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
  Scene();
  virtual ~Scene() = default;
  Scene(const Scene&) = delete;
  Scene& operator=(const Scene&) = delete;

  virtual bool Initialize(Manager& manager) = 0;
  virtual bool Update(Manager& manager, float dt) = 0;
  virtual void Finalize(Manager& manager) = 0;

  Node* RootNode() { return &rootNode; }
  const Node* RootNode() const { return &rootNode; }
  void AddChild(Node* child) { rootNode.AddChild(child); }
  void RemoveChild(Node* child) { rootNode.RemoveChild(child);  }

private:
  Node rootNode;
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
  ~Manager() = default;
  Manager(const Manager&) = delete;
  Manager& operator=(const Manager&) = delete;

  bool Start(const ScenePtr& start);
  bool Update(float dt);
  void ReplaceScene(const ScenePtr& next);
  void End();

  const Node& OriginNode() const { return originNode; }

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