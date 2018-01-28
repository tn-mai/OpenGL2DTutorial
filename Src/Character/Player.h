/**
* @file Player.h
*/
#ifndef CHARACTER_PLAYER_H_INCLUDED
#define CHARACTER_PLAYER_H_INCLUDED
#include "Character.h"
#include <vector>

namespace Character {

class Player;
using PlayerPtr = std::shared_ptr<Player>;

/**
* Ž©‹@.
*/
class Player : public CollidableSprite
{
public:
  static PlayerPtr Create(const TexturePtr& tex);
  explicit Player(const TexturePtr& tex);

  virtual void Update(glm::f32 dt) override;
  virtual void Die();

  void Shot(glm::f32 rot, glm::f32 vel, int atk);
  std::vector<CollidableSpritePtr>& ShotList() { return shotList; }
  void RemoveDeadShot();
  void GameOver(bool flag) { gameover = flag; }
  bool GameOver() const { return gameover; }
  bool Invinsible() const { return invinsibleTimer > 0; }
  void GameClear(bool flag) { gameclear = flag; }
  bool GameClear() const { return gameclear; }

private:
  bool gameclear = false;
  bool gameover = false;
  bool controllable = true;
  float invinsibleTimer = 2;
  std::vector<CollidableSpritePtr> shotList;
};

} // namespace Character

#endif // CHARACTER_PLAYER_H_INCLUDED