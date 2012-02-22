#ifndef _I_BASE_H_
#define _I_BASE_H_

/* Base Interface - Modelling Layer
 *
 * iBase.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- iBase --------------------------------------
//
// iBase is the Interface to the Design Item class
//
class iBase {
  public:
    virtual void* clone() const              = 0;
	// execution
    virtual void  render()                   = 0;
	// termination
    virtual void  suspend()                  = 0;
    virtual void  restore()                  = 0;
    virtual void  release()                  = 0;
    virtual void  Delete() const             = 0;
};

#endif
