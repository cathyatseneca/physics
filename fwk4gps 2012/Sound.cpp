/* Sound Implementation - Modelling Layer
 *
 * Sound.cpp
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Sound.h"            // for Sound class definition
#include "iCoordinator.h"     // for the Coordinator Interface
#include "iAPISound.h"        // for the APISound Interface
#include "iUtilities.h"       // for nameWithDir(), error()

#include "MathDeclarations.h" // for Vector
#include "GeneralConstants.h" // for MODEL_Z_AXIS
#include "ModellingLayer.h"   // for AUDIO_DIRECTORY

//------------------------------- Sound ------------------------------------
//
// The Sound class defines the structure of a sound in the model
//
// CreateSound creates a sound object associated with file f in directory
// AUDIO_DIRECTORY with the following properties: local (l), continuous (c), 
// on (o), outer cone (q), inner cone (i) in degrees
//
iSound* CreateSound(const wchar_t* f, bool l, bool c, bool o, float q, 
 float i) {

	return new Sound(f, l, c, o, q, i);
}

iSound* Clone(const iSound* src) { 
    
    return (iSound*)src->clone();
}

// constructor adds the sound to the coordinator, creates the apiSound, stores 
// the name of the sound file and the name of the sound file relative to the 
// execution directory and initializes the reference time
//
Sound::Sound(const wchar_t* file, bool l, bool c, bool o, float q, float i) : 
 local(l), continuous(c), on(o)  {

    coordinator->add(this);

    // apiSound on the sound device
	apiSound = CreateAPISound(q, i);

	if (file) {
        // store filename (without/with path)
	    int len = strlen(file);
	    relFile = new wchar_t[len + 1];
	    strcpy(relFile, file, len);
	    // add the directory to create the relative filename
	    len += strlen(AUDIO_DIRECTORY) + 1;
	    fileWithPath = new wchar_t[len + 1];
	    ::nameWithDir(fileWithPath, AUDIO_DIRECTORY, relFile, len);
    }
    else {
        relFile      = nullptr;
        fileWithPath = nullptr;
    }

	// prepare to start playing
    setToStart = continuous && on;
    setToStop  = false;

	// initialize reference time
	lastToggle = 0;
}

// copy constructor initializes the instance variables and calls the
 // assignment operator
//
Sound::Sound(const Sound& src) {

    coordinator->add(this);

	apiSound     = nullptr;
	relFile      = nullptr;
	fileWithPath = nullptr;
	*this        = src;
}

// assignment operator discard old sound apiSound and copies over
// data from src - does not copy over the APISound object from src
// but instead creates a new APISound object for the current sound
// object - does not copy over the Frame or the Text objects attached
// to the current object but instead initializes them to nullptr
//
Sound& Sound::operator=(const Sound& src) {

	if (this != &src) {
        *((Frame*)this) = src;
		if (apiSound)
			apiSound->Delete();
        if (src.apiSound)
		    apiSound = src.apiSound->clone();
        else    
            apiSound = nullptr;

		if (relFile)
			delete [] relFile;
        if (src.relFile) {
		    int len = strlen(src.relFile);
		    relFile = new wchar_t[len + 1];
		    strcpy(relFile, src.relFile, len);
        }
        else 
            relFile = nullptr;

		if (fileWithPath)
			delete [] fileWithPath;
        if (src.fileWithPath) {
		    int len = strlen(src.fileWithPath);
		    fileWithPath = new wchar_t[len + 1];
		    strcpy(fileWithPath, src.fileWithPath, len);
        }
        else
            fileWithPath = nullptr;

		local      = src.local;
		continuous = src.continuous;
		setToStart = src.on;
		on         = false;
		setToStop  = false;
		lastToggle = 0;
	}

	return *this;
}

// change changes the source file for the sound
//
void Sound::change(const wchar_t* file) {

	if (file) {
		//stop the current sound
		if (apiSound) {
			apiSound->stop();
			apiSound->release();
		}
		// store filename (without/with path)
		int len = strlen(file);
		if (relFile) 
			delete [] relFile;
		relFile = new wchar_t[len + 1];
		strcpy(relFile, file, len);
		// add the directory to create the relative filename
		if (fileWithPath) 
			delete [] fileWithPath;
		len += strlen(AUDIO_DIRECTORY) + 1;
		fileWithPath = new wchar_t[len + 1];
		::nameWithDir(fileWithPath, AUDIO_DIRECTORY, relFile, len);
	}
}

// toggle toggles the sound if the latency period has elapsed
//
bool Sound::toggle() {

	bool rc = false;

    if (now - lastToggle > KEY_LATENCY) {
        if (on)
            setToStop = true;
        else
            setToStart = true;
        lastToggle = now;
		rc = true;
    }

	return rc;
}

// plays the sound if the latency period has elapsed
//
bool Sound::play() {

	bool rc = false;

    if (now - lastToggle > KEY_LATENCY) {
        setToStart = true;
        lastToggle = now;
		rc = true;
    }

	return rc;
}

// plays the sound if the latency period has elapsed
//
bool Sound::stop() {

	bool rc = false;

    if (now - lastToggle > KEY_LATENCY) {
        setToStop = true;
        lastToggle = now;
		rc = true;
    }

	return rc;
}

// update updates the position and orientation of the Sound Instance
//
void Sound::update() {

	if (apiSound && local) 
	    apiSound->update(position(), orientation('z'));
}

// render schedule changes to the sound apiSound
//
void Sound::render() {

	if (setToStop) {
		if (apiSound) 
            apiSound->stop();
		setToStop  = false;
		on         = false;
	}
	if (setToStart) {
		if (apiSound) 
			apiSound->play(fileWithPath, position(), orientation('z'), local, 
             continuous);
		setToStart = false;
		setToStop  = false;
		on         = true;
	}
}

// suspend stops the playing of the sound apiSound and prepares the sound
// to start playing upon restoration if the sound is continuous and set 
// to start or on
//
void Sound::suspend() {

	if (apiSound) 
		apiSound->stop();
	setToStart = continuous && (setToStart || on);
	on         = false;
}

// restore reinitializes the time of the last toggle
//
void Sound::restore() {

	// reference time
	lastToggle = now;
}

// release disengages the sound
//
void Sound::release() {

    suspend();
    if (apiSound)
        apiSound->release();
}

// destructor deletes the sound apiSound and removes the sound
// from the coordinator
//
Sound::~Sound() {

	if (fileWithPath)
		delete [] fileWithPath;
	if (relFile)
		delete [] relFile;
    if (apiSound) 
		apiSound->Delete();
    coordinator->remove(this);
}
