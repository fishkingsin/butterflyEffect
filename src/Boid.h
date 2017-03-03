// Base boid used by all the simulations with flocking behaviour

#pragma once

#include "ofxOpenSteer.h"
#include "ofxAssimpModelLoader.h"

using namespace OpenSteer;
using namespace ofxOpenSteer;

class Boid: public ofxOpenSteerVehicle{
	
public:
    ofxAssimpModelLoader *model;
    ProximityToken* pt;
    ofVec3f start,end;
    Boid(){
        pt = NULL;
        model = NULL;
    };
    
    ~Boid(){
        if(pt) delete pt;
        pt = NULL;
        if(model != NULL){
            model->clear();
            delete model;
            model = NULL;
        }
    };    
    
    void reset(){
        start.set(0,0,0);
        end = start;

		// reset the vehicle
		ofxOpenSteerVehicle::reset ();
		
		// steering force is clipped to this magnitude
		setMaxForce (60);
		
		// velocity is clipped to this magnitude
		setMaxSpeed (9);
		
		// initial slow speed
		setSpeed (maxSpeed() * 0.3f);
		
		// randomize initial orientation
		regenerateOrthonormalBasisUF (RandomUnitVector ());
		
		// randomize initial position
		setPosition (RandomVectorInUnitRadiusSphere() * 20);
		
		// notify proximity database that our position has changed
		if(pt) pt->updateForNewPosition (position());
	};
    
    void update(){
        ofxOpenSteerVehicle::update();
        ofVec3f vec = getPosition();
        int bounce = 50;
        if(getPosition().x > bounce ){
            vec.x = -bounce;
        }else if(getPosition().x < -bounce ){
            vec.x = bounce;
        }else if(getPosition().y > bounce ){
            vec.y = -bounce;
        }else if(getPosition().y < -bounce ){
            vec.y = bounce;
        }else if(getPosition().z > bounce ){
            vec.z = -bounce;
        }else if(getPosition().z < -bounce ){
            vec.z = bounce;
        }
        setPosition(vec.x,vec.y,vec.z);
        if(pt) pt->updateForNewPosition (position());
    };
    void draw(){
        end = start;
        start = getPosition();
        if(model == NULL){
            
            ofPushStyle();
            ofxOpenSteerVehicle::draw();
            ofPopStyle();
        }
        
        if(model != NULL){
            
            model->update();
            
            ofPushStyle();
            
            ofNode node;
            node.setPosition(end);
            node.lookAt(start);
            ofPushMatrix();
            ofMultMatrix(node.getLocalTransformMatrix());
            model->drawFaces();
            ofPopMatrix();
            

            ofPopStyle();
        }
        
    }
    Vec3 getSteeringForce(const float elapsedTime){
        // if there is no proximity database, just wander
        if(!pt) return steerForWander(elapsedTime);
        
		const float separationRadius =  5.0f;
		const float separationAngle  = -0.707f;
		const float separationWeight =  12.0f;
		
		const float alignmentRadius = 7.5f;
		const float alignmentAngle  = 0.7f;
		const float alignmentWeight = 8.0f;
		
		const float cohesionRadius = 50.0f;
		const float cohesionAngle  = -0.15f;
		const float cohesionWeight = 8.0f;
		
		const float maxRadius = maxXXX (separationRadius,
										maxXXX (alignmentRadius,
												cohesionRadius));
		
		// find all flockmates within maxRadius using proximity database
		neighbors.clear();
		pt->findNeighbors (position(), maxRadius, neighbors);
		
		
		// determine each of the three component behaviors of flocking
		const Vec3 separation = steerForSeparation (separationRadius,
													separationAngle,
													neighbors);
		const Vec3 alignment  = steerForAlignment  (alignmentRadius,
													alignmentAngle,
													neighbors);
		const Vec3 cohesion   = steerForCohesion   (cohesionRadius,
													cohesionAngle,
													neighbors);
		
		// apply weights to components (save in variables for annotation)
		const Vec3 separationW = separation * separationWeight;
		const Vec3 alignmentW = alignment * alignmentWeight;
		const Vec3 cohesionW = cohesion * cohesionWeight;
		
		return separationW + alignmentW + cohesionW;	
	};
    
};
