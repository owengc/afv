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
	Figure(float data, ofxVectorGraphics * vg, ofxAudioFeaturesChannel * fc);
	
	//Methods
	void update();
	void chooseTarget();
	void moveTo();
	void draw(const float textureFeatureValue);
	
	//Properties
	ofColor color;
	float input;
	float xPosition;
	float yPosition;
	float xTarget;
	float yTarget;
    float xOrigin;
    float yOrigin;
    float radius;
    
	float lifespan;
    ofxVectorGraphics * vectorGraphics;
    ofxAudioFeaturesChannel * featuresChannel;
};
#endif