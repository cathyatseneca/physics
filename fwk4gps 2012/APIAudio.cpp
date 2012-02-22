/* APIAudio Implementation - Translation Layer
 *
 * APIAudio.cpp
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * contributions by: Jon Buckley '11
 * distributed under TPL - see ../Licenses.txt
 */

#include "APIAudio.h"        // for the APIAudio class definition
#include "APIBase.h"         // for access to APIBase::connectsThrough()
#include "iUtilities.h"      // for error()
#include "MathDefinitions.h" // for Vector

//------------------------------- APIAudio ------------------------------------
//
// The APIAudio class manages the API Sound connectivity
//
// CreateSound creates the APIAudio object
//
iAPIAudio* CreateAPIAudio(float d, int mnv, int mxv, int mnf, int mxf, int dv, 
 int df) {

	return new APIAudio(d, mnv, mxv, mnf, mxf, dv, df);
}

// constructor initializes the instance pointers and COM
//
APIAudio::APIAudio(float d, int mnv, int mxv, int mnf, int mxf,
 int dv, int df) : minVolume(mnv), maxVolume(mxv), minFrequency(mnf),
 maxFrequency(mxf), defVolume(dv), defFrequency(df) {

	// Multi-threaded audio coordinator
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    
    distanceScale = d;
    audio         = this; 
}

// setup sets up the Sound Card and attaches the APIAudio object to the 
// APISound class
//
bool APIAudio::setup() {

    bool rc = false;
	UINT32 flags = 0;

	// enable XAudio2 debugging if we're running in debug mode
	#ifdef _DEBUG
		flags |= XAUDIO2_DEBUG_ENGINE;
	#endif

	if (FAILED(XAudio2Create(&pXAudio2, flags))) {
		error(L"APIAudio::11 Failed to initialize the XAudio2 engine");
		release();
	}
	else if (FAILED(pXAudio2->CreateMasteringVoice(&pMasteringVoice))) {
		error(L"APIAudio::12 Failed to create the Mastering Voice");
		release();
	}
    else {
		XAUDIO2_DEVICE_DETAILS deviceDetails;
		pXAudio2->GetDeviceDetails(0, &deviceDetails);
		DWORD channelMask = deviceDetails.OutputFormat.dwChannelMask;

		// Initialize the X3DAudio engine
		X3DAudioInitialize(channelMask, X3DAUDIO_SPEED_OF_SOUND, X3DInstance);
		
        // set the frequency and volume range of the context
		// http://msdn.microsoft.com/en-us/library/ee415828(v=VS.85).aspx
	    ZeroMemory(&Listener, sizeof(X3DAUDIO_LISTENER));

	    // provides the API connectivity for the APISound objects
        pX3DInstance = &X3DInstance;
        pListener    = &Listener;
        rc = true;
    }

    return rc;
}

// update updates the listener to hold the current viewpoint and orientation
//
void APIAudio::update(const void* view) {

    if (view) {
        Vector position           = ((Matrix*)view)->position();
        Vector front              = ::normal(((Matrix*)view)->direction('z'));
        Vector up                 = ::normal(((Matrix*)view)->direction('y'));
	    X3DAUDIO_VECTOR eFront    = {front.x, front.y, front.z};
	    X3DAUDIO_VECTOR ePosition = {position.x, position.y, position.z};
	    X3DAUDIO_VECTOR eUp       = {up.x, up.y, up.z};
	
	    Listener.OrientFront = eFront;
	    Listener.Position    = ePosition;
	    Listener.OrientTop   = eUp;
    }
    pMasteringVoice->SetVolume(volume);
}

// suspend suspends the master voice
//
void APIAudio::suspend() {

    if (pMasteringVoice)
        pMasteringVoice->SetVolume(0);
}

// restore restores the volume
//
bool APIAudio::restore() {

    if (pMasteringVoice) 
        pMasteringVoice->SetVolume(0);

    return true;
}

// release disengages the interfaces to the COM objects
//
void APIAudio::release() {

	if (pMasteringVoice) {
		pMasteringVoice->DestroyVoice();
		pMasteringVoice = nullptr;
	}

	if (pXAudio2) {
		pXAudio2->Release();
		pXAudio2 = nullptr;
	}
}

// destructor releases the connection and uninitializes COM
//
APIAudio::~APIAudio() {

	release();
	CoUninitialize();
    audio = nullptr;
}

// convertFrequency converts model frequency into X2Audio frequency units
//
void APIAudio::convertFrequency(int f) {

	if (f < defFrequency)                          // from [MIN, DEF-]
		frequencyRatio = float(f - minFrequency) / // to   [0, 0.9999]
         (defFrequency - minFrequency);       
	else if (f > defFrequency)                     // from [DEF+, MAX]
		frequencyRatio = float(f - defFrequency) / // to   [DEF+, 1024]
         (maxFrequency - defFrequency) * (1024 - 1) + 1.0f; 
    else
	    frequencyRatio = 1.0f;
}

// convertVolume converts model volume into X2Audio volume units
//
void APIAudio::convertVolume(int v) {

	if (v < defVolume)                           // from [MIN, DEF-]
		volume = (v - (float)minVolume) / 
         (defVolume - minVolume);                // to   [0, 0.9999]
	else if (v > defVolume)                      // from [DEF+, MAX]
		volume = (float)(1 << unsigned((v - (float)defVolume) / 
         (maxVolume - defVolume) * 24));         // to   [1+, 2^24]
    else
	    volume = 1.0f;                            
}
