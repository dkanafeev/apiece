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

    struct Status
    {
        Status(): reset(true), lost(0 ){}
        float k, b;
        bool reset;
        int lost;
    };

    Status laneR, laneL;

    enum{
        SCAN_STEP = 5,			  // in pixels
        LINE_REJECT_DEGREES = 10, // in degrees
        BW_TRESHOLD = 250,		  // edge response strength to recognize for 'WHITE'
        BORDERX = 10,			  // px, skip this much from left & right borders
        MAX_RESPONSE_DIST = 5,	  // px

        CANNY_MIN_TRESHOLD = 1,	  // edge detector minimum hysteresis threshold
        CANNY_MAX_TRESHOLD = 100, // edge detector maximum hysteresis threshold

        HOUGH_TRESHOLD = 50,		// line approval vote threshold
        HOUGH_MIN_LINE_LENGTH = 50,	// remove lines shorter than this treshold
        HOUGH_MAX_LINE_GAP = 100,   // join lines to one with smaller than this gaps

        CAR_DETECT_LINES = 4,    // minimum lines for a region to pass validation as a 'CAR'
        CAR_H_LINE_LENGTH = 10,  // minimum horizontal line length from car body in px

        MAX_VEHICLE_SAMPLES = 30,      // max vehicle detection sampling history
        CAR_DETECT_POSITIVE_SAMPLES = MAX_VEHICLE_SAMPLES-2, // probability positive matches for valid car
        MAX_VEHICLE_NO_UPDATE_FREQ = 15 // remove car after this much no update frames
    };

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

    void processLines(std::vector<cv::Vec4i>& lines, Mat edges, Mat temp_frame);
    void processSide (std::vector<cv::Vec4i> lines, cv::Vec4i& main_line, Mat& edges, bool right);

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
