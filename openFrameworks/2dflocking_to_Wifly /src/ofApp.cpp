#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    d = 100;

    
// from Wifly
    ofSetVerticalSync(true);
	ofSetFrameRate(60);
    

    
    //create the socket and bind to port 11999
	udpConnection.Create();
	udpConnection.Bind(11999);
	udpConnection.SetNonBlocking(true);
    
	ofSetBackgroundAuto(false);
    ofBackground(50);
    
    recvdDataString = "Waiting for WiFly to Connect...";
// end wifly
    
    
    //  Create max Particles
    // this for loop is for every particle system :make an instance and call it
    for (int i = 0; i < N; i++) {
        Particle newParticle;
        newParticle.setInit(ofPoint(ofGetWidth()*0.5,ofGetHeight()*0.5),
                            ofPoint(ofRandom(-1, 1), ofRandom(-1, 1)));

        particles.push_back(newParticle);
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    //from wifly
    float aX,aY,aZ,mX,mY,mZ,dataID;
    
	char udpMessage[100000];
	udpConnection.Receive(udpMessage,100000);
	string message=udpMessage;
	if(message!=""){
        //      cout << "raw msg: ";
        //		cout << message << endl;
        
		vector<string> fullMsg = ofSplitString(message,"/");
        //        cout << "fullMsg size "<<fullMsg.size() << endl;
        //        cout << "fullMsg 0 "<<fullMsg[0] << endl;
        //        cout << "fullMsg 1 "<<fullMsg[1] << endl;
        
        if (fullMsg.size() >= 2) { // received a full, correct message
            
            for(int k=1; k < fullMsg.size(); k++){
                vector<string> dataPoints = ofSplitString(fullMsg[k],"\t");
                //cout << "datapoints size: "<<dataPoints.size() << endl;
                if (dataPoints.size() == 7) {
                    dataID = atof(dataPoints[0].c_str());
                    accel.push_back(ofVec3f(atof(dataPoints[1].c_str()),atof(dataPoints[2].c_str()),atof(dataPoints[3].c_str())));
                    magne.push_back(ofVec3f(atof(dataPoints[4].c_str()),atof(dataPoints[5].c_str()),atof(dataPoints[6].c_str())));
                    
                    
                    int readingNum = accel.size();
                    cout << "total readings: "<< readingNum;
                    cout << "\tid: "<< dataID; //WHICH UNIT IS THIS ONE
                    cout << "\tAccel: " << accel[accel.size()-1].x << ", " << accel[accel.size()-1].y << ", " << accel[accel.size()-1].z;
                    cout << "\t\tMagne: " << magne[accel.size()-1].x << ", " << magne[accel.size()-1].y << ", " << magne[accel.size()-1].z << endl;
                    
                    recvdDataString = "received from WiFly id# "+ofToString(dataID)
                    + "\n\nAccel: "+ ofToString(accel[accel.size()-1].x) +", "+ ofToString(accel[accel.size()-1].y) +", "+ ofToString(accel[accel.size()-1].z)
                    + "\tMagne: " + ofToString(magne[accel.size()-1].x) +", "+ ofToString(magne[accel.size()-1].y) +", "+ ofToString(magne[accel.size()-1].z);
                    
             
                    
                } else{
                    cout << "WRONG NUMBER OF DATAPOINTS RECEIVED: " << dataPoints.size() <<endl;
                    recvdDataString = "ERROR: Wrong Number of Datapoints Received !";
                }
            }
        } else {
            cout << "WRONG NUMBER OF MSG STRINGS RECEIVED: "<< fullMsg.size() << endl;
            recvdDataString = "ERROR: Wrong number of Msg Strings Received !";
        }
	}

    //end wifly

        ofPoint mouse = ofPoint(mouseX,mouseY);
    
    for (int i = 0; i < particles.size(); i++) {
    
        particles[i].addFlockingForce(particles, 80, 0.333, 0.33, 0.33);
        
        
        particles[i].avoidWalls(d);
        
        
       
        particles[i].update();
       //particles[i].infiniteWalls();
    }
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
 /*   ofSetColor(255, 0, 0);
    ofNoFill();
    ofRect(d, d, ofGetWidth()-d-d, ofGetHeight()-d-d);
*/
    
    
    
    if(ofGetKeyPressed('s')){
        ofBeginSaveScreenAsPDF(ofGetTimestampString()+".pdf");
    }
    
    ofBackground(0, 0, 0);
    
    for (int i = 0; i < N; i++) {
        if(i==person)
        {
            ofSetColor(255,0,0);
            
      //   accelX =(magne[accel.size()-1].x);
    //        if ( accelX >= 130 ) {
     //            particles[person].addVel(0.001);
      //      }
            
            ofDrawBitmapString("Average X or particles: "+ofToString(getAverageX(particles)), 25, 290);
            
            lastX = (getAverageX(particles));
            
            if (lastX != currentX) {
               

                if (lastX <= currentX) {
                    ofDrawBitmapString("Going Left!", 25, 320);
                    sentDataString = "L";
                    udpConnection.Send("L", 1);
                    
                }
                else
                {
                    ofDrawBitmapString("Going Right!", 25, 320);
                    sentDataString = "R";
                    udpConnection.Send("R", 1);
                }
                
                currentX = lastX;
            }
            
          

        
            
            
        }
        else
        {
            ofSetColor(255);
        }
        particles[i].draw();
       

    }
    
    
    if(ofGetKeyPressed('s')){
        ofEndSaveScreenAsPDF();
    }
    
    
    // from wifly
    //  ofBackground(50);
    ofSetColor(255);
    ofDrawBitmapString("total readings received: "+ofToString(accel.size()), 25, 50);
    ofSetColor(50,255,50);
    ofDrawBitmapString(recvdDataString, 25, 100);
    
    ofSetColor(200, 200, 255);
    
    //end wifly
    
    
    ofSetColor(0, 255, 0);
    ofCircle(getAverageX(particles), ofGetHeight()/2, 5);
    

    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key==357)
        particles[person].addVel(0.001);
    if(key==356)
        particles[person].addLeft(1.0);
    if(key==358)
        particles[person].addRight(1.0);

}

double ofApp::getAverageX(vector<Particle> pList) {
    
    double avgX;
    
    for (int i = 0; i < pList.size(); i++ ) {
        avgX += pList[i].getPosition().x;
    }
    
    avgX /= pList.size();
    
    return avgX;
    
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

    ofPoint mouse = ofPoint(x,y);
    //particles[person].seek(mouse,6.0);
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
