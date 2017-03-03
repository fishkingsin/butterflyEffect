//
//  Flocking.cpp
//  butterflyEffect
//
//  Created by James Kong on 2/3/2017.
//
//

#include "Flocking.h"
#define MODEL_SCALE 0.005
Flocking::Flocking(){
    pd = NULL;
};

string Flocking::name(){ return "Flocking"; };

void Flocking::draw(){
    ofxOpenSteerPlugin::draw();
    ofPushStyle();
    ofSetColor(255);
    ofSetLineWidth(3);
    circle.draw();
    ofPopStyle();
}
void Flocking::setup(){
    ofxOpenSteerPlugin::setup();
    //        ofBackground(255, 255, 0);
    
    // Create a proximity database with default settings
    pd = createProximityDatabase();
    
    for(unsigned int i=0;i<50;i++){
        PathBoid* v = new PathBoid();
        ofxAssimpModelLoader *model = new ofxAssimpModelLoader();
        float r = ofRandom(1,2);
        if(r>1){
            model->loadModel("butterfly.dae");
        }else{
            model->loadModel("butterfly2.dae");
        }
        model->setLoopStateForAllAnimations(OF_LOOP_NORMAL);
        model->playAllAnimations();
        model->setPositionForAllAnimations(ofRandomf());
        model->setScale(MODEL_SCALE,MODEL_SCALE, MODEL_SCALE);
        v->model = model;
        v->pt = allocateProximityToken(pd, v);
        v->reset();
        addVehicle(v);
        
    }
};

void Flocking::exit(){
    ofxOpenSteerPlugin::exit();
    if(pd) delete pd;
    pd = NULL;
};
void Flocking::setFlocking(){
    VehicleGroup group = getVehicles();
    pd = createProximityDatabase();
    for (vector<ofxOpenSteerVehicle*>::iterator i = group.begin();
         i < group.end();
         i++)
    {
        PathBoid *b = (PathBoid *)*i;
        b->path = NULL;
        b->pt = allocateProximityToken(pd, b);
        b->reset();
    }

}
void Flocking::setPathFollowing(){
    pd = createProximityDatabase();
    
    // Create a path
    circle.clear();
    circle.setFilled(false);
    circle.arc(ofPoint(), 20, 20, 0, 360);
    circle.close();
    
    path = createPath(circle.getOutline()[0], 1.5); // polyline, radius
    VehicleGroup group = getVehicles();
    for (vector<ofxOpenSteerVehicle*>::iterator i = group.begin();
         i < group.end();
         i++)
    {
        PathBoid *b = (PathBoid *)*i;
        b->path = path;
        b->pt = allocateProximityToken(pd, b);;

    }
    
}
