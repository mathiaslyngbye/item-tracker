#include <string>
#include <opencv2/opencv.hpp>
#include "DataQueue.hpp"

void runVirtualCamera(std::string path, int fps, DataQueue<std::shared_ptr<cv::Mat>> *outqueue);
