/* Design Implementation - Application Layer
 *
 * Design.cpp
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Design.h"          // for the Design class definition
#include "iText.h"           // for the Text Interface
#include "iHUD.h"            // for the HUD Interface
#include "iSound.h"          // for the Sound Interface
#include "iLight.h"          // for the Light Interface
#include "iObject.h"         // for the Object Interface
#include "iTexture.h"        // for the Texture Interface
#include "Camera.h"          // for the Camera Interface
#include "iGraphic.h"        // for the Graphic Interface
#include "iUtilities.h"      // for strcpy()
#include "MathDefinitions.h" // for MODEL_Z_AXIS
#include "ModellingLayer.h"  // for MOUSE_BUTTON_SCALE, ROLL_SPEED
#include "Common_Symbols.h"  // for Action and Sound enumerations
#include "Physics.h"
#include "CSphere.h"
#include "iSimpleCollisionSpace.h"

#include <strsafe.h>
const wchar_t* orient(wchar_t*, const iFrame*, char, unsigned = 1u);
const wchar_t* position(wchar_t*, const iFrame*, char = ' ', unsigned = 1u);
const wchar_t* onOff(wchar_t*, const iSwitch*);
//const Colour red(1,0,0);
//-------------------------------- Design -------------------------------------
//
// The Design class implements the game design within the Modelling Layer
//
// constructor initializes the engine and the instance pointers
//
Design::Design(void* h, int s) : Coordinator(h, s) {

	cs_=CreateSimpleCollisionSpace(60);
	CollisionGeometry::setGlobalCollisionSpace(cs_);
}

// initialize initializes the general display design coordinator, creates the 
// primitive sets, textures, objects, lights, sounds, cameras, and text items
//
void Design::initialize() {

       // general display design
    //
	Reflectivity redish = Reflectivity(red);
	Reflectivity greenish = Reflectivity(green);
	Reflectivity bluish = Reflectivity(blue);
	Reflectivity whitish = Reflectivity(white);
    setProjection(0.9f, 1.0f, 1000.0f);
    setAmbientLight(0.9f, 0.9f, 0.9f);
	// camera at a distance - in lhs coordinates
    // camera at a distance - in lhs coordinates
    iCamera* camera = CreateCamera();
    camera->translate(0,250,-250);
    camera->setRadius(17.8f);
	camera->rotatex(3.1459/4.0f);
	platform_=CreatePhysicsBox(-20,-20,-20,20,20,20,&bluish,8);
	cannon_=CreatePhysicsBox(-5,0,-5,5,40,5,&redish,1000);
	cannon_->attachTo(platform_,0,0);
	cannon_->translate(0,20,0);
	cannonball_=CreatePhysicsBox(-2.5,-2.5,-2.5,2.5,2.5,2.5,&whitish,1);
 	cannonball_->translate(0,60,0);
	cannonball_->attachTo(cannon_,0,0);

	bigGreenBox_=CreatePhysicsBox(-200,-5,-100,0,5,100,&greenish,8,true);
	bigBlueBox_=CreatePhysicsBox(-100,10,-100,-90,110,100,&bluish,8,true);
	//bigGreenBox_->translate(-100,0,0);
	//bigBlueBox_->translate(-100,150,0);

	lastfired_=now;
    lastUpdate = now;	
//	abox->setVelocity(Vector(0,40,0));
//	abox->setAcceleration(Vector(0,-9.8,0));

//	abox->translate(0,200,0);
	hud = CreateHUD(0.72f, 0.01f, 0.27f, 0.99f, CreateTexture(HUD_IMAGE));
	//setTimerText(CreateText(Rectf(0.0f, 0.05f, 0.2f, 0.15f), hud, L"", 
    //    TEXT_HEIGHT, TEXT_TYPEFACE, TEXT_LEFT));

    // cameras ----------------------------------------------------------------



	
	
	velocitytxt_=CreateText(Rectf(0.05f,0.27f,0.95f,0.37f),hud,L"",TEXT_HEIGHT+5,TEXT_TYPEFACE,TEXT_LEFT);
	deltatxt_=CreateText(Rectf(0.05f,0.17f,0.95f,0.27f),hud,L"",TEXT_HEIGHT,TEXT_TYPEFACE,TEXT_LEFT);
	positiontxt_=CreateText(Rectf(0.05f,0.38f,0.95f,0.48f),hud,L"",TEXT_HEIGHT,TEXT_TYPEFACE,TEXT_LEFT);

	iGraphic* grid=CreateGrid(-150, 150, 20);
    CreateObject(grid)->translate(-60,0,0);
	lasttextupdate=now;
	

}

// update updates the position and orientation of each object according to the 
// actions initiated by the user
//
void Design::update() {

    int delta;
    int dr = 0;  // roll the right box around its x axis
    int ds = 0;  // spin the right box around its y axis
    int dt = 0;  // roll the top   box around its z axis
    int dw = 0;  // roll the spot light around the x axis
	float vrn, J;
	Vector force,n,relativeVelocity;
	Vector g1deltap, g2deltap;
	const CollisionContact* cc;
   delta  = now - lastUpdate;
   if(pressed(FIRE_CANNONBALL)&& (now-lastfired_)>UNITS_PER_SEC/2.0f){
	    lastfired_=now;
	    iPhysics* nf=cannonball_->clone();
		nf->attachTo(NULL,100,0);
		flyinglist_.insertAtFront(nf);
		nf->setCollision(CreateCSphere(nf,2.5));
   }
   if(pressed(CANNON_ROTATE_LEFT)){
	   cannon_->rotate(Vector(0,0,1),(delta/1000.0f)*(3.14159f/4.0f));
   }
   if(pressed(CANNON_ROTATE_RIGHT)){
	   cannon_->rotate(Vector(0,0,-1),(delta/1000.0f)*(3.14159f/4.0f));
   }
   if(pressed(CANNON_ROTATE_FWD)){
	   cannon_->rotate(Vector(1,0,0),(delta/1000.0f)*(3.14159f/4.0f));
   }
   if(pressed(CANNON_ROTATE_BACK)){
	   cannon_->rotate(Vector(-1,0,0),(delta/1000.0f)*(3.14159f/4.0f));
   }
   if(pressed(CLOCKWISE_MOMENT)){
	   platform_->addMoment(Vector(0,2000,0));
   }
   if(pressed(COUNTERCLOCKWISE_MOMENT)){
	   platform_->addMoment(Vector(0,-2000,0));
   }
   cs_->populateContactList(delta/UNITS_PER_SEC);
   int nc=cs_->getNumContacts();
   if(nc!=0){
    	   cc=cs_->getContactList();
	   if(cc->g1->getPhysics()!=bigBlueBox_ && cc->g1->getPhysics()!=bigGreenBox_){
    		  n=cc->normal;

		       
		 relativeVelocity = cc->g1->getPhysics()->velocity() - cc->g2->getPhysics()->velocity();

	    // normal component of the relative velocity
		 vrn = dot(relativeVelocity, n);

	    // magnitude of the impulse at collision
		 J = - vrn * 2.0f / (1.0f / cc->g1->getPhysics()->mass() + 1.0f / cc->g2->getPhysics()->mass());


	    // force generated by the impulse
	    force = J * n / (float(delta)/float(UNITS_PER_SEC));

		 // apply the force to both objects

		cc->g1->getPhysics()->addimpulseForce(force);
		cc->g2->getPhysics()->addimpulseForce(-1*force);

		//push objects apart so that it doesn't keep colliding
        float massTotal = (cc->g1->getPhysics()->mass() + cc->g2->getPhysics()->mass()) * 0.85f;
        float rbd1mf = cc->g2->getPhysics()->mass() / massTotal;
        float rbd2mf = cc->g1->getPhysics()->mass() / massTotal;
        Vector g1deltap (-1*(rbd1mf * cc->depth * n));
        Vector g2deltap (rbd2mf * cc->depth * n);
 		cc->g1->getPhysics()->translate(g1deltap.x,g1deltap.y,g1deltap.z);
 		cc->g2->getPhysics()->translate(g2deltap.x,g2deltap.y,g2deltap.z);
	   }
	   if(cc->g2->getPhysics()!=bigBlueBox_ && cc->g2->getPhysics()!=bigGreenBox_){
 		  n=cc->normal;

		       
		 relativeVelocity = cc->g1->getPhysics()->velocity() - cc->g2->getPhysics()->velocity();

	    // normal component of the relative velocity
		 vrn = dot(relativeVelocity, n);

	    // magnitude of the impulse at collision
		 J = - vrn * 2.0f / (1.0f / cc->g1->getPhysics()->mass() + 1.0f / cc->g2->getPhysics()->mass());


	    // force generated by the impulse
	    force = J * n / (float(delta)/float(UNITS_PER_SEC));

		 // apply the force to both objects
		cc->g1->getPhysics()->addimpulseForce(force);
		cc->g2->getPhysics()->addimpulseForce(-1*force);

		//push objects apart
        float massTotal = (cc->g1->getPhysics()->mass() + cc->g2->getPhysics()->mass()) * 0.85f;
        float rbd1mf = cc->g2->getPhysics()->mass() / massTotal;
        float rbd2mf = cc->g1->getPhysics()->mass() / massTotal;
		g1deltap=Vector (-1*(rbd1mf * cc->depth * n));
        g2deltap=Vector (rbd2mf * cc->depth * n);
 		cc->g1->getPhysics()->translate(g1deltap.x,g1deltap.y,g1deltap.z);
 		cc->g2->getPhysics()->translate(g2deltap.x,g2deltap.y,g2deltap.z);

	   }
   }
   cannonball_->update(delta);
   cannon_->update(delta);
   platform_->update(delta);
   flyinglist_.goStart();
   while(flyinglist_.curr()){
	   iPhysics* cb=flyinglist_.curr()->data();
 	   cb->update(delta);
	   if(cb->position().y < -3){
		   cs_->remove(cb->collisionGeometry());
		   flyinglist_.removeCurrent();
		   delete cb;
	   }
	   else{
		   flyinglist_.goNext();
	   }
   }
 
}
