#ifndef CFONT_H
#define CFONT_H

/*
 *  CFont.h
 *  Font management class
 *
 *  Created by Alberto Dietrich on 04/11.
 *  Modified by Marcelo Cohen on 04/11.
 *
 */

#include "CMultiImage.h"

class CFont : public CMultiImage
{

	private:

		int charHeight, charWidth;

    public:
        CFont();
        virtual ~CFont();
        bool loadFont(char nomeArq[], int imgwidth, int imgHeight);
        void draw(char text[], int type=CFONT_BOTTOM | CFONT_LEFT);
        void draw(float x, float y, char text[], int type=CFONT_BOTTOM | CFONT_LEFT);
        enum CFONT_ALIGN
        {
            CFONT_TOP = 1,
            CFONT_BOTTOM = 2,
            CFONT_LEFT = 4,
            CFONT_RIGHT = 8,
            CFONT_VCENTER = 16,
            CFONT_HCENTER = 32
        };

};

#endif // CFONT_H
