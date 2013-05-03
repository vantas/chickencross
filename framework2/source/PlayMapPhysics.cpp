/*
 *  PlayMapPhysics.cpp
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
#include "PlayMapPhysics.h"
#include "PauseState.h"

PlayMapPhysics PlayMapPhysics::m_PlayMapPhysics;

using namespace std;

void PlayMapPhysics::init()
{
    map.loadMap("data/maps/misteryforest.tmx");
    xvel = yvel = 0;     // current player speed
    cameraSpeed = 300;    // base speed to use
    playerAnimRate = 34; // player animation rate when moving
    zvel = 0;
    keystate = SDL_GetKeyState(NULL); // get array of key states

    firstTime = true; // to set map position at first update
    onGround = true; // player starts on the ground

    phys = CPhysics::instance();
    // Set conversion factor (ratio between OpenGL coords and Box2D coords)
    phys->setConvFactor(30);
    // Set world gravity
    phys->setGravity(20);

    player = new CSprite();
    player->loadSpriteSparrowXML("data/img/smurfwalk.xml");
    //player->loadSprite("data/img/smurf_sprite.png", 128, 128, 0, 0, 0, 0, 4, 4, 16);
    player->setAnimRate(15);
    player->setScale(1);
    // Get corner of cell (1,11) of the map in world coords
    map.getCenter(1,11,playerX,playerY);
    player->setPosition(playerX, playerY);
    // Create physics object according to the player position and size
    playerPhys = phys->newBoxImage(PLAYER_ID, player,30,0.3,0);
    // This prevents the player to rotate
    playerPhys->SetFixedRotation(true);
    cout << "Mass: " << playerPhys->GetMass() << endl;

    // Example of NPC (non-player character): another Smurf...
    npc = new CSprite();
    npc->loadSpriteSparrowXML("data/img/smurfwalk.xml");
    //npc->loadSprite("data/img/smurf_sprite.png", 128, 128, 0, 0, 0, 0, 4, 4, 16);
    npc->setAnimRate(0);

    // NPC will be at the top platform
    float npcX, npcY;
    map.getCenter(2,3,npcX,npcY);
    npc->setPosition(npcX, npcY);
    npcPhys = phys->newBoxImage(NPC_ID, npc,30,0.3,0);
    npcPhys->SetFixedRotation(true);

    // Iterate through all collision objects defined in the map
    // and create a physics object
    cout << "Collision objects: " << map.totalObjects() << endl;
	for(int o=0; o<map.totalObjects(); o++)
	{
		Object& obj = map.getObject(o);
		phys->newBox(GROUND_ID, obj.x,obj.y,obj.width,obj.height,1000,1,0.1,true);
	}

    // Physics object: a "totem"
    float totemX, totemY;
    map.getCenter(6,11,totemX,totemY);
    totem = map.getTilesetImage();
    totem->setX(totemX);
    totem->setY(totemY);
    phys->newBoxImage(TOTEM_ID, totem, 200, 2, 0);

    // Another physics box, without sprite associated
	//phys->newBox(playerX+100,playerY-300,30,30,10,0.1,0);

    // And another one... this time we store the b2Body pointer
    //b2Body* boo = phys->newBox(playerX,playerY-250,35,60,10,0.1,0);

    ball = new CImage();
    ball->loadImage("data/img/bigball.png");
    ball->setPosition(playerX+300, playerY-300);
    ball->setScale(0.25);
    phys->newCircleImage(BALL_ID, ball, 10, 0.1, 0.5);
//    phys->newCircle(playerX+300,playerY-300,30,10,0.1,0);

    // Set offset to add to translation when drawing debug shapes
    // in physics
    phys->setDrawOffset(map.getTileWidth()/2, map.getTileHeight()/2);

    // Shaders initialization
	//#define SHADERS
    #ifdef SHADERS
    sm.createShaderProgram("simple");
    sm.attachShader("simplevs", VERTEX);
    sm.attachShader("simplefs", FRAGMENT);
    sm.loadShaderSource("simplevs","data/shaders/simple.vert");
    sm.loadShaderSource("simplefs","data/shaders/grayscale.frag");
    sm.compileShader("simplevs");
    sm.compileShader("simplefs");
    sm.attachShaderToProgram("simple", "simplevs");
    sm.attachShaderToProgram("simple", "simplefs");
    sm.linkProgramObject("simple");

    sm["simple"]->use();
    sm["simple"]->setUniform1i("tex", 0);
    sm["simple"]->unbind();
    #endif

    cout << "PlayMapPhysics Init Successful" << endl;
}

void PlayMapPhysics::cleanup()
{
    delete player;
    delete npc;
	cout << "PlayMapPhysics Clean Successful" << endl;
}

void PlayMapPhysics::pause()
{
	cout << "PlayMapPhysics Paused" << endl;
}

void PlayMapPhysics::resume()
{
	cout << "PlayMapPhysics Resumed" << endl;
}

void PlayMapPhysics::handleEvents(CGame* game)
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
                            yvel = -6000;
                            game->getAudioEngine()->play2D(jumpSoundSource);
                        }
                        break;
                    case SDLK_w:
                        if(onGround) {
                            yvel = -10000; // super jump
                            game->getAudioEngine()->play2D(superJumpSoundSource);
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
        if(!walkSound) {
            walkSound = game->getAudioEngine()->play2D(walkSoundSource, true, false, true);
        }
    } else if(walkSound) {
        // Stopped moving, stop and drop sound
        walkSound->stop();
        walkSound->drop();
        walkSound = NULL;
    }

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

    if(xvel || yvel) {
        //cout << "vel : "<< xvel << " - " << yvel << endl;
        //cout << playerX << " " << playerY << endl;
        //playerPhys->ApplyForce(b2Vec2(xvel,yvel), playerPhys->GetWorldCenter());
        playerPhys->ApplyLinearImpulse(b2Vec2(xvel,yvel), playerPhys->GetWorldCenter());
        b2Vec2 playerVel = playerPhys->GetLinearVelocity();
        float32 vy = playerVel.y;
        cout << "vel: " << playerVel.x << " " << playerVel.y << endl;
        // Test to prevent player to exceeding max speed when accelerating
        const float32 speed = playerVel.Length();
        if (speed > 20) {
            playerVel = (20/speed) * playerVel;
            playerVel.y = vy;
            playerPhys->SetLinearVelocity(playerVel);
        }
        xvel = yvel = 0;
    }

}

void PlayMapPhysics::update(CGame* game)
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
        smurf3DSoundSource = game->getAudioEngine()->addSoundSourceFromFile("data/audio/music.wav");
        walkSoundSource = game->getAudioEngine()->addSoundSourceFromFile("data/audio/walk.wav");
        jumpSoundSource = game->getAudioEngine()->addSoundSourceFromFile("data/audio/jump.wav");
        superJumpSoundSource = game->getAudioEngine()->addSoundSourceFromFile("data/audio/superjump.wav");
        walkSound = NULL;
        irrklang::vec3df pos(npc->getX(), npc->getY(), 0);
        smurf3DSoundSource->setDefaultMinDistance(300);
        smurf3DSoundSource->setDefaultMaxDistance(3000);
        loopedSound = game->getAudioEngine()->play3D(smurf3DSoundSource, pos, true, false, true);
    }

    // Player motion ?
    if(zvel != 0) {
        game->setZoom(game->getZoom()+zvel);
        game->updateCamera();
    }

//    checkCollision(game);
    phys->step();
    player->update(game->getUpdateInterval());
    playerX = player->getX();
    playerY = player->getY();
    centerPlayerOnMap(game);

    // Adjust position of NPC sound (music)
    int npcX = npc->getX();
    int npcY = npc->getY();
    loopedSound->setPosition(irrklang::vec3df(npcX,npcY,0));

    /*
    // Simple collision test with other sprite
    if(player->bboxCollision(npc)) {
        game->getAudioEngine()->play2D(jumpSoundSource);
        cout << "bump!" << endl;
    }
    */
    // Physically-based collision test with NPC
    if(phys->haveContact(PLAYER_ID, NPC_ID))
    {
        game->getAudioEngine()->play2D(jumpSoundSource);
        cout << "bump!" << endl;
    }

    if(phys->haveContact(PLAYER_ID, GROUND_ID))
    {
        cout << "on ground" << endl;
        onGround = true;
    }
    else {
        cout << "on air!" << endl;
        onGround = false;
    }
}

// Collision detection and centering based on code from
// http://www.parallelrealities.co.uk/tutorials/intermediate/tutorial14.php

void PlayMapPhysics::centerPlayerOnMap(CGame* game)
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

void PlayMapPhysics::draw(CGame* game)
{
    glClearColor(0.8,0.8,0.8,1); // light gray
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

#ifdef SHADERS
    sm["simple"]->use();
#endif

    map.draw();

	#ifdef SHADERS
    sm["simple"]->unbind();
#endif

    // Draw game objects
    player->draw();
    npc->draw();
    totem->drawFrame(1);
    ball->draw();

    //phys->debugDraw();

    SDL_GL_SwapBuffers();
}
