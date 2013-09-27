/*
 *  Physics.h
 *  Physics wrapper using Box2D
 *
 *  Created by Marcelo Cohen on 09/13.
 *  Copyright 2013 PUCRS. All rights reserved.
 *
 */

#include "Physics.h"
#include <iostream>
#define _USE_MATH_DEFINES 1
#include <math.h>

namespace cgf
{

using namespace std;

float Physics::CONV = 10;
const float Physics::timeStep = 1.0f / 30.f;
Physics Physics::m_Physics;

void Physics::setConvFactor(float conv)
{
    CONV = conv;
    cout << "Physics::setConvFactor" << conv<<endl;
}

Physics::Physics()
{
    // Init Box2D world
    b2Vec2 gravity(0,10.0f);
    world = new b2World(gravity);
    offsetX = offsetY = 0;
}

Physics::~Physics()
{
    cout << "CPhisics: cleanup" << endl;
    delete world;
}

// Add a new box and associate a Image to it
b2Body* Physics::newBoxImage(int id, Sprite* image, float density, float friction, float restitution, bool staticObj)
{
    b2BodyDef bd;
    if(!staticObj)
        bd.type = b2_dynamicBody;
    sf::Vector2f pos = image->getPosition();
    sf::Vector2f scale = image->getScale();
    sf::Vector2u size = image->getSize();
    float width = size.x*scale.x/CONV;
    float height = size.y*scale.y/CONV;

    cout << "Physics::newBoxImage " << pos.x << "," << pos.y << " - " << size.x << " x " << size.y << endl;

	b2PolygonShape box;
    //width /= 2;
    //height/= 2;
	box.SetAsBox(width/2,height/2);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;

    // Pinho
    b2Vec2 pos2;
    pos2.x = pos.x + width*CONV/2;
    pos2.y = pos.y + height*CONV/2;
    bd.position.Set(pos2.x/CONV, pos2.y/CONV);
    // End pinho
    //bd.position.Set(image->getX()/CONV,image->getY()/CONV);
	b2Body* body = world->CreateBody(&bd);

	body->CreateFixture(&fixtureDef);

	BodyData* bodyData = new BodyData;
    bodyData->id = id;
	bodyData->image = image;
	bodyData->color = b2Color(0,0,0);
    bodyData->size = b2Vec2(width*CONV,height*CONV);
    bodyData->offset = b2Vec2(width*CONV/2/scale.x,height*CONV/2/scale.y); //b2Vec2(35,60);//width*CONV,height*CONV);
	body->SetUserData(bodyData);

	return body;
}

// Add new box to world
b2Body* Physics::newBox(int id, float x, float y, float width, float height, float density, float friction, float restitution, bool staticObj)
{
    b2BodyDef bd;
    if(!staticObj)
        bd.type = b2_dynamicBody;

    cout << "Physics::newBox " << x << "," << y << " - " << width << " x " << height << endl;

	b2PolygonShape box;
    width = width/CONV;
    height = height/CONV;
	box.SetAsBox(width,height);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;

    bd.position.Set(x/CONV,y/CONV);
	b2Body* body = world->CreateBody(&bd);

	body->CreateFixture(&fixtureDef);

	BodyData* bodyData = new BodyData;
    bodyData->id = id;
	bodyData->image = NULL;
	bodyData->color = b2Color(0,0,0);
    bodyData->size = b2Vec2(width*2*CONV, height*2*CONV);
    bodyData->offset = b2Vec2(64,64); //b2Vec2(width*CONV/2, height*CONV/2);
    bodyData->offset = b2Vec2(width*CONV/2, height*CONV/2);
	body->SetUserData(bodyData);

	return body;
}

// Add a new circle and associate a Image to it
b2Body* Physics::newCircleImage(int id, Sprite* image, float density, float friction, float restitution, bool staticObj)
{
    b2BodyDef bd;
    if(!staticObj)
        bd.type = b2_dynamicBody;
    sf::Vector2f pos = image->getPosition();
    sf::Vector2f scale = image->getScale();
    sf::Vector2u size = image->getSize();
    float width = size.x*scale.x/CONV;
    float height = size.y*scale.y/CONV;
    
    // Assume radius as the longest dimension
    float radius = max(width,height);
    radius/=2;
    
    cout << "Physics::newCircleImage " << pos.x << "," << pos.y << " - " << radius << endl;
    
    b2CircleShape cs;
    cs.m_radius = radius;
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &cs;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;
  
    // Pinho 
    b2Vec2 pos2;
    pos2.x = pos.x + radius*CONV/2;
    pos2.y = pos.y + radius*CONV/2;
    bd.position.Set(pos2.x/CONV, pos2.y/CONV);
    // End pinho
    //bd.position.Set(image->getX()/CONV,image->getY()/CONV);
	b2Body* body = world->CreateBody(&bd);
    
	body->CreateFixture(&fixtureDef);
    
	BodyData* bodyData = new BodyData;
    bodyData->id = id;
	bodyData->image = image;
	bodyData->color = b2Color(0,0,0);
    bodyData->size = b2Vec2(radius,radius);
    bodyData->offset = b2Vec2(radius*CONV/scale.x,radius*CONV/scale.y);
	body->SetUserData(bodyData);
    
	return body;
}

// Add new circle to world
b2Body* Physics::newCircle(int id, float x, float y, float radius, float density, float friction, float restitution, bool staticObj)
{
    b2BodyDef bd;
    bd.type = b2_dynamicBody;

    b2CircleShape cs;
    cs.m_radius = radius/CONV;
    bd.position.Set(x/CONV,y/CONV);
	//cs.m_p.Set(0,0);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &cs;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;

	b2Body* body = world->CreateBody(&bd);
	body->CreateFixture(&fixtureDef);

	BodyData* bodyData = new BodyData;
    bodyData->id = id;
	bodyData->image = NULL;
	bodyData->color = b2Color(0,0,0);
    //bodyData->size = b2Vec2(width*2*CONV, height*2*CONV);
    //bodyData->offset = b2Vec2(width*CONV/2, height*CONV/2);
	body->SetUserData(bodyData);

    return body;
}

void Physics::setImage(b2Body* body, Sprite* Image)
{
    body->SetUserData(Image);
}

Sprite* Physics::getImage(b2Body* body)
{
    return (Sprite*) body->GetUserData();
}

void Physics::setColor(b2Body* body, const b2Color& cor)
{
    BodyData* bd = (BodyData*) body->GetUserData();
    bd->color = cor;
}

b2Color& Physics::getColor(b2Body* body)
{
    BodyData* bd = (BodyData*) body->GetUserData();
    return bd->color;
}

void Physics::destroy(b2Body* bptr)
{
    world->DestroyBody(bptr);
}

void Physics::step()
{
    world->Step(timeStep, velocityIterations, positionIterations);
    world->ClearForces();
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        BodyData* ptr = (BodyData*) b->GetUserData();
        if(ptr->image != NULL) {
            const b2Vec2& pos = b->GetPosition();
            float rot = b->GetAngle();
            rot = rot * 180/M_PI;
            float dx = 0;
            float dy = 0;
            if(ptr->offset.x != offsetX)
            {
//                cout << ptr->offset.x << " -> " << offsetX << endl;
                dx = offsetX - ptr->offset.x;
                dy = offsetY - ptr->offset.y;
            }
            ptr->image->setPosition(pos.x*CONV+dx, pos.y*CONV+dy);
            ptr->image->setRotation(rot);
        }
    }

    /*
    for (b2Contact* c = world->GetContactList(); c; c = c->GetNext())
    {
        // process c
        b2Fixture* a = c->GetFixtureA();
        b2Fixture* b = c->GetFixtureB();
        b2Body* ba = a->GetBody();
        b2Body* bb = b->GetBody();
        BodyData* bda = (BodyData*) ba->GetUserData();
        BodyData* bdb = (BodyData*) bb->GetUserData();
        b2Vec2 posa = ba->GetPosition();
        b2Vec2 posb = bb->GetPosition();
        cout <<  "Contact: " << bda->id << " with " << bdb->id << " -> " << posa.x << "," << posa.y << " with "
             << posb.x << "," << posb.y << endl;
    }
    */
}

// Process contact list for this time step, and return ptr of body
// with id2, if there was a contact between body id1 and id2
b2Body* Physics::haveContact(int id1, int id2)
{
    for (b2Contact* c = world->GetContactList(); c; c = c->GetNext())
    {
        // process c
        b2Fixture* a = c->GetFixtureA();
        b2Fixture* b = c->GetFixtureB();
        b2Body* ba = a->GetBody();
        b2Body* bb = b->GetBody();
        BodyData* bda = (BodyData*) ba->GetUserData();
        BodyData* bdb = (BodyData*) bb->GetUserData();
     //   b2Vec2 posa = ba->GetPosition();
     //   b2Vec2 posb = bb->GetPosition();
     //   cout <<  "Contact: " << bda->id << " with " << bdb->id << " -> " << posa.x << "," << posa.y << " with "
     //   << posb.x << "," << posb.y << endl;
        if(bda->id==id1 && bdb->id==id2)
            return bb;
        else if(bda->id==id2 && bdb->id==id1)
            return ba;
    }
    return NULL;
}

// Draw all physics objects with transparency
void Physics::debugDraw()
{
    sf::CircleShape circle;
    sf::RectangleShape rectangle;

    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        BodyData* ptr = (BodyData*) b->GetUserData();
        if(ptr->image == NULL || 1) {
            const b2Vec2& pos = b->GetPosition();
            float rot = b->GetAngle();
            rot = rot * 180/M_PI;
            //cout << "Box: " << pos.x << " " << pos.y << " - " << rot;
            //glColor4f(ptr->color.r, ptr->color.g, ptr->color.b, 0.2);
            // Iterate through all fixtures
            b2Fixture* f = b->GetFixtureList();
            float tx = pos.x * CONV;
            float ty = pos.y * CONV;
            //cout << " " << ptr->offset.x << " " << ptr->offset.y << endl;
            //tx += 64;//ptr->offset.x;
            //ty += 64;//ptr->offset.y;
            //tx += ptr->offset.x;
            //ty += ptr->offset.y;
            tx += offsetX;
            ty += offsetY;
            /*
            if(ptr->image != NULL)
            {
                tx += ptr->image->getXOffset();
                ty += ptr->image->getYOffset();
            }
            else {
                tx += offsetX;
                ty += offsetY;
            }
            */
            //glPushMatrix();
            //glColor4f(1,1,1,0.4);
            //glTranslatef(tx, ty, 0);
            //glRotatef(rot, 0, 0, 1);
//#define NEWAPPROACH
#ifdef NEWAPPROACH
            glBegin(GL_LINE_LOOP);
            glVertex2f(-ptr->size.x/2,-ptr->size.y/2);
            glVertex2f(-ptr->size.x/2, ptr->size.y/2);
            glVertex2f( ptr->size.x/2, ptr->size.y/2);
            glVertex2f( ptr->size.x/2,-ptr->size.y/2);
            glEnd();
            glPopMatrix();
#else
            b2PolygonShape* pol;
            b2CircleShape* circ;            
            while(f != NULL)
            {
                switch(f->GetType())
                {
                    case b2Shape::e_polygon:
                        pol = (b2PolygonShape*) f->GetShape();

                        cout << "b2Shape::poly" << endl;

                        /*glBegin(GL_POLYGON);
                          for(int i=0; i<pol->GetVertexCount(); i++)
                          {
                              const b2Vec2& v = pol->GetVertex(i);
                              //cout << v.x << "," << v.y << " ";
                              glVertex2f(v.x*CONV,v.y*CONV);
                          }
                        //cout << endl;
                        glEnd();*/
                        break;
                    case b2Shape::e_circle:
                        circ = (b2CircleShape*) f->GetShape();
                        float radius = circ->m_radius;
                        cout << "b2Shape::circle" << endl;
                        //cout << "drawing circle " << radius << endl;
                        /*glBegin(GL_TRIANGLE_FAN);
                          glVertex2f(0,0);
                          for(float a=0; a<=2*M_PI; a+=0.6)
                          {
                              float px = cos(a) * radius;
                              float py = sin(a) * radius;
                              //cout << px << " " << py << endl;
                              glVertex2f(px*CONV,py*CONV);
                          }
                        //cout << endl;
                        glEnd();*/
                }
                f = f->GetNext();
                //glPopMatrix();
            }
#endif
        }
    }
    //glColor3f(1,1,1);
}

void Physics::setGravity(float grav)
{
    gravity = grav;
    world->SetGravity(b2Vec2(0,grav));
}

float Physics::getGravity()
{
    return gravity;
}

void Physics::setPosition(b2Body* body, const b2Vec2& pos)
{
    float angle = body->GetAngle();
    b2Vec2 p = pos;
    p.x = p.x/CONV;
    p.y = p.y/CONV;
    body->SetTransform(pos, angle);
}

b2Vec2 Physics::getPosition(b2Body* body)
{
   b2Vec2 pos=body->GetPosition();
   pos.x = pos.x*CONV;
   pos.y = pos.y*CONV;
   return pos;
}

void Physics::setAngle(b2Body* body, float angle)
{
    const b2Vec2& pos = body->GetPosition();
    body->SetTransform(pos, angle*M_PI/180.0);
}

void Physics::setDrawOffset(float ox, float oy)
{
    offsetX = ox;
    offsetY = oy;
}

} // namespace cgf
