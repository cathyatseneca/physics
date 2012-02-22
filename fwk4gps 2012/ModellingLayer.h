#ifndef _MODELLING_LAYER_H_
#define _MODELLING_LAYER_H_

/* Internal Settings local to the Modelling Layer
 *
 * ModellingLayer.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "GeneralConstants.h" // framework-wide configuration settings

// File Directories
#define AUDIO_DIRECTORY   L"..\\..\\resources\\audio"
#define TEXTURE_DIRECTORY L"..\\..\\resources\\textures"
#define ASSET_DIRECTORY   L"..\\..\\resources\\assets"

// Timing Factors
//
// fps maximum - should be > flicker fusion threshold
#define FPS_MAX          200
// latency - keystroke time interval 
#define KEY_LATENCY     (unitsPerSec / 2)

// camera settings
//
// camera speed control factors
#define CAM_SPEED       0.02f
#define TURNING_RADIUS  120.00f
#define ANG_CAM_SPEED   ( CAM_SPEED / TURNING_RADIUS )

// frame motion parameters
//
// factors applied to the time interval
#define FORWARD_SPEED 10.0f / unitsPerSec
#define ROT_SPEED     0.03f * FORWARD_SPEED
#define CONSTANT_ROLL 10.0f * ROT_SPEED

// input device motion conversion factors - 
//
// mouse motion conversion factors
#define MOUSE_SPEED             10
#define POINTER_SCALE           10
// controller conversion factors
#define CTR_SPEED               0.05f
#define CTR_DISPLACEMENT_FACTOR 0.015f 
#define CTR_ROTATION_FACTOR     0.015f 

// audio controls
//
// initial volume settings
#define MIN_VOLUME		    0
#define DEFAULT_VOLUME	  500
#define MAX_VOLUME	     1000
#define STEP_VOLUME         1
// initial frequency settings
#define MIN_FREQUENCY	    0
#define DEFAULT_FREQUENCY 500
#define MAX_FREQUENCY    1000
#define STEP_FREQUENCY		1

// foreground texture transparency [\x00,\xff]
#define TEXTURE_ALPHA '\x20'

// hud and text controls
//
// hud background image
#define HUD_IMAGE L"hudBackground.bmp"
// relative limits on the size of the hud [0,1]
#define HUD_MAX    0.99f
#define HUD_MIN    0.01f
#define HUD_SPEED  (0.5f /* screen units per sec */ / unitsPerSec) 
// hud location within window [0,1]
#define HUD_W     0.43f
#define HUD_H     0.43f
// text limits within the hud [0,1]
#define TEXT_MAX   0.99f
#define TEXT_MIN   0.01f

// Text status
#define TEXT_ON  L" + " // symbol for on
#define TEXT_OFF L" o " // symbol for off

// Coordinator categories
//
typedef enum Category {
    OPAQUE_OBJECT,
    TRANSLUCENT_OBJECT,
    TEST_COLOUR,
    ALL_OBJECTS,
    ALL_HUDS,
    ALL_SOUNDS,
} Category;

#endif
