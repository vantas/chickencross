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

#include "CGameState.h"
#include "CSprite.h"
#include "CImage.h"
#include "InputManager.h"
#include <MapLoader.h>
#include "ClockHUD.h"
//#include "TMXLoader.h"

class PlayState : public CGameState
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
    CSprite player;
    InputManager* im;
    tmx::MapLoader* map;
    sf::Font font;
    sfx::FrameClock clock;
    ClockHUD* hud;
};

#endif
