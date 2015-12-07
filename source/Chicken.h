#pragma once

#include "GameState.h"
#include "InputManager.h"
#include "Sprite.h"

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
  void die();

  cgf::Sprite& getSprite() { return sprite; }

private:
  enum { RIGHT=0, LEFT, UP, DOWN };
  string walkStates[4];
  int currentDir;
  int dirx, diry;
  bool isAlive;

  sf::RenderWindow* screen;
  cgf::Sprite sprite;
  cgf::Sprite deadSprite;

  sf::SoundBuffer cluckSoundBuffer;
  sf::Sound cluckSound;

  sf::SoundBuffer fartSoundBuffer;
  sf::Sound fartSound;
};