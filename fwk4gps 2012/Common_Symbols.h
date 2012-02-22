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
    MDL_PLUS_X,
    MDL_MINUS_X,
    MDL_PLUS_Y,
    MDL_MINUS_Y,
    MDL_PLUS_Z,
    MDL_MINUS_Z,
    MDL_ROLL_BOXES,
	MDL_SPIN_LEFT,
	MDL_SPIN_TOP,
    MDL_SPIN_SPOT_P,
    MDL_SPIN_SPOT_N,
    MDL_NEW_OBJECT,
    MDL_NEW_SOUND,
    MDL_NEW_LIGHT,
    MDL_DET_CHILD,
    MDL_ATT_CHILD,
	CAM_PITCH_UP,
	CAM_PITCH_DOWN,
	CAM_YAW_LEFT,
	CAM_YAW_RIGHT,
	CAM_ADVANCE,
	CAM_RETREAT,
    CAM_ROLL_LEFT,
    CAM_ROLL_RIGHT,
    CAM_STRAFE_LEFT,
    CAM_STRAFE_RIGHT,
    CAM_STRAFE_LEFT_ALT,
    CAM_STRAFE_RIGHT_ALT,
    CAM_FLY_UP,
    CAM_FLY_DOWN,
	AUD_BKGRD,
	AUD_IMPLS,
	AUD_VOLUME_INC,
	AUD_VOLUME_DEC,
	AUD_FREQ_INC,
	AUD_FREQ_DEC,
	LIGHT_POINT,
	LIGHT_SPOT,
	LIGHT_DISTANT,
    LIGHT_CLONE,
	HUD_DISPLAY,
	HUD_LEFT,
	HUD_RIGHT,
	HUD_UP,
	HUD_DOWN,
    CAMERA_SELECT,
    HUD_SELECT,
    GF_MS_DSPX,
    GF_MS_DSPY,
    GF_MS_ROTZ,
    GF_CT_POSX,
    GF_CT_POSY,
    GF_CT_DSPZ,
    GF_CT_ROTZ,
} Action;

// user friendly descriptions of actions - used in the dialog box
// (descriptions should not exceed MAX_DESC characters)
//
#define ACTION_DESCRIPTIONS {\
    L"Translate X +", \
    L"Translate X -", \
    L"Translate Y +", \
    L"Translate Y -", \
    L"Translate Z +", \
    L"Translate Z -", \
	L"Roll Boxes",   \
	L"Spin Left Box",\
	L"Spin Top Box", \
    L"Spin Spot Light +", \
    L"Spin Spot Light -", \
    L"Clone Right Box", \
    L"Clone Left Sound", \
    L"Clone SpotLight", \
    L"Detach Child Box", \
    L"Attach Child Box", \
	L"Pitch Up",     \
	L"Pitch Down",   \
	L"Yaw Left",     \
	L"Yaw Right",    \
	L"Move Camera Forward",     \
	L"Move Camera Backward",    \
    L"Roll Camera Left", \
    L"Roll Camera Right", \
    L"Strafe Camera Left", \
    L"Strafe Camera Right", \
    L"Strafe Camera Left Alt", \
    L"Strafe Camera Right Alt", \
    L"Fly Camera Up", \
    L"Fly Camera Down", \
	L"Toggle Background Sound", \
	L"Explosion Sound", \
	L"Increase Volume", \
	L"Decrease Volume", \
	L"Speed Up Sound",  \
	L"Slow Down Sound", \
	L"Toggle Point Light",   \
	L"Toggle Spot Light",    \
	L"Toggle Distant Light", \
    L"Toggle Cloned Light", \
	L"Toggle Heads Up Display",      \
	L"Heads Up Display - Move Left", \
	L"Heads Up Display - Move Right",\
	L"Heads Up Display - Move Up",   \
	L"Heads Up Display - Move Down",  \
    L"Select Camera", \
    L"Select HUD", \
    L"Mouse Displacment X", \
    L"Mouse Displacment Y", \
    L"Mouse Rotation Z", \
    L"Controller Displacement X", \
    L"Controller Displacement Y", \
    L"Controller Displacement Z", \
    L"Controller Displacement X", \
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
#define TEXT_HEIGHT       15
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
