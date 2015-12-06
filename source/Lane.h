#pragma once

#include <set>

#include "Car.h"
#include "CarDirection.h"
#include "GameState.h"
#include "InputManager.h"
#include "Sprite.h"

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

  bool bboxCollision(cgf::Sprite& sprite);

private:
  int carCount, yPos;
  set<Car*> cars;
};