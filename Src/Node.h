/**
* @file Node.h
*/
#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED
#include <glm/glm.hpp>
#include <vector>
#include <string>

/**
* ノードクラス.
*
* ノードは「シーンを構成する木構造(シーングラフ)」を構築するための部品です.
* シーングラフを構築するには、まず根本にあたる親ノード(ルートノード)を作成し、
* addChildによって子ノードを追加していきます.
* 
* ノードはシーングラフを構成するだけで、他には何もしません.
* ノードに特別な動きをさせたい場合はNodeクラスを派生させて、Update関数か
* Draw関数、あるいはその両方をオーバーライドしてください.
*
* 画面に何かを表示したい場合はNodeクラスではなくSpriteクラスを使用してください.
*
* @sa Sprite
*/
class Node
{
public:
  friend class SpriteRenderer;

  Node() = default;
  virtual ~Node() = default;
  Node(const Node&) = default;
  Node& operator=(const Node&) = default;

  void Name(const char* n) { name = n; } 
  const std::string& Name() const { return name; }
  void Position(const glm::vec3& p) { position = p; }
  const glm::vec3& Position() const { return position; }
  void Scale(const glm::vec2& s) { scale = s; }
  const glm::vec2& Scale() const { return scale; }
  void Rotation(float r) { rotation = r; }
  float Rotation() const { return rotation; }
  const glm::mat4x4& Transform() const { return transform; }

  void AddChild(Node*);
  void RemoveChild(Node*);
  Node* Parent() const { return parent; }
  const std::vector<Node*>& Children() const { return children; }

  void UpdateRecursive(float dt);

private:
  virtual void Update(float dt);
  virtual void Draw(SpriteRenderer&) const;

  std::string name; ///< ノード名.
  glm::vec3 position; ///< ノードの座標.
  glm::vec2 scale = glm::vec2(1, 1); ///< ノードの拡大縮小率.
  float rotation = 0;

  glm::mat4x4 transform; ///< ノードの座標変換行列.

  Node* parent = nullptr; ///< 親ノード.
  std::vector<Node*> children; ///< 子ノードのリスト.
};

#endif // NODE_H_INCLUDED