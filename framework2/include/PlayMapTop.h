/*
 *  PlayMapTop.h
 *  Example gameplay: playing from a top view
 *
 *  Created by Marcelo Cohen on 09/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#ifndef PLAY_MAPTOP_H_
#define PLAY_MAPTOP_H_

#include <SDL.h>
#include "CGameState.h"
#include "CSprite.h"
#include "CFont.h"
#include "TMXLoader.h"

class PlayMapTop : public CGameState
{
public:

    void init();
    void cleanup();

    void pause();
    void resume();

    void handleEvents(CGame* game);
    void update(CGame* game);
    void draw(CGame* game);

    // Implement Singleton Pattern
    static PlayMapTop* instance()
    {
        return &m_PlayMapTop;
    }

protected:

    PlayMapTop() {}

private:

    static PlayMapTop m_PlayMapTop;

    int x, y;
    float speed;
    CSprite* player;
    TMXLoader map;
    Uint8* keystate; // state of all keys (1 means key is pressed)
    bool firstTime;

    void checkCollision(CGame* game, CSprite* obj);
    void centerPlayerOnMap(CGame* game);
};

#endif
