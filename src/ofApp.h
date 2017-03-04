#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxGui.h"
#include "ofxOpenSteer.h"
#include "ofxParticleSystemGPU.h"
#include "ofxXmlSettings.h"
#include "Flocking.h"
#include "ofxGameCamera.h"
#include "Butterfly.hpp"
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
    
//    vector<ofxOpenSteerVehicle*>::iterator it;
    int current = 0;
//    vector<Butterfly*>::iterator it;
    vector<Butterfly*> butterflies;
    
};
