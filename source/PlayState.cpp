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
#include "PlayState.h"
#include "GameOverState.h"
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
    auto lane = new Lane(0, carY[i]);
    lane->init();
    lanes.insert(lane);
  }

  isGameOver = false;

  chicken.init();

  cout << "PlayState: Init" << endl;
}

void PlayState::cleanup()
{
  chicken.cleanup();

  for (auto it = lanes.begin(); it != lanes.end(); ++it)
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
  if (isGameOver && clock.getElapsedTime().asSeconds() - timeOfDeath.asSeconds() > 2.0f)
  {
    game->changeState(GameOverState::instance());
    return;
  }

  if (isGameWon && clock.getElapsedTime().asSeconds() - timeOfVictory.asSeconds() > 2.0f)
  {
    game->changeState(WonState::instance());
    return;
  }

  screen = game->getScreen();
  sf::Event event;

  while (screen->pollEvent(event))
  {
    if (event.type == sf::Event::Closed)
      game->quit();
  }

  chicken.handleEvents(game, im);
  for (auto it = lanes.begin(); it != lanes.end(); ++it)
    (*it)->handleEvents(game);
}

void PlayState::update(cgf::Game* game)
{
  chicken.update(game);

  if (chicken.getPosition() < 2)
    gameWon(game);

  for (auto it = lanes.begin(); it != lanes.end(); ++it)
  {
    auto lane = *it;
    lane->update(game);
    if (!isGameWon && lane->bboxCollision(chicken.getSprite()))
      gameOver(game);
  }
}

void PlayState::draw(cgf::Game* game)
{
  screen = game->getScreen();
  map->Draw(*screen);
  chicken.draw(game);
  for (auto it = lanes.begin(); it != lanes.end(); ++it)
    (*it)->draw(game);
}

void PlayState::gameOver(cgf::Game* game)
{
  if (!isGameOver)
  {
    chicken.die();
    timeOfDeath = clock.getElapsedTime();
    isGameOver = true;
  }
}

void PlayState::gameWon(cgf::Game* game)
{
  if (!isGameWon)
  {
    chicken.cluck();
    timeOfVictory = clock.getElapsedTime();
    isGameWon = true;
  }
}