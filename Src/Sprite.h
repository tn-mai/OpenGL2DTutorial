/**
* @file Sprite.h
*/
#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED
#include "Texture.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class SpriteRenderer;

/**
* 矩形構造体.
*/
struct Rect
{
  glm::vec2 origin; ///< 左下原点.
  glm::vec2 size; ///< 縦横の幅.
};

/**
*
*/
class Node
{
public:
  Node() = default;
  ~Node() = default;
  Node(const Node&) = default;
  Node& operator=(const Node&) = default;

  void Position(const glm::vec3& p) { position = p; }
  const glm::vec3& Position() const { return position; }
  void Scale(const glm::vec2& s) { scale = s; }
  const glm::vec2& Scale() const { return scale; }
  const glm::mat4x4& Transform() const { return transform; }

  void AddChild(Node*);
  void RemoveChild(Node*);
  void RemoveFromParent();
  const std::vector<Node*>& Children() const { return children; }
  void Update(const glm::mat4x4&);

  virtual void Draw(SpriteRenderer&) const;

  std::string name;
private:
  glm::vec3 position;
  glm::vec2 scale = glm::vec2(1, 1);
  glm::mat4x4 transform;

  Node* parent = nullptr;
  std::vector<Node*> children;
};

/**
* スプライトクラス.
*/
class Sprite : public Node
{
public:
  Sprite() = default;
  explicit Sprite(const TexturePtr&);
  ~Sprite() = default;
  Sprite(const Sprite&) = default;
  Sprite& operator=(const Sprite&) = default;

  const TexturePtr& Texture() const { return texture; }
  void Rectangle(const Rect& r) { rect = r; }
  const Rect& Rectangle() const { return rect; }
  void Color(const glm::vec4& c) { color = c; }
  const glm::vec4& Color() const { return color; }

  virtual void Draw(SpriteRenderer&) const;

private:
  TexturePtr texture;
  Rect rect = { glm::vec2(0, 0), glm::vec2(1, 1) };
  glm::vec4 color = glm::vec4(1);
};

/**
* スプライト描画クラス.
*/
class SpriteRenderer
{
public:
  SpriteRenderer() = default;
  ~SpriteRenderer();
  SpriteRenderer(const SpriteRenderer&) = delete;
  SpriteRenderer& operator=(const SpriteRenderer&) = delete;

  bool Init(size_t maxSpriteCount);
  void BeginUpdate();
  bool AddVertices(const Sprite&);
  void EndUpdate();
  void Update(const Node*);
  void Draw(const glm::vec2&) const;
  void ClearDrawData();

private:
  void Update(const Node*, std::vector<const Node*>&);

  GLuint vbo = 0;
  GLuint ibo = 0;
  GLuint vao = 0;
  GLuint shaderProgram = 0;
  GLsizei vboCapacity = 0;        ///< VBOに格納可能な最大頂点数.
  GLsizei vboSize = 0;            ///< VBOに格納されている頂点数.
  struct Vertex* pVBO = nullptr;  ///< VBOへのポインタ.

  struct DrawData {
    size_t count;
    size_t offset;
    TexturePtr texture;
  };
  std::vector<DrawData> drawDataList;
};

#endif // SPRITE_H_INCLUDED