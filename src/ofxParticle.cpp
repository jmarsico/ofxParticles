//
//  ofxParticle.cpp
//  tsneTest
//
//  Created by Ultra Low Res Studuio on 8/9/18.
//
//


#include "ofxParticle.h"

/////////////////////////////////////////////
ofxParticle::ofxParticle(){
    position = velocity = acceleration = ofVec3f(0,0,0);
    color = ofColor(255,255,255,255);
    mass = 1.0;
    size = 1.0;
    lifeStart = life = 1.0;
    particleID = 0;
    dt = 1.0/60;
    bNoiseOn = true;
    xNoiseCounter = ofRandom(2.0);
    yNoiseCounter = ofRandom(3.0);
    zNoiseCounter = ofRandom(1.0);
}


/////////////////////////////////////////////
ofxParticle::ofxParticle(ofVec3f pos, ofVec3f vel, float size_, float life_){
    position = pos;
    velocity = vel;
    acceleration = ofVec3f(0,0,0);
    color = ofColor(255,255,255,255);
    mass = 1.0;
    size = size_;
    lifeStart = life = life_;
    particleID = 0;
    dt = 1.0/60;
}

/////////////////////////////////////////////
ofxParticle::ofxParticle(const ofxParticle &mom){
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
}


/////////////////////////////////////////////
void ofxParticle::constrainToRect(ofRectangle bounds, const float k, const float dist, const float drag){
    ofPoint minPoint = bounds.getTopLeft();
    ofPoint maxPoint = bounds.getBottomRight();
    float inverse_drag = 1.0f / drag;
    float inverse_mass = 1.0f / mass;
    float spring_constant = inverse_mass * inverse_drag;
    float force;
    ofVec3f dir;
    float dis;
    
    // left side
    if (position.x < minPoint.x) {
        velocity.x = minPoint.x - position.x;
        position.x = minPoint.x+1;
    }
    if (position.x < minPoint.x + dist) {
        dir = ofVec3f(1,0,0);
        dis = position.x - minPoint.x;
        dis = dist - dis;
        force = -k * dis * spring_constant;
        acceleration += dir*(-force);
    } else {
        // right side
        if (position.x > maxPoint.x) {
            velocity.x = maxPoint.x - position.x;
            position.x = maxPoint.x-1;
        }
        if (position.x > maxPoint.x - dist) {
            dir = ofVec3f(-1,0,0);
            dis = maxPoint.x - position.x;
            dis = dist - dis;
            force =  -k * dis * spring_constant;
            acceleration += dir*(-force);
        }
    }
    
    // top side
    if (position.y < minPoint.y) {
        velocity.y = minPoint.y - position.y;
        position.y = minPoint.y+1;
    }
    if (position.y < minPoint.y + dist) {
        dir = ofVec3f(0,1,0);
        dis = position.y - minPoint.y;
        dis = dist - dis;
        force = -k * dis * spring_constant;
        acceleration += dir*(-force);
    } else {
        // bottom side
        if (position.y > maxPoint.y) {
            velocity.y = maxPoint.y - position.y;
            position.y = maxPoint.y-1;
        }
        if (position.y > maxPoint.y - dist) {
            dir = ofVec3f(0,-1,0);
            dis = maxPoint.y - position.y;
            dis = dist - dis;
            force =  -k * dis * spring_constant;
            acceleration += dir*(-force);
        }
    }
}


/////////////////////////////////////////////
void ofxParticle::applyForce(ofVec3f force){
    acceleration += force * dt;
}



/////////////////////////////////////////////
float ofxParticle::springBoth(ofxParticle * p, const float k, const float springDist, const float drag = 1.0f, const float springSnap = 1.0f){
    float dist = position.distance(p->position);
    if(dist > springDist * springSnap) return 0.0;
    
    dist = springDist - dist;
    float force = (-k / (mass * p->mass)) * (dist / drag);
    
    ofVec3f dir = ((ofVec3f)(p->position - position)).getNormalized();
    
    acceleration += dir * (force / mass);
    p->acceleration -= dir * (force / p->mass);
    return -force;
}


/////////////////////////////////////////////
void ofxParticle::attractTo(ofxParticle p, const float accel, const float minDist, const bool consumeParticle){
    attractTo(p.position, accel, minDist, consumeParticle);
}


/////////////////////////////////////////////
void ofxParticle::attractTo(ofPoint p, const float accel, const float minDist, const bool consumeParticle){
    ofVec3f dir = p-position;
    float dist = dir.length();
    if(dist < minDist){
        dist = minDist;
        if(consumeParticle)
            life = 0;
    }
    
    dir.normalize().scale(accel*dt);
    acceleration += dir;

}

/////////////////////////////////////////////
void ofxParticle::gravitateTo(ofxParticle p, const float gravity, const float minDist, const bool consumeParticle){
    gravitateTo(p.position, gravity, p.mass, minDist, consumeParticle);
}


/////////////////////////////////////////////
void ofxParticle::gravitateTo(ofPoint p, const float gravity, const float mass2, const float minDist, const bool consumeParticle){
    if(p==position) return;
    ofVec3f dir = p-position;
    float dist = dir.length();
    if(dist < minDist){
        dist = minDist;
        if(consumeParticle)
            life = 0;
    }
    dir.normalize().scale( gravity * mass * mass2 * dt * (1.0/(dist)));
    acceleration += dir;
}



/////////////////////////////////////////////
void ofxParticle::gravitateBoth(ofxParticle * p, const float gravity, const float minDist, bool consumeParticle = false) {
    if(p==this) return;
    ofVec3f dir = p->position - position;
    float dist = dir.length();
    if(dist < minDist){
        dist = minDist;
        if(consumeParticle)
            life = 0;
    }
    dir.normalize().scale( gravity * mass * p->mass * dt * (1.0/(dist)));
    acceleration += dir;
    p->acceleration -= dir;
}


/////////////////////////////////////////////
void ofxParticle::rotateAround(ofxParticle p, const float accel, const float minDist, const bool consumeParticle) {
    rotateAround(p.position, accel, minDist, consumeParticle);
}


/////////////////////////////////////////////
void ofxParticle::rotateAround(ofPoint p, const float accel, const float minDist, const bool consumeParticle){
    ofVec3f toPoint = position - p;
    ofVec3f dir = ofVec3f(-toPoint.y, toPoint.x, toPoint.z);
    float dist = toPoint.length();
    if(dist < minDist){
        dist = minDist;
        if(consumeParticle)
            life = 0;
    }
    dir.normalize().scale((accel/dist) *dt);
    
    acceleration += dir;
}


void ofxParticle::updateParticle(const float timeStep, const float drag){
    dt = timeStep;
    velocity += acceleration * dt;
    velocity -= (velocity * dt * (1.0-drag));
    position += velocity * dt;
    acceleration -= acceleration * dt;
    rotation += rotationalVelocity * dt;
    
    life -= dt;
    bNoiseOn = true;
    if(bNoiseOn){
        //                position.x += 10.0 * ofSignedNoise(xNoiseCounter + dt * 0.1);
        //                position.y += 10.0 * ofSignedNoise(yNoiseCounter + dt * 0.2);
        //                position.z += 10.0 * ofSignedNoise(zNoiseCounter + dt * 0.8);
    }
}

void ofxParticle::drawParticle(){
    ofColor c = color;
    c.a = life/lifeStart*color.a;
    ofSetColor(c);
    ofDrawLine(position, position-velocity*dt);
}

void ofxParticle::drawParticle(ofTexture &tex){
    float w = tex.getWidth();
    float h = tex.getHeight();
    if(w > h){
        h = h/w*size;
        w = size;
    }
    else{
        w = w/h*size;
        h = size;
    }
    ofSetColor(color);
    ofPushMatrix();
    ofTranslate(position);
    ofRotateX(rotation.x);
    ofRotateY(rotation.y);
    ofRotateZ(rotation.z);
    tex.draw(w*-0.5,h*-0.5,w,h);
    ofPopMatrix();
}





