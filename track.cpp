// Standard
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <thread>
#include <experimental/filesystem>

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

// Other
#include "display.hpp"
#include "DataQueue.hpp"

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
        
        // Draw contours
        for(int i = 0; i<contours.size(); i++)
        {
            cv::Rect bounding_rect = getBoundingRect(contours[i]);
            cv::Scalar color = cv::Scalar(0,255,0);
            cv::rectangle(*image_ptr, bounding_rect, color, 2, cv::LINE_8, 0); 
        }
       
        // Push queue
        outqueue->addData(image_ptr);
    }
}
