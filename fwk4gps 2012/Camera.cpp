/* Camera Implementation - Modelling Layer
 *
 * Camera.cpp
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Camera.h"          // for Camera class definition
#include "iCoordinator.h"    // for the Coordinator Interface
#include "MathDefinitions.h" // for math functions in model coordinates
#include "ModellingLayer.h"  // for FORWARD_SPEED, ROT_SPEED
#include "Common_Symbols.h"  // for Action and Sound enumerations

//-------------------------------- Camera -------------------------------------
//
// The Camera class defines the structure of one viewpoint on the scene
//
iCamera* Camera::current = nullptr;
Matrix   Camera::view    = Matrix();

// CreateCamera creates a Camera object
//
iCamera* CreateCamera() {

	return (iCamera*)new Camera();
}

// constructor adds the Camera Instance to the coordinator
//
Camera::Camera() {

    coordinator->add(this);

    #if MODEL_Z_AXIS == FAR_TO_NEAR
    rotatey(3.14159f);
    #endif

    current = this;
}

Camera::Camera(const Camera& src) {

    coordinator->add(this);
    *this = src;
}

// update adjusts the camera's viewpoint and heading according
// to the user's interventions
//
void Camera::update() {

    int delta = now - lastUpdate;
    int dx = 0, // pitch up/down
        dy = 0, // yaw left/right
        dz = 0; // advance/retreat
    int rx = 0,
        ry = 0,
        rz = 0;

    // controller input
 //   int jx = coordinator->change(GF_CT_POSX);
 //   int jy = coordinator->change(GF_CT_POSY);
 //   int jr = coordinator->change(GF_CT_ROTZ);
 //   if (jy)
 //       rx = -(int)(jy * CTR_SPEED);
 //   if (jr)
 //       ry = (int)(jr * CTR_SPEED);
 //   if (jx)
 //       rz = (int)(jx * CTR_SPEED);

	//// mouse input
 //   int mx = coordinator->change(GF_MS_DSPX);
 //   int my = coordinator->change(GF_MS_DSPY);
 //   int mz = coordinator->change(GF_MS_ROTZ);
	//if (mx)
	//	ry += mx * MOUSE_SPEED;
	//if (my)
	//	rx += my * MOUSE_SPEED;
	//if (mz)
	//	dy += mz * MOUSE_SPEED;
	//	
	//// keyboard input
 //   if (coordinator->pressed(CAM_STRAFE_LEFT))
 //       dx -= delta;
 //   if (coordinator->pressed(CAM_STRAFE_RIGHT))
 //       dx += delta;
 //   if (coordinator->pressed(CAM_STRAFE_LEFT_ALT))
 //       dx -= delta;
 //   if (coordinator->pressed(CAM_STRAFE_RIGHT_ALT))
 //       dx += delta;
 //   if (coordinator->pressed(CAM_FLY_DOWN))
 //       dy -= delta;
 //   if (coordinator->pressed(CAM_FLY_UP))
 //       dy += delta;
 //   if (coordinator->pressed(CAM_ADVANCE))
 //       dz += delta;
 //   if (coordinator->pressed(CAM_RETREAT))
 //       dz -= delta;
 //   if (coordinator->pressed(CAM_PITCH_UP))
 //       rx -= delta;
 //   if (coordinator->pressed(CAM_PITCH_DOWN))
 //       rx += delta;
 //   if (coordinator->pressed(CAM_YAW_LEFT))
 //       ry -= delta;
 //   if (coordinator->pressed(CAM_YAW_RIGHT))
 //       ry += delta;
 //   if (coordinator->pressed(CAM_ROLL_LEFT))
 //       rz -= delta;
 //   if (coordinator->pressed(CAM_ROLL_RIGHT))
 //       rz += delta;

    // adjust camera orientation
	if ((rx || ry || rz)) {
        // yaw left/right
		if (ry)
            rotate(orientation('y'), ry * ANG_CAM_SPEED);
		// pitch up/down
        if (rx) 
            rotate(orientation('x'), rx * ANG_CAM_SPEED);
		// roll left/right
        if (rz) 
            rotate(orientation('z'), rz * ANG_CAM_SPEED);
    }
	// adjust camera position
    if ((dx || dy || dz)) { 
        Vector displacement = 
         (float) dx * CAM_SPEED * orientation('x') +
         (float) dy * CAM_SPEED * orientation('y') + 
         (float) dz * CAM_SPEED * orientation('z');
        translate(displacement.x, displacement.y, displacement.z);
    }

    current = this;

    // update the view transformation
    Vector p = position();
    Vector h = ::normal(orientation('z'));
    Vector u = ::normal(orientation('y'));
    view = ::view(p, p + h, u);
}

// destructor removes the Camera Instance from the coordinator
//
Camera::~Camera() {

    coordinator->remove(this);
}
