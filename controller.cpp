#include "controller.h"

Controller::Controller()
{
    /// @todo дописать что-нибудь, не должен же быть конструктор пустым?
}

void Controller::start()
{
    // Получение входных данных,
    InputData* inputData = new InputData();

    // проверяем видеопоток на наличие данных (считываем первый кадр),
    if (inputData->getStream().read(srcImgOCV) == false)
    {
        // если невозможно считать данные, выходим.
        std::cout << "W: End of video" << std::endl;
        return;
    }

    // получаем данные о видео (разрешение) и ROI, заносим их в параметры класса
    /// @todo дописать получение данных о видео и ROI
    int videoW = 640;
    int videoH = 360;
    int videoW1_ROI = 0;
    int videoH1_ROI = 120;
    int videoW2_ROI = 640;
    int videoH2_ROI = 360;
    CvSize videoRes    = CvSize (videoW, videoH);
    CvPoint cropAreaP1  = cvPoint(videoW1_ROI,	videoH1_ROI);
    CvPoint cropAreaP2  = cvPoint(videoW2_ROI,	videoH2_ROI);
    setData(videoRes, cropAreaP1, cropAreaP2);

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
        Mat srcImgOCVROI = srcImgOCV(cv::Rect(cropAreaP1, cropAreaP2));

        // запускаем обработчик полосы,
        lineDetector->detectLine(srcImgOCVROI);
        srcImgOCVROI.release();

        // отправляем данные на контроллер автомобиля,
        carDriver->sendData(CarDriver::LINE_DETECTOR, lineDetector->getCarData());

        // отображаем изображение ? в окне,
        cv::namedWindow("Src");
        cv::imshow("Src", srcImgOCV);

        srcImgOCV.release();
    }
    return;
}

void Controller::setData(CvSize tvideoRes, CvPoint tcropAreaP1, CvPoint tcropAreaP2)
{
    videoRes    = tvideoRes;
    cropAreaP1  = tcropAreaP1;
    cropAreaP2  = tcropAreaP2;
    srcImgOCV   = Mat (videoRes, cv::DataType<float>::type);
    frameNumber = 0;
}
