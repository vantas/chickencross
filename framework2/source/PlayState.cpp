/*
 *  PlayState.cpp
 *  Normal "play" state
 *
 *  Created by Marcelo Cohen on 04/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#include <iostream>
#include <cmath>
#include "CGame.h"
#include "PlayState.h"
#include "PauseState.h"

PlayState PlayState::m_PlayState;

using namespace std;

void PlayState::init()
{
	playSprite1 = new CImage();
    playSprite2 = new CImage();
	//playSprite->loadSprite("player.png", 36, 44, 0, 0, 0, 0, 7, 1, 7);
	//playSprite->loadSprite("char2.png", 128,128,0,0,0,53,4,2,7);
	//playSprite->loadSprite("char4.png",128,128,0,0,0,21,4,3,10);
//	playSprite1->loadSprite("data/img/char9.png",128,128,0,0,0,40,4,2,6);
    playSprite1->loadImage("data/img/Char14.png");
	playSprite1->setPosition(10,100);
//	playSprite1->setAnimRate(30);        // quadros/segundo
//	playSprite1->setXspeed(200);         // pixels/segundo
//    playSprite2->loadSprite("data/img/char9.png",128,128,0,0,0,40,4,2,6);
    playSprite2->loadImage("data/img/Char01.png");
	playSprite2->setPosition(10,300);
//	playSprite2->setAnimRate(10);        // quadros/segundo
//	playSprite2->setXspeed(30);         // pixels/segundo
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
    sf::Event event;
    sf::RenderWindow* screen = game->getScreen();

    while (screen->pollEvent(event))
    {
        // check the type of the event...
        switch (event.type)
        {
            // window closed
        case sf::Event::Closed:
            game->quit();
            break;

            // key pressed
        case sf::Event::KeyPressed:
            if(event.key.code == sf::Keyboard::Escape)
                game->quit();
            //if(event.key.code == sf::Keyboard::P)
            //    game->pushState(PauseState::instance());
            if(event.key.code == sf::Keyboard::Left)
                dir = 1;
            if(event.key.code == sf::Keyboard::Right)
                dir = -1;
             //game->changeState(PlayMap::instance());
            //game->changeState(PlayMapTop::instance());
            //game->changeState(PlayMapAI::instance());
            //game->changeState(PlayPhysics::instance());
            //game->changeState(PlayMapPhysics::instance());
            break;

            // we don't process other types of events
        default:
            break;
        }
    }
}

void PlayState::update(CGame* game)
{
    float x = playSprite1->getX();
    x += dir;
    playSprite1->setX(x);
//    playSprite1->update(game->getUpdateInterval());
//    playSprite1->update(game->getUpdateInterval());
//    playSprite2->update(game->getUpdateInterval());

//    playSprite1->setXspeed(200*dir);
//    playSprite1->setMirror(dir==1 ? false : true);
}

void PlayState::draw(CGame* game)
{
    sf::RenderWindow* screen = game->getScreen();

    screen->clear(sf::Color(0,0,0));

    playSprite1->setRotation(0);
    playSprite1->setScale(1);
    playSprite1->draw(screen);
    playSprite2->setRotation(0);
    playSprite2->setScale(1);
    playSprite2->draw(screen);
}
