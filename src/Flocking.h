//
//  Flocking.hpp
//  butterflyEffect
//
//  Created by James Kong on 2/3/2017.
//
//

#ifndef Flocking_h
#define Flocking_h

#include "ofxOpenSteer.h"
#include "Boid.h"
#include "ofxAssimpModelLoader.h"

using namespace OpenSteer;
using namespace ofxOpenSteer;
// Extend the flocking boid adding path following behaviour
class PathBoid: public Boid {
public:
    
    SteerPath* path;
    
    PathBoid(){
        path = NULL;
    };
    
    ~PathBoid(){
        path = NULL;
    };
    
    void reset(){
        // reset the vehicle
        ofVec3f prevPosition = getPosition();
        Boid::reset ();
        
        
        // randomize initial position
//        setPosition (RandomVectorInUnitRadiusSphere() * 100);
        
        // notify proximity database that our position has changed
//        setPosition(toOpenSteer(prevPosition));
        if(pt) pt->updateForNewPosition (toOpenSteer(prevPosition));
    };
    
    Vec3 getSteeringForce(const float elapsedTime){
        // Inherit the flocking force
        Vec3 flock = Boid::getSteeringForce(elapsedTime);
        
        // If there is no path, just flock
        if(!path) return flock;
        
        // Get path following forces
        Vec3 followPath = steerToFollowPath (0.6, .8f, *path);
        Vec3 stayOnPath = steerToStayOnPath(0.8f, *path);
        
        return followPath + stayOnPath + flock * 0.5; // reduce flocking force
    }
    
};

class Flocking: public ofxOpenSteerPlugin {
    
public:
    ofPath circle;
    SteerPath* path;
    ProximityDatabase* pd;
    
    Flocking();
    void draw();
    string name();
    
    void setup();
    
    void exit();
    void setFlocking();
    void setPathFollowing();
};

#endif /* Flocking_h */
