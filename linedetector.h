#ifndef LINEDETECTOR_H
#define LINEDETECTOR_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <vector>

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
     * @param srcImg – Изображение, обработанное с помощбю алгоритма поиска линий
     * @param nsensors - Количество сенсоров,по которым будет определяться угол поворта колес
     * @param step - расстояние между сенсорами (шаг)
     * @param start - координата по высоте, определящая положение первого сенсора
     * @attention Необходимо следить, чтобы расстояние между крайними сенорсами вместе с положением
     * первого сенсора находились в границах изображения
     * @return true если все хорошо, false если все плохо
     */
    void carDataFinder(Mat& srcImg, int nsensors, int step, int start = 0);

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
     */
    void detectLine     (Mat &srcImg);

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
