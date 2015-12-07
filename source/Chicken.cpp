#include <iostream>
#include <cmath>

#include "Chicken.h"
#include "Game.h"
#include "InputManager.h"
#include "PlayState.h"
#include "WonState.h"

void Chicken::init()
{
  walkStates[0] = "walk-right";
  walkStates[1] = "walk-left";
  walkStates[2] = "walk-up";
  walkStates[3] = "walk-down";
  currentDir = RIGHT;

  sprite.load("data/maps/chicken.png", 32, 32, 20, 32, 8, 24, 3, 4, 12);
  sprite.setPosition(400,680);

  sprite.loadAnimation("data/maps/chicken.xml");
  sprite.setAnimation(walkStates[currentDir]);
  sprite.setAnimRate(15);
  sprite.play();

  dirx = 0; // sprite direction: right (1), left (-1)
  diry = 0; // down (1), up (-1)
  isAlive = true;

  fartSoundBuffer.loadFromFile("data/audio/chicken_death.wav");
  fartSound.setBuffer(fartSoundBuffer);
  fartSound.setAttenuation(0);

  cluckSoundBuffer.loadFromFile("data/audio/chicken.wav");
  cluckSound.setBuffer(cluckSoundBuffer);
  cluckSound.setAttenuation(0);
}

void Chicken::cleanup() {}

void Chicken::handleEvents(cgf::Game* game, cgf::InputManager* im)
{
  dirx = diry = 0;
  int newDir = currentDir;

  if (isAlive)
  {
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

    if (im->testEvent("space"))
    {
      cluck();
    }
  }

  if (im->testEvent("quit") || im->testEvent("rightclick"))
    game->quit();

  if (im->testEvent("stats"))
    game->toggleStats();

  if (dirx == 0 && diry == 0)
    sprite.pause();
  else
  {
    if (currentDir != newDir)
    {
      sprite.setAnimation(walkStates[newDir]);
      currentDir = newDir;
    }
    sprite.play();
  }

  sprite.setXspeed(60*dirx);
  sprite.setYspeed(60*diry);
}

void Chicken::update(cgf::Game* game)
{
  sprite.update(game->getUpdateInterval());
}

void Chicken::draw(cgf::Game* game)
{
  screen = game->getScreen();
  screen->draw(sprite);
}

void Chicken::cluck()
{
  cluckSound.play();
}

void Chicken::die()
{
  if (isAlive) {
    fartSound.play();
    isAlive = false;
    sprite.load("data/img/dead_marylou.png");
  }
}