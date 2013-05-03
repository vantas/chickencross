#include "TMXLoader.h"
#include <iomanip>
#include <cmath>

using namespace std;

TMXLoader::TMXLoader()
{
    //img_tileset = new CMultiImage();
}

TMXLoader::~TMXLoader()
{
    //dtor
    cleanup();
}

void TMXLoader::cleanup()
{
    //Free the image(s)
    cout << "Freeing tilesets" << endl;
    for(int i=0; i<tilesets.size(); i++)
        delete tilesets[i].img;
    cout << "Freeing map data" << endl;
    // Free the map
    for(int layer=0; layer<layers.size(); layer++)
    {
        for(int i=0; i<m_NumMapRows; i++)
            delete [] layers[layer].layerData[i];
        delete layers[layer].layerData;
    }
}

bool 	TMXLoader::VisitEnter  (const TiXmlDocument  &doc)
{
    return true; //TODO: for performance, we may not want to return true for each of these callbacks for the visitor pattern.
}

bool 	TMXLoader::VisitExit (const TiXmlDocument &doc)
{
    return true;
}

bool 	TMXLoader::VisitEnter (const TiXmlElement &elem, const TiXmlAttribute *attrib)
{
    //cout << ">>>" << elem.Value() << endl;
	if (string("map") == elem.Value()) {
        elem.Attribute("width", &m_NumMapColumns);
        elem.Attribute("height", &m_NumMapRows);

        //elem.Attribute("tileheight");
        //elem.Attribute("tilewidth");

        //TODO: get width and height, and tilewidth and tileheight
        //m_layer_width =

    }
    else if (string("tileset") == elem.Value()) {

        // Need a new class called Tileset

        Tileset aux;
        elem.Attribute("tilewidth", &aux.tileWidth);
        elem.Attribute("tileheight", &aux.tileHeight);
        elem.Attribute("spacing", &aux.tileSpacing);
        elem.Attribute("margin", &aux.tileMargin);
        elem.Attribute("firstgid", &aux.tileFirst);
        // we assume that a <image> tag will follow with
        // the tileset image spec
        aux.img = new CMultiImage();
        tilesets.push_back(aux);

        cout << "First GID: " << aux.tileFirst << endl;
    }
    else if (string("image") == elem.Value()) {
        //string attrib = attrib.ValueStr();
        string attrib = elem.Attribute("source");
        attrib = mapPath+"/"+attrib;

        int img_width, img_height;
        elem.Attribute("width", &img_width);
        elem.Attribute("height", &img_height);
        Tileset& aux = tilesets.back();

        aux.tileRows = img_height / (aux.tileHeight + aux.tileSpacing);
        aux.tileCols = img_width  / (aux.tileWidth  + aux.tileSpacing);

        cout << "tileset: " << aux.tileRows << " x " << aux.tileCols << endl;
        cout << "spacing: " << aux.tileSpacing << endl;
        CMultiImage* auximg = new CMultiImage();
        auximg->loadMultiImage((char*)attrib.c_str(), aux.tileWidth, aux.tileHeight, aux.tileSpacing,
            aux.tileSpacing, aux.tileMargin, aux.tileMargin, aux.tileCols, aux.tileRows, aux.tileCols*aux.tileRows);
        tilesets.back().img = auximg;
//        tilesets.push_back(auximg);
    }
    else if (string("layer") == elem.Value()) {
        // We don't need to use layer width and height yet.

        //elem.Attribute("width");
        //elem.Attribute("height");
//        m_Opacity = 1.0;
        const char* tmpLayerName = elem.Attribute("name");
        m_LayerName = tmpLayerName;
        if(elem.QueryDoubleAttribute("opacity", &m_Opacity) == TIXML_NO_ATTRIBUTE)
            m_Opacity = 1.0;
//        elem.Attribute("opacity", &m_Opacity);
    }
    else if (string("data") == elem.Value()) {
        const char* text = elem.GetText();
        decode_and_store_map_data( text );
    }
	else if (string("objectgroup") == elem.Value()) {
		string groupName = elem.Attribute("name");
		cout << "TMXLoader::VisitEnter -> " << groupName << endl;
	}
	else if (string("object") == elem.Value()) {
		Object newObj;
		int x, y, width, height;
		elem.Attribute("x", &x);
		elem.Attribute("y", &y);
		elem.Attribute("width", &width);
		elem.Attribute("height",&height);
        	const char* pname = elem.Attribute("name");
        	if(pname != NULL)
            		newObj.name = pname;
		// Compute center from corners and sizes
		int cx = x+width/2;
		int cy = y+height/2;
		//cout << "center: "<< cx << " " << cy << endl;
		Tileset aux = tilesets[0];
		newObj.x = cx - aux.tileWidth/2;
		newObj.y = cy - aux.tileHeight/2;
		newObj.width = width/2;
		newObj.height = height/2;
		m_Objects.push_back(newObj);
		// cout << "Total objs: " << m_Objects.size() << endl;
	}

    return true;
}

 bool 	TMXLoader::VisitExit (const TiXmlElement &elem)
{
    return true;
}

 bool 	TMXLoader::Visit (const TiXmlDeclaration &dec)
{
    return true;
}
 bool 	TMXLoader::Visit (const TiXmlText &text)
{
    return true;
}

 bool 	TMXLoader::Visit (const TiXmlComment &comment)
{
    return true;
}

 bool 	TMXLoader::Visit (const TiXmlUnknown &unknown)
{
    return true;
}

bool TMXLoader::loadMap(char* mapName)
{
    TiXmlDocument doc(mapName);

    mapPath = mapName;

    int pos = mapPath.find_last_of("/");
    mapPath = mapPath.substr(0, pos);
    cout << "Map path: " << mapPath << endl;

    if ( ! doc.LoadFile() ) {
		return false;
	}

    //TiXmlElement* elem = doc.RootElement();

    doc.Accept(this);

    return true;
}

void TMXLoader::decode_and_store_map_data(string encoded_data)
{
//    vector< Cell > layerDataRow( getNumMapColumns() );
    int m_LayerRow = 0;
    int m_LayerCol = 0;

    vector<int> unencoded_data = base64_decode(encoded_data);

    Cell** tempLayer = new Cell*[m_NumMapRows];
    for (unsigned int i = 0; i < m_NumMapRows; i++)
    {
        tempLayer[i] = new Cell[m_NumMapColumns];
    }

    for (unsigned int i = 0; i < unencoded_data.size(); i += 4)
    {
        // Get the grid ID

        int a = unencoded_data[i];
        int b = unencoded_data[i + 1];
        int c = unencoded_data[i + 2];
        int d = unencoded_data[i + 3];

        unsigned gid = a | b << 8 | c << 16 | d << 24;

        bool flipped_horizontally = (gid & FLIPPED_HORIZONTALLY_FLAG);
        bool flipped_vertically = (gid & FLIPPED_VERTICALLY_FLAG);
        bool flipped_diagonally = (gid & FLIPPED_DIAGONALLY_FLAG);

        // Clear the flags
        gid &= ~(FLIPPED_HORIZONTALLY_FLAG |
                 FLIPPED_VERTICALLY_FLAG |
                 FLIPPED_DIAGONALLY_FLAG);

        // Resolve the tile
        int tn = 0;
        for (int i = tilesets.size()-1; i>=0; --i) {
            if (tilesets[i].tileFirst <= gid) {
                gid -= tilesets[i].tileFirst;
                tn = i;
                //tiles[y][x] = tileset->tileAt(global_tile_id - tileset->first_gid());
                break;
            }
        }
        tempLayer[m_LayerRow][m_LayerCol].tileset = tn;
        tempLayer[m_LayerRow][m_LayerCol].tileid = gid;

        if ((i + 4) % ( getNumMapColumns() * 4) == 0) {
            m_LayerRow++;
            m_LayerCol = 0;
        }
        else {
            m_LayerCol++;
        }
    }
    Layer newLayer;
    newLayer.layerData = tempLayer;
    newLayer.opacity = m_Opacity;
    newLayer.visible = (m_LayerName != "collision");
    layers.push_back(newLayer);

    // DEBUG: output map to console
    #define LAYERDEBUG
    #ifdef LAYERDEBUG
    cout << "LAYER " << layers.size()-1 << endl;
    for (int row = 0; row < m_NumMapRows; row++)
    {
        for (int col = 0; col < m_NumMapColumns; col++)
        {
           cout << hex << setw(2) << setfill('0') << layers[layers.size()-1].layerData[row][col].tileid << " ";
        }
        cout << endl;
    }
    cout << dec;
    #endif
}

void TMXLoader::draw()
{
    float origRot = tilesets[0].img->getRotation();
    float origScale = tilesets[0].img->getScale();
    float x = tilesets[0].img->getX();
    float y = tilesets[0].img->getY();
    tilesets[0].img->setRotation(0);
    tilesets[0].img->setScale(1);
	float dx = 0;//0.5/m_TileWidth;
	float dy = 0;//0.5/m_TileHeight;
	float floatCol, floatRow;
    //cout << "TMXLoader::draw " << getNumMapRows() << " x " << getNumMapColumns() << endl;
    int tn;
    for(int layer=0; layer<layers.size(); layer++)
    {
        //cout << "Drawing layer " << layer << endl;
        if(!layers[layer].visible)
            continue;
        floatRow = 0;
        for (int row = 0; row < getNumMapRows(); row++)
        {
            floatCol =0;
            for (int col = 0; col < getNumMapColumns(); col++)
            {
                int gid = layers[layer].layerData[row][col].tileid;

                if (gid == 0 && layer) {
                    floatCol += tilesets[0].tileWidth + dx;
                    continue;
                }

                //cout << "\nGID is " << gid;

                tn = layers[layer].layerData[row][col].tileset;

                //gid -= tilesets[tn].tileFirst;

                int tileset_col;
                int tileset_row;

                //gid--; // convert to something we can use with the 0 based system.

                tileset_col = gid % tilesets[tn].tileCols;
                tileset_row = gid / tilesets[tn].tileCols;

                int tileNum = tileset_row*tilesets[tn].tileCols + tileset_col;

                //if(tn == 0 && tileNum !=0) cout << "Tileset 1: " << gid << " " << floatCol << " " << floatRow << endl;

                tilesets[tn].img->setPosition(floatCol, floatRow);
                tilesets[tn].img->setRotation(0);
                tilesets[tn].img->setScale(1);
                //img_tileset->setPosition(col * getTileWidth(), row * getTileHeight());
                glColor4f(1,1,1,layers[layer].opacity);
                tilesets[tn].img->drawFrame(tileNum);
                floatCol += tilesets[tn].tileWidth + dx;
                /*
                srcrect.x = getTilesetMargin() + (getTileSpacing() + 32) * tileset_col;
                srcrect.y = getTilesetMargin() + (getTileSpacing() + 32) * tileset_row;
                srcrect.w = getTileWidth();
                srcrect.h = getTileHeight();

                dstrect.x = col * getTileWidth();
                dstrect.y = row * getTileHeight();
                dstrect.w = getTileWidth();
                dstrect.h = getTileHeight();

    //            SDL_BlitSurface(img_tileset, &srcrect, img_map, &dstrect);
    */
            }
            floatRow += tilesets[tn].tileHeight + dy;
        }
        // Draw this layer's image objects
        for(int o=0; o<m_Images.size(); o++)
            if(m_Images[o].layer == layer)
                m_Images[o].img->draw();
    }
    tilesets[0].img->setX(x);
    tilesets[0].img->setY(y);
    tilesets[0].img->setRotation(origRot);
    tilesets[0].img->setScale(origScale);
    glColor4f(1,1,1,1);
}

void TMXLoader::getCenter(int col, int row, float& x, float& y)
{
    x = col * tilesets[0].tileWidth;// + m_TileWidth/2;
    y = row * tilesets[0].tileHeight;// + m_TileHeight/2;
}

int TMXLoader::getCell(int col, int row, int layer)
{
    int gid = layers[layer].layerData[row][col].tileid;

    //if (gid == 0)
    //    return -1;

    int tileset_col;
    int tileset_row;

    //gid--; // convert to something we can use with the 0 based system.

    tileset_col = gid % tilesets[0].tileRows;
    tileset_row = gid / tilesets[0].tileCols;

    return tileset_row*tilesets[0].tileCols + tileset_col;
}

int TMXLoader::getCellFromWorld(float x, float y)
{
    int col, row;
    convertWorldToCell(x,y,col,row);
    return getCell(col, row);
}

void TMXLoader::convertWorldToCell(float x, float y, int&col, int& row)
{
    col = floor(x / tilesets[0].tileWidth);
    row = floor(y / tilesets[0].tileHeight);
}

void TMXLoader::addImageObject(CImage* img, int layer)
{
    m_Images.push_back(ImageObject { layer, img });
}

/*
SDL_Surface* TMXLoader::getMapImage()
{
    return img_map;
}
*/
