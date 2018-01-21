/**
* @file Scene.h
*
*/
#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED
#include <memory>
#include "Node.h"

namespace Scene {

class Manager;

/**
* シーンの基底クラス.
*/
class Scene
{
public:
  virtual bool Initialize(Manager& manager) = 0;
  virtual bool Update(Manager& manager, float dt) = 0;
  virtual void Finalize(Manager& manager) = 0;
};
using ScenePtr = std::shared_ptr<Scene>;

/**
* シーン管理クラス.
*/
class Manager
{
public:
  Manager();
  bool Start(const ScenePtr& start);
  bool Update(float dt);
  void NextScene(const ScenePtr& next);
  void End();
  void AddChild(Node* child) { rootNode.AddChild(child); }
  void RemoveChild(Node* child) { rootNode.RemoveChild(child); }
  Node& RootNode() { return rootNode; }
  const Node& RootNode() const { return rootNode; }

private:
  ScenePtr currentScene;
  ScenePtr nextScene;
  Node rootNode;
  bool isEnd = false;
};

} // namespace Scene

#endif // SCENE_H_INCLUDED