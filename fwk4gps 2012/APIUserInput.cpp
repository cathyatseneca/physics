/* APIUserInput Implementation - Translation Layer
 *
 * APIUserInput.cpp
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "APIUserInput.h"      // for the APIUserInput class definition
#include "iAPIInputDevice.h"         // for the APIInputDevice Interfaces
#include "iAPIDisplay.h"       // for the APIDisplaySet, APIDisplay Interfaces
#include "iAPIAudio.h"         // for the APIAudio Interface
#include "iUtilities.h"        // for strlen, etc.
#include "Translation.h"       // for local Translation Layer constants
#include "Common_Symbols.h"    // for Action and Sound enumerations
#include "Mappings.h"          // for mappings of actions and sounds
#include "DialogResource.h"    // for IDC_???

// Run in a window combo box line item descriptions
#define RUN_IN_WINDOW_DESC L"Run in a window"
#define RESOLUTION_DESC    L""
#define RUN_IN_WINDOW -1
#define NO_CONTROLLER_DESC L"No Controller"
#define TRIGGER_DESC       L""
#define NO_CONTROLLER -1
// macros for action mappings
#define PTROBJ(x) (((x) & 0xFF00) >> 8)
#define KEYOBJ(x) ((x) >> 16)
#define CTROBJ(x) ((x) & 0xFF)
#define KEYPTRCTR(k, p, c) ((k) << 16 | (p) << 8 | (c))
#define KEYPTRCTR_(m) KEYPTRCTR(m[KEYBOARD], m[POINTER], m[CONTROLLER])

BOOL CALLBACK dlgProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

//-------------------------------- APIUserInput ------------------------------
//
// The APIUserInput object collects the configuration information from the user
//
iAPIUserInput* APIUserInput::address = nullptr;
iAPIUserInput* APIUserInputAddress() { return APIUserInput::Address(); }

// CreateAPIUserDialog creates the APIUserInput Object
//
iAPIUserInput* CreateAPIUserInput(const wchar_t* a) {

    return APIUserInputAddress() ? APIUserInputAddress() : 
     new APIUserInput(a);
}

// constructor creates the device interrogators, initializes the selections,
// and loads the user friendly descriptions
//
APIUserInput::APIUserInput(const wchar_t* a) : audioDirectory(a) {

    address       = this;
    hwnd          = nullptr;

    keyboardSet   = CreateAPIInputSet(KEYBOARD);
    pointerSet    = CreateAPIInputSet(POINTER);
    controllerSet = CreateAPIInputSet(CONTROLLER);
    displaySet    = CreateAPIDisplaySet();
    keyboard      = nullptr;
    pointer       = nullptr;
    controller    = nullptr;
    nKeyboards    = 0;
    nPointers     = 0;
    nControllers  = 0;

    // default memory string settings
    strcpy(dispStr,  RUN_IN_WINDOW_DESC, MAX_DESC);
    strcpy(modeStr,  L"", MAX_DESC);
    strcpy(cntlrStr, NO_CONTROLLER_DESC, MAX_DESC);
    strcpy(trigStr,  L"", MAX_DESC);
    action_     = 0;
    sound       = 0;
    flags       = 0;

    displayId   = 0;      
    modeId      = 0;         
    pixelId     = 0;        
    runinwndw   = true;      

    // allocate memory for configurable action descriptions
	const wchar_t* actDesc[] = ACTION_DESCRIPTIONS;
	nActions = sizeof actDesc / sizeof(wchar_t*);
    actionDesc = new wchar_t[nActions][MAX_DESC + 1];
    for (unsigned i = 0; i < nActions; i++)
        strcpy(actionDesc[i], actDesc[i], MAX_DESC);

    // allocate memory for configurable keyboard object descriptions
	const wchar_t* kbdDesc[] = KEY_DESCRIPTIONS;
	nKbdObjs= sizeof kbdDesc / sizeof(wchar_t*);
    kbdObjDesc = new wchar_t[nKbdObjs][MAX_DESC + 1];
    for (unsigned i = 0; i < nKbdObjs; i++)
        strcpy(kbdObjDesc[i], kbdDesc[i], MAX_DESC);

    // allocate memory for configurable pointer object descriptions
	const wchar_t* ptrDesc[] = POINTER_OBJECT_DESCRIPTIONS;
	nPtrObjs = sizeof ptrDesc / sizeof(wchar_t*);
    ptrObjDesc = new wchar_t[nPtrObjs][MAX_DESC + 1];
    for (unsigned i = 0; i < nPtrObjs; i++)
        strcpy(ptrObjDesc[i], ptrDesc[i], MAX_DESC);

    // allocate memory for configurable controller object descriptions
	const wchar_t* ctrDesc[] = CONTROLLER_OBJECT_DESCRIPTIONS;
	nCtrObjs = sizeof ctrDesc / sizeof(wchar_t*);
    ctrObjDesc = new wchar_t[nCtrObjs][MAX_DESC + 1];
    for (unsigned i = 0; i < nCtrObjs; i++)
        strcpy(ctrObjDesc[i], ctrDesc[i], MAX_DESC);

    // populate key-action translation list and initialize other lists
    action = new unsigned[nKbdObjs][3];
    Key kbdKey[] = ACTION_KEY_MAP;
    for (unsigned i = 0; i < nKbdObjs; i++) {
        action[i][KEYBOARD]   = kbdKey[i] + 1;
        action[i][POINTER]    = 0;
        action[i][CONTROLLER] = 0;
    }
    // populate pointer-object action translation list
    //Action ptrAct[] = PTR_OBJ_ACTION_MAP;
    //for (unsigned i = 0; i < nPtrObjs; i++)
    //    action[ptrAct[i]][POINTER] = i + 1;
    // populate controller-object action translation list
    //Action ctrAct[] = CTR_OBJ_ACTION_MAP;
    //for (unsigned i = 0; i < nCtrObjs; i++)
    //    action[ctrAct[i]][CONTROLLER] = i + 1;

    // allocate memory for sound descriptions and initial sound mappings
	const wchar_t* soundDesc[] = SOUND_DESCRIPTIONS;
	const wchar_t* defFile[]   = SOUND_MAPPINGS;
	nSounds = sizeof soundDesc / sizeof (wchar_t*);
    sndDesc = new wchar_t[nSounds][MAX_DESC + 1];
    sndFile = new wchar_t[nSounds][MAX_DESC + 1];
    for (unsigned i = 0; i < nSounds; i++) {
        strcpy(sndDesc[i], soundDesc[i], MAX_DESC);
        strcpy(sndFile[i],   defFile[i], MAX_DESC);
    }
}

// getConfiguration interrogates the host for the attached devices, displays a
// DialogBox and gets the configuration selections from the user - returns true
// if user has pressed GO, false if user has cancelled
//
bool APIUserInput::getConfiguration() {

    bool rc;

    // interrogate the host for all of the attached devices
    keyboardSet->interrogate();
    pointerSet->interrogate();
    controllerSet->interrogate();
    displaySet->interrogate();

    // request the user's selections
    rc = DialogBox((HINSTANCE)application, MAKEINTRESOURCE(IDD_DLG), nullptr, 
     dlgProc) == TRUE;
    hwnd = nullptr;

    return rc;
}

// configure allocates memory for the connections to the selected input devices
// creates the input devices and configures the display and audio devices
//
void APIUserInput::configure() {

    // deallocate any previously allocated memory
    delete [] keyboard;
    delete [] pointer;
    delete [] controller;

    // selected keyboards
    nKeyboards = 0;
    for (unsigned i = 0; i < keyboardSet->noAttached(); i++)
        if (keyboardSet->selected(i))
            nKeyboards++;
    if (nKeyboards) {
        keyboard = new iAPIInputDevice*[nKeyboards];
        unsigned j = 0;
        for (unsigned i = 0; i < keyboardSet->noAttached(); i++)
            if (keyboardSet->selected(i))
                keyboard[j++] = CreateAPIKeyboard(keyboardSet->desc(i));
    }
    // selected pointers
    nPointers = 0;
    for (unsigned i = 0; i < pointerSet->noAttached(); i++)
        if (pointerSet->selected(i))
            nPointers++;
    if (nPointers) {
        pointer = new iAPIInputDevice*[nPointers];
        unsigned j = 0;
        for (unsigned i = 0; i < pointerSet->noAttached(); i++)
            if (pointerSet->selected(i))
                pointer[j++] = CreateAPIPointer(pointerSet->desc(i));
    }
    // selected controllers
    nControllers = 0;
    for (unsigned i = 0; i < controllerSet->noAttached(); i++)
        if (controllerSet->selected(i))
            nControllers++;
    if (nControllers) {
        controller = new iAPIInputDevice*[nControllers];
        unsigned j = 0;
        for (unsigned i = 0; i < controllerSet->noAttached(); i++)
            if (controllerSet->selected(i))
                controller[j++] = CreateAPIController(controllerSet->desc(i));
    }
    // configure display and set its client area dimensions
    if (!runinwndw) {
        display->configure(displayId, modeId, pixelId);
        width  = displaySet->getWidth(displayId, modeId, pixelId);
        height = displaySet->getHeight(displayId, modeId, pixelId);
    }
            wchar_t str[MAX_DESC + 1];
            sprintf(str, width, L" in userInput ");
            error(str);
    // configure audio
    // audio->configure(...); // nothing to configure presently
}

// setup sets up the selected input devices
//
bool APIUserInput::setup() {

    bool rc = false;

    if (keyboard)
        for (unsigned i = 0; i < nKeyboards; i++)
            keyboard[i]->setup();
    if (pointer)
	    for (unsigned i = 0; i < nPointers; i++)
            pointer[i]->setup();
    if (controller)
	    for (unsigned i = 0; i < nControllers; i++)
            controller[i]->setup();

    return true;
}

// update collects the most recent input
//
void APIUserInput::update() {

    if (keyboard)
        for (unsigned i = 0; i < nKeyboards; i++)
            keyboard[i]->update();
    if (pointer)
	    for (unsigned i = 0; i < nPointers; i++)
            pointer[i]->update();
    if (controller)
	    for (unsigned i = 0; i < nControllers; i++)
            controller[i]->update();
}

// pressed returns the on/off status of Action a
//
bool APIUserInput::pressed(Action a) const {

    bool rc = false;

    if (keyboard)
        for (unsigned i = 0; !rc && i < nKeyboards; i++) {
            if (action[a][KEYBOARD])
                rc = keyboard[i]->pressed(action[a][KEYBOARD] - 1);
        }
    if (pointer)
        for (unsigned i = 0; !rc && i < nPointers; i++) {
            if (action[a][POINTER])
                rc = pointer[i]->pressed(action[a][POINTER] - 1);
        }
    if (controller)
        for (unsigned i = 0; !rc && i < nControllers; i++) {
            if (action[a][CONTROLLER])
                rc = controller[i]->pressed(action[a][CONTROLLER] - 1);
        }

    return rc;
}

// ptrPressed returns the on/off status of Action a
//
bool APIUserInput::ptrPressed() const {

    bool rc = false;

    if (pointer)
        for (unsigned i = 0; !rc && i < nPointers; i++)
            rc = pointer[i]->pressed();

    return rc;
}

// ctrPressed returns the on/off status of Action a
//
bool APIUserInput::ctrPressed() const {

    bool rc = false;

    if (controller)
        for (unsigned i = 0; !rc && i < nControllers; i++)
            rc = controller[i]->pressed();

    return rc;
}

// change returns the change in Action a 
//
int APIUserInput::change(Action a) const {

    int rc = 0;
    
    if (pointer)
        for (unsigned i = 0; !rc && i < nPointers; i++) {
            if (action[a][POINTER])
                rc = pointer[i]->change(action[a][POINTER] - 1);
        }
    if (controller)
        for (unsigned i = 0; !rc && i < nControllers; i++) {
            if (action[a][CONTROLLER])
                rc = controller[i]->change(action[a][CONTROLLER] - 1);
        }

    return rc;
}

// suspend prepares the application for loss of focus
//
void APIUserInput::suspend() {

	if (controller)
        for (unsigned i = 0; i < nControllers; i++)
            controller[i]->suspend();
	if (pointer)
        for (unsigned i = 0; i < nPointers; i++)
            pointer[i]->suspend();
	if (keyboard)
        for (unsigned i = 0; i < nKeyboards; i++)
            keyboard[i]->suspend();
}

// restore prepares the application as it regains focus
//
bool APIUserInput::restore() {

	if (keyboard)
        for (unsigned i = 0; i < nKeyboards; i++)
            keyboard[i]->restore();
	if (pointer)
	    for (unsigned i = 0; i < nPointers; i++)
            pointer[i]->restore();
	if (controller)
	    for (unsigned i = 0; i < nControllers; i++)
            controller[i]->restore();

    return true;
}

// release destroys the selected input devices and deallocates the memory
// for their addresses
//
void APIUserInput::release() {

    for (unsigned i = 0; i < nControllers; i++)
        controller[i]->Delete();
    delete [] controller;
    controller = nullptr;
    for (unsigned i = 0; i < nPointers; i++)
        pointer[i]->Delete();
    delete [] pointer;
    pointer = nullptr;
    for (unsigned i = 0; i < nKeyboards; i++)
        keyboard[i]->Delete();
    delete [] keyboard;
    keyboard = nullptr;
}

// destructor destroys display and audio objects and the input sets
//
APIUserInput::~APIUserInput() {

    displaySet->Delete();
    controllerSet->Delete();
    pointerSet->Delete();
    keyboardSet->Delete();

    delete [] action;
    delete [] actionDesc;
    delete [] kbdObjDesc;
    delete [] ptrObjDesc;
    delete [] ctrObjDesc;
}

//-------------------------------- DialogBox functions ------------------------
//

// populate the user dialog with the available configurations 
//
void APIUserInput::populateAPIUserDialog(void* hwnd) {

    // store window handle for error message box
    this->hwnd = hwnd;

    // populate the adapter combo box
    populateAdapterList(hwnd);
	// populate the action combo box
	populateActionList(hwnd);
    // populate the mappable object combo boxes
    populateMappableObjectLists(hwnd);
	// populate the configurable sound file list
	populateSoundFileList(hwnd);
    // populate the controller combo box
    populateControllerList(hwnd);
}

// populateAdapterList populates the adapter combo box with a list of the
// installed apiDisplay adapters
//
void APIUserInput::populateAdapterList(void* hwndw) {

    int dev; // index of the current line item
    HWND hwnd = (HWND)hwndw; // handle to current window

    // empty the display adapter combo box
    SendDlgItemMessage(hwnd, IDC_DIS, CB_RESETCONTENT, 0, 0L);
    // retrieve the number of apiDisplay adapters available
    int nd = displaySet->noAdapters();
    for (int id = 0; id < nd; id++) {
		// retrieve adapter id's description
        if (displaySet->adapterDesc(id)[0]) {
            // add the description and the index to adapter combo box
			dev = SendDlgItemMessage(hwnd, IDC_DIS, CB_ADDSTRING, 0,
             (LPARAM)displaySet->adapterDesc(id));
            SendDlgItemMessage(hwnd, IDC_DIS, CB_SETITEMDATA, dev, id);
        }
    }
    // append the option of running in a window
    dev = SendDlgItemMessage(hwnd, IDC_DIS, CB_ADDSTRING, 0,
     (LPARAM)RUN_IN_WINDOW_DESC);
    SendDlgItemMessage(hwnd, IDC_DIS, CB_SETITEMDATA, dev, 
     RUN_IN_WINDOW);

    // set the default display adapter to the previously selected adapter
	dev = SendDlgItemMessage(hwnd, IDC_DIS, CB_FINDSTRINGEXACT, -1,
        (LPARAM)dispStr);
	if (dev == CB_ERR) dev = 0;
    SendDlgItemMessage(hwnd, IDC_DIS, CB_SETCURSEL, dev, 0L);
}

// populateActionList populates the action combo box with a list of
// the actions that can have their keys configured
//
void APIUserInput::populateActionList(void* hwndw) {

    HWND hwnd = (HWND)hwndw; // handle to current window

    // empty the action combo box
	SendDlgItemMessage(hwnd, IDC_ACT, CB_RESETCONTENT, 0, 0L);
    // retrieve the number of predefined actions
    for (unsigned a = 0; a < nActions; a++) {
        // add the description and key mapping to the action combo box
		int i = SendDlgItemMessage(hwnd, IDC_ACT, CB_ADDSTRING, 0, 
		 (LPARAM)actionDesc[a]);
        SendDlgItemMessage(hwnd, IDC_ACT, CB_SETITEMDATA, i, 
		 (LPARAM)KEYPTRCTR_(action[a]));
    }
    SendDlgItemMessage(hwnd, IDC_ACT, CB_SETCURSEL, action_, 0L);
}

// populateMappableObjectLists populates the mappable object combo boxes
// with lists of all of the configurable objects
//
void APIUserInput::populateMappableObjectLists(void* hwndw) {

    HWND hwnd = (HWND)hwndw; // handle to current window

    // empty the mappable key combo box
    SendDlgItemMessage(hwnd, IDC_KEY, CB_RESETCONTENT, 0, 0L);
    SendDlgItemMessage(hwnd, IDC_KEY, CB_ADDSTRING, 0, (LPARAM)L"----");
    // retrieve the number of mappable keys
    for (unsigned i = 0; i < nKbdObjs; i++)
        // add the key's description to the mappable key combo box
		SendDlgItemMessage(hwnd, IDC_KEY, CB_ADDSTRING, 0, 
		 (LPARAM)kbdObjDesc[i]);

    // empty the mappable pointer combo box
    SendDlgItemMessage(hwnd, IDC_PTO, CB_RESETCONTENT, 0, 0L);
    SendDlgItemMessage(hwnd, IDC_PTO, CB_ADDSTRING, 0, (LPARAM)L"----");
    // retrieve the number of mappable pointer objects
    for (unsigned i = 0; i < nPtrObjs; i++)
        // add the object's description to the mappable object combo box
		SendDlgItemMessage(hwnd, IDC_PTO, CB_ADDSTRING, 0, 
		 (LPARAM)ptrObjDesc[i]);

    // empty the mappable controller object combo box
    SendDlgItemMessage(hwnd, IDC_CTO, CB_RESETCONTENT, 0, 0L);
    SendDlgItemMessage(hwnd, IDC_CTO, CB_ADDSTRING, 0, (LPARAM)L"----");
    // retrieve the number of mappable controller objects
    for (unsigned i = 0; i < nCtrObjs; i++)
        // add the object's description to the mappable object combo box
		SendDlgItemMessage(hwnd, IDC_CTO, CB_ADDSTRING, 0, 
		 (LPARAM)ctrObjDesc[i]);

    // retrieve the index for the selected action
    int f = SendDlgItemMessage(hwnd, IDC_ACT, CB_GETCURSEL, 0, 0L);
	// find the current key for the selected action
	unsigned data = SendDlgItemMessage(hwnd, IDC_ACT, CB_GETITEMDATA, f, 0L);
	if (data == CB_ERR) data = 0;
    // set the cursor to the current object for the selected action
    SendDlgItemMessage(hwnd, IDC_KEY, CB_SETCURSEL, KEYOBJ(data), 0L);
    SendDlgItemMessage(hwnd, IDC_PTO, CB_SETCURSEL, PTROBJ(data), 0L);
    SendDlgItemMessage(hwnd, IDC_CTO, CB_SETCURSEL, CTROBJ(data), 0L);

	// enable the mappable object combo boxes
	EnableWindow(GetDlgItem(hwnd, IDC_KEY), TRUE);
	EnableWindow(GetDlgItem(hwnd, IDC_PTO), TRUE);
	EnableWindow(GetDlgItem(hwnd, IDC_CTO), TRUE);
}

// populateSoundFileList populates the sound file combo box with a
// list of the currently stored sound files, populates the sound
// combo box with the configurable sound descriptions and ties each
// configurable sound to the previously selected sound file
//
void APIUserInput::populateSoundFileList(void* hwndw) {

    HWND hwnd = (HWND)hwndw; // handle to current window

    WIN32_FIND_DATA ffd;
	wchar_t directory[MAX_DESC+1];
	HANDLE handle = INVALID_HANDLE_VALUE;
	unsigned length;
	bool keepsearching;

	// empty the available sound file combo box
	SendDlgItemMessage(hwnd, IDC_SFL, CB_RESETCONTENT, 0, 0L);
    // interrogate the audio directory for sound files
	length = wcslen(audioDirectory);
	if (length > MAX_DESC - 3)
		error(L"APIUserInput::10 Audio Directory name is too long");
	else {
		strcpy(directory, audioDirectory, MAX_DESC);
		strcat(directory, L"\\*", MAX_DESC);
		handle = FindFirstFile(directory, &ffd);
		keepsearching = handle != INVALID_HANDLE_VALUE;
		bool foundAFile = false;
		while (keepsearching) {
			if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				if (wcslen(ffd.cFileName) < MAX_DESC) {
					// add the file name to the sound file combo box
					SendDlgItemMessage(hwnd, IDC_SFL, CB_ADDSTRING, 0, 
					 (LPARAM)ffd.cFileName);
					foundAFile = true;
				}
			}
			keepsearching = FindNextFile(handle, &ffd) != 0;
		}
		if (foundAFile) {
			SendDlgItemMessage(hwnd, IDC_SFL, CB_SETCURSEL, 0, 0L);
			// enable the sound file combo box
			EnableWindow(GetDlgItem(hwnd, IDC_SFL), TRUE);
		}
	}

    // empty the configurable sound combo box
	SendDlgItemMessage(hwnd, IDC_AUD, CB_RESETCONTENT, 0, 0L);

    // populate the configurable sound combo box with a list of 
	// configurable sounds
    //
    for (unsigned msnd = 0; msnd < nSounds; msnd++) {
        // add the sound description to the configurable sound combo box
		SendDlgItemMessage(hwnd, IDC_AUD, CB_ADDSTRING, 0, 
		 (LPARAM)sndDesc[msnd]);
		// find the previously selected match in the sound file list and
		// store the sound file item index in the sound list data item
		// default to the top of the list, if match not found
		int i = 0;
		bool found = false;
		const wchar_t* storedFilename = sndFile[msnd];
		int nf = SendDlgItemMessage(hwnd, IDC_SFL, CB_GETCOUNT, 0, 0L);
		wchar_t availableFile[MAX_DESC + 1] = L"";
		for (int file = 0; file < nf && !found; file++) {
			SendDlgItemMessage(hwnd, IDC_SFL, CB_GETLBTEXT, file, 
			 (LPARAM)availableFile);
			if (!wcscmp(availableFile, storedFilename)) {
				found = true;
				i = file;
			}
		}
		SendDlgItemMessage(hwnd, IDC_AUD, CB_SETITEMDATA, msnd, i);
    }

	// set the cursor to the previously selected sound file
    SendDlgItemMessage(hwnd, IDC_AUD, CB_SETCURSEL, sound, 0L);
    int file = SendDlgItemMessage(hwnd, IDC_AUD, CB_GETITEMDATA, 
     sound, 0L);
    SendDlgItemMessage(hwnd, IDC_SFL, CB_SETCURSEL, file, 0L);
}

// populateControllerList populates the controller combo box with a list of
// the attached controllers
//
void APIUserInput::populateControllerList(void* hwndw) {

    HWND hwnd = (HWND)hwndw; // handle to current window

    // empty the controller combo box
	SendDlgItemMessage(hwnd, IDC_CNT, CB_RESETCONTENT, 0, 0L);
    // start the list with a no controller option
    int ctr = SendDlgItemMessage(hwnd, IDC_CNT, CB_ADDSTRING, 0, 
     (LPARAM)NO_CONTROLLER_DESC);
	// the data item for the no controller line item is NO_CONTROLLER
    SendDlgItemMessage(hwnd, IDC_CNT, CB_SETITEMDATA, ctr, NO_CONTROLLER);
    // assume no controller selected - correct this assumption if necessary
    SendDlgItemMessage(hwnd, IDC_CNT, CB_SETCURSEL, ctr, 0L);
    // uncheck the 'reverse y axis' checkbox
    SendDlgItemMessage(hwnd, IDC_YRV, BM_SETCHECK, BST_UNCHECKED, 0L);
    // uncheck the 'use z axis' checkbox
    SendDlgItemMessage(hwnd, IDC_ZON, BM_SETCHECK, BST_UNCHECKED, 0L);
    // disable the trigger and the check boxes
    EnableWindow(GetDlgItem(hwnd, IDC_TRG), FALSE);
    EnableWindow(GetDlgItem(hwnd, IDC_YRV), FALSE);
    EnableWindow(GetDlgItem(hwnd, IDC_ZON), FALSE);

    // populate the controller combo box
    int nc = nControllers;
    bool atLeastOne = false;
	for (int ic = 0; ic < nc; ic++) {
        if (controllerSet->description(ic)[0]) {
            // store the controller description in the address part
		    ctr = SendDlgItemMessage(hwnd, IDC_CNT, CB_ADDSTRING, 0, 
		     (LPARAM)controllerSet->description(ic));
            // store the controller index in the data part
		    SendDlgItemMessage(hwnd, IDC_CNT, CB_SETITEMDATA, ctr, 
		     (LPARAM)ic);
            atLeastOne = true;
        }
	}
    // if at least one controller is listed, correct the default choices
    if (atLeastOne) {
		// set the visible line item to the previously selected one
		ctr = SendDlgItemMessage(hwnd, IDC_CNT, CB_FINDSTRINGEXACT, -1,
         (LPARAM)controller);
		if (ctr == CB_ERR) 
            ctr = 0;
		SendDlgItemMessage(hwnd, IDC_CNT, CB_SETCURSEL, ctr, 0);
        // retrieve the index of the controller for the default line item
        int ic = SendDlgItemMessage(hwnd, IDC_CNT, CB_GETITEMDATA, ctr, 0);
        // if choice is an attached controller
        if (ic != NO_CONTROLLER) {
            // populate the trigger selection list
            populateControllerObjectList(hwnd);
            // retrieve previous checkbox settings
            bool zAOn = !!(flags & 2);
            bool yRev = !!(flags & 4);
            // set the 'reverse y axis' checkbox to the previous setting
            SendDlgItemMessage(hwnd, IDC_YRV, BM_SETCHECK, yRev ? 
             BST_CHECKED : BST_UNCHECKED, 0L);
            // set the 'use z axis' checkbox to the previous setting
            SendDlgItemMessage(hwnd, IDC_ZON, BM_SETCHECK, zAOn ?
             BST_CHECKED : BST_UNCHECKED, 0L);
            // enable the trigger combo box and the check boxes
            EnableWindow(GetDlgItem(hwnd, IDC_YRV), TRUE);
            EnableWindow(GetDlgItem(hwnd, IDC_ZON), TRUE);
        }
    }
}

// populateAdapterModeList populates the resolution combo box with the modes
// available on the selected display adapter
//
bool APIUserInput::populateAdapterModeList(void* hwndw) {

    bool rc = false;
    HWND hwnd = (HWND)hwndw; // handle to current window

    // empty the resolutions combo box
    SendDlgItemMessage(hwnd, IDC_RES, CB_RESETCONTENT, 0, 0L);
    // retrieve the index for the selected adapter
    int dev = SendDlgItemMessage(hwnd, IDC_DIS, CB_GETCURSEL, 0, 0L);
    if (dev == CB_ERR)
        error(L"APIUserInput::30 No adapter selected");
    else {
        // retrieve the id of the selected adapter
        int id = SendDlgItemMessage(hwnd, IDC_DIS, CB_GETITEMDATA, dev, 0L);
        if (id == RUN_IN_WINDOW) {
			// if window mode, disable the resolutions combo box
            EnableWindow(GetDlgItem(hwnd, IDC_RES), FALSE);
            rc = true;
        }
        else {
            // fullscreen modes
            // populate the resolutions combo box with the available modes
            bool noModes = true; // assume no modes available
            // retrieve the number of supported pixel formats
            int np = displaySet->noPixelFormats();
            int nr = displaySet->noModes();
            for (int ip = 0; ip < np; ip++) { // for each pixel format...
                for (int ir = 0; ir < nr; ir++) { // for each mode...
                    int i = (id * nr + ir) * np + ip;
                    if (displaySet->modeDesc(id, ir, ip)[0]) {
                        // mode is supported - add it to the combo box
                        int res = SendDlgItemMessage(hwnd, IDC_RES,
                         CB_ADDSTRING, 0, 
                         (LPARAM)displaySet->modeDesc(id, ir, ip));
                        // store mode|format in the data part of line item
						unsigned fmMd = (ip << 16) | ir;
                        SendDlgItemMessage(hwnd, IDC_RES, CB_SETITEMDATA, 
						 res, fmMd);
						// at least one mode has been found
                        noModes = false;
                    }
                }
            }
            if (noModes)
                error(L"APIUserInput::31 Selected display has no available modes");
			else {
                // set the cursor to the previously selected resolution
				int res = SendDlgItemMessage(hwnd, IDC_RES, CB_FINDSTRINGEXACT,  
				 -1, (LPARAM)modeStr);
				if (res == CB_ERR) res = 0;
				SendDlgItemMessage(hwnd, IDC_RES, CB_SETCURSEL, res, 0L);

				// enable the resolutions combo box
				EnableWindow(GetDlgItem(hwnd, IDC_RES), TRUE);
				rc = true;
			}
        }
    }

    return rc;
}

// populateControllerObjectList populates the trigger combo box with a list
// of the buttons that are available on the selected controller
//
void APIUserInput::populateControllerObjectList(void* hwndw) {

    int ctr; // line item of selected controller
    HWND hwnd = (HWND)hwndw; // handle to current window

    // empty the controller objects combo box
    SendDlgItemMessage(hwnd, IDC_TRG, CB_RESETCONTENT, 0, 0L);
    // assume no trigger selection possible - correct if necessary
    EnableWindow(GetDlgItem(hwnd, IDC_TRG), FALSE);
    // retrieve the line item of the selected controller
    ctr = SendDlgItemMessage(hwnd, IDC_CNT, CB_GETCURSEL, 0, 0L);
    if (ctr != CB_ERR) {
        // retrieve the index of the selected controller
        int ic = SendDlgItemMessage(hwnd, IDC_CNT, CB_GETITEMDATA, ctr, 0);
        if (ic != NO_CONTROLLER) {
		    // examine each button on the controller
		    bool found = false;
		    int nb = controllerSet->noObjects(ic);
             for (int b = 0; b < nb; b++) {
			    if (controllerSet->description(ic, b)[0]) {
                    // add the button's name to the trigger combo list
                    int trg = SendDlgItemMessage(hwnd, IDC_TRG, CB_ADDSTRING, 
                     0, (LPARAM)controllerSet->description(ic, b));
                    // save the button number in the data area of line item
                    SendDlgItemMessage(hwnd, IDC_TRG, CB_SETITEMDATA, trg, b);
				    // found at least one button
				    found = true;
                }
            }
            // if buttons exist, set the default button to previously selected one
		    if (found) {
                int trg = 0;
			    // determine the default trigger button 
			    if (!wcscmp(controllerSet->description(ic), cntlrStr)) {
				    // set the default to the previously selected trigger
				    trg = SendDlgItemMessage(hwnd, IDC_TRG, CB_FINDSTRINGEXACT, 
				     -1, (LPARAM)trigStr);
				    if (trg == CB_ERR) trg = 0;
			    }
			    SendDlgItemMessage(hwnd, IDC_TRG, CB_SETCURSEL, trg, 0L);
                // enable the trigger selection and check boxes
                EnableWindow(GetDlgItem(hwnd, IDC_TRG), TRUE);
            }
            EnableWindow(GetDlgItem(hwnd, IDC_YRV), TRUE);
            EnableWindow(GetDlgItem(hwnd, IDC_ZON), TRUE);
        }
    }
}

// showActionMapping shows the mapping of the selected action
//
void APIUserInput::showActionMapping(void* hwndw) {

    HWND hwnd = (HWND)hwndw; // handle to current window

    // retrieve the index for the selected action
    int a = SendDlgItemMessage(hwnd, IDC_ACT, CB_GETCURSEL, 0, 0L);
	if (a == CB_ERR)
        error(L"APIUserInput::41 Action selection failed");
	else {
		// retrieve the indices of the objects for the selected action
		unsigned k = SendDlgItemMessage(hwnd, IDC_ACT, CB_GETITEMDATA, a, 0L);
		// store each object's index in the data item of the selected action
		SendDlgItemMessage(hwnd, IDC_KEY, CB_SETCURSEL, KEYOBJ(k), 0L);
		SendDlgItemMessage(hwnd, IDC_PTO, CB_SETCURSEL, PTROBJ(k), 0L);
		SendDlgItemMessage(hwnd, IDC_CTO, CB_SETCURSEL, CTROBJ(k), 0L);
	}
}

// updateActionKeyMapping updates the mapping of the selected key onto the
// selected configurable action
//
void APIUserInput::updateActionKeyMapping(void* hwndw) {

    HWND hwnd = (HWND)hwndw; // handle to current window

    // retrieve the index for the selected action
    unsigned a = SendDlgItemMessage(hwnd, IDC_ACT, CB_GETCURSEL, 0, 0L);
	// retrieve the index for the currently selected key
	unsigned k = SendDlgItemMessage(hwnd, IDC_KEY, CB_GETCURSEL, 0, 0L);
    // retrieve the input objects for the selected action
    unsigned d = SendDlgItemMessage(hwnd, IDC_ACT, CB_GETITEMDATA, a, 0L);
    // erase the current mapping for the key object
    for (unsigned i = 0; i < nActions; i++) {
        unsigned j = SendDlgItemMessage(hwnd, IDC_ACT, CB_GETITEMDATA, i, 0L);
        if (k == KEYOBJ(j))
            SendDlgItemMessage(hwnd, IDC_ACT, CB_SETITEMDATA, i, 
             KEYPTRCTR(0, PTROBJ(d), CTROBJ(d)));
    }

    // store the selected key in the data item of the selected action
	if (a != CB_ERR && k != CB_ERR)
		SendDlgItemMessage(hwnd, IDC_ACT, CB_SETITEMDATA, a, 
         KEYPTRCTR(k, PTROBJ(d), CTROBJ(d)));
}

// updateActionPtrMapping updates the mapping of the selected pointer object
// onto the selected configurable action
//
void APIUserInput::updateActionPtrMapping(void* hwndw) {

    HWND hwnd = (HWND)hwndw; // handle to current window

    // retrieve the index for the selected action
    unsigned a = SendDlgItemMessage(hwnd, IDC_ACT, CB_GETCURSEL, 0, 0L);
	// retrieve the index for the currently selected key
	unsigned k = SendDlgItemMessage(hwnd, IDC_PTO, CB_GETCURSEL, 0, 0L);
    // retrieve the input objects for the selected action
    unsigned d = SendDlgItemMessage(hwnd, IDC_ACT, CB_GETITEMDATA, a, 0L);
    // erase the current mapping for the key object
    for (unsigned i = 0; i < nActions; i++) {
        unsigned j = SendDlgItemMessage(hwnd, IDC_ACT, CB_GETITEMDATA, i, 0L);
        if (k == PTROBJ(j))
            SendDlgItemMessage(hwnd, IDC_ACT, CB_SETITEMDATA, i, 
             KEYPTRCTR(KEYOBJ(d), 0, CTROBJ(d)));
    }

    // store the selected key in the data item of the selected action
	if (a != CB_ERR && k != CB_ERR)
		SendDlgItemMessage(hwnd, IDC_ACT, CB_SETITEMDATA, a, 
         KEYPTRCTR(KEYOBJ(d), k, CTROBJ(d)));
}

// updateActionCtrMapping updates the mapping of the selected controller object
// onto the selected configurable action
//
void APIUserInput::updateActionCtrMapping(void* hwndw) {

    HWND hwnd = (HWND)hwndw; // handle to current window

    // retrieve the index for the selected action
    unsigned a = SendDlgItemMessage(hwnd, IDC_ACT, CB_GETCURSEL, 0, 0L);
	// retrieve the index for the currently selected key
	unsigned k = SendDlgItemMessage(hwnd, IDC_CTO, CB_GETCURSEL, 0, 0L);
    // retrieve the input objects for the selected action
    unsigned d = SendDlgItemMessage(hwnd, IDC_ACT, CB_GETITEMDATA, a, 0L);
    // erase the current mapping for the key object
    for (unsigned i = 0; i < nActions; i++) {
        unsigned j = SendDlgItemMessage(hwnd, IDC_ACT, CB_GETITEMDATA, i, 0L);
        if (k == CTROBJ(j))
            SendDlgItemMessage(hwnd, IDC_ACT, CB_SETITEMDATA, i, 
             KEYPTRCTR(KEYOBJ(d), PTROBJ(d), 0));
    }

    // store the selected key in the data item of the selected action
	if (a != CB_ERR && k != CB_ERR)
		SendDlgItemMessage(hwnd, IDC_ACT, CB_SETITEMDATA, a, 
         KEYPTRCTR(KEYOBJ(d), PTROBJ(d), k));
}

// showSoundMapping shows the mapping of the selected sound
//
void APIUserInput::showSoundMapping(void* hwndw) {

    HWND hwnd = (HWND)hwndw; // handle to current window

    // retrieve the index for the selected sound
    int s = SendDlgItemMessage(hwnd, IDC_AUD, CB_GETCURSEL, 0, 0L);
	if (s == CB_ERR)
        error(L"APIUserInput::42 Sound selection failed");
	else {
		// retrieve the index of the file linked to the selected sound
		int f = SendDlgItemMessage(hwnd, IDC_AUD, CB_GETITEMDATA, s, 0L);
		// show the file currently linked to the selected sound
		SendDlgItemMessage(hwnd, IDC_SFL, CB_SETCURSEL, f, 0L);
	}
}

// updateSoundMapping updates the mapping of the selected sound file
// onto the selected configurable sound
//
void APIUserInput::updateSoundMapping(void* hwndw) {

    HWND hwnd = (HWND)hwndw; // handle to current window

    // retrieve the index for the selected sound
    int s = SendDlgItemMessage(hwnd, IDC_AUD, CB_GETCURSEL, 0, 0L);
	// retrieve the index of the selected sound file
	int f = SendDlgItemMessage(hwnd, IDC_SFL, CB_GETCURSEL, 0, 0L);

	if (s != CB_ERR && f != CB_ERR)
		// store the index of the selected sound file in the data item 
		// of the selected sound
		SendDlgItemMessage(hwnd, IDC_AUD, CB_SETITEMDATA, s, f);
}

// saveUserChoices configures the context for the user's selections 
//
bool APIUserInput::saveUserChoices(void* hwndw) {

    bool rcd  = false, rcc = false;
    HWND hwnd = (HWND)hwndw; // handle to current window

    //----- adapter device properties -----------------------------------------

    // Retrieve the index for the selected display device
    int dev = SendDlgItemMessage(hwnd, IDC_DIS, CB_GETCURSEL, 0, 0L);
    if (dev == CB_ERR)
        error(L"APIUserInput::50 No display adapter selected");
    else {
        int  res = 0; // selected resolution line item
        // retrieve adapter id for the selected display device line item
        displayId = SendDlgItemMessage(hwnd, IDC_DIS, CB_GETITEMDATA, dev, 0L);
		runinwndw = displayId == RUN_IN_WINDOW;
        if (!runinwndw) {
            // retrieve the index for the selected resolution
            res = SendDlgItemMessage(hwnd, IDC_RES, CB_GETCURSEL, 0, 0L);
            if (res == CB_ERR) {
                error(L"APIUserInput::51 Resolution selection failed");
                // revert to run in a window
                runinwndw = true;
                displayId = 0;
                modeId    = 0;
                pixelId   = 0;
            }
            else {
				// get the data item of the selected resolution line item
                unsigned fmMd = SendDlgItemMessage(hwnd, IDC_RES,
                 CB_GETITEMDATA, res, 0L);
                // extract the mode and format from the data item
				modeId = fmMd & 0xFFFF;
				pixelId = (fmMd >> 16);
            }
        }
        else {
            displayId = 0;
            modeId    = 0;
            pixelId   = 0;
        }
		// store display and resolution selection strings for future reference
		SendDlgItemMessage(hwnd, IDC_DIS, CB_GETLBTEXT, dev, (LPARAM)dispStr);
		if (!runinwndw)
            SendDlgItemMessage(hwnd, IDC_RES, CB_GETLBTEXT, res, 
             (LPARAM)modeStr);

		rcd = true;
    }

	//----- key mappings for model actions ------------------------------------

	// define the action mappings for the configurable objects
	for (unsigned a = 0; a < nActions; a++) {
		// extract the key mapping from the data parameter of the line item
		unsigned k = SendDlgItemMessage(hwnd, IDC_ACT, CB_GETITEMDATA, a, 0L);
        action[a][KEYBOARD]   = KEYOBJ(k);
        action[a][POINTER]    = PTROBJ(k);
        action[a][CONTROLLER] = CTROBJ(k);
	}

	// store the index of the currently visible action for future initialization
    action_ = SendDlgItemMessage(hwnd, IDC_ACT, CB_GETCURSEL, 0, 0L);

	//----- sound mappings ----------------------------------------------------

	// define the files associated with the configurable sounds
	wchar_t f[MAX_DESC + 1];
	for (unsigned s = 0; s < nSounds; s++) {
		int i = SendDlgItemMessage(hwnd, IDC_AUD, CB_GETITEMDATA, s, 0L);
		// extract the filename from the string parameter of the line item
		SendDlgItemMessage(hwnd, IDC_SFL, CB_GETLBTEXT, i, (LPARAM)f);
		strcpy(sndFile[s], f, MAX_DESC);
	}

	// store the index of the currently visible sound for future initialization
    sound = SendDlgItemMessage(hwnd, IDC_AUD, CB_GETCURSEL, 0, 0L);

    //----- controller properties ---------------------------------------------

    // retrieve the user's selections
    int ctr = SendDlgItemMessage(hwnd, IDC_CNT, CB_GETCURSEL, 0, 0L);
    if (ctr == CB_ERR)
        error(L"APIUserInput::54 Controller selection failed");
    else {
		int  trg, b = 0;
		bool y, z, none;
        // retrieve the index of the selected controller
        int ic = SendDlgItemMessage(hwnd, IDC_CNT, CB_GETITEMDATA, ctr, 0L);
        none = ic == NO_CONTROLLER;
		if (!none) {
			// retrieve the selected trigger button
			trg = SendDlgItemMessage(hwnd, IDC_TRG, CB_GETCURSEL, 0, 0L);
			if (trg == CB_ERR) {
				error(L"APIUserInput::55 Trigger selection failed");
                trg = 0;
                b = 0;
            }
			else
				b = SendDlgItemMessage(hwnd, IDC_TRG, CB_GETITEMDATA, trg, 0);
            // retrieve the selected y axis direction
            y = SendDlgItemMessage(hwnd, IDC_YRV, BM_GETCHECK, 0, 0)
             == BST_CHECKED;
            // retrieve the selected z axis usage
            z = SendDlgItemMessage(hwnd, IDC_ZON, BM_GETCHECK, 0, 0)
             == BST_CHECKED;
		}
        else {
            b  = trg = 0;
            y  = z   = false;
            ic = 0;
        }

		// (re-)define the controller aspect of the context
        flags = ((y ? 1 : 0) << 2) | ((z ? 1 : 0) << 1) | (none ? 1 : 0);
        cntlrId = ic;
        triggId = b;
        cnFlags = flags;
        // store descriptions for future initialization
		SendDlgItemMessage(hwnd, IDC_CNT, CB_GETLBTEXT, ctr, (LPARAM)cntlrStr);
		SendDlgItemMessage(hwnd, IDC_TRG, CB_GETLBTEXT, trg, (LPARAM)trigStr);

		rcc = true;
    }

    return rcd && rcc;
}

// error apiDisplays a message box with message msg and write the message
// to the log file
//
void APIUserInput::error(const wchar_t* msg) const {

    if (hwnd) 
        MessageBox((HWND)hwnd, msg, L"Dialog Error", MB_OK);
    logError(msg);
}

// dlgProc handles each message "msg" to the Dialog Box window "hwnd"
//
BOOL CALLBACK dlgProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {

    BOOL            rc        = FALSE;
    static bool     firsttime = true;
    iAPIUserInput* apiDialog = APIUserInputAddress();

    // Process message msg
    switch (msg) {
      case WM_INITDIALOG:
        // make this dialog window a layered window 
        SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) 
         | WS_EX_LAYERED);
        // make this dialog window 5% transparent
        SetLayeredWindowAttributes(hwnd, 0, (255 * 95) / 100, LWA_ALPHA);
        // populate dialog
        apiDialog->populateAPIUserDialog(hwnd);
		rc = true;
        break;

      case WM_COMMAND:          // user accessed a dialog box control
        switch (LOWORD(wp)) {   // which control?
          case IDC_DIS:     // accessed the apiDisplay combo box
            // only process this if it is the first time or the user
            // has changed their selection.  This block resets the static
            // variable "firsttime".  Exiting resets the variable to true
            if (firsttime && HIWORD(wp) == CBN_SETFOCUS ||
             HIWORD(wp) == CBN_SELCHANGE ) {
                firsttime = false;  // won't be 1st time again for this box!
                // populate resolutions combo box for the selected adapter
                if (rc = apiDialog->populateAdapterModeList(hwnd))
                    // safe to activate GO button now
                    EnableWindow(GetDlgItem(hwnd, IDC_GO), TRUE);
            } else
                rc = TRUE;
            break;

          case IDC_ACT:  // user accessed the action combo box
            // show the current mappable key for the selected action
            apiDialog->showActionMapping(hwnd);
            break;

          case IDC_KEY:  // user accessed the mappable key combo box
            if (HIWORD(wp) == CBN_SELCHANGE)
                // associate the selected action with the selected key
                apiDialog->updateActionKeyMapping(hwnd);
            break;

          case IDC_PTO:  // user accessed the mappable pointer object combo box
            if (HIWORD(wp) == CBN_SELCHANGE)
                // associate the selected action with the selected object
                apiDialog->updateActionPtrMapping(hwnd);
            break;

          case IDC_CTO:  // user accessed the mappable controller object combo box
            if (HIWORD(wp) == CBN_SELCHANGE)
                // associate the selected action with the selected object
                apiDialog->updateActionCtrMapping(hwnd);
            break;

          case IDC_AUD:  // user accessed the audio combo box
            // show the current mappable key for the selected action
            apiDialog->showSoundMapping(hwnd);
            break;

          case IDC_SFL:  // user accessed the mappable file combo box
            if (HIWORD(wp) == CBN_SELCHANGE)
                // associate the selected action with the selected key
                apiDialog->updateSoundMapping(hwnd);
            break;

          case IDC_CNT:  // user accessed the controller combo box
            if (HIWORD(wp) == CBN_SELCHANGE)
                // populate the objects combo box for the selected controller
                apiDialog->populateControllerObjectList(hwnd);
            break;

          case IDC_GO:  // user pressed the Go button
            // save the context information
            if (apiDialog->saveUserChoices(hwnd)) {
                EndDialog(hwnd, TRUE);
                firsttime = true;
                rc = TRUE;
            }
            break;

          case IDCANCEL:  // user pressed Cancel button, or Esc, etc.
            EndDialog(hwnd, FALSE);
            firsttime = true;
            rc = TRUE;
            break;
        }
        break;
    }

    return rc;
}

