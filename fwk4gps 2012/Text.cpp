/* Text Implementation - Modelling Layer
 *
 * Text.cpp
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Text.h"           // for the Text class definition
#include "iCoordinator.h"   // for the Coordinator Interface
#include "iAPIText.h"       // for the APIText Interface
#include "iHUD.h"           // for the HUD Interface
#include "iUtilities.h"     // for error()
#include "ModellingLayer.h" // for TEXT_MIN, TEXT_MAX, TEXT_ON, TEXT_OFF
#include "Common_Symbols.h" // symbols common to Modelling/Translation layers

//-------------------------------- Text --------------------------------------
//
// The Text class defines the structure of a text item in the Modelling Layer
//
// CreateText creates a simple Text object
//
iText* CreateText(Rectf r, void* h, const wchar_t* s, int j, const wchar_t* u,
 unsigned f, unsigned c) {

	return new Text(r, h, s, j, u, f, c);
}

// CreateText creates a compound Text object
//
iText* CreateText(Rectf r, void* h, const wchar_t* s, 
 const wchar_t* (*v)(wchar_t*, const iFrame*, char, unsigned), 
 iFrame* q, char a, unsigned x, int j, const wchar_t* u, unsigned f, 
 unsigned c) {

	return new Text(r, h, s, v, q, a, x, j, u, f, c);
}

iText* CreateText(Rectf r, void* h, const wchar_t* s, 
 const wchar_t* (*v)(wchar_t*, const iFrame*, char, unsigned), 
 iFrame** q, char a, unsigned x, int j, const wchar_t* u, unsigned f, 
 unsigned c) {

	return new Text(r, h, s, v, q, a, x, j, u, f, c);
}

iText* CreateText(Rectf r, void* h, const wchar_t* t, 
 const wchar_t* (*v)(wchar_t*, const iSwitch*), iSwitch* s, int j, 
 const wchar_t* u, unsigned f, unsigned c) {

     return new Text(r, h, t, v, s, j, u, f, c);
}

iText* Clone(const iText* src) {

    return (iText*)src->clone();
}

// init adds the address of the current object to the coordinator, creates
// the APIText object, validates the rectangle dimensions, and copies in 
// the text string
// 
void Text::init(Rectf& r, const wchar_t* text, const wchar_t* face, int height,
 unsigned flags, unsigned colour) {

    coordinator->add(this);

    font = CreateAPIText(face, height, flags, colour);

    if (r.topLeftX < TEXT_MIN) r.topLeftX = TEXT_MIN;
    else if (r.topLeftX > TEXT_MAX) r.topLeftX = TEXT_MAX;
    if (r.topLeftY < TEXT_MIN) r.topLeftY = TEXT_MIN;
    else if (r.topLeftY > TEXT_MAX) r.topLeftY = TEXT_MAX;
    if (r.bottomRightX < TEXT_MIN) r.bottomRightX = TEXT_MIN;
    else if (r.bottomRightX > TEXT_MAX) r.bottomRightX = TEXT_MAX;
    if (r.bottomRightY < TEXT_MIN) r.bottomRightY = TEXT_MIN;
    else if (r.bottomRightY > TEXT_MAX) r.bottomRightY = TEXT_MAX;
    rect  = new Rectf;
    *rect = r;

    if (text) set(text);
}

// constructor initialializes the instance variables and stores the 
// the object's address, the rectangle, and the text string
//
Text::Text(Rectf r, void* h, const wchar_t* text, int j, const wchar_t* type,
 unsigned flags, unsigned colour) :  intToWCStr(nullptr), boolToWCStr(nullptr), 
 swtch(nullptr), pFrame(nullptr), frame(nullptr), hud((iHUD*)h) {

    init(r, text, type, j, flags, colour);
}

Text::Text(Rectf r, void* h, const wchar_t* text, 
 const wchar_t* (*fn)(wchar_t*, const iFrame*, char, unsigned), iFrame* q, 
 char c, unsigned x, int j, const wchar_t* type, unsigned flags, 
 unsigned colour) : frame(q), axis(c), factor(x), intToWCStr(fn),
 boolToWCStr(nullptr), swtch(nullptr), pFrame(nullptr), hud((iHUD*)h) {

    init(r, text, type, j, flags, colour);
}

Text::Text(Rectf r, void* h, const wchar_t* text, 
 const wchar_t* (*fn)(wchar_t*, const iFrame*, char, unsigned), iFrame** q, 
 char c, unsigned x, int j, const wchar_t* type, unsigned flags, 
 unsigned colour) : frame(nullptr), axis(c), factor(x), intToWCStr(fn), 
 boolToWCStr(nullptr), swtch(nullptr), pFrame(q), hud((iHUD*)h) {

    init(r, text, type, j, flags, colour);
}

Text::Text(Rectf r, void* h, const wchar_t* text, 
 const wchar_t* (*fn)(wchar_t*, const iSwitch*), iSwitch* s, int j, 
 const wchar_t* type, unsigned flags, unsigned colour) : swtch(s), axis(' '),
 factor(0), boolToWCStr(fn), frame(nullptr), intToWCStr(nullptr), hud((iHUD*)h) {

    init(r, text, type, j, flags, colour);
}

// copy constructor adds the new object's address to the coordinator,
// initializes the instance pointers and calls the assignment operator
//
Text::Text(const Text& src) {

    coordinator->add(this);
    font  = nullptr;
	*this = src;
}

// assignment operator discards old data and copies over data from src
// creates a new APIText object for the current Text object
//
Text& Text::operator=(const Text& src) {

	if (this != &src) {
		if (font)
			font->Delete();
        if (src.font)
            font = src.font->clone();
        else
            font = nullptr;
        if (rect)
            delete rect;
        rect = new Rectf;

        hud         = src.hud;
        intToWCStr  = src.intToWCStr;
        boolToWCStr = src.boolToWCStr;
        frame       = src.frame;
        swtch       = src.swtch;
        factor      = src.factor;
        axis        = src.axis;
        *rect       = *src.rect;
		strcpy(label, src.label, MAX_DESC);
	}

	return *this;
}

// set copies the string text into the Text object
//
void Text::set(const wchar_t* text) {

    strcpy(label, text, MAX_DESC);
}

// render draws the text string using the APIText object
//
void Text::render() {

    wchar_t text[MAX_DESC + 1];

    // value follows label
    if (intToWCStr && frame) {
        wchar_t value[MAX_DESC + 1];
        intToWCStr(value, frame, axis, factor);
        strcpy(text, label, MAX_DESC);
        strcat(text, value, MAX_DESC);
    }
    else if (intToWCStr && pFrame) {
        wchar_t value[MAX_DESC + 1];
        intToWCStr(value, *pFrame, axis, factor);
        strcpy(text, label, MAX_DESC);
        strcat(text, value, MAX_DESC);
    }
    // value precedes label
    else if (boolToWCStr && swtch) {
        boolToWCStr(text, swtch);
        strcat(text, label, MAX_DESC);
    }
    else
        strcpy(text, label, MAX_DESC);

    // retrieves the current hud coordinates, if any
    Rectf hudRect = Rectf(0, 0, 1, 1);
    if (hud) 
        hudRect = hud->getRect();
    float width  = hudRect.bottomRightX - hudRect.topLeftX;
    float height = hudRect.bottomRightY - hudRect.topLeftY;
    Rectf textRect(
     hudRect.topLeftX + width * rect->topLeftX, 
     hudRect.topLeftY + height * rect->topLeftY, 
     hudRect.topLeftX + width * rect->bottomRightX, 
     hudRect.topLeftY + height * rect->bottomRightY);

    if (font)
		font->draw(textRect, text);
}

// suspends suspends the font associated with the text object
//
void Text::suspend() {

	if (font)
		font->suspend();
}

// restore restores the font associated with the text object
//
void Text::restore() {

	if (font)
		font->restore();
}

// release releases the font associated with the text object
//
void Text::release() {

	if (font)
		font->release();
}

// destructor destroys the font associated with this text object
// and removes the pointer to the Text object from the coordinator
//
Text::~Text() {

    if (font)
        font->Delete();
    if (rect)
        delete rect;
    coordinator->remove(this);
}


//-------------------------------- Text Generating Function -------------------
//
// orient returns a text string with the orientation of the local c axis
// of *frame
//
const wchar_t* orient(wchar_t* str, const iFrame* frame, char c, unsigned f) {

	if (frame) {
        Vector v = frame->orientation(c);
		sprintf(str, (int)(f*v.x), (int)(f*v.y), (int)(f*v.z));
	}

	return str;
}

// position returns a text string with the world position of *frame
//
const wchar_t* position(wchar_t* str, const iFrame* frame, char c, unsigned f) 
{

	if (frame) {
        Vector v = frame->position();
		sprintf(str, (int)(f*v.x), (int)(f*v.y), (int)(f*v.z));
	}

	return str;
}

// onOff returns a text string with the on off state of *iSwitch
//
const wchar_t* onOff(wchar_t* str, const iSwitch* item) {

	if (item)
        strcpy(str, item->isOn() ? TEXT_ON : TEXT_OFF, MAX_DESC);

	return str;
}
