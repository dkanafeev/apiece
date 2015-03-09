#ifndef LINEDETECTOR_H
#define LINEDETECTOR_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;
using namespace cv;

typedef Vec<uchar, 1> Vec1b;

class LineDetector
{
private:
    int *lastPos;
    int sizeLastPos;
    double  alpha;
    double  beta;
    double  gamma;
    int     angle;
    int     delta;
    int     carSpeed;
    int     carAngle;
    double  k;
    double  b;

    void colorReduce    (Mat& image, int div=64);
    int  houghOpenCV    (const Mat &srcImg);
    void carDataFinder(Mat& srcImg, int nsensors, int step, int start = 0);
    Mat previousThreshold;
public:
    LineDetector        ();
    LineDetector        (Mat srcImgOCVROI);
    LineDetector        (IplImage* srcImgOCVROI);
    void detectLine     (Mat &srcImg);
    Vec2i getCarData()
    {
        return Vec2i(carAngle, carSpeed);
    }


};

#endif // LINEDETECTOR_H
