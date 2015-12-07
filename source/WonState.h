/*
 *  WonState.h
 *
 */

#pragma once

#include "GameState.h"
#include "Sprite.h"

class WonState : public cgf::GameState
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
  static WonState* instance()
  {
    return &m_WonState;
  }

protected:

  WonState() {}

private:

  static WonState m_WonState;

  cgf::Sprite wonSprite;
  sf::Music music;
};
