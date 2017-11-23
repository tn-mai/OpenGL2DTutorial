/**
* @file Node.h
*/
#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED
#include <glm/glm.hpp>
#include <vector>
#include <string>

/**
* �m�[�h�N���X.
*
* �m�[�h�́u�V�[�����\������؍\��(�V�[���O���t)�v���\�z���邽�߂̕��i�ł�.
* �V�[���O���t���\�z����ɂ́A�܂����{�ɂ�����e�m�[�h(���[�g�m�[�h)���쐬���A
* addChild�ɂ���Ďq�m�[�h��ǉ����Ă����܂�.
* 
* �m�[�h�̓V�[���O���t���\�����邾���ŁA���ɂ͉������܂���.
* �m�[�h�ɓ��ʂȓ��������������ꍇ��Node�N���X��h�������āAUpdate�֐���
* Draw�֐��A���邢�͂��̗������I�[�o�[���C�h���Ă�������.
*
* ��ʂɉ�����\���������ꍇ��Node�N���X�ł͂Ȃ�Sprite�N���X���g�p���Ă�������.
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

  std::string name; ///< �m�[�h��.
  glm::vec3 position; ///< �m�[�h�̍��W.
  glm::vec2 scale = glm::vec2(1, 1); ///< �m�[�h�̊g��k����.
  float rotation = 0;

  glm::mat4x4 transform; ///< �m�[�h�̍��W�ϊ��s��.

  Node* parent = nullptr; ///< �e�m�[�h.
  std::vector<Node*> children; ///< �q�m�[�h�̃��X�g.
};

#endif // NODE_H_INCLUDED