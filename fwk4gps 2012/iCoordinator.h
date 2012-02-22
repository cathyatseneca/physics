#ifndef _I_COORDINATOR_H_
#define _I_COORDINATOR_H_

/* Coordinator Interface - Modelling Layer
 *
 * iCoordinator.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Base.h" // for the Base class definition

//-------------------------------- iCoordinator -------------------------------------
//
// iCoordinator is the Interface to the Coordinator class
//
class iObject;
class iTexture;
class iLight;
class iCamera;
class iSound;
class iText;
class iHUD;
class iGraphic;
enum  Action;
enum  ModelSound;


class iCoordinator : public Base {
  public:
	// initialization
    virtual void initialize()                                       = 0;
    virtual void add(iObject* o)                                    = 0;
    virtual void add(iTexture* t)                                   = 0;
    virtual void add(iLight* l)                                     = 0;
    virtual void add(iCamera* c)                                    = 0;
    virtual void add(iSound* s)                                     = 0;
    virtual void add(iGraphic* v)                                   = 0;
    virtual void add(iText* t)                                      = 0;
    virtual void add(iHUD* h)                                       = 0;
    virtual void reset()                                            = 0;
	// execution
    virtual void update()                                           = 0;
    virtual bool pressed(Action a) const                            = 0;
    virtual bool ptrPressed() const                                 = 0;
    virtual bool ctrPressed() const                                 = 0;
    virtual int  change(Action a) const                             = 0;
    virtual void resize()                                           = 0;
	virtual int  run()                                              = 0;
	// termination
	virtual void remove(iObject* o)                                 = 0;
	virtual void remove(iTexture* t)                                = 0;
	virtual void remove(iLight* l)                                  = 0;
    virtual void remove(iCamera* c)                                 = 0;
    virtual void remove(iSound* s)                                  = 0;
    virtual void remove(iGraphic* v)                                = 0;
    virtual void remove(iText* t)                                   = 0;
    virtual void remove(iHUD* h)                                    = 0;
};

iCoordinator* CoordinatorAddress();

#endif
