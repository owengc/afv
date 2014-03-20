/*
 *  figure.cpp
 *  audioInputExample
 *
 *  Created by Owen Campbell on 7/5/12.
 *  Copyright 2012 tick tock audio. All rights reserved.
 *
 */

#include "figure.h"

Figure::Figure(const float l, const float s, const float t, ofxVectorGraphics * vg, ofxAudioFeaturesChannel * fc){
    vectorGraphics = vg;
	featuresChannel = fc;
	ofSeedRandom();
	lifespan = l * 100.0;
    size = s * 10.0;
    radius = s;
    texture = t;
	color.setHsb(ofRandom(255),ofRandom(255),ofRandom(255), lifespan * 2);
	color.setSaturation(ofMap(size, 0.0, 1.0, 127.0, 64.0, true));
	color.setBrightness(ofMap(size, 0.0, 1.0, 127.0, 255.0, true));

    xOrigin = ofGetWindowWidth()*0.5;
    yOrigin = ofGetWindowHeight()*0.5;
    
	xPosition = xOrigin;
	yPosition = yOrigin;
	
	xTarget = xOrigin;
	yTarget = yOrigin;
}

void Figure::chooseTarget(){
    float t = 2.0 * (float)M_PI * ofRandomf();
    float u = ofRandomf() + ofRandomf();
    float r = (u > 1)?2 - u:u;
    r *= ofGetWindowWidth();
	xTarget = xOrigin + (r * cos(t));
	yTarget = yOrigin + (r * sin(t));
}

void Figure::moveTo(){
	xPosition += (xTarget - xPosition)*size;
	yPosition += (yTarget - yPosition)*size;
}
void Figure::update(const float t, const float x){
    if(lifespan > 0.01){
		lifespan -= lifespan * size * 10 * x;
        radius = (lifespan * yOrigin) * size * x;//making this proportional to life span
        texture = t;//updating this continuously

	}
	else {
		lifespan = 0.0;
        return;
	}
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
	
}
void Figure::draw(){
	if(lifespan > 0.0){
		ofSetColor(color);//change back to white at some point
		ofFill();
        ofCircle(xPosition, yPosition, 0.0, radius);
        /*if(radius > 1.0){
            
            vectorGraphics->setColor(color.getHex());
            vectorGraphics->beginShape();
            
            int numSteps = texture * 24.0;
            
            //make sure we don't go bellow 3 sides
            numSteps = MAX(3, numSteps);
            
            float step		= TWO_PI / (numSteps);
            float angle		= 2.0 * (float)M_PI * ofRandomf();
            float scale = 1.0;// + input
            
            for(int i = 0; i < numSteps; i++){
                float rx = xPosition + cos(angle) * radius;
                float ry = yPosition + sin(angle) * radius;
                
                if(i == 0){
                    vectorGraphics->polyVertex(rx, ry);
                }
                
                float rx2 = xPosition + cos(angle+step) * radius;
                float ry2 = yPosition + sin(angle+step) * radius;
                
                //lets make our control points in between each side and out a little way
                float cx = xPosition + cos(angle + step*0.5) * radius * scale;
                float cy = yPosition + sin(angle + step*0.5) * radius * scale;
                
                vectorGraphics->bezierVertex(cx, cy, cx, cy, rx2, ry2);
                
                angle += step;
            }
            vectorGraphics->endShape(true);
        }*/
	}
}