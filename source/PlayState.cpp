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
  playSprite1.load("data/maps/chicken.png", 32, 32, 20, 32, 8, 24, 3, 4, 12);
  playSprite1.loadAnimation("data/maps/chicken.xml");
  playSprite1.setPosition(400,550);
  //playSprite1.setAnimRate(1);
  playSprite1.setAnimation("walk-up");

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

  map = new tmx::MapLoader("data/maps/roads");

  map->Load("roads.tmx");

  cout << "PlayState: Init" << endl;
}

void PlayState::cleanup()
{
  delete map;
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

  if(im->testEvent("left")) {
    playSprite1.setAnimation("walk-left");
    dirx = -1;
  }

  if(im->testEvent("right")) {
    playSprite1.setAnimation("walk-right");
    dirx = 1;
  }

  if(im->testEvent("up")) {
    playSprite1.setAnimation("walk-up");
    diry = -1;
  }

  if(im->testEvent("down")) {
    playSprite1.setAnimation("walk-down");
    diry = 1;
  }

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
  playSprite1.update(game->getUpdateInterval());
  //playSprite1.play();
  player.update(game->getUpdateInterval());
}

void PlayState::draw(cgf::Game* game)
{
  screen = game->getScreen();
  map->Draw(*screen);
  screen->draw(playSprite1);
}
