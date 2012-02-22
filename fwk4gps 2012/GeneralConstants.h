#ifndef _GENERAL_CONSTANTS_H_
#define _GENERAL_CONSTANTS_H_

/* Header for General Constants
 *
 * GeneralConstants.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#define WND_CAPTION      L"fwk4gps"

#define MAX_DESC 255     // max string length throughout

// Table of Available Z Directions
#define NEAR_TO_FAR ( 1)
#define FAR_TO_NEAR (-1)

// make your selections below 
//

// the absolute direction of the z-axis 
//
#define MODEL_Z_AXIS NEAR_TO_FAR
//#define MODEL_Z_AXIS FAR_TO_NEAR

// the direction of the z-axis with respect to the camera heading
//
#define Z_AXIS NEAR_TO_FAR
//#define Z_AXIS FAR_TO_NEAR

#if Z_AXIS == NEAR_TO_FAR
#define WND_NAME WND_CAPTION L" (Z Axis Near to Far)"
#elif Z_AXIS == FAR_TO_NEAR
#define WND_NAME WND_CAPTION L" (Z Axis Far to Near)"
#endif

// Default Window Dimensions
#define WND_WIDTH  800   // minimum window width
#define WND_HEIGHT 600   // minimum window height

#endif
