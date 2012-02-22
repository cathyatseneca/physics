/* Object Implementation - Modelling Layer
 *
 * Object.cpp
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iCoordinator.h"   // for the Coordinator Interface
#include "iGraphic.h"       // for the Graphic Interface
#include "iTexture.h"       // for the Texture Interface
#include "iUtilities.h"     // for error()

#include "Object.h"         // for the Object class definition
#include "ModellingLayer.h" // for Category symbols
#include "Common_Symbols.h" // symbols common to Modelling/Translation layers

//-------------------------------- Object -------------------------------------
//
// The Object class defines the structure of a single object in the model
//
iObject* CreateObject(iGraphic* v, const Reflectivity* r) {

    return new Object(TEST_COLOUR, v, r);
}

// Clone create a clone of Object* src and returns its address
//
iObject* Clone(const iObject* src) {

    return (iObject*)src->clone();
}

// constructor initializes an object with material reflectivity *r
//
Object::Object(Category d, iGraphic* v, const Reflectivity* r) : category(d),
 graphic(v), texture(0), flags(TEX_DEFAULT) {
    
    coordinator->add(this);

    // store reflectivity and texture pointer
    if (r) {
        reflectivity = new Reflectivity;
        *reflectivity = *r;

        // specialize category for translucency only if TEST_COLOUR
        if (category == TEST_COLOUR) {
            category = OPAQUE_OBJECT;
            if (reflectivity->translucent())
                category = TRANSLUCENT_OBJECT;
        }
    }
    else {
        category = OPAQUE_OBJECT;
        reflectivity = nullptr;
    }
}

// copy constructor initializes the instance pointer and calls the assignment
// operator
//
Object::Object(const Object& src) {

    coordinator->add(this);
    reflectivity = nullptr;
    *this = src;
}
            
// assignment operator discards old data and creates a copy from
// src - does not copy the text items attached to src - instead 
// initializes them to nullptr
//
Object& Object::operator=(const Object& src) {

    if (this != &src) {
        *((iObject*)this) = src;
        if (reflectivity) 
            delete reflectivity;
        if (src.reflectivity) {
            reflectivity  = new Reflectivity;
            *reflectivity = *src.reflectivity;
        }
        else
            reflectivity = nullptr;
        category     = src.category;
        graphic      = src.graphic; 
        flags        = src.flags;
        texture      = src.texture;
    }

    return *this;
}

// attach attaches texture *t to the object
//
void Object::attach(iTexture* t) {

    texture = t;
}

// render draws the object
//
void Object::render() { 
	
    if (texture && flags) texture->setFilter(flags);
    if (graphic) graphic->render();
}

// destructor removes the object from the model coordinator
//
Object::~Object() {

    coordinator->remove(this);
    if (reflectivity)
        delete reflectivity;
}

