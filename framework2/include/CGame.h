/*
 *  CGame.h
 *  Main game class
 *
 *  Created by Marcelo Cohen on 05/13.
 *  Copyright 2013 PUCRS. All rights reserved.
 *
 */

#ifndef CGAME_H
#define CGAME_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <stack>

class CGameState;

class CGame
{
    public:

        CGame(int minFrameRate=15, int maxFrameRate=20);
        void init(const char* title, int width, int height, bool fullscreen);
        void handleEvents();
        void changeState(CGameState* state);
        void pushState(CGameState* state);
        void popState();
        void update();
        void draw();
        void clean();
        bool isRunning() { return running; }
        void quit()    { running = false; }
        void resize(int w, int h);
        void updateCamera();
        sf::RenderWindow* getScreen() { return screen; }
        static void printAttributes();
        double getUpdateInterval() { return updateInterval; }

        void setXpan(float xpan);
        void setYpan(float ypan);

        void setZoom(float z);

        float getXpan() { return panX; }
        float getYpan() { return panY; }
        float getZoom() { return zoom; }
        float getWidth();
        float getHeight();

        // Access to audio engine
//        irrklang::ISoundEngine* getAudioEngine() {
//            return audioEngine;
//        }

    private:

        sf::RenderWindow* screen;
        bool running;
        bool fullscreen;
        int bpp; // bits per pixel (screen colour depth)
        int flags; // SDL initialization flags
        std::stack<CGameState*> states;
        float xmin,xmax,ymin,ymax;
        float zoom;
        float panX;
        float panY;
        // Fixed interval time-based animation
        sf::Clock gameClock;
        int minFrameRate;
        int maxFrameRate;
        double updateInterval;
        double maxCyclesPerFrame;
        double lastFrameTime;
        double cyclesLeftOver;
        // Audio engine
//        irrklang::ISoundEngine* audioEngine;
};

#endif

