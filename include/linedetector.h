#ifndef LINEDETECTOR_H
#define LINEDETECTOR_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <map>

using cv::Vec2i;
using cv::Vec4i;
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
    int     carSpeed;           /**< Скорость автомобиля*/
    float   carAngle;           /**< Угол поворота колес*/
    Mat previousThreshold;      /**< @todo Подробное описание*/
    Vec4i default_left;     /**< @todo Подробное описание*/
    Vec4i default_right;    /**< @todo Подробное описание*/
    static Vec4i main_left;  /**< @todo Подробное описание*/
    static Vec4i main_right;  /**< @todo Подробное описание*/
    std::vector<Point> defaultPoints; /**< @todo Подробное описание*/

    /**
      * @brief Метод, ???
      * @param point –
      * @param a –
      * @param b –
      * @param c –
      * @param result –
      * @todo поправить описание
      */
    void distanceLinePoint(Point point, float a, float b, float c, float& result);

    /**
      * @brief Метод, ???
      * @param line –
      * @param point1 -
      * @param point2 -
      * @todo поправить описание
      */
    void convectVec4iToPoints(Vec4i line, Point& point1, Point& point2);

    /**
      * @brief Метод, ???
      * @param point1 -
      * @param point2 -
      * @param line –
      * @todo поправить описание
      */
    void convectPointsToVec4i(Point point1, Point point2, Vec4i& line);

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
    void carDataFinder(Mat);

    /**
      * @brief Метод, ???
      * @param lines –      
      * @param temp_frame -
      * @todo поправить описание
      */
    void processLines(std::vector<Vec4i>& lines, Mat temp_frame);

    /**
      * @brief Метод, ???
      * @param lines –
      * @param output -
      * @param isRight -
      * @todo поправить описание
      */
    void processSide(std::vector<Vec4i> lines, cv::Mat output, bool isRight);

    /**
      * @brief Метод, ???
      * @param a –
      * @param b -
      * @param c -
      * @param x -
      * @param y -
      * @param findY -
      * @todo поправить описание
      */
    void getCoordinates(float a, float b, float c, int& x, int& y, bool findY);

public:

    /// @brief Конструктор по умолчанию
    /// @todo необходимо дописать!
    LineDetector        ();

    /// @brief Деструктор по умолчанию
    /// @todo необходимо дописать!
    ~LineDetector        ();

    /**
     * @brief Метод, запускающий детектор линий
     * @param srcImg - исходное изображение
     * @param defaultPoints - ???
     */
    void detectLine     (Mat &srcImg, std::vector<Point>   defaultPoints);

    /**
     * @brief Метод, возвращающий расчитанные даннные скорости автомобиля
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
