//
//  handdetector.h
//  hand_gesture_demo
//
//  Created by Minghuang Ma on 9/16/14.
//  Copyright (c) 2014 Minghuang Ma. All rights reserved.
//

#ifndef hand_gesture_demo_handdetector_h
#define hand_gesture_demo_handdetector_h

#include "config.h"
#include <vector>

using namespace std;

class HandDetector{
protected:
    
public:
    virtual void detect(cv::Mat &raw_img, cv::Mat &hand_img, cv::Mat &fg_mask, vector<vector<cv::Point> > &contours, int param) = 0;
};


#endif
