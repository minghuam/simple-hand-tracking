//
//  hogsvmdetector.hpp
//  hand_gesture_demo
//
//  Created by Minghuang Ma on 9/16/14.
//  Copyright (c) 2014 Minghuang Ma. All rights reserved.
//

#ifndef hand_gesture_demo_hogsvmdetector_hpp
#define hand_gesture_demo_hogsvmdetector_hpp

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include "../libsvm/svm.h"

class HOGSVMDetector{
private:
    cv::HOGDescriptor hog;
    struct svm_model *s_model;
    struct svm_node *s_node;
    vector<cv::Vec2f> s_range;
    double feat_val_min;
    double feat_val_max;
    
    int hog_win_width;
    int hog_win_height;
    int hog_descriptor_size;
    float roi_scale;
    
    inline bool in_rect(cv::Rect a, cv::Rect b){
        return (a.x >= b.x) && (a.x+a.width <= b.x+b.width) && (a.y >= b.y) && (a.y+a.height <= b.y+b.height);
    }
    
    void load_range(const char *range_file, vector<cv::Vec2f> &range){
        ifstream fs(range_file);
        string line;
        if(fs.is_open()){
            getline(fs, line);  // x
            getline(fs, line);  // min, max
            sscanf(line.c_str(), "%lf %lf\n", &(this->feat_val_min), &(this->feat_val_max));
            int id;
            float min_v, max_v;
            while(getline(fs, line)){
                sscanf(line.c_str(), "%d %f %f\n", &id, &min_v, &max_v);
                range.push_back(cv::Vec2f(min_v, max_v));
            }
            fs.close();
        }
    }
    
public:
    HOGSVMDetector(const char* svm_model_file, const char* svm_range_file, int hog_win_width = 64, \
                   int hog_win_height = 128, int hog_descriptor_size = 3780+1, float roi_scale = 1.2f) \
    {
        this->hog_win_width = hog_win_width;
        this->hog_win_height = hog_win_height;
        this->hog_descriptor_size = hog_descriptor_size;
        this->roi_scale = roi_scale;
        
        // setup svm
        s_model = svm_load_model(svm_model_file);
        s_node = new svm_node[hog_descriptor_size];
        s_node[hog_descriptor_size-1].index = -1;
        this->load_range(svm_range_file, s_range);
        
        // setup hog
        cv::HOGDescriptor hog;
        hog.winSize = cv::Size(hog_win_width, hog_win_height);
    }
    
    ~HOGSVMDetector(){
        // clearn up
        delete [] this->s_node;
        svm_free_and_destroy_model(&this->s_model);
    }
    
    double detect(cv::Mat &raw_img, cv::Mat &hand_img, cv::Point hand_pos, bool draw = true, double label_to_draw = 1.0){
        // start with minimum roi
        int start_w = hog_win_width*2;
        int start_h = hog_win_height*2;
        cv::Rect roi = cv::Rect(hand_pos.x - start_w/2, hand_pos.y - start_h/2, start_w, start_h);
        cv::Mat roi_img;
        cv::Mat gray_img;
        vector<float> features;
        double label = 0.0;
        double ret = -1.0;
        while(in_rect(roi, cv::Rect(0,0,raw_img.cols, raw_img.rows))){
            if(draw){
                // draw roi
                cv::rectangle(hand_img, roi.tl(), roi.br(), cv::Scalar(255,255,0));
            }
            // copy roi image
            roi_img = raw_img(roi).clone();
            // scale roi image
            cv::resize(roi_img, roi_img, cv::Size(this->hog_win_width, this->hog_win_height));
            // covert to gray
            cv::cvtColor(roi_img, gray_img, CV_BGR2GRAY);
            
            // calc hog features
            features.clear();
            hog.compute(gray_img, features);
            
            // scale hog features to range
            float max_range = this->feat_val_max - this->feat_val_min;
            for(int i=0; i<features.size(); i++){
                this->s_node[i].index = i;
                this->s_node[i].value = this->feat_val_min + max_range*(features[i] - this->s_range[i][0])/(s_range[i][1] - s_range[i][0]);
            }
            
            // predict
            label = svm_predict(this->s_model, this->s_node);
            if(label == label_to_draw) ret = label_to_draw;
            
            if(draw){
                if(label == label_to_draw){
                    cv::rectangle(hand_img, roi.tl(), roi.br(), cv::Scalar(0,0,255),3);
                }
            }
            
            // calc next roi
            int cx = roi.x + roi.width/2;
            int cy = roi.y + roi.height/2;
            int w = (int)(roi.width*this->roi_scale);
            int h = (int)(roi.height*this->roi_scale);
            roi = cv::Rect(cx - w/2, cy - h/2, w, h);
        }

        return ret;
    }
    
    double detect_multi(cv::Mat &raw_img, cv::Mat &hand_img, cv::Point hand_pos, bool draw = true, double label_to_draw = 1.0){
        // offset hand location in four directions
        vector<cv::Point> points;
        cv::Point c = hand_pos;
        points.push_back(c);
        points.push_back(cv::Point(c.x - this->hog_win_width/2, c.y));
        points.push_back(cv::Point(c.x + this->hog_win_width/2, c.y));
        points.push_back(cv::Point(c.x, c.y - this->hog_win_height/2));
        points.push_back(cv::Point(c.x, c.y + this->hog_win_height/2));
        double ret = -1.0;
        for(int i=0;i<points.size();i++){
            double label = detect(raw_img, hand_img, points[i], draw, label_to_draw);
            if(label == label_to_draw) ret = label_to_draw;
        }
        return ret;
    }
};

#endif
