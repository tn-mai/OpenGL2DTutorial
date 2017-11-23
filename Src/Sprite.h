/**
* @file Sprite.h
*/
#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED
#include "Node.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <vector>

/**
* ��`�\����.
*/
struct Rect
{
  glm::vec2 origin; ///< �������_.
  glm::vec2 size; ///< �c���̕�.
};

/**
* �X�v���C�g�N���X.
*/
class Sprite : public Node
{
public:
  Sprite() = default;
  explicit Sprite(const TexturePtr&);
  virtual ~Sprite() = default;
  Sprite(const Sprite&) = default;
  Sprite& operator=(const Sprite&) = default;

  const TexturePtr& Texture() const { return texture; }
  void Rectangle(const Rect& r) { rect = r; }
  const Rect& Rectangle() const { return rect; }
  void Color(const glm::vec4& c) { color = c; }
  const glm::vec4& Color() const { return color; }

private:
  virtual void Draw(SpriteRenderer&) const override;

  TexturePtr texture;
  Rect rect = { glm::vec2(0, 0), glm::vec2(1, 1) };
  glm::vec4 color = glm::vec4(1);
};

/**
* �X�v���C�g�`��N���X.
*/
class SpriteRenderer
{
public:
  SpriteRenderer() = default;
  ~SpriteRenderer();
  SpriteRenderer(const SpriteRenderer&) = delete;
  SpriteRenderer& operator=(const SpriteRenderer&) = delete;

  bool Init(size_t maxSpriteCount);
  void Update(const Node&);
  void Draw(const glm::vec2&) const;
  void ClearDrawData();

  bool AddVertices(const Sprite&);

private:
  void BeginUpdate();
  void EndUpdate();
  void MakeNodeList(const Node&, std::vector<const Node*>&);

  GLuint vbo = 0;
  GLuint ibo = 0;
  GLuint vao = 0;
  GLuint shaderProgram = 0;
  GLsizei vboCapacity = 0;        ///< VBO�Ɋi�[�\�ȍő咸�_��.
  GLsizei vboSize = 0;            ///< VBO�Ɋi�[����Ă��钸�_��.
  struct Vertex* pVBO = nullptr;  ///< VBO�ւ̃|�C���^.

  struct DrawData {
    size_t count;
    size_t offset;
    TexturePtr texture;
  };
  std::vector<DrawData> drawDataList;
};

#endif // SPRITE_H_INCLUDED