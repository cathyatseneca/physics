/* Light Implementation - Modelling Layer
 *
 * Light.cpp
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Light.h"           // for Light class definitions
#include "iCoordinator.h"    // for the Coordinator Interface
#include "iAPILight.h"       // for the APILight Interface
#include "iUtilities.h"      // for error()

#include "MathDefinitions.h" // for math functions in coordinator coordinates
#include "ModellingLayer.h"  // for KEY_LATENCY
#include "Common_Symbols.h"  // symbols common to Modelling/Translation layers

//-------------------------------- Light --------------------------------------
//
// The Light class defines the structure of a light source
//
// CreateDistantLight creates a distant Light object
//
iLight* CreateDistantLight(Colour d, Colour a, Colour s, bool o) {

	return new Light(DIRECTIONAL_LIGHT, d, a, s, 0, o);
}

// CreatePointLight creates a point Light object
//
iLight* CreatePointLight(Colour d, Colour a, Colour s, float r, bool o,
 float a0, float a1, float a2) {

	return new Light(POINT_LIGHT, d, a, s, r, o, a0, a1, a2);
}

// CreateSpotLight creates a spot Light object
//
iLight* CreateSpotLight(Colour d, Colour a, Colour s, float r, bool o, 
 float a0, float a1, float a2, float ph, float th, float f) {

	return new Light(SPOT_LIGHT, d, a, s, r, o, a0, a1, a2, ph, th, f);
}

iLight* Clone(const iLight* src) {

    return (iLight*)src->clone();
}

// constructor stores the properties of the light, creates the APILight
// object and initializes the reference time
//
Light::Light(LightType t, Colour d, Colour a, Colour s, float r, bool o, 
 float a0, float a1, float a2, float ph, float th, float f) : on(false), 
 turnOn(o), turnOff(false) {

    coordinator->add(this);

    apiLight = CreateAPILight(t, d, a, s, r, o, a0, a1, a2, ph, th, f);

	// reference time
	lastToggle  = 0;
}

// copy constructor initializes the APILight pointer and copies over the
// instance variables from src
//
Light::Light(const Light& src) {

    coordinator->add(this);

	apiLight = nullptr;
	*this    = src;
}

// assignment operator discards the old APILight and copies over properties
// from src
//
Light& Light::operator=(const Light& src) {

	if (this != &src) {
        *((Frame*)this) = src;
		if (apiLight) 
			apiLight->Delete();
        if (src.apiLight)
            apiLight = src.apiLight->clone();
        else
            apiLight = nullptr;
		on         = false;     
		turnOn     = src.on;    
		turnOff    = false;   
		lastToggle = 0;
	}

	return *this;
}

// toggle toggles the light's state if outside the latency period
// and returns the future state
//
bool Light::toggle() {

	bool rc = on;

    if (now - lastToggle > KEY_LATENCY) {
		if (on)
			turnOff = true;
		else
			turnOn  = true;
		rc          = !rc;
        lastToggle  = now;
    }

	return rc;
}

// update updates or turns the api light on or off
//
void Light::update() {

	if (turnOn) {
		apiLight->turnOn(position(), orientation('z'));
		turnOn = false;
		on     = true;
	}
	else if (turnOff) {
		apiLight->turnOff();
		turnOff = false;
		on      = false;
	}
    else if (on) {
		apiLight->update(position(), orientation('z'));
    }
}

// suspend suspends the api light and prepares for turning on the light
// upon restoration
//
void Light::suspend() {

	if (apiLight) 
		apiLight->suspend();
	turnOn  = on || turnOn;
	turnOff = false;
}

// restore reinitializes the time of the last toggle
//
void Light::restore() {

	lastToggle = now;
}

// destructor deletes the light source from the coordinator
//
Light::~Light() {

    coordinator->remove(this);
}
