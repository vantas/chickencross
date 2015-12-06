#include "Lane.h"
#include "Game.h"
#include "InputManager.h"

Lane::Lane(int carCount, int yPos)
  : carCount(carCount), yPos(yPos)
  {}

void Lane::init()
{
  for (int i = 0; i < carCount; i++)
  {
    auto car = new Car(rand() % 801, yPos);
    car->init();
    cars.insert(car);
  }
}

void Lane::cleanup() {
  for (auto it = cars.begin(); it != cars.end(); ++it)
  {
    (*it)->cleanup();
    delete *it;
  }
}

void Lane::handleEvents(cgf::Game* game)
{
  for (auto it = cars.begin(); it != cars.end(); ++it)
    (*it)->handleEvents(game);
}

void Lane::update(cgf::Game* game)
{
  for (auto it = cars.begin(); it != cars.end(); ++it)
    (*it)->update(game);
}

void Lane::draw(cgf::Game* game)
{
  for (auto it = cars.begin(); it != cars.end(); ++it)
    (*it)->draw(game);
}

bool Lane::bboxCollision(cgf::Sprite& sprite)
{
  for (auto it = cars.begin(); it != cars.end(); ++it)
  {
    auto carSprite = (*it)->getSprite();
    if (carSprite.bboxCollision(sprite))
      return true;
  }
  return false;
}