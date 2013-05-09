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
#include "InputManager.h"

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
    dirx = 0; // direção do sprite: para a direita (1), esquerda (-1)
    diry = 0;

    im = InputManager::instance();

    im->addKeyInput("left", sf::Keyboard::Left);
    im->addKeyInput("right", sf::Keyboard::Right);
    im->addKeyInput("up", sf::Keyboard::Up);
    im->addKeyInput("down", sf::Keyboard::Down);
    im->addKeyInput("quit", sf::Keyboard::Escape);
    im->addMouseInput("rightclick", sf::Mouse::Right);

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
        if(event.type == sf::Event::Closed)
            game->quit();
    }

    dirx = diry = 0;

    if(im->testEvent("left"))
        dirx = -1;

    if(im->testEvent("right"))
        dirx = 1;

    if(im->testEvent("up"))
        diry = -1;

    if(im->testEvent("down"))
        diry = 1;

    if(im->testEvent("quit") || im->testEvent("rightclick"))
        game->quit();

    //game->changeState(PlayMap::instance());
    //game->changeState(PlayMapTop::instance());
    //game->changeState(PlayMapAI::instance());
    //game->changeState(PlayPhysics::instance());
    //game->changeState(PlayMapPhysics::instance());
}

void PlayState::update(CGame* game)
{
    float x = playSprite1->getX();
    float y = playSprite1->getY();
    x += dirx*5;
    y += diry*5;
    playSprite1->setX(x);
    playSprite1->setY(y);
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
