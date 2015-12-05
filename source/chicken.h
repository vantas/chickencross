#pragma once

#include "GameState.h"
#include "Sprite.h"
#include "InputManager.h"

using namespace std;

class Chicken
{
public:
  void init();
  void cleanup();

  void handleEvents(cgf::Game*, cgf::InputManager*);
  void update(cgf::Game*);
  void draw(cgf::Game*);

  void cluck();

private:
  enum { RIGHT=0, LEFT, UP, DOWN };
  string walkStates[4];
  int currentDir;
  int dirx, diry;

  sf::RenderWindow* screen;
  cgf::Sprite chickenSprite;

  sf::SoundBuffer chickenSoundBuffer;
  sf::Sound chickenSound;
};