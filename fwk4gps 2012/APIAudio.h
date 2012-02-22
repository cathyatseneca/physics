#ifndef _API_AUDIO_H_
#define _API_AUDIO_H_

/* APIAudio Definition - Translation Layer
 *
 * APIAudio.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski
 * contributions by: Jon Buckley '11
 * distributed under TPL - see ../Licenses.txt
 */

#include "APIPlatformSettings.h" // for selected API headers 
#include "APIBase.h"             // for the APIBase class definition
#include "iAPIAudio.h"           // for the APIAudio Interface

//------------------------------- APIAudio ------------------------------------
//
// The APIAudio class provides the API connectivity to the sound card 
//
class APIAudio : public iAPIAudio, public APIBase {

	X3DAUDIO_HANDLE   X3DInstance; // X3DAudio constants
	X3DAUDIO_LISTENER Listener;	   // cameras's position, orientation

    int               minVolume;
    int               maxVolume;
    int               defVolume;
    int               minFrequency;
    int               maxFrequency;
    int               defFrequency;

    APIAudio(const APIAudio& s);            // prevent copying
    APIAudio& operator=(const APIAudio& s); // prevent assignments
    virtual ~APIAudio();

	// Volume/Frequency conversion functions
	void convertVolume(int);
	void convertFrequency(int);

  public:
    APIAudio(float, int, int, int, int, int, int);
    bool setup();
	// execution function
    void setVolume(int v)         { convertVolume(v); }
    void setFrequencyRatio(int f) { convertFrequency(f); }
    void update(const void*);
	// termination
    void suspend();
    bool restore();
    void release();
	void Delete() const { delete this; }
};

#endif
