#ifndef LINEDETECTOR_H
#define LINEDETECTOR_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <vector>

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
    Vec4i default_left;         /**< положение левой линии разметки, определенное пользователем*/
    Vec4i default_right;        /**< положение правой линии разметки, определенное пользователем*/
    static Vec4i main_left;     /**< найденая левая линия разметки*/
    static Vec4i main_right;    /**< найденная правая линия разметки*/

    std::vector<Point> defaultPoints; /**< Точки линий, заданные пользователем*/

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
      * @brief Метод, конвертирущий cv::Vec4i в два cv::Point
      * @param line – входящий cv::Vec4i
      * @param point1 - исходящая точка 1
      * @param point2 - исходящая точка 2
      */
    void convectVec4iToPoints(Vec4i line, Point& point1, Point& point2);

    /**
      * @brief Метод, конвертирующий два cv::Point в cv::Vec4i
      * @param point1 - входящая точка 1
      * @param point2 - входящая точка 1
      * @param line – исходящий cv::Vec4i
      */
    void convectPointsToVec4i(Point point1, Point point2, Vec4i& line);

    /**
      * @brief Метод, совершающий огрубление изображения
      * @param image – ссылка на изображение
      * @param div - параметр, отвечащий за уровень огрубления
      */
    void colorReduce    (Mat& image, int div=64);

    /**
     * @brief Главный метод, осуществляет поиск линий и расчет скорости автомобиля
     * и угла поворота колес
     * @param srcImg – исходное изображение
     */
    int  houghOpenCV    (const Mat &srcImg);

    /**
     * @brief Метод, расчитывающий скорость автомобиля и углы поворота колес
     * @param cv::Mat для вывода найденой информации
     */
    void carDataFinder(Mat);

    /**
      * @brief Метод, обрабатывающий линии, найденые с помощью преобразования Hough
      * @param lines – вектор найденых линий
      * @param temp_frame - cv::Mat для вывода найденой информации
      */
    void processLines(std::vector<Vec4i>& lines, Mat temp_frame);

    /**
      * @brief Метод, обрабаытвающщий линии одной стороны полосы
      * @param lines – вектор линий одной стороны полсы
      * @param output - cv::Mat для вывода найденой информации
      * @param isRight - определяет, какую сторону мы будем обрабатывать (true - правую)
      */
    void processSide(std::vector<Vec4i> lines, cv::Mat output, bool isRight);

    /**
      * @brief Метод находит координату x или y, принадлежащую прямой  ax + by + c = 0
      * @param a – коэффициент при x
      * @param b - коэффициент при y
      * @param c - свободный член
      * @param x - ссылка на координату x
      * @param y - ссылка на координату y
      * @param findY - определяет, какую координату искать (true - y, false - x)
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
     * @param defaultPoints - точки линий, заданные пользователем
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
