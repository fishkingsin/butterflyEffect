//
//  Butterfly.hpp
//  butterflyEffect
//
//  Created by James Kong on 3/3/2017.
//
//

#ifndef Butterfly_hpp
#define Butterfly_hpp
#include "ofxAssimpModelLoader.h"
#include <stdio.h>
class Butterfly {
public:
    ~Butterfly();
    void setup();
    void update(const ofCamera &cam, const ofVec2f &force,const ofVec3f &attractPoint, bool bfollow);
    void draw();
    ofxAssimpModelLoader *model;
    
    ofVec3f start,end,vel;
    ofVec3f acc;
    ofVec3f getPosition();
    float _div,elapseTime,elapseTimeDiff,noiseStrength,radius,velDecay,targetScale,delayEmmit;
    int age;
//    ofxParticleEmitter		m_emitter;
    ofNode geometry,future;
    
    
    void set(float x, float y, float z);
    void resetPosition(float delay = 3, float x = 0 , float y = 0);
    
    
};
#endif /* Butterfly_hpp */
