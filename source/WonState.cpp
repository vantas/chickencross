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
  wonSprite.load("data/img/won.png"); // load menu state bitmap
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
// window closed
      case sf::Event::Closed:
        game->quit();
        break;

// key pressed
      case sf::Event::KeyPressed:
        if(event.key.code == sf::Keyboard::Space)
          game->changeState(PlayState::instance());
        if(event.key.code == sf::Keyboard::Escape)
          game->quit();
  //game->changeState(PlayMap::instance());
  //game->changeState(PlayMapTop::instance());
  //game->changeState(PlayMapAI::instance());
  //game->changeState(PlayPhysics::instance());
  //game->changeState(PlayMapPhysics::instance());
        break;

// we don't process other types of events
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
  // menuSprite->draw(game->getScreen());
  game->getScreen()->draw(wonSprite);
}
