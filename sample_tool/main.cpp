//
//  main.cpp
//  sample_tool
//
//  Created by Minghuang Ma on 9/16/14.
//  Copyright (c) 2014 Minghuang Ma. All rights reserved.
//

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

int main(int argc, const char * argv[])
{
    const int CAM_WIDTH = 640;
    const int CAM_HEIGHT = 480;
    const int FEAT_WIDTH = 64;
    const int FEAT_HEIGHT = 128;
    
    const std::string RAW_WINDOW = "Raw";
    string RAW_PATH = "./raw";
    
    cv::VideoCapture cap;
    if(!cap.open(0)){
        fprintf(stderr, "Failed to open camera 0...");
        return -1;
    }
    cap.set(CV_CAP_PROP_FRAME_WIDTH, CAM_WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, CAM_HEIGHT);
    cap.set(CV_CAP_PROP_AUTO_EXPOSURE, 0);
    
    cv::Mat raw_img;
    cv::Mat gui_img;
    cv::Mat feat_img;
    
    double ratio = (double)CAM_HEIGHT/(double)FEAT_HEIGHT;
    int crop_width = round(ratio*FEAT_WIDTH);
    cv::Rect roi = cv::Rect(CAM_WIDTH - crop_width, 0, crop_width, CAM_HEIGHT);
    int save_img_cnt = 0;
    
    bool running = true;
    while(running){
        if(!cap.read(raw_img)){
            fprintf(stderr, "Failed to read from camera...");
            return -1;
        }
        cv::flip(raw_img, raw_img, 1);
        gui_img = raw_img.clone();
        cv::rectangle(gui_img, roi, cv::Scalar(0,255,0));
        
        raw_img(roi).copyTo(feat_img);
        cv::resize(feat_img, feat_img, cv::Size(FEAT_WIDTH, FEAT_HEIGHT));
        
        char buf[128];
        switch(cv::waitKey(30) & 255){
            case 27:    //exit
                running = false;
                break;
            case 's':   //save
                sprintf(buf, "%d.jpg", save_img_cnt++);
                string filename = buf;
                filename = RAW_PATH + "/" + filename;
                cout << "saving: " << filename << endl;
                cv::imwrite(filename, feat_img);
                
                break;
        }
        
        cv::imshow(RAW_WINDOW, gui_img);
    }
    
    //clean up
    
    return 0;
}


