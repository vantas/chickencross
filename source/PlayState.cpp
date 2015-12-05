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
  chicken.update(game);
}

void PlayState::draw(cgf::Game* game)
{
  screen = game->getScreen();
  map->Draw(*screen);
  chicken.draw(game);
}
