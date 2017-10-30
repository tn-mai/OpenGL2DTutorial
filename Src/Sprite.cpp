/**
* @file Sprite.cpp
*/
#include "Sprite.h"
#include "Shader.h"
#include <vector>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

/**
* ���_�f�[�^�^.
*/
struct Vertex
{
  glm::vec3 position; ///< ���W
  glm::vec4 color; ///< �F
  glm::vec2 texCoord; ///< �e�N�X�`�����W
};

/**
* Vertex Buffer Object���쐬����.
*
* @param size ���_�f�[�^�̃T�C�Y.
* @param data ���_�f�[�^�ւ̃|�C���^.
*
* @return �쐬����VBO��ID.
*/
GLuint CreateVBO(GLsizeiptr size, const GLvoid* data)
{
  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return vbo;
}

/**
* Index Buffer Object���쐬����.
*
* @param size �C���f�b�N�X�f�[�^�̃T�C�Y.
* @param data �C���f�b�N�X�f�[�^�ւ̃|�C���^.
*
* @return �쐬����IBO��ID.
*/
GLuint CreateIBO(GLsizeiptr size, const GLvoid* data)
{
  GLuint ibo = 0;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  return ibo;
}

/**
* ���_�A�g���r���[�g��ݒ肷��.
*
* @param index ���_�A�g���r���[�g�̃C���f�b�N�X.
* @param cls   ���_�f�[�^�^��.
* @param mbr   ���_�A�g���r���[�g�ɐݒ肷��cls�̃����o�ϐ���.
*/
#define SetVertexAttribPointer(index, cls, mbr) \
  SetVertexAttribPointerI(index, sizeof(cls::mbr) / sizeof(float), sizeof(cls), reinterpret_cast<GLvoid*>(offsetof(cls, mbr)))
void SetVertexAttribPointerI(GLuint index, GLint size, GLsizei stride, const GLvoid* pointer)
{
  glEnableVertexAttribArray(index);
  glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, pointer);
}

/**
* Vertex Array Object���쐬����.
*
* @param vbo VAO�Ɋ֘A�t������VBO��ID.
* @param ibo VAO�Ɋ֘A�t������IBO��ID.
*
* @return �쐬����VAO��ID.
*/
GLuint CreateVAO(GLuint vbo, GLuint ibo)
{
  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  SetVertexAttribPointer(0, Vertex, position);
  SetVertexAttribPointer(1, Vertex, color);
  SetVertexAttribPointer(2, Vertex, texCoord);
  glBindVertexArray(0);
  return vao;
}

/**
* Sprite�R���X�g���N�^.
*/
Sprite::Sprite(const TexturePtr& tex) :
  texture(tex),
  rect({ glm::vec2(), glm::vec2(tex->Width(), tex->Height()) })
{
}

/**
* SpriteRenderer�f�X�g���N�^.
*/
SpriteRenderer::~SpriteRenderer()
{
  if (shaderProgram) {
    glDeleteProgram(shaderProgram);
  }
  if (vao) {
    glDeleteVertexArrays(1, &vao);
  }
  if (ibo) {
    glDeleteBuffers(1, &ibo);
  }
  if (vbo) {
    glDeleteBuffers(1, &vbo);
  }
}

/**
* �X�v���C�g�Ǘ��N���X������������.
*
* @retval true  ����������.
* @retval false ���������s.
*/
bool SpriteRenderer::Init(size_t maxSpriteCount)
{
  vbo = CreateVBO(sizeof(Vertex) * maxSpriteCount * 4, nullptr);
  std::vector<GLushort> indices;
  indices.resize(maxSpriteCount * 6);
  for (GLushort i = 0; i < maxSpriteCount; ++i) {
    indices[i * 6 + 0] = (i * 4) + 0;
    indices[i * 6 + 1] = (i * 4) + 1;
    indices[i * 6 + 2] = (i * 4) + 2;
    indices[i * 6 + 3] = (i * 4) + 2;
    indices[i * 6 + 4] = (i * 4) + 3;
    indices[i * 6 + 5] = (i * 4) + 0;
  }
  ibo = CreateIBO(indices.size() * sizeof(GLushort), indices.data());
  vao = CreateVAO(vbo, ibo);
  shaderProgram = Shader::CreateProgramFromFile("Res/Tutorial.vert", "Res/Tutorial.frag");
  if (!vbo || !ibo || !vao || !shaderProgram) {
    return false;
  }

  vboCapacity = static_cast<GLsizei>(4 * maxSpriteCount);
  return true;
}

/**
*
*
*/
void SpriteRenderer::BeginUpdate()
{
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  pVBO = static_cast<Vertex*>(glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vboCapacity, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
  vboSize = 0;
}

/**
*
*
*/
void SpriteRenderer::AddVertices(const Sprite& sprite)
{
  const TexturePtr& texture = sprite.Texture();
  const glm::vec2 textureSize(texture->Width(), texture->Height());
  const glm::vec2 reciprocalSize(glm::vec2(1) / textureSize);

  Rect rect = sprite.Rectangle();
  rect.origin *= reciprocalSize;
  rect.size *= reciprocalSize;
  const glm::vec3 lb = sprite.Position();
  const glm::vec3 rt = lb + glm::vec3(sprite.Rectangle().size * sprite.Scale(), 0);

  pVBO[0].position = lb;
  pVBO[0].color = sprite.Color();
  pVBO[0].texCoord = rect.origin;

  pVBO[1].position = glm::vec3(lb.x, rt.y, lb.z);
  pVBO[1].color = sprite.Color();
  pVBO[1].texCoord = glm::vec2(rect.origin.x, rect.origin.y + rect.size.y);

  pVBO[2].position = glm::vec3(rt.x, rt.y, lb.z);
  pVBO[2].color = sprite.Color();
  pVBO[2].texCoord = rect.origin + rect.size;

  pVBO[3].position = glm::vec3(rt.x, lb.y, lb.z);
  pVBO[3].color = sprite.Color();
  pVBO[3].texCoord = glm::vec2(rect.origin.x + rect.size.x, rect.origin.y);

  pVBO += 4;
  vboSize += 4;
}

/**
*
*
*/
void SpriteRenderer::EndUpdate()
{
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  pVBO = nullptr;
}

/**
*
*
*/
void SpriteRenderer::Draw(const TexturePtr& texture, const glm::vec2& screenSize) const
{
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glUseProgram(shaderProgram);

  const GLint matMVPLoc = glGetUniformLocation(shaderProgram, "matMVP");
  if (matMVPLoc >= 0) {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    const GLfloat w = static_cast<GLfloat>(viewport[2]);
    const GLfloat h = static_cast<GLfloat>(viewport[3]);
    const glm::mat4x4 matProj = glm::perspective(glm::radians(45.0f), screenSize.x / screenSize.y, 200.0f, 1200.0f);
    const glm::mat4x4 matView = glm::lookAt(glm::vec3(0, 0, glm::tan(glm::radians(90.0f - 22.5f)) * screenSize.y * 0.5f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    const glm::mat4x4 matMVP = matProj * matView;
    glUniformMatrix4fv(matMVPLoc, 1, GL_FALSE, &matMVP[0][0]);
  }

  const GLint colorSamplerLoc = glGetUniformLocation(shaderProgram, "colorSampler");
  if (colorSamplerLoc >= 0) {
    glUniform1i(colorSamplerLoc, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->Id());
  }

  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, (vboSize / 4) * 6, GL_UNSIGNED_SHORT, 0);
  glBindVertexArray(0);
}
