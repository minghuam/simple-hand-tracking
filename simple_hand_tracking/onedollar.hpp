//
//  onedollar.hpp
//  simple_hand_tracking
//
//  C++ implementation of $1 guesture recognizer
//  http://depts.washington.edu/aimgroup/proj/dollar/
//
//  Created by Minghuang Ma on 9/17/14.
//  Copyright (c) 2014 Minghuang Ma. All rights reserved.
//

#ifndef simple_hand_tracking_onedollar_hpp
#define simple_hand_tracking_onedollar_hpp
#pragma once

#include <vector>
#include <fstream>
#include <cfloat>
#include <opencv2/opencv.hpp>
#include <iostream>

#include "util.h"
#include "gesturedata.h"

using namespace std;


class OneDollar{
    // rotation range [theta_a, theta_b]
    float ratio;
    float theta_a;
    float theta_b;
    
    // rotation resolution
    float theta_d;
    
    // number of normalized points
    int n_points;
    
    // normalized scale size
    float scale_size;
    
    // half of diagonal distance
    float half_diagonal;
    
    // template path
    vector<vector<Vec2f> > npaths;
    
public:
    
    OneDollar(){
        ratio = (-1.0f + sqrt(5))/2.0f;
        theta_a = PI/4;
        theta_b = -PI/4;
        theta_d = PI*2/180;
        n_points = 64;
        scale_size = 250.0f;
        half_diagonal = 0.5 * sqrt((scale_size*scale_size) + (scale_size*scale_size));
        
        SampleTemplates ts;
        
        // normalize templates
        for(int i=0;i<ts.raw_paths.size();i++){
            vector<Vec2f> pts;
            normalize(ts.raw_paths[i], pts);
            npaths.push_back(pts);
        }
    }
    
    void cv_draw_template(cv::Mat &ui_img, int t){
        if(t < 0 || t > npaths.size()-1) return;
        // draw template trace
        for(vector<Vec2f>::iterator it=npaths[t].begin(); it!=--npaths[t].end();++it){
            if(it == npaths[t].begin()){
                cv::circle(ui_img, cv::Point((int)(it->x), (int)(it->y)), 3, cv::Scalar(0,255,255));
            }
            cv::Point pt1((int)(it->x), (int)(it->y)); ++it;
            cv::Point pt2((int)(it->x), (int)(it->y)); --it;
            cv::line(ui_img, pt1, pt2, cv::Scalar(255,0,255), 1);
        }
        // draw bounding box
        cv::rectangle(ui_img, cv::Point(0,0), cv::Point(250,250), cv::Scalar(0,255,255));
    }
    
    void cv_resample(list<cv::Point2f> &points, list<cv::Point2f> &new_points, int n){
        new_points.clear();
        if(points.size() == 0) return;
        
        // remove duplicates and convert from cv::Point2f to Vect2f
        vector<Vec2f> pts;
        pts.push_back(Vec2f(points.begin()->x, points.begin()->y));
        cv::Point2f prev = *points.begin();
        list<cv::Point2f>::iterator it = points.begin();
        ++it;
        while(it != points.end()){
            if(it->x != prev.x || it->y != prev.y){
                pts.push_back(Vec2f(it->x, it->y));
                prev = *it;
            }
            ++it;
        }
        
        // resample
        vector<Vec2f> new_pts;
        resample(pts, new_pts, n);
        
        // convert back to cv
        for(int i=0; i<new_pts.size(); i++){
            new_points.push_back(cv::Point2f(new_pts[i].x, new_pts[i].y));
        }
    }
    
    float recognize_cv(list<cv::Point2f> &points, list<cv::Point2f> &new_points, int t){
        new_points.clear();
        if(points.size() == 0 || t < 0 || t > npaths.size()-1) return 0.0f;

        // only pick last 64(n_points) points
        vector<Vec2f> pts;
        pts.push_back(Vec2f(points.rbegin()->x, points.rbegin()->y));
        cv::Point2f prev = *points.rbegin();
        list<cv::Point2f>::reverse_iterator it = points.rbegin();
        ++it;
        int size = 1;
        while(it != points.rend() && size < n_points){
            if(it->x != prev.x || it->y != prev.y){
                pts.push_back(Vec2f(it->x, it->y));
                prev = *it;
                size++;
            }
            ++it;
        }
        // reverse
        vector<Vec2f> pts_r;
        for(vector<Vec2f>::reverse_iterator it = pts.rbegin(); it != pts.rend(); ++it){
            pts_r.push_back(*it);
        }

        // compare with template t
        vector<Vec2f> new_pts;
        float score = recognize(pts_r, new_pts, t);
        
        // convert back to cv
        for(int i=0; i<new_pts.size(); i++){
            new_points.push_back(cv::Point2f(new_pts[i].x, new_pts[i].y));
        }
        
        return score;
    }
    
    float recognize(vector<Vec2f> &points, vector<Vec2f> &new_points, int t){
        // calculate distance from points to template points
        if(points.size() < 2) return -1;
        vector<Vec2f> temp;
        normalize(points, new_points);
        
        float d = distance_at_best_angle(new_points, this->npaths[t], this->theta_a, this->theta_b, this->theta_d, this->ratio);
        return 1-d/this->half_diagonal;
    }
    
    void normalize(vector<Vec2f> &points, vector<Vec2f> &new_points){
        vector<Vec2f> temp1;
        vector<Vec2f> temp2;
        resample(points, temp1, n_points);
        float angle = indicative_angle(temp1);
        rotate_by(temp1, temp2, -angle);
        scale_to(temp2, temp1, scale_size);
        translate_to(temp1, new_points, Vec2f(0.0f, 0.0f));
    }
    
    float path_length(vector<Vec2f> &points){
        // calculate distance from two set of points
        float len = 0.0f;
        if(points.size() < 2) return 0.0f;
        for(int i=0; i<points.size()-1; i++){
            len += (points[i+1] - points[i]).length();
        }
        return len;
    }
    
    void resample(vector<Vec2f> &points, vector<Vec2f> &new_points, int n){
        // resample points to n new points
        float I = path_length(points)/(n-1);
        float D = 0.0f;
        new_points.clear();
        new_points.push_back(points[0]);
        for(int i=1; i<points.size(); i++){
            Vec2f prev = points[i-1];
            Vec2f curr = points[i];
            // make sure that curr != prev, otherwise, d = 0
            float d = (curr - prev).length();
            if(D+d >= I){
                double qx = prev.x + ((I-D)/d) * (curr.x - prev.x);
                double qy = prev.y + ((I-D)/d) * (curr.y - prev.y);
                Vec2f q(qx,qy);
                new_points.push_back(q);
                points.insert(points.begin()+i, q);
                D = 0.0f;
            }else{
                D += d;
            }
        }
        if(new_points.size() < n){
            new_points.push_back(points.back());
        }
        if(new_points.size() > n){
            new_points.erase(new_points.begin(), new_points.begin()+n);
        }
    }
    
    float indicative_angle(vector<Vec2f> &points){
        Vec2f c = centroid(points);
        if(c.x - points[0].x == 0){
            return 0.0f;
        }
        return atan((c.y - points[0].y)/(c.x - points[0].x));
    }
    
    Vec2f centroid(vector<Vec2f> &points){
        float x = 0.0f;
        float y = 0.0f;
        for(int i=0;i<points.size();i++){
            x += points[i].x;
            y += points[i].y;
        }
        x /= points.size();
        y /= points.size();
        return Vec2f(x,y);
    }
    
    void rotate_by(vector<Vec2f> &points, vector<Vec2f> &new_points, float rad){
        // rotate around centroid
        Vec2f c = centroid(points);
        new_points.clear();
        float cosw = cos(rad);
        float sinw = sin(rad);
        for(int i=0;i<points.size();i++){
            float dx = points[i].x - c.x;
            float dy = points[i].y - c.y;
            float qx = dx * cosw - dy * sinw + c.x;
            float qy = dx * sinw + dy * cosw + c.y;
            new_points.push_back(Vec2f(qx,qy));
        }
    }
    
    Rect2f bounding_box(vector<Vec2f> &points){
        float min_x = FLT_MAX, min_y = FLT_MAX, max_x = FLT_MIN, max_y = FLT_MIN;
        for(int i=0; i<points.size(); i++){
            if(points[i].x < min_x) min_x = points[i].x;
            if(points[i].x > max_x) max_x = points[i].x;
            if(points[i].y < min_y) min_y = points[i].y;
            if(points[i].y > max_y) max_y = points[i].y;
        }
        return Rect2f(min_x, min_y, max_x - min_x, max_y - min_y);
    }
    
    void scale_to(vector<Vec2f> &points, vector<Vec2f> &new_points, float size = 250.0f){
        Rect2f bbox = bounding_box(points);
        
        new_points.clear();
        float x_scale = size/bbox.w;
        float y_scale = size/bbox.h;
        
        if(bbox.w*bbox.h == 0) return;
        for(int i=0; i<points.size(); i++){
            new_points.push_back(Vec2f(points[i].x*x_scale, points[i].y*y_scale));
        }
    }
    
    void translate_to(vector<Vec2f> &points, vector<Vec2f> &new_points, Vec2f k){
        // align centroid
        /*
        Vec2f c = centroid(points);
        for(int i=0; i<points.size(); i++){
            new_points.push_back(Vec2f(points[i].x + k.x - c.x, points[i].y + k.y - c.y));
        }
        */
        // align bbox
        Rect2f b = bounding_box(points);
        for(int i=0; i<points.size(); i++){
            new_points.push_back(Vec2f(points[i].x + k.x - b.x, points[i].y + k.y - b.y));
        }
    }
    
    float path_distance(vector<Vec2f> &A, vector<Vec2f> &B){
        // TODO: what if A and B have different size ?????
        float d = 0.0f;
        for(int i=0;i<A.size();i++){
            d += (A[i] - B[i]).length();
        }
        return d/A.size();
    }
    
    float distance_at_angle(vector<Vec2f> &points, vector<Vec2f> &Tpoints, float theta){
        vector<Vec2f> new_points;
        rotate_by(points, new_points, theta);
        return path_distance(new_points, Tpoints);
    }
    
    float distance_at_best_angle(vector<Vec2f> &points, vector<Vec2f> &Tpoints, float theta_a, float theta_b, float theta_d, float ratio){
        /*
        int step = abs((int)((theta_a - theta_b)/theta_d));
        float f = FLT_MAX;
        for(int i=0; i<step; i++){
            float theta = theta_b + i*theta_d;
            float ff = distance_at_angle(points, Tpoints, theta);
            if(ff < f) f = ff;
        }
        
        return f;
        */

        float x1 = ratio * theta_a + (1.0f - ratio) * theta_b;
        float f1 = distance_at_angle(points, Tpoints, x1);
        float x2 = (1.0f - ratio) * theta_a + ratio * theta_b;
        float f2 = distance_at_angle(points, Tpoints, x2);
        while(abs(theta_b - theta_a) > theta_d){
            if(f1 < f2){
                theta_b = x2;
                x2 = x1;
                f2 = f1;
                x1 = ratio * theta_a + (1 - ratio) * theta_b;
                f1 = distance_at_angle(points, Tpoints, x1);
            }else{
                theta_a = x1;
                x1 = x2;
                f1 = f2;
                x2 = (1 - ratio) * theta_a + ratio * theta_b;
                f2 = distance_at_angle(points, Tpoints, x2);
            }
        }
        //cout << " -> " << theta_a << "," << theta_b << "," << min(f1,f2) << endl;
        
        return min(f1, f2);
    }
    
    
};

#endif

