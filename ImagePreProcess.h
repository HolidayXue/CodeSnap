#pragma once
#ifndef __IMAGEPREPROCESS_H
#define __IMAGEPREPROCESS_H


#include <opencv2/opencv.hpp>
class ImagePreProcess
{
public:
    ImagePreProcess();
    ~ImagePreProcess();
    static int ImageSpilt(const cv::Mat& input_mat, int rows, int cols, std::vector<cv::Mat>& output_mat_vector);
    static int GetImageCenter(const cv::Mat& input_mat, int width, int height, cv::Mat& output_mat);

    static int NormalizeImageToFixSize_Vector(cv::Size input_size, std::vector<cv::Mat>& output_mat_vector);
    
    static int ShowInvisibleImage(cv::Mat input_mat, int rows);
};

#endif