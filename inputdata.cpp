#include "inputdata.h"

InputData::InputData()
{
    leftPoint1 = CvPoint (140, 360);
    leftPoint2 = CvPoint (305, 150);
    rightPoint1 = CvPoint (500, 360);
    rightPoint2 = CvPoint (360, 150);

    VideoInput(TEST_MODE);
}

void InputData::VideoInput(int mode = TEST_MODE)
{
    switch (mode)
    {
    case TEST_MODE:
        {
            stream = cv::VideoCapture ("E:/Test.mp4");
            if (stream.isOpened() == false)
            {
                printf ("E: Cannot open test video! \n");
                exit(1);
            }
            break;
        }
    case NORMAL_MODE:
        {
            stream = cv::VideoCapture (0);
            if (stream.isOpened() == false)
            {
                printf ("E: Cannot open video! \n");
                exit(1);
            }
            break;
        }
    default:
        {
            printf ("E: Unknown mode! \n");
        }
    }
}
