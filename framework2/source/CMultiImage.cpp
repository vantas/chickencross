#include <iostream>
#include "CMultiImage.h"

using namespace std;

/** @brief CMultiImage
 *
 * @todo: document this function
 */
CMultiImage::CMultiImage() : CImage()
{
}


/** @brief ~CMultiImage
  *
  * @todo: document this function
  */
CMultiImage::~CMultiImage()
{

}

/** @brief loadMultiImage
  *
  * @todo: document this function
  */
bool CMultiImage::loadMultiImage(char nomeArq[], int w, int h, int hSpace, int vSpace, int xIni, int yIni, int column, int row, int total)
{
    if(!loadImage(nomeArq))
        return false;

    // Verifica se os valores de espaço entre os frames são corretos
	if ( (hSpace<0 || vSpace<0) || (hSpace>width || vSpace>height) )
		return false;

	if ( xIni<0 || yIni<0 )
		return false;

	if ( column<1 || row<1 )
		return false;

    totalFrames = total;

	if (totalFrames < 1)
		return false;

/**/
	float xAux;
	float yAux;
    
    // This computes min and max tex coords to use,
    // ensuring only texel CENTERS will be sampled
    float minU = .5/(float)width;
    float maxU = 1-minU;
    float minV = .5/(float)height;
    float maxV = 1-minV;
    
    // Find the texcoord dimensions for U and V
    float factorU = maxU - minU;
    float factorV = maxV - minV;

    float fw = (float)w/((float)width*factorU);
    float fh = (float)h/((float)height*factorV);
    
    // Compute the actual image width and height to use
    float imgW = fw * factorU;
    float imgH = fh * factorV;

//    float deltaX = fw; //fw / imgW; //width;
//    float deltaY = fh; //fh / imgH; //height;
    
    // The above doesn't  work - left only for reference...
    
    float deltaX = (float)w/width;
    float deltaY = (float)h/height;

    float hSpaceF = (float)hSpace/width;
    float vSpaceF = (float)vSpace/height;

    //float texelX = 1.0f/width;
    //float texelY = 1.0f/height;

    yAux = (float)yIni/height; // minV
    for (unsigned int i=0; i<row; i++)
	{
	    xAux = (float) xIni/width; // minU
        for (unsigned int j=0; j<column && uvs.size()<totalFrames; j++)
		{
			// Cria um "sub-bitmap" do pai, nas coordenadas especificadas com o tamanho especificado.
			float u1 = xAux;
			float v1 = yAux;
			float u2 = xAux+deltaX;//+texelX;
			float v2 = yAux+deltaY;//+texelY;
			TexRect rect = { u1, v1, u2, v2 };
			//cout << "Rect: " << rect.u1 << "," << rect.v1
            //    << " - " << rect.u2 << "," << rect.v2 << endl;
			uvs.push_back(rect);
			xAux += deltaX + hSpaceF;
		}
		yAux += deltaY + vSpaceF;
	}
/**/
/*
	int xAux;
	int yAux = yIni;

	float dx = 0.5/w;
	float dy = 0.5/h;

	for (int i=0; i<row; i++)
	{
	    xAux = xIni;
		for (int j=0; j<column && uvs.size()<totalFrames; j++)
		{
			// Cria um "sub-bitmap" do pai, nas coordenadas especificadas com o tamanho especificado.
			float u1 = (float)xAux/width;
			float v1 = (float)yAux/height;
			float u2 = (float)(xAux+w)/width;
			float v2 = (float)(yAux+h)/height;
			TexRect rect = { u1, v1, u2, v2 };
			//cout << "Rect: " << rect.u1 << "," << rect.v1
            //    << " - " << rect.u2 << "," << rect.v2 << endl;
			uvs.push_back(rect);
			xAux += w + hSpace;// - dx;
			//xAux--;
		}
		xAux = xIni;
		yAux += h + vSpace;// - dy;
		//yAux--;
	}
*/
    
//    xTexOffset = w/(float)width;
//    yTexOffset = h/(float)height;
    
	width = w;
	height = h;

    xOffset = w/2;
    yOffset = h/2;

    //cout << "CMultiImage::load: " << xOffset << " " << yOffset << endl;
    cout << "CMultiImage::load: total frames = " << totalFrames << endl;

    mirror = false;
    return true;
}

/** @brief drawFrame
  *
  * @todo: document this function
  */
void CMultiImage::drawFrame(int frame)
{
    if(!visible) return;

    TexRect& t = uvs[frame];
	float u1 = t.u1 + xTexOffset;
    float v1 = t.v1 + yTexOffset;
    float u2 = t.u2 + xTexOffset;
    float v2 = t.v2 + yTexOffset;
    
    glPushMatrix();
    glTranslatef(xOffset, yOffset, 0);
    glTranslatef(x,y,0);
    glRotatef(rotation, 0, 0, 1);
    float s = scale;
    if(mirror)
        s = -scale;
    glScalef(s, scale, 1);
    glTranslatef(-xOffset, -yOffset, 0);
    
    if(!glIsEnabled(GL_TEXTURE_2D))
        glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, texture );
    
    glBegin( GL_QUADS );

    //Bottom-left vertex (corner)
    glTexCoord2f( u1, v1 );
    glVertex3f( 0, 0, 0.0f );

    //Bottom-right vertex (corner)
    glTexCoord2f( u2, v1 );
    glVertex3f( width, 0, 0.f );

    //Top-right vertex (corner)
    glTexCoord2f( u2, v2 );
    glVertex3f( width, height, 0.f );

    //Top-left vertex (corner)
    glTexCoord2f( u1, v2 );
    glVertex3f( 0, height, 0.f );
    glEnd();
    
//#define SPRITE_DEBUG
#ifdef SPRITE_DEBUG
    glDisable(GL_TEXTURE_2D);
    glColor4f(1,0,0,0.3);
    glBegin( GL_QUADS );
    
    //Bottom-left vertex (corner)
    glTexCoord2f( u1, v1 );
    glVertex3f( 0, 0, 0.0f );
    
    //Bottom-right vertex (corner)
    glTexCoord2f( u2, v1 );
    glVertex3f( width, 0, 0.f );
    
    //Top-right vertex (corner)
    glTexCoord2f( u2, v2 );
    glVertex3f( width, height, 0.f );
    
    //Top-left vertex (corner)
    glTexCoord2f( u1, v2 );
    glVertex3f( 0, height, 0.f );
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glColor3f(1,1,1);
#endif
    
    glPopMatrix();
}

// Sets the mirroring of frames
// true - horizontally flips frames
void CMultiImage::setMirror(bool mirror)
{
	this->mirror = mirror;
}
