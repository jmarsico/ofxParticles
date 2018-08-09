//
//  ofxParticle.hpp
//  tsneTest
//
//  Created by Ultra Low Res Studuio on 8/9/18.
//
//

#ifndef ofxParticle_h
#define ofxParticle_h

#include <stdio.h>
#include "ofMain.h"

class ofxParticle {
public:
    
    
    ofxParticle();
    ofxParticle(ofVec3f pos, ofVec3f vel, float size_, float life_);
    ofxParticle(const ofxParticle &mom);
    ~ofxParticle(){};
    
    void constrainToRect(ofRectangle bounds, const float k, const float dist, const float drag);
    void applyForce(ofVec3f force);
    float springBoth(ofxParticle * p, const float k, const float springDist, const float drag, const float springSnap);
    
    void attractTo(ofxParticle p, const float accel, const float minDist, const bool consumeParticle);
    void attractTo(ofPoint p, const float accel, const float minDist, const bool consumeParticle);
    
    void gravitateTo(ofxParticle p, const float gravity, const float minDist, const bool consumeParticle);
    void gravitateTo(ofPoint p, const float gravity, const float mass2, const float minDist, const bool consumeParticle);
    void gravitateBoth(ofxParticle * p, const float gravity, const float minDist, bool consumeParticle = false);
    
    void rotateAround(ofxParticle p, const float accel, const float minDist, const bool consumeParticle);
    void rotateAround(ofPoint p, const float accel, const float minDist, const bool consumeParticle);
    
    void updateParticle(const float timeStep, const float drag);
    void drawParticle();
    void drawParticle(ofTexture &tex);
    bool isAlive();
    
    
    bool operator < (const ofxParticle &b){
        return position.z < b.position.z;
    }
    
    ofxParticle & operator=(const ofxParticle &mom){
        if(&mom==this) return *this;
        position = mom.position;
        velocity = mom.velocity;
        acceleration = mom.acceleration;
        rotation = mom.rotation;
        rotationalVelocity = mom.rotationalVelocity;
        color = mom.color;
        mass = mom.mass;
        size = mom.size;
        life = mom.life;
        lifeStart = mom.lifeStart;
        particleID = mom.particleID;
        dt = 1.0/60;
        return *this;
    }


    
    ofVec3f position;
    ofVec3f velocity; // pixels/sec
    ofVec3f acceleration; // pixels/sec/sec
    ofColor color;
    int ID;
    
    ofVec3f rotation;
    ofVec3f rotationalVelocity;
    
    float mass;
    float size;
    float life;
    float lifeStart;
    float dt;
    int particleID;
    
    bool bNoiseOn;
    float xNoiseCounter;
    float yNoiseCounter;
    float zNoiseCounter;
    
}


#endif /* ofxParticle_hpp */
