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
class Butterfly : public ofVec3f{
public:
    ~Butterfly();
    void setup();
    void update(ofCamera *cam);
    void draw();
    ofxAssimpModelLoader *model;
    ofVec3f start,end,vel;
    ofVec3f getPosition();
    float _div,elapseTime,elapseTimeDiff;
    
};
#endif /* Butterfly_hpp */
