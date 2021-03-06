#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/cudaobjdetect.hpp" 
#include <iostream>
#include <stdio.h>
 
using namespace std;
using namespace cv;
 
int main( )
{
    Mat h_image;
    h_image = imread("images/lena_color_512.tif", 0);  
    Ptr<cuda::CascadeClassifier> cascade = cuda::CascadeClassifier::create("haarcascade_eye.xml");
    cuda::GpuMat d_image;
    cuda::GpuMat d_buf;
    int64 start = cv::getTickCount();
    d_image.upload(h_image);
    cascadeGPU->setMinNeighbors(0);
    cascadeGPU->setScaleFactor(1.02);
    cascade->detectMultiScale(d_image, d_buf);
    std::vector<Rect> detections;
    cascade->convert(d_buf, detections);

    if (detections.empty())
        std::cout << "No detection." << std::endl;

    cvtColor(h_image,h_image,COLOR_GRAY2BGR);

    for(int i = 0; i < detections.size(); ++i)
    {
        rectangle(h_image, detections[i], Scalar(0,255,255), 5);
    }
    
    double fps = cv::getTickFrequency() / (cv::getTickCount() - start);
    std::cout << "FPS : " << fps << std::endl;
    imshow("Result image on Jetson TX1", h_image);
     
    waitKey(0);   
    return 0;
}