#ifndef _I_API_GRAPHIC_H_
#define _I_API_GRAPHIC_H_

/* APIGraphic Interface - Translation Layer
 *
 * iAPIGraphic.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- iAPIGraphic --------------------------------
//
// iAPIGraphic is the Interface to the APIGraphic hierarchy
//
enum PrimitiveType;
class iGraphic;

class iAPIGraphic {
  public:
    virtual iAPIGraphic* clone() const                              = 0;
    virtual void draw(unsigned)                                     = 0;
    virtual void suspend()                                          = 0;
    virtual void release()                                          = 0;
	virtual void Delete() const                                     = 0;
};

iAPIGraphic* CreateAPIVertexList(PrimitiveType, unsigned, unsigned, unsigned,
 iGraphic*);

#endif
