/* APIText Implementation - Translation Layer
 *
 * APIText.cpp
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "APIPlatformSettings.h" // for API headers
#include "APIText.h"             // for the APIText class definition
#include "Common_Symbols.h"      // symbols common to Modelling/Translation

//-------------------------------- APIText ------------------------------------
//
// The APIText class implements a Text item at the API level 
//
// CreateAPIText creates an APIText object
//
iAPIText* CreateAPIText(const wchar_t* t, int h, unsigned f, unsigned c) {

	return new APIText(t, h, f, c);
}

// constructor initializes the instance variables
//
APIText::APIText(const wchar_t* t, int h, unsigned f, unsigned c) : 
 typeFace(t), fontHght(h) {

    d3dfont = nullptr;

    // define the text alignment flags
	if (!f) f = TEXT_DEFAULT;
    align = 0;
    if (f & TEXT_LEFT)           align |= DT_LEFT;
    else if (f & TEXT_RIGHT)     align |= DT_RIGHT;
    else if (f & TEXT_CENTER)    align |= DT_CENTER;
    if (f & TEXT_TOP)            align |= DT_TOP;
    else if (f & TEXT_BOTTOM)    align |= DT_BOTTOM;
    else if (f & TEXT_MIDDLE)    align |= DT_VCENTER;
    // define the font weight
    if (f & TEXT_NORMAL)         weight = FW_NORMAL;
    else if (f & TEXT_MEDIUM)    weight = FW_MEDIUM;
    else if (f & TEXT_SEMIBOLD)  weight = FW_SEMIBOLD;
    else if (f & TEXT_BOLD)      weight = FW_BOLD;
    else                         weight = FW_DONTCARE;
    // define the output precision
    if (f & TEXT_TRUETYPE)       precision = OUT_TT_PRECIS;
    else if (f & TEXT_RASTER)    precision = OUT_RASTER_PRECIS;
    else                         precision = OUT_DEFAULT_PRECIS;
    // define the output quality
    if (f & TEXT_ANTIALIAS)      quality = ANTIALIASED_QUALITY;
    else if (f & TEXT_PROOF)     quality = PROOF_QUALITY;
    else if (f & TEXT_CLEARTYPE) quality = CLEARTYPE_QUALITY;
    else                         quality = DEFAULT_QUALITY;
    // use defaults if height, typeface and colour undefined (0)
    if (!fontHght) fontHght = TEXT_HEIGHT;
    if (!typeFace) typeFace = TEXT_TYPEFACE;
    if (!c)             c   = TEXT_COLOUR;
    unsigned char alpha = (c & 0xFF000000) >> 24;
    unsigned char red   = (c & 0xFF0000) >> 16;
    unsigned char green = (c & 0xFF00) >> 8;
    unsigned char blue  = (c & 0xFF);
    colour = D3DCOLOR_ARGB(alpha, red, green, blue);
}

// copy constructor
//
APIText::APIText(const APIText& src) {

    d3dfont = nullptr;
    *this = src;
}

// assignment operator does not copy the font
//
APIText& APIText::operator=(const APIText& src) {

    if (this != &src) {
        release();
        d3dfont   = nullptr;
        align     = src.align;
        colour    = src.colour;
        weight    = src.weight;
        precision = src.precision;
        quality   = src.quality;
        typeFace  = src.typeFace;
        fontHght  = src.fontHght;
    }

    return *this;
}

// setup retrieves an interface to the font COM object 
//
void APIText::setup() {

    // retrieve an API interface to the font object 
    if (FAILED(D3DXCreateFont(d3dd, fontHght, 0, weight, 0, 0, 
     DEFAULT_CHARSET, precision, quality, FF_DONTCARE | DEFAULT_PITCH,
     typeFace, &d3dfont)))
        error(L"APIText::10 Failed to retrieve the font interface");
}

// draw draws text within Rectf r adjusted to the display dimensions
//
void APIText::draw(const Rectf& r, const wchar_t* text) {

    if (!d3dfont) setup();

	if (d3dfont && manager && text) {
		// creates the drawing rectangle in screen coordinates
		RECT rect;
        SetRect(&rect, (int)(width * r.topLeftX), (int)(height * r.topLeftY), 
         (int)(width * r.bottomRightX), (int)(height * r.bottomRightY));
        // draws text within the drawing rectangle rect
		d3dfont->DrawText(manager, text, -1, &rect, align, colour);
	}
}

// suspend detaches the font from the APIDisplay device
//
void APIText::suspend() {

    if (d3dfont)
        d3dfont->OnLostDevice();
}

// restore re-attaches to the APIDisplay device
//
bool APIText::restore() {

    bool rc = false;

    if (d3dfont)
        rc = d3dfont->OnResetDevice() == D3D_OK;

    return rc;
}

// release releases the interface to the font object 
//
void APIText::release() {

	suspend();

    if (d3dfont) {
        d3dfont->Release();
        d3dfont = nullptr;
    }
}

// destructor releases the font object
//
APIText::~APIText() {

	release();
}

