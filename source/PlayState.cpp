/*
 *  PlayState.cpp
 *  Normal "play" state
 *
 *  Created by Marcelo Cohen on 08/13.
 *  Copyright 2013 PUCRS. All rights reserved.
 *
 */

#include <cmath>
#include <iostream>

#include "Game.h"
#include "InputManager.h"
#include "PlayState.h"
#include "WonState.h"

PlayState PlayState::m_PlayState;

using namespace std;

void PlayState::init()
{
  im = cgf::InputManager::instance();
  im->addKeyInput("space", sf::Keyboard::Space);
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

  int carY[10] = { 35, 95, 165, 225, 290, 350, 420, 480, 545, 605 };
  for (int i = 0; i < 10; i++)
  {
    auto car = new Car(rand() % 801, carY[i]);
    car->init();
    cars.insert(car);
  }

  chicken.init();

  cout << "PlayState: Init" << endl;
}

void PlayState::cleanup()
{
  chicken.cleanup();

  for (auto it = cars.begin(); it != cars.end(); ++it)
  {
    (*it)->cleanup();
    delete *it;
  }
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
  for (auto it = cars.begin(); it != cars.end(); ++it)
    (*it)->handleEvents(game, im);
}

void PlayState::update(cgf::Game* game)
{
  chicken.update(game);
  for (auto it = cars.begin(); it != cars.end(); ++it)
    (*it)->update(game);
}

void PlayState::draw(cgf::Game* game)
{
  screen = game->getScreen();
  map->Draw(*screen);
  chicken.draw(game);
  for (auto it = cars.begin(); it != cars.end(); ++it)
    (*it)->draw(game);
}
