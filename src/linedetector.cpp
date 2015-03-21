#include <include/linedetector.h>

LineDetector::LineDetector()
{
    alpha = 0.5;
    beta = 0.5;
    gamma = 0;
    delta = 5;
    angle = 30;
}

void LineDetector::detectLine(Mat &srcImg, std::vector<cv::Point> defaultPoints)
{
    std::cout << "detectLine..." << std::endl;
    this->defaultPoints = defaultPoints;
    houghOpenCV(srcImg);
}

void LineDetector::colorReduce(Mat& image, int div)
{
    std::cout << "colorReduce " << std::endl;
    int nl = image.rows;                    // number of lines
    int nc = image.cols * image.channels(); // number of elements per line

    for (int j = 0; j < nl; j++)
    {
        // get the address of row j
        uchar* data = image.ptr<uchar>(j);
        for (int i = 0; i < nc; i++)
        {
            // process each pixel
            data[i] = data[i] / div * div + div / 2;
        }
    }
}

int LineDetector::houghOpenCV(const Mat &srcImg)
{
    std::cout << "houghOpenCV " << std::endl;

    Mat grayImg         = Mat( srcImg.size(), CV_8UC1, Scalar (0) );
    Mat gaussImg        = Mat( srcImg.size(), CV_8UC1, Scalar (0) );
    Mat thresholdImg    = Mat( srcImg.size(), CV_8UC1, Scalar (0) );
    Mat cannyImg        = Mat( srcImg.size(), CV_8UC1, Scalar (0) );
    Mat lineImg         = Mat( srcImg.size(), CV_8UC3, Scalar (0,0,0) );
    Mat dstImg          = Mat( srcImg.size(), CV_8UC3, Scalar (0,0,0) );

    // перевод в чб
    cv::cvtColor(srcImg, grayImg, CV_BGR2GRAY);

    // бинаризация по пороговому значению
    //cv::threshold(grayImg, thresholdImg, 127, 255, CV_THRESH_BINARY);

    // наложение c предыдущими кадрами
    /// @todo Доделать наложение либо удалить

    // размытие
    cv::GaussianBlur( grayImg, gaussImg, cv::Size( 5, 5 ), 0, 0 );

    // детектирование границ и наложение кадров
    cv::Canny ( gaussImg, cannyImg, 1, 100);

    // конвертируем в цветное изображение
    cv::cvtColor( cannyImg, dstImg, CV_GRAY2BGR );

    // хранилище памяти для хранения найденных линий
    std::vector<cv::Vec4i> lines;

    // нахождение линий
    cv::HoughLinesP( cannyImg, lines, 1, CV_PI/180, 80, 10, 10 );

    // сортировка и уточнение линии
    processLines(lines, cannyImg, srcImg);

    // показываем
//    cv::namedWindow("Lines", CV_WINDOW_AUTOSIZE);
//    cv::imshow("Lines", lineImg);
//    cv::namedWindow("HoughLines", CV_WINDOW_AUTOSIZE);
//    cv::imshow("HoughLines", dstImg);

    // освобождаем ресурсы
    dstImg.release();
    grayImg.release();
    gaussImg.release();
    cannyImg.release();
    thresholdImg.release();

    return 0;
}

void LineDetector::processLines(std::vector<cv::Vec4i>& lines, Mat edges, Mat temp_frame)
{
    /// @todo сделать сортировку и уточнение линии
    // разделим правые и левые линии
    std::vector<cv::Vec4i> left, right;

    for ( size_t i = 0; i < lines.size(); i++ )
    {
        int dx = lines[i][0] - lines[i][2];
        int dy = lines[i][1] - lines[i][3];
        float angle = atan2f(dy, dx) * 180.0 / CV_PI;

        // Исключаем вертикальные горизонтальные линии
        if (angle <= 10 && angle >= 180 - 10) {
            continue;
        }
        // Исключаем горизонтальные линии
        if (angle <= 90 - 10 && angle >= 90 + 10) {
            continue;
        }

        //сортируем линии по положению середины линии относительно середины изображения
        int midx = (lines[i][0] + lines[i][2]) / 2;

        if (midx < temp_frame.size().width/2)
        {
            left.push_back(cv::Vec4i(lines[i][0], lines[i][1], lines[i][2], lines[i][3]));
        }
        else if (midx > temp_frame.size().width/2)
        {
            right.push_back(cv::Vec4i(lines[i][0], lines[i][1], lines[i][2], lines[i][3]));
        }
    }

    // показываем правые линии
    for( size_t i = 0; i < right.size(); i++ )
    {
        cv::line( temp_frame, Point(right[i][0], right[i][1]),
              Point(right[i][2], right[i][3]), Scalar(255,0,255), 3, 8 );
    }
    // показываем левые линии
    for( size_t i = 0; i < left.size(); i++ )
    {
        cv::line( temp_frame, Point(left[i][0], left[i][1]),
              Point(left[i][2], left[i][3]), Scalar(0,255,255), 3, 8 );
    }

    cv::Vec4i main_left;
    cv::Vec4i main_right;

    //запускаем обработку правых линий для поиска главной линии
    cv::Vec4i rightLine;
    convectPointsToVec4i(defaultPoints[0], defaultPoints[1], rightLine);
    processSide(left, main_left, rightLine);

    //запускаем обработку правых линий для поиска главной линии
    cv::Vec4i leftLine;
    convectPointsToVec4i(defaultPoints[2], defaultPoints[3], leftLine);
    processSide(right, main_right, leftLine);

    // определяем скорость автомобиля и угол поворота колес
    carDataFinder(main_left, false);
    carDataFinder(main_right, true);

    // рисуем главные линии
    cv::line( temp_frame, Point(main_left[0], main_left[1]),
          Point(main_left[2], main_left[3]), Scalar(255,0,0), 3, 8 );
    cv::line( temp_frame, Point(main_right[0], main_right[1]),
          Point(main_right[2], main_right[3]), Scalar(0,255,0), 3, 8 );

    left.clear();
    right.clear();
}

void LineDetector::distanceLinePoint(cv::Point point, float a, float b, float c, float& result)
{
    result = abs(a*point.x + b*point.y + c)/sqrt(powl(a, 2) + powl(b, 2));
}

void LineDetector::processSide(std::vector<cv::Vec4i> lines, cv::Vec4i& main_line, cv::Vec4i defaultLine)
{
    /// @todo переписать этот код
    float a = defaultLine[3] - defaultLine[1];
    float b = defaultLine[0] - defaultLine[2];
    float c = defaultLine[0] * (-a) + defaultLine[1] * (-b);

    float h1 = 10.0;
    float h2 = 10.0;

    float distance_1 = 0;
    float distance_2 = 0;

    for (std::vector<cv::Vec4i>::iterator line = lines.begin(); line < lines.end(); line++)
    {
        cv::Point point1;
        cv::Point point2;
        convectVec4iToPoints(*line, point1, point2);
        distanceLinePoint(point1, a, b, c, distance_1);
        distanceLinePoint(point2, a, b, c, distance_2);
        std::cout<<" dis1 "<<distance_1<<" dis2 "<<distance_2<<std::endl;
        if (distance_1 < h1 || distance_2 < h2)
        {
            h1 = distance_1;
            h2 = distance_2;
            main_line = *line;
        };
    }
}

void LineDetector::carDataFinder(cv::Vec4i& main_line, bool right)
{
    // Прямоугольники 1, 2, 3.
    // Центр прямоугольников - середина main_line
    // Диагональ прямоугольника 3 равна высоте
    // Линейные размеры других прямоуголников уточняются
    //
    // Алгоритм
    // 0) статус = 0
    // 1) Берем линии
    // 2) Вкладываем линию в прямоугольник
    // 3) Проверяем, выходит ли линия за границы прямоугольника
    // 4) Если не выходит, то по статусу определяем
    // 5) Иначе, инкрементируем статус, определяем сторону, с которой выходит и берем следующий прямоугольник -> 2)
    // 6) Если больше прмоугольников нет, то еще раз инкрементируем статус.
    //
    // Статусы:
    // 0 - Движемся на высокой скорости, колеса прямо
    // 1 - Движемся на средней скорости, колеса прямо
    // 2 - Движемся на средней скорости, колеса в нужную сторону на небольшой угол
    // 3 - Движемся на низкой скорости, колеса в нужную сторону на большой угол
}

void LineDetector::convectVec4iToPoints(cv::Vec4i line, cv::Point& point1, cv::Point& point2)
{
    point1.x = line[0];
    point1.y = line[1];
    point2.x = line[2];
    point2.y = line[3];
}
void LineDetector::convectPointsToVec4i(cv::Point point1, cv::Point point2, cv::Vec4i& line)
{
    line[0] = point1.x;
    line[1] = point1.y;
    line[2] = point2.x;
    line[3] = point2.y;
}
