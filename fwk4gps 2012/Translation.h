#ifndef _TRANSLATION_H_
#define _TRANSLATION_H_

/* Internal Settings local to the Translation Layer
 *
 * Translation.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

// DO NOT REMOVE WITHOUT WRITTEN PERMISSION
#define COPYRIGHT_LINE_1 "fwk4gps is copyright (c) 2012, Chris Szalwinski. Source code is distributed under the"
#define COPYRIGHT_LINE_2 "PostgreSQL Open Source License (TPL).  For the full text, see ../Licenses.txt"
#define COPYRIGHT_LINE_3 "Audio clips are redistributed under Creative Commons Sampling Plus 1.0 licenses.  "
#define COPYRIGHT_LINE_4 "For full text, see ../Licenses.txt"

// maximum number of attached input devices
#define MAX_INPUT_DEVICES 32

// Input Device Types
//
typedef enum UserDeviceType {
    KEYBOARD,
    POINTER,
    CONTROLLER, 
} UserDeviceType;

// key symbols used for action-key mapping (do not initialize these constants)
//
typedef enum Key { 
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_0,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_SPACE,
    KEY_ENTER,
    KEY_UP   ,
    KEY_DOWN, 
    KEY_PGUP, 
    KEY_PGDN, 
    KEY_LEFT, 
    KEY_RIGHT,
    KEY_NUM1,
    KEY_NUM2,
    KEY_NUM3,
    KEY_NUM4,
    KEY_NUM5,
    KEY_NUM6,
    KEY_NUM7,
    KEY_NUM8,
    KEY_NUM9,
    KEY_ESCAPE,
	KEY_SEMICOLON,
	KEY_APOSTROPHE,
	KEY_O_BRACKET,
	KEY_C_BRACKET,
	KEY_BACKSLASH,
	KEY_COMMA,
	KEY_PERIOD,
	KEY_SLASH,
	KEY_TIMES,
	KEY_GRAVE,
	KEY_MINUS,
	KEY_UNDERSCORE,
	KEY_EQUALS,
	KEY_PLUS
} Key;

// keys listed in the user dialog
//
#define KEY_DESCRIPTIONS { \
	L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H", L"I", L"J", \
	L"K", L"L", L"M", L"N", L"O", L"P", L"Q", L"R", L"S", L"T", \
	L"U", L"V", L"W", L"X", L"Y", L"Z", \
	L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"0", \
	L"F1", L"F2", L"F3", L"F4", L"F5", L"F6", L"F7", \
	L"F8", L"F9", L"F10", L"F11", L"F12", \
	L"Space", L"Enter", L"Up", L"Down", L"PageUp", L"PageDown", \
	L"Left", L"Right", \
	L"NumPad 1", L"NumPad 2", L"NumPad 3", L"NumPad 4", L"NumPad 5", \
	L"NumPad 6", L"NumPad 7", L"NumPad 8", L"NumPad 9", \
	L"Escape", L";", L"'", L"[", L"]", L"\\", L",", \
	L".", L"/", L"*", L"`", L"-", L"_", L"=", L"+" \
}

// pointer buttons listed in the user dialog
//
#define POINTER_OBJECT_DESCRIPTIONS { \
    L"Left Mouse", L"Center Mouse", L"Right Mouse" \
}

// controller buttons listed in the user dialog
//
#define CONTROLLER_OBJECT_DESCRIPTIONS { \
    L"Trigger", L"Button 1", L"Button 2", L"Button 3", L"Button 4", \
    L"Button 5", L"Button 6", L"Button 7", L"Button 8", L"Button 9", \
    L"Button 10", L"North", L"East", L"West",  L"South" \
}

#endif
