/*
 *  PlayMapAI.cpp
 *  Testbed for AI experiments
 *
 *  Created by Marcelo Cohen on 05/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#include <iostream>
#include <cmath>
#include "Game.h"
#include "PlayMapAI.h"
#include "VectorUtils.h"

PlayMapAI PlayMapAI::m_PlayMapAI;

using namespace std;

const string PlayMapAI::modes[] = { "Chase", "Arrive", "Pursuit", "Flee", "Evade" };

void PlayMapAI::init()
{
    map = new tmx::MapLoader("data/maps");
    map->Load("dungeon2layers.tmx");

    playerK.sprite = &player;
    playerK.vel.x = playerK.vel.y = 0;   // current player speed
    cameraSpeed = 8;   // speed to use

    player.loadXML("data/img/hunter.xml");
    player.setPosition(50,100);
    player.loadAnimation("data/img/hunteranim.xml");
    player.setAnimRate(15);

    ghost.load("data/img/Char14.png");
    ghost.setPosition(100,300);
    ghost.setScale(sf::Vector2f(2,2));

    enemyK.sprite = &ghost;
    enemyK.pos.x = 100;
    enemyK.pos.y = 300;
    enemyK.maxSpeed = 7;

    steerMode = CHASE_BEHAVIOR; // default: chase player

    font.loadFromFile("data/fonts/arial.ttf");

    firstTime = true; // to set map position at first update

    showTrails = false;

	cout << "PlayMapAI Init Successful" << endl;
}

void PlayMapAI::cleanup()
{
    delete map;
    //  delete playSprite2;
	cout << "PlayMapAI Clean Successful" << endl;
}

void PlayMapAI::pause()
{
	cout << "PlayMapAI Paused" << endl;
}

void PlayMapAI::resume()
{
	cout << "PlayMapAI Resumed" << endl;
}

void PlayMapAI::handleEvents(cgf::Game* game)
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
            else if(event.key.code == sf::Keyboard::A)
            {
                steerMode++;
                if(steerMode > EVADE_BEHAVIOR)
                    steerMode = CHASE_BEHAVIOR;
            }
            else if(event.key.code == sf::Keyboard::T)
                showTrails = !showTrails;
    }

    int dirx, diry;
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

    playerK.vel.x = dirx*cameraSpeed;
    playerK.vel.y = diry*cameraSpeed;
}

// Steering Behavior: chase target
sf::Vector3f PlayMapAI::chase(Kinematic& vehicle, sf::Vector3f& target)
{
    sf::Vector3f desiredVel = target - vehicle.pos;
    vecutils::normalize(desiredVel);
    desiredVel *= vehicle.maxSpeed;
    return desiredVel - vehicle.vel;
}

// Steering Behavior: arrive at target
sf::Vector3f PlayMapAI::arrive(Kinematic& vehicle, sf::Vector3f& target, float decel)
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
sf::Vector3f PlayMapAI::flee(Kinematic& vehicle, sf::Vector3f& target, float panicDistance)
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
sf::Vector3f PlayMapAI::pursuit(Kinematic& vehicle, Kinematic& target)
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
sf::Vector3f PlayMapAI::evade(Kinematic& vehicle, Kinematic& target)
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

void PlayMapAI::update(cgf::Game* game)
{
    // First time in update, set initial camera pos
    if(firstTime) {
        screen = game->getScreen();
        firstTime = false;
    }

    checkCollision(1, game, playerK);
    centerMapOnPlayer();

#define STEERING
#ifdef STEERING
    // Apply steering behavior(s)

    //sf::Vector3f steeringForce = flee(enemyK, playerK,100);
    //sf::Vector3f steeringForce = pursuit(enemyK, playerK);
    sf::Vector3f steeringForce;

    switch(steerMode) {
        case CHASE_BEHAVIOR:
            steeringForce = chase(enemyK, playerK.pos);
            break;
        case ARRIVE_BEHAVIOR:
            steeringForce = arrive(enemyK, playerK.pos, 1); // 0.3 - lento ... 1 - rápido
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
    sf::Vector3f accel = steeringForce/1.f; // mass;

    enemyK.vel += accel; // * deltaTime

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
#endif
    checkCollision(1, game, enemyK);

    if(trail.size()>30)
        trail.pop_back();
    trail.push_front(enemyK.pos);
}

void PlayMapAI::centerMapOnPlayer()
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

bool PlayMapAI::checkCollision(uint8_t layer, cgf::Game* game, Kinematic& obj)
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
    sf::Vector2u objsize = obj.sprite->getSize();
    objsize.x *= obj.sprite->getScale().x;
    objsize.y *= obj.sprite->getScale().y;

    float px = obj.pos.x; //->getPosition().x;
    float py = obj.pos.y; //->getPosition().y;

    double deltaTime = game->getUpdateInterval();

    sf::Vector2f offset(obj.vel.x/1000 * deltaTime, obj.vel.y/1000 * deltaTime);

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

    obj.pos.x = px+vx;
    obj.pos.y = py+vy;
    obj.sprite->setPosition(px+vx,py+vy);
    px = obj.sprite->getPosition().x;
    py = obj.sprite->getPosition().y;

    obj.sprite->update(deltaTime, false); // only update animation

    // Check collision with edges of map
    if (px < 0)
        obj.sprite->setPosition(px,py);
    else if (px + objsize.x >= mapsize.x * tilesize.x)
        obj.sprite->setPosition(mapsize.x*tilesize.x - objsize.x - 1,py);

    if(py < 0)
        obj.sprite->setPosition(px,0);
    else if(py + objsize.y >= mapsize.y * tilesize.y)
        obj.sprite->setPosition(px, mapsize.y*tilesize.y - objsize.y - 1);

    return bump;
}

#ifdef OLDCOLL
void PlayMapAI::checkCollision(uint8_t layer, cgf::Game* game, Kinematic& obj)
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

    // << obj.pos.X << " " << obj.pos.Y << endl;

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

                int upRight   = map.getCell(x2,y1,1);
                int downRight = map.getCell(x2,y2,1);
                if (upRight || downRight)
                {
                    // Place the player as close to the solid tile as possible
                    obj.pos.x = x2 * tileW;
                    obj.pos.x -= playerW;
                    obj.vel.x = 0;
                }
            }

            else if (obj.vel.x < 0)
            {
                // Trying to move left

                int upLeft   = map.getCell(x1,y1,1);
                int downLeft = map.getCell(x1,y2,1);
                if (upLeft || downLeft)
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
        x2 = ((obj.pos.x + i -1) / tileW);

        y1 = ((obj.pos.y + obj.vel.y) / tileH);
        y2 = ((obj.pos.y + obj.vel.y + playerH - 1) / tileH);

        if (x1 >= 0 && x2 < maxMapX && y1 >= 0 && y2 < maxMapY)
        {
            if (obj.vel.y > 0)
            {
                // Trying to move down
                int downLeft  = map.getCell(x1,y2,1);
                int downRight = map.getCell(x2,y2,1);
                if (downLeft || downRight)
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

                int upLeft  = map.getCell(x1,y1,1);
                int upRight = map.getCell(x2,y1,1);
                if (upLeft || upRight)
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
#endif

sf::Uint16 PlayMapAI::getCellFromMap(uint8_t layernum, float x, float y)
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

void PlayMapAI::draw(cgf::Game* game)
{
    //sf::View view = screen->getView();

    screen->clear(sf::Color(0,0,0));

    map->Draw(*screen, 0);
    map->Draw(*screen, 1);

    //player->setPosition(playerK.pos.x,playerK.pos.y);
    screen->draw(player);

    /*
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
    */

    ghost.setPosition(enemyK.pos.x, enemyK.pos.y);
    screen->draw(ghost);

    sf::Text text;
    // select the font
    text.setFont(font);
    text.setCharacterSize(12);
    text.setColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    text.setPosition(enemyK.pos.x, enemyK.pos.y-14);
    text.setString(modes[steerMode]);
    screen->draw(text);
}
