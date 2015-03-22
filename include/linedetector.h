#ifndef LINEDETECTOR_H
#define LINEDETECTOR_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <map>

using cv::Vec2i;
using cv::Mat;
using cv::Scalar;
using cv::Point;

/**
@class LineDetector
@brief Данный класс реализует детектор линий.
*/
class LineDetector
{
private:
    int     *lastPos;       /**< @todo Подробное описание*/
    int     sizeLastPos;    /**< @todo Подробное описание*/
    double  alpha;          /**< @todo Подробное описание*/
    double  beta;           /**< @todo Подробное описание*/
    double  gamma;          /**< @todo Подробное описание*/
    int     angle;          /**< @todo Подробное описание*/
    int     delta;          /**< @todo Подробное описание*/
    int     carSpeed;       /**< Скорость автомобиля*/
    int     carAngle;       /**< Угол поворота колес*/
    double  k;              /**< @todo Подробное описание*/
    double  b;              /**< @todo Подробное описание*/
    Mat previousThreshold;  /**< @todo Подробное описание*/
    static cv::Vec4i main_left;  /**< @todo Подробное описание*/
    static cv::Vec4i main_right;  /**< @todo Подробное описание*/
    std::vector<cv::Point> defaultPoints; /**< @todo Подробное описание*/

    /**
      * @brief Метод, ???
      * @param point –
      * @param a –
      * @param b –
      * @param c –
      * @param result –
      * @todo поправить описание
      */
    void distanceLinePoint(cv::Point point, float a, float b, float c, float& result);

    /**
      * @brief Метод, ???
      * @param line –
      * @param point1 -
      * @param point2 -
      * @todo поправить описание
      */
    void convectVec4iToPoints(cv::Vec4i line, cv::Point& point1, cv::Point& point2);

    /**
      * @brief Метод, ???
      * @param point1 -
      * @param point2 -
      * @param line –
      * @todo поправить описание
      */
    void convectPointsToVec4i(cv::Point point1, cv::Point point2, cv::Vec4i& line);

    /**
      * @brief Метод, совершающий огрубление изображения
      * @param image – ссылка на изображение
      * @param div - параметр, отвечащий за уровень огрубления
      * @todo поправить описание
      */
    void colorReduce    (Mat& image, int div=64);

    /**
     * @brief Главный метод, осуществляет поиск линий и расчет скорости автомобиля
     * и улга поворота колес
     * @param srcImg – исходное изображение
     */
    int  houghOpenCV    (const Mat &srcImg);

    /**
     * @brief Метод, расчитывающий скорость автомобиля и углы поворота колес
     * @attention в данный момент незавершен
     * @todo Доделать определение скорости автомобиля и угла поворота колес
     */
    void carDataFinder(cv::Vec4i& main_line, bool right);

    /**
      * @brief Метод, ???
      * @param lines –
      * @param edges -
      * @param temp_frame -
      * @todo поправить описание
      */
    void processLines(std::vector<cv::Vec4i>& lines, Mat edges, Mat temp_frame);

    /**
      * @brief Метод, ???
      * @param lines –
      * @param main_line -
      * @param defaultLine -
      * @param output -
      * @todo поправить описание
      */
    void processSide (std::vector<cv::Vec4i> lines, cv::Vec4i& main_line, cv::Vec4i defaultLine, cv::Mat output);

    /**
      * @brief Метод, ???
      * @param a –
      * @param b -
      * @param c -
      * @param x -
      * @todo поправить описание
      */
    void getCoordinates(float a, float b, float c, int& x, int& y, bool findY);

public:
    /// @name Конструкторы
    /// @{
    /// @brief Конструктор по умолчанию
    LineDetector        ();
    /// @deprecated Конструктор по умолчанию 1
    /// LineDetector        (Mat srcImgOCVROI);
    /// @deprecated Конструктор по умолчанию 2
    /// LineDetector        (IplImage* srcImgOCVROI);
    /// @}

    /// @brief Деструктор по умолчанию
    /// @todo необходимо дописать!
    ~LineDetector        ();

    /**
     * @brief Метод, запускающий детектор линий
     * @param srcImg - исходное изображение
     * @param defaultPoints - ???
     */
    void detectLine     (Mat &srcImg, std::vector<cv::Point>   defaultPoints);

    /** @brief Метод, возвращающий расчитанные даннные скорости автомобиля
     * и угла поворта колес.
     * @return контейнер, содержащий два целочисленных значений в диапазоне
     * от 0 до 100, где первое значение является углом поворота колес, а
     * второе значение - скорость автомобиля
     */
    Vec2i getCarData()
    {
        return Vec2i(carAngle, carSpeed);
    }

};

#endif // LINEDETECTOR_H
