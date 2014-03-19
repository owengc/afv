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
#include "ofxVectorGraphics.h"
class Figure {
public:
	//Consructor
	Figure(float data);
	
	//Methods
	void update();
	void chooseTarget();
	void moveTo();
	void draw();
	
	//Properties
	ofColor color;
	float input;
	float xPosition;
	float yPosition;
	float xTarget;
	float yTarget;
	
	float lifespan;
};
#endif