#ifndef INPUTDATA_H
#define INPUTDATA_H
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

/**
@class InputData
@brief Данный класс реализует возможность по получению данных
@details В данной версии программы, входящими данными являются
 - камера или видеофайл
 - начальное положение полосы
*/
class InputData
{
public:
    /// @brief Конструктор по умолчанию
    InputData();

    /**
     * @brief Метод, возвращающий видеопоток
     * @return stream - видеопоток
     */
    cv::VideoCapture getStream(){return stream;}

    /**
      * @brief Метод, возвращающий разрешение видео
      * @return videoRes - разрешение видео
      */
    cv::Size getVideoSize(){ return videoRes;}
    /**
      * @brief Метод, возвращающий размер интересующей области
      * @return videoROI - интересующая область
      */
    cv::Rect getVideoROI(){ return videoROI;}

    /// @brief Перечисление режимов работы
    enum MODE
    {
        TEST_MODE,          /**< Тестовый режим, чтение из файла*/
        NORMAL_MODE_0,      /**< Нормальный режим, получение изображения с камеры 0*/
        NORMAL_MODE_1       /**< Нормальный режим, получение изображения с камеры 1*/
    };
private:

    cv::VideoCapture stream;    /**< видеопоток */
    cv::Size         videoRes;  /**< разрешение видеокадра */
    cv::Rect         videoROI;  /**< интересующая облать на кадре */

    /**
     * @brief Метод, открывающий видеопоток
     * @param mode - режим работы, выбирается из перечисления MODE
     */
    void VideoInput(int);
};

#endif // INPUTDATA_H
