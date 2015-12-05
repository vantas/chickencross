#include "chicken.h"
#include <iostream>
#include <cmath>
#include "Game.h"
#include "PlayState.h"
#include "InputManager.h"
#include "WonState.h"

void Chicken::init()
{
  walkStates[0] = "walk-right";
  walkStates[1] = "walk-left";
  walkStates[2] = "walk-up";
  walkStates[3] = "walk-down";
  currentDir = RIGHT;

  chickenSprite.load("data/maps/chicken.png", 32, 32, 20, 32, 8, 24, 3, 4, 12);
  chickenSprite.setPosition(400,550);

  chickenSprite.loadAnimation("data/maps/chicken.xml");
  chickenSprite.setAnimation(walkStates[currentDir]);
  chickenSprite.setAnimRate(15);
  chickenSprite.play();

  dirx = 0; // sprite direction: right (1), left (-1)
  diry = 0; // down (1), up (-1)

  chickenSoundBuffer.loadFromFile("data/audio/chicken.wav");
  chickenSound.setBuffer(chickenSoundBuffer);
  chickenSound.setAttenuation(0);
  chickenSound.setLoop(true);
  chickenSound.play();
}

void Chicken::cleanup() {}

void Chicken::handleEvents(cgf::Game* game, cgf::InputManager* im)
{
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

void Chicken::update(cgf::Game* game)
{
  chickenSprite.update(game->getUpdateInterval());
}

void Chicken::draw(cgf::Game* game)
{
  screen = game->getScreen();
  screen->draw(chickenSprite);
}