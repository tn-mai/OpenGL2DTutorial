/**
* @file Sprite.cpp
*/
#include "Sprite.h"
#include "Shader.h"
#include <vector>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

/**
* 頂点データ型.
*/
struct Vertex
{
  glm::vec3 position; ///< 座標
  glm::vec4 color; ///< 色
  glm::vec2 texCoord; ///< テクスチャ座標
};

/**
* Vertex Buffer Objectを作成する.
*
* @param size 頂点データのサイズ.
* @param data 頂点データへのポインタ.
*
* @return 作成したVBOのID.
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
* Index Buffer Objectを作成する.
*
* @param size インデックスデータのサイズ.
* @param data インデックスデータへのポインタ.
*
* @return 作成したIBOのID.
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
* 頂点アトリビュートを設定する.
*
* @param index 頂点アトリビュートのインデックス.
* @param cls   頂点データ型名.
* @param mbr   頂点アトリビュートに設定するclsのメンバ変数名.
*/
#define SetVertexAttribPointer(index, cls, mbr) \
  SetVertexAttribPointerI(index, sizeof(cls::mbr) / sizeof(float), sizeof(cls), reinterpret_cast<GLvoid*>(offsetof(cls, mbr)))
void SetVertexAttribPointerI(GLuint index, GLint size, GLsizei stride, const GLvoid* pointer)
{
  glEnableVertexAttribArray(index);
  glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, pointer);
}

/**
* Vertex Array Objectを作成する.
*
* @param vbo VAOに関連付けられるVBOのID.
* @param ibo VAOに関連付けられるIBOのID.
*
* @return 作成したVAOのID.
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
* コンストラクタ.
*/
Sprite::Sprite(const TexturePtr& tex) :
  texture(tex),
  rect({ glm::vec2(), glm::vec2(tex->Width(), tex->Height()) })
{
}

/**
* スプライト管理クラスを初期化する.
*
* @retval true  初期化成功.
* @retval false 初期化失敗.
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
* デストラクタ.
*/
SpriteRenderer::~SpriteRenderer()
{
  glDeleteProgram(shaderProgram);
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
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
