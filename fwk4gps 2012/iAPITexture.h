#ifndef _I_API_TEXTURE_H_
#define _I_API_TEXTURE_H_

/* APITexture Interface - Translation Layer
 *
 * iAPITexture.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- iAPITexture --------------------------------
//
// iAPITexture is the Interface to the APITexture class
//
struct Rectf;

class iAPITexture {
  public:
    virtual iAPITexture* clone() const                                = 0;
	// execution
	virtual void attach()                                             = 0;
    virtual void setFilter(unsigned flags)                            = 0;
	virtual void detach()                                             = 0;
	virtual void render(const Rectf&, unsigned char, bool = false)    = 0;
	// termination
	virtual void suspend()                                            = 0;
	virtual void release()                                            = 0;
	virtual void Delete() const                                       = 0;
};

iAPITexture* CreateAPITexture(const wchar_t* file, unsigned filter);

#endif