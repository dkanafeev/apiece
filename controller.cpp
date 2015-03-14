#include "controller.h"

Controller::Controller()
{
    /// @todo дописать что-нибудь, не должен же быть конструктор пустым?
}

void Controller::start()
{
    // Получение входных данных,
    InputData* inputData = new InputData();

    // получаем данные о видео (разрешение) и ROI, заносим их в параметры класса
    setData(inputData->getVideoSize(), inputData->getVideoROI());

    // получаем начальные условия (положение полос, задается пользователем),
    /// @todo дописать получение начальных условий о положении полос

    // запускаем детектор полосы,
    LineDetector * lineDetector = new LineDetector();

    // запускаем модуль управления автомобилем,
    CarDriver * carDriver = new CarDriver();

    while(cvWaitKey(1) != 27) //Esc
    {

        frameNumber+=1;
        std::cout << "frameNumber=" << frameNumber << std::endl;

        // считываем кадр из потока,
        if (inputData->getStream().read(srcImgOCV) == false)
        {
            // выходим, если кадров больше нет.
            break;
        }

        // обрезаем видеокадр для ускорения работы,
        srcImgOCVROI = srcImgOCV(inputData->getVideoROI());

        // запускаем обработчик полосы,
        lineDetector->detectLine(srcImgOCVROI);

        // отправляем данные на контроллер автомобиля,
        carDriver->sendData(CarDriver::LINE_DETECTOR, lineDetector->getCarData());

        // отображаем изображение ? в окне,
        cv::namedWindow("Src");
        cv::imshow("Src", srcImgOCV);

        // освобождаем память
        srcImgOCV.release();
        srcImgOCVROI.release();
    }
    //завершаем работу
    // останавливаем работу модулей
    /// @todo Завершение работы модулей в Controller::start()
    //выходим
    return;
}

void Controller::setData(cv::Size tvideoRes, cv::Rect tvideoROI)
{
    videoRes    = tvideoRes;
    videoROI    = tvideoROI;
    srcImgOCV   = Mat (videoRes, cv::DataType<uchar>::type);
    frameNumber = 0;
}
