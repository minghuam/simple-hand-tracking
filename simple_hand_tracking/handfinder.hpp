//
//  handfinder.hpp
//  simple_hand_tracking
//
//  Created by Minghuang Ma on 9/17/14.
//  Copyright (c) 2014 Minghuang Ma. All rights reserved.
//

#ifndef simple_hand_tracking_handfinder_hpp
#define simple_hand_tracking_handfinder_hpp

#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

class HandFinder{
private:
    cv::CascadeClassifier classifier;
    
    cv::Rect detect_max_obj(cv::Mat& img, cv::CascadeClassifier& classifier, double scale){
        vector<cv::Rect> objs;
        vector<cv::Rect> objs_src;
        cv::Mat gray;
        cv::Mat small_img(cvRound(img.rows/scale), cvRound(img.cols/scale), CV_8UC1);
        cv::cvtColor(img, gray, CV_BGR2GRAY);
        cv::resize(gray, small_img, small_img.size(), 0, 0, cv::INTER_LINEAR);
        cv::equalizeHist(small_img, small_img);
        
        classifier.detectMultiScale(small_img, objs, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE|CV_HAAR_FIND_BIGGEST_OBJECT, cv::Size(30,30));
        if(objs.size()>0)
        {
            cv::Rect r = objs[0];
            cv::Rect rect(cvRound(r.x*scale), cvRound(r.y*scale), cvRound(r.width*scale), cvRound(r.height*scale));
            return rect;
        }
        return cv::Rect(0,0,0,0);
    }
    
public:
    HandFinder(string training_file){
        if(!this->classifier.load(training_file)){
            cout << "failed to load training file" << endl;
        }
    }
    
    ~HandFinder(){}
    
    cv::Rect find(cv::Mat &raw){
        cv::Mat half;
        cv::resize(raw, half, cv::Size(raw.cols/2, raw.rows/2));
        cv::Rect ret = this->detect_max_obj(half, this->classifier, 1.0);
        return cv::Rect(ret.tl()*2, ret.br()*2);
    }
};

#endif
