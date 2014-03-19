/*
 *  figure.cpp
 *  audioInputExample
 *
 *  Created by Owen Campbell on 7/5/12.
 *  Copyright 2012 tick tock audio. All rights reserved.
 *
 */

#include "figure.h"

Figure::Figure(float data){
	
	ofSeedRandom();
	input = data;
	lifespan = data*100.0;
	color.setHsb(ofRandom(255),ofRandom(255),ofRandom(255));
	
	xPosition = ofGetWindowWidth()*0.5;
	yPosition = ofGetWindowHeight()*0.5;
	
	xTarget = ofGetWindowWidth()*0.5;
	yTarget = ofGetWindowHeight()*0.5;
}

void Figure::chooseTarget(){
	xTarget = ofRandom(ofGetWindowWidth());
	yTarget = ofRandom(ofGetWindowHeight());
}

void Figure::moveTo(){
	xPosition += (xTarget - xPosition)*input;
	yPosition += (yTarget - yPosition)*input;
}
void Figure::update(){
	if(abs(xPosition-xTarget)/ofGetWindowWidth()<0.1 && abs(yPosition-yTarget)/ofGetWindowHeight()<0.1){
		if(ofRandom(5)==1){
			chooseTarget();
		}
		else{
			xTarget = ofGetWindowWidth()*0.5;
			yTarget = ofGetWindowHeight()*0.5;	
		}
	}
	moveTo();
	
	
	color.setSaturation(ofMap(input, 0.0, 1.0, 127.0, 64.0, true));
	color.setBrightness(ofMap(input, 0.0, 1.0, 127.0, 255.0, true));
	if(lifespan>0.01){
		lifespan -= lifespan*input;
	}
	else {
		lifespan=0.0;
	}

}
void Figure::draw(){
	if(lifespan>0.0){
		ofSetColor(color);
		ofFill();
		ofCircle(xPosition, yPosition, 0.0, ofMap(input, 0.0, 1.0, 1.0+(min(ofGetWindowWidth(),ofGetWindowHeight())/4*input), min(ofGetWindowWidth(),ofGetWindowHeight()/2)*input));
	}
}