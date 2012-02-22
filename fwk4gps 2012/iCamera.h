#ifndef _I_CAMERA_H_
#define _I_CAMERA_H_

/* Camera Interface - Modelling Layer
 *
 * iCamera.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Frame.h" // for the Shape definition
#include "Base.h"  // for the Base class definition

//-------------------------------- iCamera ------------------------------------
//
// iCamera is the Interface to the Camera class
//
class iCamera : public Shape, public Base {
  public:
    virtual void update()           = 0;
};

iCamera* CreateCamera();

#endif
