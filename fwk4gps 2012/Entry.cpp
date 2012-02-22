/* An Open Instructional Framework for Game Programming Students
 *
 * Entry.cpp
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h> // for WinMain and Windows Types

#include "Design.h"  // for the Design class definition

// Entry point for the Application
//
int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hprev, LPSTR cp, int show) {

    Design game(hinst, show);

    return game.run();
}
