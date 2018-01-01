/**
* @file cocos2d.cpp
*/
#include "cocos2d.h"
#include <iostream>
namespace cocos2d {

const Vec2 Vec2::ZERO(0, 0);

Sprite* Sprite::create(const char* name)
{
  Sprite* p = new Sprite;
  if (p) {
    if (!p->init()) {
      delete p;
      return nullptr;
    }
    p->setTexture(name);
  }
  return p;
}

void Sprite::setTexture(const char* name)
{
  Texture(TextureCache::getInstance()->load(name));
  Rectangle({ glm::vec2(0, 0), glm::vec2(static_cast<float>(Texture()->Width()), static_cast<float>(Texture()->Height())) });
}

void Sprite::setTextureRect(const Rect& r)
{
  Rectangle({ glm::vec2(r.origin.x, r.origin.y), glm::vec2(r.size.width, r.size.height) });
}

Director* Director::getInstance()
{
  static Director instance;
  return &instance;
}

void Director::runWithScene(Scene* s)
{
  if (pScene) {
    delete pScene;
  }
  pScene = s;
}

void Director::replaceScene(Scene* s)
{
  if (pScene) {
    delete pScene;
  }
  pScene = s;
}

void Director::mainLoop(float dt)
{
  if (pScene) {
    pScene->update(dt);
    pScene->UpdateRecursive(dt);
  }
}

void Director::end()
{
  if (pScene) {
    delete pScene;
  }
  pScene = nullptr;
}

Size Director::getWinSize() const
{
  const glm::vec2 size = window->WindowSize();
  return Size(size.x, size.y);
}

Size Director::getVisibleSize() const
{
  const glm::vec2 size = window->WindowSize();
  return Size(size.x, size.y);
}

Point Director::getVisibleOrigin() const { return Point(0, 0); }


TextureCache* TextureCache::getInstance()
{
  static TextureCache instance;
  return &instance;
}

TexturePtr TextureCache::load(const char* name)
{
  auto itr = cache.find(name);
  if (itr != cache.end()) {
    return itr->second;
  }
  std::string ddsName = std::string("Resources/") + name;
  const size_t length = ddsName.size();
  if (length > 4 && std::equal(ddsName.begin() + length - 4, ddsName.end(), ".png")) {
    ddsName[length - 3] = 'd';
    ddsName[length - 2] = 'd';
    ddsName[length - 1] = 's';
  }
  TexturePtr p = Texture::LoadFromFile(ddsName.c_str());
  if (p) {
    cache.insert(std::make_pair(std::string(name), p));
  } else {
    std::cerr << "ERROR: " << name << "‚ð“Ç‚Ýž‚ß‚Ü‚¹‚ñ" << std::endl;
  }
  return p;
}

void TextureCache::clear()
{
  cache.clear();
}

} // namespace cocos2d