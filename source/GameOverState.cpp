/*
*
*  GameOverState.cpp
*
*/

#include <iostream>
#include "Game.h"
#include "InputManager.h"
#include "GameOverState.h"
#include "PlayState.h"

GameOverState GameOverState::m_GameOverState;

using namespace std;

void GameOverState::init()
{
  gameOverSprite.load("data/img/gameover.png"); // load menu state bitmap

  music.openFromFile("data/audio/funeral.wav");
  music.setVolume(100);
  music.setLoop(true);  // modo de loop: repete continuamente.
  music.play();

  cout << "GameOverState Init Successful" << endl;
}

void GameOverState::cleanup()
{
  cout << "GameOverState Cleanup Successful" << endl;
}

void GameOverState::pause()
{
  cout << "GameOverState Paused" << endl;
}

void GameOverState::resume()
{
  cout << "GameOverState Resumed" << endl;
}

void GameOverState::handleEvents(cgf::Game* game)
{
  sf::Event event;
  sf::RenderWindow* screen = game->getScreen();

  while (screen->pollEvent(event))
  {
// check the type of the event...
    switch (event.type)
    {
      case sf::Event::Closed:
        game->quit();
        break;

      case sf::Event::KeyPressed:
        if(event.key.code == sf::Keyboard::Space)
          game->changeState(PlayState::instance());
        if(event.key.code == sf::Keyboard::Escape)
          game->quit();
        break;

      default:
        break;
    }
  }
}

void GameOverState::update(cgf::Game* /*game*/)
{
}

void GameOverState::draw(cgf::Game *game)
{
  gameOverSprite.setPosition(50,50);
  game->getScreen()->draw(gameOverSprite);
}
