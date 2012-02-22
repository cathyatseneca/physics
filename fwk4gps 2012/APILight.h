#ifndef _API_LIGHT_H_
#define _API_LIGHT_H_

/* APILight Definition - Translation Layer
 *
 * APILight.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "APIBase.h"          // for the APIBase class definition
#include "iAPILight.h"        // for the APILight Interface
#include "MathDeclarations.h" // for Colour

//-------------------------------- APILight -----------------------------------
//
// The APILight class implements a Light at the API level
//
class APILight : public iAPILight, public APIBase {

    static bool* lightIndex;       // list of available device lights
    static int   maxLights;        // maximum number of lights

	int          index;        // identifier for this light
	bool         isSetup;      // this api light has been setup?

    LightType    type;         // light type
    Colour       diffuse;      // diffuse component
    Colour       ambient;      // ambient component
    Colour       specular;     // specular component
    float        range;        // beyond which light ceases
    float        attenuation0; // constant attenuation
    float        attenuation1; // linear attenuation
    float        attenuation2; // quadratic attenuation
    float        phi;          // angle of outer spot in radians
    float        theta;        // angle of inner spot in radians
    float        falloff;      // falloff factor [0,1]

    APILight(const APILight&);
	bool setup();

  public:
    static void alloc(int max);
    static void dealloc();
	APILight(LightType, Colour, Colour, Colour, float, bool, float, 
     float, float, float, float, float);
    iAPILight* clone() const { return new APILight(*this); }
    APILight&  operator=(const APILight& src);
	void       turnOn(const Vector&, const Vector&);
	void       update(const Vector&, const Vector&);
	void       turnOff();
	void       suspend();
	void       Delete() const { delete this; }
};

#endif