#ifndef _I_GRAPHIC_H_
#define _I_GRAPHIC_H_

/* Graphic Interface - Modelling Layer
 *
 * iGraphic.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Base.h"       // for the Base class definition

//-------------------------------- iGraphic ------------------------------
//
// iGraphic is the Interface to the Graphic hierarchy
//
struct Vector;
enum PrimitiveType;
struct Colour;

class iGraphic : public Base {
  public:
    virtual void   populate(unsigned, void**)                    = 0;
    virtual Vector position(int) const                           = 0;
    virtual void   render()                                      = 0;
};

iGraphic* CreateBox(float minx, float miny, float minz, float maxx, 
 float maxy, float maxz);

iGraphic* CreateGrid(float min, float max, int n);

iGraphic* TriangleList(const wchar_t* file);

iGraphic* TriangleList(const wchar_t* file, const Colour&);

iGraphic* CreateRectangleList(float minx, float miny, float maxx, 
 float maxy);

#endif
