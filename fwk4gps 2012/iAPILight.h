#ifndef _I_API_LIGHT_H_
#define _I_API_LIGHT_H_

/* API Light Interface - Translation Layer
 *
 * iAPILight.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- iAPILight ----------------------------------
//
// iAPILight is the Interface to the APILight class 
//
enum LightType;
struct Colour;
struct Vector;

class iAPILight {
  public:
    virtual iAPILight* clone() const                  = 0;
	// execution
	virtual void turnOn(const Vector&, const Vector&) = 0;
	virtual void update(const Vector&, const Vector&) = 0;
	virtual void turnOff()                            = 0;
	// termination
	virtual void suspend()                            = 0;
	virtual void Delete() const                       = 0;
};

iAPILight* CreateAPILight(LightType, Colour, Colour, Colour, float, 
 bool, float, float, float, float = 0, float = 0, float = 0);

#endif