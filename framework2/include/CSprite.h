#ifndef CSPRITE_H
#define CSPRITE_H

/*
 *  CSprite.cpp
 *  Sprite class
 *
 *  Created by Marcelo Cohen on 04/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#include <vector>
#include "CMultiImage.h"
#include "TexRect.h"
#include "tinyxml.h"

class CSprite : public CMultiImage, public TiXmlVisitor
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
        void update(double updateInterval);
        void draw();
        double getXspeed() { return xspeed; }
        double getYspeed() { return yspeed; }
        int getCurrentFrame() { return curframe; }
        int getTotalFrames() { return totalFrames; }
        bool bboxCollision(CSprite* other);
        bool circleCollision(CSprite* other);

        // TiXmlVisitor overrides
        //virtual bool 	VisitEnter  (const TiXmlDocument  &);
        //virtual bool 	VisitExit (const TiXmlDocument &);
        virtual bool 	VisitEnter (const TiXmlElement &, const TiXmlAttribute *);
        //virtual bool 	VisitExit (const TiXmlElement &);
        //virtual bool 	Visit (const TiXmlDeclaration &);
        //virtual bool 	Visit (const TiXmlText &);
        //virtual bool 	Visit (const TiXmlComment &);
        //virtual bool 	Visit (const TiXmlUnknown &);
    
private:
        double xspeed,yspeed;       // speed in pixels/s
        int updateCount;            // current count of updates
        int firstFrame, lastFrame; // frame range
        int curframe;		        // current frame
        double curFrameD;           // the current frame as double
		int framecount,framedelay;  // slow down the frame animation
        int spriteW, spriteH;       // width and height of a single sprite frame
};

#endif // CSPRITE_H
