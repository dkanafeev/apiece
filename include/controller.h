#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <include/linedetector.h>
#include <include/cardriver.h>
#include <include/inputdata.h>

/**
@class Controller
@brief Класс, управляющий всеми модулями программы
@todo  дописать детали класса
*/
class Controller
{
private:
    cv::Size        videoRes;       /**< разрешение входного видеопотока */
    cv::Rect        videoROI;       /**< интересующая облать на кадре */
    Mat             srcImgOCV;      /**< обрабатываемый кадр из видеопотока */
    Mat             srcImgOCVROI;   /**< обрезанный кадр из видеопотока */
    int             frameNumber;    /**< счетчик кадров  */
    bool            isWork;         /**< Общее состояние ??? */
    bool            isRun;          /**< Запущена ли основная работа? ??? */
    InputData*      inputData;      /**< Указатель на объект входных данных ??? */
    LineDetector*   lineDetector;   /**< Указатель на детектор линий ??? */
    CarDriver*      carDriver;      /**< Указатель на объект исходящих данных  ??? */

    std::vector<cv::Point>   defaultPoints; /**< определяет начальное положение полосы (4 точки)*/

    /// @brief Метод, который записывает в вектор начальные точки
    void readDefaultPoints(int event, int x, int y);

    /// @brief Метод, который читает изображения
    void readImage();

    /// @brief Метод, который проверяет нажатие клавиш
    void keyStatus();

    /// @brief Метод, который рисует фигуру с начальными значениями
    void drawDefault();


public:
    /// @brief Контруктор по умолчанию
    Controller ();

    /// @brief Деструктор по умолчанию
    /// @todo дописать!
    ~Controller ();

    /**
     * @brief Метод, запускающий работу программы
     * @details этот метод запускает поочередно все модули
     * а затем запускает циклическою обработку изображений из видеопотока
     * Выход из цикла производится нажанием клавиши Esc
     */
    void start();

    /**
     * @brief Метод, записывающий в параметры данные об входном потоке и RO
     * @details  ROI необходим для ускорения обработки изображения
     * путем выделения области, в которой расположена дорога и разметка.
     * @param tvideoRes - разрешение изображения
     * @param tcropAreaP1 - первая точка ROI
     * @param tcropAreaP2 - вторая точка ROI
     */
    void setData(cv::Size tvideoRes, cv::Rect tvideoROI);

    /// @brief обработчки сигналов от мыши, чтение координат начальных точек
    /// CallBackFunc
    static void onMouse(int event, int x, int y, int, void* userdata);
};

#endif // CONTROLLER_H
