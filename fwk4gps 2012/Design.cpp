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
#include <strsafe.h>
const wchar_t* orient(wchar_t*, const iFrame*, char, unsigned = 1u);
const wchar_t* position(wchar_t*, const iFrame*, char = ' ', unsigned = 1u);
const wchar_t* onOff(wchar_t*, const iSwitch*);
const Colour red(1,0,0);
//-------------------------------- Design -------------------------------------
//
// The Design class implements the game design within the Modelling Layer
//
// constructor initializes the engine and the instance pointers
//
Design::Design(void* h, int s) : Coordinator(h, s) {


}

// initialize initializes the general display design coordinator, creates the 
// primitive sets, textures, objects, lights, sounds, cameras, and text items
//
void Design::initialize() {

    // general display design
    //
	Reflectivity redish = Reflectivity(red);
    setProjection(0.9f, 1.0f, 1000.0f);
    setAmbientLight(0.9f, 0.9f, 0.9f);
	iGraphic* box=CreateBox(5,5,5,-5,-5,-5);
    iObject* b=CreateObject(box);
	 b->translate(0,0,50);
	 box=CreateBox(5,5,5,-5,-5,-5);
	 b=CreateObject(box);
	 b->translate(20,60,60);
	 box=CreateBox(5,5,5,-5,-5,-5);
	 b=CreateObject(box,&redish);
	 b->translate(-20,40,60);
	abox = CreatePhysicsBox(1,1,1,-1,-1,-1);
	anotherbox = CreatePhysicsBox(2,2,2,-2,-2,-2);
	anotherbox->attachTo(abox,0,0);
	anotherbox->translate(0,5,0);
//	abox->setVelocity(Vector(0,40,0));
//	abox->setAcceleration(Vector(0,-9.8,0));
	abox->setAngularVelocity(Vector(1,0,1));
//	abox->translate(0,200,0);
	hud = CreateHUD(0.72f, 0.01f, 0.27f, 0.99f, CreateTexture(HUD_IMAGE));
	//setTimerText(CreateText(Rectf(0.0f, 0.05f, 0.2f, 0.15f), hud, L"", 
    //    TEXT_HEIGHT, TEXT_TYPEFACE, TEXT_LEFT));

    // cameras ----------------------------------------------------------------

    // camera at a distance - in lhs coordinates
    iCamera* camera = CreateCamera();
    camera->translate(0,60,-60);
    camera->setRadius(17.8f);
	camera->rotatex(3.1459/8);

	
	
	velocitytxt_=CreateText(Rectf(0.05f,0.27f,0.95f,0.37f),hud,L"",TEXT_HEIGHT+5,TEXT_TYPEFACE,TEXT_LEFT);
	deltatxt_=CreateText(Rectf(0.05f,0.17f,0.95f,0.27f),hud,L"",TEXT_HEIGHT,TEXT_TYPEFACE,TEXT_LEFT);
	positiontxt_=CreateText(Rectf(0.05f,0.38f,0.95f,0.48f),hud,L"",TEXT_HEIGHT,TEXT_TYPEFACE,TEXT_LEFT);

	iGraphic* grid=CreateGrid(-150, 150, 20);
    CreateObject(grid)->translate(0,0,0);
	lasttextupdate=now;
	

}

// update updates the position and orientation of each object according to the 
// actions initiated by the user
//
void Design::update() {
	int delta = now - lastUpdate;
	wchar_t str[100];
	abox->update(delta);
	anotherbox->update(delta);
	if(now-lasttextupdate>2000){
		lasttextupdate=now;
		StringCbPrintfW(str,MAX_DESC,L"Velocity %.2lf",abox->velocity().y);
		velocitytxt_->set(str);
		StringCbPrintfW(str,MAX_DESC,L"Position %.2lf",abox->position().y);
		positiontxt_->set(str);
		StringCbPrintfW(str,MAX_DESC,L"dt %.2lf",delta/1000.0f);
		deltatxt_->set(str);
	}
	if(pressed(LIGHT_POINT)){
		anotherbox->attachTo(NULL,0,0);
	}
}
