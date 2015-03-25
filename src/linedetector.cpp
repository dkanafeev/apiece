#include <include/linedetector.h>

//необходимо инициализировать для работы с mouseCallBack
cv::Vec4i LineDetector::main_left  = cv::Vec4i(0,0,0,0);
cv::Vec4i LineDetector::main_right = cv::Vec4i(0,0,0,0);

LineDetector::LineDetector()
{
}

void LineDetector::detectLine(Mat &srcImg, std::vector<Point> defaultPoints)
{
    this->defaultPoints = defaultPoints;
    houghOpenCV(srcImg);
}

void LineDetector::colorReduce(Mat& image, int div)
{
    int nl = image.rows;                    // количество линий
    int nc = image.cols * image.channels(); // количество элементов в одной линии

    for (int j = 0; j < nl; j++)
    {
        // получчаем адрес линии j
        uchar* data = image.ptr<uchar>(j);
        for (int i = 0; i < nc; i++)
        {
            // считаем для каждого пискеля
            data[i] = data[i] / div * div + div / 2;
        }
    }
}

int LineDetector::houghOpenCV(const Mat &srcImg)
{
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
    std::vector<Vec4i> lines;

    // нахождение линий
    cv::HoughLinesP( cannyImg, lines, 1, CV_PI/180, 80, 10, 10 );

    // сортировка и уточнение линии
    processLines(lines, srcImg);

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
    lineImg.release();
    thresholdImg.release();

    return 0;
}

void LineDetector::processLines(std::vector<Vec4i>& lines, Mat temp_frame)
{
    // разделим правые и левые линии
    std::vector<Vec4i> left, right;
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
            left.push_back(Vec4i(lines[i][0], lines[i][1], lines[i][2], lines[i][3]));
        }
        else if (midx > temp_frame.size().width/2)
        {
            right.push_back(Vec4i(lines[i][0], lines[i][1], lines[i][2], lines[i][3]));
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
    convectPointsToVec4i(defaultPoints[0], defaultPoints[1], default_left);
    processSide(left, temp_frame, false);

    //запускаем обработку правых линий для поиска главной линии
    convectPointsToVec4i(defaultPoints[2], defaultPoints[3], default_right);
    processSide(right, temp_frame, true);

    // определяем скорость автомобиля и угол поворота колес
    carDataFinder(temp_frame);
    carDataFinder(temp_frame);

    // рисуем главные линии
    cv::line( temp_frame, Point(main_left[0], main_left[1]),
          Point(main_left[2], main_left[3]), Scalar(255,0,0), 3, 8 );
    cv::line( temp_frame, Point(main_right[0], main_right[1]),
          Point(main_right[2], main_right[3]), Scalar(0,255,0), 3, 8 );

    left.clear();
    right.clear();
}

void LineDetector::processSide(std::vector<Vec4i> lines, cv::Mat output, bool isRight)
{
    Vec4i& default_line = isRight ? default_right : default_left;
    Vec4i& main_line = isRight ? main_right : main_left;

    // получаем уравнение прямой default_line в виде ax + by + c = 0
    const float aDef = default_line[3] - default_line[1];
    const float bDef = default_line[0] - default_line[2];
    const float cDef = default_line[0] * (-aDef) + default_line[1] * (-bDef);

    // преобразование отрезков, на весь roi
    int x;
    int y;

    y = 0;
    getCoordinates(aDef, bDef, cDef, x, y, false);
    default_line[0] = x;
    default_line[1] = y;

    y = output.size().height;
    getCoordinates(aDef, bDef, cDef, x, y, false);
    default_line[2] = x;
    default_line[3] = y;

    // вывод линии на экран
    cv::line(output, Point(default_line[0], default_line[1]),
              Point(default_line[2], default_line[3]), Scalar(0,0,255), 3, 8 );

    // начинаем работы по поиску главной линии
    const float h1_const = 50.0;
    const float h2_const = 25.0;
    static float h1 = h1_const;
    static float h2 = h2_const;

    float distance_1 = 0;
    float distance_2 = 0;

    std::vector<Vec4i> goodLines;
    Vec4i tempLine;

    for (std::vector<Vec4i>::iterator line = lines.begin(); line < lines.end(); line++)
    {
        Point point1;
        Point point2;
        convectVec4iToPoints(*line, point1, point2);
        distanceLinePoint(point1, aDef, bDef, cDef, distance_1);
        distanceLinePoint(point2, aDef, bDef, cDef, distance_2);

        // выбираем отрезки, близкие к default_line
        if (distance_1 < h1 && distance_2 < h2)
        {
            // запоминаем их
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
        //если линия есть, но область поиска большая, возвращаем область в начальное состояние
        h1 = h1_const;
        h2 = h2_const;
    }

    //Для поиска main_line найдем среднее значение точек "хороших" линий
    for (std::vector<Vec4i>::iterator line = goodLines.begin(); line < goodLines.end(); line++)
    {
        // выводим промежуточчный результат - близкие линии
//        cv::line(output, Point((*line)[0], (*line)[1]),
//                  Point((*line)[2], (*line)[3]), Scalar(255,255,0), 3, 8 );
        sum_0 += (*line)[0];
        sum_1 += (*line)[1];
        sum_2 += (*line)[2];
        sum_3 += (*line)[3];
        ++counter;
    }
    sum_0 /= counter;
    sum_1 /= counter;
    sum_2 /= counter;
    sum_3 /= counter;

    // получаем прямую

    const float aMain = sum_3 - sum_1;
    const float bMain = sum_0 - sum_2;
    const float cMain = sum_0 * (-aMain) + sum_1 * (-bMain);

    // Примитвино проверяем ее на правильность - уг

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

void LineDetector::carDataFinder(cv::Mat output)
{

    int deltaUM = (main_right[0] + main_left[0]) / 2;
    int deltaDD = (default_right[2] + default_left[2]) / 2;

    cv::line(output, Point(deltaUM, 0),
              Point(deltaDD, output.size().height), Scalar(0,255,255), 3, 8 );
    carAngle = atan2 (main_left[3], (deltaDD - deltaUM)) * 180.0 / CV_PI - 45;

    carSpeed = abs(carAngle - 45) < 8 ? 20 : abs(carAngle - 45) < 16 ? 10 : 5;

    std::ostringstream strA;
    strA << " Car angle: " << carAngle;
    cv::putText(output, strA.str(), Point(5,50), CV_FONT_HERSHEY_PLAIN, 1, Scalar(0,0,250));

    std::ostringstream strS;
    strS << " Car speed: " << carSpeed;
    cv::putText(output, strS.str(), Point(5,65), CV_FONT_HERSHEY_PLAIN, 1, Scalar(0,0,250));

    std::ostringstream strD;
    strD << " Direction: " << ( (carAngle > 48) ? "/// RIGHT" : (carAngle < 42) ? "\\\\\\ LEFT" : "||| F" );
    cv::putText(output, strD.str(), Point(5,80), CV_FONT_HERSHEY_PLAIN, 1, Scalar(0,0,250));

    return;
}

void LineDetector::convectVec4iToPoints(Vec4i line, Point& point1, Point& point2)
{
    point1.x = line[0];
    point1.y = line[1];
    point2.x = line[2];
    point2.y = line[3];
}
void LineDetector::convectPointsToVec4i(Point point1, Point point2, Vec4i& line)
{
    line[0] = point1.x;
    line[1] = point1.y;
    line[2] = point2.x;
    line[3] = point2.y;
}

void LineDetector::distanceLinePoint(Point point, float a, float b, float c, float& result)
{
    result = abs(a*point.x + b*point.y + c)/sqrt(powl(a, 2) + powl(b, 2));
}
