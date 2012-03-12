#ifndef _I_FRAME_H_
#define _I_FRAME_H_

/* Frame Interface - Modelling Layer
 *
 * iFrame.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- iSwitch ------------------------------------
//
// iSwitch is the Interface for a switchable design item
//
class iSwitch {
public:
    virtual bool isOn() const = 0;
};

//-------------------------------- iFrame -------------------------------------
//
// iFrame is the Interface to the Frame class
//
struct Matrix;
struct Vector;

class iFrame {
  public:
    virtual void   translate(float x, float y, float z) = 0;
    virtual void   scale(float x, float y, float z)     = 0;
    virtual void   rotatex(float rad)                   = 0;
    virtual void   rotatey(float rad)                   = 0;
    virtual void   rotatez(float rad)                   = 0;
    virtual Vector position() const                     = 0;
	virtual Matrix rotation() const                     = 0;
    virtual Vector orientation(char axis) const         = 0;
	virtual Matrix world() const                        = 0;
    virtual void   attachTo(iFrame* parent)             = 0;

	//added for phyics
	// rotate rotates the Frame using rotation matrix rot
	virtual Vector relativePosition()					= 0;
	virtual void rotate(const Matrix& rot)                      = 0;
};

class iShape {
  public:
    virtual void setRadius(float r)                     = 0;
    virtual void setRadius(float x, float y, float z)   = 0;
    virtual void setPlane(Vector n, float d)            = 0;
    virtual void setAxisAligned(Vector min, Vector max) = 0;
};

#endif
