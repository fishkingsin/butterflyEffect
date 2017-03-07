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
    velDecay = 0.94;
    
}
void Butterfly::update(const ofCamera &cam, const ofVec2f &force , const ofVec3f &attractPoint , bool bfollow){
    if(delayEmmit<ofGetElapsedTimef()){
        age++;
        float t = (elapseTime) * 0.9f;
        elapseTime+=elapseTimeDiff;
        float div = _div;
        
        ofVec3f vec = ofVec3f (ofSignedNoise(t,geometry.getY()/div, geometry.getZ()/div),
                               ofSignedNoise(geometry.getX()/div, t, geometry.getZ()/div),
                               0)*noiseStrength+(force);
        vec *= acc * ofGetLastFrameTime();
        vec.z = 0;
        ofVec3f attract;
        if(bfollow){
            attract = (attractPoint-getPosition())*0.001;
            attract.z = getPosition().z;
        }
        vel+=vec+attract;
        
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
        ofVec3f min = cam.screenToWorld(ofVec3f(1,1,1));
        ofVec3f max = cam.screenToWorld(ofVec3f(ofGetWidth(), ofGetHeight(),1));
        
        
        float mergin = 100;
        if(screenPos.x > ofGetWidth()+mergin ){
            resetPosition(0.1);
        }else if(screenPos.x < -mergin ){
            resetPosition(0.1);
            
        }else if(screenPos.y > ofGetHeight()+mergin ){
            resetPosition(0.1);
        }else if(screenPos.y <  -mergin ){
            resetPosition(0.1);
        }
        
        
        vel *= velDecay;
        if(model != NULL){
            model->update();
        }
    }
    
}
void Butterfly::draw(){
    if(age>0){
        if(model != NULL){
            
            
            
            ofPushStyle();
            ofSetColor(255);
            
            
            ofPushMatrix();
            
            ofMultMatrix(geometry.getLocalTransformMatrix());
            
            model->drawFaces();
            ofPopMatrix();
            
            
            ofPopStyle();
            
        }
    }
}

ofVec3f Butterfly::getPosition(){
    return geometry.getPosition();
}

void Butterfly::set(float x, float y, float z){
    geometry.setPosition(x, y, z);
    future.setPosition(x, y, z);
}

void Butterfly::resetPosition(float delay , float x, float y){
    delayEmmit = ofGetElapsedTimef() + ofRandom(delay);
    if(delay>0){
        float rad = ofRandom(-PI,PI);
        float _x = sin(rad)*radius;
        float _y = cos(rad)*radius;
        set(_x,_y,
            0);
        model->setScale(targetScale,targetScale,targetScale);
        vel = ofVec3f(_x,_y,0)*0.1;
    }
    else{
        set(x,y,
            0);
    }
    
    age = 0;
}
