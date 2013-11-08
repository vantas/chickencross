/*
 *  PlayPhysicsAIAI.cpp
 *  Testbed for Box2D (physics) experiments + AI (steering behaviours)
 *
 *  Created by Marcelo Cohen on 05/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#include <iostream>
#include <SDL.h>
#include "Graphics.h"
#include <cmath>
#include "CGame.h"
#include "PauseState.h"
#include "PlayPhysicsAI.h"
#include "VectorUtils.h"

PlayPhysicsAI PlayPhysicsAI::m_PlayPhysicsAI;

const string PlayPhysicsAI::modes2[] = { "Chase", "Arrive", "Pursuit", "Flee", "Evade" };

using namespace std;

void PlayPhysicsAI::init()
{
    map.loadMap("data/maps/dungeon.tmx");
	cout << "Collision objects: " << map.totalObjects() << endl;
    playerK.vel.x = playerK.vel.y = 0;   // current player speed
    cameraSpeed = 18;   // speed to use
    zvel = 0;

    keystate = SDL_GetKeyState(NULL);

    player = new CSprite();
    player->loadSprite("data/img/Char19s.png", 32, 32, 0, 0, 0, 0, 1, 1, 1);
//    map.getCenter(2,2,playerK.pos.X,playerK.pos.Y);
    map.getCenter(35,5,playerK.pos.x,playerK.pos.y);

    enemy = new CSprite();
    enemy->loadSprite("data/img/Char14s.png", 32, 32, 0, 0, 0, 0, 1, 1, 1);
    float ex, ey;
//    map.getCenter(12,15,ex,ey);
    map.getCenter(45,4,ex,ey);
    enemyK.pos.x = ex;
    enemyK.pos.y = ey;
    enemyK.maxSpeed = 10;

    steerMode = CHASE_BEHAVIOR; // default: chase player

    firstTime = true; // to set map position at first update

    // Init array of blocking ids
    for(int i=0; i<256; i++)
        blocks[i] = 0; // default: not blocking

    // Set indices of blocking ids
    blocks[0] = blocks[1] = blocks[2] = 1;
    blocks[4] = 1;
    for(int i=8; i<=14; i++)
        blocks[i] = 1;

    phys = CPhysics::instance();
    phys->setGravity(0);
    phys->setConvFactor(10);
    //float bx, by;
    //map.getCenter(33,17,bx,by);
    //b2Body* ground2= phys->newBox(bx  ,by ,48,16,1000,0.5,0.5,true);
    //b2Body* ground = phys->newBox(1200,500,200,10,1000,0.5,0.5,true); // ground
    //phys->setAngle(ground, 10); //15);
    player->setPosition(playerK.pos.x, playerK.pos.y);
    playerPhys = phys->newBoxImage(PLAYER_ID, player,1,0.5,0);
    playerPhys->SetLinearDamping(3);

    enemy->setPosition(enemyK.pos.x, enemyK.pos.y);
    enemyPhys = phys->newBoxImage(ENEMY_ID, enemy,1,0.5,0.1);
    enemyPhys->SetLinearDamping(3);

    playerPhys->SetFixedRotation(true);
    enemyPhys ->SetFixedRotation(true);
	for(int o=0; o<map.totalObjects(); o++)
	{
		Object& obj = map.getObject(o);
		phys->newBox(WALL_ID, obj.x,obj.y,obj.width,obj.height,1000,0.5,0.1,true);
	}

	font = new CFont();
    font->loadFont("data/fonts/lucida12.png", 112, 208);

    showTrails = false;

    // Set offset to add to translation when drawing debug shapes
    // in physics
    phys->setDrawOffset(map.getTileWidth()/2, map.getTileHeight()/2);
    cout << "PlayPhysicsAI Init Successful" << endl;
}

void PlayPhysicsAI::cleanup()
{
    delete player;
    //  delete playSprite2;
	cout << "PlayPhysicsAI Clean Successful" << endl;
}

void PlayPhysicsAI::pause()
{
	cout << "PlayPhysicsAI Paused" << endl;
}

void PlayPhysicsAI::resume()
{
	cout << "PlayPhysicsAI Resumed" << endl;
}

void PlayPhysicsAI::handleEvents(CGame* game)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				game->quit();
				break;

            case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
                    case SDLK_p:
                        game->pushState(PauseState::instance());
                        break;
                    case SDLK_a:
                        steerMode++;
                        if(steerMode > EVADE_BEHAVIOR)
                            steerMode = CHASE_BEHAVIOR;
                        break;
                    case SDLK_ESCAPE:
                        game->quit();
                        break;
                    case SDLK_t:
                        showTrails = !showTrails;
                        break;
                    case SDLK_f:
                        playerPhys->ApplyLinearImpulse(b2Vec2(0,-300), playerPhys->GetWorldCenter());
                        break;
                    default:
                        break;
                }
                break;
            case SDL_VIDEORESIZE:
                game->resize(event.resize.w, event.resize.h);
                //map.cleanup();
                //map.loadMap("data/maps/desert.tmx");
		}
	}
    playerK.vel.x = -keystate[SDLK_LEFT]*cameraSpeed + keystate[SDLK_RIGHT]*cameraSpeed;
    playerK.vel.y = -keystate[SDLK_UP]*cameraSpeed + keystate[SDLK_DOWN]*cameraSpeed;

    playerPhys->ApplyLinearImpulse(b2Vec2(playerK.vel.x,playerK.vel.y), playerPhys->GetWorldCenter());
    zvel = -5*keystate[SDLK_LSHIFT] + 5*keystate[SDLK_RSHIFT];
//    playerK.heading = playerK.vel / playerK.vel.getLength();
}

// Steering Behavior: chase target
sf::Vector3f PlayPhysicsAI::chase(Kinematic& vehicle, sf::Vector3f& target)
{
    sf::Vector3f desiredVel = target - vehicle.pos;
    vecutils::normalize(desiredVel);
    desiredVel *= vehicle.maxSpeed;
    return desiredVel - vehicle.vel;
}

// Steering Behavior: arrive at target
sf::Vector3f PlayPhysicsAI::arrive(Kinematic& vehicle, sf::Vector3f& target, float decel)
{
    sf::Vector3f toTarget = target - vehicle.pos;
    float d = vecutils::length(toTarget);
    if(d > 0)
    {
        // Calculate the speed required to reach the target given the desired
        // deceleration
        float speed = d / decel;

        // Make sure the velocity does not exceed the max
        speed = min(speed, vehicle.maxSpeed);

        // From here proceed just like chase, except we don't need to normalize
        // the toTarget vector because we have already gone to the trouble
        // of calculating its length: d
        sf::Vector3f desiredVel = toTarget * speed / d;
        return desiredVel - vehicle.vel;
    }
    return sf::Vector3f(0,0,0);
}

// Steering Behavior: flee from target
sf::Vector3f PlayPhysicsAI::flee(Kinematic& vehicle, sf::Vector3f& target, float panicDistance)
{
    float panicDistance2 = panicDistance * panicDistance;
    if(vecutils::distanceSquared(enemyK.pos, target) > panicDistance2)
        return sf::Vector3f(0,0,0);
    sf::Vector3f desiredVel = vehicle.pos - target;
    vecutils::normalize(desiredVel);
    desiredVel *= vehicle.maxSpeed;
    return desiredVel - vehicle.vel;
}

// Steering Behavior: pursuit target
sf::Vector3f PlayPhysicsAI::pursuit(Kinematic& vehicle, Kinematic& target)
{
    sf::Vector3f toEvader = target.pos - vehicle.pos;
    double relativeHeading = vecutils::dotProduct(vehicle.heading, target.heading);
    // If target is facing us, go chase it
    if(vecutils::dotProduct(toEvader, vehicle.heading) > 0 && relativeHeading < -0.95) // acos(0.95) = 18 graus
        return arrive(vehicle, target.pos);

    // Not facing, so let's predict where the target will be

    // The look-ahead time is proportional to the distance between the target
    // and the enemy, and is inversely proportional to the sum of the
    // agents' velocities

    float vel = vecutils::length(target.vel);
    float lookAheadTime = vecutils::length(toEvader) / (vehicle.maxSpeed + vel);

    // Now chase to the predicted future position of the target

    sf::Vector3f predicted(target.pos + target.vel * lookAheadTime);
    return arrive(vehicle, predicted, 1);
}

// Steering Behavior: evade target
sf::Vector3f PlayPhysicsAI::evade(Kinematic& vehicle, Kinematic& target)
{
    sf::Vector3f toPursuer = target.pos - vehicle.pos;

    // The look-ahead time is proportional to the distance between the pursuer
    // and the vehicle, and is inversely proportional to the sum of the
    // agents' velocities

    float vel = vecutils::length(target.vel);
    float lookAheadTime = vecutils::length(toPursuer) / (vehicle.maxSpeed + vel);

    // Now chase to the predicted future position of the target

    sf::Vector3f predicted(target.pos + target.vel * lookAheadTime);
    return flee(vehicle, predicted);
}

void PlayPhysicsAI::update(CGame* game)
{
    // First time in update, set initial camera pos
    if(firstTime) {
        game->setYpan(6*32);
        game->setXpan(-10*32);
        game->updateCamera();
        firstTime = false;
    }

    // Player motion ?
    if(zvel != 0) {
        game->setZoom(game->getZoom()+zvel);
        game->updateCamera();
    }

    phys->step();
    playerK.pos.x = player->getX();
    playerK.pos.y = player->getY();

    enemyK.pos.x = enemy->getX();
    enemyK.pos.y = enemy->getY();
    centerPlayerOnMap(game);

#define STEERING
#ifdef STEERING
    // Apply steering behavior(s)
    sf::Vector3f steeringForce;

    switch(steerMode) {
        case CHASE_BEHAVIOR:
            steeringForce = chase(enemyK, playerK.pos);
            break;
        case ARRIVE_BEHAVIOR:
            steeringForce = arrive(enemyK, playerK.pos, 0.3); // 0.3 - lento ... 1 - rápido
            break;
        case PURSUIT_BEHAVIOR:
            steeringForce = pursuit(enemyK, playerK);
            break;
        case FLEE_BEHAVIOR:
            steeringForce = flee(enemyK, playerK.pos, 100);
            break;
        case EVADE_BEHAVIOR:
            steeringForce = evade(enemyK, playerK);
    }
    sf::Vector3f accel = steeringForce/enemyPhys->GetMass();

    b2Vec2 thrust = b2Vec2(accel.x*80,accel.y*80);

    enemyPhys->ApplyLinearImpulse(thrust,enemyPhys->GetWorldCenter());
    enemyK.vel.x += thrust.x;
    enemyK.vel.y += thrust.y;

    // Can't exceed max speed
    if(vecutils::lengthSquared(enemyK.vel) > enemyK.maxSpeed*enemyK.maxSpeed) {
        vecutils::normalize(enemyK.vel);
        enemyK.vel *= enemyK.maxSpeed;
    }

    // Only update heading if speed is above minimum threshold
    if(vecutils::lengthSquared(enemyK.vel) > 0.00000001) {
        enemyK.heading = enemyK.vel / vecutils::length(enemyK.vel);
    }
#else
    // Basic chase

    enemyK.vel.set(0,0,0);
    if(enemyK.pos.X < playerK.pos.X)
        enemyK.vel.X = 2;
    else if(enemyK.pos.X > playerK.pos.X)
        enemyK.vel.X = -2;
    if(enemyK.pos.Y < playerK.pos.Y)
        enemyK.vel.Y = 2;
    else if(enemyK.pos.Y > playerK.pos.Y)
        enemyK.vel.Y = -2;
    enemyPhys->ApplyLinearImpulse(b2Vec2(enemyK.vel.X, enemyK.vel.Y), enemyPhys->GetWorldCenter());
#endif

    if(trail.size()>30)
        trail.pop_back();
    trail.push_front(enemyK.pos);
}

// Collision detection and centering based on code from
// http://www.parallelrealities.co.uk/tutorials/intermediate/tutorial14.php

void PlayPhysicsAI::centerPlayerOnMap(CGame* game)
{
    float maxMapX = map.getNumMapColumns() * map.getTileWidth();
    float maxMapY = map.getNumMapRows() * map.getTileHeight();

    float gameWidth  = game->getWidth();
    float gameHeight = game->getHeight();

    float panX = playerK.pos.x - (gameWidth/2);

    if(panX < 0)
        panX = 0;

    else if(panX + gameWidth >= maxMapX)
        panX = maxMapX - gameWidth;

    float panY = playerK.pos.y - (gameHeight/2);

    if(panY < 0)
        panY = 0;

    else if(panY + gameHeight >= maxMapY)
        panY = maxMapY - gameHeight;

    game->setXpan(panX);
    game->setYpan(panY);
    game->updateCamera();
}

void PlayPhysicsAI::checkCollision(CGame* game, Kinematic& obj)
{
    int i, x1, x2, y1, y2;

    // Get the limits of the map
    int maxMapX = map.getNumMapColumns();
    int maxMapY = map.getNumMapRows();

    // Get the width and height of a single tile
    int tileW = map.getTileWidth();
    int tileH = map.getTileHeight();

    // Get the height and width of the object (in this case, 100% of a tile)
    int playerH = tileH;
    int playerW = tileW;

    // Test the horizontal movement first
    i = playerH > tileH ? tileH : playerH;

    for (;;)
    {
        x1 = (obj.pos.x + obj.vel.x) / tileW;
        x2 = (obj.pos.x + obj.vel.x + playerW - 1) / tileW;

        y1 = (obj.pos.y) / tileH;
        y2 = (obj.pos.y + i - 1) / tileH;

        if (x1 >= 0 && x2 < maxMapX && y1 >= 0 && y2 < maxMapY)
        {
            if (obj.vel.x > 0)
            {
                // Trying to move right

                int upRight   = map.getCell(x2,y1);
                int downRight = map.getCell(x2,y2);
                if (blocks[upRight] == 1 || blocks[downRight] == 1)
                {
                    // Place the player as close to the solid tile as possible
                    obj.pos.x = x2 * tileW;
                    obj.pos.x -= playerW + 1;
                    obj.vel.x = 0;
                }
            }

            else if (obj.vel.x < 0)
            {
                // Trying to move left

                int upLeft   = map.getCell(x1,y1);
                int downLeft = map.getCell(x1,y2);
                if (blocks[upLeft] == 1 || blocks[downLeft] == 1)
                {
                    // Place the player as close to the solid tile as possible
                    obj.pos.x = (x1+1) * tileW;
                    obj.vel.x = 0;
                }
            }
        }

        if (i == playerH) // Checked player height with all tiles ?
        {
            break;
        }

        i += tileH; // done, check next tile upwards

        if (i > playerH)
        {
            i = playerH;
        }
    }

    // Now test the vertical movement

    i = playerW > tileW ? tileW : playerW;

    for (;;)
    {
        x1 = (obj.pos.x / tileW);
        x2 = ((obj.pos.x + i) / tileW);

        y1 = ((obj.pos.y + obj.vel.y) / tileH);
        y2 = ((obj.pos.y + obj.vel.y + playerH) / tileH);

        if (x1 >= 0 && x2 < maxMapX && y1 >= 0 && y2 < maxMapY)
        {
            if (obj.vel.y > 0)
            {
                // Trying to move down
                int downLeft  = map.getCell(x1,y2);
                int downRight = map.getCell(x2,y2);
                if (blocks[downLeft] == 1 || blocks[downRight] == 1)
                {
                    // Place the player as close to the solid tile as possible
                    obj.pos.y = y2 * tileH;
                    obj.pos.y -= playerH;
                    obj.vel.y = 0;
                }
            }

            else if (obj.vel.y < 0)
            {
                // Trying to move up

                int upLeft  = map.getCell(x1,y1);
                int upRight = map.getCell(x2,y1);
                if (blocks[upLeft] == 1 || blocks[upRight] == 1)
                {
                    // Place the player as close to the solid tile as possible
                    obj.pos.y = (y1 + 1) * tileH;
                    obj.vel.y = 0;
                }
            }
        }

        if (i == playerW)
        {
            break;
        }

        i += tileW; // done, check next tile to the right

        if (i > playerW)
        {
            i = playerW;
        }
    }

    // Now apply the movement

    obj.pos += obj.vel;

    if (obj.pos.x < 0)
        obj.pos.x = 0;
    else if (obj.pos.x + playerW >= maxMapX * tileW)
        obj.pos.x = maxMapX*tileW - playerW - 1;

    if(obj.pos.y < 0)
        obj.pos.y = 0;
    else if(obj.pos.y + playerH >= maxMapY * tileH)
        obj.pos.y = maxMapY*tileH - playerH - 1;
}


void PlayPhysicsAI::draw(CGame* game)
{
    glClearColor(0.8,0.8,0.8,1); // light gray
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    map.draw();

    //player->setPosition(playerK.pos.X,playerK.pos.Y);
    player->draw();

    if(showTrails)
    {
        list<sf::Vector3f>::iterator it = trail.begin();
        while(it != trail.end())
        {
            glColor4f(1,1,1,0.1);
            enemy->setPosition(it->x, it->y);
            enemy->draw();
            it++;
        }
        glColor4f(1,1,1,1);
    }

    enemy->setPosition(enemyK.pos.x, enemyK.pos.y);
    enemy->draw();

    font->draw(enemyK.pos.x, enemyK.pos.y-14, (char *)modes2[steerMode].c_str());

//    phys->debugDraw();

    SDL_GL_SwapBuffers();
}
