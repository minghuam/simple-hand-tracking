//
//  main.cpp
//  hand_gesture_demo
//
//  Created by Minghuang Ma on 9/15/14.
//  Copyright (c) 2014 Minghuang Ma. All rights reserved.
//

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <dirent.h>
#include "handdetector.h"
#include "hdbg.hpp"
#include "hdskin.hpp"
#include "../libsvm/svm.h"
#include "hogsvmdetector.hpp"
#include <stdio.h>
using namespace std;

void print_help(){
    cout << "HOG-SVM based hand tracking demo.\n" << \
            "   [*] press 'd' to toggle debug mode.\n" << \
            "   [*] press 'b' to update background model.\n" << \
            "   [*] press ESC to exit.\n" << endl;
}

int main(int argc, const char * argv[])
{
    print_help();

    const char *SVM_MODEL_FILE = "./hand.train.model";
    const char *SVM_RANGE_FILE = "./hand.train.range";
    
    // setup camera
    cv::VideoCapture cap;
    if(!cap.open(0)){
        fprintf(stderr, "Failed to open camera 0...");
        return -1;
    }
    cap.set(CV_CAP_PROP_FRAME_WIDTH, CAM_WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, CAM_HEIGHT);
    cap.set(CV_CAP_PROP_AUTO_EXPOSURE, 0);
    
    // skin detection works better...
    HDBackgroundSubtraction hd_bg;
    HDSkin hd_skin;
    HandDetector *hd = &hd_skin;
    
    // hog & svm
    HOGSVMDetector hs(SVM_MODEL_FILE, SVM_RANGE_FILE);
    
    vector<vector<cv::Point> > contours;
    bool update_bg_model = true;
    bool running = true;
    bool debug = true;
    int detect_cnt = 0;
    int detect_cnt_threshold = 5;
    int count_down_count = 0;
    
    cv::Mat raw_img;
    cv::Mat hand_img;
    cv::Mat fg_mask;
    cv::Mat pic = cv::Mat::zeros(CAM_HEIGHT, CAM_WIDTH, CV_8UC1);

    
    while(running){
        if(!cap.read(raw_img)){
            fprintf(stderr, "Failed to read from camera...");
            return -1;
        }
        
        cv::flip(raw_img, raw_img, 1);
        
        hd->detect(raw_img, hand_img, fg_mask, contours, update_bg_model ? -1 : 0);
        
        // calculate centroids
        vector<cv::Moments> mu(contours.size());
        vector<cv::Point2f> mc(contours.size());
        for(int i=0; i<contours.size(); i++){
            mu[i] = cv::moments(contours[i], false);
            mc[i] = cv::Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
            cv::Rect bounding_rect = cv::boundingRect(contours[i]);
            if(debug) cv::rectangle(hand_img, bounding_rect.tl(), bounding_rect.br(), cv::Scalar(0,255,0), 2);
        }
        //cv::drawContours(hand_img, contours, -1, cv::Scalar(0,0,255));
        
        // detect hand
        for(int i=0; i<mc.size(); i++){
            double l = hs.detect_multi(raw_img, hand_img, cv::Point(mc[i].x, mc[i].y), debug);
            if(l == 1.0){
                detect_cnt++;
            }
        }
        if(detect_cnt >= detect_cnt_threshold){
            // start count down
            if(count_down_count == 0){
                count_down_count = 29;
            }
            detect_cnt = 0;
        }
        if(count_down_count > 0){
            // countdown
            char buf[8];
            sprintf(buf, "%d", count_down_count/10 + 1);
            cv::putText(hand_img, buf, cv::Point(hand_img.cols/2-20,hand_img.rows/2), cv::FONT_HERSHEY_SIMPLEX, 5, cv::Scalar(0,255,100), 20);
            count_down_count--;
            if(count_down_count == 0){
                // take a snapshot
                pic = raw_img.clone();
            }
        }

        
        cv::imshow("hand", hand_img);
        cv::imshow("pic", pic);
        
        cv::Mat cropped_img;
        switch(cv::waitKey(30) & 255){
            case 27:
                //exit
                running = false;
                break;
            case 'b':
                update_bg_model = !update_bg_model;
                cout << "update_bg_model = " << update_bg_model << endl;
                break;
            case 'd':
                debug = !debug;
                break;
            default:
                break;
        }
    }
    //clean up
    
    return 0;
}

