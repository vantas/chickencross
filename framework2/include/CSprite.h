#ifndef CSPRITE_H
#define CSPRITE_H

/*
 *  CSprite.cpp
 *  Sprite class
 *
 *  Created by Marcelo Cohen on 08/13.
 *  Copyright 2013 PUCRS. All rights reserved.
 *
 */

#include <vector>
#include "TextureManager.h"
#include "TexRect.h"
#include "tinyxml.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>

//#include "Animation.hpp"

class CSprite : public sf::Drawable, public sf::Transformable, public TiXmlVisitor
{
public:
    CSprite();
    virtual ~CSprite();

    bool loadSprite(char nomeArq[], int w, int h, int hSpace, int vSpace, int xIni, int yIni,
                int column, int row, int total);
    bool loadSpriteSparrowXML(char nomeArq[]);
    void setXspeed(double xspeed);
    void setYspeed(double yspeed);
    void setCurrentFrame(int c);
    bool setFrameRange(int first, int last);
    void frameForward();
    void frameBack();
    void setAnimRate(int fdelay);
    void update(double deltaTime);
    double getXspeed() { return xspeed; }
    double getYspeed() { return yspeed; }
    int getCurrentFrame() { return curframe; }
    int getTotalFrames() { return totalFrames; }
    bool bboxCollision(CSprite* other);
    bool circleCollision(CSprite* other);

    // TiXmlVisitor overrides
    virtual bool 	VisitEnter (const TiXmlElement &, const TiXmlAttribute *);

//    void setAnimation(const Animation& animation);
//    void setFrameTime(sf::Time time);
//    void play();
//    void pause();
//    void stop();
//    void setLooped(bool looped);
//    void setColor(const sf::Color& color);
//    sf::FloatRect getLocalBounds() const;
//    sf::FloatRect getGlobalBounds() const;
//    bool isLooped() const;
//    bool isPlaying() const;
//    sf::Time getFrameTime() const;
//    void setFrame(std::size_t newFrame, bool resetTime = true);

private:
    //const Animation* m_animation;
    //sf::Time m_frameTime;
    //sf::Time m_currentTime;
    //std::size_t m_currentFrame;
    //bool m_isPaused;
    //bool m_isLooped;

    static TextureManager* tm;

    bool mirror;
    double xspeed,yspeed;       // speed in pixels/s
    int updateCount;            // current count of updates
    int firstFrame, lastFrame; // frame range
    int totalFrames;
    int curframe;		        // current frame
    double curFrameD;           // the current frame as double
	int framecount,framedelay;  // slow down the frame animation
    int spriteW, spriteH;       // width and height of a single sprite frame

    const sf::Texture* tex;
    std::vector<sf::IntRect> frames;
    sf::Vertex vertices[4];

    bool loadMultiImage(char nomeArq[], int w, int h, int hSpace, int vSpace, int xIni, int yIni, int column, int row, int total);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // CSPRITE_H
