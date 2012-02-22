#ifndef _API_INPUT_DEVICE_H_
#define _API_INPUT_DEVICE_H_

/* Input Device Definition - Translation Layer
 *
 * APIInputDevice.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#define  WIN32_LEAN_AND_MEAN
#define  DIRECTINPUT_VERSION 0x0800
#define  INITGUID             // enables access to GUIDs used by DirectInput
#include <dinput.h>           // for basic Direct Input
#undef   INITGUID

#include "iAPIInputDevice.h"  // for the APIInputDevice Interface
#include "APIBase.h"          // for the APIBase class definition
#include "GeneralConstants.h" // for MAX_DESC

//------------------------------- APIInputDevice ---------------------------------
//
// The APIInputDevice class implements the APIInputDevice at the API level
//
class APIInputDeviceSet;

class APIInputDeviceDesc {
	GUID     guid;
	wchar_t  desc[MAX_DESC + 1];
    unsigned nObjects;
    unsigned iObject;
    unsigned flags;
    wchar_t  (*objectDesc)[MAX_DESC + 1];
    bool     isSelected;
  public:
    APIInputDeviceDesc();
    void select(bool s, unsigned i, unsigned f)   { isSelected = s; iObject = i; flags = f; }
    bool selected() const { return isSelected; }
    ~APIInputDeviceDesc();
    void alloc(int);
    void set(const GUID, const wchar_t*);
    void set(const wchar_t*); 
    friend class APIInputDeviceSet;
    friend class APIInputDevice;
};

//------------------------------- APIInputDeviceSet ---------------------------------
//
// The APIInputDeviceSet class manages a set of input devices at the API level
//
class APIInputDeviceSet : public iAPIInputDeviceSet, public APIBase {

    LPDIRECTINPUT8   di;        // points to the Direct Input object
    unsigned         nDevices;  // number of attached devices
    unsigned         iDevice;   // index of current device being interrogated
    unsigned         enumFlags; // enumeration flags
    LPCDIDATAFORMAT  format;    // format of data packet
	APIInputDeviceDesc* device;    // points to descriptions of attached devices

	APIInputDeviceSet(const APIInputDeviceSet& k);            // prevents copying
	APIInputDeviceSet& operator=(const APIInputDeviceSet& k); // prevents assignment
    virtual ~APIInputDeviceSet();

  public:
    APIInputDeviceSet(UserDeviceType);
    bool interrogate();
    unsigned noAttached() const     { return (int)nDevices; }
    void select(unsigned i, unsigned o, unsigned f) { device[i].select(true, o, f); }
    bool selected(unsigned i) const { return device[i].selected(); }
    APIInputDeviceDesc* nextDevice()   { return &device[iDevice++]; }
    APIInputDeviceDesc* desc(unsigned i) const { return &device[i]; }
    unsigned noObjects(unsigned i) const { return device[i].nObjects; }
    const wchar_t* description(unsigned i, unsigned j) const { 
     return device[i].objectDesc[j]; }
    const wchar_t* description(unsigned i) const;
    void release();
    void Delete() const { delete this; }
};

//------------------------------- APIInputDevice ------------------------------------
//
// The APIInputDevice class manages an input device at the API level
//
class APIInputDevice : public iAPIInputDevice, public APIBase {

    LPDIRECTINPUT8       di;          // points to the Direct Input object
    LPDIRECTINPUTDEVICE8 inputDevice; // points to Direct Input Device
    APIInputDeviceDesc*     deviceDesc;  // points to the device description

	APIInputDevice(const APIInputDevice& k);            // prevents copying
	APIInputDevice& operator=(const APIInputDevice& k); // prevents assignment

  protected:
    APIInputDevice(APIInputDeviceDesc*);
    GUID* guid() const { return &deviceDesc->guid; }
    void* setup(const GUID& guid, LPCDIDATAFORMAT format, DWORD flags);
    unsigned getFlags() const { return deviceDesc->flags; }
    unsigned selectedObject() const { return deviceDesc->iObject; }
    void release();
    virtual ~APIInputDevice();
};

//------------------------------- Keyboard ------------------------------------
//
// The Keyboard class manages a keyboard at the API level
//
class Keyboard : public APIInputDevice {

	static const int SAMPLE_BUFFER_SIZE =  30;

    LPDIRECTINPUTDEVICE8 keyboard;     // points to the Direct Input Keyboard
    bool                 key[256];     // the current key states

	Keyboard(const Keyboard& k);            // prevents copying
	Keyboard& operator=(const Keyboard& k); // prevents assignment

  public:
    Keyboard(APIInputDeviceDesc*);
	// initialization
    bool setup();
	// execution
    void update();
    bool pressed(unsigned a) const;
    bool pressed() const;
    int  change(unsigned a) const { return 0; }
    // suspend execution
    void suspend();
    bool restore();
	// termination
    void release();
	void Delete() const { delete this; }
};

//------------------------------- Pointer ---------------------------------------
//
// The Pointer class manages a Pointer device at the API level
//
class Pointer : public APIInputDevice {

	static const int SAMPLE_BUFFER_SIZE = 30;

    LPDIRECTINPUTDEVICE8 pointer;   // points to the Direct Input Pointer
    bool                 button[8]; // the current button states
    int                  motion[3]; // the accumulated motion since last poll

	Pointer(const Pointer& m);            // prevents copying
	Pointer& operator=(const Pointer& m); // prevents assignment

  public:
    Pointer(APIInputDeviceDesc*);
	// initialization
    bool setup();
	// execution
    void update();
    bool pressed(unsigned a) const;  
    bool pressed() const;
    int  change(unsigned a) const;
    // suspend execution
    void suspend();
    bool restore();
    // termination
    void release();
	void Delete() const { delete this; }
};

//---------------------------------- Controller -------------------------------
//
// The Controller class manages a controller at the API level
//
class Controller : public APIInputDevice {

    LPDIRECTINPUTDEVICE8 controller; // points to the Direct Input controller

    // execution
    bool axisIsActive[4];            // is axis active?
    bool povIsActive;                // point of view is active?
    bool reversey;                   // reverse direction of y axis
    int  motion[12];                 // current state of controller
    bool button[128];                // the current button state

	Controller(const Controller& j);            // prevents copying
	Controller& operator=(const Controller& j); // prevents assignment

  public:
    Controller(APIInputDeviceDesc*);
    // initialization
    bool setup();
	// execution
    void update();
    bool pressed(unsigned a) const;  
    bool pressed() const;
    int  change(unsigned a) const;
    // suspend execution
    void suspend();
    bool restore();
    // termination
    void release();
	void Delete() const { delete this; }
};

#endif
