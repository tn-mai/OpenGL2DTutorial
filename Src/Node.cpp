/**
* @file Node.cpp
*/
#include "Node.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

/**
* �q�m�[�h��ǉ�����.
*
* @param node �ǉ�����q�m�[�h�ւ̃|�C���^.
*/
void Node::AddChild(Node* node)
{
  node->parent = this;
  children.push_back(node);
}

/**
* �q�m�[�h���O��.
*
* @param node �O���q�m�[�h�ւ̃|�C���^.
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
* �m�[�h�̏�Ԃ��ċA�I�ɍX�V����.
*
* @param dt �O��̍X�V����̌o�ߎ���.
*/
void Node::UpdateRecursive(float dt)
{
  glm::mat4x4 parentTransform;
  if (parent) {
    parentTransform = parent->Transform();
  }
  transform = glm::rotate(glm::scale(glm::translate(parentTransform, position), glm::vec3(scale, 1.0f)), rotation, glm::vec3(0, 0, 1));

  Update(dt);
  for (auto& e : children) {
    e->UpdateRecursive(dt);
  }
}

/**
* �m�[�h�̏�Ԃ��X�V����.
*
* @param dt �O��̍X�V����̌o�ߎ���.
*
* �m�[�h�̍X�V���@��ύX�������ꍇ�͂��̊֐����I�[�o�[���C�h���܂��B
*/
void Node::Update(float dt)
{
  // �������Ȃ�.
}

/**
* �m�[�h��`�悷��.
*
* @param �`����s�������_���[.
*
* �m�[�h�̕`����@��ύX�������ꍇ�͂��̊֐����I�[�o�[���C�h���܂��B
*/
void Node::Draw(SpriteRenderer& renderer) const
{
  // �������Ȃ�.
}
