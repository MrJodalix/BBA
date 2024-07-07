/**
 *  @file
 *  Modul für ungebundene Hilfsfunktionen.
 *
 *  @author Ilana Schmara, Joshua-Scott Schöttke, Gruppe 14
 */

#include "util.hpp"

/**
 *  Führt den uebergebenen Funktor auf allen Pixel eines Bildes aus.
 *  @param img das Bild, auf dessen Pixeln gearbeitet wird
 *  @param filter der Funktor, welcher auf allen Pixeln angewendet wird
 */
void for_each_pixel(cv::Mat &img, std::function<void(cv::Mat &, int, int)> filter)
{

  for (int y = 0; y < img.rows; y++)
  {
    for (int x = 0; x < img.cols; x++)
    {
      filter(img, y, x);
    }
  }
}

/**
 * Erstellt einen quadratischen Kernel der ueber ein gegebenes Bild erstellt wird.
 * @param kernelImg der Kernel der bearbeitet und verändert wird
 * @param y die y-Achse des original Bildes
 * @param x die x-Achse des original Bildes
 * @param length die Laenge und Breite des Kernel. Soll nur ungerade sein.
 * @param floatImg das original Bild als Float Bild
 * @return die positiven Werte des Kernel in einem Vector
 */
std::vector<int> createKernelVector(cv::Mat kernelImg, int y, int x, int length, cv::Mat floatImg)
{
  std::vector<int> kernelVec{};
  for_each_pixel(kernelImg, [y, x, length, floatImg, &kernelVec](cv::Mat &kernelImg, int medY, int medX)
                 {
                if( (y + medY - length/2 >= 0 && x + medX - length/2 >= 0) && 
                (y + medY - length/2 < floatImg.rows && x + medX - length/2 < floatImg.cols))
                {
                    kernelImg.at<float>(medY, medX) = floatImg.at<float>(y + medY - length/2,x + medX - length/2);

                    kernelVec.push_back(kernelImg.at<float>(medY, medX));
                } });
  return kernelVec;
}

/**
 *
 */
cv::Mat newFinalImage(cv::Mat img, cv::Mat matrix)
{
  std::vector<cv::Mat> lengthCoord;
  int rows = img.rows;
  int cols = img.cols;
  double xmin = 0.0;
  double ymin = 0.0;
  double xmax = 0.0;
  double ymax = 0.0;

  cv::Matx31f lengthVec1(0.0, 0.0, 1.0);
  lengthCoord.push_back(matrix * lengthVec1);

  cv::Matx31f lengthVec2(0.0, rows, 1.0);
  lengthCoord.push_back(matrix * lengthVec2);

  cv::Matx31f lengthVec3(cols, 0.0, 1.0);
  lengthCoord.push_back(matrix * lengthVec3);

  cv::Matx31f lengthVec4(cols, rows, 1.0);
  lengthCoord.push_back(matrix * lengthVec4);

  for (int i = 0; i < 4; i++)
  {
    if (xmin > lengthCoord.at(i).at<float>(0, 0))
    {
      xmin = lengthCoord.at(i).at<float>(0, 0);
    }
    if (xmax < lengthCoord.at(i).at<float>(0, 0))
    {
      xmax = lengthCoord.at(i).at<float>(0, 0);
    }
    if (ymin > lengthCoord.at(i).at<float>(0, 1))
    {
      ymin = lengthCoord.at(i).at<float>(0, 1);
    }
    if (ymax < lengthCoord.at(i).at<float>(0, 1))
    {
      ymax = lengthCoord.at(i).at<float>(0, 1);
    }
  }

  cv::Mat resImg = cv::Mat::zeros(ceil(abs(ymin) + abs(ymax)), ceil(abs(xmin) + abs(xmax)), CV_8UC1);
  return resImg;
}