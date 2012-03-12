#ifndef _DESIGN_H_
#define _DESIGN_H_

/* Design Definition - Modelling Layer
 *
 * Design.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Coordinator.h" // for the Coordinator class definition
#include "iphysics.h"
#include "iCollisionSpace.h"
#include "tlist.h"
//-------------------------------- Design -------------------------------------
//
// The Design class defines the structure of the game design
//
class iObject;
class iLight;
class iSound;
class iHUD;
const int MAX_CANNONBALLS=100;
const Colour red(1,0,0);
const Colour green(0,1,0);
const Colour blue(0,0,1);
const Colour white(0,0,0);
class Design : public Coordinator {
    iPhysics* cannon_;
	TList<iPhysics*> flyinglist_;
	iPhysics* cannonball_;
	bool isFlying_[MAX_CANNONBALLS];
	iPhysics* platform_;
	int numCannonballs_;
	iPhysics* bigGreenBox_;
	iPhysics* bigBlueBox_;
	int lastfired_;                //time last time something was fired
	iCollisionSpace* cs_;

	iSound*   background;          // points to the background sound
	iSound*   discrete;            // points to the discrete sound
	iSound*   locall;              // points to local sound on the left
	iSound*   localr;              // points to local sound on the right

    iLight*   pointLight;          // points to the point light
	iLight*   spotLight;           // points to the spot light
	iLight*   distantLight;        // points to the directional light
    iLight*   cloneLight;          // points to the cloned light
    iLight*   topLight;            // points to the spot on the top box

    iHUD*     hud;                 // points to the hud

    iCamera*  camera;              // points to the main camera

    iObject*  rollLeft;            // points to left spinner
    iObject*  rollRight;           // points to right spinner
	iObject*  spinTop;             // points to top spinner
    iObject*  floor;               // points to the floor box
    iObject*  child;               // points to the child box
	iSound*   objectSnd;           // points to the object sound
    iSound*   cloneSound;          // points to the cloned sound

	iText* velocitytxt_;
	iText* deltatxt_;
	iText* positiontxt_;
	int lasttextupdate;

    Design(const Design& s);            // prevents copying
    Design& operator=(const Design& s); // prevents assignment

  public:
    Design(void*, int);
    void initialize();
    void update();
};

#endif
