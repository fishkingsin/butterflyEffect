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
    

    
}
void Butterfly::update(){
    
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
