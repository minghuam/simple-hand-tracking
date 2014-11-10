//
//  main.cpp
//  prepare_traning
//
//  Created by Minghuang Ma on 9/16/14.
//  Copyright (c) 2014 Minghuang Ma. All rights reserved.
//

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <dirent.h>
#include <fstream>
#include <stdio.h>

using namespace std;

void list_files(vector<string> &file_list, const char* folder, const char* suffix){
    // list files
    cout << "list folder: " << folder << endl;
    DIR *dir = opendir(folder);
    struct dirent *entry = readdir(dir);
    string s_fd = folder;
    while (entry != NULL)
    {
        if (entry->d_type != DT_DIR){
            string filename = entry->d_name;
            if (filename.substr(max(4, (int)(filename.size()))-4) == std::string(suffix)) {
                filename = s_fd + "/" + filename;
                cout << filename << endl;
                file_list.push_back(filename);
            }
        }
        entry = readdir(dir);
    }
}

void calc_features_from_one_img(const string& img_file, vector<float> &descriptors, cv::HOGDescriptor &hog){
    // load image
    // http://web.mit.edu/vondrick/ihog/
    // http://www.vlfeat.org/overview/hog.html
    cv::Mat img = cv::imread(img_file);
    cv::cvtColor(img, img, CV_BGR2GRAY);
    if(img.empty()){
        descriptors.clear();
        cout << "failed to load: " << img_file;
        return;
    }
    vector<cv::Point> locations;
    hog.compute(img, descriptors);
    img.release();
}

void calc_and_save_features(const vector<string> &pos_imgs, const vector<string> &neg_imgs, const string &out_file, cv::HOGDescriptor &hog){
    /* calculate hog features and save to a file */
    fstream fs;
    fs.open(out_file.c_str(), ios::out);
    if(!fs.is_open()){
        cout << "failed to open: " << out_file;
        return;
    }
    int feature_cnt = 0;
    
    vector<float> descriptors;
    
    cout << "calc pos features" << endl;
    for(int i=0;i<pos_imgs.size();i++){
        descriptors.clear();
        calc_features_from_one_img(pos_imgs[i], descriptors, hog);
        fs << "+1";
        feature_cnt++;
        for(int j=0;j<descriptors.size();j++){
            fs << " " << (j+1) << ":" << descriptors[j];
        }
        fs << endl;
    }
    cout << "calc neg features" << endl;
    for(int i=0;i<neg_imgs.size();i++){
        descriptors.clear();
        calc_features_from_one_img(neg_imgs[i], descriptors, hog);
        fs << "-1";
        feature_cnt++;
        for(int j=0;j<descriptors.size();j++){
            fs << " " << (j+1) << ":" << descriptors[j];
        }
        fs << endl;
    }
    
    cout << feature_cnt << " images in total." << endl;
    
    fs.flush();
    fs.close();
}

void resize_and_crop(vector<string> &files, int width, int height, string in_folder, string out_folder){
    
    char buf[128];
    int save_img_cnt = 0;
    
    for(int i=0; i<files.size(); i++){
        cv::Mat img = cv::imread(in_folder + "/" + files[i]);
        // resize
        double ratio = (double)width/(double)img.cols;
        int h =round(img.rows*ratio);
        cv::resize(img, img, cv::Size(width,h));
        
        cv::Mat out_img(height,width,CV_8UC3,cv::Scalar::all(0));
        
        // crop
        if(h < height){
            cv::Rect roi(0,0,width,h);
            img.copyTo(out_img(roi));
        }else{
            cv::Rect roi(0,0,width,height);
            img(roi).copyTo(out_img);
        }
        
        sprintf(buf, "%d.jpg", save_img_cnt++);
        string save_name = buf;
        cv::imwrite(out_folder + "/" + save_name, out_img);
    }
}

int main(int argc, const char * argv[])
{
    const char* TRAIN_HAND_FOLDER = "./train/hand";
    const char* TRAIN_OTHER_FOLDER = "./train/other";
    const char* TEST_HAND_FOLDER = "./test/hand";
    const char* TEST_OTHER_FOLDER = "./test/other";
    const string TRAIN_OUT_FILE = "./train/hand.train";
    const string TEST_OUT_FILE = "./test/hand.test";
    const int HOG_WIN_WIDTH = 64;
    const int HOG_WIN_HEIGHT = 128;

    vector<string> train_hands;
    vector<string> train_others;
    vector<string> test_hands;
    vector<string> test_others;
    
    list_files(train_hands, TRAIN_HAND_FOLDER, ".jpg");
    list_files(train_others, TRAIN_OTHER_FOLDER, ".jpg");
    list_files(test_hands, TEST_HAND_FOLDER, ".jpg");
    list_files(test_others, TEST_OTHER_FOLDER, ".jpg");
    
    cv::HOGDescriptor hog;
    hog.winSize = cv::Size(HOG_WIN_WIDTH, HOG_WIN_HEIGHT);
    
    cout << "calc training descriptors." << endl;
    calc_and_save_features(train_hands, train_others, TRAIN_OUT_FILE, hog);
    cout << "calc test descriptors." << endl;
    calc_and_save_features(test_hands, test_others, TEST_OUT_FILE, hog);
    
    return 0;
}

