#include "CImage.h"
#include "TextureManager.h"

using namespace std;

TextureManager* CImage::tm = TextureManager::getInstance();

CImage::CImage() : sf::Sprite()
{
    tex = NULL;
    visible = true;
}

bool CImage::loadImage(char nomeArq[])
{
    tex = tm->findTexture(nomeArq);

//    if(!tex.loadFromFile(nomeArq)) {
//		cout << "Error loading " << nomeArq << endl;
//		return false;
//	}

//	width  = tex->getSize().x;
//	height = tex->getSize().y;

//    texture = TextureManager::getInstance()->findTexture(nomeArq);

//   xOffset = width/2;
//   yOffset = height/2;

    setTexture(*tex);

	return true;
}

