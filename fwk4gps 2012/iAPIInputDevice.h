#ifndef _I_API_INPUT_H_
#define _I_API_INPUT_H_

/* APIInputDeviceSet and APIInputDevice Interfaces - Translation Layer
 *
 * APIInputDevice.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//------------------------------- iAPIInputDeviceSet --------------------------
//
// iAPIInputDeviceSet is the Interface to the APIInputDeviceSet class
//
enum  UserDeviceType;
class APIInputDeviceDesc;

class iAPIInputDeviceSet {
  public:
    virtual bool interrogate()                                       = 0;
    virtual unsigned noAttached() const                              = 0;
    virtual APIInputDeviceDesc* desc(unsigned) const                 = 0;
    virtual unsigned noObjects(unsigned i) const                     = 0;
    virtual const wchar_t* description(unsigned) const               = 0;
    virtual const wchar_t* description(unsigned i, unsigned j) const = 0;
    virtual bool selected(unsigned) const                            = 0;
    virtual void select(unsigned, unsigned = 0u, unsigned = 0u)      = 0;
    virtual void Delete() const                                      = 0;
};

iAPIInputDeviceSet* CreateAPIInputSet(UserDeviceType);

//------------------------------- iAPIInputDevice -----------------------------
//
// iAPIInputDevice is the Interface to the APIInputDevice class
//
class iAPIInputDevice {
  public:
	// initialization
    virtual bool setup()                             = 0;
	// execution
    virtual void update()                            = 0;
    virtual bool pressed(unsigned a) const           = 0;
    virtual bool pressed() const                     = 0;
    virtual int  change(unsigned a) const            = 0;
    // suspend execution
    virtual void suspend()                           = 0;
    virtual bool restore()                           = 0;
	// termination
    virtual void release()                           = 0;
    virtual void Delete() const                      = 0;
};

iAPIInputDevice* CreateAPIKeyboard(APIInputDeviceDesc*);
iAPIInputDevice* CreateAPIPointer(APIInputDeviceDesc*);
iAPIInputDevice* CreateAPIController(APIInputDeviceDesc*);

#endif
