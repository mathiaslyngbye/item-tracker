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

void runDisplay(DataQueue<std::shared_ptr<cv::Mat>> *inqueue)
{
    // Thread loop
    cv::namedWindow("Camera", cv::WINDOW_AUTOSIZE); 
    while(true)
    {
        // Pop queue
        std::shared_ptr<cv::Mat> image = inqueue->getData();

        // Load and show image
        cv::imshow("Camera", *image);
        cv::waitKey(1);
    }
}
