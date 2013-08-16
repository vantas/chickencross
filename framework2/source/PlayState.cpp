/*
 *  PlayState.cpp
 *  Normal "play" state
 *
 *  Created by Marcelo Cohen on 08/13.
 *  Copyright 2013 PUCRS. All rights reserved.
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
	//playSprite->loadSprite("player.png", 36, 44, 0, 0, 0, 0, 7, 1, 7);
	//playSprite->loadSprite("char2.png", 128,128,0,0,0,53,4,2,7);
	//playSprite->loadSprite("char4.png",128,128,0,0,0,21,4,3,10);
//	playSprite1->loadSprite("data/img/char9.png",128,128,0,0,0,40,4,2,6);

    map = new tmx::MapLoader("data/maps");
    map->Load("dungeon.tmx");

    playSprite1.loadImage("data/img/Char14.png");
	playSprite1.setPosition(10,100);

//	playSprite1->setAnimRate(30);        // quadros/segundo
//	playSprite1->setXspeed(200);         // pixels/segundo
//    playSprite2->loadSprite("data/img/char9.png",128,128,0,0,0,40,4,2,6);

    playSprite2.loadImage("data/img/Char01.png");
	playSprite2.setPosition(10,300);

	playSprite3.loadImage("data/img/Char01.png");
	playSprite3.setPosition(50,300);

    //player.loadSprite("data/img/smurf_sprite.png", 128, 128, 0, 0, 0, 0, 7, 3, 16);
    player.loadSpriteSparrowXML("data/img/smurf_sprite.xml");
    player.setPosition(30,30);
    player.setAnimRate(15);
    player.setXspeed(100);
//    player.setScale(1);

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
    im->addKeyInput("stats", sf::Keyboard::S);
    im->addKeyInput("zoomin", sf::Keyboard::Z);
    im->addKeyInput("zoomout", sf::Keyboard::X);
    im->addMouseInput("rightclick", sf::Mouse::Right);

	cout << "PlayState Init Successful" << endl;
}

void PlayState::cleanup()
{
	cout << "PlayState Clean Successful" << endl;
	delete map;
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
    sf::View view = screen->getView();

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

    if(im->testEvent("stats"))
        game->enableStats();

    if(im->testEvent("zoomin"))
    {
        view.zoom(1.01);
        screen->setView(view);
    }
    else if(im->testEvent("zoomout"))
    {
        view.zoom(0.99);
        screen->setView(view);
    }

    //game->changeState(PlayMap::instance());
    //game->changeState(PlayMapTop::instance());
    //game->changeState(PlayMapAI::instance());
    //game->changeState(PlayPhysics::instance());
    //game->changeState(PlayMapPhysics::instance());
}

void PlayState::update(CGame* game)
{
    screen = game->getScreen();
    float x = playSprite1.getPosition().x;
    float y = playSprite1.getPosition().y;
    x += dirx*5;
    y += diry*5;
    playSprite1.setPosition(x,y);
//    playSprite1->update(game->getUpdateInterval());
//    playSprite1->update(game->getUpdateInterval());
//    playSprite2->update(game->getUpdateInterval());

//    playSprite1->setXspeed(200*dir);
//    playSprite1->setMirror(dir==1 ? false : true);

    //player.rotate(1);
    //player.setOrigin(64,64);
    //player.setXspeed(0);
    player.update(game->getUpdateInterval());

    auto layers = map->GetLayers();
    tmx::MapLayer layer1 = layers[0];

    if(player.getPosition().x > 600)
    {
        player.setXspeed(-100);
        player.setMirror(true);
    }
    if(player.getPosition().x < 50)
    {
        player.setXspeed(100);
        player.setMirror(false);
    }

}

void PlayState::draw(CGame* game)
{
    //sf::View view = screen->getView();

    screen->clear(sf::Color(0,0,0));
    map->Draw(*screen);

    playSprite1.setRotation(0);
    playSprite1.setScale(1,1);
    screen->draw(playSprite1);
    playSprite2.setRotation(0);
    playSprite2.setScale(1,1);

    screen->draw(playSprite2);
    screen->draw(playSprite3);

    screen->draw(player);
}
