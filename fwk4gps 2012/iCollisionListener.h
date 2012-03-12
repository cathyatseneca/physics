#ifndef _I_COLLISIONLISTENER_H_
#define _I_COLLISIONLISTENER_H_

/* Collision Listener Interface - Physics Scene Component - Model Branch
 *
 * iCollisionListener.h
 * December 8 2010
 */

//--------------------------- iCollisionListener ------------------------------
//
// A listener for collision events. Can be attached to a rigid body.
//

#include "MathDeclarations.h"

class iRigidBody;

class iCollisionListener {
public:
    virtual void HitWall(
        iRigidBody* other, 
        Vector hitNormal, 
        Vector hitLocation)                                             = 0;
};
#endif
