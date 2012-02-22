#ifndef _CAMERA_H_
#define _CAMERA_H_

/* Camera Definition - Modelling Layer
 *
 * Camera.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iCamera.h"          // for the Camera Interface
#include "MathDeclarations.h" // for Matrix

//-------------------------------- Camera -------------------------------------
//
// The Camera class defines the structure of a viewpoint
//
class Camera : public iCamera {

    static iCamera* current; // points to the current camera
    static Matrix   view;    // the view transformation for the current camera

    virtual ~Camera();

  public:
    static iFrame** getCurrent() { return (iFrame**)&current; }
    static void*    getView()    { return &view; }
    Camera();
	Camera(const Camera& c);
    void* clone() const          { return new Camera(*this); }
	// execution
    void  update();
};

#endif
