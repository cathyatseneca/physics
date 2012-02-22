#ifndef _I_API_WINDOW_H_
#define _I_API_WINDOW_H_

/* APIWindow Interface - Translation Layer
 *
 * iAPIWindow.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- iAPIWindow -------------------------------
//
// iAPIWindow is the Interface to the APIWindow class
//
class iAPIWindow {
  public:
	// initialization
    virtual bool  setup()                                    = 0;
	// execution
	virtual bool  processMessages(int& rc, bool& done) const = 0;
	virtual void  resize()                                   = 0;
    virtual int   getClientWidth() const                     = 0;
    virtual int   getClientHeight() const                    = 0;
    virtual bool  getWindowMode() const                      = 0;
    virtual float aspectRatio() const                        = 0;
    virtual void  configure()                                = 0;
	virtual void  moveToForeground() const                   = 0;
    virtual void  messageBox(const wchar_t*) const           = 0;
    virtual void  error(const wchar_t*) const                = 0;
	virtual int   time() const                               = 0;
    virtual void  wait()                                     = 0;
    // termination
    virtual void  release()                                  = 0;
    virtual void  Delete() const                             = 0;
};

iAPIWindow* CreateAPIWindow(void*, int);

#endif