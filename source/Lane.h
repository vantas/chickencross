#pragma once

#include <set>

#include "Car.h"
#include "CarDirection.h"
#include "GameState.h"
#include "InputManager.h"

using namespace std;

class Lane
{
public:
  Lane(int, int);
  void init();
  void cleanup();

  void handleEvents(cgf::Game*);
  void update(cgf::Game*);
  void draw(cgf::Game*);

private:
  int carCount, yPos;
  set<Car*> cars;
  sf::RenderWindow* screen;
};