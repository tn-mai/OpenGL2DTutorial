#pragma once
#include "../Sprite.h"
#include "../GLFWEW.h"
#include <functional>
#include <unordered_map>
#include <mutex>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

#define CREATE_FUNC(type) \
static type* create() { \
  type* p = new type; \
  if (p) { \
    if (!p->init()) { delete p; p = nullptr; } \
  } \
  return p; \
}

#define CCLOG(str) std::cerr << str << std::endl;

#define CC_CALLBACK_1(func, arg0) std::bind(&func, arg0, std::placeholders::_1)

#define CC_SAFE_DELETE(p) if (p) { delete p; p = nullptr; } (void)0

#define USING_NS_CC using namespace cocos2d;

#define OutputDebugString(str) std::cerr << str << std::endl;

namespace cocos2d {

namespace experimental {}

class Ref {};

typedef std::function<void(Ref*)> CCMenuCallback;

class Event : public Ref {};

enum class CCTextAlignment
{
  LEFT,
  CENTER,
  RIGHT,
};

class EventKeyboard {
public:
  enum class KeyCode {
    KEY_ESCAPE = GLFW_KEY_ESCAPE,
    KEY_LEFT_ARROW = GLFW_KEY_LEFT,
    KEY_UP_ARROW = GLFW_KEY_UP,
    KEY_RIGHT_ARROW = GLFW_KEY_RIGHT,
    KEY_DOWN_ARROW = GLFW_KEY_DOWN,
    KEY_SPACE = GLFW_KEY_SPACE,
    KEY_A = GLFW_KEY_A,
    KEY_C = GLFW_KEY_C,
    KEY_D = GLFW_KEY_D,
    KEY_S = GLFW_KEY_S,
    KEY_X = GLFW_KEY_X,
    KEY_Z = GLFW_KEY_Z,
  };
};

class StringUtils {
public:
  static std::string format(const char* fmt, ...) { return std::string(fmt); }
};

template<typename T>
class NodeBase : public T
{
public:
  virtual bool init() { return true; }
  virtual void update(float dt) {
    for (auto& e : Children()) {
       static_cast<NodeBase*>(e)->update(dt);
    }
  }
  virtual void scheduleUpdate() {}
  virtual Size getContentSize() const { return Size(0, 0); }

  void autorelease() {}

  void setVisible(bool) {}
  bool isVisible() const { return true; }
  void addChild(Node* p, int zorder=0) { AddChild(p); }
  void removeFromParent() { Parent()->RemoveChild(this); }
  void setPosition(float x, float y) {
    Position(glm::vec3(x, y, 0));
  }
  void setPosition(const Vec2& p) { setPosition(p.x, p.y); }
};

class Node : public NodeBase<::Node>
{
};

class Sprite : public NodeBase<::Sprite>
{
public:
  static Sprite* create(const char*);
  void setTexture(const char* name);
  void setTextureRect(const Rect& r);
  virtual Size getContentSize() const override {
    if (auto tex = Texture()) {
      return Size(static_cast<float>(tex->Width()), static_cast<float>(tex->Height()));
    }
    return Size(0, 0);
  }
};

class MenuItem : public Node
{
};

class MenuItemImage : public MenuItem
{
public:
  static MenuItemImage* create(const char*, const char*, const CCMenuCallback&) { return new MenuItemImage; }
};

class Label : public Node
{
public:
  static Label* createWithTTF(const char*, const char*, int) { return new Label; }

  void setString(const std::string&) {}
  void setColor(const Color3B& c) {}
  void setSystemFontName(const char*) {}
  void setSystemFontSize(int) {}
  void setHorizontalAlignment(cocos2d::CCTextAlignment) {}
  void setDimensions(int, int) {}
};

class CCProgressTimer : public Sprite
{
public:
  enum class Type {
    BAR,
  };
  bool initWithSprite(Sprite* p) { return true; }
  void setPercentage(float) {}
  void setType(Type) {}
  void setBarChangeRate(Point) {}
  void setMidpoint(Point) {}
};
typedef CCProgressTimer ProgressTimer;

class TMXLayer : public Node
{
public:
  Size getLayerSize() const { return Size(8, 8); }
  Size getMapTileSize() { return Size(8, 8); }
};

class TMXTiledMap : public Node
{
public:
  TMXTiledMap() {
    layerList.resize(2);
  }
  TMXLayer* getLayer(const char*) { return &layerList[0]; }
  bool initWithTMXFile(const std::string&) { return true; }

private:
  std::vector<TMXLayer> layerList;
};

class Scene : public Node
{
public:
  Scene() = default;
  virtual ~Scene() = default;

  CREATE_FUNC(Scene)

  virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {}
  virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* unused_event) {}
};

class Layer : public Scene {
public:
  Layer() = default;
  virtual ~Layer() = default;

  CREATE_FUNC(Layer)

  virtual bool init() { return true; }
  virtual void update(float deltaTime) {}
  void setKeyboardEnabled(bool) {}
};

class Menu : public Layer
{
public:
  Menu() = default;
  virtual ~Menu() = default;

  static Menu* create(MenuItem*, const void*) { return new Menu; }
  static Menu* create(MenuItem*, MenuItem*, const void*) { return new Menu; }
};

class Director
{
public:
  static Director* getInstance();
  void runWithScene(Scene* s);
  void replaceScene(Scene* s);
  void mainLoop(float dt);
  void end();

  Size getWinSize() const;
  Size getVisibleSize() const;
  Point getVisibleOrigin() const;
  const Scene* getScene() const { return pScene; }
  void init(GLFWEW::Window* w) { window = w; }

private:
  Scene* pScene = nullptr;
  GLFWEW::Window* window = nullptr;
};

class TextureCache
{
public:
  static TextureCache* getInstance();
  TexturePtr load(const char* name);
  void clear();

private:
  TextureCache() = default;
  TextureCache(const TextureCache&) = default;
  TextureCache& operator=(const TextureCache&) = default;
  ~TextureCache() = default;

  std::unordered_map<std::string, TexturePtr> cache;
};

} // namespace cocos2d