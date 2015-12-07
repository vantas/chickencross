/*
 *  GameOverState.h
 *
 */

#pragma once

#include "GameState.h"
#include "Sprite.h"

class GameOverState : public cgf::GameState
{
public:

  void init();
  void cleanup();

  void pause();
  void resume();

  void handleEvents(cgf::Game* game);
  void update(cgf::Game* game);
  void draw(cgf::Game* game);

    // Implement Singleton Pattern
  static GameOverState* instance()
  {
    return &m_GameOverState;
  }

protected:

  GameOverState() {}

private:

  static GameOverState m_GameOverState;

  cgf::Sprite gameOverSprite;
  sf::Music music;
};
