// RecursivePathInverlist.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <boost/filesystem.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <opencv2/opencv.hpp>
#include "MatSerilization.h"
#include <map>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>

const std::string root_path = "E:/ImageIKEARenderFeature/";
std::map<std::string, cv::Mat> all_feature_map;

void ProcesSingleFile(std::string input_path)
{
    cv::Mat feature_mat;
    std::ifstream sf1(input_path);
    boost::archive::text_iarchive tia1(sf1);
    tia1 >> feature_mat;

    std::string key_string = input_path.substr(root_path.length());

    all_feature_map.insert(std::pair<std::string, cv::Mat>(key_string, feature_mat));

}

void resucurePath(boost::filesystem::path src_path)
{
    using namespace boost::filesystem;


    if (is_directory(src_path))
    {
        directory_iterator tmp_directory_end;
        directory_iterator tmp_dir_it(src_path);

        for (tmp_dir_it; tmp_dir_it != tmp_directory_end; tmp_dir_it++)
        {
            resucurePath((*tmp_dir_it).path());
        }
    }
    else if (is_regular_file(src_path))
    {
        std::string file_extension = boost::to_lower_copy(src_path.extension().string());
        if (file_extension == ".txt")
        {
            ProcesSingleFile(src_path.string());
        }
    }
}

int _tmain(int argc, char* argv[])
{

    //std::string tmpfile = "E:/ImageBlendFeature/5707434054603/5707434054603_000_train.txt"; 
    //ProcesSingleFile(tmpfile);

    boost::filesystem::path input_path(root_path);


    
    std::string output_file_name = "E:/ImageIKEARenderFeature/AllFeature.binaryfeature";
    std::ofstream output_file(output_file_name,std::ios::binary);
    boost::archive::binary_oarchive boa(output_file);


    resucurePath(input_path);
    boa << all_feature_map;

    output_file.close();
    
	return 0;
}

