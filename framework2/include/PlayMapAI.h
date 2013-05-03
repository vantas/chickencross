/*
 *  PlayMapAI.h
 *  Testbed for AI experiments
 *
 *  Created by Marcelo Cohen on 05/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#ifndef PLAY_MAPAI_H_
#define PLAY_MAPAI_H_

#include <SDL.h>
//#include <irrKlang.h>
#include "CGameState.h"
#include "CSprite.h"
#include "TMXLoader.h"

struct Kinematic
{
//    irrklang::vec3df pos;
//    irrklang::vec3df vel;
//    irrklang::vec3df heading;
    float maxForce;
    float maxSpeed;
    float maxTurnRate;
};

class PlayMapAI : public CGameState
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
    static PlayMapAI* instance()
    {
        return &m_PlayMapAI;
    }

protected:

    PlayMapAI() {}

private:

    static PlayMapAI m_PlayMapAI;

    int x, y;
    float cameraSpeed;
    float zvel;
    CSprite* player;
    CSprite* enemy;
    Kinematic playerK, enemyK;
    TMXLoader map;
    Uint8* keystate; // state of all keys (1 means key is pressed)
    bool firstTime;
    int blocks[256]; // indicate collision status of each block id

    void checkCollision(CGame* game);
    void checkCollision(CGame* game, Kinematic& obj);
    void centerPlayerOnMap(CGame* game);

    enum {
         CHASE_BEHAVIOR, ARRIVE_BEHAVIOR, PURSUIT_BEHAVIOR, FLEE_BEHAVIOR, EVADE_BEHAVIOR
    };
    int steerMode;
    irrklang::vec3df chase(Kinematic& vehicle, irrklang::vec3df& target); // ir diretamente ao jogador
    irrklang::vec3df arrive(Kinematic& vehicle, irrklang::vec3df& target, float decel=0.3); // ir diretamente ao jogador
    irrklang::vec3df pursuit(Kinematic& vehicle, Kinematic& target); // perseguir o jogador, prevendo a posição futura
	irrklang::vec3df flee(Kinematic& vehicle, irrklang::vec3df& target, float panicDistance=100);  // fugir do jogador
    irrklang::vec3df evade(Kinematic& vehicle, Kinematic& target);
};

#endif
