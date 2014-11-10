//
//  hdbg.hpp
//  hand_gesture_demo
//  Hand detection based on background subtraction
//  http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html
//  Created by Minghuang Ma on 9/15/14.
//  Copyright (c) 2014 Minghuang Ma. All rights reserved.
//

#ifndef hand_gesture_demo_HDBackgroundSubtraction_hpp
#define hand_gesture_demo_HDBackgroundSubtraction_hpp

#include <opencv2/opencv.hpp>
#include <vector>
#include "handdetector.h"

using namespace std;

class HDBackgroundSubtraction : public HandDetector{
private:
    cv::BackgroundSubtractorMOG2 mog2;
    
public:
    
    HDBackgroundSubtraction(){
    }
    
    ~HDBackgroundSubtraction(){
    }
    
    void learn(cv::Mat &raw_img, cv::Mat &hand_img){
        this->mog2.operator()(raw_img, hand_img);
    }
    
    void detect(cv::Mat &raw_img, cv::Mat &hand_img, cv::Mat &fg_mask, vector<vector<cv::Point> > &contours, int param){
        
        // smooth raw image
        cv::GaussianBlur(raw_img, raw_img, cv::Size(7,7), 1, 1);
        
        // background subtraction
        this->mog2(raw_img, fg_mask, param);
        
        // threshold
        cv::threshold(fg_mask, fg_mask, HAND_MASK_THRESHOLD, 255, CV_THRESH_BINARY);
        
        // remove small objects
        cv::erode(fg_mask, fg_mask, cv::Mat::ones(5, 5, CV_8U));
        cv::dilate(fg_mask, fg_mask, cv::Mat::ones(5, 5, CV_8U));
        
        // fill small holes
        cv::dilate(fg_mask, fg_mask, cv::Mat::ones(9, 9, CV_8U));
        cv::erode(fg_mask, fg_mask, cv::Mat::ones(9, 9, CV_8U));
        
        // smooth
        cv::medianBlur(fg_mask, fg_mask, 5);
        
        // apply mask
        hand_img = cv::Scalar::all(0);
        raw_img.copyTo(hand_img, fg_mask);
        
        // find contours
        cv::Mat m = fg_mask.clone();
        cv::findContours(m, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
        
        // remove small contours
        for(vector<vector<cv::Point> >::iterator it=contours.begin(); it!=contours.end();){
            if(cv::contourArea(*it) < MIN_HAND_CONTOUR_AREA){
                it = contours.erase(it);
            }else{
                ++it;
            }
        }
        
    }
};

#endif
