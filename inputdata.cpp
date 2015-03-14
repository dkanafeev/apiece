#include "inputdata.h"

InputData::InputData()
{
    /// @todo доделать получение данных начальных точек либо удалить
    //leftPoint1 = CvPoint (140, 360);
    //leftPoint2 = CvPoint (305, 150);
    //rightPoint1 = CvPoint (500, 360);
    //rightPoint2 = CvPoint (360, 150);

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
                std::cerr << "E: Cannot open test video!" <<std::endl;
                exit(1);
            }
            videoRes.height = (int) stream.get(CV_CAP_PROP_FRAME_HEIGHT);
            videoRes.width  = (int) stream.get(CV_CAP_PROP_FRAME_WIDTH);
            videoROI.x = 0;
            videoROI.y = videoRes.height/2;
            videoROI.width  = videoRes.width  - videoROI.x;
            videoROI.height = videoRes.height - videoROI.y;
            break;
        }
    case NORMAL_MODE:
        {
            stream = cv::VideoCapture (0);
            if (stream.isOpened() == false)
            {
                std::cerr << "E: Cannot open video from camera!" <<std::endl;
                exit(1);
            }
            videoRes.height = (int) stream.get(CV_CAP_PROP_FRAME_HEIGHT);
            videoRes.width  = (int) stream.get(CV_CAP_PROP_FRAME_WIDTH);
            cv::Point pTopLeft  (0, videoRes.height/2); //Top left
            cv::Point pBottomRight (videoRes.width,videoRes.height); //Bottom right
            videoROI.x = 0;
            videoROI.y = videoRes.height/2;
            videoROI.width  = videoRes.width  - videoROI.x;
            videoROI.height = videoRes.height - videoROI.y;
            break;
        }
    default:
        {
            std::cerr << "E: Unknown mode!"  <<std::endl;

        }
    }
}
