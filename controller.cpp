#include "controller.h"

Controller::Controller()
{
    videoW = 640;
    videoH = 360;

    videoW1_ROI = 0;
    videoH1_ROI = 120;
    videoW2_ROI = 640;
    videoH2_ROI = 360;
    videoRes    = CvSize (videoW, videoH);
    cropAreaP1  = cvPoint(videoW1_ROI,	videoH1_ROI);
    cropAreaP2  = cvPoint(videoW,	videoH);
    srcImgOCV   = Mat (videoRes, DataType<float>::type);
    frameNumber = 0;
}

int Controller::start()
{
    //Получение входных данных
    InputData* inputData = new InputData();
    //Проверка видео
    if (inputData->getStream().read(srcImgOCV) == false)
    {
        printf("W: End of video\n");
        return 1; //end of video
    }
    //Получаем данные о видео

    //Получаем начальные условия (задаются пользователем)

    //Запускаем детектор полосы
    LineDetector * lineDetector = new LineDetector();
    //Запускаем модуль управления автомобилем
    CarDriver * carDriver = new CarDriver();

    while(cvWaitKey(1) != 27) //Esc
    {

        frameNumber+=1;
        printf	("frameNumber=%d\n", frameNumber);

        if (inputData->getStream().read(srcImgOCV) == false)
            break; //end of video
        //для ускорения работы обрезаем видеокадр
        Mat srcImgOCVROI = srcImgOCV(Rect(cropAreaP1, cropAreaP2));
        lineDetector->detectLine(srcImgOCVROI);
        srcImgOCVROI.release();

        //отправляем данные на контроллер автомобиля
        carDriver->sendData(CarDriver::LINE_DETECTOR, lineDetector->getCarData());

        namedWindow("Src");
        imshow("Src", srcImgOCV);

        srcImgOCV.release();
    }
    return 0;
}
