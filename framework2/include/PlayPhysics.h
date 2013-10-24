/*
 *  PlayPhysics.h
 *  Testbed for Box2D (physics) experiments
 *
 *  Created by Marcelo Cohen on 09/13.
 *  Copyright 2013 PUCRS. All rights reserved.
 *
 */

#ifndef PLAY_PHYSICS_H_
#define PLAY_PHYSICS_H_

#include "GameState.h"
#include "Sprite.h"
#include "InputManager.h"
#include <MapLoader.h>
#include "Physics.h"

class PlayPhysics : public cgf::GameState
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
    static PlayPhysics* instance()
    {
        return &m_PlayPhysics;
    }

    protected:

    PlayPhysics() {}

    private:

    static PlayPhysics m_PlayPhysics;

    // Get a cell GID from the map (x and y are view coords)
    sf::Uint16 getCellFromMap(uint8_t layernum, float x, float y);

    // Centers the camera on the player position (if player is too close to the borders, stop)
    void centerMapOnPlayer();

    bool checkCollision(uint8_t layer, cgf::Game* game, cgf::Sprite* obj);

    bool firstTime;

    int x, y;
    int dirx, diry;
    cgf::Sprite player;
    cgf::Sprite ghost;
//    cgf::Sprite playSprite2;
//    cgf::Sprite playSprite3;
    sf::RenderWindow* screen;
    cgf::InputManager* im;
    tmx::MapLoader* map;
    sf::Font font;
    sf::Text text;

    cgf::Physics* phys;
    b2Body* bplayer, *bghost;

};

#endif
