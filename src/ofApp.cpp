#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    bool USBviaETH = false;
    camWidth 		= USBviaETH ? 320 : 1920;//1920;//640;	// try to grab at this size.
    camHeight 		= USBviaETH ? 240 : 1080;//1080;//480;
    
    waveAmplitude = 60.f;
    brightness = 1.0f;
    contrast = 1.0f;
    saturation = 1.0f;
    kaleidoscopeSpd = .1;
    
    //we can now get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    
    for(int i = 0; i < devices.size(); i++){
        cout << devices[i].id << ": " << devices[i].deviceName;
        if( devices[i].bAvailable ){
            vector<ofVideoFormat> vidFormats = devices[i].formats;
            cout << ", " << vidFormats.size() << " formats" << endl;
            
            //list all formats, and select the one with the most pixels
            /*
             for(auto & format : vidFormats){
             cout << devices[i].id << ": " << format.width << "x" << format.height << endl;
             if (camWidth*camHeight < format.width*format.height){
             camWidth = format.width;
             camHeight = format.height;
             }
             }*/
        }else{
            cout << " - unavailable " << endl;
        }
    }
    
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(60);
    vidGrabber.initGrabber(640,480);
    
    screenSize.x = 1920;
    screenSize.y = 1920;
    kaleidoscope.setup(screenSize.x, screenSize.y, kaleidoscopeSpd);
    
    ofSetVerticalSync(true);
    
    gui = new ofxUICanvas(100,500);        //Creates a canvas at (0,0) using the default width
    gui->addToggle("FULLSCREEN", false);
    gui->addToggle("VISIBLE", true);
    viewRawCam = false;
    gui->addToggle("raw feed", &viewRawCam);
    gui->addSlider("triangleCameraGrab", 200, 2000, &kaleidoscope.triangleCameraGrab, 300, 10);
    kCenter.x = kaleidoscope.centerPoint.x;
    kCenter.y = kaleidoscope.centerPoint.y;
    gui->add2DPad("centerPoint", ofxUIVec2f(0, 1000), ofxUIVec2f(0, 1000), &kCenter, 50, 50);
    kScale.x = kaleidoscope.ksx;
    kScale.y = kaleidoscope.ksy;
    gui->add2DPad("scale", ofxUIVec2f(100, 600), ofxUIVec2f(100, 600), &kScale, 100, 100);
    gui->add2DPad("screenDims", ofxUIVec2f(0, 2000), ofxUIVec2f(0, 2000), &screenSize, 100, 100);
//    gui->addToggle("drawDebug", &kaleidoscope.bDrawDebug);
    gui->addFPS();
    //TODO: add ddl for cameras and resolutions
    gui->add2DPad("translate", ofxUIVec2f(-3000, 0), ofxUIVec2f(-3000, 0), &m_Trans, 300, 300);
    gui->addSlider("rotation", 0, 90, &m_fRotate, 300, 20);
    gui->addSlider("speed", 0, 1.0f, &kaleidoscopeSpd , 300, 10);
    gui->addSlider("waveSize", 0, 200, &waveAmplitude, 300, 10);
    gui->addSlider("brightness", 0, 1, &brightness, 300, 10);
    gui->addSlider("contrast", 0, 5, &contrast, 300, 10);
    gui->addSlider("saturation", 0, 5, &saturation, 300, 10);
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent, this, &ofApp::guiEvent);
    gui->loadSettings("settings.xml");
}

//--------------------------------------------------------------
void ofApp::update(){
    vidGrabber.update();
    kaleidoscope.centerPoint.x = kCenter.x;
    kaleidoscope.centerPoint.y = kCenter.y;
    kaleidoscope.ksx = kScale.x;
    kaleidoscope.ksy = kScale.y;
    kaleidoscope.update( vidGrabber.getTextureReference(), waveAmplitude, kaleidoscopeSpd, brightness, contrast, saturation );
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushMatrix();
    ofRotate(m_fRotate);
    ofTranslate(m_Trans.x, m_Trans.y);
    if (viewRawCam){
        vidGrabber.draw(0, 0, 1000, 1000);
    } else {
        kaleidoscope.render(screenSize.x*2, screenSize.y*2, vidGrabber.getTextureReference());
    }
    ofPopMatrix();
    
//    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate()), 500, 500);
    
}

void ofApp::guiEvent(ofxUIEventArgs &e)
{
    if(e.getName() == "FULLSCREEN")
    {
        ofxUIToggle *toggle = e.getToggle();
        ofSetFullscreen(toggle->getValue());
    } else if (e.getName() == "VISIBLE"){
        gui->setVisible(e.getToggle()->getValue());
        if (e.getToggle()->getValue()){
            ofShowCursor();
        } else {
            ofHideCursor();
        }
    }
}

void ofApp::exit()
{
    gui->saveSettings("settings.xml");
    delete gui;
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key){
        case 'g':
        {
            ((ofxUIToggle*)gui->getWidget("VISIBLE"))->setValue(!gui->isVisible());
            gui->getWidget("VISIBLE")->triggerSelf();
            break;
        }
        case 'f':
        {
            kaleidoscope.togglePredator();
            break;
        }
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){}