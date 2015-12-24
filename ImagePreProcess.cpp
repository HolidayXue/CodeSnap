#include "ImagePreProcess.h"


ImagePreProcess::ImagePreProcess()
{
}


ImagePreProcess::~ImagePreProcess()
{
}


int ImagePreProcess::ImageSpilt(const cv::Mat& input_mat, int rows, int cols, std::vector<cv::Mat>& output_mat_vector)
{
    int return_result = -1;
    do
    {
        if (input_mat.cols < cols || input_mat.rows < rows)
        {
            break;
        }

        double split_width = (double)input_mat.cols / cols;
        double split_height = (double)input_mat.rows / rows;
        for (size_t i = 0; i < rows; i++)
        {
            for (size_t j = 0; j < cols; j++)
            {
                cv::Mat tmp(input_mat, cv::Rect(j*split_width, i*split_height, split_width, split_height));
                output_mat_vector.push_back(tmp);
            }
        }
        int return_result = 0;
    } while (0);

    return return_result;
}

int ImagePreProcess::GetImageCenter(const cv::Mat& input_mat, int width, int height, cv::Mat& output_mat)
{
    int return_result = -1;
    do
    {
        if (width > input_mat.cols || height>input_mat.rows)
        {
            break;
        }
        int start_x((input_mat.cols - width) / 2);
        int start_y((input_mat.rows - height) / 2);

        output_mat = cv::Mat(input_mat, cv::Rect(start_x, start_y, width, height));
        return_result = 0;
    } while (0);

    return return_result;
}

int ImagePreProcess::NormalizeImageToFixSize_Vector(cv::Size input_size, std::vector<cv::Mat>& output_mat_vector)
{
    int return_result = -1;
    do
    {
        if (output_mat_vector.empty())
        {
            break;
        }
        for (auto i = output_mat_vector.begin(); i != output_mat_vector.end(); i++)
        {
            cv::resize(*i, *i, input_size);
        }
        return_result = 0;
    } while (0);

    return return_result;
}

int ImagePreProcess::ShowInvisibleImage(cv::Mat input_mat,int rows)
{
    std::vector<std::string> window_name = {"1","2","3","4","5"};
    for (int i = 0; i < input_mat.rows; i++)
    {
        cv::Mat tmp_mat(input_mat.cols,1,CV_32F);
        input_mat.row(i).copyTo(tmp_mat);
        tmp_mat = tmp_mat.reshape(0, rows);
        cv::normalize(tmp_mat, tmp_mat, 1.0, 0.0, cv::NORM_MINMAX);
        cv::Mat show_mat;
        
        tmp_mat.convertTo(show_mat, CV_8UC1,255.0);
        imshow(window_name[i],show_mat);
    }
    cv::waitKey(0);
    return 0;
}
