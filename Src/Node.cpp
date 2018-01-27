/**
* @file Node.cpp
*/
#include "Node.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

/**
* デストラクタ.
*/
Node::~Node()
{
  if (parent) {
    parent->RemoveChild(this);
  }
  for (auto& e : children) {
    e->parent = nullptr;
  }
}

/**
* 子ノードを追加する.
*
* @param node 追加する子ノードへのポインタ.
*/
void Node::AddChild(Node* node)
{
  node->parent = this;
  children.push_back(node);
}

/**
* 子ノードを外す.
*
* @param node 外す子ノードへのポインタ.
*/
void Node::RemoveChild(Node* node)
{
  auto itr = std::find(children.begin(), children.end(), node);
  if (itr != children.end()) {
    (*itr)->parent = nullptr;
    children.erase(itr);
  }
}

/**
* ノードの状態を再帰的に更新する.
*
* @param dt 前回の更新からの経過時間.
*/
void Node::UpdateRecursive(float dt)
{
  if (tweener) {
    tweener->Update(*this, dt);
  }
  Update(dt);
  auto tmp = children;
  for (auto& e : tmp) {
    e->UpdateRecursive(dt);
  }
}

void Node::UpdateTransform()
{
  glm::mat4x4 parentTransform;
  if (parent) {
    parentTransform = parent->Transform();
  }
  transform = glm::rotate(glm::scale(glm::translate(parentTransform, position), glm::vec3(scale, 1.0f)), rotation, glm::vec3(0, 0, 1));
  worldPosition = transform * glm::vec4(0, 0, 0, 1);
  for (auto& e : children) {
    e->UpdateTransform();
  }
}

/**
* ノードの状態を更新する.
*
* @param dt 前回の更新からの経過時間.
*
* ノードの更新方法を変更したい場合はこの関数をオーバーライドします。
*/
void Node::Update(float dt)
{
  // 何もしない.
}

/**
* ノードを描画する.
*
* @param 描画を行うレンダラー.
*
* ノードの描画方法を変更したい場合はこの関数をオーバーライドします。
*/
void Node::Draw(SpriteRenderer& renderer) const
{
  // 何もしない.
}
