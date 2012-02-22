#ifndef _I_TEXTURE_H_
#define _I_TEXTURE_H_

/* Texture Interface - Modelling Layer
 *
 * iTexture.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Base.h"       // for the Base class definition

//-------------------------------- iTexture -----------------------------------
//
// iTexture is the Interface to the Texture class
//
struct Rectf;

class iTexture : public Base {
  public:
	virtual void attach() const                                        = 0;
    virtual void setFilter(unsigned) const                             = 0;
	virtual void detach()                                              = 0;
	virtual void render(const Rectf&, bool = false)                    = 0;
};

iTexture* CreateTexture(const wchar_t* file, unsigned filter = 0);

iTexture* Clone(const iTexture*);

#endif
