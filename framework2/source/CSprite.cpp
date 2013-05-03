/*
 *  CSprite.cpp
 *  Sprite class
 *
 *  Created by Marcelo Cohen on 04/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#include "CSprite.h"
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

// Construtor
CSprite::CSprite() : CMultiImage()
{
    firstFrame = 0;
    lastFrame = 0;
    curFrameD = 0.0;
    curframe = 0;
    curframe = 0;
    framedelay = 10;
}

bool CSprite::loadSprite(char nomeArq[], int w, int h, int hSpace, int vSpace, int xIni, int yIni,
                 int column, int row, int total)
{
    if(!loadMultiImage(nomeArq,w,h,hSpace,vSpace,xIni,yIni,column,row,total))
        return false;

	// Init animation vars
	xspeed = 0;
	yspeed = 0;
	curframe = 0;
    curFrameD = 0;
    firstFrame = 0;
    lastFrame = total-1;
	return true;
}

bool CSprite::loadSpriteSparrowXML(char xmlFile[])
{
    cout << "CSprite::loadSpriteSparrowXML " << xmlFile << endl;
    TiXmlDocument doc(xmlFile);

    if ( ! doc.LoadFile() ) {
		return false;
	}

    //TiXmlElement* elem = doc.RootElement();

    doc.Accept(this);

    xOffset = spriteW/2;
    yOffset = spriteH/2;

    width = spriteW;
    height = spriteH;

    totalFrames = uvs.size();

    cout << "CSprite::loadSpriteSparrowXML total frames = " << totalFrames << endl;

    // Init animation vars
	xspeed = 0;
	yspeed = 0;
	curframe = 0;
    curFrameD = 0;
    firstFrame = 0;
    lastFrame = totalFrames-1;

    mirror = false;
    return true;
}

CSprite::~CSprite()
{
    //dtor
}

// Especifica quantos pixels o sprite ira se mover em x.
void CSprite::setXspeed(double xspeed)
{
	this->xspeed = xspeed;
}

// Especifica quantos pixels a sprite ira se mover em y.
void CSprite::setYspeed(double yspeed)
{
	this->yspeed = yspeed;
}

// Sets the current frame
void CSprite::setCurrentFrame(int c)
{
	if ( c>=0 && c<totalFrames )
		curframe = c;
	else
		curframe = 0;
    curFrameD = curframe;
}

/** @brief setFrameRange
  *
  * @todo: document this function
  */
bool CSprite::setFrameRange(int first, int last)
{
    if(first > last || first < 0 || last >= totalFrames)
        return false;
    firstFrame = first;
    lastFrame = last;
    return true;
}

// Advance to next frame
void CSprite::frameForward()
{
	curframe++;
	if (curframe > lastFrame)
		curframe = firstFrame;
}

// Go back to previous frame
void CSprite::frameBack()
{
	curframe--;
	if (curframe < firstFrame)
		curframe = lastFrame;
}

// Recebe por parametro o valor que sera usado para especificar o atributo
// framedelay, responsavel por diminuir ou aumentar a taxa de animacao.
void CSprite::setAnimRate(int fdelay)
{
	if (fdelay >= 0)
		framedelay = fdelay;
	else
		framedelay = 0;

    // Reset framecount so next draw will work as it should be
    framecount = 0;
}

// Metodo responsavel por fazer as atualizacoes necessarias para a correta
// animacao do sprite.
void CSprite::update(double updateInterval)
{
    // Move sprite according to its speed and the amount of time that has passed
    x += xspeed/1000*updateInterval;
    y += yspeed/1000*updateInterval;

    curFrameD += (double)framedelay/1000*updateInterval;
    curframe = (int) curFrameD;
    if(curframe > lastFrame) {
        curFrameD = firstFrame;
        curframe = firstFrame;
    }
}

// Draw the sprite
void CSprite::draw()
{
    drawFrame(curframe);
}

// Check bounding box collision between this and other sprite
bool CSprite::bboxCollision(CSprite* other)
{
    float width1 = this->width/2 * this->scale;
    float width2 = other->width/2 * other->scale;

    float height1 = this->height/2 * this->scale;
    float height2 = other->height/2 * other->scale;

    float x0 = this->x - width1;
    float y0 = this->y - height1;
    float x1 = this->x + width1;
    float y1 = this->y + height1;

    float x2 = other->x - width2;
    float y2 = other->y - height2;
    float x3 = other->x + width2;
    float y3 = other->y + height2;

    return !(x1<x2 || x3<x0 || y1<y2 || y3<y0);
    //return !(x1<other->x || x3<this->x || y1<other->y || y3<this->y);
}

// Check circle collision between this and other sprite
bool CSprite::circleCollision(CSprite* other)
{
   int radius1 = max(this->width, this->height)/2;
   int radius2 = max(other->width, other->height)/2;
   radius1 *= this->scale;
   radius2 *= other->scale;
   float dist = sqrt(pow(this->x-other->x,2)+pow(this->y-other->y,2));
   //cout << "Radius: " << radius1 << " and " << radius2 << endl;
   //cout << "distance: " << dist << endl;
   return (dist < radius1 + radius2);
}

// TiXml visitor implementation: load texture atlas in Sparrow format (http://www.sparrow-framework.org/)
bool CSprite::VisitEnter (const TiXmlElement &elem, const TiXmlAttribute *attrib)
{
    cout << "CSprite::VisitEnter " << elem.Value() << "*"<< endl;
	if (elem.Value() == string("SubTexture")) {

        int x1, y1, h, w;
        elem.Attribute("x", &x1);
        elem.Attribute("y", &y1);
        elem.Attribute("height", &h);
        elem.Attribute("width", &w);

        spriteW = w;
        spriteH = h;

        cout << "Texture: " << x1 << " " << y1 << " " << w-1 << " " << h-1 << endl;
        float u1 = (float)x1/width;
        float v1 = (float)y1/height;
        float u2 = (float)(x1+w)/width;
        float v2 = (float)(y1+h)/height;
        TexRect rect = { u1, v1, u2, v2 };
        cout << "Rect: " << rect.u1 << "," << rect.v1
            << " - " << rect.u2 << "," << rect.v2 << endl;
        uvs.push_back(rect);

        //TODO: get spacing and margin
    }
	else if (elem.Value() == string("TextureAtlas")) { //string("TextureAtlas") == elem.Value()) {
        string attrib = elem.Attribute("imagePath");
		string prefix = "data/img/";
        prefix.append(attrib);// = "data/img/"+attrib;
        cout << "TextureAtlas: " << prefix << endl;
        bool ok = loadImage((char *) prefix.c_str());
        if(!ok)
        {
			cout << "ERROR LOADING SPRITE IMG: " << prefix.c_str() << endl;
        }
    }
    return true;
}
