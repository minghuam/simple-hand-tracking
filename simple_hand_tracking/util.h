//
//  util.hpp
//  simple_hand_tracking
//
//  Created by Minghuang Ma on 9/17/14.
//  Copyright (c) 2014 Minghuang Ma. All rights reserved.
//

#ifndef simple_hand_tracking_util_hpp
#define simple_hand_tracking_util_hpp


#include <vector>
#include <fstream>
#include <cfloat>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

#define PI 3.14159265

struct Vec2f{
    float x,y;
    Vec2f() { Vec2f(0.0f, 0.0f); }
    Vec2f(float _x, float _y) : x(_x), y(_y) {}
    float length() { return sqrt(x*x + y*y); }
    
    Vec2f operator -(const Vec2f &v){
        Vec2f ret(x - v.x, y - v.y);
        return ret;
    }
    
    Vec2f operator +(const Vec2f &v){
        Vec2f ret(x + v.x, y + v.y);
        return ret;
    }
    
    void operator =(const Vec2f &v){
        x = v.x;
        y = v.y;
    }
    
    bool operator ==(const Vec2f &v){
        return (x == v.x) && (y == v.y);
    }
    
};

struct Rect2f{
    float x,y;
    float w,h;
    Rect2f() { Rect2f(0.0f,0.0f,0.0f,0.0f); }
    Rect2f(float _x, float _y, float _w, float _h) : x(_x),y(_y),w(_w),h(_h) {}
};

#endif
