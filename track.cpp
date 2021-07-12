// Standard
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <thread>
#include <cmath>
#include <experimental/filesystem>

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

// Other
#include "display.hpp"
#include "DataQueue.hpp"
#include "TrackEntity.hpp"

// Defines
#define CONTOUR_AREA_MIN 100
#define DISTANCE_MAX 50 

double getRectDistance(cv::Rect a, cv::Rect b)
{
    // Get center of rectangles
    int a_x = a.x+(a.width/2);
    int a_y = a.y+(a.height/2);
    int b_x = b.x+(b.width/2);
    int b_y = b.y+(b.height/2);

    // Calculate euclidean distance 
    double distance = std::sqrt(std::pow((b_x-a_x),2)+std::pow((b_y-a_y),2));
    return std::abs(distance);
}

cv::Rect getBoundingRect(std::vector<cv::Point> contour)
{
    int x_min = 99999, x_max = 0, y_min = 99999, y_max = 0;
    for(int i = 0; i < contour.size(); i++)
    {
        // Get max values
        cv::Point test_point = contour[i];
        if(test_point.x <= x_min)
            x_min = test_point.x;
        if(test_point.x >= x_max)
            x_max = test_point.x;
        if(test_point.y <= y_min)
            y_min = test_point.y;
        if(test_point.y >= y_max)
            y_max = test_point.y;
    }

    // Create OpenCV rectangle
    int width = x_max-x_min;
    int height = y_max-y_min;
    cv::Rect bounding_rect(x_min,y_min,width,height);
    
    // Return bounding rectangle
    return bounding_rect;
}

void runTracking(DataQueue<std::shared_ptr<cv::Mat>> *inqueue, DataQueue<std::shared_ptr<cv::Mat>> *outqueue)
{
    std::vector<TrackEntity> tracked_units;
    std::vector<TrackEntity> tracked_candidates;
    int tracked_unit_count = 0;

    // Thread loop
    while(true)
    {
        // Pop queue
        std::shared_ptr<cv::Mat> image_ptr = inqueue->getData();
        
        // Find contours
        cv::Mat image_gray;
        cv::cvtColor(*image_ptr, image_gray, cv::COLOR_BGR2GRAY);
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(image_gray, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);
        
        // Detect current units
        std::vector<cv::Rect> detected_units;
        for(int i = 0; i<contours.size(); i++)
        {
            // Generate bounding rect for comparison
            cv::Rect bounding_rect = getBoundingRect(contours[i]);
            
            if(cv::contourArea(contours[i]) >= CONTOUR_AREA_MIN &&
                bounding_rect.x > 0 && (bounding_rect.x+bounding_rect.width)<(image_ptr->cols-1))
            {
                detected_units.push_back(bounding_rect); 
            }
        }

        // Assigned closest detected units to tracked units
        std::vector<bool> assigned(tracked_units.size(),false);
        for(int i = 0; i<tracked_units.size(); i++)
        {
            TrackEntity unit = tracked_units[i];
            cv::Rect unit_bounding_rect = unit.getBoundingRect();

            double rect_distance_min = 99999;
            int rect_distance_min_index = 0;
            for(int j = 0; j<detected_units.size(); j++)
            {
                double rect_distance = getRectDistance(unit_bounding_rect, detected_units[j]);
                if(rect_distance <= rect_distance_min)
                {
                    rect_distance_min = rect_distance;
                    rect_distance_min_index = j;
                }
            }

            // Assign detected unit to tracked 
            if(rect_distance_min < DISTANCE_MAX)
            {
                tracked_units[i].setBoundingRect(detected_units[rect_distance_min_index]);
                assigned[i] = true;
                detected_units.erase(detected_units.begin() + rect_distance_min_index);
            }
        }
        
        // Erase disappeared objects
        for(int i = assigned.size()-1; i>=0; i--)
        {
            if(!assigned[i])
            {
                tracked_units.erase(tracked_units.begin() + i);
            }
        }

        // Add unassigned units to tracked units for now
        for(int i = 0; i<detected_units.size(); i++)
        {
            TrackEntity unit(detected_units[i]);
            unit.setId(tracked_unit_count++);
            tracked_units.push_back(unit);
        }

       
        // Draw tracked units on image
        for(int i = 0; i<tracked_units.size(); i++)
        {
            cv::Rect bounding_rect = tracked_units[i].getBoundingRect();
            cv::Scalar color = tracked_units[i].getColor();
            
            // Draw bounding rectangle
            cv::rectangle(*image_ptr, bounding_rect, color, 2, cv::LINE_8, 0); 

            // Draw ID
            int x = bounding_rect.x+(bounding_rect.width/2); 
            int y = bounding_rect.y+(bounding_rect.height/2); 
            std::string id = std::to_string(tracked_units[i].getId());
            cv::Size text_size = cv::getTextSize(id,cv::FONT_HERSHEY_COMPLEX_SMALL,1.5,2,0);
            cv::putText(*image_ptr, id, cv::Point(x-(text_size.width/2),y+(text_size.height/2)), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.5, color, 2, cv::LINE_AA);
        }
       
        // Print some info on image
        std::string output_text = "Active units: " + std::to_string(tracked_units.size());
        cv::putText(*image_ptr, output_text, cv::Point(5,20), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(255,255,255), 1, cv::LINE_AA);

        // Push queue
        outqueue->addData(image_ptr);
    }
}
