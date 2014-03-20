//
//  feature.h
//  afv
//
//  Created by Owen Campbell on 3/17/14.
//
//

#ifndef __afv__feature__
#define __afv__feature__




#include <iostream>
#include <vector>
#include <cmath>
#endif /* defined(__afv__feature__) */

#ifndef CLAMP
#define CLAMP(val,min,max) ((val) < (min) ? (min) : ((val > max) ? (max) : (val)))
#endif

class Feature{
public:
    std::string name;
    std::vector <float> history;
    float smoothed;
    float scaled;
    float minimum;
    float maximum;
    float longAvg;
    float shortAvg;

    Feature(){
        history.resize(0);
    };
    ~Feature(){

    };
    
    void init(std::string n, const float min, const float max){
        name = n;
        history.resize(400, 0.0);
        minimum = min;
        maximum = max;
    }
    void update(const float v){
        //smoothed holds an unscaled quasi-average value of the feature measurement
        smoothed *= 0.93;
        smoothed += 0.07 * v;
        int i = 1;
        while(smoothed > maximum){
            smoothed /= pow(2.0, i);
        }
        //scaled holds a clamped, normalized version of the smoothed value which will be stored in the history
        scaled = map(smoothed, minimum, maximum, 0.0, 1.0, true);
        
        
        history.push_back(scaled);
        //if we are bigger the the size we want to record - lets drop the oldest values
        if( history.size() >= 800 ){
            history.erase(history.begin(), history.begin()+400);
        }
        int size = history.size();
        longAvg = getAvg((int)(size * 0.3), size);
        shortAvg = getAvg((int)(size * 0.9), size);
    }
    float getAvg(int start, int size){
        float sum = 0.0;
        if(size <= 1){
            return 0.0;
        }
        for (int i = start; i < size; ++i) {
            sum += history[i];
        }
        return sum / size;
    }
    
    float map(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp){
        float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
        if( clamp ){
            if(outputMax < outputMin){
                if( outVal < outputMax )outVal = outputMax;
                else if( outVal > outputMin )outVal = outputMin;
            }else{
                if( outVal > outputMax )outVal = outputMax;
                else if( outVal < outputMin )outVal = outputMin;
            }
        }
        return outVal;
    }
};