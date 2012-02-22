#ifndef _I_SOUND_H_
#define _I_SOUND_H_

/* Sound Interface - Modelling Layer
 *
 * iSound.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Frame.h"      // for the Frame class definition
#include "Base.h"       // for the Base class definition

//-------------------------------- iSound -------------------------------------
//
// iSound is the Interface to the Sound class
//

class iSound : public Frame, public iSwitch, public Base {
  public:
	// initialization
	virtual const wchar_t* relFileName() const = 0;
	virtual void  change(const wchar_t* f)     = 0;
    virtual void  loop(bool)                   = 0;
	// execution
    virtual bool  isOn() const                 = 0;
	virtual bool  toggle()                     = 0;
	virtual void  update()                     = 0;
	virtual bool  stop()                       = 0;
	virtual bool  play()                       = 0;
};

iSound* CreateSound(const wchar_t*, bool = false, bool = true, bool = true, 
 float = 0, float = 0);

iSound* Clone(const iSound*);

#endif
