#include <boost/make_shared.hpp>

// these need to be included after boost on OS X
#include <string>  // NOLINT(build/include_order)
#include <vector>  // NOLINT(build/include_order)
#include <fstream>  // NOLINT

#include "caffe/caffe.hpp"
#include <opencv2/opencv.hpp>

#include "ImagePreProcess.h"
#include "MatSerilization.h"

#include <boost/archive/text_oarchive.hpp>

void FloatMatToDatum(const cv::Mat& float_mat, caffe::Datum* datum) {

    datum->set_channels(float_mat.channels());
    datum->set_height(float_mat.rows);
    datum->set_width(float_mat.cols);
    datum->clear_data();
    datum->clear_float_data();
    datum->set_encoded(false);
    int datum_channels = datum->channels();
    int datum_height = datum->height();
    int datum_width = datum->width();
    int datum_size = datum_channels * datum_height * datum_width;
    std::string buffer(datum_size, ' ');
    for (int h = 0; h < datum_height; ++h) {
        const float* ptr = float_mat.ptr<float>(h);
        int img_index = 0;
        for (int w = 0; w < datum_width; ++w) {
            for (int c = 0; c < datum_channels; ++c) {
                int datum_index = (c * datum_height + h) * datum_width + w;
                buffer[datum_index] = static_cast<char>(ptr[img_index++]);
            }
        }
    }
    datum->set_data(buffer);
}


cv::Mat ReadMeanMat(const std::string& mean_file)
{
    caffe::BlobProto blob_proto;
    ReadProtoFromBinaryFileOrDie(mean_file.c_str(), &blob_proto);

    /* Convert from BlobProto to Blob<float> */
    caffe::Blob<float> mean_blob;
    mean_blob.FromProto(blob_proto);
    int num_channels_ = 3;
    CHECK_EQ(mean_blob.channels(), num_channels_)
        << "Number of channels of mean file doesn't match input layer.";

    /* The format of the mean file is planar 32-bit float BGR or grayscale. */
    std::vector<cv::Mat> channels;
    float* data = mean_blob.mutable_cpu_data();
    for (int i = 0; i < num_channels_; ++i) {
        /* Extract an individual channel. */
        cv::Mat channel(mean_blob.height(), mean_blob.width(), CV_32FC1, data);
        channels.push_back(channel);
        data += mean_blob.height() * mean_blob.width();
    }

    /* Merge the separate channels into a single image. */
    cv::Mat mean;
    cv::merge(channels, mean);
    return mean;
}

static bool PairCompare(const std::pair<float, int>& lhs,
    const std::pair<float, int>& rhs) {
    return lhs.first > rhs.first;
}

static std::vector<int> Argmax(const std::vector<float>& v, int N) {
    std::vector<std::pair<float, int> > pairs;
    for (size_t i = 0; i < v.size(); ++i)
        pairs.push_back(std::make_pair(v[i], i));


    //std::partial_sort(pairs.begin(), pairs.begin() + N, pairs.end(), PairCompare);

    std::sort(pairs.begin(), pairs.end(), PairCompare);

    std::vector<int> result;
    for (int i = 0; i < N; ++i)
        result.push_back(pairs[i].second);
    return result;
}

static void CheckFile(const std::string& filename) {
    std::ifstream f(filename.c_str());
    if (!f.good()) {
        f.close();
        throw std::runtime_error("Could not open file " + filename);
    }
    f.close();
}


template <typename Dtype>
caffe::Net<Dtype>* Net_Init_Load(
    std::string param_file, std::string pretrained_param_file, caffe::Phase phase)
{
    CheckFile(param_file);
    CheckFile(pretrained_param_file);

    caffe::Net<Dtype>* net(new caffe::Net<Dtype>(param_file, phase));


    net->CopyTrainedLayersFrom(pretrained_param_file);
    return net;
}
#define NetF float
//#define NetF double

int main(int _Argc, char ** _Argv)
{
    std::string imagepath = _Argv[1];
    std::string outtxt = _Argv[2];
    std::string deploy_txt = _Argv[3];
    std::string deploy_model = _Argv[4];
    std::string feature_layer_name = _Argv[5];
    /* std::string imagepath = "E:/ImageBlend/CH0055_00_0012/CH0055_00_0012_008_train.png";
     std::string outtxt = "D:/ImageWatch/CH0055_00_0012_008_train.txt";
     std::string deploy_txt = "D:/models/deploy_Test.prototxt";
     std::string deploy_model = "D:/models/bvlc_reference_caffenet.caffemodel";
     std::string feature_layer_name = "pool5";*/

    cv::Mat image = cv::imread(imagepath);
    std::vector<cv::Mat> mat_vector;
    mat_vector.clear();
    ImagePreProcess::ImageSpilt(image, 2, 2, mat_vector);
    cv::Mat center_mat;
    ImagePreProcess::GetImageCenter(image, image.cols / 2, image.rows / 2, center_mat);
    mat_vector.push_back(center_mat);
    ImagePreProcess::NormalizeImageToFixSize_Vector(cv::Size(256, 256), mat_vector);

    std::vector<int> dvl(5,0);
    caffe::Net<NetF>* _net = Net_Init_Load<NetF>(deploy_txt, deploy_model, caffe::TEST);

    caffe::MemoryDataLayer<NetF> *memory_layer_ = (caffe::MemoryDataLayer<NetF> *)_net->layers()[0].get();
    memory_layer_->set_batch_size(5);
    memory_layer_->AddMatVector(mat_vector, dvl);

    float loss = 0.0;
    std::vector<caffe::Blob<float>*> results = _net->ForwardPrefilled(&loss);
    for (auto i = _net->blob_names().begin(); i != _net->blob_names().end(); i++)
    {
        std::cout << *i << std::endl;
    }

    boost::shared_ptr<caffe::Blob<NetF>> prob = _net->blob_by_name("prob");


    boost::shared_ptr<caffe::Blob<NetF>> pool5 = _net->blob_by_name(feature_layer_name);
    cv::Mat Pool5FeatureMat(pool5->num(), pool5->width()*pool5->height()*pool5->channels(), CV_32F);
    int channel_step = pool5->height()*pool5->width();
    for (size_t n = 0; n < pool5->num(); n++)
    {
        for (size_t c = 0; c < pool5->channels(); c++)
        {

            for (size_t h = 0; h < pool5->height(); h++)
            {
                for (size_t w = 0; w < pool5->width(); w++)
                {
                    Pool5FeatureMat.at<float>(n, channel_step*c + h*pool5->width() + w) = pool5->data_at(n, c, h, w);
                }

            }
        }
    }

    std::ofstream sf(outtxt);
    boost::archive::text_oarchive toa(sf);

    toa << Pool5FeatureMat;
    sf.flush();
    sf.close();

    return 0;
}