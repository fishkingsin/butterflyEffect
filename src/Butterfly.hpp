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
class Butterfly{
public:
    ~Butterfly();
    void setup();
    void update();
    void draw();
    ofxAssimpModelLoader *model;
    ofVec3f start,end;
};
#endif /* Butterfly_hpp */
