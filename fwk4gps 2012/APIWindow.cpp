/* APIWindow Implementation - Translation Layer
 *
 * APIWindow.cpp
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>         // for timeGetTime
#include "APIWindow.h"        // for the APIWindow class definition
#include "iCoordinator.h"     // for the Coordinator Interface
#include "iAPIDisplay.h"      // for the APIDisplay Interface
#include "GeneralConstants.h" // for WND_NAME

bool registerAPIWindowClass(HINSTANCE hinst);
LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

#define CLASS_NAME    L"fwk4gps"
// Window styles
#define WND_STYLE_W   WS_OVERLAPPEDWINDOW
#define WND_EXSTYLE_W 0
#define WND_STYLE     WS_POPUP
#define WND_EXSTYLE   WS_EX_TOPMOST
// units of time per sec returned by operating system
//#define UNITS_PER_SEC 1000

//------------------------------ APIWindow ---------------------------------------
//
// The APIWindow class manages the main application window
//
// CreateMainAPIWindow creates the APIWindow Object
//
iAPIWindow* CreateAPIWindow(void* h, int s) {

	return new APIWindow(h, s);
}

// constructor initializes the instance variables and registers the window
// class for the application
//
APIWindow::APIWindow(void* h, int s) : show(s) {

    window          = this;
    application     = h;
    clientHeight    = WND_WIDTH;
    clientWidth     = WND_HEIGHT;
    oldClientWidth  = WND_WIDTH;
    oldClientHeight = WND_HEIGHT;

	registerAPIWindowClass((HINSTANCE)application);
}

// configure sets the width and height of the client area
//
void APIWindow::configure() {

    if (runinwndw) {
        clientWidth  = oldClientWidth;
        clientHeight = oldClientHeight;
    }
    else {
        clientWidth  = width; 
        clientHeight = height; 
    }
}

// create (re)creates the main application window
//
bool APIWindow::setup() {

    int wndStyle;   // window style
    int wndExStyle; // window style extension
    int wndWidth;   // entire window width
    int wndHeight;  // entire window height

    // window creation parameters
    if (runinwndw) {
		// set the window styles
		wndStyle    = WND_STYLE_W;
		wndExStyle  = WND_EXSTYLE_W;
        // determine windw size for the specified client area
        RECT rect;
        rect.left   = 0;
        rect.top    = 0;
        rect.right  = clientWidth;  // desired width of the client area
        rect.bottom = clientHeight; // desired height of the client area
        AdjustWindowRectEx(&rect, wndStyle, FALSE, wndExStyle);
        wndWidth    = rect.right  - rect.left;
        wndHeight   = rect.bottom - rect.top;
    } else {
        // set the window styles
		wndStyle   = WND_STYLE;
		wndExStyle = WND_EXSTYLE;
        // client area same as window size
        wndWidth   = clientWidth;
        wndHeight  = clientHeight;
    }
	// (re)create the main application window
	if (!(hwnd = CreateWindowEx(wndExStyle, CLASS_NAME, WND_NAME, wndStyle, 
	 0, 0, wndWidth, wndHeight, nullptr, nullptr, (HINSTANCE)application, nullptr)))
		error(L"APIWindow::10 Failed to create application window");
	else {
		// how to APIDisplay the window
		ShowWindow((HWND)hwnd, show);
		// send the first WM_PAINT message to the window
		UpdateWindow((HWND)hwnd);
	}

    return hwnd != nullptr;
}

// resize resets the window size 
//
void APIWindow::resize() {

	// determine the new size of the entire window
	RECT rect;
	GetClientRect((HWND)hwnd, &rect);
    clientWidth     = rect.right  - rect.left;
    clientHeight    = rect.bottom - rect.top;
    oldClientWidth  = clientWidth;
    oldClientHeight = clientHeight;
    // APIBase class variables
    width           = clientWidth;
    height          = clientHeight;
}

// processMessages checks extracts a message from the message queue and
// processes that message if one exists
// if the message is to quit, sets rc and keepgoing and returns them
//
bool APIWindow::processMessages(int& rc, bool& keepgoing) const {

	bool retrievedMessage = false;

	MSG msg;
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            rc = msg.wParam;
            keepgoing = false;
        }
        else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		retrievedMessage = true;
    }

	return retrievedMessage;
}

// wait places the application in a wait state until there is a message for
// the application window
//
void APIWindow::wait() {

    WaitMessage();
}

// moveToForeground moves hwnd to the foreground
//
void APIWindow::moveToForeground() const {

	if (hwnd)
		SetForegroundWindow((HWND)hwnd);
}

// messageBox pops up a MessageBox in window hwnd with the message str
//
void APIWindow::messageBox(const wchar_t* str) const {

    if (hwnd)
        MessageBox((HWND)hwnd, str, L"Error", MB_OK);
}

// time returns the current system time
//
int APIWindow::time() const { 
    return timeGetTime();
}

// release destroys the main application window
//
void APIWindow::release() {

	// destroy the current application window, if one exists
	if (hwnd) {
		DestroyWindow((HWND)hwnd);
		show = SW_SHOW;
        hwnd = nullptr;
	}
}

// destructor destroys the main application window
//
APIWindow::~APIWindow() {

	release();
    window = nullptr;
}

// registerAPIWindowClass registers the class for the main application window
//
bool registerAPIWindowClass(HINSTANCE application) {

    // Define the "class" (type) of window we want to have
    // - a necessary step before we can create the main window

    WNDCLASS wc;

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = ::wndProc; // this function will be called...
    wc.cbClsExtra = 0;           // ...to handle messages
    wc.cbWndExtra = 0;
    wc.hInstance = application;
    wc.hIcon = LoadIcon(application, IDI_APPLICATION);
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = CLASS_NAME;

    // Register the "class" for all subsequent calls to create
    // the main application window
    //
    return RegisterClass(&wc) != 0;
}

// wndProc handles the message "msg" sent to the main application window "hwnd"
//
LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {

    static bool   quit        = false;
    iCoordinator* coordinator = CoordinatorAddress();

    switch (msg) {
      case WM_CREATE:    // called once when the window is first created
      case WM_SETCURSOR: // called whenever the mouse is moved to ...
        SetCursor(nullptr); // ...lets us change the cursor if we want to
        break;

      // sent when the user switches applications
      case WM_ACTIVATEAPP:
        // window is about to be activated
		if (wp)
			coordinator->restore();
         // window is about to be de-activated
        else
	        coordinator->suspend();
        break;

      case WM_KEYDOWN:
        switch (wp) {
          case VK_ESCAPE: 
            // close the main window and ...
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            // prepare to quit
            quit = true;
            break;

          case VK_F1:
            // select the new context 
			// - deactivate and post a user message
            PostMessage(hwnd, WM_ACTIVATEAPP, 0, 0);
            PostMessage(hwnd, WM_USER, 0, 0);
            break;
		}
		break;

      case WM_USER:
        coordinator->reset();
        break;

      case WM_SIZE:
        coordinator->resize();
        break;

	  case WM_CLOSE:
	    quit = true;
      case WM_DESTROY:
        // we need to respond to WM_DESTROY for the main window to shutdown properly
        if (quit)
            PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, msg, wp, lp);
}
