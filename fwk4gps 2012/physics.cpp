#include "Physics.h"
#include "iGraphic.h"
#include "MathDefinitions.h"
#include "CollisionGeometry.h"
#include "COBB.h"
#include "CSphere.h"
Physics::Physics(float mass, PhysicsType pt, iObject* bv){
	mass_=mass>MIN_MASS?mass:MIN_MASS;
	bv_=bv;
	parent_=0;
	momentInertia_=Matrix(mass);   //assuming point
	invMomentInertia_=Matrix(1/mass);
	physicsType_=pt;
}
void Physics::attachTo(iPhysics* newParent, float speed, float angularSpeed) {

	// detach from current parent, if any
    if (parent_){
		bodyForce_=mass_*9.8f*Vector(0,-1,0);
		velocity_=velocity()          //initial velocity of detached frame is made of its current velocity
					+ tanVelocity()	  //velocity from parent's spin
					+ speed * normal(bv_->relativePosition() * parent_->rotation());  //launch velocity
		acceleration_=tanAcceleration();  //tangential acceleration from the parent's rotation
		//angVel_ = angularSpeed*Vector(0,1,0);
		//angAccel_= Vector(0,-1,0);
	}
	parent_=0;
	parent_=newParent;
	if(newParent){
 		bv_->attachTo(newParent->bv());
	}
	else{
		bv_->attachTo(0);
		physicsType_=PHYS_Falling;
	}
}
// angAcceleration returns the angular acceleration of the frame in world
// space.
Vector Physics::angularAcceleration() const{
	return parent_ ? angAccel_+ parent_->angularAcceleration() : angAccel_;
}

// tanVelelocityReference returns the tangential velocity component arising from
// the rotation of the frame's parent about the origin of world space.
// This component is used by tanVelocity() in determining the tangential
// velocity of the frame itself.
Vector Physics::tanVelocityReference() const{
	return parent_ ? (cross(angVel_,  parent_->position()) + parent_->tanVelocityReference()) : Vector();
}
// tanVelocity returns the tangential velocity of the frame in world space.
Vector Physics::tanVelocity() const{
	return cross(angularVelocity(),position())-tanVelocityReference();
}
// tanAccRef returns the tangential acceleration component arising from
// the rotation of the frame's parent about the origin of world space.
// This component is used by tanAcceleration() in determining the tangential
// acceleration of the frame itself.
Vector Physics::tanAccelerationReference() const{
	return parent_ ? cross(angAccel_ ,parent_->position()) + parent_->tanAccelerationReference() : Vector();
}

// tanAcceleration returns the tangential acceleration of the frame in
// world space.
Vector Physics::tanAcceleration() const{
	return cross(angularAcceleration(),position())-tanAccelerationReference();
}

Vector Physics::worldCentreOfMass() const{
	return position()+(centreOfMass_* bv_->rotation());
}
Vector Physics::velocityAtPoint(const Vector& p)const{
	return velocity() + cross(angularVelocity(), p-worldCentreOfMass());
}
void Physics::updateKinematics(float delta){
	Vector displacement=delta*velocity_+0.5f*delta*delta*acceleration_;
	bv_->translate(displacement.x,displacement.y,displacement.z);
	velocity_+=(delta*acceleration());

	Matrix omega(	0,		angVel_.z,	-angVel_.y, 0,
				-angVel_.z,		0,		angVel_.x, 0,
				angVel_.y,	-angVel_.x,		0,		0,
				0,0,0,0);                                         //set up the matrix for calculating rotation based on
	                                                        //the objects angular velocity

	Matrix Rcurr= bv_->rotation();								//Get the current rotation of the object

	Matrix Rdelta=delta*(omega*Rcurr);							//Calculate the change in rotation of the object

	Matrix Rnew=orthoNormalize(Rcurr+Rdelta);				//calculate the new rotation matrix

	Matrix Rnet=orthoNormalize(Rcurr.transpose()*Rnew);		//figure out what the net rotation is

	angVel_=angVel_+delta*angAccel_;							//update the angular velocity

	bv_->rotate(Rnet);


}
void Physics::update(int delta){
	float dt=delta/1000.0f;
//	float mindt = 0.005f;
//	float leftover=(dt>mindt)?dt-mindt:0;
	//calculate the amount of force being applied to object for a split second
	Vector appliedForce(impulseForce_+bodyForce_);
	acceleration_=appliedForce/mass_;
	//reduce the impulse force to 0

	// impulse moments
	Matrix R  = rotation();
	Matrix RT = R.transpose();
	Vector ml = moment_ * RT;
	Vector wl = angularVelocity() * RT;
	setAngularAcceleration((ml - cross(wl*momentInertia_ ,wl)) * invMomentInertia_ * R);
	updateKinematics(dt);
	moment_    = Vector();
	impulseForce_=Vector();   
	//setAngularAcceleration(Vector());
	//calculate a new acceleration for the rest of the frame based soley on the body force
	//acceleration_=bodyForce_/mass_;
}
void Physics::setMomentInertia(const Vector& mi){
	momentInertia_=Matrix(mi);
	invMomentInertia_=Matrix(Vector(RECIP(mi.x),RECIP(mi.y),RECIP(mi.z)));
}


iPhysics* Physics::clone(){
	Physics* rc= new Physics(mass_,physicsType_, (iObject*)bv_->clone());
	rc->parent_=parent_;
	rc->velocity_=velocity_;       
	rc->acceleration_=acceleration_;   
	rc->angVel_=angVel_;
	rc->angAccel_=angAccel_;
	rc->bodyForce_=bodyForce_;     
	rc->impulseForce_=impulseForce_;  
	rc->moment_=moment_;			
	rc->momentInertia_=momentInertia_;	
	rc->invMomentInertia_=invMomentInertia_; 
	return rc;
}

Physics::~Physics(){
	delete bv_;
}

iPhysics* CreatePhysicsBox(float minx, float miny, float minz, 
	 float maxx, float maxy, float maxz, const Reflectivity* r, float mass,PhysicsType pt, bool hasCollision){

	iGraphic* box=CreateBox(minx,miny,minz*MODEL_Z_AXIS,maxx,maxy,maxz*MODEL_Z_AXIS);		 
  	Physics* rc= new Physics(mass,pt,CreateObject(box,r));
	
	float xsq=maxx-minx;
	float ysq=maxy-miny;
	float zsq=maxz-minz;
	float coef=(1.0f/12.0f)*mass;
    if(hasCollision)
	  rc->cg_=CreateCOBB(rc,Vector(xsq/2.0f,ysq/2.0f,zsq/2.0f));
	else
	  rc->cg_=0;
	xsq*=xsq;
	ysq*=ysq;
	zsq*=zsq;
	rc->setMomentInertia(Vector(coef*(ysq+zsq),coef*(xsq+zsq),coef*(xsq+ysq)));
	return rc;
}
