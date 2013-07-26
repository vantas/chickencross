#ifndef TMXLOADER_H
#define TMXLOADER_H

#include "../CMultiImage.h"

#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "base64.h"
#include "tinyxml.h"

using namespace std;

struct Object
{
    int x, y, width, height;
    string name;
};

struct ImageObject
{
    int layer;
    CImage* img;
};

struct Tileset
{
    CMultiImage* img;
    int tileWidth, tileHeight;
    int tileRows, tileCols;
    int tileSpacing, tileMargin;
    int tileFirst;
};

struct Cell
{
    unsigned char tileset;
    unsigned tileid;
};

struct Layer
{
    Cell** layerData;
    bool visible;
    double opacity;
};

class CGame;

class TMXLoader : public TiXmlVisitor
{
    public:
        TMXLoader();
        virtual ~TMXLoader();

        void cleanup();

        virtual bool 	VisitEnter  (const TiXmlDocument  &);
        virtual bool 	VisitExit (const TiXmlDocument &);
        virtual bool 	VisitEnter (const TiXmlElement &, const TiXmlAttribute *);
        virtual bool 	VisitExit (const TiXmlElement &);
        virtual bool 	Visit (const TiXmlDeclaration &);
        virtual bool 	Visit (const TiXmlText &);
        virtual bool 	Visit (const TiXmlComment &);
        virtual bool 	Visit (const TiXmlUnknown &);

        bool loadMap(char* mapName);

        CMultiImage* getTilesetImage(int p) { return tilesets[p].img; }
        //SDL_Surface* getMapImage();

        int getTileWidth(int t=0) { return tilesets[t].tileWidth; }
        int getTileHeight(int t=0) { return tilesets[t].tileHeight; }
        int getTileSpacing(int t=0) { return tilesets[t].tileSpacing; }
        int getTilesetMargin(int t=0) { return tilesets[t].tileMargin; }
        int getNumMapColumns() { return m_NumMapColumns; }
        int getNumMapRows() { return m_NumMapRows; }

        void draw(CGame* game);

        // Return center of cell in world units
        void getCenter(int col, int row, float& x, float& y);
        // Return specific cell
        int getCell(int col, int row, int layer=0);
        int getCellFromWorld(float x, float y);
        // Convert from world to cell coords
        void convertWorldToCell(float x, float y, int&col, int& row);

		int totalObjects() { return m_Objects.size(); }
		Object& getObject(int o) { return m_Objects[o]; }

        // Methods for layered drawing of image objects
		void addImageObject(CImage* img, int layer=0);
		int totalImageObjects() { return m_Images.size(); }
		ImageObject& getImageObject(int o) { return m_Images[o]; }
		void deleteObject(int o) { m_Images.erase(m_Images.begin()+o); }

    protected:

    private:

        string mapPath; // path to map (used to load tilesets)

        int m_NumMapColumns;
        int m_NumMapRows;
        double m_Opacity; // temp to store layer opacity (while reading map file)
        string m_LayerName; // temp to store layer name (same)

        vector<Tileset> tilesets;

        vector<Layer> layers;
		vector<Object> m_Objects;
		vector<ImageObject> m_Images;   // images to draw

        void decode_and_store_map_data( string encoded_data );
        //TiXmlDocument *doc;

        // Bits on the far end of the 32-bit global tile ID are used for tile flags
        static const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
        static const unsigned FLIPPED_VERTICALLY_FLAG   = 0x40000000;
        static const unsigned FLIPPED_DIAGONALLY_FLAG   = 0x20000000;
};

#endif // TMXLOADER_H
