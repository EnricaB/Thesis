#pragma once

#include "ofMain.h"
#include "Particle.h"
#include "ofxNetwork.h"

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
    double getAverageX(vector<Particle> pList);
    float accelX;

    vector<Particle> particles;
    int person=0;
    int N=1000;
    int lastX;
    int currentX;
    float d;
    
    ofxUDPManager udpConnection;
    
    vector<ofVec3f> accel;
    vector<ofVec3f> magne;
    
    string recvdDataString;
    string sentDataString;
};
