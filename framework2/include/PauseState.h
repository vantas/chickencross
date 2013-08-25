/*
 *  PauseState.h
 *  Example "pause" state
 *
 *  Created by Marcelo Cohen on 04/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#ifndef PAUSE_STATE_H_
#define PAUSE_STATE_H_

//#include <SDL.h>
#include "GameState.h"
#include "CImage.h"


class CFont;

class PauseState : public cgf::GameState
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
    static PauseState* instance()
    {
        return &m_PauseState;
    }

    protected:

    PauseState() {}

    private:

    static PauseState m_PauseState;

    CImage* pauseSprite;
    CFont*  pauseFont;

};

#endif
