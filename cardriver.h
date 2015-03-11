#ifndef CARDRIVER_H
#define CARDRIVER_H

#include <QtSerialPort/QSerialPort>
#include <opencv2/core.hpp>

using cv::Vec2i;

/**
@class CarDriver
@brief Данный класс реализует отправку данных на микрокотроллер
@details В данной версии программы, отправляются данные следующие данные:
 - данные о скорости
 - данные об угле поворота колес
*/
class CarDriver
{
public:
    /// @brief Конструктор по умолчанию
    CarDriver();
    /// @brief Перечисление отправителей
    /// @attention Данная версия программы подразумевает наличие только
    /// одного отправителя - детектор линий.
    /// Для примера создана еще одна переменная для детектора знаков.
    enum Sender
    {
        LINE_DETECTOR, /**< детектор линий */
        SIGN_DETECTOR /**< детектор знаков, для примера*/
    };
    /// @brief Отправляет даные на микроконтроллер
    void sendData(Sender, Vec2i);

};

#endif // CARDRIVER_H
