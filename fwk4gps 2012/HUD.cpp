/* HUD Implementation - Modelling Layer
 *
 * HUD.cpp
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "HUD.h"            // for HUD class definition
#include "iTexture.h"       // for the Texture Interface
#include "iCoordinator.h"   // for the Coordinator Interface
#include "ModellingLayer.h" // for HUD_MIN, HUD_MAX
#include "Common_Symbols.h" // for Action and Sound enumerations

//-------------------------------- HUD ----------------------------------------
//
// CreateHUD creates a HUD at position x,y of dimensions w,h with texture *t
//
iHUD* CreateHUD(float x, float y, float w, float h, iTexture* t) {

    return (iHUD*)new HUD(x, y, w, h, t);
}

// constructor initializes the HUD's position and off state, adds its address
// to the coordinator and validates the position and dimensions
//
HUD::HUD(float x, float y, float w, float h, iTexture* t) : texture(t), 
 on(true) {

    coordinator->add(this);

    w = w > HUD_MAX - HUD_MIN ? HUD_MAX - HUD_MIN : w < HUD_MIN ? HUD_MIN : w;
    h = h > HUD_MAX - HUD_MIN ? HUD_MAX - HUD_MIN : h < HUD_MIN ? HUD_MIN : h;

    rect = new Rectf(x, y, x + w, y + h);
    validate();

    lastToggle = 0;
}

// copy constructor
//
HUD::HUD(const HUD& src) {

    coordinator->add(this);
    rect  = nullptr;
    *this = src;
}

// assignment operator
//
HUD& HUD::operator=(const HUD& src) {

    if (this != &src) {
        if (rect)
            delete rect;
        rect = new Rectf(*src.rect);
        lastToggle = src.lastToggle;
        lastUpdate = src.lastUpdate;
        on         = src.on;
        texture    = src.texture;
    }
    return *this;
}

// validate keeps the HUD within the client area
//
void HUD::validate() {

    float width  = rect->bottomRightX - rect->topLeftX;
    float height = rect->bottomRightY - rect->topLeftY;

	if (rect->topLeftX < HUD_MIN) { 
		rect->topLeftX = HUD_MIN;
        rect->bottomRightX = HUD_MIN + width;
    }
    else if (rect->bottomRightX > HUD_MAX) {
		rect->bottomRightX = HUD_MAX;
        rect->topLeftX = HUD_MAX - width;
    }
	if (rect->topLeftY < HUD_MIN) {
		rect->topLeftY = HUD_MIN;
        rect->bottomRightY = HUD_MIN + height;
    }
    else if (rect->bottomRightY > HUD_MAX) { 
		rect->bottomRightY = HUD_MAX;
        rect->topLeftY = HUD_MAX - height;
    }
}

// updates the HUD for translations initiated by the user
//
void HUD::update() {

	// update the HUD only if it is being displayed
	if (on) {
        // translate the HUD
        int delta = now - lastUpdate;
	    int dx = 0, dy = 0;
        if (coordinator->pressed(HUD_RIGHT))
			dx += delta;
        if (coordinator->pressed(HUD_LEFT))
			dx -= delta;
        if (coordinator->pressed(HUD_UP))
			dy -= delta;
        if (coordinator->pressed(HUD_DOWN))
			dy += delta;
		rect->topLeftX     += dx * HUD_SPEED;
		rect->topLeftY     += dy * HUD_SPEED;
		rect->bottomRightX += dx * HUD_SPEED;
		rect->bottomRightY += dy * HUD_SPEED;
		validate();
	}
}

// toggle toggles the on/off state of the HUD
//
bool HUD::toggle() {

	if (now - lastToggle > KEY_LATENCY) {
        lastToggle = now;
        on         = !on;
    }
    return on;
}

// render draws the HUD texture at posX, posY
//
void HUD::render() {

	if (texture)
		texture->render(*rect);
}

// restore resets the timers
//
void HUD::restore() {

    lastToggle = now;
}

// destructor deallocates memory for rect and removes the hud from the model
//
HUD::~HUD() {

    if (rect)
        delete [] rect;
    coordinator->remove(this);
}

