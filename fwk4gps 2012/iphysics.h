#ifndef IPHYSICS_H
#define IPHYSICS_H
#include "MathDeclarations.h"
#include "Frame.h"
#include "Object.h"

class iPhysics{

public:
/*the following have the same functionality as they do in the iFrame class*/
    virtual void   scale(float x, float y, float z)					= 0;
    virtual void   rotatex(float rad)								= 0;
    virtual void   rotatey(float rad)								= 0;
    virtual void   rotatez(float rad)								= 0;
	virtual void rotate(const Vector& axis, float rad)				= 0;
	virtual void translate(float x, float y, float z)				= 0;
	virtual Vector position() const									= 0;
	virtual Matrix rotation() const									= 0;
    virtual Vector orientation(char axis) const						= 0;
	virtual Matrix world() const									= 0;

	
	virtual void rotate(const Matrix& M)							= 0;

	virtual void attachTo(iPhysics* newParent, float speed, float angularSpeed)	= 0;
	virtual void update(int delta)									= 0;
	virtual iObject* bv() const										= 0;
	virtual iPhysics* clone()										= 0;

	virtual ~iPhysics(){}

	//Linear Motion
	virtual void	setVelocity(Vector v)							= 0;
	virtual void	setAcceleration(Vector v)						= 0;
	virtual Vector velocity() const									= 0;
	virtual Vector acceleration() const								= 0;

	virtual void	setAngularVelocity(Vector v)							= 0;
	virtual void	setAngularAcceleration(Vector v)						= 0;
	virtual Vector angularVelocity() const							= 0;
	virtual Vector angularAcceleration() const						= 0;


};
extern "C"
	iPhysics* CreatePhysicsBox(float minx, float miny, float minz, 
	 float maxx, float maxy, float maxz,bool hasCollision=false);

#endif