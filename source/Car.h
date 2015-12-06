#pragma once

#include <map>

#include "CarColor.h"
#include "CarDirection.h"
#include "GameState.h"
#include "InputManager.h"
#include "Sprite.h"

using namespace std;

class Car
{
public:
  Car(int, int);
  void init();
  void cleanup();

  void handleEvents(cgf::Game*);
  void update(cgf::Game*);
  void draw(cgf::Game*);

  cgf::Sprite& getSprite() { return sprite; }

private:
  enum { RIGHT=0, LEFT, UP, DOWN };
  string driveStates[4];
  int currentDir;
  int dirx, diry, inix, iniy;
  int speed;

  CarColor color;
  map<CarColor, string> colors
  {
    { RED,    "data/maps/carred.png"},
    { BLUE,   "data/maps/carblue.png"},
    { GREEN,  "data/maps/cargreen.png"},
    { YELLOW, "data/maps/caryellow.png" }
  };

  sf::RenderWindow* screen;
  cgf::Sprite sprite;
};