//
//  handtracker.hpp
//  simple_hand_tracking
//
//  Created by Minghuang Ma on 9/17/14.
//  Copyright (c) 2014 Minghuang Ma. All rights reserved.
//

#ifndef simple_hand_tracking_handtracker_hpp
#define simple_hand_tracking_handtracker_hpp

#include <vector>
#include <list>
#include <opencv2/opencv.hpp>
#include "onedollar.hpp"

using namespace std;

#define MAX_LOSING_FRAMES 5
#define MAX_FRAME_DISTANCE 0.5f
#define SMOOTH_COEFFICIENT 0.5f
#define MAX_POINTS 128

#define DISTANCE(x1,y1,x2,y2) (sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)))

class HandTracker{
private:
    /*
    struct Point{
        float x;
        float y;
        Point() { Point(0.0f,0.0f); }
        Point(float _x,float _y):x(_x),y(_y){}
    };
    */
    
    enum state_t {LOST, TRACKING, LOSING} state;
    list<cv::Point2f> points;
    
    int recognize_guesture(){
        int gesture = 0;
        
        
        
        return gesture;
    }
    
public:
    HandTracker(){
        state = LOST;
    }
    
    list<cv::Point2f> get_current_trace(){
        return points;
    }
    
    int track(float x, float y){
        int ret = 0;
        static int losing_frames = MAX_LOSING_FRAMES;
        static cv::Point2f last_pt;
        
        switch(this->state){
            case LOST:
                if(x >= 0 && y >= 0){
                    //cout << "found object at: " << x << "," << y << endl;
                    if(points.size() == MAX_POINTS) points.pop_front();
                    points.push_back(cv::Point2f(x,y));
                    last_pt = cv::Point2f(x,y);
                    this->state = TRACKING;
                }else{
                    points.clear();
                }
                break;
            case TRACKING:
                if(x >= 0 && y >= 0){
                    // new points
                    if(DISTANCE(x,y,last_pt.x,last_pt.y) > MAX_FRAME_DISTANCE){
                        cout << "too much movement, lost!" << endl;
                        this->state = LOST;
                        points.clear();
                    }else{
                        // smooth
                        x = x*SMOOTH_COEFFICIENT + last_pt.x*(1-SMOOTH_COEFFICIENT);
                        y = y*SMOOTH_COEFFICIENT + last_pt.y*(1-SMOOTH_COEFFICIENT);
                        if(points.size() == MAX_POINTS) points.pop_front();
                        points.push_back(cv::Point2f(x,y));
                        last_pt = cv::Point2f(x,y);
                        ret = recognize_guesture();
                    }
                }else{
                    //cout << "losing object..." << endl;
                    this->state = LOSING;
                    points.clear();
                }
                break;
            case LOSING:
                if(x >= 0 && y >= 0){
                    // back to tracking
                    losing_frames = MAX_LOSING_FRAMES;
                    this->state = TRACKING;
                }else{
                    // can't find object for several times, then lost
                    losing_frames--;
                    if(losing_frames == 0){
                        //cout << "lost!" << endl;
                        losing_frames = MAX_LOSING_FRAMES;
                        this->state = LOST;
                        points.clear();
                    }
                }
                break;
        }
        
        return ret;
    }
    
};

#endif
