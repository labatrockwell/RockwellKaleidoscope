#pragma once

#define SCREEN_RES_W 1024
#define SCREEN_RES_H 768

#include "ofMain.h"
#include "Kaleidoscope.h"
#include "ofxUI.h"

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
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void exit();
    void guiEvent(ofxUIEventArgs &e);
private:
    ofVideoGrabber vidGrabber;
    int camWidth, camHeight;
    Kaleidoscope kaleidoscope;
    ofxUICanvas *gui;
    ofxUIVec3f kCenter;
    ofxUIVec3f kScale;
    ofxUIVec3f screenSize;
    bool viewRawCam;
    float m_fRotate;
    float waveAmplitude;
    float kaleidoscopeSpd;
    ofxUIVec3f m_Trans;
    
};
