#ifndef __MATSERIALIZATION_H_
#define __MATSERIALIZATION_H_


#include <opencv2/opencv.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/base_object.hpp>


//BOOST_CLASS_EXPORT_GUID(::cv::Matx<double,1,4>);

BOOST_SERIALIZATION_SPLIT_FREE(::cv::Mat)
BOOST_SERIALIZATION_SPLIT_FREE(::cv::Point2f)
BOOST_SERIALIZATION_SPLIT_FREE(::cv::KeyPoint)
//BOOST_SERIALIZATION_SPLIT_FREE(::cv::Matx)



namespace boost 
{  
namespace serialization
{    
	/** Serialization support for cv::Mat */    
	template<class Archive>
	void save(Archive & ar, const ::cv::Mat& m, const unsigned int version)    
	{      
		size_t elem_size = m.elemSize();      
		size_t elem_type = m.type();
		ar & m.cols;
		ar & m.rows;
		ar & elem_size;
		ar & elem_type;
		const size_t data_size = m.cols * m.rows * elem_size;
		ar & boost::serialization::make_array(m.ptr(), data_size);    
	}    /** Serialization support for cv::Mat */    
	template <class Archive>    
	void load(Archive & ar, ::cv::Mat& m, const unsigned int version)
	{      
		int cols, rows;
		size_t elem_size, elem_type;
		ar & cols;      
		ar & rows;
		ar & elem_size;
		ar & elem_type;
		m.create(rows, cols, elem_type);
		size_t data_size = m.cols * m.rows * elem_size;
		ar & boost::serialization::make_array(m.ptr(), data_size);    
	}

	template<class Archive>
	void save(Archive & ar, const::cv::Point2f& m, const unsigned int version)
	{
		ar&m.x;
		ar&m.y;
	}
	template<class Archive>
	void load(Archive & ar, ::cv::Point2f& m, const unsigned int version)
	{
		ar&m.x;
		ar&m.y;
	}

	template<class Archive>
	void save(Archive & ar, const ::cv::KeyPoint& m, const unsigned int version)
	{
		ar & m.angle;
		ar & m.class_id;
		ar & m.octave;
		ar & m.pt;
		ar & m.response;
		ar & m.size;
	}
	template<class Archive>
	void load(Archive & ar, ::cv::KeyPoint& m, const unsigned int version)
	{
		ar & m.angle;
		ar & m.class_id;
		ar & m.octave;
		ar & m.pt;
		ar & m.response;
		ar & m.size;
	}
   
    template<class Archive, int m, int n>
    inline void serialize(
        Archive & ar,
        cv::Matx<bool, m, n> & t,
        const unsigned int file_version
        )
    {
        boost::serialization::split_free(ar, t, file_version);
    }

    template<class Archive, typename _Tp, int m, int n>
    void serialize(Archive& ar, ::cv::Matx<_Tp, m, n>& _m, const unsigned int /*version*/) {
        ar & make_array<_Tp>(_m.val,m*n);
    }

    template<class Archive, typename _Tp, int cn>
    void serialize(Archive& ar, ::cv::Vec<_Tp ,cn>& _m, const unsigned int /*version*/) {
        ar & make_array<_Tp>(_m.val, cn);
    }
    template<class Archive, typename _Tp>
    void serialize(Archive& ar, ::cv::Scalar_<_Tp>& _m, const unsigned int /*version*/) {
        ar & make_array<_Tp>(_m.val,_m.channels);
    }
}}


#endif//!__MATSERIALIZATION_H_


/************************************************************************/
/* std::vector < std::pair<std::string, cv::Scalar>> outlist;

    for (auto i = all_feature_map.begin(); i != all_feature_map.end();i++)
    {
    cv::Scalar tmp_scalar = cv::sum(cv::abs(i->second - Fc6FeatureMat1));

    outlist.push_back(std::pair<std::string, cv::Scalar>(i->first,tmp_scalar));
    }

    std::sort(outlist.begin(), outlist.end(), compare);

    input_file.close();




    std::ofstream osf(last_result_filename, std::ios::out);
    boost::archive::xml_oarchive xmloa1(osf);

    cv::Scalar tmp2;
    tmp2.val[0] = 0.3;
    tmp2.val[1] = 0.1;
    tmp2.val[2] = 0.1;
    tmp2.val[3] = 0.1;

    xmloa1 << BOOST_SERIALIZATION_NVP(outlist);                                                                     */
/************************************************************************/