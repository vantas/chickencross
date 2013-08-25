#ifndef CGF_SPRITE_H
#define CGF_SPRITE_H

/*
 *  Sprite.cpp
 *  Animtated sprite class
 *
 *  Created by Marcelo Cohen on 08/13.
 *  Copyright 2013 PUCRS. All rights reserved.
 *
 */

#include <vector>
#include <map>
#include "TextureManager.h"
#include "CAnim.h"
#include "pugixml/pugixml.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>

namespace cgf
{

class Sprite : public sf::Drawable, public sf::Transformable
{
public:
    Sprite();
    virtual ~Sprite();

    bool loadSprite(char filename[], int w, int h, int hSpace, int vSpace, int xIni, int yIni,
                int column, int row, int total);
    bool loadSpriteXML(char filename[]);
    bool loadAnimation(char filename[]);

    // Mirroring (X-axis)
    void setMirror(bool mirror) { this->mirror = mirror; setCurrentFrame(curframe); }
    bool getMirror() { return mirror; }

    // Sprite speed
    void setXspeed(double xspeed);
    void setYspeed(double yspeed);
    double getXspeed() { return xspeed; }
    double getYspeed() { return yspeed; }

    // Animation control
    void setAnimation(std::string name);
    void setAnimRate(int fdelay);
    void play();
    void pause();
    void stop();
    void setLooped(bool looped) { looping = looped; }

    // Fine tuning animation controls
    bool setFrameRange(int first, int last);
    void setCurrentFrame(int c);
    void frameForward();
    void frameBack();

    int getCurrentFrame() { return curframe; }
    int getTotalFrames() { return totalFrames; }

    void update(double deltaTime);

    // Basic collision checking
    bool bboxCollision(Sprite& other);
    bool circleCollision(Sprite& other);

//    void setColor(const sf::Color& color);
//    sf::FloatRect getLocalBounds() const;
//    sf::FloatRect getGlobalBounds() const;
//    bool isLooped() const;
//    bool isPlaying() const;
//    sf::Time getFrameTime() const;
//    void setFrame(std::size_t newFrame, bool resetTime = true);

private:

    static TextureManager* tm;

    // Rendering
    const sf::Texture* tex;
    sf::Vertex vertices[4];
    int spriteW, spriteH;       // width and height of a single sprite frame
    bool mirror;

    // Motion
    double xspeed,yspeed;       // speed in pixels/s
    int updateCount;            // current count of updates

    // Animation
    std::map<std::string, CAnim> anims;
    std::vector<sf::IntRect> frames;
    CAnim* currentAnim;
    int firstFrame, lastFrame;
    bool looping;
    bool paused;
    int totalFrames;
    int curframe;		        // current frame
    double curFrameD;           // the current frame as double
    int framecount,framedelay;  // slow down the frame animation

    bool loadMultiImage(char nomeArq[], int w, int h, int hSpace, int vSpace, int xIni, int yIni, int column, int row, int total);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

} // namespace cgf

#endif // CGF_SPRITE_H
