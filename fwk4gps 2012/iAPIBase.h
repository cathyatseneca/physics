#ifndef _I_API_BASE_H_
#define _I_API_BASE_H_

/* API Base Interface - Translation Layer
 *
 * iAPIBase.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- iAPIBase -----------------------------------
//
// iAPIBase is the Interface to the API Base class
//
class iAPIBase {
  public:
	// execution
    virtual void render()                                        = 0;
	// termination
    virtual void suspend()                                       = 0;
    virtual bool restore()                                       = 0;
    virtual void release()                                       = 0;
    virtual void Delete() const                                  = 0;
    virtual void error(const wchar_t*, const wchar_t* = 0) const = 0;
    virtual void logError(const wchar_t*) const                  = 0;
};

#endif
