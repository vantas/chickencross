/*
 *  PlayState.cpp
 *  Normal "play" state
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
#include "PlayState.h"
#include "PauseState.h"

PlayState PlayState::m_PlayState;

using namespace std;

void PlayState::init()
{
	playSprite1 = new CSprite();
    playSprite2 = new CSprite();
	//playSprite->loadSprite("player.png", 36, 44, 0, 0, 0, 0, 7, 1, 7);
	//playSprite->loadSprite("char2.png", 128,128,0,0,0,53,4,2,7);
	//playSprite->loadSprite("char4.png",128,128,0,0,0,21,4,3,10);
	playSprite1->loadSprite("data/img/char9.png",128,128,0,0,0,40,4,2,6);
	playSprite1->setPosition(10,100);
	playSprite1->setAnimRate(30);        // quadros/segundo
	playSprite1->setXspeed(200);         // pixels/segundo
    playSprite2->loadSprite("data/img/char9.png",128,128,0,0,0,40,4,2,6);
	playSprite2->setPosition(10,300);
	playSprite2->setAnimRate(10);        // quadros/segundo
	playSprite2->setXspeed(30);         // pixels/segundo
    dir = 1; // direção do sprite: para a direita (1), esquerda (-1)
	cout << "PlayState Init Successful" << endl;
}

void PlayState::cleanup()
{
    delete playSprite1;
    delete playSprite2;
	cout << "PlayState Clean Successful" << endl;
}

void PlayState::pause()
{
	cout << "PlayState Paused" << endl;
}

void PlayState::resume()
{
	cout << "PlayState Resumed" << endl;
}

void PlayState::handleEvents(CGame* game)
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
                    case SDLK_RIGHT:
                        dir = 1;
                        break;
                    case SDLK_LEFT:
                        dir = -1;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_VIDEORESIZE:
                game->resize(event.resize.w, event.resize.h);
		}
	}
}

void PlayState::update(CGame* game)
{
    playSprite1->update(game->getUpdateInterval());
    playSprite2->update(game->getUpdateInterval());

    playSprite1->setXspeed(200*dir);
    playSprite1->setMirror(dir==1 ? false : true);
}

void PlayState::draw(CGame* game)
{
    glClearColor(0.8,0.8,0.8,1); // light gray
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    playSprite1->setRotation(0);
    playSprite1->setScale(1);
    playSprite1->draw();
    playSprite2->setRotation(0);
    playSprite2->setScale(1);
    playSprite2->draw();

    SDL_GL_SwapBuffers();
}
