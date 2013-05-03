/*
 *  PlayMapTop.cpp
 *  Example gameplay: playing from a top view
 *
 *  Created by Marcelo Cohen on 09/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#include <iostream>
#include <SDL.h>
#include "Graphics.h"
#include <cmath>
#include "CGame.h"
#include "PauseState.h"
#include "PlayMapTop.h"

PlayMapTop PlayMapTop::m_PlayMapTop;

using namespace std;

void PlayMapTop::init()
{
    map.loadMap("data/maps/dungeon2layers.tmx");
    speed = 8;   // speed to use

    keystate = SDL_GetKeyState(NULL);

    player = new CSprite();
    player->loadSprite("data/img/Char19s.png", 32, 32, 0, 0, 0, 0, 1, 1, 1);
    float px, py;
    map.getCenter(40,5,px,py);
    player->setPosition(px,py);

    firstTime = true; // to set map position at first update

    cout << "PlayMapTop Init Successful" << endl;
}

void PlayMapTop::cleanup()
{
    delete player;
    cout << "PlayMapTop Clean Successful" << endl;
}

void PlayMapTop::pause()
{
    cout << "PlayMapTop Paused" << endl;
}

void PlayMapTop::resume()
{
    cout << "PlayMapTop Resumed" << endl;
}

void PlayMapTop::handleEvents(CGame* game)
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
    float vx, vy;
    vx = -keystate[SDLK_LEFT]*speed + keystate[SDLK_RIGHT]*speed;
    vy = -keystate[SDLK_UP]*speed + keystate[SDLK_DOWN]*speed;

    player->setXspeed(vx);
    player->setYspeed(vy);
}

void PlayMapTop::update(CGame* game)
{
    // First time in update, set initial camera pos
    if(firstTime) {
        game->setYpan(6*32);
        game->setXpan(-10*32);
        game->updateCamera();
        firstTime = false;
    }

    checkCollision(game, player);
    player->update(game->getUpdateInterval());
    centerPlayerOnMap(game);
}

// Collision detection and centering based on code from
// http://www.parallelrealities.co.uk/tutorials/intermediate/tutorial14.php

void PlayMapTop::centerPlayerOnMap(CGame* game)
{
    float maxMapX = map.getNumMapColumns() * map.getTileWidth();
    float maxMapY = map.getNumMapRows() * map.getTileHeight();

    float gameWidth  = game->getWidth();
    float gameHeight = game->getHeight();

    float px = player->getX();
    float py = player->getY();
    float panX = px - (gameWidth/2);

    if(panX < 0)
        panX = 0;

    else if(panX + gameWidth >= maxMapX)
        panX = maxMapX - gameWidth;

    float panY = py - (gameHeight/2);

    if(panY < 0)
        panY = 0;

    else if(panY + gameHeight >= maxMapY)
        panY = maxMapY - gameHeight;

    game->setXpan(panX);
    game->setYpan(panY);
    game->updateCamera();
}

void PlayMapTop::checkCollision(CGame* game, CSprite* obj)
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

    float px = obj->getX();
    float py = obj->getY();

    float vx = obj->getXspeed();
    float vy = obj->getYspeed();

    // Test the horizontal movement first
    i = playerH > tileH ? tileH : playerH;

    for (;;)
    {
        x1 = (px + vx) / tileW;
        x2 = (px + vx + playerW - 1) / tileW;

        y1 = (py) / tileH;
        y2 = (py + i - 1) / tileH;

        if (x1 >= 0 && x2 < maxMapX && y1 >= 0 && y2 < maxMapY)
        {
            if (vx > 0)
            {
                // Trying to move right

                int upRight   = map.getCell(x2,y1,1);
                int downRight = map.getCell(x2,y2,1);
                if (upRight || downRight)
                {
                    // Place the player as close to the solid tile as possible
                    px = x2 * tileW;
                    px -= playerW;// + 1;
                    vx = 0;
                }
            }

            else if (vx < 0)
            {
                // Trying to move left

                int upLeft   = map.getCell(x1,y1,1);
                int downLeft = map.getCell(x1,y2,1);
                if (upLeft || downLeft)
                {
                    // Place the player as close to the solid tile as possible
                    px = (x1+1) * tileW;
                    vx = 0;
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
        x1 = (px / tileW);
        x2 = ((px + i-1) / tileW);

        y1 = ((py + vy) / tileH);
        y2 = ((py + vy + playerH-1) / tileH);

        if (x1 >= 0 && x2 < maxMapX && y1 >= 0 && y2 < maxMapY)
        {
            if (vy > 0)
            {
                // Trying to move down
                int downLeft  = map.getCell(x1,y2,1);
                int downRight = map.getCell(x2,y2,1);
                cout << "downleft, downright: " << downLeft << " " << downRight << endl;
                if (downLeft || downRight)
                {
                    // Place the player as close to the solid tile as possible
                    py = y2 * tileH;
                    py -= playerH;
                    vy = 0;
                }
            }

            else if (vy < 0)
            {
                // Trying to move up

                int upLeft  = map.getCell(x1,y1,1);
                int upRight = map.getCell(x2,y1,1);
                if (upLeft || upRight)
                {
                    // Place the player as close to the solid tile as possible
                    py = (y1 + 1) * tileH;
                    vy = 0;
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

    obj->setPosition(px+vx,py+vy);
    obj->setXspeed(0);
    obj->setYspeed(0);
    px = obj->getX();
    py = obj->getY();

    // Check collision with edges of map
    if (px < 0)
        obj->setX(0);
    else if (px + playerW >= maxMapX * tileW)
        obj->setX(maxMapX*tileW - playerW - 1);

    if(py < 0)
        obj->setY(0);
    else if(py + playerH >= maxMapY * tileH)
        obj->setY(maxMapY*tileH - playerH - 1);
}

void PlayMapTop::draw(CGame* game)
{
    glClearColor(0.8,0.8,0.8,1); // light gray
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    map.draw();
    player->draw();

    SDL_GL_SwapBuffers();
}
