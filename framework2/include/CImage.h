#ifndef CIMAGE_H
#define CIMAGE_H

#include <iostream>
#include <SDL.h>
#include "Graphics.h"
#include <SDL_image.h>

class CImage
{
	protected:
		GLuint texture;     // texture id for this image
		int width, height;	// image width and height
        float x, y;
        float scale;
        float rotation;
        float xOffset;	    // offset to apply when drawing (pivot for scaling/rotation)
		float yOffset;
        float xTexOffset;   // offset to apply to tex coords (for parallax, etc)
        float yTexOffset;
        bool visible;
		
	public:
		CImage();
		virtual ~CImage();
		bool loadImage(char nomeArq[]);
        virtual void draw();
		int getWidth()  { return width; }
		int getHeight() { return height; }
		void setX(float x);
		void setY(float y);
		void setPosition(float x, float y);
		float getX() { return x; }
		float getY() { return y; }
		void setScale(float s);
		float getScale() { return scale; }
		void setRotation(float r);
		float getRotation() { return rotation; }
        void setXOffset(float xo);
		void setYOffset(float yo);
		float getXOffset() { return xOffset; }
		float getYOffset() { return yOffset; }
        void setXTexOffset(float xo);
        void setYTexOffset(float yo);
        float getXTexOffset() { return xTexOffset; }
        float getYTexOffset() { return yTexOffset; }
        void setVisible(bool status) { visible = status; }
        bool getVisible() { return visible; }
};

#endif  // CIMAGE_H
