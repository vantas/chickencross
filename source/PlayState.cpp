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

  music.openFromFile("data/audio/road.wav");
  music.setVolume(30);  // 30% do volume máximo
  music.setLoop(true);  // modo de loop: repete continuamente.
  music.play();

  chicken.init();

  cout << "PlayState: Init" << endl;
}

void PlayState::cleanup()
{
  chicken.cleanup();
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
  /*sf::View view = screen->getView();
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
  screen->setView(view);*/
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

  chicken.handleEvents(game, im);
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
  chicken.update(game);
}

void PlayState::draw(cgf::Game* game)
{
  screen = game->getScreen();
  map->Draw(*screen);
  chicken.draw(game);
}
