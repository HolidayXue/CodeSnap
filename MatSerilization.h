#ifndef __MATSERIALIZATION_H_
#define __MATSERIALIZATION_H_


#include <opencv.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/string.hpp>

BOOST_SERIALIZATION_SPLIT_FREE(::cv::Mat)
BOOST_SERIALIZATION_SPLIT_FREE(::cv::Point2f)
BOOST_SERIALIZATION_SPLIT_FREE(::cv::KeyPoint)

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
}}


#endif//!__MATSERIALIZATION_H_