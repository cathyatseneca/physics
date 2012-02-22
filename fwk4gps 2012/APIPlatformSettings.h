#ifndef _API_PLATFORM_SETTINGS_H_
#define _API_PLATFORM_SETTINGS_H_

/* Header for Configuring the Translation Layer
 *
 * APIPlatformSettings.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * contributions by: Jon Buckley '11
 * distributed under TPL - see ../Licenses.txt
 */

#include <d3d9.h>      // for basic D3D
#include <d3dx9.h>     // for D3D utilities
#include <d3dx9core.h> // for LPD3DXFONT
#include <xaudio2.h>   // for XAudio2
#include <X3DAudio.h>  // for X3D

#define D3D_NO_DOC_FORMATS 6
#define D3D_DOC_FORMATS { D3DFMT_X8R8G8B8, D3DFMT_A8R8G8B8,\
     D3DFMT_A2R10G10B10, D3DFMT_X1R5G5B5, D3DFMT_A1R5G5B5,\
     D3DFMT_R5G6B5 }
// short descriptions for each format. In practice, you
// just might want to call them either "16" or "32" (so
// the user is deciding between 16 and 32 bit colour)
#define D3D_FORMAT_DESC { L"X32", L"A32", L"A30+2", L"X15", L"A16", L"16" }

// APIDisplay background colour
//
#define BGROUND_R 200
#define BGROUND_G 200
#define BGROUND_B 200

#define MAX_ACTIVE_LIGHTS 8

#define SOUND_DISTANCE_SCALE 1.0f

#endif
