/* Collision Geometry Implementation - Physics Scene Component - Model Branch
 *
 * CollisionGeometry.cpp
 * November 25 2010
 */

#include "iCollisionSpace.h"
#include "CollisionGeometry.h"
#include "iPhysics.h"
#include "iUtilities.h"

iCollisionSpace* CollisionGeometry::_collisionSpace = NULL;

#define TESTING 0

/**
 * Simple ctor.
 */
CollisionGeometry::CollisionGeometry() : _rb(NULL){
    if (!_collisionSpace){
#if !TESTING
//        error(L"Object::00 Couldn\'t access the collision space");
#endif
    } else if(!_collisionSpace->add(this)) {
#if !TESTING
 //       error(L"Object::01 Couldn\'t add the collision geometry to the Collision Space");
#endif
    }
}

/**
 * Simple dtor.
 */
CollisionGeometry::~CollisionGeometry(){
    if (_collisionSpace) {
        _collisionSpace->remove(this);
    }
}

/**
 * attach
 *
 * Attaches a rigid body to this collision geometry.
 */
void CollisionGeometry::attach(iPhysics* o){
    _rb = o;
}

// RB accessors
iPhysics* CollisionGeometry::getPhysics(){
    return _rb;
}
const iPhysics* CollisionGeometry::getPhysics() const{
    return _rb;
}

/**
 * Generic suspend.
 */
void CollisionGeometry::suspend() const{}

/**
 * Generic release.
 */
void CollisionGeometry::release() const{}

/**
 * Deletes this collision geom component.
 */
void CollisionGeometry::Delete() const{
    delete this;
}
