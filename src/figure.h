/*
 *  figure.h
 *  audioInputExample
 *
 *  Created by Owen Campbell on 7/5/12.
 *  Copyright 2012 tick tock audio. All rights reserved.
 *
 */
#ifndef figure_h
#define figure_h

#include "ofMain.h"
#include "ofxAudioFeaturesChannel.h"
#include "ofxVectorGraphics.h"
class Figure {
public:
	//Consructor
	Figure(const float l, const float s, const float t, ofxVectorGraphics * vg, ofxAudioFeaturesChannel * fc);
	
	//Methods
	void update(const float t, const float x);
	void chooseTarget();
	void moveTo();
	void draw();
	
	//Properties
	ofColor color;
	float xPosition;
	float yPosition;
	float xTarget;
	float yTarget;
    float xOrigin;
    float yOrigin;
    float radius;
    
	float lifespan;
    float size;
    float texture;
    
    
    ofxVectorGraphics * vectorGraphics;
    ofxAudioFeaturesChannel * featuresChannel;
};
#endif