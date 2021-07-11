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
#include "virtual_camera.hpp"
#include "DataQueue.hpp"

void runVirtualCamera(std::string path, int fps, DataQueue<std::shared_ptr<cv::Mat>> *outqueue)
{
    // Calculate delay
    int delay_ms = 1000/fps;

    // Get image paths
    std::vector<std::string> paths;
    for(const auto &entry : std::experimental::filesystem::directory_iterator(path))
        paths.push_back(entry.path());
    std::sort(paths.begin(), paths.end());
    int sequence_length = paths.size();

    // Thread loop
    int sequence_index = 0;
    while(true)
    {
        // Start time
        auto start = std::chrono::steady_clock::now();

        // Load and show image
        cv::Mat image = cv::imread(paths[sequence_index], cv::IMREAD_COLOR);
        std::shared_ptr<cv::Mat> image_ptr = std::make_shared<cv::Mat>(image);
        outqueue->addData(image_ptr,1);

        // Make sequence loop
        sequence_index++;
        if(sequence_index == sequence_length)
            sequence_index = 0;
        
        // End time
        auto end = std::chrono::steady_clock::now();

        // Sleep to match fps 
        int elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms-elapsed_ms));
    }
}
