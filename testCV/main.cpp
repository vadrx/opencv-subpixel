//
//  main.cpp
//  testCV
//
//  Created by Anton Volkov on 12/04/2018.
//  Copyright © 2018 Anton Volkov. All rights reserved.
//
#include <iostream>
#include <OpenCL/OpenCL.h>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/highgui.hpp>

float subpixel_peak(const cv::Mat &response, const std::string &s, const cv::Point2f &p)
{
    int i_p0, i_p_l, i_p_r;     // indexes in response
    float p0, p_l, p_r;         // values in response
    std::cout<<"p.x:\t"<<p.x<<std::endl;
    std::cout<<"p.y:\t"<<p.y<<std::endl;
    if (s.compare("vertical") == 0)
    {
        // neighbouring rows
        i_p0 = cvRound(p.y);
        i_p_l = fmodl(cvRound(p.y) - 1, response.rows);
        i_p_r = fmodl(cvRound(p.y) + 1, response.rows);
        int px = static_cast<int>(p.x);
        p0 = response.at<float>(i_p0, px);
        
        p_l = response.at<float>(i_p_l, px);
        p_r = response.at<float>(i_p_r, px);
    }
    else if (s.compare("horizontal") == 0)
    {
        // neighbouring cols
        i_p0 = cvRound(p.x);
        i_p_l = fmodl(cvRound(p.x) - 1, response.cols);
        i_p_r = fmodl(cvRound(p.x) + 1, response.cols);
        int py = static_cast<int>(p.y);
        p0 = response.at<float>(py, i_p0);
        
        p_l = response.at<float>(py, i_p_l);
        p_r = response.at<float>(py, i_p_r);
    }
    else
    {
        std::cout << "Warning: unknown subpixel peak direction!" << std::endl;
        return 0;
    }
    float delta = 0.5f * (p_r - p_l) / (2 * p0 - p_r - p_l);
    if (!std::isfinite(delta))
    {
        delta = 0;
    }
    std::cout << "Delta:\t" << delta << std::endl;
    return delta;
}
void estimate_new_position(const cv::Mat &image)
{
    cv::Mat resp = image;
    std::vector<cv::Mat> ch;
    cv::split(resp, ch);
    
    cv::Mat ch0 = ch[0];
//    ch0.convertTo(ch0, CV_32F);
//    cv::imwrite("rr.jpg", ch0);
//    int t = ch0.type();


    cv::Point max_loc;
    cv::minMaxLoc(ch0, NULL, NULL, NULL, &max_loc);
    // take into account also subpixel accuracy
    float col = ((float)max_loc.x) + subpixel_peak(ch0, "horizontal", max_loc);
    float row = ((float)max_loc.y) + subpixel_peak(ch0, "vertical", max_loc);
    
    std::cout << " -- Out -- " << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << " Matr[" << i << "]:\t" << resp.at<float>(i) << std::endl;
    }
    
    std::cout<<"max_loc.x:\t"<< (float)max_loc.x<<std::endl;
    std::cout<<"max_loc.y:\t"<< (float)max_loc.y <<std::endl;
    
    std::cout<<"Col:\t"<<col<<std::endl;
    std::cout<<"Row:\t"<<row<<std::endl;
    
}

int main(int argc, const char * argv[]) {
    const cv::Mat image = cv::imread("rr.jpg");

    estimate_new_position(image);
    
    return 0;
}
