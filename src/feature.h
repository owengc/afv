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
#endif /* defined(__afv__feature__) */


class Feature{
public:
    std::string name;
    std::vector <float> history;
    float smoothed;
    float scaled;
    float minimum;
    float maximum;

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
        //lets record the volume into an array
        history.push_back( v );
        //if we are bigger the the size we want to record - lets drop the oldest value
        if( history.size() >= 1200 ){
            history.erase(history.begin(), history.begin()+1);
        }
    }
    
};