/*
 *  MenuState.cpp
 *  Example "menu" state
 *
 *  Created by Marcelo Cohen on 04/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */


#include <iostream>
#include <SDL.h>
#include "Graphics.h"
#include "CGame.h"
#include "MenuState.h"
#include "PlayState.h"
#include "PlayMap.h"
#include "PlayMapTop.h"
//#include "PlayMapAI.h"
//#include "PlayPhysics.h"
//#include "PlayMapPhysics.h"

MenuState MenuState::m_MenuState;

using namespace std;

void MenuState::init()
{
	menuSprite = new CImage();
	menuSprite->loadImage("data/img/menu.png"); // load menu state bitmap
	cout << "MenuState Init Successful" << endl;
}

void MenuState::cleanup()
{
    delete menuSprite;
	cout << "MenuState Cleanup Successful" << endl;
}

void MenuState::pause()
{
	cout << "MenuState Paused" << endl;
}

void MenuState::resume()
{
	cout << "MenuState Resumed" << endl;
}

void MenuState::handleEvents(CGame* game)
{
	SDL_Event event;

    if (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				game->quit();
				break;

			case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {

                    case SDLK_SPACE:
                        //game->changeState(PlayState::instance());
                        game->changeState(PlayMap::instance());
                        //game->changeState(PlayMapTop::instance());
                        //game->changeState(PlayMapAI::instance());
                        //game->changeState(PlayPhysics::instance());
                        //game->changeState(PlayMapPhysics::instance());
                        break;
                    case SDLK_ESCAPE:
                        game->quit();
                        break;
                    default:
                        break;
                }
		}
	}
}

void MenuState::update(CGame* game)
{
}

void MenuState::draw(CGame* game)
{
    glClearColor(0.6,0.6,0.6,1); // gray
    glClear(GL_COLOR_BUFFER_BIT);
    menuSprite->setPosition(50,50);
    menuSprite->draw();
    SDL_GL_SwapBuffers();
}

