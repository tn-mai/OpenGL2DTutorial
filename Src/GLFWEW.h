/**
* @file GLFWEW.h
*/
#ifndef GLFWEW_H_INCLUDED
#define GLFWEW_H_INCLUDED
#include "GamePad.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <functional>

namespace GLFWEW {

/**
* GLFWとGLEWのラッパークラス.
*/
class Window
{
public:
  static Window& Instance();
  bool Init(int w, int h, const char* title);
  bool ShouldClose() const;
  void SwapBuffers() const;
  const GamePad& GetGamePad() const;
  void Update();
  void ResetDeltaTime();
  float DeltaTime() const { return deltaTime; }
  glm::vec2 WindowSize() const;

  typedef std::function<void(int, int, int, int)> KeyCallbackType;
  void KeyCallback(const KeyCallbackType& func) { keyCallback = func; }

private:
  Window() = default;
  ~Window();
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  void UpdateGamePad();
  friend void KeyCallback(GLFWwindow*, int k, int s, int a, int m);

  bool isGLFWInitialized = false;
  bool isInitialized = false;
  GLFWwindow* window = nullptr;
  GamePad gamepad;

  KeyCallbackType keyCallback;

  double prevTime = 0;
  float deltaTime = 0;
};

} // namespace GLFWEW

#endif // GLFWEW_H_INCLUDED
