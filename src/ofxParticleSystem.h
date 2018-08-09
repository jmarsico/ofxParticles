//
//  ofxParticleSystem.hpp
//  tsneTest
//
//  Created by Ultra Low Res Studuio on 8/9/18.
//
//

#ifndef ofxParticleSystem_hpp
#define ofxParticleSystem_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxParticle.h"


class ofxParticleSystem {
public:
    ofxParticleSystem();
    ~ofxParticleSystem(){}
    
    void addParticle(ofxParticle *p);
    list<ofxParticle*> getParticles();
    
    void attractTo(ofPoint p, const float accel, const float minDist, const bool consumeParticle);
    
    void gravitateTo(ofPoint p, const float gravity, const float mass, const float minDist, const float consumeParticle);
    
    void rotateAround(ofPoint p, const float accel, const float minDist, const float consumeParticle);
    
    void applyVectorField(float * field, int fieldWidth, int fieldHeight, int numComponents, ofRectangle areaOfInfluence, float force);
    
    int update(float timeStep, float drag);
    
    void cull(int numToCull);
    
    void draw();
    void draw(ofTexture &tex);
    void draw(ofTexture &tex, ofTexture &tex2);
    int getNumParticles(){return numParticles;}
    
private:
    list<ofxParticle*> particles;
    int numParticles;
    int totalParticlesEmitted;
};


#endif /* ofxParticleSystem_hpp */
