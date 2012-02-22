#ifndef _I_OBJECT_H_
#define _I_OBJECT_H_

/* Object Interface - Modelling Layer
 *
 * iObject.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Frame.h"      // for the Shape class definition
#include "Base.h"       // for the Base class definition

//-------------------------------- iObject ------------------------------------
//
// iObject is the Interface to the Object class
//
class  iTexture;
class  iGraphic;
struct Reflectivity;
enum   Category;

class iObject : public Shape, public Base {
  public:
	// initialization
	virtual void        attach(iTexture* t)                  = 0;
	// execution
    virtual void        setTextureFilter(unsigned)           = 0;
    virtual iTexture*   getTexture() const                   = 0;
    virtual const void* getReflectivity() const              = 0;
    virtual bool        belongsTo(Category category) const   = 0;
};

iObject* CreateObject(iGraphic* v, const Reflectivity* r = 0); 

iObject* Clone(const iObject*);

#endif
