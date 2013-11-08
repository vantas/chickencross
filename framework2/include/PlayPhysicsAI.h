/*
 *  PlayPhysicsAI.h
 *  Testbed for Box2D (physics) experiments + AI (steering behaviours)
 *
 *  Created by Marcelo Cohen on 05/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#ifndef PLAY_PHYSICS_H_
#define PLAY_PHYSICS_H_

#include <list>
#include <SDL.h>
#include <Box2D/Box2D.h>
#include "CGameState.h"
#include "CSprite.h"
#include "TMXLoader.h"
#include "CPhysics.h"
#include "CFont.h"

struct Kinematic
{
    sf::Vector3f pos;
    sf::Vector3f vel;
    sf::Vector3f heading;
    float maxForce;
    float maxSpeed;
    float maxTurnRate;
};

class PlayPhysicsAI : public CGameState
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
    static PlayPhysicsAI* instance()
    {
        return &m_PlayPhysicsAI;
    }

protected:

    PlayPhysicsAI() {}

private:

    static PlayPhysicsAI m_PlayPhysicsAI;

    enum {
        PLAYER_ID, ENEMY_ID, WALL_ID
    };
    CPhysics* phys;
    b2Body* playerPhys, * enemyPhys;

    list<sf::Vector3f> trail;
    bool showTrails;

    int x, y;
    float cameraSpeed;
    float zvel;
    CSprite* player;
    CSprite* enemy;
    CFont* font;
    Kinematic playerK, enemyK;
    TMXLoader map;
    Uint8* keystate; // state of all keys (1 means key is pressed)
    bool firstTime;
    int blocks[256]; // indicate collision status of each block id

    void checkCollision(CGame* game);
    void checkCollision(CGame* game, Kinematic& obj);
    void centerPlayerOnMap(CGame* game);

    enum {
         CHASE_BEHAVIOR=0, ARRIVE_BEHAVIOR, PURSUIT_BEHAVIOR, FLEE_BEHAVIOR, EVADE_BEHAVIOR
    };
    int steerMode;

    const static string modes2[];

    sf::Vector3f chase(Kinematic& vehicle, sf::Vector3f& target); // ir diretamente ao jogador
    sf::Vector3f arrive(Kinematic& vehicle, sf::Vector3f& target, float decel=0.3); // ir diretamente ao jogador
    sf::Vector3f pursuit(Kinematic& vehicle, Kinematic& target); // perseguir o jogador, prevendo a posição futura
	sf::Vector3f flee(Kinematic& vehicle, sf::Vector3f& target, float panicDistance=100);  // fugir do jogador
    sf::Vector3f evade(Kinematic& vehicle, Kinematic& target);
};

#endif
