/*
 *  PlayMap.h
 *  Normal "play" state - tiled map viewing
 *
 *  Created by Marcelo Cohen on 04/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#ifndef PLAY_MAP_H_
#define PLAY_MAP_H_

#include <SDL.h>
#include "CGameState.h"
#include "CSprite.h"
#include "TMXLoader.h"

class PlayMap : public CGameState
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
    static PlayMap* instance()
    {
        return &m_PlayMap;
    }

    protected:

    PlayMap() {}

    private:

    static PlayMap m_PlayMap;

    int x, y;
    float cameraSpeed;
    float xvel, yvel;
    float zvel;
    bool onGround; // true if player is on the ground
    CSprite* player;
    CSprite* npc;
    float playerX, playerY;
    TMXLoader map;
    Uint8* keystate; // state of all keys (1 means key is pressed)
    bool firstTime;
    float playerAnimRate;
    static const float G;
    static const float MAX_FALL_SPEED;

    // Collision with map

    void checkCollision(CGame* game);
    void centerPlayerOnMap(CGame* game);

    // Audio
    /*
    irrklang::ISoundSource* walkSoundSource;
    irrklang::ISound* walkSound;
    irrklang::ISoundSource* jumpSoundSource;
    irrklang::ISoundSource* superJumpSoundSource;
    irrklang::ISoundSource* smurf3DSoundSource;
    */
};

#endif
