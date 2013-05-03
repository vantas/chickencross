/*
 *  PlayMap.cpp
 *  Normal "play" state - tiled map viewing
 *
 *  Created by Marcelo Cohen on 04/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#include <iostream>
#include <SDL.h>
#include "Graphics.h"
#include <cmath>
#include "CGame.h"
#include "PlayMap.h"
#include "PauseState.h"

PlayMap PlayMap::m_PlayMap;

using namespace std;

const float PlayMap::G = 1.00;            // gravity
const float PlayMap::MAX_FALL_SPEED = 40; // maximum falling velocity

void PlayMap::init()
{
    map.loadMap("data/maps/misteryforest.tmx");
    xvel = yvel = 0;     // current player speed
    cameraSpeed = 8;    // base speed to use
    playerAnimRate = 34; // player animation rate when moving
    zvel = 0;
    keystate = SDL_GetKeyState(NULL); // get array of key states

    firstTime = true; // to set map position at first update
    onGround = true; // player starts on the ground

    player = new CSprite();
    player->loadSpriteSparrowXML("data/img/smurfwalk.xml");
//player->loadSprite("data/img/smurf_sprite.png", 128, 128, 0, 0, 0, 0, 4, 4, 16);
    player->setAnimRate(15);
    player->setScale(1);
    cout << "total frames " << player->getTotalFrames() << endl;


    // Get corner of cell (1,11) of the map in world coords
    map.getCenter(1,11,playerX,playerY);

    // Example of NPC: another Smurf...
    npc = new CSprite();
    npc->loadSprite("data/img/smurf_sprite.png", 128, 128, 0, 0, 0, 0, 4, 4, 16);
    npc->setAnimRate(0);
    float npcX, npcY;

    // NPC will be at the top platform
    map.getCenter(2,3,npcX,npcY);
    npc->setPosition(npcX, npcY);

	cout << "PlayMap Init Successful" << endl;
}

void PlayMap::cleanup()
{
    delete player;
    delete npc;
	cout << "PlayMap Clean Successful" << endl;
}

void PlayMap::pause()
{
	cout << "PlayMap Paused" << endl;
}

void PlayMap::resume()
{
	cout << "PlayMap Resumed" << endl;
}

void PlayMap::handleEvents(CGame* game)
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
                    case SDLK_ESCAPE:
                        game->quit();
                        break;
                    case SDLK_SPACE:
                        if(onGround) {
                            // jump only if on ground
                            yvel = -25;
//                            game->getAudioEngine()->play2D(jumpSoundSource);
                        }
                        break;
                    case SDLK_w:
                        if(onGround) {
                            yvel = -40; // super jump
//                            game->getAudioEngine()->play2D(superJumpSoundSource);
                        }
                        break;
                    case SDLK_a:
                        playerAnimRate++;
                        cout << "A: " << playerAnimRate << endl;
                        break;
                    case SDLK_s:
                        playerAnimRate--;
                        cout << "A: " << playerAnimRate << endl;
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
    xvel = -keystate[SDLK_LEFT]*cameraSpeed + keystate[SDLK_RIGHT]*cameraSpeed;
    zvel = -5*keystate[SDLK_LSHIFT] + 5*keystate[SDLK_RSHIFT];

    if(xvel) {
        // Starting to walk, check if we have the sound object
//        if(!walkSound) {
//           walkSound = game->getAudioEngine()->play2D(walkSoundSource, true, false, true);
//        }
//    } else if(walkSound) {
//        // Stopped moving, stop and drop sound
//        walkSound->stop();
//        walkSound->drop();
//        walkSound = NULL;
    }

}

void PlayMap::update(CGame* game)
{
    // First time in update, set initial camera pos
    if(firstTime) {
        game->setYpan(9*128);
        game->setXpan(-2*128);
        game->updateCamera();
        firstTime = false;
        // "Startup" sound
        //game->getAudioEngine()->play2D("data/audio/looperman_159051_30989_Effect - Fairytales.wav");
        // Load sounds
//        smurf3DSoundSource = game->getAudioEngine()->addSoundSourceFromFile("data/audio/laugh.wav");
//        walkSoundSource = game->getAudioEngine()->addSoundSourceFromFile("data/audio/walk.wav");
//        jumpSoundSource = game->getAudioEngine()->addSoundSourceFromFile("data/audio/jump.wav");
//        superJumpSoundSource = game->getAudioEngine()->addSoundSourceFromFile("data/audio/superjump.wav");
//        walkSound = NULL;
//        irrklang::vec3df pos(npc->getX(), npc->getY(), 0);
//        smurf3DSoundSource->setDefaultMinDistance(300);
//        smurf3DSoundSource->setDefaultMaxDistance(3000);
//        game->getAudioEngine()->play3D(smurf3DSoundSource, pos, true);
    }

    // Player motion ?
    if(zvel != 0) {
        game->setZoom(game->getZoom()+zvel);
        game->updateCamera();
    }

    // Gravity always pulls the player down
    yvel += G;

    if (yvel >= MAX_FALL_SPEED)
        yvel = MAX_FALL_SPEED;

    checkCollision(game);
    centerPlayerOnMap(game);

    // Simple collision test with other sprite
    if(player->bboxCollision(npc)) {
//        game->getAudioEngine()->play2D(jumpSoundSource);
        cout << "bump!" << endl;
    }
}

// Collision detection and centering based on code from
// http://www.parallelrealities.co.uk/tutorials/intermediate/tutorial14.php

void PlayMap::centerPlayerOnMap(CGame* game)
{
    float maxMapX = map.getNumMapColumns() * map.getTileWidth();
    float maxMapY = map.getNumMapRows() * map.getTileHeight();

    float gameWidth  = game->getWidth();
    float gameHeight = game->getHeight();

    float panX = playerX - (gameWidth/2);

    if(panX < 0)
        panX = 0;

    else if(panX + gameWidth >= maxMapX)
        panX = maxMapX - gameWidth;

    float panY = playerY - (gameHeight/2);

    if(panY < 0)
        panY = 0;

    else if(panY + gameHeight >= maxMapY)
        panY = maxMapY - gameHeight;

    game->setXpan(panX);
    game->setYpan(panY);
    game->updateCamera();
    // Update audio listener position
    irrklang::vec3df position(playerX,playerY,0);        // position of the listener
    irrklang::vec3df lookDirection(0,0,1); // the direction the listener looks into
    irrklang::vec3df velPerSecond(0,0,0);    // only relevant for doppler effects
    irrklang::vec3df upVector(0,1,0);        // where 'up' is in your 3D scene
    game->getAudioEngine()->setListenerPosition(position, lookDirection, velPerSecond, upVector);
}

void PlayMap::checkCollision(CGame* game)
{
    int i, x1, x2, y1, y2;

    // Remove the user from the ground
    onGround = false;

    // Get the limits of the map
    int maxMapX = map.getNumMapColumns();
    int maxMapY = map.getNumMapRows();

    // Get the width and height of a single tile
    int tileW = map.getTileWidth();
    int tileH = map.getTileHeight();

    // Get the height and width of the player (in this case, width is 80% of a tile)
    // (not perfect, computation below takes off only from right and bottom sides)
    int playerH = player->getHeight(); //* 0.9;
    int playerW = player->getWidth();  //* 0.6;

    // Test the horizontal movement first
    i = playerH > tileH ? tileH : playerH;

    for (;;)
    {
        // Note: width counted from the left side
        x1 = (playerX + xvel) / tileW;
        x2 = (playerX + xvel + playerW - 1) / tileW;

        y1 = (playerY) / tileH;
        y2 = (playerY + i - 1) / tileH;

        if (x1 >= 0 && x2 < maxMapX && y1 >= 0 && y2 < maxMapY)
        {
            if (xvel > 0)
            {
                // Trying to move right

                int upRight   = map.getCell(x2,y1);
                int downRight = map.getCell(x2,y2);
                if (upRight != -1 || downRight != -1)
                {
                    // Place the player as close to the solid tile as possible
                    playerX = x2 * tileW;
                    playerX -= playerW + 1;
                    xvel = 0;
                }
            }

            else if (xvel < 0)
            {
                // Trying to move left

                int upLeft   = map.getCell(x1,y1);
                int downLeft = map.getCell(x1 ,y2);
                if (upLeft != -1 || downLeft != -1)
                {
                    // Place the player as close to the solid tile as possible
                    playerX = (x1+1) * tileW;
                    xvel = 0;
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
        // Note: height counted from the left side
        x1 = (playerX / tileW);
        x2 = ((playerX + i) / tileW);

        y1 = ((playerY + yvel) / tileH);
        y2 = ((playerY + yvel + playerH) / tileH);

        if (x1 >= 0 && x2 < maxMapX && y1 >= 0 && y2 < maxMapY)
        {
            if (yvel > 0)
            {
                // Trying to move down
                int downLeft  = map.getCell(x1,y2);
                int downRight = map.getCell(x2,y2);
                if (downLeft != -1 || downRight != -1)
                {
                    // Place the player as close to the solid tile as possible
                    playerY = y2 * tileH;
                    playerY -= playerH;
                    yvel = 0;
                    onGround = true;
                }
            }

            else if (yvel < 0)
            {
                // Trying to move up

                int upLeft  = map.getCell(x1,y1);
                int upRight = map.getCell(x2,y1);
                if (upLeft != -1 || upRight != -1)
                {
                    // Place the player as close to the solid tile as possible
                    playerY = (y1 + 1) * tileH;
                    yvel = 0;
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
    playerX += xvel;
    playerY += yvel;
    if(xvel > 0) {
        player->setMirror(false);
        player->setAnimRate(playerAnimRate);
    }
    else if(xvel < 0) {
        player->setMirror(true);
        player->setAnimRate(playerAnimRate);
    }
    else {
        player->setAnimRate(0);     // stopped
        player->setCurrentFrame(3); // standing still
    }

    player->update(game->getUpdateInterval());

    if (playerX < 0)
        playerX = 0;
    else if (playerX + playerW >= maxMapX * tileW)
        playerX = maxMapX - playerW - 1;
}

void PlayMap::draw(CGame* game)
{
    glClearColor(0.8,0.8,0.8,1); // light gray
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    map.draw();

    //CMultiImage* mapimg = map.getTilesetImage();
    //mapimg->setPosition(playerX,playerY);
    //mapimg->drawFrame(0);
    player->setPosition(playerX, playerY);
    player->draw();

    npc->draw();

//    playSprite1->setRotation(0);
//    playSprite1->setScale(1);
//    playSprite1->draw();
//    playSprite2->setRotation(0);
//    playSprite2->setScale(1);
//    playSprite2->draw();

    SDL_GL_SwapBuffers();
}
