#ifndef _I_API_SOUND_H_
#define _I_API_SOUND_H_

/* APISound Interface - Translation Layer
 *
 * iAPISound.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- APISound ------------------------------------
//
// iAPISound is the Interface to the APISound class
//
struct Vector;

class iAPISound {
  public:
    virtual iAPISound* clone() const                                = 0;
	// execution
    virtual void soundCone(float, float)                            = 0;
    virtual void update(const Vector&, const Vector&)               = 0;
    virtual void play(const wchar_t*, const Vector&, const Vector&, 
     bool, bool)                                                    = 0;
    virtual void stop()                                             = 0;
	// termination
	virtual void release()                                          = 0;
	virtual void Delete() const                                     = 0;
};

iAPISound* CreateAPISound(float, float);

#endif
