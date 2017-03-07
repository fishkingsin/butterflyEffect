#include "ofApp.h"
#define MODEL_SCALE 0.1
//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofSetSmoothLighting(true);
    ofEnableArbTex();
    ofEnableSmoothing();
    shadertoy.load("shaders/Glow.frag");
//    ofEnableSeparateSpecularLight();
    
    cam.setVFlip(true);
    gui.setup();
    gui.add(lightPos.set("lightPos", ofVec3f::zero(),ofVec3f(-ofGetWidth(), -ofGetHeight() , 0),ofVec3f(ofGetWidth(), ofGetHeight() , 2000 )));
    gui.add(emmit.setup("emmitButterfly"));
    emmit.addListener(this, &ofApp::emmitButterfly);
    particleParaGroup.setName("Particle");
    
    particleParaGroup.add(noiseMode.set("noiseMode", 0,0,1));
    particleParaGroup.add(velx.set("velx", 0,0,50));
    particleParaGroup.add(vely.set("vely", 0,0,50));
    particleParaGroup.add(velz.set("velz", 0,0,50));
    particleParaGroup.add(velstrength.set("velstrength", 0,0,1));
    particleParaGroup.add(noiseStrength.set("noiseStrength", 0,0,5));
    particleParaGroup.add(color.set("color", 0,0,255));
    particleParaGroup.add(satuation.set("satuation", 0,0,255));
    particleParaGroup.add(bright.set("brightness", 0,0,255));
    particleParaGroup.add(colorVari.set("colorVari", 0,0,255));
    particleParaGroup.add(satuationVari.set("satuationVari", 0,0,255));
    particleParaGroup.add(brightVari.set("brightnessVari", 0,0,255));
    particleParaGroup.add(velsDecay.set("velsDecay", 0,0,1));
    particleParaGroup.add(velsDecayVari.set("velsDecayVari", 0,0,0.5));
    
    particleParaGroup.add(sizeTarget.set("sizeTarget", 100,10,200));
    particleParaGroup.add(sizeTargetVari.set("sizeTargetVari", 0,0,50));
    
    particleParaGroup.add(ageTarget.set("ageTarget", 100,0,200));
    particleParaGroup.add(ageTargetVari.set("ageTargetVari", 0,0,50));
    particleParaGroup.add(ageDecay.set("ageDecay", 0,0,1));
    
    gui.add(particleParaGroup);
    
    butterflyParaGroup.setName("butterfly");
    butterflyParaGroup.add(bFollow.set("bFollow",false));
    butterflyParaGroup.add(mousePoint.set("attractPoint",ofVec3f(1,1,1),ofVec3f(-ofGetWidth(), -ofGetHeight() ,0),ofVec3f(ofGetWidth(), ofGetHeight() ,0)));
    butterflyParaGroup.add(accTarget.set("acc",ofVec3f(1,1,1),ofVec3f(0,0,0),ofVec3f(100,100,100)));
    butterflyParaGroup.add(force.set("force", ofVec2f::zero(),ofVec2f::zero(),ofVec2f(-10,10)));
    butterflyParaGroup.add(bNoiseStrength.set("bNoiseStrength",1,0,10));
    butterflyParaGroup.add(radiusTarget.set("radiusTarget",0,0,400));
    butterflyParaGroup.add(radiusTargetVari.set("radiusTargetVari",1,0,400));
    butterflyParaGroup.add(velDecay.set("velDecay",0.9,0,1));
    butterflyParaGroup.add(bSize.set("bSize",0.9,0,1));
    butterflyParaGroup.add(bSzieVair.set("bSzieVair",0.9,0,1));
    gui.add(butterflyParaGroup);
    gui.loadFromFile("settings.xml");
    
    for(int i = 0 ; i < 50 ; i++){
        Butterfly *butterfly = new Butterfly();
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
        float szieVari = ofRandom(-MODEL_SCALE*0.1,MODEL_SCALE*0.1);
        model->setScale(MODEL_SCALE + szieVari, MODEL_SCALE + szieVari, MODEL_SCALE + szieVari);
        butterfly->model = model;
        butterfly->setup();
        butterfly->targetScale = bSize + ofRandom(-bSzieVair,bSzieVair);
        butterfly->resetPosition();
        butterflies.push_back(butterfly);
    }
    
    
    
    
    //    flocking.setup();
    //    VehicleGroup group = flocking.getVehicles();
    
    billboards.getVertices().resize(NUM_BILLBOARDS);
    billboards.getColors().resize(NUM_BILLBOARDS);
    billboards.getNormals().resize(NUM_BILLBOARDS,ofVec3f(0));
    
    // ------------------------- billboard particles
    for (int i=0; i<NUM_BILLBOARDS; i++) {
        
        billboardVels[i].set(ofRandomf(), -1.0, ofRandomf());
        billboards.getVertices()[i].set(ofRandom(-500, 500),
                                        ofRandom(-500, 500),
                                        ofRandom(-500, 500));
        
        billboards.getColors()[i].set(ofColor::fromHsb(color+ofRandom(-colorVari,colorVari),
                                                       satuation+ofRandom(-satuationVari,satuationVari),
                                                       bright+ofRandom(-brightVari,brightVari)
                                                       ));
        billboardSizeTarget[i] = 100;
        age[i] = 0;
        billboardVelsDecay[i] = 0.94;
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
    texture.load("texture.png");
    ofEnableAlphaBlending();
    
    fbo.allocate(ofGetWidth(), ofGetHeight());
    light.setPosition(0,0,2000);
    
//    light2.setPosition(-ofGetWidth()*0.5, 0, 2000);
//    light3.setPosition(ofGetWidth()*0.5, 0, 2000);
//    light4.setPosition(0, ofGetHeight()*0.5, 2000);
}

void ofApp::emmitButterfly(){
    for (vector<Butterfly*>::iterator i = butterflies.begin();
         i < butterflies.end();
         i++){
        Butterfly * b = *i;
        b->resetPosition(1);
    }
    for (int i=0; i<NUM_BILLBOARDS; i++) {
        age[i] = 0;
    }
}
float minNoise = 0.499;
float maxNoise = 0.501;
float getRads(float val1, float val2, float mult, float div){
    float rads = ofNoise(val1/div, val2/div, ofGetElapsedTimef()/div);
    
    if (rads < minNoise) minNoise = rads;
    if (rads > maxNoise) maxNoise = rads;
    
    rads -= minNoise;
    rads *= 1.0/(maxNoise - minNoise);
    
    return rads * mult;
}
//--------------------------------------------------------------
void ofApp::update(){
    float t = (ofGetElapsedTimef()) * 0.9f;
    float div = 250.0;
    
    for (int i=0; i<NUM_BILLBOARDS; i++) {
        ofVec3f vec;
        if(noiseMode == 0){
        vec = ofVec3f (ofSignedNoise(t, billboards.getVertex(i).y, billboards.getVertex(i).z),
                   ofSignedNoise(billboards.getVertex(i).x, t, billboards.getVertex(i).z),
                   ofSignedNoise(billboards.getVertex(i).x, billboards.getVertex(i).y, t))*noiseStrength;
        }else{
            float xyRads      = getRads( billboards.getVertex(i).x, billboards.getVertex(i).z, 20.0, 50.0 );
            float yRads       = getRads( billboards.getVertex(i).x, billboards.getVertex(i).y, 20.0, 50.0 );

            vec = ofVec3f (cos(xyRads), -sin(yRads), sin(xyRads) ) *noiseStrength;
        }
        
        billboardVels[i] += vec ;
        billboards.getVertices()[i] += billboardVels[i];
        billboardVels[i] *= billboardVelsDecay[i];
        billboards.setNormal(i,ofVec3f((age[i]/ageTarget)*billboardSizeTarget[i],0,0));
        
        ofColor c = billboards.getColors()[i];
        c.setBrightness(255*(age[i]/100.0f));
        billboards.getColors()[i].set(c);
        age[i] *= ageDecay;
    }
    
    //    flocking.update(ofGetElapsedTimef(), 0.06);
    //    VehicleGroup group = flocking.getVehicles();
    //    for (vector<ofxOpenSteerVehicle*>::iterator i = group.begin();
    //         i < group.end();
    //         i++)
    //    {
    //        PathBoid *b = (PathBoid*)*i;
    //        if(b!=NULL && b->pt != NULL){
    //            age[current] = ofRandom(80, 100) ;
    //            billboards.getVertices()[current] = b->start;
    //            billboardVels[current] = ofVec3f (ofRandom(-10,10),
    //                                              ofRandom(-10,10),
    //                                              ofRandom(-10,10))*0.05 + ofVec3f (ofSignedNoise(t, billboards.getVertex(current).y/div, billboards.getVertex(current).z/div),
    //                                                                                   ofSignedNoise(billboards.getVertex(current).x/div, t, billboards.getVertex(current).z/div),
    //                                                                                   ofSignedNoise(billboards.getVertex(current).x/div, billboards.getVertex(current).y/div, t))*0.01;
    //            billboards.getColors()[current].set(ofColor::fromHsb(ofRandom(96, 160), ofRandom(96, 160), ofRandom(160, 255)));
    //            current++;
    //            current%=NUM_BILLBOARDS;
    //        }
    //
    //    }
    
    
    for (vector<Butterfly*>::iterator i = butterflies.begin();
         i < butterflies.end();
         i++){
        Butterfly * b = *i;
        b->update(cam, force.get() , mousePoint.get() , bFollow);
        b->acc = accTarget.get();
        b->velDecay = velDecay;
        b->radius = radiusTarget - ofRandom(0,radiusTargetVari);
        b->noiseStrength = bNoiseStrength;
        b->targetScale = bSize + ofRandom(-bSzieVair,bSzieVair);
        if(b->age > 0){
            age[current] = ageTarget + ofRandom(-ageTargetVari, ageTargetVari) ;
            billboards.getVertices()[current] = b->getPosition();
            billboardVels[current] = ofVec3f (ofRandom(-velx,velx),
                                              ofRandom(-vely,vely),
                                              ofRandom(-velz,velz))*velstrength
            + ofVec3f (ofSignedNoise(t, billboards.getVertex(current).y, billboards.getVertex(current).z),
                       ofSignedNoise(billboards.getVertex(current).x, t, billboards.getVertex(current).z),
                       ofSignedNoise(billboards.getVertex(current).x, billboards.getVertex(current).y, t))*noiseStrength;
            float brightness = ofRandom(160, 255);
            billboards.getColors()[current].set(ofColor::fromHsb(color+ofRandom(-colorVari,colorVari),
                                                                 satuation+ofRandom(-satuationVari,satuationVari),
                                                                 bright+ofRandom(-brightVari,brightVari)
                                                                 ));
            billboardVelsDecay[current] = velsDecay+ofRandom(-velsDecayVari,velsDecayVari);
            billboardSizeTarget[current] = sizeTarget + +ofRandom(-sizeTargetVari,sizeTargetVari);
            
            current++;
            current%=NUM_BILLBOARDS;
        }
        
    }
    light2.setPosition(lightPos);
}

//--------------------------------------------------------------
void ofApp::draw(){
#ifndef TARGET_PROGRAMMABLE_GL
    glShadeModel(GL_SMOOTH); //some model / light stuff
#endif
    ofBackgroundGradient(ofColor::black, ofColor::darkGray);
    ofEnableDepthTest();
    fbo.begin();
    ofBackground(0);
    glPushAttrib(GL_ENABLE_BIT);
    light.enable();
    light2.enable();
//    light3.enable();
//    light4.enable();
    ofEnableSeparateSpecularLight();
    
    cam.begin();
    

    ofDrawGrid(100, ofGetWidth()/100, true, true, true, true);
    ofDrawArrow( light2.getPosition() ,  light2.getPosition()+light2.getLookAtDir() , 10);
    ofPushStyle();
    ofNoFill();
    ofSetColor(255,0,0);
    ofDrawCircle(0, 0, radiusTarget);
    ofSetColor(0,255,0);
    ofDrawCircle(0, 0, radiusTarget-radiusTargetVari);
    ofPopStyle();
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
    
    
    glDepthMask(true);
    for (vector<Butterfly*>::iterator i = butterflies.begin();
         i < butterflies.end();
         i++){
        Butterfly * b = *i;
        b->draw();
    }
    ofPushStyle();
    ofSetColor(ofColor::cyan);
    ofDrawArrow(mousePoint, mousePoint,10);
    ofPopStyle();
    cam.end();
#ifndef TARGET_PROGRAMMABLE_GL
    glEnable(GL_NORMALIZE);
#endif
    ofDisableDepthTest();
    light.disable();
    ofDisableLighting();
    ofDisableSeparateSpecularLight();
    fbo.end();
    
//    fbo.draw(0,0);
    shadertoy.setTexture(0, fbo.getTexture());
    shadertoy.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch(key){
        case '1':
            //            flocking.setFlocking();
            break;
        case '2':
            //            flocking.setPathFollowing();
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
//    float t = (ofGetElapsedTimef()) * 0.9f;
//    float div = 250.0;
//    
//    age[current] = ofRandom(80, 100) ;
//    billboards.getVertices()[current] = ofPoint(cam.screenToWorld(ofPoint(x,y)));
//    billboardVels[current] = ofVec3f (ofRandom(-10,10),
//                                      ofRandom(-10,10),
//                                      ofRandom(-10,10))*0.01 + ofVec3f (ofSignedNoise(t, billboards.getVertex(current).y/div, billboards.getVertex(current).z/div),
//                                                                        ofSignedNoise(billboards.getVertex(current).x/div, t, billboards.getVertex(current).z/div),
//                                                                        ofSignedNoise(billboards.getVertex(current).x/div, billboards.getVertex(current).y/div, t))*0.01;
//    float brightness = ofRandom(160, 255);
//    billboards.getColors()[current].set(ofColor::fromHsb(brightness,50,255));
//    
//    current++;
//    current%=NUM_BILLBOARDS;
    
    
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




