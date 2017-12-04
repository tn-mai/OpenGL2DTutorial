/**
* @file GLFWEW.h
*/
#ifndef GLFWEW_H_INCLUDED
#define GLFWEW_H_INCLUDED
#include "GamePad.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

private:
  Window() = default;
  ~Window();
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  void UpdateGamePad();

  bool isGLFWInitialized = false;
  bool isInitialized = false;
  GLFWwindow* window = nullptr;
  GamePad gamepad;

  double prevTime = 0;
  float deltaTime = 0;
};

} // namespace GLFWEW

#endif // GLFWEW_H_INCLUDED
