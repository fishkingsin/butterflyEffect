//
//  Butterfly.cpp
//  butterflyEffect
//
//  Created by James Kong on 3/3/2017.
//
//

#include "Butterfly.hpp"
Butterfly::~Butterfly(){
    
    if(model != NULL){
        model->clear();
        delete model;
        model = NULL;
    }
}
void Butterfly::setup(){
    _div = ofRandom(200,250);
    elapseTimeDiff = ofRandomf()*0.001;
    
}
void Butterfly::update(ofCamera *cam){
    float t = (elapseTime) * 0.9f;
    elapseTime+=elapseTimeDiff;
    float div = _div;
    
    end = start;
    start = getPosition();
    ofVec3f vec = ofVec3f (ofSignedNoise(t,y/div, z/div),
                           ofSignedNoise(x/div, t, z/div),
                           ofSignedNoise(x/div,y/div, t))*0.1;
    
    vec *= 10 * ofGetLastFrameTime();
    vec.z = 0;
    vel+=vec;
    
    ofVec3f screenPos = cam->worldToScreen(*this);
    ofVec3f min = cam->screenToWorld(ofPoint(0,0));
    ofVec3f max = cam->screenToWorld(ofPoint(ofGetWidth(),ofGetHeight()));
    float mergin = 100;
    if(screenPos.x > ofGetWidth()+mergin ){
        screenPos.x = -mergin*0.5;
        
    }else if(screenPos.x < -mergin ){
        screenPos.x = ofGetWidth()+mergin*0.5;
        
    }else if(screenPos.y > ofGetHeight()+mergin ){
        screenPos.y = -mergin*0.5;
        
    }else if(screenPos.y <  -mergin ){
        screenPos.y = ofGetHeight()+mergin*0.5;
        
    }
    set(cam->screenToWorld(screenPos));
    *this+=vel;
    vel *= 0.94f;
    
    
}
void Butterfly::draw(){
    if(model != NULL){
        end = start;
        start = getPosition();
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

ofVec3f Butterfly::getPosition(){
    return *this;
}
