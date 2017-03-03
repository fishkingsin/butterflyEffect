#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(0, 0, 0);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    
    // this uses depth information for occlusion
    // rather than always drawing things on top of each other
    glEnable(GL_DEPTH_TEST);
    
    
    cam.setDistance(30);
    
    flocking.setup();
    VehicleGroup group = flocking.getVehicles();
    
    billboards.getVertices().resize(NUM_BILLBOARDS);
    billboards.getColors().resize(NUM_BILLBOARDS);
    billboards.getNormals().resize(NUM_BILLBOARDS,ofVec3f(0));
    
    // ------------------------- billboard particles
    for (int i=0; i<NUM_BILLBOARDS; i++) {
        
        billboardVels[i].set(ofRandomf(), -1.0, ofRandomf());
        billboards.getVertices()[i].set(ofRandom(-500, 500),
                                        ofRandom(-500, 500),
                                        ofRandom(-500, 500));
        
        billboards.getColors()[i].set(ofColor::fromHsb(ofRandom(96, 160), ofRandom(96, 160), 255));
        billboardSizeTarget[i] = ofRandom(1, 2);
         age[i] = ofRandom(50, 100) ;
        
    }
    
    
    billboards.setUsage( GL_DYNAMIC_DRAW );
    billboards.setMode(OF_PRIMITIVE_POINTS);
//    billboards.setVertexData(billboardVels, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
//    billboards.setColorData(billboardColor, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
    
    // load the billboard shader
    // this is used to change the
    // size of the particle
    if(ofIsGLProgrammableRenderer()){
        billboardShader.load("shadersGL3/Billboard");
    }else{
        billboardShader.load("shadersGL2/Billboard");
    }
    
    // we need to disable ARB textures in order to use normalized texcoords
    ofDisableArbTex();
    texture.load("dot.png");
    ofEnableAlphaBlending();
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    float t = (ofGetElapsedTimef()) * 0.9f;
    float div = 250.0;
    
    for (int i=0; i<NUM_BILLBOARDS; i++) {
        billboards.getVertices()[i] += billboardVels[i];
        billboardVels[i] *= 0.94f;
        billboards.setNormal(i,ofVec3f((age[i]/100.0f)*billboardSizeTarget[i],0,0));
        age[i] *= 0.97;
    }
    
    flocking.update(ofGetElapsedTimef(), 0.06);
    VehicleGroup group = flocking.getVehicles();
    for (vector<ofxOpenSteerVehicle*>::iterator i = group.begin();
         i < group.end();
         i++)
    {
        PathBoid *b = (PathBoid*)*i;
        if(b!=NULL && b->pt != NULL){
            age[current] = ofRandom(80, 100) ;
            billboards.getVertices()[current] = b->start;
            billboardVels[current] = ofVec3f (ofRandom(-10,10),
                                              ofRandom(-10,10),
                                              ofRandom(-10,10))*0.05 + ofVec3f (ofSignedNoise(t, billboards.getVertex(current).y/div, billboards.getVertex(current).z/div),
                                                                                   ofSignedNoise(billboards.getVertex(current).x/div, t, billboards.getVertex(current).z/div),
                                                                                   ofSignedNoise(billboards.getVertex(current).x/div, billboards.getVertex(current).y/div, t))*0.01;
            billboards.getColors()[current].set(ofColor::fromHsb(ofRandom(96, 160), ofRandom(96, 160), ofRandom(160, 255)));
            current++;
            current%=NUM_BILLBOARDS;
        }
        
    }
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    cam.begin();

    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofPushMatrix();
    glDepthMask(false);
    // bind the shader so that wee can change the
    // size of the points via the vert shader
    billboardShader.begin();
    
    ofEnablePointSprites(); // not needed for GL3/4
    texture.getTexture().bind();
    billboards.draw();
    texture.getTexture().unbind();
    ofDisablePointSprites(); // not needed for GL3/4
    
    billboardShader.end();
    
    ofPopMatrix();
    ofSetColor(255);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
//    ofEnableDepthTest();
    flocking.draw();
//    assimp.drawFaces();
    
    

    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
            case '1':
            flocking.setFlocking();
            break;
            case '2':
            flocking.setPathFollowing();
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}




