#ifndef _API_WINDOW_H_
#define _API_WINDOW_H_

/* APIWindow Definition - Translation Layer
 *
 * APIWindow.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iAPIWindow.h" // for the APIWindow Interface
#include "APIBase.h"    // for the APIBase class definition

//-------------------------------- APIWindow -------------------------------------
//
// The APIWindow class manages the main application window
//
class APIWindow : public iAPIWindow, public APIBase {

	int  show;                // how to display the application window
    int  clientWidth;         // width of client area 
    int  clientHeight;        // height of client area
    int  oldClientWidth;      // width of old client area in a window
    int  oldClientHeight;     // height of old client area in a window

	APIWindow(const APIWindow&);
	APIWindow& operator=(const APIWindow&);
    virtual ~APIWindow();

  public:
    APIWindow(void*, int);
	// initialization function
    bool setup();
	// execution
	bool processMessages(int&, bool&) const;
    void configure();
	void resize();
    int  getClientWidth() const            { return clientWidth; }
    int  getClientHeight() const           { return clientHeight; }
    bool getWindowMode() const             { return runinwndw; }
    float aspectRatio() const    { return (float) clientWidth / clientHeight; }
	void moveToForeground() const;
    void messageBox(const wchar_t*) const;
    void error(const wchar_t* msg) const   { APIBase::error(msg); }
    int  time() const;
    void wait();
	// termination
    void release();
	void Delete() const                    { delete this; }
};

#endif