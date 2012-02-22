#ifndef _SOUND_H_
#define _SOUND_H_

/* Sound Definition - Modelling Layer
 *
 * Sound.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iSound.h"     // for the Sound Interface

//------------------------------- Sound ---------------------------------------
//
// The Sound class defines the structure of a sound in the Modelling Layer
//
class iAPISound;

class Sound : public iSound {

    iAPISound* apiSound;          // points to the sound at the api level
    wchar_t*   fileWithPath;      // name of the sound file with the path
	wchar_t*   relFile;           // name of the sound file without the path

    bool       local;             // is this sound local ?
    bool       on;                // is this sound on?
    bool       setToStart;        // is this sound ready to start playing?
    bool       setToStop;         // is this sound ready to stop playing?
    bool       continuous;        // is this sound continuous?

    unsigned   lastToggle;        // time of the last toggle

	Sound(const Sound&);
    virtual ~Sound();

  public:
    Sound(const wchar_t*, bool, bool, bool, float = 0, float = 0);
	Sound& operator=(const Sound&);
    void* clone() const                { return new Sound(*this); }
	// initialization
	const wchar_t* relFileName() const { return fileWithPath; }
	void  change(const wchar_t* f);
    void  loop(bool on)                { this->on = on; }
	// execution
    bool  isOn() const                 { return on; }
	bool  toggle();
	void  update();
	bool  play();
	bool  stop();
    void  render();
	// termination function
	void  suspend();
	void  restore();
    void  release();
};

#endif
