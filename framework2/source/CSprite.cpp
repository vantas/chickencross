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
#include <iomanip>
#include <string>
#include <cmath>

using namespace std;

TextureManager* CSprite::tm = TextureManager::getInstance();

// Construtor
CSprite::CSprite()
{
    firstFrame = 0;
    lastFrame = 0;
    curFrameD = 0.0;
    curframe = 0;
    curframe = 0;
    framedelay = 10;
    mirror = false;
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
    setCurrentFrame(0);
    //setOrigin(w/2, h/2);
	return true;
}

bool CSprite::loadMultiImage(char nomeArq[], int w, int h, int hSpace, int vSpace, int xIni, int yIni, int column, int row, int total)
{
    tex = tm->findTexture(nomeArq);

    int width  = tex->getSize().x;
    int height = tex->getSize().y;

    // Check if the input parameters are valid
	if ( (hSpace<0 || vSpace<0) || (hSpace>width || vSpace>height) )
		return false;

	if ( xIni<0 || yIni<0 )
		return false;

	if ( column<1 || row<1 )
		return false;

    totalFrames = total;

	if (totalFrames < 1)
		return false;

    int x, y, tot;

    tot = 0;
    y = yIni;

    for(int r=0; r<row && tot<total; row++)
    {
        x = xIni;
        for(int c=0; c<column && tot<total; c++)
        {
            sf::IntRect rect;
            rect.left = x;
            rect.width = w;
            rect.top = y;
            rect.height = h;
            cout << "frame " << setw(3) << tot << ": " << x << " " << y << " " << w << " " << h << endl;
            frames.push_back(rect);

            x += w + hSpace;
            tot++;
        }
        y += h + vSpace;
    }

//    xOffset = w/2;
//    yOffset = h/2;

    //setOrigin(w/2, h/2);

    //cout << "CMultiImage::load: " << xOffset << " " << yOffset << endl;
    cout << "CSprite::loadMultimage total frames = " << total << endl;

    mirror = false;
    return true;
}

bool CSprite::loadSpriteSparrowXML(char xmlFile[])
{
    cout << "CSprite::loadSpriteSparrowXML " << xmlFile << endl;

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(xmlFile);

    if ( !result ) {
		return false;
	}

    // Read texture atlas file name

    pugi::xml_node atlas = doc.child("TextureAtlas");
    pugi::xml_attribute imagepath = atlas.attribute("imagePath");

    string attrib = imagepath.as_string();
    string prefix = "data/img/";

    cout << "CSprite::loadSpriteSparrowXML: " << attrib << endl;

    prefix.append(attrib);// = "data/img/"+attrib;

    cout << "TextureAtlas: " << prefix << endl;

    tex = tm->findTexture((char *)prefix.c_str());

    // Read all subtextures (frames)
    for(pugi::xml_node subtex = atlas.child("SubTexture"); subtex; subtex = subtex.next_sibling("SubTexture"))
//    for (pugi::xml_node subtex: doc.children("SubTexture"))
    {
        int x1, y1, h, w;

        cout << subtex.name() << endl;
        x1 = subtex.attribute("x").as_int();
        y1 = subtex.attribute("y").as_int();
        w = subtex.attribute("width").as_int();
        h = subtex.attribute("height").as_int();

        spriteW = w;
        spriteH = h;

        //cout << "Texture: " << x1 << " " << y1 << " " << w-1 << " " << h-1 << endl;
        sf::IntRect rect;
        rect.left = x1;
        rect.width = w;
        rect.top = y1;
        rect.height = h;
        cout << "frame " << setw(3) << frames.size() << ": " << rect.left << "," << rect.top
            << " - " << rect.width << "x" << rect.height << endl;
        frames.push_back(rect);

        //TODO: get spacing and margin
    }
//        bool ok = loadImage((char *) prefix.c_str());
//        if(!ok)
//        {
//			cout << "ERROR LOADING SPRITE IMG: " << prefix.c_str() << endl;
//        }

//    xOffset = spriteW/2;
//    yOffset = spriteH/2;

//    width = spriteW;
//    height = spriteH;

    totalFrames = frames.size();

    cout << "CSprite::loadSpriteSparrowXML total frames = " << totalFrames << endl;

    // Init animation vars
	xspeed = 0;
	yspeed = 0;
	curframe = 0;
    curFrameD = 0;
    firstFrame = 0;
    lastFrame = totalFrames-1;

    setMirror(false);
    setCurrentFrame(0);

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

    //sf::IntRect rect = m_animation->getFrame(m_currentFrame);
    sf::IntRect rect = frames[curframe];

    vertices[0].position = sf::Vector2f(0, 0);
    vertices[1].position = sf::Vector2f(0, rect.height);
    vertices[2].position = sf::Vector2f(rect.width, rect.height);
    vertices[3].position = sf::Vector2f(rect.width, 0);

    float left = static_cast<float>(rect.left) + 0.0001;
    float right = left + rect.width;
    float top = static_cast<float>(rect.top);
    float bottom = top + rect.height;

    if(mirror) {
        float tmp = left;
        left = right;
        right = tmp;
    }

    vertices[0].texCoords = sf::Vector2f(left, top);
    vertices[1].texCoords = sf::Vector2f(left, bottom);
    vertices[2].texCoords = sf::Vector2f(right, bottom);
    vertices[3].texCoords = sf::Vector2f(right, top);
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
void CSprite::update(double deltaTime)
{
    // Move sprite according to its speed and the amount of time that has passed
    sf::Vector2f offset(xspeed/1000 * deltaTime, yspeed/1000 * deltaTime);
//    x += xspeed/1000 * deltaTime;
//    y += yspeed/1000 * deltaTime;

    move(offset);

    int lastf = curframe;
    curFrameD += (double)framedelay/1000*deltaTime;
    curframe = (int) curFrameD;
    if(curframe > lastFrame) {
        curFrameD = firstFrame;
        curframe = firstFrame;
    }
    if(curframe != lastf)
        setCurrentFrame(curframe);
}

/*
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
*/

/*
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
*/

void CSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (tex)
    {
        states.transform *= getTransform();
        //if(mirror) states.transform.scale(-1,1);
        states.texture = tex;
        target.draw(vertices, 4, sf::Quads, states);
    }
}
