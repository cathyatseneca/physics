#ifndef _MAPPINGS_H_
#define _MAPPINGS_H_

/* Header for Mappings of Actions and Sounds
 *
 * Mappings.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

// initial mappings of actions to keys
//
#define ACTION_KEY_MAP {KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, \
    KEY_R, KEY_T, KEY_Y, KEY_SEMICOLON, \
    KEY_BACKSLASH, KEY_U, KEY_I, KEY_O, KEY_P, KEY_V, \
    KEY_PGUP, KEY_PGDN, KEY_Z, KEY_C, KEY_W, KEY_S, KEY_Q, KEY_E, \
    KEY_A, KEY_D, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, \
	KEY_F3, KEY_F4, KEY_F6, KEY_F7, KEY_W, KEY_S, \
    KEY_J, KEY_K, KEY_L, KEY_H, \
    KEY_F, KEY_V,   \
	KEY_B, KEY_N, KEY_M, \
    KEY_X, KEY_1, \
    KEY_1, KEY_1, KEY_1, KEY_1, KEY_1, KEY_1, KEY_1 \
}

// initial mappings of pointer objects to actions
//
#define PTR_OBJ_ACTION_MAP { \
    CAM_PITCH_UP, CAM_PITCH_DOWN, CAM_YAW_LEFT \
}

// initial mappings of controller objects to actions
//
#define CTR_OBJ_ACTION_MAP { \
    CAM_PITCH_UP, CAM_PITCH_DOWN, CAM_YAW_LEFT, CAM_YAW_RIGHT, \
    CAM_ADVANCE, CAM_RETREAT, CAM_ROLL_LEFT, CAM_ROLL_RIGHT, \
    CAM_STRAFE_LEFT, CAM_STRAFE_RIGHT, CAM_STRAFE_LEFT_ALT, \
    CAM_STRAFE_RIGHT_ALT, CAM_FLY_UP, CAM_FLY_DOWN, AUD_BKGRD, \
}

// initial selection of configurable sounds 
//
// include the authors name for CCS+ accreditation
//
#define SOUND_MAPPINGS {\
	L"Crickets (by reinsamba) .xwma",\
	L"Gong (by reinsamba) .xwma",\
	L"Street_accordeonist (by reinsamba) .xwma",\
	L"Goat (by reinsamba) .xwma",\
	L"Fortaleza election campaign (by reinsamba) .xwma",\
}

#endif
