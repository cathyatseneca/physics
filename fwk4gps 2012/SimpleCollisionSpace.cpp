/* Simple Collision Space Implementation - Physics Scene Component - Model Branch
 *
 * SimpleCollisionSpace.cpp
 * November 17 2010
 */

//--------------------------- SimpleCollisionSpace ----------------------------
//
// A simple collision space. Performance is N^2 rigid bodies.
//
#include "iSimpleCollisionSpace.h"
#include "SimpleCollisionSpace.h"
#include "iCollisionGeometry.h"
#include "iPhysics.h"
#include "CollisionTests.h"

/**
 * Creates a new simple collision space and returns a pointer to it.
 */
iCollisionSpace* CreateSimpleCollisionSpace(size_t samplesPerSecond){
    SimpleCollisionSpace* cs = new SimpleCollisionSpace();
    cs->_samplesPerSecond = samplesPerSecond;
    return cs;
}

/**
 * SimpleCollisionSpace ctor
 *
 * Trivial constructor.
 */
SimpleCollisionSpace::SimpleCollisionSpace()
{}

/**
 * SimpleCollisionSpace dtor
 * Trivial dtor.
 */
SimpleCollisionSpace::~SimpleCollisionSpace(){}

/**
 * add
 *
 * Adds a new collision geometry to this space.
 */
bool SimpleCollisionSpace::add(iCollisionGeometry* o){
    bool rv = false;
    if (o){
        _geomList.push_back(o);
        rv = true;
    }
    return rv;
}

/**
 * remove
 *
 * Remove a given collision geometry from this space.
 */
void SimpleCollisionSpace::remove(iCollisionGeometry* o){
    if (o){
        _geomList.remove(o);
    }
}


/**
 * getAttachedGeometry
 *
 * Retrieves a list of all attached collision geometry.
 */
const std::list<iCollisionGeometry*>& SimpleCollisionSpace::getAttachedGeometry(){
    return _geomList;
}

/**
 * _collide
 *
 * Attempts to collide two collision geometries. Returns true if they
 * collided and the given collision contact was updated. Otherwise,
 * returns false and does not modify the given contact.
 * By principle, the normal always points in direction of g2.
 */
bool SimpleCollisionSpace::_collide(
    iCollisionGeometry* g1,
    iCollisionGeometry* g2, 
    CollisionContact* contact
) const{
    bool res = false;

    if (g1 && g2 && contact && g1->getPhysics() && g2->getPhysics()) {
        // Collide them based on type
        switch (g1->getCollisionType()) {
        case CT::OBB:
            switch (g2->getCollisionType()) {
            case CT::OBB:
                res = CBoxBox(g1, g2, contact);
                break;
            case CT::SPHERE:
                res = CSphereBox(g2, g1, contact);
                contact->normal *= -1;
                iCollisionGeometry* temp = contact->g1;
                contact->g1 = contact->g2;
                contact->g2 = temp;
                break;
            }
            break;
        case CT::SPHERE:
            switch (g2->getCollisionType()) {
            case CT::OBB:
                res = CSphereBox(g1, g2, contact);
                break;
            case CT::SPHERE:
                res = CSphereSphere(g1, g2, contact);
                break;
            }
            break;
        }
    }
    
    return res;
}

/**
 * populateContactList
 *
 * Performs internal collision checks on all collision geometries and
 * populates the internal contact list. Very slow, O(N^2) runtime.
 * Requires a time delta since last populating of contact list.
 */
void SimpleCollisionSpace::populateContactList(float delta){
#define GeomIterator std::list<iCollisionGeometry*>::iterator

    _contactList.clear();

    for (GeomIterator i = _geomList.begin(); i != _geomList.end(); i++) {
        GeomIterator j = i;
        j++;
        for (; j != _geomList.end(); j++) {

            // Ensure that we have ready geoms
            bool processGeoms = (*i)->getPhysics() && (*j)->getPhysics() &&

            // Don't process geoms that are fixed in space
            !((*i)->getPhysics()->physicsType() == PHYS_FixedInSpace 
            && (*j)->getPhysics()->physicsType() == PHYS_FixedInSpace);

            if (processGeoms) {
                CollisionContact contact;
                if (_collide(*i, *j, &contact)) {
                    _contactList.push_back(contact);
                }
            }
        }
    }
#undef GeomIterator
}

/**
 * getContactList
 *
 * Returns a pointer to the idx'th structure in the contact list.
 * Contact normals face toward g2.
 */
const CollisionContact* SimpleCollisionSpace::getContactList(int idx) const{
    if (_geomList.size() && (idx < _contactList.size())) {
        return &(_contactList[idx]);
    } else {
        return NULL;
    }
}

/**
 * getNumContacts
 *
 * Returns the number of contacts in the contat list.
 */
size_t SimpleCollisionSpace::getNumContacts() const{
    return _contactList.size();
}

/**
 * Suspends all known collision geometries.
 */
void SimpleCollisionSpace::suspend() const{
    std::list<iCollisionGeometry*>::const_iterator it;
    for (it = _geomList.begin(); it != _geomList.end(); it++) {
        if (*it) {
            (*it)->suspend();
        }
    }
}

/**
 * Releases this collision space, releases all known collision geometries.
 */
void SimpleCollisionSpace::release() const{
    std::list<iCollisionGeometry*>::const_iterator it;
    for (it = _geomList.begin(); it != _geomList.end(); it++) {
        if (*it) (*it)->release();
    }
}

/**
 * Deletes this collision space, deletes all known collision geometries.
 */
void SimpleCollisionSpace::Delete() const{
    std::list<iCollisionGeometry*> tempList = _geomList;
    std::list<iCollisionGeometry*>::const_iterator it;
    for (it = tempList.begin(); it != tempList.end(); it++) {
        (*it)->Delete();
    }

    delete this;
}
