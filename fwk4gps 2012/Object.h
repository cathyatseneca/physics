#ifndef _OBJECT_H_
#define _OBJECT_H_

/* Object Definition - Modelling Layer
 *
 * Object.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iObject.h" // for the Object Interface
#include "Base.h"    // for the Base class definition

//-------------------------------- Object -------------------------------------
//
// The Object class defines an independent object in the Modelling Layer
//
class iGraphic;

class Object : public iObject  {

    Category      category;           // drawing category
    iGraphic*     graphic;            // points to the object's vertex list
    Reflectivity* reflectivity;       // material reflectivity
	iTexture*     texture;            // points to attached texture
	unsigned      flags;              // texture sampling flags

  protected:
    virtual       ~Object();

  public:
    Object(Category, iGraphic*, const Reflectivity*);
    Object(const Object&);            
    Object& operator=(const Object&); 
    void*       clone() const                { return new Object(*this); }
	// initialization
	void        attach(iTexture*);
	// execution
    void        setTextureFilter(unsigned f) { flags = f; }
    iTexture*   getTexture() const           { return texture; }
    const void* getReflectivity() const      { return reflectivity; }
    bool        belongsTo(Category c) const  { return c == category; }
    void        render();
};

#endif
