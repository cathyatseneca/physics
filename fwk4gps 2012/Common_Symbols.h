#ifndef _COMMON_SYMBOLS_H_
#define _COMMON_SYMBOLS_H_

/* Header for the Model Actions, Model Sounds, and Common Symbols and structs
 *
 * Common_Symbols.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- Model Enumerations -------------------------
//
// Primitive Types
//
typedef enum PrimitiveType {
    POINT_LIST     = 1,
    LINE_LIST      = 2,
    LINE_STRIP     = 3,
    TRIANGLE_LIST  = 4,
    TRIANGLE_STRIP = 5,
    TRIANGLE_FAN   = 6,
} PrimitiveType;

// Rendering State Flags

typedef enum RenderState {
    ALPHA_BLEND    = 1,
    Z_ENABLE       = 2,
    LIGHTING       = 3
} RenderState;

// Light Types
//
typedef enum LightType {
    POINT_LIGHT,
    SPOT_LIGHT,
    DIRECTIONAL_LIGHT
} LightType;

// Model Actions
//
// to add an action
// - add the enumeration constant for the new action
// - add the friendly description of the new action
// - add the default mappings for the new action in Mappings.h
//
// enumeration constants
typedef enum Action {
	FIRE_CANNONBALL,				//space
	CANNON_ROTATE_LEFT,				//A
	CANNON_ROTATE_RIGHT,			//D
	CANNON_ROTATE_BACK,				//S
	CANNON_ROTATE_FWD,				//W
	CLOCKWISE_MOMENT,				//Q
	COUNTERCLOCKWISE_MOMENT,		//E
	AUD_VOLUME_INC,
	AUD_VOLUME_DEC,
	AUD_SPEED_UP,
	AUD_SLOW_DOWN,
	HUD_DISPLAY,
	HUD_LEFT,
	HUD_RIGHT,
	HUD_UP,
	HUD_DOWN,

} Action;

// user friendly descriptions of actions - used in the dialog box
// (descriptions should not exceed MAX_DESC characters)
//
#define ACTION_DESCRIPTIONS {\
	L"fire cannonball",   \
	L"Rotate Cannon to the left",   \
	L"Rotate Cannon to the right",   \
	L"Rotate Cannon to the back",   \
	L"Rotate Cannon to the front",   \
	L"clockwise moment to platform", \
	L"counterclockwise moment to platform", \
	L"Increase Volume", \
	L"Decrease Volume", \
	L"Speed Up Sound",  \
	L"Slow Down Sound", \
	L"Toggle Heads Up Display",      \
	L"Heads Up Display - Move Left", \
	L"Heads Up Display - Move Right",\
	L"Heads Up Display - Move Up",   \
	L"Heads Up Display - Move Down",  \
}

// Model Sounds
//
// to add a configurable sound
// - add its enumeration constant
// - add its description
// - add the default filename for the sound in Mappings.h
//
typedef enum ModelSound {
	SND_BKGRD,
	SND_DISCR,
	SND_OBJECT,
	SND_LOCAL_L,
	SND_LOCAL_R
} ModelSound;

// friendly descriptions of configurable sounds used in the dialog box
// (descriptions should not exceed MAX_DESC characters)
//
#define SOUND_DESCRIPTIONS {\
	L"Background",\
	L"Discrete",\
	L"Object",\
	L"Local Left",\
	L"Local Right"\
}

// Font description flags
//
#define TEXT_LEFT          1
#define TEXT_RIGHT         2
#define TEXT_CENTER        4
#define TEXT_TOP           8
#define TEXT_BOTTOM       16
#define TEXT_MIDDLE       32
#define TEXT_NORMAL       64
#define TEXT_MEDIUM      128
#define TEXT_SEMIBOLD    256
#define TEXT_BOLD        512
#define TEXT_TRUETYPE   1024
#define TEXT_RASTER     2048
#define TEXT_ANTIALIAS  4096
#define TEXT_PROOF      8192
#define TEXT_CLEARTYPE 16384
#define TEXT_EXTRA     32768
#define TEXT_DEFAULT TEXT_LEFT | TEXT_TOP | TEXT_NORMAL 
#define TEXT_HEIGHT       12
#define TEXT_TYPEFACE L"ARIAL"
// font colour [0,255]
#define TEXT_R 0
#define TEXT_G 0
#define TEXT_B 0
#define TEXT_A 255
#define TEXT_COLOUR  (TEXT_A << 24 | TEXT_R << 16 | TEXT_G << 8 | TEXT_B)

// Texture filtering flags
//
#define MAX_STAGES           2
#define TEX_MIN_POINT        1
#define TEX_MIN_LINEAR       2
#define TEX_MIN_ANISOTROPIC  4
#define TEX_MAG_POINT        8
#define TEX_MAG_LINEAR      16
#define TEX_MAG_ANISOTROPIC 32
#define TEX_DEFAULT TEX_MIN_LINEAR | TEX_MAG_LINEAR

//-------------------------------- APIDisplay ---------------------------------
//
// HUD Transparency
//
#define HUD_ALPHA 1

//-------------------------------- APIAudio -----------------------------------
//
// sound parameters
//
#define SOUND_DISTANCE_FACTOR       1.0f  // metres per coordinator unit
#define ATTENUATION_DISTANCE_FACTOR 10    // where attenuation begins

//-------------------------------- Common Structs -----------------------------
//
typedef struct Recti {
    int topLeftX;
    int topLeftY;
    int bottomRightX;
    int bottomRightY;
    Recti(int tlx = 0, int tly = 0, int brx = 1, int bry = 1) : 
     topLeftX(tlx), topLeftY(tly), bottomRightX(brx), bottomRightY(bry) {}
} Recti;

typedef struct Rectf {
    float topLeftX;
    float topLeftY;
    float bottomRightX;
    float bottomRightY;
    Rectf(float tlx = 0.0f, float tly = 0.0f, float brx = 1.0f, 
     float bry = 1.0f) : 
     topLeftX(tlx), topLeftY(tly), bottomRightX(brx), bottomRightY(bry) {}
    Recti absolute(int w, int h) { 
        return Recti((int)(topLeftX * w), (int)(topLeftY * h), 
         (int)(bottomRightX * w), (int)(bottomRightY * h)); 
    }
} Rectf;

#endif
