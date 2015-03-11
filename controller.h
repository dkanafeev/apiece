#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <linedetector.h>
#include <cardriver.h>
#include <inputdata.h>

/**
@class Controller
@brief Класс, управляющий всеми модулями программы
@todo  дописать детали класса
*/
class Controller
{
private:
    CvSize	videoRes;       /**< разрешение входного видеопотока */
    CvPoint	cropAreaP1;     /**< первая точка ROI*/
    CvPoint	cropAreaP2;     /**< вторая точка ROI */
    Mat     srcImgOCV;      /**< обрабатываемый кадр из видеопотока*/
    int     frameNumber;    /**< счетчик кадров */

public:
    /// @brief Контруктор по умолчанию
    Controller ();

    /// @brief Деструктор по умолчанию
    /// @todo дописать!
    ~Controller (){}

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
    void setData(CvSize tvideoRes, CvPoint tcropAreaP1, CvPoint tcropAreaP2);
};

#endif // CONTROLLER_H
