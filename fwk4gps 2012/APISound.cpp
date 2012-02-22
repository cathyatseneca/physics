/* APISound Implementation - Translation Layer
 *
 * APISound.cpp
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * contributions by: Jon Buckley '11
 * distributed under TPL - see ../Licenses.txt
 */

#include "APISound.h"         // for the APISound class definition
#include "MathDeclarations.h" // for Vector
#include "iAPIBase.h"         // for error()

/* WAVE/xWMA files are classified as 'RIFF' files by Microsoft. They share
 * the same basic file format. Read about it here:
 * http://msdn.microsoft.com/en-us/library/ee415713(v=VS.85).aspx
 * Windows running on x86/x64 is little-endian, which is why the FOURCC
 * identifiers are reversed
 */
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT  ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

//-------------------------------- APISound ------------------------------------
//
// The APISound class implements a Sound at the API level
//
// CreateAPISound creates a APISound object
//
iAPISound* CreateAPISound(float o, float i) {

	return new APISound(o, i);
}

// constructor initializes the instance pointers
//
APISound::APISound(float o, float i) : outerCone(o), innerCone(i), 
 matrix(nullptr), matrixSize(0) {

    // align inner and outer cones
    if (!outerCone)
        innerCone = 0;
    if (!innerCone || innerCone > outerCone)
        innerCone = outerCone;

    pSourceVoice = nullptr;
	pDataBuffer  = nullptr;
	pDpdsBuffer  = nullptr;
}

// soundCone resets the sound cone angles
//
void APISound::soundCone(float o, float i) {

    outerCone = o;
    innerCone = i;
    release();
}

// copy constructor initializes the instance pointers
//
APISound::APISound(const APISound& src) {

    matrix       = nullptr;
    pSourceVoice = nullptr;
	pDataBuffer  = nullptr;
	pDpdsBuffer  = nullptr;
    *this = src;
}

// assignment operator releases old interfaces and initializes
// new ones
//
APISound& APISound::operator=(const APISound& src) {

    if (this != &src) {
        release();
        outerCone = src.outerCone;
        innerCone = src.innerCone;
    }

    return *this;
}

// setup creates the sound segment from the sound file and in the
// case of a non-global sound creates a 3d audio path and extracts 
// the 3d buffer from that path 
//
bool APISound::setup(const wchar_t* sound, bool local, bool continuous) {

    bool rc = false;
	HANDLE file;
	DWORD chunkSize = 0, chunkDataPosition = 0, fileType = 0;

	WAVEFORMATEXTENSIBLE wfx = {0};
	XAUDIO2_BUFFER buffer = {0};
	XAUDIO2_BUFFER_WMA wmaBuffer = {0};

	file = CreateFile(sound, GENERIC_READ, FILE_SHARE_READ, 
     nullptr, OPEN_EXISTING, 0, nullptr);
	if (file == INVALID_HANDLE_VALUE)
		error(L"APISound::10 Failed to open audio file");
	else if (FAILED(FindChunk(file, fourccRIFF, chunkSize, chunkDataPosition)))
		error(L"APISound::11 Failed to find RIFF segment");
	else if (FAILED(ReadChunkData(file, &fileType, sizeof(DWORD), 
     chunkDataPosition)))
		error(L"APISound::12 Failed to read RIFF segment");
	else if ( fileType != fourccWAVE && fileType != fourccXWMA )
		error(L"APISound::13 File is not a valid WAVE or XWMA file");
    else {
		// No more file-related error handling from this point forward
		// Read in WAVEFORMATEXTENSIBLE from 'fmt ' chunk
		FindChunk(file, fourccFMT, chunkSize, chunkDataPosition);
		ReadChunkData(file, &wfx, chunkSize, chunkDataPosition);

		// Fill out audio data buffer with contents of the fourccDATA chunk
		FindChunk(file, fourccDATA, chunkSize, chunkDataPosition);
		pDataBuffer = new BYTE[chunkSize];
		ReadChunkData(file, pDataBuffer, chunkSize, chunkDataPosition);

		// Populate XAUDIO2_BUFFER, set looping here
		buffer.AudioBytes = chunkSize;
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.pAudioData = pDataBuffer;
		buffer.PlayBegin  = 0;
		buffer.PlayLength = 0;
		buffer.LoopBegin  = 0;
		buffer.LoopLength = 0;
		buffer.LoopCount  = continuous ? XAUDIO2_LOOP_INFINITE : 0;

		// If the file is a XWMA file, then load in the additional buffer
		if (fileType == fourccXWMA) {
			FindChunk(file, fourccDPDS, chunkSize, chunkDataPosition);
			// Divide by 4 to get a DWORD packet count 
            // http://forums.create.msdn.com/forums/t/11568.aspx (Dugan)
			wmaBuffer.PacketCount = chunkSize / 4;

			pDpdsBuffer = new UINT32[chunkSize];
			ReadChunkData(file, pDpdsBuffer, chunkSize, chunkDataPosition);
			wmaBuffer.pDecodedPacketCumulativeBytes = pDpdsBuffer;
		}

		if(FAILED(pXAudio2->CreateSourceVoice( &pSourceVoice, 
         (WAVEFORMATEX*)&wfx, XAUDIO2_VOICE_USEFILTER , 
         XAUDIO2_DEFAULT_FREQ_RATIO, nullptr, nullptr, nullptr)))
			error(L"APISound::14 Failed to create Source Voice");
		else if (fileType == fourccXWMA) {
			if(FAILED(pSourceVoice->SubmitSourceBuffer(&buffer, &wmaBuffer)))
				error(L"APISound::15 Failed to submit XWMA Source Buffer");
		}
		else {
			if(FAILED(pSourceVoice->SubmitSourceBuffer(&buffer)))
				error(L"APISound::16 Failed to submit WAVE Source Buffer ");
            else
                rc = true;
		}
		
		// Setup X3DAUDIO_DSP_SETTINGS and X3DAUDIO_EMITTER if this is a local sound
		// http://msdn.microsoft.com/en-us/library/microsoft.directx_sdk.x3daudio.x3daudio_dsp_settings(v=VS.85).aspx
		// http://msdn.microsoft.com/en-us/library/microsoft.directx_sdk.x3daudio.x3daudio_emitter(v=VS.85).aspx
		if (local) {
			XAUDIO2_DEVICE_DETAILS deviceDetails;
			pXAudio2->GetDeviceDetails(0, &deviceDetails);

			// Clear out the structs
			X3DAUDIO_EMITTER      clear2 = {0};
			X3DAUDIO_DSP_SETTINGS clear3 = {0};
			X3DAUDIO_CONE         clear4 = {0};

            unsigned nInput  = wfx.Format.nChannels;
            unsigned nOutput = deviceDetails.OutputFormat.Format.nChannels;
            matrixSize       = nInput * nOutput;

            matrix = new float[matrixSize]; 
			for (unsigned i = 0; i < matrixSize; i++)
				matrix[i] = 0;
			Emitter     = clear2;
			DSPSettings = clear3;

			DSPSettings.SrcChannelCount     = nInput;
			DSPSettings.DstChannelCount     = nOutput;
			DSPSettings.pMatrixCoefficients = matrix;

			Emitter.ChannelCount = 1;
            // No idea if this is the right setting
			Emitter.CurveDistanceScaler = distanceScale; 
            // If the sound is omni-directional, we don't need a sound cone
			if (outerCone) { 
				cone = clear4;
				cone.OuterAngle  = outerCone * 0.0174532f; // degrees to radians
				cone.OuterVolume = 0.0f; // Silent outside the Outer Cone
				cone.InnerAngle  = innerCone * 0.0174532f; // degrees to radians
				cone.InnerVolume = 1.0f; // Volume doesn't change within Inner Cone
				Emitter.pCone = &cone;
			}
		}
	}
	
	// Close the file if we have a valid handle to it
	if (file != INVALID_HANDLE_VALUE)
		CloseHandle(file);

    return rc;
}

// update updates the position and the orientation of the api sound
//
void APISound::update(const Vector& position, const Vector& front) {

    if (pSourceVoice) {
		X3DAUDIO_VECTOR eFront    = {front.x, front.y, front.z};
		X3DAUDIO_VECTOR ePosition = {position.x, position.y, position.z};
		
		Emitter.OrientFront = eFront;
		Emitter.Position    = ePosition;
		// What about Velocity (Doppler) and OrientTop (apparently only for multi-channel audio?)

		// X3DAudioCalculate() http://msdn.microsoft.com/en-us/library/microsoft.directx_sdk.x3daudio.x3daudiocalculate(v=VS.85).aspx
		X3DAudioCalculate(*pX3DInstance, pListener, &Emitter, X3DAUDIO_CALCULATE_MATRIX,
			&DSPSettings );

		pSourceVoice->SetVolume(volume);
		pSourceVoice->SetFrequencyRatio(frequencyRatio);
		pSourceVoice->SetOutputMatrix(pMasteringVoice, DSPSettings.SrcChannelCount, 
         DSPSettings.DstChannelCount, DSPSettings.pMatrixCoefficients);
	}
}

// play starts playing the voice, sets it up if necessary
//
void APISound::play(const wchar_t* file, const Vector& position, 
 const Vector& heading, bool local, bool continuous) {

    // create the voice if it doesn't yet exist
    if (!pSourceVoice && setup(file, local, continuous) && local)
        update(position, heading);

	if (pSourceVoice) {
		pSourceVoice->Start(0);
        pSourceVoice->SetVolume(volume);
    }
}

// stop stops playing the voice
//
void APISound::stop() {

	if(pSourceVoice) {
        pSourceVoice->SetVolume(0);
		pSourceVoice->Stop(0);
    }
}

// release stops and destroys the voice, deletes the buffers
//
void APISound::release() {

    stop();

    if (pSourceVoice) {
		pSourceVoice->DestroyVoice();
		pSourceVoice = nullptr;
	}

	if (matrix) {
		delete [] matrix;
		matrix = nullptr;
        matrixSize = 0;
	}

	if (pDataBuffer) {
		delete [] pDataBuffer;
		pDataBuffer = nullptr;
	}

	if (pDpdsBuffer) {
		delete [] pDpdsBuffer;
		pDpdsBuffer = nullptr;
	}
}

// destructor releases the voice
//
APISound::~APISound() {

    release();
}

// FindChunk 
//
HRESULT APISound::FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition) {

    HRESULT hr = S_OK;
    if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, 0, nullptr, FILE_BEGIN ) )
        return HRESULT_FROM_WIN32( GetLastError() );

    DWORD dwChunkType;
    DWORD dwChunkDataSize;
    DWORD dwRIFFDataSize = 0;
    DWORD dwFileType;
    DWORD bytesRead = 0;
    DWORD dwOffset = 0;

    while (hr == S_OK)
    {
        DWORD dwRead;
        if( 0 == ReadFile( hFile, &dwChunkType, sizeof(DWORD), &dwRead, nullptr ) )
            hr = HRESULT_FROM_WIN32( GetLastError() );

        if( 0 == ReadFile( hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, nullptr ) )
            hr = HRESULT_FROM_WIN32( GetLastError() );

        switch (dwChunkType)
        {
        case fourccRIFF:
            dwRIFFDataSize = dwChunkDataSize;
            dwChunkDataSize = 4;
            if( 0 == ReadFile( hFile, &dwFileType, sizeof(DWORD), &dwRead, nullptr ) )
                hr = HRESULT_FROM_WIN32( GetLastError() );
            break;

        default:
            if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, dwChunkDataSize, nullptr, FILE_CURRENT ) )
            return HRESULT_FROM_WIN32( GetLastError() );            
        }

        dwOffset += sizeof(DWORD) * 2;
        
        if (dwChunkType == fourcc)
        {
            dwChunkSize = dwChunkDataSize;
            dwChunkDataPosition = dwOffset;
            return S_OK;
        }

        dwOffset += dwChunkDataSize;
        
        if (bytesRead >= dwRIFFDataSize) return S_FALSE;

    }

    
    return S_OK;
    
}

// ReadChunkData
//
HRESULT APISound::ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset)
{
    HRESULT hr = S_OK;
    if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, bufferoffset, nullptr, FILE_BEGIN ) )
        return HRESULT_FROM_WIN32( GetLastError() );
    DWORD dwRead;
    if( 0 == ReadFile( hFile, buffer, buffersize, &dwRead, nullptr ) )
        hr = HRESULT_FROM_WIN32( GetLastError() );
    return hr;
}
