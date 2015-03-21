#include <include/controller.h>

Controller::Controller()
{
    // Получение входных данных,
    inputData = new InputData();

    // запускаем детектор полосы,
    lineDetector = new LineDetector();

    // запускаем модуль управления автомобилем,
    carDriver = new CarDriver();

    // получаем данные о видео (разрешение) и ROI, заносим их в параметры класса
    setData(inputData->getVideoSize(), inputData->getVideoROI());

    // получаем начальные условия (положение полос, задается пользователем),
    /// @todo дописать получение начальных условий о положении полос

    //Test
    readImage();

    isWork = true;
    isRun = false;
}

Controller::~Controller()
{
    srcImgOCV.release();
    srcImgOCVROI.release();
    // останавливаем работу модулей
}

void Controller::start()
{
    //Создаем окна для вывода
    cv::namedWindow("SRC");
    cv::namedWindow("SRC_ROI");
    cv::setMouseCallback("SRC_ROI", onMouse, this);

    // отображаем изображение исходное в окне,
    cv::imshow("SRC", srcImgOCV);

    // отображаем изображение исходное обрезаное в окне,
    cv::imshow("SRC_ROI", srcImgOCVROI);

    while (isWork)
    {

        keyStatus();

        if (isRun && defaultPoints.size() == 4)
        {
            frameNumber+=1;
            std::cout << "frameNumber=" << frameNumber << std::endl;

            //Получаем изображение
            readImage();

            // запускаем обработчик полосы,
            lineDetector->detectLine(srcImgOCVROI, defaultPoints);

            // отправляем данные на контроллер автомобиля,
            carDriver->sendData(CarDriver::LINE_DETECTOR, lineDetector->getCarData());

            //Рисуем линии зоны полосы по умолчанию
            drawDefault();

            // отображаем изображение исходное в окне,
            cv::imshow("SRC", srcImgOCV);

            // отображаем изображение исходное обрезаное в окне,
            cv::imshow("SRC_ROI", srcImgOCVROI);
        }

    }

    //выходим
}

void Controller::setData(cv::Size tvideoRes, cv::Rect tvideoROI)
{
    videoRes    = tvideoRes;
    videoROI    = tvideoROI;
    srcImgOCV   = Mat (videoRes, cv::DataType<uchar>::type);
    frameNumber = 0;
}

void Controller::readImage()
{
    // освобождаем память
    srcImgOCV.release();
    srcImgOCVROI.release();
    // считываем кадр из потока,
    if (inputData->getStream().read(srcImgOCV) == false)
    {
        // выходим, если кадров больше нет.
        std::cerr << "Error: No frames in stream! Exit..." << std::endl ;
        exit (1);
    }
    // обрезаем видеокадр для ускорения работы,
    srcImgOCVROI = srcImgOCV(inputData->getVideoROI());
}

void Controller::keyStatus()
{
    switch (cv::waitKey(1))
    {
    case 27://esc
    case 'q':
    {
        isWork = false;
        break;
    }
    case 'r':
    {
        isRun = true;
        break;
    }
    case 's':
    {
        isRun = false;
        break;
    }
    case 'n':
    {
        readImage();
        break;
    }
    default:
    {
        //пока ничего
    }
    }
}

void Controller::drawDefault()
{
    return;
    /// @todo Может есть другой вариант этого кода?
    /// @code
    /// @{
    std::vector<cv::Point>::iterator it1 = defaultPoints.begin();
    std::vector<cv::Point>::iterator it2 = ++defaultPoints.begin();
    std::vector<cv::Point>::iterator it3 = ++++defaultPoints.begin();
    std::vector<cv::Point>::iterator it4 = ++++++defaultPoints.begin();

    cv::line( srcImgOCVROI, *it1, *it2, Scalar(0,0,255), 3, 8 );
    cv::line( srcImgOCVROI, *it2, *it3, Scalar(0,0,255), 3, 8 );
    cv::line( srcImgOCVROI, *it3, *it4, Scalar(0,0,255), 3, 8 );
    cv::line( srcImgOCVROI, *it4, *it1, Scalar(0,0,255), 3, 8 );
    /// @}
}


void Controller::onMouse(int event, int x, int y, int, void* userdata)
{
    assert (userdata != nullptr);

    Controller* controller = reinterpret_cast<Controller*>(userdata);
    controller->readDefaultPoints(event, x, y);
}

void Controller::readDefaultPoints(int event, int x, int y)
{
     if  ( event == cv::EVENT_LBUTTONDOWN )
     {
        std::cout << "Left button of the mouse is clicked" << std::endl;
        std::cout << "Position (" << x << ", " << y << ")" << std::endl;
        if (defaultPoints.size() < 4)
        {
            defaultPoints.push_back(cv::Point(x,y));
        }
        else
        {
            std::cout << "New vect" << std::endl;
            defaultPoints.clear();
            defaultPoints.push_back(cv::Point(x,y));
        }

     }
}
