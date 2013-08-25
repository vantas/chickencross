#ifndef CIMAGE_H
#define CIMAGE_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
//#include "Graphics.h"

class CImage : public sf::Sprite
{
	protected:
        bool visible;
        static cgf::TextureManager* tm;

    //private:
    //    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	public:
		CImage();
		virtual ~CImage() { }
		bool loadImage(char nomeArq[]);

        void setVisible(bool status) { visible = status; }
        bool getVisible() { return visible; }
};

#endif  // CIMAGE_H
