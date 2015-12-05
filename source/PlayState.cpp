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
  walkStates[0] = "walk-right";
  walkStates[1] = "walk-left";
  walkStates[2] = "walk-up";
  walkStates[3] = "walk-down";
  currentDir = RIGHT;

  //                                           w,  h,  hSpace, vSpace, xIni, yIni, cols, rows, total
  chickenSprite.load("data/maps/chicken.png", 32, 32,      20,     32,    8,   24,    3,    4,    12);
  chickenSprite.setPosition(400,550);

  chickenSprite.loadAnimation("data/maps/chicken.xml");
  chickenSprite.setAnimation(walkStates[currentDir]);
  chickenSprite.setAnimRate(15);
  chickenSprite.play();

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

void PlayState::centerMapOnPlayer()
{
  sf::View view = screen->getView();
  sf::Vector2u mapsize = map->GetMapSize();
  sf::Vector2f viewsize = view.getSize();
  viewsize.x /= 2;
  viewsize.y /= 2;
  sf::Vector2f pos = chickenSprite.getPosition();

  float panX = viewsize.x; // minimum pan
  if(pos.x >= viewsize.x)
    panX = pos.x;

  if(panX >= mapsize.x - viewsize.x)
    panX = mapsize.x - viewsize.x;

  float panY = viewsize.y; // minimum pan
  if(pos.y >= viewsize.y)
    panY = pos.y;

  if(panY >= mapsize.y - viewsize.y)
    panY = mapsize.y - viewsize.y;

  view.setCenter(sf::Vector2f(panX,panY));
  screen->setView(view);
}

void PlayState::handleEvents(cgf::Game* game)
{
  screen = game->getScreen();
  sf::Event event;

  while (screen->pollEvent(event))
  {
    if (event.type == sf::Event::Closed)
      game->quit();
  }

  dirx = diry = 0;
  int newDir = currentDir;

  if (im->testEvent("left"))
  {
    dirx = -1;
    newDir = LEFT;
  }

  if (im->testEvent("right"))
  {
    dirx = 1;
    newDir = RIGHT;
  }

  if (im->testEvent("up"))
  {
    diry = -1;
    newDir = UP;
  }

  if (im->testEvent("down"))
  {
    diry = 1;
    newDir = DOWN;
  }

  if (im->testEvent("quit") || im->testEvent("rightclick"))
    game->quit();

  if (im->testEvent("stats"))
    game->toggleStats();

  if (dirx == 0 && diry == 0)
    chickenSprite.pause();
  else
  {
    if (currentDir != newDir)
    {
      chickenSprite.setAnimation(walkStates[newDir]);
      currentDir = newDir;
    }
    chickenSprite.play();
  }

  chickenSprite.setXspeed(100*dirx);
  chickenSprite.setYspeed(100*diry);
}

void PlayState::update(cgf::Game* game)
{
  /*float x = chickenSprite.getPosition().x;
  float y = chickenSprite.getPosition().y;
  x += dirx*5;
  y += diry*5;

  if (y < 50) {
    cout << "Player won, yo" << endl;
    game->changeState(WonState::instance());
  }

  screen = game->getScreen();
  centerMapOnPlayer();

  chickenSprite.setPosition(x,y);*/
  chickenSprite.update(game->getUpdateInterval());
}

void PlayState::draw(cgf::Game* game)
{
  screen = game->getScreen();
  map->Draw(*screen);
  screen->draw(chickenSprite);
}
