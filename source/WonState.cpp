/*
*
*  WonState.cpp
*
*/

#include <iostream>
#include "Game.h"
#include "InputManager.h"
#include "WonState.h"
#include "PlayState.h"

WonState WonState::m_WonState;

using namespace std;

void WonState::init()
{
  wonSprite.load("data/img/chicken_dance.png"); // load menu state bitmap

  music.openFromFile("data/audio/chicken_dance.wav");
  music.setVolume(100);  // 30% do volume máximo
  music.setLoop(true);  // modo de loop: repete continuamente.
  music.play();

  cout << "WonState Init Successful" << endl;
}

void WonState::cleanup()
{
  cout << "WonState Cleanup Successful" << endl;
}

void WonState::pause()
{
  cout << "WonState Paused" << endl;
}

void WonState::resume()
{
  cout << "WonState Resumed" << endl;
}

void WonState::handleEvents(cgf::Game* game)
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

void WonState::update(cgf::Game* /*game*/)
{
}

void WonState::draw(cgf::Game *game)
{
  wonSprite.setPosition(50,50);
  game->getScreen()->draw(wonSprite);
}
