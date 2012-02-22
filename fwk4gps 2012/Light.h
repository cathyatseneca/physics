#ifndef _LIGHT_H_
#define _LIGHT_H_

/* Light Definition - Modelling Layer
 *
 * Light.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iLight.h"           // for the Light Interface
#include "MathDeclarations.h" // for Colour

//-------------------------------- Light --------------------------------------
//
// The Light class defines the structure of a light source
//
class iAPILight;

class Light : public iLight {

	iAPILight* apiLight;   // points to the api light

	bool       on;         // light is on?
	bool       turnOn;     // turn on this light?
	bool       turnOff;    // turn off this light?
    int        lastToggle; // time of the last toogle

	Light(const Light&);
	virtual ~Light();

  public:
    Light(LightType, Colour, Colour, Colour, float, bool, float = 1, 
	 float = 0, float = 0, float = 0, float = 0, float = 0);
	Light&  operator=(const Light&);
    void* clone() const  { return new Light(*this); }
    bool  isOn() const   { return on; }
	bool  toggle();
	void  update();
	void  suspend();
	void  restore();
};

#endif
