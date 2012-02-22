#ifndef _I_API_AUDIO_H_
#define _I_API_AUDIO_H_

/* APIAudio Interface - Translation Layer
 *
 * iAPIAudio.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//------------------------------- iAPIAudio -----------------------------------
//
// iAPIAudio is the Interface to the APIAudio class 
//
class iAPIAudio {
  public:
	// execution
    virtual void setVolume(int)                                      = 0;
    virtual bool setup()                                             = 0;
    virtual void setFrequencyRatio(int)                              = 0;
    virtual void update(const void*)                                 = 0;
	// termination
    virtual void suspend()                                           = 0;
    virtual bool restore()                                           = 0;
    virtual void release()                                           = 0;
	virtual void Delete() const                                      = 0;
};

iAPIAudio* CreateAPIAudio(float, int, int, int, int, int, int);

#endif
