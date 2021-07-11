#ifndef _TRACKENTITY_HPP_
#define _TRACKENTITY_HPP_

#include <opencv2/opencv.hpp>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>

class TrackEntity{
    private:
        void generate_color()
        {
            cv::RNG rng(cv::getTickCount());
            m_color = cv::Scalar(rng.uniform(0,256), rng.uniform(0,256), rng.uniform(0,256));
        }
        
        int m_id = -1;
        cv::Rect m_bounding_rect;
        cv::Scalar m_color; 
    
    public:
        TrackEntity(cv::Rect bounding_rect)
        {
            m_bounding_rect = bounding_rect;
            generate_color();
        }

        cv::Rect getBoundingRect()
        {
            return m_bounding_rect;
        }

        void setBoundingRect(cv::Rect bounding_rect)
        {
            m_bounding_rect = bounding_rect;
        }
        
        cv::Scalar getColor()
        {
            return m_color;
        }

        void setId(int id)
        {
            m_id = id;
        }
        
        int getId()
        {
            return m_id;
        }
};

#endif
