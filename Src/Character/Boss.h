/**
* @file Boss.h
*/
#ifndef CHARACTER_BOSS_H_INCLUDED
#define CHARACTER_BOSS_H_INCLUDED
#include "Character.h"

namespace Character {

class Boss;
using BossPtr = std::shared_ptr<Boss>;

/**
* �{�X.
*/
class Boss : public CollidableSprite
{
public:
  Boss(const TexturePtr& tex, const NodePtr& player, std::vector<CollidableSpritePtr>& enemyList, std::vector<CollidableSpritePtr>& enemyShotList, std::vector<FrameAnimation::TimelinePtr>& timelineList);

private:
  Node escortNode;
  NodePtr player;
  std::vector<CollidableSpritePtr>& enemyList;
  std::vector<CollidableSpritePtr>& enemyShotList;
};

} // namespace Character

#endif // CHARACTER_BOSS_H_INCLUDED