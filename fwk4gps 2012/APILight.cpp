/* APILight Implementation  - Translation Layer
 *
 * APILight.cpp
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "APIPlatformSettings.h" // for API headers
#include "APILight.h"            // for the APILight class definition
#include "iAPIBase.h"            // for error()
#include "MathDeclarations.h"    // for Matrix, Vector and Colour
#include "Common_Symbols.h"      // symbols common to Modelling/Translation

//-------------------------------- APILight -----------------------------------
//
// The APILight class implements a Light at the API level
//
// The lightIndex array keeps track of the available light indeces
//
bool* APILight::lightIndex = nullptr;
int   APILight::maxLights  = 0;
//
// CreateAPILight creates an APILight object
//
iAPILight* CreateAPILight(LightType t, Colour d, Colour a, Colour s,
 float r, bool o, float a0, float a1, float a2, float p, float th, float f) {

	return new APILight(t, d, a, s, r, o, a0, a1, a2, p, th, f);
}

// constructor initializes the api light as needing to be setup
//
APILight::APILight(LightType t, Colour d, Colour a, Colour s,
 float r, bool o, float a0, float a1, float a2, float p, float th, float f) : 
 type(t), diffuse(d), ambient(a), specular(s), range(r), attenuation0(a0), 
 attenuation1(a1), attenuation2(a2), phi(p), theta(th), falloff(f), 
 isSetup(false), index(0) {}

// copy constructor

APILight::APILight(const APILight& src) {

    *this = src;
}

APILight& APILight::operator=(const APILight& src) {

    if (this != &src) {
		type          = src.type;   
		diffuse       = src.diffuse ;  
		ambient       = src.ambient ;  
		specular      = src.specular ;  
		range         = src.range;       
		attenuation0  = src.attenuation0; 
		attenuation1  = src.attenuation1;
		attenuation2  = src.attenuation2;
        phi           = src.phi;
        theta         = src.theta;
        falloff       = src.falloff;
        isSetup       = false;
        index         = 0;
    }

    return *this;
}

void APILight::alloc(int max) {

    if (lightIndex)
        delete [] lightIndex;
    maxLights = max;
    lightIndex = new bool[maxLights];
    for (int i = 0; i < maxLights; i++)
        lightIndex[i] = true;
}

void APILight::dealloc() {

    if (lightIndex)
        delete [] lightIndex;
    lightIndex = nullptr;
}

// setup finds an index for the api light and creates the api light using
// light's properties - returns true if successful, false otherwise
//
bool APILight::setup() {

	bool rc = false;

	// find an index for this api light and assign it
    index = -1;
    for (int i = 0; i < maxLights; i++) {
        if (lightIndex && lightIndex[i]) {
            lightIndex[i] = false;
            index = i;
            i = maxLights;
        }
    }
	if (index == -1)
		error(L"APILight::21 No more room for lights on this device");
	else {
		D3DLIGHT9 d3dLight;
		ZeroMemory(&d3dLight, sizeof d3dLight);
		switch (type) {
		  case POINT_LIGHT:
			d3dLight.Type = D3DLIGHT_POINT;
			break;
		  case SPOT_LIGHT:
			d3dLight.Type = D3DLIGHT_SPOT;
			break;
		  case DIRECTIONAL_LIGHT:
			d3dLight.Type = D3DLIGHT_DIRECTIONAL;
			break;
		}
		d3dLight.Diffuse      = D3DXCOLOR(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
		d3dLight.Ambient      = D3DXCOLOR(ambient.r, ambient.g, ambient.b, ambient.a);
		d3dLight.Specular     = D3DXCOLOR(specular.r, specular.g, specular.b, specular.a);
		d3dLight.Attenuation0 = attenuation0;
		d3dLight.Attenuation1 = attenuation1;
		d3dLight.Attenuation2 = attenuation2;
		d3dLight.Phi          = phi;
		d3dLight.Theta        = theta;
		d3dLight.Falloff      = falloff;
		d3dLight.Range        = range;
		d3dLight.Position     = D3DXVECTOR3(0, 0, 0);
		d3dLight.Direction    = D3DXVECTOR3(0, 0, 0);

		if (!d3dd || FAILED(d3dd->SetLight(index, &d3dLight)))
			error(L"APILight::22 Failed to create device light");
		else
			rc = true;
	}

	return rc;
}

// turnOn sets up the api light if it needs to be setup,
// adjusts its position and orientation if it is mobile and
// turns on the device light 
//
void APILight::turnOn(const Vector& p, const Vector& o) {

	if (!isSetup) isSetup = setup();

	if (isSetup) {
		D3DLIGHT9 d3dLight;
		if (FAILED(d3dd->GetLight(index, &d3dLight)))
			error(L"APILight::23 Failed to find a device light");
		else {
			d3dLight.Position  = D3DXVECTOR3(p.x, p.y, p.z);
			d3dLight.Direction = D3DXVECTOR3(o.x, o.y, o.z);
			if (FAILED(d3dd->SetLight(index, &d3dLight)))
				error(L"APILight:24 Failed to update position");
		}
		d3dd->LightEnable(index, true);
	}
}

// update sets up the api light if it needs to be setup and adjusts
// its position and orientation 
//
void APILight::update(const Vector& p, const Vector& o) {

	if (!isSetup) isSetup = setup();

	if (isSetup) {
		D3DLIGHT9 d3dLight;
		if (FAILED(d3dd->GetLight(index, &d3dLight)))
			error(L"APILight::23 Failed to find a device light");
		else {
			d3dLight.Position  = D3DXVECTOR3(p.x, p.y, p.z);
			d3dLight.Direction = D3DXVECTOR3(o.x, o.y, o.z);
			if (FAILED(d3dd->SetLight(index, &d3dLight)))
				error(L"APILight:24 Failed to update position");
		}
		d3dd->LightEnable(index, true);
    }
}

// turnOff turns off the api light
//
void APILight::turnOff() {

    if (isSetup) {
         if (d3dd)
            d3dd->LightEnable(index, false);
    }
}

// suspend turns off the api light and makes its index available
// for future restoration
//
void APILight::suspend() {

    turnOff();

	if (isSetup) {
		lightIndex[index] = true;
		isSetup = false;
	}
}
