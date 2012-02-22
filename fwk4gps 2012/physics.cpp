#include "Physics.h"
#include "MathDefinitions.h"
#include "Common_Symbols.h"
#include "iGraphic.h"
#include "iTexture.h"

Physics::Physics( iObject* bv){

	bv_=bv;       
	parent_=0;

}
void Physics::attachTo(iPhysics* newParent, float speed, float angularSpeed) {

	// detach from current parent, if any
    if (parent_){
		velocity_=velocity();          //initial velocity of detached frame is made of its current velocity
		angVel_=angularVelocity();
	}
	parent_=0;
	parent_=newParent;
	if(newParent){
 		bv_->attachTo(newParent->bv());
	}
	else{
		bv_->attachTo(0);
	}
}

void Physics::update(int delta){
	float dt=delta/1000.0f;       //all calculations in physics are done /sec but time
	                              //is in millisec so convert.  THIS IS REALLY IMPORTANT

	Vector displacement=dt*velocity_+0.5f*dt*dt*acceleration_;
	bv_->translate(displacement.x,displacement.y,displacement.z);
	velocity_+=(dt*acceleration());

	Matrix omega(	0,		angVel_.z,	-angVel_.y, 0,
				-angVel_.z,		0,		angVel_.x, 0,
				angVel_.y,	-angVel_.x,		0,		0,
				0,0,0,0); 

    Matrix Rcurr= bv_->rotation();								//Get the current rotation of the object

	Matrix Rdelta=dt*(omega*Rcurr);							//Calculate the change in rotation of the object

	Matrix Rnew=orthoNormalize(Rcurr+Rdelta);				//calculate the new rotation matrix

	Matrix Rnet=orthoNormalize(Rcurr.transpose()*Rnew);		//figure out what the net rotation is

	angVel_=angVel_+dt*angAccel_;							//update the angular velocity

	bv_->rotate(Rnet);

}

// angAcceleration returns the angular acceleration of the frame in world
// space.
Vector Physics::angularAcceleration() const{
	return parent_ ? angAccel_+ parent_->angularAcceleration() : angAccel_;
}

iPhysics* Physics::clone(){
	Physics* rc= new Physics(Clone(bv_));
	rc->parent_=parent_;
	rc->velocity_=velocity_;       
	rc->acceleration_=acceleration_;   

	return rc;
}

Physics::~Physics(){
	delete bv_;
}
iPhysics* CreatePhysicsBox(float minx, float miny, float minz, 
	 float maxx, float maxy, float maxz, bool hasCollision){
    iTexture* checkdsy = CreateTexture(L"daisy.bmp", TEX_MIN_ANISOTROPIC | \
                         TEX_MAG_ANISOTROPIC);
	iGraphic* box=CreateBox(minx,miny,minz,maxx,maxy,maxz);
	iObject* obj=CreateObject(box);
	obj->attach(checkdsy);
	Physics* rc=new Physics(obj);	 
	return rc;
}
