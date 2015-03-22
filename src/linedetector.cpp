#include <include/linedetector.h>

cv::Vec4i LineDetector::main_left  = cv::Vec4i(0,0,0,0);
cv::Vec4i LineDetector::main_right = cv::Vec4i(0,0,0,0);

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
    int delta_angle = 10;

    for ( size_t i = 0; i < lines.size(); i++ )
    {
        int dx = lines[i][0] - lines[i][2];
        int dy = lines[i][1] - lines[i][3];
        float angle = atan2f(dy, dx);
        angle = angle < 0 ? angle + CV_PI : angle;
        angle =  angle * 180.0 / CV_PI;
        // Исключаем горизонтальные линии
        if (angle <= delta_angle || angle >= 180 - delta_angle) {
            continue;
        }
        // Исключаем веритакльные линии
        if (abs(90 - angle) <= delta_angle) {
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

//    // показываем правые линии
//    for( size_t i = 0; i < right.size(); i++ )
//    {
//        cv::line( temp_frame, Point(right[i][0], right[i][1]),
//              Point(right[i][2], right[i][3]), Scalar(255,0,255), 3, 8 );
//    }
//    // показываем левые линии
//    for( size_t i = 0; i < left.size(); i++ )
//    {
//        cv::line( temp_frame, Point(left[i][0], left[i][1]),
//              Point(left[i][2], left[i][3]), Scalar(0,255,255), 3, 8 );
//    }

    //запускаем обработку правых линий для поиска главной линии
    cv::Vec4i rightLine;
    convectPointsToVec4i(defaultPoints[0], defaultPoints[1], rightLine);
    processSide(left, main_left, rightLine, temp_frame);

    //запускаем обработку правых линий для поиска главной линии
    cv::Vec4i leftLine;
    convectPointsToVec4i(defaultPoints[2], defaultPoints[3], leftLine);
    processSide(right, main_right, leftLine, temp_frame);

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

void LineDetector::processSide(std::vector<cv::Vec4i> lines, cv::Vec4i& main_line, cv::Vec4i defaultLine, cv::Mat output)
{    
    // получаем уравнение прямой defaultLine в виде ax + by + c = 0
    const float aDef = defaultLine[3] - defaultLine[1];
    const float bDef = defaultLine[0] - defaultLine[2];
    const float cDef = defaultLine[0] * (-aDef) + defaultLine[1] * (-bDef);

    // вывод линии на экран
    cv::line(output, Point(defaultLine[0], defaultLine[1]),
              Point(defaultLine[2], defaultLine[3]), Scalar(0,0,255), 3, 8 );

    // начинаем работы по поиску главной линии
    const float h1_const = 50.0;
    const float h2_const = 25.0;
    static float h1 = h1_const;
    static float h2 = h2_const;

    float distance_1 = 0;
    float distance_2 = 0;

    std::vector<cv::Vec4i> goodLines;
    cv::Vec4i tempLine;

    for (std::vector<cv::Vec4i>::iterator line = lines.begin(); line < lines.end(); line++)
    {
        cv::Point point1;
        cv::Point point2;
        convectVec4iToPoints(*line, point1, point2);
        distanceLinePoint(point1, aDef, bDef, cDef, distance_1);
        distanceLinePoint(point2, aDef, bDef, cDef, distance_2);

        // выбираем отрезки, близкие к defaultLine
        if (distance_1 < h1 && distance_2 < h2)
        {
            // ищем их уравнения и записываем в матрицу
            tempLine = *line;
            goodLines.push_back(*line);
        };
    }

    int sum_0 = 0;
    int sum_1 = 0;
    int sum_2 = 0;
    int sum_3 = 0;
    int counter = 0;

    if (goodLines.size() == 0)
    {
        std::cout<<"Line not found! "<<std::endl;
        // расширяем область поиска, если линия не найдена
        h1 += 1.0;
        h2 += 1.0;
        return;
    }
    else if (h1 > h1_const || h2 > h2_const)
    {
        //если линия есть, но область поиска большая, возвращаем ее в начальное состояние
        h1 = h1_const;
        h2 = h2_const;
    }

    tempLine = goodLines.at(0);

    //Для поиска main_line найдем среднее значение точек "хороших" линий
    for (std::vector<cv::Vec4i>::iterator line = goodLines.begin(); line < goodLines.end(); line++)
    {
        // выводим промежуточчный результат - близкие линии
//        cv::line(output, Point((*line)[0], (*line)[1]),
//                  Point((*line)[2], (*line)[3]), Scalar(255,255,0), 3, 8 );
        tempLine = *line;
        sum_0 += (*line)[0];
        sum_1 += (*line)[1];
        sum_2 += (*line)[2];
        sum_3 += (*line)[3];
        ++counter;
    }
    sum_0/=counter;
    sum_1/=counter;
    sum_2/=counter;
    sum_3/=counter;

    // получаем прямую и записываем в main_line
    const float aMain = sum_3 - sum_1;
    const float bMain = sum_0 - sum_2;
    const float cMain = sum_0 * (-aMain) + sum_1 * (-bMain);

    int x;
    int y;

    y = 0;
    getCoordinates(aMain, bMain, cMain, x, y, false);
    main_line[0] = x;
    main_line[1] = y;

    y = output.size().height;
    getCoordinates(aMain, bMain, cMain, x, y, false);
    main_line[2] = x;
    main_line[3] = y;

    goodLines.clear();
}

void LineDetector::getCoordinates(float a, float b, float c, int& x, int& y, bool findY)
{
    // ax + by + c = 0
    if (findY == true)
        y = - (a * x + c) / b ;
    else
        x = - (b * y + c) / a ;
}

void LineDetector::carDataFinder(cv::Vec4i& main_line, bool right)
{
    /// @todo необходимо дописать!
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

void LineDetector::distanceLinePoint(cv::Point point, float a, float b, float c, float& result)
{
    result = abs(a*point.x + b*point.y + c)/sqrt(powl(a, 2) + powl(b, 2));
}
