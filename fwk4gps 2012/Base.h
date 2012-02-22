#ifndef _BASE_H_
#define _BASE_H_

/* Base Definition - Modelling Layer
 *
 * Base.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iBase.h" // for the Base Interface

//-------------------------------- Base ---------------------------------------
//
// The Base class holds the absolute state for the Modelling Layer
//
class iCoordinator;

class Base : public iBase {

protected:

    static iCoordinator* coordinator; // points to the Coordinator object

    static unsigned      unitsPerSec; // units of system time in one second
    static unsigned      now;         // current time in system units
    static unsigned      lastUpdate;  // time of the last update
    static int           frequency;   // current ambient frequency
    static int           volume;      // current volume
    static bool          active;      // application is active?

    virtual ~Base()      { }

public:
    void* clone() const  { return nullptr; }
    void render()        { }
    void suspend()       { }
    void restore()       { }
    void release()       { }
    void Delete() const  { delete this; }
};

#endif
