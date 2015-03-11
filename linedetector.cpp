#include "linedetector.h"

LineDetector::LineDetector()
{
    alpha = 0.5;
    beta = 0.5;
    gamma = 0;
    delta = 5;
    angle = 30;
}

void LineDetector::detectLine(Mat &srcImg)
{
    std::cout << "detectLine..." << std::endl;
    houghOpenCV(srcImg);
}

void LineDetector::colorReduce(Mat& image, int div)
{
    std::cout << "colorReduce " << std::endl;
    int nl = image.rows;                    /// number of lines
    int nc = image.cols * image.channels(); /// number of elements per line

    for (int j = 0; j < nl; j++)
    {
        // get the address of row j
        uchar* data = image.ptr<uchar>(j);
        for (int i = 0; i < nc; i++)
        {
            // process each pixel
            data[i] = data[i] / div * div + div / 2;
        }
    }
}

int LineDetector::houghOpenCV(const Mat &srcImg)
{
    std::cout << "houghOpenCV " << std::endl;

    Mat grayImg         = Mat( srcImg.size(), 8, 1 );
    Mat thresholdImg    = Mat( srcImg.size(), 8, 1 );
    Mat cannyImg        = Mat( srcImg.size(), 8, 1 );
    Mat dstImg          = Mat( srcImg.size(), 8, 3 );

    // перевод в чб
    cv::cvtColor(srcImg, grayImg, CV_BGR2GRAY);

    // бинаризация по пороговому значению
    cv::threshold(grayImg, thresholdImg, 127, 255, CV_THRESH_BINARY);

    // наложение
    /// @todo Доделать наложение или удалить

    // детектирование границ и наложение кадров
    cv::Canny ( grayImg, cannyImg, 250, 200);

    // хранилище памяти для хранения найденных линий
    std::vector<cv::Vec2f> lines;

    // нахождение линий
    cv::HoughLines(cannyImg, lines, 1, CV_PI/180, 150, 0, 0 );

    // конвертируем в цветное изображение
    cv::cvtColor( cannyImg, dstImg, CV_GRAY2BGR );

    // нарисуем найденные линии
    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        if( 1 || theta>CV_PI/180*(180 - angle) || theta<CV_PI/180*(180 + angle))
        {
            Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
            cv::line( dstImg, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
        }
    }

    // хранилище памяти для хранения найденных линий
    std::vector<cv::Vec4i> linesP;

    // нахождение линий
    cv::HoughLinesP( cannyImg, linesP, 1, CV_PI/180, 80, 10, 10 );

    // нарисуем найденные линии
    for( size_t i = 0; i < linesP.size(); i++ )
    {
        cv::line( dstImg, Point(linesP[i][0], linesP[i][1]),
              Point(linesP[i][2], linesP[i][3]), Scalar(0,255,0), 3, 8 );
    }
    // показываем
    cv::namedWindow("HoughLines", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
    cv::imshow("HoughLines", dstImg); //display the image which is stored in the 'img' in the "MyWindow" window

    // определяем скорость автомобиля и угол поворота колес
    /// @todo Доделать определение скорости автомобиля и угла поворота колес
    carDataFinder(dstImg, 25, 5);

    // освобождаем ресурсы
    grayImg.release();
    thresholdImg.release();
    cannyImg.release();
    dstImg.release();
    return 0;
}

void LineDetector::carDataFinder(Mat& srcImg, int nsensors, int step, int start)
{
    ///@todo Доделать, в данный момент метод отключен
    return;

    uint8_t* ptr = (uint8_t*)srcImg.data;
    int cn = srcImg.channels();
    //assert - nsensor*step < src.size().height
    std::vector<Vec2i> bluePix;
    std::vector<Vec2i> greenPix;
    std::vector<Vec2i> redPix;
    std::vector<Vec2i> whitePix;
    for (int y = start; y < nsensors * step; y+=step)
    {
        for (int i =  - delta; i < delta; i++)
        {
            int isBlue = 0;
            int isGreen = 0;
            int isRed = 0;
            int x = (y - b)/k + i;
            int position = ( x * srcImg.cols + y) * cn ;
            if (ptr[position] == 255)
            {
               isBlue = 1;
            }
            if (ptr[ position + 1] == 255)
            {
                isGreen = 1;
            }
            if (ptr[ position + 2] == 255)
            {
                isRed = 1;
            }
            if (!isRed && !isGreen && isBlue)
            {
                //blue pixel
                bluePix.push_back(Vec2i(y, i));

            }
            if (isRed && !isGreen && !isBlue)
            {
                //red pixel
                redPix.push_back(Vec2i(y, i));
            }
            if (!isRed && isGreen && !isBlue)
            {
                //green pixel
                greenPix.push_back(Vec2i(y, i));
            }
            if (isRed && isGreen && isBlue)
            {
                //white pixel
                whitePix.push_back(Vec2i(y, i));
            }
        }
    }
    int sizeGreen = greenPix.size();
    int deltaGreenSum = 0;
    for (int i = 0; i < sizeGreen; i++);
        //deltaGreenSum += greenPix.pop_back();
    deltaGreenSum/=sizeGreen;
}
