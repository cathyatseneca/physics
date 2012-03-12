/* Frame Implementation - Modelling Layer
 *
 * Frame.cpp
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * Chris Szalwinski
 */

#include "Frame.h"           // for the Frame class definition
#include "MathDefinitions.h" // for position, rotation

//-------------------------------- Frame --------------------------------------
//
// The Frame object represents a reference frame in the Modelling Layer
//
Frame::Frame() : T(1), parent(0) { }

//returns this object's position relative to the parent
Vector Frame::relativePosition(){
	return ::position(T);
}
// position returns the Frame's position vector in world space
//
// Note that this method constructs the vector recursively
//
Vector Frame::position() const {

	return parent ? ::position(T) * parent->rotation() + parent->position() : 
	 ::position(T);
}

// rotation returns the Frame's orientation in world space
//
// Note that this method constructs the transformation recursively
//
Matrix Frame::rotation() const {

	return parent ? ::rotation(T) * parent->rotation() : ::rotation(T);
}

// orientation returns the orientation of local vector v in world space
//
Vector Frame::orientation(const Vector& v) const {

    return v * rotation();
}


// orientation returns the orientation of the local ? axis of the
// Frame in world space
//
Vector Frame::orientation(char axis) const {

    Vector v;

    switch(axis) {
        case 'x':
            v = Vector(1, 0, 0);
            break;
        case 'y':
            v = Vector(0, 1, 0);
            break;
        case 'z':
            v = Vector(0, 0, 1);
            break;
    }

    return orientation(v);
}

// world returns a reference to the homogeneous transformation of
// the Frame with respect to world space corrected for
// the coordinate system of the coordinator
//
// Note that this function constructs the transformation recursively
//
Matrix Frame::world() const {

    Matrix w = parent ? T * parent->world() : T;
    w.m43 *= MODEL_Z_AXIS;
    return w;
}

// attachTo attaches the current Frame to iFrame* newParent
//
// if a parent existed, recalculates the current position and orientation
// as world values and realigns the current Frame with the new parent, so 
// that the new transformation matrix holds the values relative to the new
// parent and the attachment is a smooth one; 
// if a parent did not exist, assumes that the current position and 
// orientation are relative to the new parent otherwise applies the 
// existing transformation 
//
void Frame::attachTo(iFrame* newParent) {

	// detach from current parent, if any
    if (parent)
        T = world();
    parent = 0;
    // attach to newParent
	parent = newParent;
    if (parent) {
        // convert rotation to a relative rotation wrt parent frame
		Matrix m = parent->rotation();
		m = m.transpose();
		T.rotate(m);
        // express offset in local coordinates wrt to parent frame
        Vector p = (::position(T) - parent->position()) * m;
        T.m41 = p.x;
        T.m42 = p.y;
        T.m43 = p.z;
    }
}

//-------------------------------- Shape ----------------------------
//
void Shape::setRadius(float r) { 
    radius = r; 
    sphere = true;
}

void Shape::setRadius(float x, float y, float z) { 
    setRadius(sqrtf(x*x + y*y + z*z)); 
}

void Shape::setPlane(Vector n, float d) { 
    plane  = true;
    normal = n;
    radius = d;
}

void Shape::setAxisAligned(Vector min, Vector max) {
    axisAligned = true;
    minimum     = min;
    maximum     = max;
}

bool collision(const Vector& an, const Vector& ax, const Vector& bne,
 const Vector& bxe, Vector& d);

// collision receives the addresses of two shapes and the translation that
// brought them to their current states, determines if a collision occurred
// and returns in the translation vector the translation that needs to be
// applied to correct for the collision, if any occured
//
bool collision(const Shape* f1, const Shape* f2, Vector& d) {

    bool collide = false;

    if (f1->sphere && f2->sphere) {
        float dd = f1->radius + f2->radius;
        Vector separation = f1->position() - f2->position();
        collide = dot(separation, separation) <= dd * dd;
        // needs to be refined
        d.x = d.y = d.z = 0;
    }
    else if (f1->sphere && f2->plane) {
       collide = dot(f2->normal, f1->position() - f2->position()) <= 
        f1->radius + f2->radius;
       // needs to be refined
       d.x = d.y = d.z = 0;
    }
    else if (f1->plane && f2->sphere) {
       collide = dot(f1->normal, f2->position() - f1->position()) <= 
        f1->radius + f2->radius;
       // needs to be refined
       d.x = d.y = d.z = 0;
    }
    else if (f1->sphere && f2->axisAligned) {
        Vector ax = f1->position() + f1->getRadius() * Vector(1, 1, 1);
        Vector an = f1->position() + f1->getRadius() * Vector(-1, -1, -1);
        Vector bx = f2->maximum + f2->position();
        Vector bn = f2->minimum + f2->position();
        collide = collision(an, ax, bn, bx, d);
    }
    else if (f1->axisAligned && f2->sphere) {
        Vector ax = f1->maximum + f1->position();
        Vector an = f1->minimum + f1->position();
        Vector bx = f2->position() + f2->getRadius() * Vector(1, 1, 1);
        Vector bn = f2->position() + f1->getRadius() * Vector(-1, -1, -1);
        collide = collision(an, ax, bn, bx, d);
    }
    else if (f1->axisAligned && f2->axisAligned) {
        Vector ax = f1->maximum + f1->position();
        Vector an = f1->minimum + f1->position();
        Vector bx = f2->maximum + f2->position();
        Vector bn = f2->minimum + f2->position();
        collide = collision(an, ax, bn, bx, d);
    }

    return collide;
}

// collision determines if a collision occurs between shapes bounded by
// [an,ax] and [bne,bxe] where d is the relative translation and returns
// in d the corrective translation in the event that collison has occurred 
//
bool collision(const Vector& an, const Vector& ax, const Vector& bne,
 const Vector& bxe, Vector& d) {

     bool collide;

    // is there collision?
    collide = 
        ax.x >= bne.x && an.x <= bxe.x &&
        ax.y >= bne.y && an.y <= bxe.y &&
        ax.z >= bne.z && an.z <= bxe.z;
    if (collide) {
        // initial position
        Vector bx = bxe - d;
        Vector bn = bne - d;
        // was there initial penetration?
        bool penetration =
            ax.x >= bn.x && an.x <= bx.x &&
            ax.y >= bn.y && an.y <= bx.y &&
            ax.z >= bn.z && an.z <= bx.z;
        if (penetration) {
            // correct for initial penetration, if necessary
            // y-z plane penetration
            float lambdax = -1.0f;
            if (bx.x >= an.x && bn.x < an.x && d.x > 0)
                lambdax = (an.x - bx.x) / d.x;
            else if (bn.x <= ax.x && bx.x > ax.x && d.x < 0) 
                lambdax = (ax.x - bn.x) / d.x;
            // x-z plane penetration
            float lambday = -1.0f;
            if (bx.y >= an.y && bn.y < an.y && d.y > 0)
                lambday = (an.y - bx.y) / d.y;
            else if (bn.y <= ax.y && bx.y > ax.y && d.y < 0)
                lambday = (ax.y - bn.y) / d.y;
            // x-y plane penetration
            float lambdaz = -1.0f;
            if (bx.z >= an.z && bn.z < an.z && d.z > 0)
                lambdaz = (an.z - bx.z) / d.z;
            else if (bn.z <= ax.z && bx.z > ax.z && d.z < 0)
                lambdaz = (ax.z - bn.z) / d.z;
            // determine plane of contact
            if (lambdax > lambday && lambdax > lambdaz) {
                d.x = lambdax * d.x;
                d.y = d.z = 0;
            } 
            else if (lambday > lambdax && lambday > lambdaz) {
                d.y = lambday * d.y;
                d.x = d.z = 0;
            }
            else if (lambdaz > lambday && lambdaz > lambdax) {
                d.z = lambdaz * d.z;
                d.x = d.y = 0;
            }
            else {
                d.x = d.y = d.z = 0;
            }
        }
        else {
            // initial separation
            // y-z plane collision
            float lambdax = 1.0f;
            if (bx.x <= an.x && d.x > 0)
                lambdax = (an.x - bx.x) / d.x;
            else if (bn.x >= ax.x && d.x < 0)
                lambdax = (ax.x - bn.x) / d.x;
            // x-z plane collision
            float lambday = 1.0f;
            if (bx.y <= an.y && d.y > 0)
                lambday = (an.y - bx.y) / d.y;
            else if (bn.y >= ax.y && d.y < 0)
                lambday = (ax.y - bn.y) / d.y;
            // x-y plane collision
            float lambdaz = 1.0f;
            if (bx.z <= an.z && d.z > 0)
                lambdaz = (an.z - bx.z) / d.z;
            else if (bn.z >= ax.z && d.z < 0)
                lambdaz = (ax.z - bn.z) / d.z;
            // determine plane of first contact
            if (lambdax < lambday && lambdax < lambdaz) {
                d.x = - 2 * (1 - lambdax) * d.x;
                d.y = d.z = 0;
            } 
            else if (lambday < lambdax && lambday < lambdaz) {
                d.y = - 2 * (1 - lambday) * d.y;
                d.x = d.z = 0;
            }
            else if (lambdaz < lambday && lambdaz < lambdax) {
                d.z = - 2 * (1 - lambdaz) * d.z;
                d.x = d.y = 0;
            }
            else {
                d.x = d.y = d.z = 0;
            }
        }
    }

    return collide;
}

//added for physics
// rotate rotates the Frame using rotation matrix rot
void Frame::rotate(const Matrix& rot) {

    Vector cr(T.m41, T.m42, T.m43);				//centre of rotation of the object (its position for now)
    translate(-cr.x, -cr.y, -cr.z);				//move object to its centre of rotation
    T *= rot;									//rotate it
    translate(cr.x, cr.y, cr.z);				//move it back.
}
