#pragma once

#include "ofMain.h"

class Particle {
public:
    Particle();
    
    void setInit(ofPoint _pos, ofPoint _vel = ofPoint(0,0));
    void setDamp(float damp);
    float   getRadius();
    ofPoint getPosition();
    ofPoint getVelocity();
    ofPoint getX();
    ofPoint getHeading();
    
    void addForce(ofPoint _force);
    
    void addRepulsionForce(ofPoint _pos, float _rad, float _scale);
    void addAttractionForce(ofPoint _pos, float _rad, float _scale);
    void addClockwiseForce( ofPoint _pos, float _rad, float _scale);
    void addCounterClockwiseForce( ofPoint _pos, float _rad, float _scale);
    void addVel(float _vel);
    void addLeft(float _scale);
    void addRight(float _scale);
    void seek( ofPoint dest,float max_force );
    void addFlockingForce(vector<Particle> &_particles, float _neighborhoodRadius, float _alignmentScale = 1, float _separationScale = 0.33, float _cohesionScale = 0.33);
    
    void bounceOnWall();
    void infiniteWalls();
    //   from avoidwalls
    void resetForces();
    //void applyForce(ofVec2f force);
    void avoidWalls(float d);
    
  //  ofVec2f pos, vel, acc;
    float mass;
    
    float maxSpeed, maxForce;
    // end walls
    
    
    void update();
    void draw();
    
private:
    ofPoint pos;
    ofPoint vel;
    ofPoint acc;
    ofPoint Xpos;
    ofPoint heading;

    float radius;
    float damp;
};