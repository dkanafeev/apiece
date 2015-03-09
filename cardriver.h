#ifndef CARDRIVER_H
#define CARDRIVER_H

#include <QtSerialPort/QSerialPort>
#include <opencv2/core.hpp>

using cv::Vec2i;

class CarDriver
{
public:
    CarDriver();
    enum Sender
    {
        LINE_DETECTOR
    };
    void sendData(Sender, Vec2i);

};

#endif // CARDRIVER_H
