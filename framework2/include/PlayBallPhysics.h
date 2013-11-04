/*
 *  PlayBallPhysics.h
 *  Example gameplay: ball-hits-sheep, with physics...
 *
 *  Created by Marcelo Cohen on 10/13.
 *  Copyright 2013 PUCRS. All rights reserved.
 *
 */

#ifndef PLAY_BALL_PHYS_H_
#define PLAY_BALL_PHYS_H_

#include "GameState.h"
#include "Sprite.h"
#include "InputManager.h"
#include <MapLoader.h>
#include "Physics.h"

class PlayBallPhysics : public cgf::GameState
{
public:

    void init();
    void cleanup();

    void pause();
    void resume();

    void handleEvents(cgf::Game* game);
    void update(cgf::Game* game);
    void draw(cgf::Game* game);

    // Implement Singleton Pattern
    static PlayBallPhysics* instance()
    {
        return &m_PlayBallPhysics;
    }

protected:

    PlayBallPhysics() {}

private:

    static PlayBallPhysics m_PlayBallPhysics;

    int x, y;
    float speed;
    sf::RenderWindow* screen;
    cgf::InputManager* im;
    tmx::MapLoader* map;
    cgf::Sprite ball;
    cgf::Sprite sheep;
    cgf::Sprite block;
    cgf::Sprite boom;

    sf::Font font;
    sf::Text text;

    bool firstTime;
    bool exploding;

    cgf::Physics* phys;
    b2Body* pball, *psheep;
    b2Body* pblock;
    enum { BALL_ID, SHEEP_ID, WALL_ID, GROUND_ID };

    // Centers the camera on the player position (if player is too close to the borders, stop)
    void centerMapOnPlayer();
};

#endif
