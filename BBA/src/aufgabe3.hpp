/**
 *  @file
 *  Schnittstellen-Modul, welches einen Teil der geforderten Funktionalitäten
 *  aus Aufgabe 3 kapselt.
 *
 *  @author Ilana Schmara, Joshua-Scott Schöttke, Gruppe 14
 */

#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/base.hpp>

/**
 * 
 */
cv::Mat rotation(double degree);

/**
 * 
 */
cv::Mat scaling(float xDir, float yDir);

/**
 * 
 */
cv::Mat affinefit(float length, float height, float x1, float y1, float x2, float y2, float x3, float y3);

/**
 * 
 */
cv::Mat transform(cv::Mat matrix, cv::Mat img, int nearest);

/**
 * Verkettung von zwei Transformationsmatritzen (Rotation, Skaling)
 */
cv::Mat connect(cv::Mat firstMatrix, cv::Mat secMatrix);

/**
 * 
 */
cv::Mat tinyplanet(cv::Mat img);
