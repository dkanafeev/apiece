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
    int nl = image.rows;                    // number of lines
    int nc = image.cols * image.channels(); // number of elements per line

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

    Mat grayImg         = Mat( srcImg.size(), CV_8UC1, Scalar (0) );
    Mat gaussImg        = Mat( srcImg.size(), CV_8UC1, Scalar (0) );
    Mat thresholdImg    = Mat( srcImg.size(), CV_8UC1, Scalar (0) );
    Mat cannyImg        = Mat( srcImg.size(), CV_8UC1, Scalar (0) );
    Mat lineImg         = Mat( srcImg.size(), CV_8UC3, Scalar (0,0,0) );
    Mat dstImg          = Mat( srcImg.size(), CV_8UC3, Scalar (0,0,0) );

    // перевод в чб
    cv::cvtColor(srcImg, grayImg, CV_BGR2GRAY);

    // бинаризация по пороговому значению
    //cv::threshold(grayImg, thresholdImg, 127, 255, CV_THRESH_BINARY);

    // наложение c предыдущими кадрами
    /// @todo Доделать наложение либо удалить

    // размытие
    cv::GaussianBlur( grayImg, gaussImg, cv::Size( 5, 5 ), 0, 0 );

    // детектирование границ и наложение кадров
    cv::Canny ( gaussImg, cannyImg, 250, 200);

    // конвертируем в цветное изображение
    cv::cvtColor( cannyImg, dstImg, CV_GRAY2BGR );

    // хранилище памяти для хранения найденных линий
    std::vector<cv::Vec4i> lines;

    // нахождение линий
    cv::HoughLinesP( cannyImg, lines, 1, CV_PI/180, 80, 10, 10 );

    // сортировка и уточнение линии
    /// @todo сделать сортировку и уточнение линии

    // нарисуем найденные линии
    for( size_t i = 0; i < lines.size(); i++ )
    {
        cv::line( lineImg, Point(lines[i][0], lines[i][1]),
              Point(lines[i][2], lines[i][3]), Scalar(0,255,0), 3, 8 );
        cv::line( srcImg, Point(lines[i][0], lines[i][1]),
              Point(lines[i][2], lines[i][3]), Scalar(0,255,0), 3, 8 );
    }
    // показываем
    cv::namedWindow("Lines", CV_WINDOW_AUTOSIZE);       //create a window with the name "MyWindow"
    cv::imshow("Lines", lineImg);                       //display the image which is stored in the 'img' in the "MyWindow" window
    cv::namedWindow("HoughLines", CV_WINDOW_AUTOSIZE);  //create a window with the name "MyWindow"
    cv::imshow("HoughLines", dstImg);                   //display the image which is stored in the 'img' in the "MyWindow" window

    // определяем скорость автомобиля и угол поворота колес
    carDataFinder();

    // освобождаем ресурсы
    dstImg.release();
    grayImg.release();
    gaussImg.release();
    cannyImg.release();
    thresholdImg.release();

    return 0;
}

void LineDetector::carDataFinder()
{

}
