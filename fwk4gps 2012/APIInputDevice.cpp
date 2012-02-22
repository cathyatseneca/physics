/* APIInputDevice Class' Implementation - Translation Layer
 *
 * APIInputDevice.cpp
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "APIInputDevice.h" // for the class definitions
#include "iAPIBase.h"       // for error()
#include "iUtilities.h"     // for strlen()
#include "Translation.h"    // for KEY_* and InputDeviceType enumeration

const GUID GUID_NULL = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

BOOL CALLBACK countInstances(LPCDIDEVICEINSTANCE didesc, void*);
BOOL CALLBACK enumInputDevicesDesc(LPCDIDEVICEINSTANCE didesc, void*);
BOOL CALLBACK enumInputDeviceObjectDesc(LPCDIDEVICEINSTANCE didesc, void*);

//------------------------------- APIInputDeviceDesc --------------------------
//
// The APIInputDeviceDesc class holds the description of an Input Device
//
// constructor retrieves Interface to DirectInput object and initializes
// the instance variable
//
APIInputDeviceDesc::APIInputDeviceDesc() {

    guid       = GUID_NULL;
    desc[0]    = L'\0';
    nObjects   = 0;
    iObject    = 0;
    objectDesc = nullptr;
}

// set stores the GUID and the description of the input device
//
void APIInputDeviceDesc::set(const GUID g, const wchar_t* d) {

    guid = g;
    strcpy(desc, d, MAX_DESC);
}

// set stores the description of the next object on the input device
//
void APIInputDeviceDesc::set(const wchar_t* d) {

    strcpy(objectDesc[iObject++], d, MAX_DESC);
}

// allocates memory for the descriptions of the device's objects
//
void APIInputDeviceDesc::alloc(int n) {

    if (objectDesc)
        delete [] objectDesc;
    objectDesc = new wchar_t[n][MAX_DESC + 1];
}

// destructor deallocates memory
//
APIInputDeviceDesc::~APIInputDeviceDesc() {

    delete [] objectDesc;
}

//------------------------------- APIInputDeviceSet ---------------------------
//
// The APIInputDeviceSet class manages a set of Input Devices at the API level
//
iAPIInputDeviceSet* CreateAPIInputSet(UserDeviceType t) {

    return new APIInputDeviceSet(t);
}

// constructor retrieves Interface to DirectInput object and initializes the
// array of descriptions and the number of devices
//
APIInputDeviceSet::APIInputDeviceSet(UserDeviceType type) {

    // retrieve an Interface to DirectInput object for this application
    di = nullptr;
    if (FAILED(DirectInput8Create((HINSTANCE)application, DIRECTINPUT_VERSION, 
     IID_IDirectInput8, (void**)&di, nullptr))) {
        error(L"APIInputDeviceSet::00 Failed to obtain an Interface to Direct "
         L"Input");
    }
    device   = nullptr;
    nDevices = 0;
    switch (type) {
    case KEYBOARD:
        enumFlags = DI8DEVCLASS_KEYBOARD;
        format    = &c_dfDIKeyboard;
        break;
    case POINTER:
        enumFlags = DI8DEVCLASS_POINTER;
        format    = &c_dfDIMouse2;
        break;
    case CONTROLLER:
        enumFlags = DI8DEVCLASS_GAMECTRL;
        format    = &c_dfDIJoystick2;
        break;
    default:
        error(L"APIInputDeviceSet::01 Invalid Input Device Type ");
    }
}

// interrogate retrieves the descriptions of all attached devices that belong
// to the category specified by flags
//
bool APIInputDeviceSet::interrogate() {

    bool rc = false;
    int count;

    // find the number of attached devices and allocate memory for descriptions
    count = 0;
    di->EnumDevices(enumFlags, (LPDIENUMDEVICESCALLBACK)countInstances, 
     (void*)&count, DIEDFL_ATTACHEDONLY);
    nDevices = count;
    if (nDevices > MAX_INPUT_DEVICES) {
        wchar_t str[MAX_DESC + 1];
        sprintf(str, nDevices, 
         L" Input Devices found - increase MAX_INPUT_DEVICES");
        error(str);
        nDevices = MAX_INPUT_DEVICES;
    }
    if (nDevices) {
	    device = new APIInputDeviceDesc[nDevices];
        // enumerate controller descriptions
        iDevice = 0;
	    di->EnumDevices(enumFlags, 
         (LPDIENUMDEVICESCALLBACK)enumInputDevicesDesc,
         (void*)this, DIEDFL_ATTACHEDONLY);

	    // store each device that creates successfully
        DIDEVICEOBJECTINSTANCE didoi; // holds object info for the device
	    didoi.dwSize = sizeof didoi;
        for (unsigned i = 0; i < nDevices; i++) {
            // create the device temporarily in order to interrogate it
            bool ok = false;
            device[i].select(false, 0u, 0u);
            LPDIRECTINPUTDEVICE8 inputDevice; // pointer to the input device
            GUID guid = device[i].guid;
	        if (guid != GUID_NULL && 
             SUCCEEDED(di->CreateDevice(guid, &inputDevice, nullptr)) && 
             SUCCEEDED(inputDevice->SetDataFormat(format))) {
                count = 0;
                inputDevice->EnumObjects(
                 (LPDIENUMDEVICEOBJECTSCALLBACKW)countInstances, (void*)&count, 
                 DIDFT_ALL);
                device[i].nObjects = count;
                device[i].iObject  = 0;
                if (count) {
                    device[i].objectDesc = new wchar_t[count][MAX_DESC + 1];
                    inputDevice->EnumObjects(
                     (LPDIENUMDEVICEOBJECTSCALLBACKW)enumInputDeviceObjectDesc, 
                     (void**)&device[i], DIDFT_ALL);
			        ok = true;
                }
		        inputDevice->Release();
		        inputDevice = nullptr;
            }
            if (ok) {
                rc = true;
                device[i].select(true, 0u, 0u);
            }
            
        }
    }

    return rc || nDevices == 0;
}

// description returns the address of the user-friendly description of device i
//
const wchar_t* APIInputDeviceSet::description(unsigned i) const { 
    
    return device[i % nDevices].desc; 
}

// release releases the allocated memory for the input device set
//
void APIInputDeviceSet::release() {

    if (device) {
        delete [] device;
        device   = nullptr;
        nDevices = 0;
    }
}

// destructor disengages the interface to the Direct Input object
//
APIInputDeviceSet::~APIInputDeviceSet() {

    release();
    if (di) {
        di->Release();
        di = nullptr;
    }
}

// countInstances increments count for each instance found
//
BOOL CALLBACK countInstances(LPCDIDEVICEINSTANCE didesc, void* count) {

	(*((int*)count))++;

    return DIENUM_CONTINUE;
}

// enumInputDevicesDesc saves the description of an enumerated device and
// increments the device index
//
BOOL CALLBACK enumInputDevicesDesc(LPCDIDEVICEINSTANCE didesc, void* set) {
    
	APIInputDeviceSet* deviceSet  = (APIInputDeviceSet*)set;
    APIInputDeviceDesc* device = deviceSet->nextDevice();
    device->set(didesc->guidInstance, didesc->tszInstanceName);

    return DIENUM_CONTINUE;
}

// enumInputDeviceObjectDesc saves the description of an enumerated device
//
BOOL CALLBACK enumInputDeviceObjectDesc(LPCDIDEVICEINSTANCE didesc, void* dev) 
{
    
    ((APIInputDeviceDesc*)dev)->set(didesc->tszInstanceName);

    return DIENUM_CONTINUE;
}

//------------------------------- APIInputDevice ------------------------------
//
// The APIInputDevice class manages the base part of an Input Device
//
// constructor retrieves Interface to DirectInput object and initializes
// the instance variable
//
APIInputDevice::APIInputDevice(APIInputDeviceDesc* d) : deviceDesc(d) {

    // acquire an Interface to DirectInput object for this application
    di = nullptr;
    if (FAILED(DirectInput8Create((HINSTANCE)application, DIRECTINPUT_VERSION, 
     IID_IDirectInput8, (void**)&di, nullptr))) {
        error(L"APIInputDevice::00 Failed to obtain an Interface to Direct "
         L"Input");
    }

    inputDevice = nullptr;
}

// setup accesses the APIInputDevice, sets its data format and cooperative
// level sets the size of the Input Device buffer and acquires the Input Device
//
void* APIInputDevice::setup(const GUID& guid, LPCDIDATAFORMAT format, 
 DWORD flags) {

    // obtain an interface to the APIInputDevice
    if (FAILED(di->CreateDevice(guid, &inputDevice, nullptr)))
        error(L"APIInputDevice::10 Failed to obtain interface to the device");
    // set the data format for the APIInputDevice data
    else if (FAILED(inputDevice->SetDataFormat(format))) {
        release();
        error(L"APIInputDevice::11 Failed to set data format for the device");
    }
    // set the cooperative level
    else if (FAILED(inputDevice->SetCooperativeLevel((HWND)hwnd, flags))) {
        release();
        error(L"APIInputDevice::12 Failed to set the cooperative level for "
         L"APIInputDevice");
    }

    return inputDevice;
}

// release releases the interface to the input device
//
void APIInputDevice::release() {

    if (inputDevice) {
        inputDevice->Release();
        inputDevice = nullptr;
    }
}

// destructor releases the current object and disengages the interface to
// the Direct Input object
//
APIInputDevice::~APIInputDevice() {

    release();
    if (di) {
        di->Release();
        di = nullptr;
    }
}

//------------------------------- Keyboard ------------------------------------
//
// The Keyboard class manages a keyboard at the API level
//
// CreateAPIKeyboard creates the Keyboard object
//
iAPIInputDevice* CreateAPIKeyboard(APIInputDeviceDesc* d) { 
    
    return new Keyboard(d); 
}

// constructor initializes the keyboard pointer and the key pressings
//
Keyboard::Keyboard(APIInputDeviceDesc* d) : APIInputDevice(d) {
    
    keyboard = nullptr;
    for (int i = 0; i < 256; i++)
        key[i] = false;
}

// setup sets up the keyboard, sets its buffer size and acquires the keyboard
//
bool Keyboard::setup() {

    bool rc  = false;
    keyboard = (LPDIRECTINPUTDEVICE8)APIInputDevice::setup(*guid(), 
     &c_dfDIKeyboard, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

    if (keyboard) {
        keyboard->AddRef();
        // set the size of the keyboard's buffer
        //
        // property struct consists of a header and a data member
        DIPROPDWORD dipdw;
        // property struct header
        // - size of enclosing structure
        dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
        // - always size of DIPROPHEADER
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        // - identifier for property in question - 0 for entire device
        dipdw.diph.dwObj        = 0;
        // - DIPH_DEVICE since entire device is involved
        dipdw.diph.dwHow        = DIPH_DEVICE;
        // property struct data member (takes a single word of data)
        // - the buffer size goes here
        dipdw.dwData            = SAMPLE_BUFFER_SIZE;

        // set the size of the buffer
        if (FAILED(keyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph))) {
            release();
            error(L"Keyboard::13 Failed to set size of keyboard buffer");
        }
        else {
			// try to acquire the keyboard
            HRESULT hr = keyboard->Acquire();
			if (hr == S_OK || hr == S_FALSE || hr == DIERR_OTHERAPPHASPRIO) {
				// clear buffer - this data will be ignored
                update();
                rc = true;
            }
        }
    }

    return rc;
}

// update retrieves the contents of the keyboard's buffer and stores
// key press/release values in keys[] for subsequent polling
//
void Keyboard::update() {

    HRESULT hr;
    DWORD items = SAMPLE_BUFFER_SIZE;
    DIDEVICEOBJECTDATA dod[SAMPLE_BUFFER_SIZE];

    if (keyboard) {
        hr = keyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), dod, 
		 &items, 0);
		// if keyboard is lost, try to re-acquire it
        if (DIERR_INPUTLOST == hr && SUCCEEDED(keyboard->Acquire()))
            hr = keyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), 
			 dod, &items, 0);
        if (SUCCEEDED(hr))
            for (DWORD i = 0; i < items; i++) {
                int k = -1;
                // note that not all keys have been included in this table -
                // only those keys that are mappable as described in the 
                // ModellingLayer header
                //
                switch (dod[i].dwOfs) {
                  case DIK_A: k = KEY_A; break;
                  case DIK_B: k = KEY_B; break;
                  case DIK_C: k = KEY_C; break;
                  case DIK_D: k = KEY_D; break;
                  case DIK_E: k = KEY_E; break;
                  case DIK_F: k = KEY_F; break;
                  case DIK_G: k = KEY_G; break;
                  case DIK_H: k = KEY_H; break;
                  case DIK_I: k = KEY_I; break;
                  case DIK_J: k = KEY_J; break;
                  case DIK_K: k = KEY_K; break;
                  case DIK_L: k = KEY_L; break;
                  case DIK_M: k = KEY_M; break;
                  case DIK_N: k = KEY_N; break;
                  case DIK_O: k = KEY_O; break;
                  case DIK_P: k = KEY_P; break;
                  case DIK_Q: k = KEY_Q; break;
                  case DIK_R: k = KEY_R; break;
                  case DIK_S: k = KEY_S; break;
                  case DIK_T: k = KEY_T; break;
                  case DIK_U: k = KEY_U; break;
	              case DIK_V: k = KEY_V; break;
                  case DIK_W: k = KEY_W; break;
                  case DIK_X: k = KEY_X; break;
                  case DIK_Y: k = KEY_Y; break;
                  case DIK_Z: k = KEY_Z; break;
                  case DIK_1: k = KEY_1; break;
                  case DIK_2: k = KEY_2; break;
                  case DIK_3: k = KEY_3; break;
                  case DIK_4: k = KEY_4; break;
                  case DIK_5: k = KEY_5; break;
                  case DIK_6: k = KEY_6; break;
                  case DIK_7: k = KEY_7; break;
                  case DIK_8: k = KEY_8; break;
                  case DIK_9: k = KEY_9; break;
                  case DIK_0: k = KEY_0; break;
                  case DIK_F1:  k = KEY_F1;  break;
                  case DIK_F2:  k = KEY_F2;  break;
                  case DIK_F3:  k = KEY_F3;  break;
                  case DIK_F4:  k = KEY_F4;  break;
                  case DIK_F5:  k = KEY_F5;  break;
                  case DIK_F6:  k = KEY_F6;  break;
                  case DIK_F7:  k = KEY_F7;  break;
                  case DIK_F8:  k = KEY_F8;  break;
                  case DIK_F9:  k = KEY_F9;  break;
                  case DIK_F10: k = KEY_F10; break;
                  case DIK_F11: k = KEY_F11; break;
                  case DIK_F12: k = KEY_F12; break;
                  case DIK_SPACE : k = KEY_SPACE; break;
                  case DIK_RETURN: k = KEY_ENTER; break;
                  case DIK_UP    : k = KEY_UP;     break;
                  case DIK_DOWN  : k = KEY_DOWN;   break;
                  case DIK_PRIOR : k = KEY_PGUP;  break;
                  case DIK_NEXT  : k = KEY_PGDN;   break;
                  case DIK_LEFT  : k = KEY_LEFT;   break;
                  case DIK_RIGHT : k = KEY_RIGHT;  break;
                  case DIK_NUMPAD1:  k = KEY_NUM1; break;
                  case DIK_NUMPAD2:  k = KEY_NUM2; break;
                  case DIK_NUMPAD3:  k = KEY_NUM3; break;
                  case DIK_NUMPAD4:  k = KEY_NUM4; break;
                  case DIK_NUMPAD5:  k = KEY_NUM5; break;
                  case DIK_NUMPAD6:  k = KEY_NUM6; break;
                  case DIK_NUMPAD7:  k = KEY_NUM7; break;
                  case DIK_NUMPAD8:  k = KEY_NUM8; break;
                  case DIK_NUMPAD9:  k = KEY_NUM9; break;
                  case DIK_ESCAPE    : k = KEY_ESCAPE; break;
	              case DIK_SEMICOLON : k = KEY_SEMICOLON; break;
	              case DIK_APOSTROPHE: k = KEY_APOSTROPHE; break;
	              case DIK_LBRACKET  : k = KEY_O_BRACKET; break;
	              case DIK_RBRACKET  : k = KEY_C_BRACKET; break;
	              case DIK_BACKSLASH : k = KEY_BACKSLASH; break;
	              case DIK_COMMA     : k = KEY_COMMA; break;
	              case DIK_PERIOD    : k = KEY_PERIOD; break;
	              case DIK_SLASH     : k = KEY_SLASH; break;
	              case DIK_MULTIPLY  : k = KEY_TIMES; break;
	              case DIK_GRAVE     : k = KEY_GRAVE; break;
	              case DIK_MINUS     : k = KEY_MINUS; break;
	              case DIK_UNDERLINE : k = KEY_UNDERSCORE; break;
	              case DIK_EQUALS    : k = KEY_EQUALS; break;
	              case DIK_ADD       : k = KEY_PLUS; break;
                }
                if (k != -1) 
                    key[k] = !!(dod[i].dwData & 0x80);
            }
    }
}

// pressed returns the pressed state of key a
//
bool Keyboard::pressed(unsigned k) const { 
    return key[k % 256]; 
}  

// pressed returns the pressed state of the selected key
//
bool Keyboard::pressed() const { 
    return key[selectedObject()]; 
}  

// suspends unacquires the keyboard in preparation for loss of focus
//
void Keyboard::suspend() {

    if (keyboard) 
        keyboard->Unacquire();
}

// restore re-acquires the keyboard on regaining focus
//
bool Keyboard::restore() {

    bool rc = true;

    if (keyboard) {
		HRESULT hr = keyboard->Acquire();
		if (hr != S_OK && hr != S_FALSE && hr != DIERR_OTHERAPPHASPRIO) {
            release();
            error(L"Keyboard::70 Failed to re-acquire the keyboard");
            rc = false;
        }
    }

    return rc;
}

// release unacquires the keyboard and disengages its interface
//
void Keyboard::release() {

    suspend();
	if (keyboard) {
        keyboard->Release();
        keyboard = nullptr;
    }
    APIInputDevice::release();
}

//------------------------------- Pointer -------------------------------------
//
// The Pointer class manages a pointing device at the API level
//
// CreateAPIPointer creates the Pointer object
//
iAPIInputDevice* CreateAPIPointer(APIInputDeviceDesc* d) { 
    
    return new Pointer(d); 
}

// constructor initializes the pointer's address and the state of its
// buttons
//
Pointer::Pointer(APIInputDeviceDesc* d) : APIInputDevice(d) {

    pointer = nullptr;
    for (int i = 0; i < 8; i++)
        button[i] = false;
    for (int i = 0; i < 3; i++)
        motion[i] = 0;
}

// setup accesses the system Pointer, sets its data format and cooperative 
// level, sets the size of the Pointer's buffer and acquires the device
//
bool Pointer::setup() {

    bool rc = false;
    pointer = (LPDIRECTINPUTDEVICE8)APIInputDevice::setup(*guid(), 
     &c_dfDIMouse2, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

    if (pointer) {
        pointer->AddRef();
        // set the size of the Pointer's buffer
        //
        // proerty structure consists of a header and a data member
        DIPROPDWORD dipdw;
        // property header
        // - size of enclosing structure
        dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
        // - always size of DIPROPHEADER
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        // - identifier for property in question - 0 for entire device
        dipdw.diph.dwObj        = 0;
        // - DIPH_DEVICE since entire device is involved
        dipdw.diph.dwHow        = DIPH_DEVICE;
        // property data member (takes a single word of data)
        // - the buffer size goes here
        dipdw.dwData            = SAMPLE_BUFFER_SIZE;

        // set the buffer size
        if (FAILED(pointer->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph))) {
            release();
            error(L"Pointer::13 Failed to set the buffer size");
        }
        // flush the buffer: data currently in buffer will be ignored
        else {
			// try to acquire the keyboard
            HRESULT hr = pointer->Acquire();
			if (hr == S_OK || hr == S_FALSE || hr == DIERR_OTHERAPPHASPRIO) {
				// clear buffer - this data will be ignored
                update();
                rc = true;
            }
        }
    }

    return rc;
}

// update retrieves the contents of the Pointer's buffer and accumulates
// the values for subsequent polling
//
void Pointer::update() {

    HRESULT hr;
    DWORD items = SAMPLE_BUFFER_SIZE;
    DIDEVICEOBJECTDATA dod[SAMPLE_BUFFER_SIZE];

    if (pointer) {
        hr = pointer->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), dod, 
		 &items, 0);
		// try to re-acquire if lost
        if (DIERR_INPUTLOST == hr && SUCCEEDED(pointer->Acquire()))
            hr = pointer->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), dod,
             &items, 0);
        if (SUCCEEDED(hr)) {
            for (int i = 0; i < 3; i++)
                motion[i] = 0;
            for (DWORD i = 0; i < items; i++) {
                switch (dod[i].dwOfs) {
                  case DIMOFS_BUTTON0:
                      button[0] = (dod[i].dwData & 0x80) == 0x80;
                      break;
                  case DIMOFS_BUTTON1:
                      button[1] = (dod[i].dwData & 0x80) == 0x80;
                      break;
                  case DIMOFS_BUTTON2:
                      button[2] = (dod[i].dwData & 0x80) == 0x80;
                      break;
                  case DIMOFS_BUTTON3:
                      button[3] = (dod[i].dwData & 0x80) == 0x80;
                      break;
                  case DIMOFS_BUTTON4:
                      button[4] = (dod[i].dwData & 0x80) == 0x80;
                      break;
                  case DIMOFS_BUTTON5:
                      button[5] = (dod[i].dwData & 0x80) == 0x80;
                      break;
                  case DIMOFS_BUTTON6:
                      button[6] = (dod[i].dwData & 0x80) == 0x80;
                      break;
                  case DIMOFS_BUTTON7:
                      button[7] = (dod[i].dwData & 0x80) == 0x80;
                      break;
                  case DIMOFS_X:
                      motion[0] += dod[i].dwData;
                      break;
                  case DIMOFS_Y:
                      motion[1] += dod[i].dwData;
                      break;
                  case DIMOFS_Z:
                      motion[2] += dod[i].dwData;
                      break;
                }
            }
        }
    }
}

// pressed returns the pressed state of button a
//
bool Pointer::pressed(unsigned b) const { 
    return button[b % 8]; 
}  

// pressed returns the pressed state of the selected button
//
bool Pointer::pressed() const { 
    return button[selectedObject()]; 
}  

// change returns the accumulated change in a
//
int Pointer::change(unsigned a) const { 
    return motion[a % 3]; 
}  

// suspends unacquires the pointer device in preparation for loss of focus
//
void Pointer::suspend() {

    if (pointer) {
        pointer->Unacquire();
        for (int i = 0; i < 8; i++)
            button[i] = false;
        for (int i = 0; i < 3; i++)
            motion[i] = 0;
    }
}

// restore re-acquires the pointer device on regaining focus
//
bool Pointer::restore() {

    bool rc = true;

    if (pointer) {
        HRESULT hr = pointer->Acquire();
		if (hr != S_OK && hr != S_FALSE && hr != DIERR_OTHERAPPHASPRIO) {
            release();
            error(L"Pointer::70 Failed to re-acquire the Pointer");
            rc = false;
        }
        // clear buffer - this data will be ignored
        update();
        for (int i = 0; i < 8; i++)
            button[i] = false;
        for (int i = 0; i < 3; i++)
            motion[i] = 0;
    }

    return rc;
}

// release suspends the pointer and disengages its interface
//
void Pointer::release() {

    suspend();
	if (pointer) {
        pointer->Release();
        pointer = nullptr;
    }
    APIInputDevice::release();
}

//------------------------------- Controller ----------------------------------
//
// The Controller class describes a controller device at the API level
//

// CreateAPIController creates a controller object
//
iAPIInputDevice* CreateAPIController(APIInputDeviceDesc* d) {

	return new Controller(d);
}

// constructor initializes the controller's address and the state of all of its
// objects
//
Controller::Controller(APIInputDeviceDesc* d) : APIInputDevice(d) {

    controller = nullptr;
    axisIsActive[0] = axisIsActive[1] = axisIsActive[2] = axisIsActive[3] = 
     povIsActive = reversey = false;
    for (unsigned i = 0; i < 128; i++)
        button[i] = false;
    for (unsigned i = 0; i < 12; i++)
        motion[i] = 0;
}

// setup accesses the controller, sets up its data format and cooperative
// level, sets the buffer size, and acquires the controller
//
bool Controller::setup() {

    bool rc   = false;

    // retrieve the controller properties
    unsigned flags  = getFlags();
    unsigned button = selectedObject();
    bool none       = flags & 1;
    bool zAxisOn    = !!(flags & 2);
    reversey        = !!(flags & 4);
    if (!none)
        controller = (LPDIRECTINPUTDEVICE8)APIInputDevice::setup(*guid(), 
         &c_dfDIJoystick2, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
    else {
        controller = nullptr;
        rc = true;
    }

    // obtain an interface to the controller
    if (controller) {
        controller->AddRef();
        // retrieve the axes that are active on this device
        DIDEVICEOBJECTINSTANCE didoi;
        didoi.dwSize = sizeof didoi;
        if (SUCCEEDED(controller->GetObjectInfo(&didoi, DIJOFS_X,
         DIPH_BYOFFSET)))
            axisIsActive[0] = true;
        if (SUCCEEDED(controller->GetObjectInfo(&didoi, DIJOFS_Y,
         DIPH_BYOFFSET)))
            axisIsActive[1] = true;
        if (SUCCEEDED(controller->GetObjectInfo(&didoi, DIJOFS_Z,
         DIPH_BYOFFSET)))
            axisIsActive[2] = true;
        if (SUCCEEDED(controller->GetObjectInfo(&didoi, DIJOFS_RZ,
         DIPH_BYOFFSET)))
            axisIsActive[3] = true;
        // ignore what GetObjectInfo returned if we don't want z axis
        if (!zAxisOn) {
            axisIsActive[2] = false;
            axisIsActive[3] = false;
        }

        // Set the range, deadzone, and saturation for each axis

        DIPROPRANGE range;

        range.diph.dwSize = sizeof range;
        range.diph.dwHeaderSize = sizeof range.diph;
        range.diph.dwObj = DIJOFS_X;
        range.diph.dwHow = DIPH_BYOFFSET;
        range.lMin = -100;
        range.lMax =  100;

        DIPROPDWORD dead,
                    sat;

        dead.diph.dwSize = sizeof dead;
        dead.diph.dwHeaderSize = sizeof dead.diph;
        dead.diph.dwObj = DIJOFS_X;
        dead.diph.dwHow = DIPH_BYOFFSET;
        dead.dwData = 300; // hundredths of a percent [0,10000]

        sat = dead;
        sat.dwData = 9800;

        if (axisIsActive[0]) {
            controller->SetProperty(DIPROP_RANGE, &range.diph);
            controller->SetProperty(DIPROP_DEADZONE, &dead.diph);
            controller->SetProperty(DIPROP_SATURATION, &sat.diph);
        }

        if (axisIsActive[1]) {
            range.diph.dwObj = DIJOFS_Y;
            dead.diph.dwObj  = DIJOFS_Y;
            sat.diph.dwObj   = DIJOFS_Y;
            controller->SetProperty(DIPROP_RANGE, &range.diph);
            controller->SetProperty(DIPROP_DEADZONE, &dead.diph);
            controller->SetProperty(DIPROP_SATURATION, &sat.diph);
        }

        if (axisIsActive[2]) {
            range.diph.dwObj = DIJOFS_Z;
            dead.diph.dwObj  = DIJOFS_Z;
            sat.diph.dwObj   = DIJOFS_Z;
            controller->SetProperty(DIPROP_RANGE, &range.diph);
            controller->SetProperty(DIPROP_DEADZONE, &dead.diph);
            controller->SetProperty(DIPROP_SATURATION, &sat.diph);
        }

        if (axisIsActive[3]) {
            range.diph.dwObj = DIJOFS_RZ;
            dead.diph.dwObj  = DIJOFS_RZ;
            sat.diph.dwObj   = DIJOFS_RZ;
            controller->SetProperty(DIPROP_RANGE, &range.diph);
            controller->SetProperty(DIPROP_DEADZONE, &dead.diph);
            controller->SetProperty(DIPROP_SATURATION, &sat.diph);
        }

		// try to acquire the controller
        HRESULT hr = controller->Acquire();
		if (hr == S_OK || hr == S_FALSE || hr == DIERR_OTHERAPPHASPRIO) {
			// clear buffer - this data will be ignored
            update();
            rc = true;
        }
    }

    return rc;
}

// retrieve retrieves the current state of the controller and stores
// the axes' and button values for subsequent polling
//
void Controller::update() {

    HRESULT hr;
    DIJOYSTATE2 state;

    if (controller) {
        // make the current state available
        controller->Poll();
        // retrieve the state of the controller
        hr = controller->GetDeviceState(sizeof(DIJOYSTATE2), &state);
        if (DIERR_INPUTLOST == hr && SUCCEEDED(controller->Acquire()))
            hr = controller->GetDeviceState(sizeof(DIJOYSTATE2), &state);
        if (SUCCEEDED(hr)) {
            // current state components
            if (axisIsActive[0])
                motion[0] = state.lX;
            if (axisIsActive[1])
                motion[1] = reversey ? -state.lY : state.lY;
            if (axisIsActive[2])
                motion[2] = state.lZ;
            if (axisIsActive[3])
                motion[5] = state.lRz;
            if (povIsActive)
                for (int i = 0; i < 4; i++)
                    motion[i + 8] = state.rgdwPOV[i];
            // buttons currently pressed
            for (int i = 0; i < 128; i++)
                button[i] = (state.rgbButtons[i] & 0x80) != 0;
        }
    }
}

// pressed returns the pressed state of button b
//
bool Controller::pressed(unsigned b) const { 
    return button[b % 128]; 
}  

// pressed returns the pressed state of the selected button
//
bool Controller::pressed() const { 
    return button[selectedObject()]; 
}  

// change returns the current change in a
//
int Controller::change(unsigned a) const { 
    return motion[a % 12]; 
}  

// suspends unacquires the device in preparation for loss of focus
//
void Controller::suspend() {

    if (controller) 
        controller->Unacquire();
}

// restore re-acquires the device on regaining focus
//
bool Controller::restore() {

    bool rc = true;

    if (controller) {
		HRESULT hr = controller->Acquire();
		if (hr != S_OK && hr != S_FALSE && hr != DIERR_OTHERAPPHASPRIO) {
            release();
            error(L"Controller::70 Failed to re-acquire the controller");
            rc = false;
        }
        else {
            // clear buffer - this data will be ignored
            update();
            for (unsigned i = 0; i < 128; i++)
                button[i] = false;
            for (unsigned i = 0; i < 12; i++)
                motion[i] = 0;
        }
    }

    return rc;
}

// release suspends the controller and detaches its interface
//
void Controller::release() {

    suspend();
	if (controller) {
        controller->Release();
        controller = nullptr;
    }
    APIInputDevice::release();
}
