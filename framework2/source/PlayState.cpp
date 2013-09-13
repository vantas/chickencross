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
#include "Game.h"
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
    map->Load("dungeon-tilesets2.tmx");

    playSprite1.load("data/img/Char14.png");
	playSprite1.setPosition(10,100);

//	playSprite1->setAnimRate(30);        // quadros/segundo
//	playSprite1->setXspeed(200);         // pixels/segundo
//    playSprite2->loadSprite("data/img/char9.png",128,128,0,0,0,40,4,2,6);

    playSprite2.load("data/img/Char01.png");
	playSprite2.setPosition(10,300);

    playSprite3.load("data/img/Char01.png");
	playSprite3.setPosition(50,300);

    //player.load("data/img/smurf_sprite.png", 128, 128, 0, 0, 0, 0, 7, 3, 16);
    //player.loadSpriteSparrowXML("data/img/smurf_sprite.xml");
    player.loadXML("data/img/monkey.xml");
    player.loadAnimation("data/img/monkeyanim.xml");
    player.setPosition(30,30);
    player.setAnimRate(15);
//    player.setFrameRange(12,13);
    player.setAnimation("walk-right");
    player.setXspeed(100);
    player.setRotation(0);
    player.play();
//    player.setScale(1);

//	playSprite2->setAnimRate(10);        // quadros/segundo
//	playSprite2->setXspeed(30);         // pixels/segundo

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

void PlayState::handleEvents(cgf::Game* game)
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

void PlayState::update(cgf::Game* game)
{
    screen = game->getScreen();
    float x = playSprite1.getPosition().x;
    float y = playSprite1.getPosition().y;
    x += dirx*5;
    y += diry*5;
    playSprite1.setPosition(x,y);
    player.update(game->getUpdateInterval());

    if(playSprite1.bboxCollision(playSprite2))
        cout << "Bump!" << endl;

//    cout << "x: " << x << " y: " << y << endl;
    cout << getCellFromMap(2, x,y) << endl;

//    auto layers = map->GetLayers();
//    tmx::MapLayer& layer = layers[2];
//    int moo = 0;
//    for(auto tile: layer.tiles)
//    {
//        cout << tile.gridCoord.x << "," << tile.gridCoord.y << " (" << tile.gid << ") ";
//        if(++moo>3) break;
//    }
//    cout << endl;
//    cout << layer.name << endl;
//    for(auto object = layer.objects.begin(); object != layer.objects.end(); ++object)
//    {
//        cout << object->GetShapeType() << endl;
//    }

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

    centerMapOnPlayer();
}

void PlayState::checkCollision(cgf::Game* game, cgf::Sprite* obj)
{
    int i, x1, x2, y1, y2;

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

    float px = obj->getPosition().x;
    float py = obj->getPosition().y;

    float vx = obj->getXspeed();
    float vy = obj->getYspeed();

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

                int upRight   = getCellFromMap(2, x2, y1);
                int downRight = getCellFromMap(2, x2, y2);
                if (upRight || downRight)
                {
                    // Place the player as close to the solid tile as possible
                    px = x2 * tilesize.x;
                    px -= objsize.x;// + 1;
                    vx = 0;
                }
            }

            else if (vx < 0)
            {
                // Trying to move left

                int upLeft   = getCellFromMap(2, x1, y1);
                int downLeft = getCellFromMap(2, x1, y2);
                if (upLeft || downLeft)
                {
                    // Place the player as close to the solid tile as possible
                    px = (x1+1) * tilesize.x;
                    vx = 0;
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
                int downLeft  = getCellFromMap(2, x1, y2);
                int downRight = getCellFromMap(2, x2, y2);
                cout << "downleft, downright: " << downLeft << " " << downRight << endl;
                if (downLeft || downRight)
                {
                    // Place the player as close to the solid tile as possible
                    py = y2 * tilesize.y;
                    py -= objsize.y;
                    vy = 0;
                }
            }

            else if (vy < 0)
            {
                // Trying to move up

                int upLeft  = getCellFromMap(2, x1, y1);
                int upRight = getCellFromMap(2, x2, y1);
                if (upLeft || upRight)
                {
                    // Place the player as close to the solid tile as possible
                    py = (y1 + 1) * tilesize.y;
                    vy = 0;
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

    // Now apply the movement

    obj->setPosition(px+vx,py+vy);
    obj->setXspeed(0);
    obj->setYspeed(0);
    px = obj->getX();
    py = obj->getY();

    // Check collision with edges of map
    if (px < 0)
        obj->setX(0);
    else if (px + objsize.x >= maxMapX * tilesize.x)
        obj->setX(maxMapX*tilesize.x - objsize.x - 1);

    if(py < 0)
        obj->setY(0);
    else if(py + objsize.y >= maxMapY * tilesize.y)
        obj->setY(maxMapY*tilesize.y - objsize.y - 1);
}

sf::Uint16 PlayState::getCellFromMap(uint8_t layernum, float x, float y)
{
    auto layers = map->GetLayers();
    tmx::MapLayer& layer = layers[layernum];
    sf::Vector2u mapsize = map->GetMapSize();
    sf::Vector2u tilesize = map->GetMapTileSize();
    mapsize.x /= tilesize.x;
    mapsize.y /= tilesize.y;
//    cout << "tile.x: " << tilesize.x << ", y: "<< tilesize.y << endl;
    int col = floor(x / tilesize.x);
    int row = floor(y / tilesize.y);
//    cout << "col: "<< col << " row: " << row << " ";
    return layer.tiles[row*mapsize.x + col].gid;
//    cout << tile.gridCoord.x << "," << tile.gridCoord.y << " (" << tile.gid << ")" << endl;
//    return tile.gid;
}

void PlayState::centerMapOnPlayer()
{
    sf::View view = screen->getView();
    sf::Vector2u mapsize = map->GetMapSize();
    sf::Vector2f viewsize = view.getSize();
    viewsize.x /= 2;
    viewsize.y /= 2;
    sf::Vector2f pos = playSprite1.getPosition();

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

void PlayState::draw(cgf::Game* game)
{
    //sf::View view = screen->getView();

    screen->clear(sf::Color(0,0,0));

    map->Draw(*screen, 0);
    map->Draw(*screen, 1);
    screen->draw(playSprite1);
    screen->draw(playSprite2);
    screen->draw(playSprite3);
    screen->draw(player);

    sf::Text text;
    // select the font
    text.setFont(font);
    text.setString(L"Arrá!");
    text.setCharacterSize(24); // in pixels, not points!
    text.setColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    screen->draw(text);
}
