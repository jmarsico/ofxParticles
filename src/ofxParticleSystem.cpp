//
//  ofxParticleSystem.cpp
//  tsneTest
//
//  Created by Ultra Low Res Studuio on 8/9/18.
//
//

#include "ofxParticleSystem.h"


ofxParticleSystem::ofxParticleSystem(){
    numParticles = 0;
    totalParticlesEmitted = 0;
}


void ofxParticleSystem::addParticle(ofxParticle *p){
    particles.push_back(p);
    
}


list<ofxParticle*> ofxParticleSystem::getParticles(){
    return particles;
}

void ofxParticleSystem::attractTo(ofPoint p, const float accel, const float minDist, const bool consumeParticle){
    for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
        (**it).attractTo(p, accel, minDist, consumeParticle);
    }
}

void ofxParticleSystem::gravitateTo(ofPoint p, const float gravity, const float mass, const float minDist, const float consumeParticle){
    for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
        (**it).gravitateTo(p, gravity, mass, minDist, consumeParticle);
    }
}

void ofxParticleSystem::rotateAround(ofPoint p, const float accel, const float minDist, const float consumeParticle) {
    for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
        (**it).rotateAround(p, accel, minDist, consumeParticle);
    }
}

void ofxParticleSystem::applyVectorField(float * field, int fieldWidth, int fieldHeight, int numComponents, ofRectangle areaOfInfluence, float force) {
    for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
        ofxParticle & p = (**it);
        ofVec2f pos(p.position.x,p.position.y);
        if(areaOfInfluence.inside(pos)){
            int x = (int)ofMap(pos.x, areaOfInfluence.getMinX(), areaOfInfluence.getMaxX(), 0, fieldWidth-1);
            int y = (int)ofMap(pos.y, areaOfInfluence.getMinY(), areaOfInfluence.getMaxY(), 0, fieldHeight-1);
            int index = (x+y*fieldWidth)*numComponents;
            ofVec2f dir(field[index],field[index+1]);
            dir.scale(force);
            //                                    cout << "(" << dir.x << "," << dir.y << ")\n";
            p.applyForce(dir);
        }
    }
}

int ofxParticleSystem::update(float timeStep, float drag){
    int particlesRemoved = 0;
    for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
        if((**it).isAlive()){
            (**it).update(timeStep, drag);
        }
        else{
            ofxParticle * p = (*it);
            it = particles.erase(it);
            delete p;
            particlesRemoved++;
        }
    }
    numParticles-=particlesRemoved;
    return particlesRemoved;
}

void ofxParticleSystem::cull(int numToCull){
    
    for(size_t i = 0; i < numToCull; i++){
        list<ofxParticle*>::iterator it = particles.begin();
        advance(it, floor(ofRandom(particles.size())));
        (**it).life = ofRandom(0.5);
    }
    
}

void ofxParticleSystem::draw(){
    for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
        (**it).draw();
    }
}
void ofxParticleSystem::draw(ofTexture &tex){
    //                particles.sort();
    for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
        (**it).draw(tex);
    }
}

void ofxParticleSystem::draw(ofTexture &tex, ofTexture &tex2){
    //                particles.sort();
    for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
        int index = (**it).particleID;
        if(index%2==0)
            (**it).draw(tex);
        else
            (**it).draw(tex2);
    }
}

int ofxParticleSystem::getNumParticles(){
    return numParticles;
}
