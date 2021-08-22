#ifndef _TRACKENTITY_HPP_
#define _TRACKENTITY_HPP_

#include <opencv2/opencv.hpp>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>
#include <ctime>
#include <string>
#include <sstream>

class TrackEntity{
    private:
        cv::Scalar generate_color()
        {
            cv::RNG rng(cv::getTickCount());
            cv::Scalar color = cv::Scalar(rng.uniform(0,256), rng.uniform(0,256), rng.uniform(0,256));
            return color;
        }
        
        std::string generate_uuid()
        {
            std::time_t time_stamp = std::time(0);
            
            std::stringstream uuid_ss;
            uuid_ss << std::to_string(time_stamp);
            for(int i = 0; i < 3; i++)
                uuid_ss << '-' << std::setfill('0') << std::setw(3) << (int)m_color[i];

            std::string uuid;
            uuid_ss >> uuid;

            return uuid;
        }
        
        int m_id = -1;
        std::string m_uuid;
        cv::Rect m_bounding_rect;
        cv::Scalar m_color; 
    
    public:
        TrackEntity(cv::Rect bounding_rect)
        {
            m_bounding_rect = bounding_rect;
            m_color = generate_color();
            m_uuid = generate_uuid();
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
