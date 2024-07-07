/**
 *  @file
 *  Modul, welches einen Teil der geforderten Funktionalitäten
 *  aus Aufgabe 2 kapselt.
 *
 *  @author Ilana Schmara, Joshua-Scott Schöttke, Gruppe 14
 */

#include "aufgabe0.hpp"
#include "aufgabe1.hpp"
#include "aufgabe2.hpp"
#include "argparse.hpp"
#include "util.hpp"
#define _USE_MATH_DEFINES
#include "math.h"



/**
 *  Faltet das Bild anhand eines uebergebenen Filter-Kernels mittels Convolution.
 *  @param kernel der anzuwendende Filter-Kernel
 *  @param img das Bild, welches gefaltet wird
 */
cv::Mat fold(cv::Mat kernel, cv::Mat img)
{
    cv::Mat flippedKernel;
    cv::Mat resImg = cv::Mat(img.rows, img.cols, img.type());
    cv::flip(kernel, flippedKernel, -1);
    cv::filter2D(img, resImg, -1, flippedKernel, cv::Point(-1,-1), 0, cv::BORDER_CONSTANT);
    return resImg;
}

/**
 *  Faltet das Bild anhand eines uebergebenen Filter-Kernels mittels Correlation.
 *  @param kernel der anzuwendende Filter-Kernel
 *  @param img das Bild, welches gefaltet wird
 */
cv::Mat correlate(cv::Mat kernel, cv::Mat img)
{
    cv::Mat resImg = cv::Mat(img.rows, img.cols, img.type());
    cv::filter2D(img, resImg, -1, kernel, cv::Point(-1, -1), 0, cv::BORDER_CONSTANT);
    return resImg;
}

/**
 *  Erstellt den Filter-Kernel eines Box-Filters beliebiger Groesze und gibt diesen zurueck.
 *  @param length die Groesze des Filter-Kernels (Quadratseite)
 *  @return den Filter-Kernel des Box-Filters als Bild
 */
cv::Mat box(int length)
{
    cv::Mat resImg = cv::Mat(length, length, CV_32F, 1/(length*length));
    return resImg;
}

/**
 *  Erstellt den Filter-Kernel eines Laplace-Filters (Groesze 3x3) und gibt diesen zurueck.
 *  @return den Filter-Kernel des Laplace-Filters als Bild
 */
cv::Mat laplace(void)
{
    cv::Mat resImg = cv::Mat(3, 3, CV_32F, 1);
    int count = 0;
    for_each_pixel(resImg, [&count](cv::Mat &resImg, int y, int x)
                   {
        if(count % 2 == 0) {
            resImg.at<float>(y,x) = 0;
        } else {
            resImg.at<float>(y,x) = -1;
        } 
        count++; });

    resImg.at<float>(1, 1) = 4;
    return resImg;
}

/**
 *  Erstellt den Filter-Kernel eines Gauss-Filters - mit beliebiger Groesze und Standardabweichung
 *  für den Radius der Funktion des Filters - und gibt diesen zurueck.
 *  @param length die Groesze des Filter-Kernels (Quadratseite)
 *  @param factor die Standardabweichung fuer die Funktion des Filters
 *  @return den Filter-Kernel des Gauss-Filters als Bild
 */
cv::Mat gaussian(int length, float factor)
{
    cv::Mat resImg = cv::Mat(length, length, CV_32F);
    float gausX = -(length / 2);
    float gausY = -(length / 2);
    float sum = 0;
    for (int y = 0; y < length; y++)
    {
        for (int x = 0; x < length; x++)
        {
            resImg.at<float>(y, x) = std::exp(-((std::pow(gausX, 2) + std::pow(gausY, 2)) / (2 * std::pow(factor, 2))));
            sum += std::exp(-((std::pow(gausX, 2) + std::pow(gausY, 2)) / (2 * std::pow(factor, 2))));
            gausX++;
        }
        gausX = -(length / 2);
        gausY++;
    }

    for_each_pixel(resImg, [sum](cv::Mat &resImg, int y, int x)
                   { resImg.at<float>(y, x) = ((1 / sum) * resImg.at<float>(y, x)); });
    return resImg;
}


/**
 * Funktion, die den Median-Filter mit beliebiger Groesze des Filter-Kernels auf ein uebergebenes Bild anwendet.
 * @param img das Bild, auf welches der Median-Filter angewendet wird
 * @param length die Groesze des Median-Filter-Kernels
 * @return das mit dem Median-Filter gefaltete Bild
 */
cv::Mat median(cv::Mat img, int length)
{
    cv::Mat resImg = cv::Mat(img.rows, img.cols, CV_32F);

    if (length == 1)
    {
        resImg = img;
    }
    else
    {
        cv::Mat floatImg = toFloat(img);
        cv::Mat kernelImg = cv::Mat(length, length, CV_32F, -1);
        std::vector<int> kernelVec{};
        int median = 0;
        for_each_pixel(resImg, [&kernelImg, length, floatImg, &kernelVec, &median](cv::Mat &resImg, int y, int x)
                       {
            kernelVec = createKernelVector(kernelImg, y, x, length, floatImg);

            sort(kernelVec.begin(), kernelVec.end());
        
            median = kernelVec.size();
        
            if((median % 2 == 0))
            {
                median /= 2;
                median -= 1;
            } 
            else 
            {
                median /= 2;
            }
            resImg.at<float>(y,x) = kernelVec[median];
            //nach dem setzen
            kernelImg = cv::Mat(length, length, CV_32F, -1);
            kernelVec.resize(0); });
    }
    resImg = toUChar(resImg);
    return resImg;
}

/**
 * Funktion, die den Erosions-Filter mit einem zu waehlendem Minimum auf ein uebergebenes Bild anwendet.
 * @param img das Bild, auf welches der Erosions-Filter angewendet wird
 * @param min das zu waehlende Minimum mit dem der Erosions-Filter arbeiten soll
 * @return das mit dem Erosions-Filter gefaltete Bild
 */
cv::Mat erode(cv::Mat img, int min)
{
    cv::Mat resImg = cv::Mat(img.rows, img.cols, CV_32F);

    if (min == 1)
    {
        resImg = img;
    }
    else
    {
        cv::Mat floatImg = toFloat(img);
        cv::Mat kernelImg = cv::Mat(min, min, CV_32F, -1);
        std::vector<int> kernelVec{};

        for_each_pixel(resImg, [&kernelImg, min, floatImg, &kernelVec](cv::Mat &resImg, int y, int x)
                       {
            kernelVec = createKernelVector(kernelImg, y, x, min, floatImg);

            sort(kernelVec.begin(), kernelVec.end());
        
            resImg.at<float>(y,x) = kernelVec[0];
            //nach dem setzen
            kernelImg = cv::Mat(min, min, CV_32F, -1);
            kernelVec.resize(0); });
    }
    resImg = toUChar(resImg);
    return resImg;
}

/**
 * Funktion, die den Dilatations-Filter mit einem zu waehlendem Maximum auf ein uebergebenes Bild anwendet.
 * @param img das Bild, auf welches der Dilatations-Filter angewendet wird
 * @param min das zu waehlende Maximum mit dem der Dilatations-Filter arbeiten soll
 * @return das mit dem Dilatations-Filter gefaltete Bild
 */
cv::Mat dilate(cv::Mat img, int max)
{
    cv::Mat resImg = cv::Mat(img.rows, img.cols, CV_32F);

    if (max == 1)
    {
        resImg = img;
    }
    else
    {
        cv::Mat floatImg = toFloat(img);
        cv::Mat kernelImg = cv::Mat(max, max, CV_32F, -1);
        std::vector<int> kernelVec{};

        for_each_pixel(resImg, [&kernelImg, max, floatImg, &kernelVec](cv::Mat &resImg, int y, int x)
                       {
            kernelVec = createKernelVector(kernelImg, y, x, max, floatImg);

            sort(kernelVec.begin(), kernelVec.end());

            resImg.at<float>(y,x) = kernelVec[kernelVec.size()-1];
            
            //nach dem setzen
            kernelImg = cv::Mat(max, max, CV_32F, -1);
            kernelVec.resize(0); });
    }
    resImg = toUChar(resImg);
    return resImg;
}

/**
 * Entscheidet, ob es sich im uebergebenen Bild um Baeume oder Treppen handelt und gibt diese Information
 * auf der Standardausgabe aus.
 * @param img das Bild, fuer welches die Analyse durchgefuehrt wird
 * @param buffer ein Buffer fuer die Rekursive Erhoehung bei Farbabweichungen, falls kein eindeutiges Ergebnis erzielt werden konnte
 */
void stairTreeDecider(cv::Mat img, int buffer)
{
    int horizontal = 0;
    int vertical = 0;
    int countHorizontal = 0;
    int countVertical = 0;
    for_each_pixel(img, [&horizontal, &countHorizontal, &buffer](cv::Mat &img, int y, int x)
                   {
        if (x == img.cols - 1)
        {
            if (countHorizontal > img.rows / 2)
            {
                horizontal++;
            }
            countHorizontal = 0;
        }

        if (x + 1 < img.cols)
        {
            if ((img.at<uchar>(y, x) <= img.at<uchar>(y, x + 1) + buffer) &&
                (img.at<uchar>(y, x) >= img.at<uchar>(y, x + 1) - buffer))
            {
                countHorizontal++;
            }
        } });

    for (int x = 0; x < img.cols; x++)
    {
        for (int y = 0; y < img.rows; y++)
        {
            if (y == img.rows - 1)
            {
                if (countVertical > img.cols / 2)
                {
                    vertical++;
                }
                countVertical = 0;
            }

            if (y + 1 < img.rows)
            {
                if ((img.at<uchar>(y, x) <= img.at<uchar>(y + 1, x) + buffer) &&
                    (img.at<uchar>(y, x) >= img.at<uchar>(y + 1, x) - buffer))
                {
                    countVertical++;
                }
            }
        }
    }

    if (horizontal > vertical)
    {
        std::cout << "treppe" << std::endl;
    }
    else if (vertical > horizontal)
    {
        std::cout << "wald" << std::endl;
    }
    else
    {
        stairTreeDecider(img, ++buffer);
    }
}

/**
 * Sucht im uebergebenem Bild nach allen Vorkommen des Icons, das im zweiten Bild uebergeben wird und schreibt
 * die Koordinaten aller Vorkommen auf die Standardausgabe.
 * @param img das Bild, in welchem nach dem Icon gesucht wird
 * @param icon das zu suchende Icon
*/
void iconFinder(cv::Mat img, cv::Mat icon)
{
    cv::Mat result;
    std::vector<int> coords = {};

    cv::Mat differ = {};
    cv::Mat smallCopy = icon;
    int sumPixel = 0;

    int iconSize = (icon.rows * icon.cols);


    for_each_pixel(img, [&icon, &coords, iconSize, &differ, &smallCopy, &sumPixel](cv::Mat img, int y, int x)
                   {    
        if(x + icon.cols <= img.cols && y + icon.rows <= img.rows) {

            smallCopy = img(cv::Rect(x,y,icon.cols, icon.rows));
            differ = aDiff(icon, smallCopy);

            for_each_pixel(differ, [&sumPixel](cv::Mat differ, int differY, int differX){
                sumPixel += differ.at<uchar>(differY, differX);
            });
            if (sumPixel/(iconSize) >= 0 && sumPixel/(iconSize) < 5)
            {
                coords.push_back(x + icon.cols/2);
                coords.push_back(y + icon.rows/2);
            }
            sumPixel = 0;
        } else {
            y = img.rows;
        }
        
        });
    if (coords.size() != 0)
    {
        result = cv::Mat(coords).reshape(0,coords.size()/2);
        std::cout << result << std::endl;
    } else {
        std::cout << "[]" << std::endl;
    }
}
