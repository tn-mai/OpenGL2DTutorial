/**
* @file Boss.h
*/
#ifndef CHARACTER_BOSS_H_INCLUDED
#define CHARACTER_BOSS_H_INCLUDED
#include "Character.h"

namespace GameObject {

class Boss;
using BossPtr = std::shared_ptr<Boss>;

/**
* �{�X.
*/
class Boss : public Character
{
public:
  Boss(const TexturePtr& tex, const NodePtr& player, std::vector<CharacterPtr>& enemyList, std::vector<CharacterPtr>& enemyShotList, std::vector<FrameAnimation::TimelinePtr>& timelineList);
  const std::deque<Node*>& EscortList() const { return escortNode.Children(); }
  void Shot(const CharacterPtr& shot)
  {
    enemyShotList.push_back(shot);
    Parent()->AddChild(shot.get());
  }

private:
  Node escortNode;
  NodePtr player;
  std::vector<CharacterPtr>& enemyList;
  std::vector<CharacterPtr>& enemyShotList;
};

} // namespace GameObject

#endif // CHARACTER_BOSS_H_INCLUDED