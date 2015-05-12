#include "Particle.h"

Particle::Particle() {
    pos.x = ofGetWindowWidth()*0.5;
    pos.y = ofGetWindowHeight()*0.5;
    pos.z = 0.0;
    
    vel.set(0,0,0);
    acc.set(0,0,0);
    
    damp = 0.97; // de resistance!!!
    radius = ofRandom(1, 3);
    
    maxSpeed = 10.0;
    maxForce = 0.25;
    
}

void Particle::setInit(ofPoint _pos, ofPoint _vel){
    pos = _pos;
    vel = _vel;
    acc.set(0,0,0);
}

void Particle::setDamp(float damp)
{
    this->damp=damp;

}
ofPoint Particle::getPosition(){
    return pos;
}

ofPoint Particle::getX(){
    return Xpos;
}

ofPoint Particle::getHeading(){
    return heading;
}



ofPoint Particle::getVelocity(){
    return vel;
}

float Particle::getRadius(){
    return radius;
}

void Particle::addForce(ofPoint _force){
    acc += _force;
}

void Particle::addRepulsionForce(ofPoint _pos, float _rad, float _scale){
    
    ofPoint diff = _pos - pos;
    if( diff.length() < _rad ){
        diff *= 1.0-diff.length()/_rad;// PRO way
        addForce(-diff*_scale);
    }
}

void Particle::addAttractionForce(ofPoint _pos, float _rad, float _scale){
    ofPoint diff = _pos - pos;
    if( diff.length() < _rad ){
        diff *= 1.0-diff.length()/_rad;
        addForce(diff*_scale);
    }
}


void Particle::addClockwiseForce( ofPoint _pos, float _rad, float _scale){
	ofVec2f diff = pos - _pos;
	
	if (diff.length() < _rad){
		float pct = 1 - (diff.length() / _rad);
		diff.normalize();
		acc.x -= diff.y * pct * _scale;
		acc.y += diff.x * pct * _scale;
	}
}

void Particle::addCounterClockwiseForce( ofPoint _pos, float _rad, float _scale){
	ofVec2f diff = pos - _pos;
	
	if (diff.length() < _rad){
		float pct = 1 - (diff.length() / _rad);
		diff.normalize();
		acc.x += diff.y * pct * _scale;
		acc.y -= diff.x * pct * _scale;
	}
}


void Particle::addVel(float _vel=0.001)
{
      vel  +=   vel;
    
}




void Particle::addLeft(float _scale=1.0)
{
    acc.x -=   _scale;
    acc.y +=   _scale;
    
}

void Particle::addRight(float _scale=1.0)
{
    acc.x +=  _scale;
    acc.y -=  _scale;
    
}

void Particle::seek( ofPoint dest,float maxForce=0.4 ) {
    float maxSpeed = 10.0;

    
    float slowDownRadius = 200.0;
    
    ofPoint desired = dest - pos;
    
    if( desired.length() < slowDownRadius ){
        float newMag = ofMap( desired.length(), 0, slowDownRadius, 0, maxSpeed);
        
        desired.normalize();
        desired *= newMag;
    } else {
        desired.normalize();
        desired *= maxSpeed;
    }
    
    ofPoint steer = desired - vel;
    steer.limit( maxForce );
    
    addForce( steer );
}


void Particle::addFlockingForce(vector<Particle> &_particles, float _neighborhoodRadius, float _alignmentScale, float _separationScale, float _cohesionScale) {
    
    float zoneRadiusSq = _neighborhoodRadius * _neighborhoodRadius;
    
    ofPoint sep;    //  SEPARATION

    ofPoint ali;    //  ALIGNMENT
    int aliCount = 0;

	ofPoint coh;    //  COHESION
    ofPoint posSum;
    int cohCount = 0;
    
    for (int i = 0 ; i < _particles.size() ; i++) {
        if(this != &_particles[i]){
            ofPoint diff = pos - _particles[i].getPosition();
            float lenSq = diff.lengthSquared();
            
            // We only want a boid to act on another boid if it's with zoneRadiusSq.
            // If they aren't within that distance, they are too far away and don't affect each other.
            if( lenSq < zoneRadiusSq ){
            
                //  add separation force
                //
                ofPoint repulse = diff;
                repulse.normalize();
                repulse /= sqrt(lenSq);
                sep += repulse;
                
                //  add align force
                //
                ali += _particles[i].getVelocity();
                aliCount++;
                
                //  add cohesion force
                //
                posSum += _particles[i].getPosition(); // Add location
                cohCount++;
            }
        }
    }
    
    //  Average of Align
    //
    if (aliCount > 0) {
		ali /= (float)aliCount;
		ali.limit(0.3);
    }
    
    //  Average of cohesion
    //
    if (cohCount > 0){
		posSum /= (float)cohCount;
    }
	coh = posSum - pos;
	coh.limit(0.5);
	
    
    //  Apply forces into acc
    //
	acc += ali * _alignmentScale;
	acc += coh * _separationScale;
	acc += sep * _cohesionScale;
}

void Particle::update() {
    
    vel += acc;
    vel *= damp;
    pos += vel;
    acc *= 0.0;
  //  Xpos += pos;

}

/* void Particle::bounceOnWall(){
    if( pos.x < 0.0+radius || pos.x > ofGetWidth()-radius ){
        pos.x -= vel.x; // Bounced back! think of this as an undo;
        vel.x *= -1.0;
    }
    
    if( pos.y < 0.0+radius || pos.y > ofGetHeight()-radius ){
        pos.y -= vel.y; // Bounced back! think of this as an undo;
        vel.y *= -1.0;
    }
} */

void Particle::infiniteWalls(){
    if( pos.x < 0.0 ){
        pos.x = ofGetWidth()-1;
    }
    
    if( pos.x > ofGetWidth()){
        pos.x = 1;
    }
    
    if( pos.y < 0){
        pos.y = ofGetHeight()-1;
    }
    
    if( pos.y > ofGetHeight()){
        pos.y = 1;
    }
}

void Particle::draw() {
    ofCircle(pos, radius);
//    ofCircle(pos, ofRandom(2, 5));
//    ofEllipse(pos.x, pos.y, ofRandom(4, 10), ofRandom(2, 5));
    
    ofPoint velNormal = vel;
	velNormal.normalize();
	
	ofVec2f velPerp;
	velPerp.x = -velNormal.y;
	velPerp.y = velNormal.x;
	
	//ofLine(pos.x, pos.y, pos.x + velNormal.x*10, pos.y + velNormal.y*10);
}

void Particle::avoidWalls(float d) {
    
    ofPoint desired;
    desired.set(0, 0);
    
    if (pos.x < d) {
        desired.set(maxSpeed, vel.y);
    }
    else if (pos.x > ofGetWidth()-d) {
        desired.set(-maxSpeed, vel.y);
    }
    
    if (pos.y < d) {
        desired.set(vel.x, maxSpeed);
    }
    else if (pos.y > ofGetHeight()-d) {
        desired.set(vel.x, -maxSpeed);
    }
    
    
    
    if (desired.length() != 0.0) {
        desired.normalize();
        desired *= maxSpeed;
        ofPoint steer = desired - vel;
        steer.limit(maxForce);
        //applyForce(steer);
        addForce(steer);
    }
    
}