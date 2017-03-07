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
    void update(const ofCamera &cam, const ofVec2f &force);
    void draw();
    ofxAssimpModelLoader *model;
    
    ofVec3f start,end,vel;
    ofVec3f getPosition();
    float _div,elapseTime,elapseTimeDiff;
//    ofxParticleEmitter		m_emitter;
    ofNode geometry,future;
    
    
    void set(float x, float y, float z);
    void resetPosition();
};
#endif /* Butterfly_hpp */
