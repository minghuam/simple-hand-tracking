//
//  hdskin.hpp
//  hand_gesture_demo
//
//  Hand detector using skin detection
//  http://spottrlabs.blogspot.com/2012/01/super-simple-skin-detector-in-opencv.html
//  http://www.morethantechnical.com/2013/03/05/skin-detection-with-probability-maps-and-elliptical-boundaries-opencv-wcode/
//  Created by Minghuang Ma on 9/16/14.
//  Copyright (c) 2014 Minghuang Ma. All rights reserved.
//

#ifndef hand_gesture_demo_HDskin_hpp
#define hand_gesture_demo_HDskin_hpp

#include <opencv2/opencv.hpp>
#include <vector>
#include "handdetector.h"

class HDSkin: public HandDetector{
private:
    cv::Mat skin_crcb_hist;
    cv::Mat ycrcb_img;
    
public:
    HDSkin(){
        skin_crcb_hist = cv::Mat::zeros(cv::Size(256,256), CV_8UC1);
        cv::ellipse(skin_crcb_hist, cv::Point(113, 155), cv::Size(23, 15), 43, 0, 360, cv::Scalar(255, 255, 255), -1);
    }
    ~HDSkin(){
        
    }

    void detect(cv::Mat &raw_img, cv::Mat &hand_img, cv::Mat &fg_mask, vector<vector<cv::Point> > &contours, int param){
        if(fg_mask.empty()){
            fg_mask = cv::Mat::zeros(raw_img.size(), CV_8UC1);
        }
        // BGR to YCrCb
        cv::cvtColor(raw_img, ycrcb_img, CV_BGR2YCrCb);
        
        // skin detection
        for(int i=0; i<raw_img.cols; i++){
            for(int j=0; j<raw_img.rows; j++){
                cv::Vec3b ycrcb = ycrcb_img.at<cv::Vec3b>(j,i);
                fg_mask.at<uchar>(j,i) = skin_crcb_hist.at<uchar>(ycrcb[1], ycrcb[2]) > 0 ? 255 : 0;
            }
        }
        
        // remove small objects
        cv::erode(fg_mask, fg_mask, cv::Mat::ones(5, 5, CV_8U));
        cv::dilate(fg_mask, fg_mask, cv::Mat::ones(5, 5, CV_8U));
        
        // fill small holes
        cv::dilate(fg_mask, fg_mask, cv::Mat::ones(5, 5, CV_8U));
        cv::erode(fg_mask, fg_mask, cv::Mat::ones(5, 5, CV_8U));
        
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
