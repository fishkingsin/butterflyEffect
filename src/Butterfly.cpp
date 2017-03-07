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
    model->setRotation(0, 90, 1, 0, 0);
    
}
void Butterfly::update(const ofCamera &cam, const ofVec2f &force){
    float t = (elapseTime) * 0.9f;
    elapseTime+=elapseTimeDiff;
    float div = _div;

    ofVec3f vec = ofVec3f (ofSignedNoise(t,geometry.getY()/div, geometry.getZ()/div),
                           ofSignedNoise(geometry.getX()/div, t, geometry.getZ()/div),
                           0)+(force);
    
    vec *= 10 * ofGetLastFrameTime();
    vec.z = 0;
    vel+=vec;
    
    future.move(vel);
    ofVec3f normal = geometry.getPosition() - future.getPosition();
    normal.normalize();
    
    float rotationAmount;
    ofVec3f rotationAngle;
    ofQuaternion rotation;
    
    ofVec3f axis(0, 1, 0);
    rotation.makeRotate(axis, normal);
    
    rotation.getRotate(rotationAmount, rotationAngle);
    

    geometry = future;
    geometry.setOrientation(rotation);

    
    ofVec3f screenPos = cam.worldToScreen(getPosition(),ofRectangle(0,0,ofGetWidth(), ofGetHeight()));
    
    
        float mergin = 100;
        if(screenPos.x > ofGetWidth()+mergin ){
            resetPosition();
        }else if(screenPos.x < -mergin ){
            resetPosition();

        }else if(screenPos.y > ofGetHeight()+mergin ){
            resetPosition();
        }else if(screenPos.y <  -mergin ){
            resetPosition();
        }
        
    
    vel *= 0.94f;
    if(model != NULL){
        model->update();
    }
    
}
void Butterfly::draw(){
    if(model != NULL){
        
        
        
        ofPushStyle();
        ofSetColor(255);

        
        ofPushMatrix();
        
//        geometry.rotate(degree, ofVec3f(0,0,1));
        ofMultMatrix(geometry.getLocalTransformMatrix());
        
        model->drawFaces();
        ofPopMatrix();
        
        
        ofPopStyle();
        end = start;
    }
}

ofVec3f Butterfly::getPosition(){
    return geometry.getPosition();
}

void Butterfly::set(float x, float y, float z){
    geometry.setPosition(x, y, z);
    future.setPosition(x, y, z);
}

void Butterfly::resetPosition(){
    
    set(ofRandom(ofGetWidth())-ofGetWidth()*0.5,
        ofRandom(ofGetHeight())-ofGetHeight()*0.5,
        0);
    vel.set(ofRandom(-10,10),ofRandom(-10,10),0);
}
