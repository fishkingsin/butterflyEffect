#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"

#include "ofxGameCamera.h"
#include "Butterfly.hpp"
#include "ofxShadertoy.h"
#define NUM_BILLBOARDS 5000
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    ofEasyCam cam;
//    Flocking flocking;
    
    float billboardSizeTarget[NUM_BILLBOARDS];
    
    ofShader billboardShader;
    ofImage texture;
    
    ofVboMesh billboards;
    ofVec3f billboardVels[NUM_BILLBOARDS];
    float age[NUM_BILLBOARDS];
    float billboardVelsDecay[NUM_BILLBOARDS];
//    vector<ofxOpenSteerVehicle*>::iterator it;
    int current = 0;
//    vector<Butterfly*>::iterator it;
    vector<Butterfly*> butterflies;
    ofxPanel gui;
    
    ofParameterGroup particleParaGroup;
    ofParameter<float> velx;
    ofParameter<float> vely;
    ofParameter<float> velz;
    ofParameter<float> velstrength;
    ofParameter<float> noiseStrength;
    ofParameter<int> color;
    ofParameter<int> size;
    ofParameter<int> satuation;
    ofParameter<int> bright;
    
    ofParameter<int> colorVari;
    ofParameter<int> satuationVari;
    ofParameter<int> brightVari;
    ofParameter<float> velsDecay;
    ofParameter<float> velsDecayVari;
    ofParameter<int> noiseMode;
    ofParameter<float> sizeTarget;
    ofParameter<float> sizeTargetVari;
    
    
    ofParameter<int> ageTarget;
    ofParameter<int> ageTargetVari;
    ofParameter<float> ageDecay;
    ofParameter<ofVec3f> lightPos;
    ofParameter<ofVec2f> force;
    ofParameter<ofVec3f> accTarget;
    ofParameter<float> bNoiseStrength;
    ofParameterGroup butterflyParaGroup;
    ofFbo fbo;
    ofLight	light,light2;//,light3,light4;
    
    ofxShadertoy shadertoy;

};
