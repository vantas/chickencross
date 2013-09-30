/*
 *  PlayPhysics.cpp
 *  Testbed for Box2D (physics) experiments
 *
 *  Created by Marcelo Cohen on 09/13.
 *  Copyright 2013 PUCRS. All rights reserved.
 *
 */

#include <iostream>
#include <cmath>
#include "Game.h"
#include "PlayPhysics.h"
#include "PauseState.h"
#include "InputManager.h"

PlayPhysics PlayPhysics::m_PlayPhysics;

using namespace std;

void PlayPhysics::init()
{
    player.loadXML("data/img/hunter.xml");
    player.setPosition(50,100);
    player.loadAnimation("data/img/hunteranim.xml");
    player.setAnimRate(15);
    //playSprite->loadSprite("player.png", 36, 44, 0, 0, 0, 0, 7, 1, 7);
    //playSprite->loadSprite("char2.png", 128,128,0,0,0,53,4,2,7);
    //playSprite->loadSprite("char4.png",128,128,0,0,0,21,4,3,10);
    //playSprite1->loadSprite("data/img/char9.png",128,128,0,0,0,40,4,2,6);

    map = new tmx::MapLoader("data/maps");
    map->Load("dungeon.tmx");

    ghost.load("data/img/Char14.png");
    ghost.setPosition(100,300);
    ghost.setScale(sf::Vector2f(2,2));
    ghost.setXspeed(100);
//    playSprite1.load("data/img/Char14.png");
//    playSprite1.setPosition(80,100);
//    playSprite1.setScale(sf::Vector2f(6,6));

//    playSprite1->setAnimRate(30);        // quadros/segundo
//    playSprite1->setXspeed(200);         // pixels/segundo
//    playSprite2->loadSprite("data/img/char9.png",128,128,0,0,0,40,4,2,6);

//  playSprite2.load("data/img/Char01.png");
//	playSprite2.setPosition(10,300);

//    playSprite3.load("data/img/Char01.png");
//	playSprite3.setPosition(50,300);

    //player.load("data/img/smurf_sprite.png", 128, 128, 0, 0, 0, 0, 7, 3, 16);
    //player.loadSpriteSparrowXML("data/img/smurf_sprite.xml");

    /*
    monkey.loadXML("data/img/monkey.xml");
    monkey.loadAnimation("data/img/monkeyanim.xml");
    monkey.setPosition(30,30);
    monkey.setAnimRate(15);
//    monkey.setFrameRange(12,13);
    monkey.setAnimation("walk-right");
    monkey.setXspeed(100);
    monkey.setRotation(0);
    monkey.play();
    */

    dirx = 0; // direção do sprite: para a direita (1), esquerda (-1)
    diry = 0;

    im = cgf::InputManager::instance();

    im->addKeyInput("left", sf::Keyboard::Left);
    im->addKeyInput("right", sf::Keyboard::Right);
    im->addKeyInput("up", sf::Keyboard::Up);
    im->addKeyInput("down", sf::Keyboard::Down);
    im->addKeyInput("quit", sf::Keyboard::Escape);
    im->addKeyInput("zoomin", sf::Keyboard::Z);
    im->addKeyInput("zoomout", sf::Keyboard::X);
    im->addMouseInput("rightclick", sf::Mouse::Right);

    if (!font.loadFromFile("data/fonts/arial.ttf"))
    {
        cout << "Cannot load arial.ttf font!" << endl;
        exit(1);
    }

    phys = cgf::Physics::instance();
    phys->setGravity(0);
    phys->setConvFactor(30);

    bplayer = phys->newBoxImage(0, &player, 50, 0.1, 0.1);
    bghost  = phys->newCircleImage(1, &ghost, 50, 0.1, 0.1);

    auto layers = map->GetLayers();
    tmx::MapLayer& layer = layers[1];
//    int moo = 0;
//    for(auto tile: layer.tiles)
//    {
//        cout << tile.gridCoord.x << "," << tile.gridCoord.y << " (" << tile.gid << ") ";
//        if(++moo>3) break;
//    }
//    cout << endl;
//    cout << layer.name << endl;
    for(auto object: layer.objects) //.begin(); object != layer.objects.end(); ++object)
    {
    //    cout << "object" << endl;
//        cout << object.GetShapeType() << endl;
        sf::FloatRect rect = object.GetAABB();
//        cout << "box: " << rect.left << "," << rect.top << " - " << rect.width << " x " << rect.height << endl;
        phys->newBox(-3, rect.left, rect.top, rect.width, rect.height, 1, 0, 0, true);
    }

    firstTime = true;
    cout << "PlayPhysics Init Successful" << endl;
}

void PlayPhysics::cleanup()
{
    cout << "PlayPhysics Clean Successful" << endl;
    delete map;
}

void PlayPhysics::pause()
{
    cout << "PlayPhysics Paused" << endl;
}

void PlayPhysics::resume()
{
    cout << "PlayPhysics Resumed" << endl;
}

void PlayPhysics::handleEvents(cgf::Game* game)
{
    sf::Event event;
    sf::View view = screen->getView();

    while (screen->pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            game->quit();
        if(event.type == sf::Event::KeyPressed)
            if(event.key.code == sf::Keyboard::S)
                game->toggleStats();
    }

    dirx = diry = 0;

    if(im->testEvent("left")) {
        if(player.getXspeed() >= 0) {
            player.setAnimation("walk-left");
            player.play();
        }
        dirx = -1;
    }
    else
    if(im->testEvent("right")) {
        if(player.getXspeed() <= 0) {
            player.setAnimation("walk-right");
            player.play();
        }
        dirx = 1;
    }

    if(im->testEvent("up")) {
        if(player.getYspeed() >= 0) {
            player.setAnimation("walk-up");
            player.play();
        }
        diry = -1;
    }

    if(im->testEvent("down")) {
        if(player.getYspeed() <= 0) {
            player.setAnimation("walk-down");
            player.play();
        }
        diry = 1;
    }

    if(!dirx && !diry) // parado?
    {
        player.setCurrentFrame(0);
        player.pause();
    }

    if(im->testEvent("quit") || im->testEvent("rightclick"))
        game->quit();

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

    //player.setXspeed(dirx*100);
    //player.setYspeed(diry*100);

    bplayer->ApplyLinearImpulse(b2Vec2(dirx*5,diry*5), bplayer->GetWorldCenter());

//    playSprite1.setXspeed(dirx * 100);
//    playSprite1.setYspeed(diry * 100);

    //game->changeState(PlayMap::instance());
    //game->changeState(PlayMapTop::instance());
    //game->changeState(PlayMapAI::instance());
    //game->changeState(PlayPhysics::instance());
    //game->changeState(PlayMapPhysics::instance());
}

void PlayPhysics::update(cgf::Game* game)
{
    screen = game->getScreen();
    if(firstTime)
    {
        phys->setRenderTarget(*screen);
        firstTime = false;
    }

//    if(playSprite1.bboxCollision(playSprite2))
//        cout << "Bump!" << endl;

    //checkCollision(2, game, &player);

    //if(checkCollision(2, game, &ghost)) {
    //    cout << "BUMP!" << endl;
    //    ghost.setXspeed(-ghost.getXspeed());
    //}

//    playSprite1.update(game->getUpdateInterval());


    /*
    if(monkey.getPosition().x > 600)
    {
        monkey.setXspeed(-100);
        monkey.setMirror(true);
    }
    if(monkey.getPosition().x < 50)
    {
        monkey.setXspeed(100);
        monkey.setMirror(false);
    }*/

    phys->step();

    centerMapOnPlayer();
}

bool PlayPhysics::checkCollision(u_int8_t layer, cgf::Game* game, cgf::Sprite* obj)
{
    int i, x1, x2, y1, y2;
    bool bump = false;

    // Get the limits of the map
    sf::Vector2u mapsize = map->GetMapSize();
    // Get the width and height of a single tile
    sf::Vector2u tilesize = map->GetMapTileSize();

    mapsize.x /= tilesize.x;
    mapsize.y /= tilesize.y;
    mapsize.x--;
    mapsize.y--;

    // Get the height and width of the object (in this case, 100% of a tile)
    sf::Vector2u objsize = obj->getSize();
    objsize.x *= obj->getScale().x;
    objsize.y *= obj->getScale().y;

    float px = obj->getPosition().x;
    float py = obj->getPosition().y;

    double deltaTime = game->getUpdateInterval();

    sf::Vector2f offset(obj->getXspeed()/1000 * deltaTime, obj->getYspeed()/1000 * deltaTime);

    float vx = offset.x;
    float vy = offset.y;

    //cout << "px,py: " << px << " " << py << endl;

    //cout << "tilesize " << tilesize.x << " " << tilesize.y << endl;
    //cout << "mapsize " << mapsize.x << " " << mapsize.y << endl;

    // Test the horizontal movement first
    i = objsize.y > tilesize.y ? tilesize.y : objsize.y;

    for (;;)
    {
        x1 = (px + vx) / tilesize.x;
        x2 = (px + vx + objsize.x - 1) / tilesize.x;

        y1 = (py) / tilesize.y;
        y2 = (py + i - 1) / tilesize.y;

        if (x1 >= 0 && x2 < mapsize.x && y1 >= 0 && y2 < mapsize.y)
        {
            if (vx > 0)
            {
                // Trying to move right

                int upRight   = getCellFromMap(layer, x2*tilesize.x, y1*tilesize.y);
                int downRight = getCellFromMap(layer, x2*tilesize.x, y2*tilesize.y);
                if (upRight || downRight)
                {
                    // Place the player as close to the solid tile as possible
                    px = x2 * tilesize.x;
                    px -= objsize.x;// + 1;
                    vx = 0;
                    bump = true;
                }
            }

            else if (vx < 0)
            {
                // Trying to move left

                int upLeft   = getCellFromMap(layer, x1*tilesize.x, y1*tilesize.y);
                int downLeft = getCellFromMap(layer, x1*tilesize.x, y2*tilesize.y);
                if (upLeft || downLeft)
                {
                    // Place the player as close to the solid tile as possible
                    px = (x1+1) * tilesize.x;
                    vx = 0;
                    bump = true;
                }
            }
        }

        if (i == objsize.y) // Checked player height with all tiles ?
        {
            break;
        }

        i += tilesize.y; // done, check next tile upwards

        if (i > objsize.y)
        {
            i = objsize.y;
        }
    }

    // Now test the vertical movement

    i = objsize.x > tilesize.x ? tilesize.x : objsize.x;

    for (;;)
    {
        x1 = (px / tilesize.x);
        x2 = ((px + i-1) / tilesize.x);

        y1 = ((py + vy) / tilesize.y);
        y2 = ((py + vy + objsize.y-1) / tilesize.y);

        if (x1 >= 0 && x2 < mapsize.x && y1 >= 0 && y2 < mapsize.y)
        {
            if (vy > 0)
            {
                // Trying to move down
                int downLeft  = getCellFromMap(layer, x1*tilesize.x, y2*tilesize.y);
                int downRight = getCellFromMap(layer, x2*tilesize.x, y2*tilesize.y);
                if (downLeft || downRight)
                {
                    // Place the player as close to the solid tile as possible
                    py = y2 * tilesize.y;
                    py -= objsize.y;
                    vy = 0;
                    bump = true;
                }
            }

            else if (vy < 0)
            {
                // Trying to move up

                int upLeft  = getCellFromMap(layer, x1*tilesize.x, y1*tilesize.y);
                int upRight = getCellFromMap(layer, x2*tilesize.x, y1*tilesize.y);
                if (upLeft || upRight)
                {
                    // Place the player as close to the solid tile as possible
                    py = (y1 + 1) * tilesize.y;
                    vy = 0;
                    bump = true;
                }
            }
        }

        if (i == objsize.x)
        {
            break;
        }

        i += tilesize.x; // done, check next tile to the right

        if (i > objsize.x)
        {
            i = objsize.x;
        }
    }

    // Now apply the movement and animation

    obj->setPosition(px+vx,py+vy);
    px = obj->getPosition().x;
    py = obj->getPosition().y;

    obj->update(deltaTime, false); // only update animation

    // Check collision with edges of map
    if (px < 0)
        obj->setPosition(px,py);
    else if (px + objsize.x >= mapsize.x * tilesize.x)
        obj->setPosition(mapsize.x*tilesize.x - objsize.x - 1,py);

    if(py < 0)
        obj->setPosition(px,0);
    else if(py + objsize.y >= mapsize.y * tilesize.y)
        obj->setPosition(px, mapsize.y*tilesize.y - objsize.y - 1);

    return bump;
}

sf::Uint16 PlayPhysics::getCellFromMap(uint8_t layernum, float x, float y)
{
    auto layers = map->GetLayers();
    tmx::MapLayer& layer = layers[layernum];
    sf::Vector2u mapsize = map->GetMapSize();
    sf::Vector2u tilesize = map->GetMapTileSize();
    mapsize.x /= tilesize.x;
    mapsize.y /= tilesize.y;
    int col = floor(x / tilesize.x);
    int row = floor(y / tilesize.y);
    return layer.tiles[row*mapsize.x + col].gid;
}

void PlayPhysics::centerMapOnPlayer()
{
    sf::View view = screen->getView();
    sf::Vector2u mapsize = map->GetMapSize();
    sf::Vector2f viewsize = view.getSize();
    viewsize.x /= 2;
    viewsize.y /= 2;
    sf::Vector2f pos = player.getPosition();

//    cout << "vw: " << view.getSize().x << " " << view.getSize().y << endl;

    float panX = viewsize.x; // minimum pan
    if(pos.x >= viewsize.x)
        panX = pos.x;

    if(panX >= mapsize.x - viewsize.x)
        panX = mapsize.x - viewsize.x;

    float panY = viewsize.y; // minimum pan
    if(pos.y >= viewsize.y)
        panY = pos.y;

    if(panY >= mapsize.y - viewsize.y)
        panY = mapsize.y - viewsize.y;

//    cout << "pos: " << pos.x << " " << pos.y << endl;
//    cout << "pan: " << panX << " " << panY << endl;

    view.setCenter(sf::Vector2f(panX,panY));
    screen->setView(view);
}

void PlayPhysics::draw(cgf::Game* game)
{
    //sf::View view = screen->getView();

    screen->clear(sf::Color(0,0,0));

    map->Draw(*screen, 0);
    map->Draw(*screen, 1);
//    screen->draw(playSprite1);
//    screen->draw(playSprite2);
//    screen->draw(playSprite3);
    screen->draw(ghost);
    screen->draw(player);

    phys->drawDebugData();
    //phys->debugDraw(*screen);

    sf::Text text;
    // select the font
    text.setFont(font);
    text.setString(L"Arrá!");
    text.setCharacterSize(24); // in pixels, not points!
    text.setColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    screen->draw(text);
}
