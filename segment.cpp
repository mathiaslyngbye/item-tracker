// Standard
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <experimental/filesystem>

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

// Other
#include "display.hpp"
#include "DataQueue.hpp"

void runSegmentation(DataQueue<std::shared_ptr<cv::Mat>> *inqueue, DataQueue<std::shared_ptr<cv::Mat>> *outqueue)
{
    // Thread loop
    while(true)
    {
        // Pop queue
        std::shared_ptr<cv::Mat> image_ptr = inqueue->getData();
        
        // Threshold image
        cv::Mat image_gray, image_segmented;
        cv::cvtColor(*image_ptr, image_gray, cv::COLOR_BGR2GRAY);
        threshold(image_gray, image_gray, 100, 255, cv::THRESH_BINARY);
        cv::cvtColor(image_gray, image_segmented, cv::COLOR_GRAY2BGR);
       
        // Push queue
        outqueue->addData(std::make_shared<cv::Mat>(image_segmented));
    }
}
