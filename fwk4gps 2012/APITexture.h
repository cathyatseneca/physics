#ifndef _API_TEXTURE_H_
#define _API_TEXTURE_H_

/* APITexture Definition - Translation Layer
 *
 * APITexture.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iAPITexture.h" // for the APITexture Interface
#include "APIBase.h"     // for the APIBase class definition

//-------------------------------- APITexture ---------------------------------
//
// The APITexture class implements a Texture at the API level
//
struct IDirect3DTexture9;

class APITexture : public iAPITexture, public APIBase {

    wchar_t*           file;   // points to file with texture image
    unsigned           filter; // default texture filtering flags

    IDirect3DTexture9* tex;    // interface to texture COM object

	virtual ~APITexture();

	void setSamplerState(int i, unsigned flags) const;
	void setup(int w, int h, int c);

  public:
    APITexture(const wchar_t* file, unsigned filter = 0);
	APITexture(const APITexture&);
	iAPITexture& operator=(const APITexture&);
    iAPITexture* clone() const { return new APITexture(*this); }
	// execution
	void   attach();
    void   setFilter(unsigned filter);
	void   detach();
	void   render(const Rectf&, unsigned char, bool);
    // suspension
    void   suspend();
	// termination
    void   release();
	void   Delete() const { delete this; }
};

#endif