#ifndef _I_API_USER_DIALOG_H_
#define _I_API_USER_DIALOG_H_

/* APIUserInput Interface - Translation Layer
 *
 * iAPIUserInput.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- iAPIUserInput --------------------------------
//
// iAPIUserInput is the Interface to the APIUserInput class
//
enum  ModelSound;
enum  Action;

class iAPIUserInput {
  public:
	// initialization
    virtual bool getConfiguration()                                        = 0;
    virtual void configure()                                               = 0;
    virtual bool setup()                                                   = 0;
    virtual const wchar_t* soundFile(ModelSound s) const                   = 0;
	// execution
    virtual void update()                                                  = 0;
    virtual bool pressed(Action) const                                     = 0;
    virtual bool ptrPressed() const                                        = 0;
    virtual bool ctrPressed() const                                        = 0;
    virtual int  change(Action) const                                      = 0;
	// termination
    virtual void suspend()                                                 = 0;
    virtual bool restore()                                                 = 0;
    virtual void release()                                                 = 0;
    virtual void Delete() const                                            = 0;
    // DialogBox
    virtual void populateAPIUserDialog(void*)                              = 0;
    virtual bool populateAdapterModeList(void*)                            = 0;
    virtual void populateControllerObjectList(void*)                       = 0;
	virtual void showActionMapping(void*)                                  = 0;
	virtual void updateActionKeyMapping(void*)                             = 0;
	virtual void updateActionPtrMapping(void*)                             = 0;
	virtual void updateActionCtrMapping(void*)                             = 0;
	virtual void showSoundMapping(void*)                                   = 0;
	virtual void updateSoundMapping(void*)                                 = 0;
    virtual bool saveUserChoices(void*)                                    = 0;
};

iAPIUserInput* CreateAPIUserInput(const wchar_t*);
iAPIUserInput* APIUserInputAddress();

#endif