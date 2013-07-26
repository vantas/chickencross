#include "CImage.h"
#include "TextureManager.h"

using namespace std;

CImage::CImage()
{
	xOffset = 0;
	yOffset = 0;
    xTexOffset = 0;
    yTexOffset = 0;
	scale = 1;
	rotation = 0;
	x = 0;
    y = 0;
    tex = NULL;
    visible = true;
}

bool CImage::loadImage(char nomeArq[])
{
    tex = TextureManager::getInstance()->findTexture(nomeArq);

//    if(!tex.loadFromFile(nomeArq)) {
//		cout << "Error loading " << nomeArq << endl;
//		return false;
//	}

	width  = tex->getSize().x;
	height = tex->getSize().y;

//    texture = TextureManager::getInstance()->findTexture(nomeArq);

    xOffset = width/2;
    yOffset = height/2;

    sprite.setTexture(*tex);

	return true;
}

/** @brief setPosition
  *
  * @todo: document this function
  */
void CImage::setPosition(float x, float y)
{
    setX(x);
    setY(y);
}

/** @brief setY
  *
  * @todo: document this function
  */
void CImage::setY(float y)
{
    this->y = y;
    sprite.setPosition(x,y);
}

/** @brief setX
  *
  * @todo: document this function
  */
void CImage::setX(float x)
{
    this->x = x;
    sprite.setPosition(x,y);
}

/** @brief setRotation
  *
  * @todo: document this function
  */
void CImage::setRotation(float r)
{
    this->rotation = r;
    sprite.setRotation(r);
}

/** @brief setScale
  *
  * @todo: document this function
  */
void CImage::setScale(float s)
{
    this->scale = s;
    sprite.setScale(s,s);
}

// Funcao que desenha a imagem toda e sera sobrescrita nas subclasses.
void CImage::draw(sf::RenderWindow* screen)
{
    if(!visible) return;

    screen->draw(sprite);
}

void CImage::setXOffset(float xo)
{
	xOffset = xo;
}

void CImage::setYOffset(float yo)
{
	yOffset = yo;
}

void CImage::setXTexOffset(float xo)
{
	xTexOffset = xo;
}

void CImage::setYTexOffset(float yo)
{
	yTexOffset = yo;
}

// Destrutor para limpar a area ocupada pelo bitmap.
// (agora isso é feito por TextureManager)
CImage::~CImage()
{
    //glDeleteTextures(1, &texture);
}
