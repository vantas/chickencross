/*
 *  PlayState.h
 *  Normal "play" state
 *
 *  Created by Marcelo Cohen on 08/13.
 *  Copyright 2013 PUCRS. All rights reserved.
 *
 */

#pragma once

#include <set>
#include <tmx/MapLoader.h>

#include "Chicken.h"
#include "GameState.h"
#include "InputManager.h"
#include "Lane.h"

using namespace std;

class PlayState : public cgf::GameState
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
  static PlayState* instance()
  {
      return &m_PlayState;
  }

protected:
  PlayState() {}

private:
  static PlayState m_PlayState;

  sf::RenderWindow* screen;
  cgf::InputManager* im;
  tmx::MapLoader* map;
  sf::Music music;

  Chicken chicken;
  set<Lane*> lanes;

  sf::Clock clock;
  sf::Time timeOfDeath;
  sf::Time timeOfVictory;

  bool isGameWon;
  bool isGameOver;

  void centerMapOnPlayer();
  void gameOver(cgf::Game* game);
  void gameWon(cgf::Game* game);
};
