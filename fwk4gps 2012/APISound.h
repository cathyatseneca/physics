#ifndef _APISOUND_H_
#define _APISOUND_H_

/* APISound Definition - Translation Layer
 *
 * APISound.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * contributions by: Jon Buckley '11
 * distributed under TPL - see ../Licenses.txt
 */

#include "APIPlatformSettings.h" // for IXAudio2, ...
#include "APIBase.h"             // for the APIBase class definition
#include "iAPISound.h"           // for the APISound Interface

//-------------------------------- APISound -----------------------------------
//
// The APISound class implements a Sound at the API level 
//
class APIAudio;

class APISound : public iAPISound, public APIBase {

    IXAudio2SourceVoice*  pSourceVoice;  // sound source
	BYTE*                 pDataBuffer;   // Stores WAVE buffer
	UINT32*               pDpdsBuffer;   // Stores xWMA buffer
	X3DAUDIO_EMITTER      Emitter;	     // Represents the frame in 3D space
	X3DAUDIO_DSP_SETTINGS DSPSettings;   // Stores 3D audio settings
	X3DAUDIO_CONE         cone;          // Stores the sound cone settings
    float                 outerCone;     // outer cone angle in degrees
    float                 innerCone;     // inner cone angle in degrees
    unsigned              matrixSize;    // size of translation matrix 
	float*                matrix;	     // calculates output volumes

    virtual ~APISound();

	// RIFF File handling
	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize,
     DWORD & dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, 
     DWORD bufferoffset);

  public:
    APISound(float, float);
    APISound(const APISound& s);
    APISound& operator=(const APISound& s);
    iAPISound* clone() const { return new APISound(*this); }
	// initialization
    bool setup(const wchar_t*, bool global, bool continuous);
	// execution
    void soundCone(float, float);
    void update(const Vector&, const Vector&);
    void play(const wchar_t*, const Vector&, const Vector&, bool, bool);
    void stop();
	// termination
	void release();
	void Delete() const { delete this; }
};

#endif
