#ifndef _APITEXT_H_
#define _APITEXT_H_

/* APIText Definition - Translation Layer
 *
 * APIText.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "APIBase.h"  // for the APIBase class definition
#include "iAPIText.h" // for the APIText Interface

//-------------------------------- APIText ------------------------------------
//
// The APIText class implements a Text item at the API level
//
struct ID3DXFont;

class APIText : public iAPIText, public APIBase {

    ID3DXFont*     d3dfont;   // points to the D3D font Interface

	unsigned       align;     // alignment flags
    unsigned       colour;    // font colour 
    unsigned       weight;    // normal, medium, bold
    unsigned       precision; // true type, raster
    unsigned       quality;   // antialias, cleartype
    const wchar_t* typeFace;  // typeFace name of font
    int            fontHght;  // height of font

    virtual ~APIText();
    void    setup();

  public:
    APIText(const wchar_t*, int, unsigned, unsigned);
    APIText(const APIText& v);
    APIText& operator=(const APIText& v);
    iAPIText* clone() const                 { return new APIText(*this); }
	// execution function
    void   draw(const Rectf& r, const wchar_t* text);
	// termination
    void   suspend();
    bool   restore();
	void   release();
	void   Delete() const                   { delete this; }
};

#endif