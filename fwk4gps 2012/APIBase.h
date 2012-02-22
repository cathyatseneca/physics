#ifndef _API_BASE_H_
#define _API_BASE_H_

/* API Base Definition - Translation Layer
 *
 * APIBase.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iAPIBase.h" // for the APIBase Interface

//-------------------------------- APIBase ------------------------------------
//
// The APIBase class manages the connections to the underlying APIs
//
class  iAPIDisplaySet;
class  iAPIInputDeviceSet;
class  iAPIDisplay;
class  iAPIAudio;
class  iAPIWindow;
struct IDirect3D9;
struct IDirect3DDevice9;
struct ID3DXSprite;
struct IXAudio2;
struct IXAudio2MasteringVoice;
struct X3DAUDIO_LISTENER;
typedef unsigned char X3DAUDIO_HANDLE[20]; // from x3daudio.h

class APIBase : public iAPIBase {

protected:

    static iAPIDisplaySet*         displaySet;    // the attached displays
    static iAPIInputDeviceSet*     keyboardSet;   // the attached keyboards
    static iAPIInputDeviceSet*     pointerSet;    // the attached pointers
    static iAPIInputDeviceSet*     controllerSet; // the attached controllers

    static iAPIDisplay*            display;       // the graphics card object
    static iAPIAudio*              audio;         // the sound card object
    static iAPIWindow*             window;        // the application window

    static void*                   application; // points to the application
    static void*                   hwnd;        // handle to application window

    static IDirect3D9*             d3d;         // points to Direct3D object
    static IDirect3DDevice9*       d3dd;        // points to Direct3D display
    static ID3DXSprite*            manager;     // points to sprite manager 
    static int                     width;       // width of the client area
    static int                     height;      // height of the client area
    static bool                    runinwndw;   // running in a window?

    static IXAudio2*               pXAudio2;		// XAudio2 engine
	static X3DAUDIO_HANDLE*        pX3DInstance;	// X3DAudio constants
	static X3DAUDIO_LISTENER*      pListener;		// camera's frame
	static IXAudio2MasteringVoice* pMasteringVoice; // masteringVoice
                                   
    static float                   volume;
    static float                   frequencyRatio;
    static float                   distanceScale; // scale to user units

    virtual ~APIBase()             { }

public:
    void render()                  { }
    void suspend()                 { }
    bool restore()                 { return true; }
    void release()                 { }
    void Delete() const            { delete this; }
    void error(const wchar_t*, const wchar_t* = 0) const;
    void logError(const wchar_t*) const;
};

#endif
