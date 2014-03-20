#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	//graphics
	ofEnableSmoothing();
    ofEnableAlphaBlending();
	ofSetFrameRate(30);
	ofSeedRandom();
	
	ofSetVerticalSync(true);
	ofSetCircleResolution(80);
    
	bgColor1.setHsb(0,0,0);
	bgColor2.setHsb(0,1,50);
	
	// 0 output channels,
	// 2 input channels
	// 44100 samples per second
	// 256 samples per buffer
	// 4 num buffers (latency)
	
	soundStream.listDevices();
	
	//if you want to set a different device id
	//soundStream.setDeviceID(0); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
	
	int bufferSize = 2048;
	
	
	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	
	soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
	featuresChannel.setup(bufferSize, bufferSize / 2, 44100);
    //get indexes for feature names specified in header file
    for(int i = 0; i < featuresChannel.usingFeatures.size(); ++i){
        string name = featuresChannel.usingFeatures[i].name;
        if(name == lifespanFeatureName){
            lifespanFeatureIdx = i;
        }
        if(name == onsetFeatureName){
            onsetFeatureIdx = i;
        }
        if(name == movementFeatureName){
            movementFeatureIdx = i;
        }
        if(name == sizeFeatureName){
            sizeFeatureIdx = i;
        }
        if(name == colorFeatureName){
            colorFeatureIdx = i;
        }
        if(name == textureFeatureName){
            textureFeatureIdx = i;
        }
    }
}

//--------------------------------------------------------------
void testApp::update(){
	//first we'll update our audio features
    for(int i = 0; i < featuresChannel.usingFeatures.size(); ++i){
        //each feature's history will be updated with the latest smoothed & scaled measurement
        featureName = featuresChannel.usingFeatures[i].name;
        float featureValue = (float)*fvec_get_data(featuresChannel.spectralFeatureOutputBuffer[featureName]);
        featuresChannel.usingFeatures[i].update(featureValue);
        scaledFeatureVal = featuresChannel.usingFeatures[i].scaled;
        //std::cout << "feature " << i << " value: " << scaledFeatureVal << std::endl;
        //now store particular values with which to initiate new figures
        if(i == lifespanFeatureIdx){
            lifespanFeatureVal = scaledFeatureVal;
        }
        if(i == sizeFeatureIdx){
            sizeFeatureVal = scaledFeatureVal;
        }
        if(i == colorFeatureIdx){
            colorFeatureVal = scaledFeatureVal;
        }
        if(i == textureFeatureIdx){
            textureFeatureVal = scaledFeatureVal;
        }
    }
    //here we'll check if we should create new figures based on the onset feature
    Feature * onsetFeature = &featuresChannel.usingFeatures[onsetFeatureIdx];
    if(onsetFeature->shortAvg + onsetFeature->scaled > onsetFeature->longAvg || figures.size() == 0){
        int numNewFigures = 1 + (int) onsetFeature->smoothed * curVolume;
        for(int i = 0; i < numNewFigures && i < 100; i++){//setting max threshold for number of new figures per frame
            Figure *newFigure = new Figure(lifespanFeatureVal, sizeFeatureVal, textureFeatureVal,
                                           &vectorGraphics, &featuresChannel);
            figures.push_back(*newFigure);
            std::cout << "drawing " << figures.size() << " figures. last size: " << sizeFeatureVal<< std::endl;
        }
    }
    //now we'll update the figures
    int j = 0;
    while(j < figures.size()){
        //cull dead figures
        if(figures[j].lifespan == 0.0){
            figures.erase(figures.begin() + j);
        }
        else{
            //update living figures
            
            if(onsetFeature->scaled > abs(onsetFeature->longAvg - onsetFeature->shortAvg) * 1.75){//scaledFeature * curVolume
                figures[j].chooseTarget();
            }
            figures[j].update(textureFeatureVal, curVolume);
            j++;
        }
    }
    /*
     if(i == onsetFeatureIdx){
     figures[j].input = curVolume / scaledFeature;
     //std::cout << "onset feature: " << scaledFeature << std::endl;
     if(scaledFeature > abs(longAvg - shortAvg) * 1.75){//scaledFeature * curVolume
     figures[j].chooseTarget();
     }
     figures[j].update();//make sure this only gets called for one feature
     }
     if(i == colorFeatureIdx && scaledFeature < longAvg * 0.1) {
     figures[j].color.setHue(ofRandom(255));
     }*/
}



//--------------------------------------------------------------
void testApp::draw(){
	ofBackgroundGradient(bgColor1, bgColor2, OF_GRADIENT_CIRCULAR);
	for (int i = 0; i < figures.size(); ++i) {
		figures[i].draw();
	}
}

//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels){
    
    curVolume = 0.0;
    
    // samples are "interleaved"
    int numCounted = 0;
    float monoSample = 0.0;
    for(int i = 0; i < bufferSize; ++i){
        monoSample += input[i*2] * 0.5;
        monoSample += input[i*2+1] * 0.5;
        curVolume = monoSample * monoSample;
        numCounted+=2;
        featuresChannel.inputBuffer[i] = monoSample;
        //std::cout << monoSample << std::endl;
    }
    //this is how we get the mean of rms :)
    curVolume /= (float)numCounted;
    
    // this is how we get the root of rms :)
    curVolume = sqrt( curVolume );
    curVolume = (curVolume > 1.0)?1.0:curVolume;
    featuresChannel.process(time(0));
    //smpl_t * featureValue = fvec_get_data(featuresChannel.spectralFeatureOutputBuffer["specflux"]);
    //std::cout << "Feature measurement: " << *featureValue << std::endl;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if( key == 's' ){
		soundStream.start();
	}
	
	if( key == 'e' ){
		soundStream.stop();
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
