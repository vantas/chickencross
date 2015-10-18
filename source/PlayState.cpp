/*
 *  PlayState.cpp
 *  Normal "play" state
 *
 *  Created by Marcelo Cohen on 08/13.
 *  Copyright 2013 PUCRS. All rights reserved.
 *
 */

#include <iostream>
#include <cmath>
#include "Game.h"
#include "PlayState.h"
#include "InputManager.h"
#include "WonState.h"

PlayState PlayState::m_PlayState;

using namespace std;

void PlayState::init()
{
  playSprite1.load("data/img/Char14.png");
  playSprite1.setPosition(400,550);

  dirx = 0; // sprite direction: right (1), left (-1)
  diry = 0; // down (1), up (-1)

  im = cgf::InputManager::instance();

  im->addKeyInput("left", sf::Keyboard::Left);
  im->addKeyInput("right", sf::Keyboard::Right);
  im->addKeyInput("up", sf::Keyboard::Up);
  im->addKeyInput("down", sf::Keyboard::Down);
  im->addKeyInput("quit", sf::Keyboard::Escape);
  im->addKeyInput("stats", sf::Keyboard::S);
  im->addMouseInput("rightclick", sf::Mouse::Right);

  cout << "PlayState: Init" << endl;
}

void PlayState::cleanup()
{
  cout << "PlayState: Clean" << endl;
}

void PlayState::pause()
{
  cout << "PlayState: Paused" << endl;
}

void PlayState::resume()
{
  cout << "PlayState: Resumed" << endl;
}

void PlayState::handleEvents(cgf::Game* game)
{
  sf::Event event;

  while (screen->pollEvent(event))
  {
    if(event.type == sf::Event::Closed)
      game->quit();
  }

  dirx = diry = 0;

  if(im->testEvent("left"))
    dirx = -1;

  if(im->testEvent("right"))
    dirx = 1;

  if(im->testEvent("up"))
    diry = -1;

  if(im->testEvent("down"))
    diry = 1;

  if(im->testEvent("quit") || im->testEvent("rightclick"))
    game->quit();

  if(im->testEvent("stats"))
    game->toggleStats();
}

void PlayState::update(cgf::Game* game)
{
  float x = playSprite1.getPosition().x;
  float y = playSprite1.getPosition().y;
  x += dirx*5;
  y += diry*5;

  if (y < 50) {
    cout << "Player won, yo" << endl;
    game->changeState(WonState::instance());
  }

  playSprite1.setPosition(x,y);
  player.update(game->getUpdateInterval());
}

void PlayState::draw(cgf::Game* game)
{
  screen = game->getScreen();
  screen->draw(playSprite1);
}
