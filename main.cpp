// Standard
#include <iostream>
#include <string>
#include <thread>

// OpenCV
#include "opencv2/opencv.hpp"

// Other
#include "DataQueue.hpp"
#include "TrackEntity.hpp"
#include "virtual_camera.hpp"
#include "display.hpp"
#include "segment.hpp"
#include "track.hpp"

int main()
{
    DataQueue<std::shared_ptr<cv::Mat>> segmentation_queue;
    DataQueue<std::shared_ptr<cv::Mat>> tracking_queue;
    DataQueue<std::shared_ptr<cv::Mat>> display_queue;
    std::string path = "../images/";
    int fps = 30;

    std::thread virtual_camera_th(runVirtualCamera, path, fps, &segmentation_queue);
    std::thread segmentation_th(runSegmentation, &segmentation_queue, &tracking_queue);
    std::thread tracking_th(runTracking, &tracking_queue, &display_queue);
    std::thread display_th(runDisplay, &display_queue);
    
    virtual_camera_th.join();
    segmentation_th.join();
    tracking_th.join();
    display_th.join();
    
    std::cout << "End of main!" << std::endl;
    return 0;
}
