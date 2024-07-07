/**
 *  @file
 *  Modul, welches einen Teil der geforderten Funktionalitäten
 *  aus Aufgabe 3 kapselt.
 *
 *  @author Ilana Schmara, Joshua-Scott Schöttke, Gruppe 14
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include "argparse.hpp"
#include "aufgabe1.hpp"
#include "util.hpp"

// Globale Variable, die die aktuelle Randbehandlung speichert
cv::BorderTypes borderType = cv::BORDER_CONSTANT;

/**
 *
 */
cv::Mat translation(float xDir, float yDir)
{
    cv::Matx33f scalingMatrix(1, 0, xDir,
                              0, 1, yDir,
                              0, 0, 1);
    cv::Mat result(scalingMatrix);
    return result;
}

/**
 *
 */
cv::Mat rotation(double degree)
{
    degree = degree * M_PI / 180.0;
    double cos = trunc(std::cos(degree) * 1000.0) / 1000.0;
    double sin = trunc(std::sin(degree) * 1000.0) / 1000.0;
    cv::Matx33f rotationMatrix(cos, -sin, 0,
                               sin, cos, 0,
                               0, 0, 1);
    cv::Mat result(rotationMatrix);
    return result;
}

/**
 *
 */
cv::Mat scaling(float xDir, float yDir)
{
    cv::Matx33f scalingMatrix(xDir, 0, 0,
                              0, yDir, 0,
                              0, 0, 1);
    cv::Mat result(scalingMatrix);
    return result;
}

/**
 *
*/
cv::Mat affinefit(float length, float height, float x1, float y1, float x2, float y2, float x3, float y3)
{
    cv::Mat mat = cv::Mat::zeros(6, 6, CV_32FC1);
    mat.at<float>(0,0) = x1;
    mat.at<float>(0,1) = y1;
    mat.at<float>(0,2) = 1.0;

    mat.at<float>(1,3) = x1;
    mat.at<float>(1,4) = y1;
    mat.at<float>(1,5) = 1.0;

    mat.at<float>(2,0) = x2;
    mat.at<float>(2,1) = y2;
    mat.at<float>(2,2) = 1.0;

    mat.at<float>(3,3) = x2;
    mat.at<float>(3,4) = y2;
    mat.at<float>(3,5) = 1.0;

    mat.at<float>(4,0) = x3;
    mat.at<float>(4,1) = y3;
    mat.at<float>(4,2) = 1.0;

    mat.at<float>(5,3) = x3;
    mat.at<float>(5,4) = y3;
    mat.at<float>(5,5) = 1.0;

    cv::Matx66f start(mat);

    cv::Matx61f end( 0.0, height, 0.0, 0.0, length, 0.0);

    cv::Matx61f transform = start.solve(end);

    cv::Mat transformMat(transform);

    cv::Mat resMatrix(3,3, CV_32FC1);

    resMatrix.at<float>(0,0) = transformMat.at<float>(0);
    resMatrix.at<float>(0,1) = transformMat.at<float>(1);
    resMatrix.at<float>(0,2) = transformMat.at<float>(2);

    resMatrix.at<float>(1,0) = transformMat.at<float>(3);
    resMatrix.at<float>(1,1) = transformMat.at<float>(4);
    resMatrix.at<float>(1,2) = transformMat.at<float>(5);

    resMatrix.at<float>(2,0) = 0.0;
    resMatrix.at<float>(2,1) = 0.0;
    resMatrix.at<float>(2,2) = 1.0;

    return resMatrix;
}

/**
 *
 */
cv::Mat transform(cv::Mat matrix, cv::Mat img, int nearest)
{
    cv::Matx31f pixelVec(0.0, 0.0, 1.0);
    cv::Matx31f resPixelVec;
    cv::Matx33f matrixCopy(matrix);
    // let border be the same in all directions
    int borderTopBottom = img.rows;
    int borderLeftRight = img.cols;

    // constructs a larger image to fit both the image and the border
    cv::Mat gray_buf(img.rows + borderTopBottom * 2, img.cols + borderLeftRight * 2, img.depth());


    if (borderType == cv::BORDER_CONSTANT)
    {
        // form a border in-place
        cv::copyMakeBorder(img, gray_buf, borderTopBottom, borderTopBottom, borderLeftRight, borderLeftRight, borderType, 0);
    }
    else
    {
        // form a border in-place
        cv::copyMakeBorder(img, gray_buf, borderTopBottom, borderTopBottom, borderLeftRight, borderLeftRight, borderType);
    }

    cv::Mat resImg = newFinalImage(img, matrix);

    cv::Matx33f translated = translation(-((float)resImg.cols) / 2.0, -((float)resImg.rows) / 2.0);
    cv::Matx33f reTranslated = translation(((float)gray_buf.cols) / 2.0 + 0.5, ((float)gray_buf.rows) / 2.0 + 0.5);
   
    for_each_pixel(resImg, [matrixCopy, img, nearest, gray_buf, translated, reTranslated, &pixelVec, &resPixelVec](cv::Mat resImg, int y, int x)
                   {
                        // Vektor, der den Punkt aus dem Zielbild hat
                        cv::Matx31f pixelVec(x + 0.5, y + 0.5, 1.0);

                        // Berechnung des Zielvektors im Quellbild
                        resPixelVec = reTranslated *  matrixCopy.inv() * translated * pixelVec;

                        cv::Mat resPixel(resPixelVec);

                        // Verschiebung um den Mittelpunkt rueckgaengig machen

                        resPixel.at<float>(0, 0) -= 0.5;
                        resPixel.at<float>(1, 0) -= 0.5;

                        if(nearest) 
                        {
                            // Pixel zuweisen 
                            resImg.at<uchar>(y, x) = gray_buf.at<uchar>(int(resPixel.at<float>(1)), int(resPixel.at<float>(0)));
                        } else {
                            float top    = gray_buf.at<uchar>(int(resPixel.at<float>(1)) - 1 , int(resPixel.at<float>(0)) );
                            float bottom = gray_buf.at<uchar>(int(resPixel.at<float>(1)) + 1 , int(resPixel.at<float>(0)) );
                            float left   = gray_buf.at<uchar>(int(resPixel.at<float>(1)) , int(resPixel.at<float>(0)) - 1);
                            float right  = gray_buf.at<uchar>(int(resPixel.at<float>(1)) , int(resPixel.at<float>(0)) + 1);
                            resImg.at<uchar>(y,x) = ((top + bottom + left + right) / 4.0);
                        } });

    return resImg;
}

/**
 * Verkettung von zwei Transformationsmatritzen (Rotation, Skaling)
 */
cv::Mat connect(cv::Mat firstMatrix, cv::Mat secMatrix)
{
    cv::Mat resImg = firstMatrix * secMatrix;
    return resImg;
}

/**
 *
*/
cv::Mat tinyplanet(cv::Mat img)
{
    cv::Mat resImg;
    cv::Size2f dsize(img.cols, img.cols);
    cv::Point2f center((float)(img.cols/2.0),(float)(img.cols/2.0));
    warpPolar(img, resImg, dsize, center, img.cols/2.0, cv::WARP_INVERSE_MAP + cv::WARP_POLAR_LINEAR);
    return resImg;
}

/**
 *  Wechselt die auszufuehrende Randbehandlung der Filter fuer die Bilder.
 *  @param choice Wahl der zu setzenden Randbehandlung: 0 -> null, 1 -> reflect, 2 -> replicate
 */
void edge(int choice)
{
    switch (choice)
    {
    case 0:
        borderType = cv::BORDER_CONSTANT;
        break;
    case 1:
        borderType = cv::BORDER_REFLECT_101;
        break;
    case 2:
        borderType = cv::BORDER_REPLICATE;
        break;
    }
}