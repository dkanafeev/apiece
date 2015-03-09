#ifndef INPUTDATA_H
#define INPUTDATA_H
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
class InputData
{
public:
    InputData();
    cv::VideoCapture getStream(){return stream;}
    enum MODE
    {
        TEST_MODE = 0,
        NORMAL_MODE = 1
    };
private:
    //Получаем начальные данные (положение полос)
    CvPoint leftPoint1;
    CvPoint leftPoint2;
    CvPoint rightPoint1;
    CvPoint rightPoint2;
    void VideoInput(int);
    cv::VideoCapture stream;
};

#endif // INPUTDATA_H
