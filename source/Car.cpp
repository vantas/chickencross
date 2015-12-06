#include <iostream>
#include <cmath>

#include "Car.h"
#include "Game.h"
#include "InputManager.h"
#include "PlayState.h"
#include "WonState.h"

Car::Car(int inix, int iniy)
  : inix(inix), iniy(iniy)
  {}

void Car::init()
{
  driveStates[0] = "drive-right";
  driveStates[1] = "drive-left";
  driveStates[2] = "drive-up";
  driveStates[3] = "drive-down";

  if (rand() % LAST_DIRECTION == LEFT)
  {
    dirx = 1;
    currentDir = RIGHT;
  }
  else
  {
    dirx = -1;
    currentDir = LEFT;
  }
  diry = 0;

  auto color = &colors[static_cast<CarColor>(rand() % LAST_COLOR)][0u];
  sprite.load(color, 115, 60, 0, 20, 0, 20, 4, 4, 16);
  sprite.setPosition(inix,iniy);
  sprite.loadAnimation("data/maps/car.xml");
  sprite.setAnimation(driveStates[currentDir]);
  sprite.setAnimRate(15);
  sprite.play();

  speed = 180 + (rand() % (int)(300 - 100 + 1));
}

void Car::cleanup() {}

void Car::handleEvents(cgf::Game* game)
{
  int newDir = currentDir;

  sf::Vector2f pos = sprite.getPosition();
  if (pos.x <= -150) // menor que zero pra dar o efeito de "sumindo" da tela
  {
    dirx = 1;
    newDir = RIGHT;
  }

  if (pos.x >= game->getScreen()->getSize().x)
  {
    dirx = -1;
    newDir = LEFT;
  }

  if (currentDir != newDir)
  {
    // atualiza cor, animação e velocidade
    auto color = &colors[static_cast<CarColor>(rand() % LAST_COLOR)][0u];
    sprite.load(color, 115, 60, 0, 20, 0, 20, 4, 4, 16);
    sprite.setAnimation(driveStates[newDir]);
    currentDir = newDir;
    sprite.play();

    speed = 180 + (rand() % (int)(300 - 100 + 1));
  }

  sprite.setXspeed(speed * dirx);
}

void Car::update(cgf::Game* game)
{
  sprite.update(game->getUpdateInterval());
}

void Car::draw(cgf::Game* game)
{
  screen = game->getScreen();
  screen->draw(sprite);
}