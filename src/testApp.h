#pragma once

#include "ofMain.h"
#include "ofxAudioFeaturesChannel.h"
#include "ofxAudioDeviceControl.h"
#include "ofxVectorGraphics.h"
#include "ofxUI.h"
#include "figure.h"
#include <ctime>

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
    void audioIn(float * input, int bufferSize, int nChannels);
    float getAvg(int start, vector<float> * data);
    
    ofColor bgColor1;
    ofColor bgColor2;
    ofxVectorGraphics vectorGraphics;
    vector <ofPoint> pts;
    
    ofSoundStream soundStream;
    ofxAudioFeaturesChannel featuresChannel;
    vector <float> left;
    vector <float> right;
    vector<Figure> figures;
    
    string featureName;

    
    float curVolume;
    float scaledFeatureVal;
    
    const string lifespanFeatureName = "specflux";//"energy";
    const string onsetFeatureName = "specflux";
    const string movementFeatureName = "specflux";//"hfc";
    const string sizeFeatureName = "specflux";//"energy";
    const string colorFeatureName = "decrease";
    const string textureFeatureName = "spread";
    
    int lifespanFeatureIdx;
    int onsetFeatureIdx;
    int movementFeatureIdx;
    int sizeFeatureIdx;
    int colorFeatureIdx;
    int textureFeatureIdx;
    
    float lifespanFeatureVal;
    float sizeFeatureVal;
    float colorFeatureVal;
    float textureFeatureVal;
    float movementFeatureVal, movementFeatureLongAvg, movementFeatureShortAvg;
};
