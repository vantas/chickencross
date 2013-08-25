/*
 *  PlayState.h
 *  Normal "play" state
 *
 *  Created by Marcelo Cohen on 04/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#ifndef PLAY_STATE_H_
#define PLAY_STATE_H_

#include "GameState.h"
#include "Sprite.h"
#include "CImage.h"
#include "InputManager.h"
#include <MapLoader.h>
//#include "TMXLoader.h"

class PlayState : public cgf::GameState
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
    static PlayState* instance()
    {
        return &m_PlayState;
    }

    protected:

    PlayState() {}

    private:

    static PlayState m_PlayState;

    int x, y;
    int dirx, diry;
    CImage playSprite1;
    CImage playSprite2;
    CImage playSprite3;
    cgf::Sprite player;
    sf::RenderWindow* screen;
    InputManager* im;
    tmx::MapLoader* map;
};

#endif
