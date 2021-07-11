#include <opencv2/opencv.hpp>
#include "DataQueue.hpp"

void runSegmentation(DataQueue<std::shared_ptr<cv::Mat>> *inqueue, DataQueue<std::shared_ptr<cv::Mat>> *outqueue);
