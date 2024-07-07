/**
 *  @file
 *  Modul, welches einen Teil der geforderten Funktionalitäten
 *  aus Aufgabe 0 kapselt.
 * 
 *  Fasst die PNM Ausgabe und den Filter Gradient in Funktionen zusammen.
 * 
 *  @author Ilana Schmara, Joshua-Scott Schöttke, Gruppe 14
 */

#include "aufgabe0.hpp"
#include "util.hpp"

/**
 *  Hilfsfunktion - Berechnet die nötige Menge der Leerstellen zwischen
 *  den einzelnen Pixel-Werten. Benötigt für printpnm.
 *  @param img das Bild, was untersucht wird
 *  @return die Menge der Leerstellen
 */
int widthCalc(cv::Mat img)
{
    int maxValueA = 0,
        maxValueB = 0,
        maxValueG = 0,
        maxValueR = 0,
        whiteSpaceWidth = 1;

    // bestimmt ob Grau oder Farbkanal benutzt wird
    if (img.channels() == 1)
    {
        for_each_pixel(img, [&maxValueA](cv::Mat &img, int y, int x)
                       {
        if (img.at<uchar>(y, x) > maxValueA)
          {
            // zieht den Farbwert aus dem Bild
            maxValueA = img.at<uchar>(y, x);
          } });
        whiteSpaceWidth = maxValueA;
    }
    else
    {
        for_each_pixel(img, [&maxValueB, &maxValueG, &maxValueR](cv::Mat &img, int y, int x)
                       {
        if (img.at<cv::Vec3b>(y, x)[0] > maxValueB)
          {
            // zieht den Farbwert fuer Blau aus dem Bild
            maxValueB = img.at<cv::Vec3b>(y, x)[0];
          }
          if (img.at<cv::Vec3b>(y, x)[1] > maxValueG)
          {
            // zieht den Farbwert fuer Gruen aus dem Bild
            maxValueG = img.at<cv::Vec3b>(y, x)[1];
          }
          if (img.at<cv::Vec3b>(y, x)[2] > maxValueR)
          {
            // zieht den Farbwert fuer Rot aus dem Bild
            maxValueR = img.at<cv::Vec3b>(y, x)[2];
          } });
        whiteSpaceWidth = std::max(maxValueB, maxValueR);
        whiteSpaceWidth = std::max(whiteSpaceWidth, maxValueG);
    }
    if (whiteSpaceWidth >= 100)
    {
        whiteSpaceWidth = 3;
    }
    else if (whiteSpaceWidth >= 10)
    {
        whiteSpaceWidth = 2;
    }
    else
    {
        whiteSpaceWidth = 1;
    }
    return whiteSpaceWidth;
}

/**
 *  Gibt ein Bild im Portable-Anymap-Format auf der Konsole aus.
 *  @param img das Bild, welchen als PNM dargestellt wird
 */
void printpnm(cv::Mat img)
{
    int whiteSpaceWidth = widthCalc(img);
    if (img.channels() == 1)
    {
        std::cout << "P2" << std::endl;
        std::cout << img.cols << " " << img.rows << std::endl;
        // Gibt den Wert der höchsten Helligkeit im Bild an
        std::cout << 255 << std::endl;

        for (int y = 0; y < img.rows; y++)
        {
            for (int x = 0; x < img.cols; x++)
            {
                int pix = img.at<uchar>(y, x);
                std::cout << std::setw(whiteSpaceWidth) << pix;
                if (!(x + 1 > img.cols))
                {
                    std::cout << " ";
                }
            }
            std::cout << "\n";
        }
    }
    else
    {
        std::cout << "P3" << std::endl;
        std::cout << img.cols << " " << img.rows << std::endl;
        std::cout << 255 << std::endl;

        for (int y = 0; y < img.rows; y++)
        {
            for (int x = 0; x < img.cols; x++)
            {
                for (int c = 2; c >= 0; c--)
                {
                    int pix = img.at<cv::Vec3b>(y, x)[c];
                    std::cout << std::setw(whiteSpaceWidth) << pix;
                    if (!(c == 0))
                    {
                        std::cout << " ";
                    }
                }
                if (!(x + 1 >= img.cols))
                {
                    std::cout << "   ";
                }
            }
            std::cout << "\n";
        }
    }
}

/**
 *  Filter, der aus einem Leerbild ein Bild mit einem weichen Farb-Verlauf
 *  von schwarz nach weiss erzeugt.
 *  @param img das Leerbild
 *  @param s Summand s für die Berechnung
 *  @param t Summand t für die Berechnung
 *  @param u Faktor u für die Berechnung
 *  @param v Faktor v für die Berechnung
 */
void gradient(cv::Mat img, float s, float t, float u, float v)
{
    int pixel = 0;
    for_each_pixel(img, [s, t, u, v, &pixel](cv::Mat &img, int y, int x)
                   {
        pixel = (x - s) * u + (y - t) * v;
        if (pixel > 255)
        {
          img.at<uchar>(y, x) = 255;
        }
        else if (pixel < 0)
        {
          img.at<uchar>(y, x) = 0;
        }
        else
        {
          img.at<uchar>(y, x) = pixel;
        } });
}