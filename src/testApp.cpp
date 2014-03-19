#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	//graphics
	ofEnableSmoothing();
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
	
    
	smoothedFeature     = 0.0;
	scaledFeature		= 0.0;
    
	soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
	featuresChannel.setup(bufferSize, bufferSize / 2, 44100);

    
	Figure newFigure (1.0);
	figures.push_back(newFigure);
}

//--------------------------------------------------------------
void testApp::update(){
    /*
     
     //float featureValue = (float)aubio_bintofreq(fvec_read_sample(featuresChannel.spectralFeatureOutputBuffer["centroid"], 0), 44100, 2048);
     //float featureValue = (float)fvec_read_sample(featuresChannel.spectralFeatureOutputBuffer["decrease"], 0);
     //featureValue = CLAMP(featureValue, featureMin, featureMax);
     smoothedFeature += 0.07 * featureValue;
     //scaled = ofMap(smoothedFeature, minimum, maximum, 0.0, 1.0, true);
     
     //smpl_t featureValue = featuresChannel.ons
     std::cout << "Smoothed feature measurement: " << featureValue << std::endl;
     */
	//lets scale the vol up to a 0-1 range
    for(int i = 0; i < featuresChannel.usingFeatures.size(); ++i){

        featureName = featuresChannel.usingFeatures[i].name;
        featureMin = featuresChannel.usingFeatures[i].minimum;
        featureMax = featuresChannel.usingFeatures[i].maximum;
        featureValue = *fvec_get_data(featuresChannel.spectralFeatureOutputBuffer[featureName]);
        featureValue = CLAMP(featureValue, featureMin, featureMax);

        featuresChannel.usingFeatures[i].smoothed *= 0.93;
        featuresChannel.usingFeatures[i].smoothed += 0.07 * featureValue;
        smoothedFeature = featuresChannel.usingFeatures[i].smoothed;
        scaledFeature = ofMap(smoothedFeature, featureMin, featureMax, 0.0, 1.0, true);
        featuresChannel.usingFeatures[i].update(scaledFeature);
        
        if(i == 0 || i == 1){//0th feature is 'onset' feature, use it to control figures
            vector<float> * featureHistory = &featuresChannel.usingFeatures[i].history;
            float longAvg = getAvg(0, featureHistory);
            float shortAvg = getAvg((int)(featureHistory->size() * 0.95), featureHistory);
            if(i == 0 && (shortAvg + scaledFeature > longAvg || figures.size() == 0)){
                Figure *newFigure = new Figure (scaledFeature);
                figures.push_back(*newFigure);
            }
            //update figures
            int j = 0;
            while(j < figures.size()){
                if(figures[j].lifespan == 0.0){
                    figures.erase(figures.begin() + j);
                }
                else{
                    figures[j].input = scaledFeature;
                    if(i == 0 && scaledFeature > longAvg * 1.75){
                        figures[j].chooseTarget();
                    }
                    if(i== 1  && scaledFeature < longAvg * 0.1) {
                        figures[j].color.setHue(ofRandom(255));
                    }
                    if(i == 0){
                        figures[j].update();
                    }
                    j++;
                }
            }
        }
    }
    //smpl_t * featureValue = fvec_get_data(featuresChannel.spectralFeatureOutputBuffer["energy"]);
    //std::cout << "Feature measurement: " << *featureValue << std::endl;
}

float testApp::getAvg(int start, vector<float> * data){
	float sum = 0.0;
    int size = data->size();
	for (int i = start; i < size; ++i) {
		sum += data->operator[](i);
	}
	return sum / size;
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
    
    float curVolume = 0.0;
    
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
    featuresChannel.process(time(0));
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
