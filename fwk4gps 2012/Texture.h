#ifndef _TEXTURE_H_
#define _TEXTURE_H_

/* Texture Definition - Modelling Layer
 *
 * Texture.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iTexture.h" // for the Texture Interface

//-------------------------------- Texture ------------------------------------
//
// The Texture class defines the structure of a texture in the Modelling Layer
//
class iAPITexture;

class Texture : public iTexture {

	iAPITexture* apiTexture;   // points to the api texture

	Texture(const Texture&);
	virtual ~Texture();

  public:
	Texture(const wchar_t* file, unsigned filter = 0);
	Texture& operator=(const Texture&);
    void* clone() const { return new Texture(*this); }
	// execution
	void attach() const;
    void setFilter(unsigned) const;
	void detach();
	void render(const Rectf&, bool);
	// termination
	void suspend();
    void release();
};

#endif
