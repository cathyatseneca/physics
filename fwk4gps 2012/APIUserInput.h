#ifndef _API_USER_INPUT_H_
#define _API_USER_INPUT_H_

/* APIUserInput Definition - Translation Layer
 *
 * APIUserInput.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iAPIUserInput.h"    // for the APIUserInput Interface
#include "APIBase.h"          // for the APIBase class definition
#include "GeneralConstants.h" // for MAX_DESC

//-------------------------------- APIUserInput ---------------------------------
//
// The APIUserInput class collects the configuration information from the user
//
class iAPIInputDevice;

class APIUserInput : public iAPIUserInput, public APIBase {

    static iAPIUserInput* address; // points to this singleton

    void*           hwnd;           // points to the dialog window
    const wchar_t*  audioDirectory; // points to the Audio File Directory

    // action translation tables
    unsigned        nActions;  
    wchar_t         (*actionDesc)[MAX_DESC + 1];
    unsigned        (*action)[3];
    unsigned        nKbdObjs;
    wchar_t         (*kbdObjDesc)[MAX_DESC + 1];
    unsigned        nPtrObjs;
    wchar_t         (*ptrObjDesc)[MAX_DESC + 1];
    unsigned        nCtrObjs;
    wchar_t         (*ctrObjDesc)[MAX_DESC + 1];

    // audio
    unsigned        nSounds;
    wchar_t         (*sndDesc)[MAX_DESC + 1];
    wchar_t         (*sndFile)[MAX_DESC + 1];

    unsigned        nKeyboards;
    unsigned        nPointers;
    unsigned        nControllers;

    iAPIInputDevice**  keyboard;       // points to the keyboard objects
    iAPIInputDevice**  pointer;        // points to the pointer objects 
    iAPIInputDevice**  controller;     // points to the controller objects

    unsigned        displayId;      // display adapter identifier
    unsigned        modeId;         // resolution mode identifier
    unsigned        pixelId;        // pixel format identifier

    unsigned        cntlrId;        // controller identifier
    unsigned        triggId;        // trigger identifier
    unsigned        cnFlags;        // controller flags

    // most recent configuration memory
    wchar_t         dispStr [MAX_DESC + 1]; 
    wchar_t         modeStr [MAX_DESC + 1];
    wchar_t         cntlrStr[MAX_DESC + 1];
    wchar_t         trigStr [MAX_DESC + 1];
    int             action_;
    int             sound;
    int             flags;

	APIUserInput(const APIUserInput&);            // prevent copies
	APIUserInput& operator=(const APIUserInput&); // prevent assignments
    virtual ~APIUserInput();

  public:
    static iAPIUserInput* Address()              { return address; }
    APIUserInput(const wchar_t*);
    bool  getConfiguration();
    void  configure();
    bool  setup();
    const wchar_t* soundFile(ModelSound s) const { return sndFile[s]; }
	// execution
    void  update();
    bool  pressed(Action) const;
    bool  ptrPressed() const;
    bool  ctrPressed() const;
    int   change(Action a) const;
	// termination functions
    void  suspend();
    bool  restore();
    void  release();
	void  Delete() const                         { delete this; }
    // DialogBox methods
    void  populateAPIUserDialog(void*);
    void  populateAdapterList(void*);
    void  populateActionList(void*);
	void  populateSoundFileList(void*);
    void  populateControllerList(void*);
	void  populateMappableObjectLists(void*);
    bool  populateAdapterModeList(void*);
    void  populateControllerObjectList(void*);
	void  showActionMapping(void*);
	void  updateActionKeyMapping(void*);
	void  updateActionPtrMapping(void*);
	void  updateActionCtrMapping(void*);
	void  showSoundMapping(void*);
	void  updateSoundMapping(void*);
    bool  saveUserChoices(void*);
    void  error(const wchar_t*) const;
};

#endif