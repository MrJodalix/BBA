/**
 *  @file
 *  Modul, welches einen Teil der geforderten Funktionalitäten
 *  aus Aufgabe 1 kapselt.
 *
 *  @author Ilana Schmara, Joshua-Scott Schöttke, Gruppe 14
 */

#include "aufgabe1.hpp"
#include "argparse.hpp"
#include "util.hpp"

/**
 * Hilfsfunktion
 * Grenzueberpruefung fuer den jeweiligen Pixel nach Aufruf des Funktors.
 * @param img das Bild
 * @param pixel der Pixelwert
 * @param y y-Achsenabschnitt
 * @param x x-Achsenabschnitt
 * @param func der Funktor
 */
void pixelRange(cv::Mat &img, float &pixel, int y, int x, std::function<void(void)> func)
{
    func();

    if (pixel >= 0)
    {
        if (pixel < 256)
        {
            img.at<uchar>(y, x) = pixel;
        }
        else
        {
            img.at<uchar>(y, x) = 255;
        }
    }
    else
    {
        img.at<uchar>(y, x) = 0;
    }
}

/**
 *  Manipuliert die Helligkeit aller Pixel eines Bildes um
 *  einen Wert, der pro Pixel addiert wird.
 *  @param img das Bild, welches verändert wird
 *  @param add der pro Pixel addierte Wert
 */
void brightness(cv::Mat img, float add)
{
    img = img + cv::Scalar(add);
}

/**
 *  Manipuliert die Helligkeit aller Pixel eines Bildes um
 *  einen Wert, der pro Pixel multipliziert wird.
 *  @param img das Bild, welches verändert wird
 *  @param fact der pro Pixel multiplizierte Wert
 */
void factor(cv::Mat img, float fact)
{
    img = img * (double)fact;
}

/**
 *  Manipuliert die Helligkeit aller Pixel eines Bildes mit
 *  Hilfe einer linearen Funktion, die für jeden Pixel
 *  angewendet wird.
 *  @param img das Bild, welches verändert wird
 *  @param gradient die Steigung der Funktion
 *  @param axis der y-Achsenabschnitt
 */
void linear(cv::Mat img, float gradient, int axis)
{
    img = img * (double)gradient + cv::Scalar(axis);
}

/**
 *  Manipuliert die Helligkeit aller Pixel eines Bildes mit
 *  Hilfe einer linearen Funktion, die den Kontrast des Bildes aendert.
 *  @param img das Bild, welches verändert wird
 *  @param intensity Wert, um welche die Intensitaet verändert wird
 */
void contrast(cv::Mat img, float intensity)
{
    float pixel = 0;
    for_each_pixel(img, [intensity, &pixel](cv::Mat &img, int y, int x)
                   {
            pixel = img.at<uchar>(y, x);
            pixelRange(img, pixel, y, x, [&pixel, intensity]()
                       { pixel = (pixel - (255/2)) * intensity + 255/2; }); });
}

/**
 * Konvertiert das uebergebene Bild in ein Float-Wert Bild.
 * @param img Das Bild das ubergeben wird
 * @return das konvertierte Bild
 */
cv::Mat toFloat(cv::Mat img)
{
    cv::Mat floatImg;

    img.convertTo(floatImg, CV_32F);

    return floatImg;
}

/**
 * Konvertiert das uebergebene Bild in ein unsigned-Char-Bild.
 * @param img Das Bild das ubergeben wird
 * @return das konvertierte Bild
 */
cv::Mat toUChar(cv::Mat img)
{
    cv::Mat ucharImg;

    img.convertTo(ucharImg, CV_8U);

    return ucharImg;
}

/**
 * Bildet die absolute Differenz von zwei Bildern und liefert das Ergebnis als Bild zurueck.
 * @param img das erste Bild
 * @param secImg das zweite Bild
 * @return das Ergebnisbild
 */
cv::Mat
aDiff(cv::Mat img, cv::Mat secImg)
{
    cv::Mat resImg = cv::Mat(img.rows, img.cols, CV_8UC1);
    resImg = abs(img - secImg);
    return resImg;
}

/**
 * Legt ein Bild mit Alphakanal auf ein RGB-Bild ohne Alphakanal.
 * Die Bilder muessen gleich groß sein.
 * @param topImg das Bild mit Alphakanal
 * @param bottomImg das RGB Bild
 * @return das Ergebnisbild
 */
cv::Mat
overlay(cv::Mat topImg, cv::Mat bottomImg)
{
    cv::Mat resImg = cv::Mat(topImg.rows, topImg.cols, CV_8UC4);

    bottomImg.copyTo(resImg);
    double opacity = 0;
    unsigned char topImgPx = '\0';
    unsigned char bottomImgPx = '\0';
    for_each_pixel(bottomImg, [&resImg, topImg, &opacity, &topImgPx, &bottomImgPx](cv::Mat &bottomImg, int y, int x)
                   {
            // Durchsichtigkeit des Alphakanals bestimmen
            opacity = ((double)topImg.data[y * topImg.step + x * topImg.channels() + 3]) / 255.0;

            // Wenn es Durchsichtigkeit gibt, wird das obere Pixel auf das unter Pixel angepasst.
            for (int c = 0; opacity > 0 && c < resImg.channels(); ++c)
            {
                topImgPx = topImg.data[y * topImg.step + x * topImg.channels() + c];
                bottomImgPx = bottomImg.data[y * bottomImg.step + x * bottomImg.channels() + c];
                resImg.data[y * resImg.step + resImg.channels() * x + c] = bottomImgPx * (1.0 - opacity) + topImgPx * opacity;
            } });

    return resImg;
}

/**
 * Erstellt das Histogramm eines Grauwertbildes mit Gruppierung.
 * @param img das Bild
 * @param sections die Anzahl an Abschnitten, in die das Histogramm aufgeteilt wird
 * @return das Histogramm
 */
cv::Mat
histogram(cv::Mat img, int sections)
{
    float range[] = {0, 256};
    const float *histRange[] = {range};
    bool uniform = true, accumulate = false;
    cv::Mat gray_hist;

    calcHist(&img, 1, 0, cv::Mat(), gray_hist, 1, &sections, histRange, uniform, accumulate);

    for_each_pixel(gray_hist, [img](cv::Mat &gray_hist, int y, int x)
                   { gray_hist.at<float>(y, x) = gray_hist.at<float>(y, x) / (img.cols * img.rows); });

    return gray_hist.t();
}

/**
 * Eine Histogramm-Matrix wird als Bild ausgegeben.
 * @param hist die Histogramm-Matrix
 * @param width die Bildbreite fuer das Histogramm
 * @param heigth die Bildhoehe fuer das Histogramm
 * @return das Histogramm als Bild
 */
cv::Mat
histogramDraw(cv::Mat hist, int width, int height)
{
    int bin_w = cvRound((double)width / hist.cols);

    cv::Mat histImage(height, width, CV_8UC1, cv::Scalar(0));

    for (int i = 1; i <= hist.cols; i++)
    {
        cv::rectangle(histImage, cv::Point(bin_w * (i - 1), height - cvRound(height * hist.at<float>(i - 1))),
                      cv::Point(bin_w * (i), height),
                      255, -1, 4, 0);
    }

    return histImage;
}

/**
 * Aequilibriert das Bild anhand des uebergebenen Histogramms.
 * @param hist das Histogramm
 * @param img das Bild
 * @return das aequilibrierte Bild
 */
cv::Mat
equalize(cv::Mat hist, cv::Mat img)
{
    cv::Mat histCopy = img;
    cv::Mat acc_hist = cv::Mat::zeros(1, 256, CV_32F);

    float sumPix = 0;

    // berechnet das Summenhistogramm
    for_each_pixel(hist, [&sumPix, &acc_hist](cv::Mat &hist, int y, int x)
                   {
        sumPix += (hist.at<float>(y,x) * 255);
            acc_hist.at<float>(y, x) = sumPix; });

    // Mapping
    cv::Mat resImage = cv::Mat::zeros(img.size(), CV_32FC1);
    for_each_pixel(img, [&resImage, &acc_hist](cv::Mat &img, int y, int x)
                   { resImage.at<float>(y, x) = acc_hist.at<float>(img.at<uchar>(y, x)); });

    // Ergebnisbild konvertieren
    resImage.convertTo(resImage, CV_8UC1);

    return resImage;
}

/**
 * Splittet ein Bild in seine drei Farbkanaele.
 * @param img das Bild
 * @return ein Vektor, mit den Bildern der drei Farbkanaele
 */
std::vector<cv::Mat>
split(cv::Mat img)
{
    std::vector<cv::Mat> splitVec;
    cv::split(img, splitVec);
    return splitVec;
}

/**
 * Konvertiert den Farbraum des BGR-Bildes in den HSV-Farbraums.
 * @param img das Bild
 */
void bgr2hsv(cv::Mat img)
{
    cv::cvtColor(img, img, cv::COLOR_BGR2HSV);
}

/**
 * Wertet prozentual die Farbwerte des Bildes aus.
 * Dabei werden Farbwerte die im gruenen Bereich liegen
 * ausgelassen um nur rote und gelbe Werte einzufangen.
 * Wir erwarten dabei nur Bilder die rote/gelbe und gruene Farbwerte aufweisen.
 * @param img das Bild
 * @return den Prozentwert an roten/gelben Pixeln
 */
int leavesDetect(cv::Mat img)
{
    cv::Mat copyImg;
    img.copyTo(copyImg);
    // Bild in den HSV-Raum konvertieren, um den Farbton zu erkennen
    bgr2hsv(copyImg);
    std::vector<cv::Mat> splitVec = split(copyImg);

    cv::Mat hueImg = splitVec[0];

    float res = 0;

    for_each_pixel(hueImg, [&res](cv::Mat hueImg, int y, int x)
                   {
        if (!(hueImg.at<uchar>(y,x) >= 30 && hueImg.at<uchar>(y,x) <= 95)) {
            res++;
        } });
    res /= (img.cols * img.rows);
    res *= 100;
    return res;
}
