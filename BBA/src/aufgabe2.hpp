/**
 *  @file
 *  Schnittstellen-Modul, welches einen Teil der geforderten Funktionalitäten
 *  aus Aufgabe 2 kapselt.
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
 *  Faltet das Bild anhand eines uebergebenen Filter-Kernels mittels Convolution.
 *  @param kernel der anzuwendende Filter-Kernel
 *  @param img das Bild, welches gefaltet wird
 */
cv::Mat fold(cv::Mat kernel, cv::Mat img);

/**
 *  Faltet das Bild anhand eines uebergebenen Filter-Kernels mittels Correlation.
 *  @param kernel der anzuwendende Filter-Kernel
 *  @param img das Bild, welches gefaltet wird
 */
cv::Mat correlate(cv::Mat kernel, cv::Mat img);

/**
 *  Erstellt den Filter-Kernel eines Box-Filters beliebiger Groesze und gibt diesen zurueck.
 *  @param length die Groesze des Filter-Kernels (Quadratseite)
 *  @return den Filter-Kernel des Box-Filters als Bild
 */
cv::Mat box(int length);

/**
 *  Erstellt den Filter-Kernel eines Laplace-Filters (Groesze 3x3) und gibt diesen zurueck.
 *  @return den Filter-Kernel des Laplace-Filters als Bild
 */
cv::Mat laplace(void);

/**
 *  Erstellt den Filter-Kernel eines Gauss-Filters - mit beliebiger Groesze und Standardabweichung
 *  für den Radius der Funktion des Filters - und gibt diesen zurueck.
 *  @param length die Groesze des Filter-Kernels (Quadratseite)
 *  @param factor die Standardabweichung fuer die Funktion des Filters
 *  @return den Filter-Kernel des Gauss-Filters als Bild
 */
cv::Mat gaussian(int length, float factor);

/**
 *  Wechselt die auszufuehrende Randbehandlung der Filter fuer die Bilder.
 *  @param choice Wahl der zu setzenden Randbehandlung: 0 -> null, 1 -> reflect, 2 -> replicate
 */
void edge(int choice);

/**
 * Funktion, die den Median-Filter mit beliebiger Groesze des Filter-Kernels auf ein uebergebenes Bild anwendet.
 * @param img das Bild, auf welches der Median-Filter angewendet wird
 * @param length die Groesze des Median-Filter-Kernels
 * @return das mit dem Median-Filter gefaltete Bild
 */
cv::Mat median(cv::Mat img, int length);

/**
 * Funktion, die den Erosions-Filter mit einem zu waehlendem Minimum auf ein uebergebenes Bild anwendet.
 * @param img das Bild, auf welches der Erosions-Filter angewendet wird
 * @param min das zu waehlende Minimum mit dem der Erosions-Filter arbeiten soll
 * @return das mit dem Erosions-Filter gefaltete Bild
 */
cv::Mat erode(cv::Mat img, int min);

/**
 * Funktion, die den Dilatations-Filter mit einem zu waehlendem Maximum auf ein uebergebenes Bild anwendet.
 * @param img das Bild, auf welches der Dilatations-Filter angewendet wird
 * @param min das zu waehlende Maximum mit dem der Dilatations-Filter arbeiten soll
 * @return das mit dem Dilatations-Filter gefaltete Bild
 */
cv::Mat dilate(cv::Mat img, int max);

/**
 * Entscheidet, ob es sich im uebergebenen Bild um Baeume oder Treppen handelt und gibt diese Information
 * auf der Standardausgabe aus.
 * @param img das Bild, fuer welches die Analyse durchgefuehrt wird
 * @param buffer ein Buffer fuer die Rekursive Erhoehung bei Farbabweichungen, falls kein eindeutiges Ergebnis erzielt werden konnte
 */
void stairTreeDecider(cv::Mat img, int buffer);

/**
 * Sucht im uebergebenem Bild nach allen Vorkommen des Icons, das im zweiten Bild uebergeben wird und schreibt
 * die Koordinaten aller Vorkommen auf die Standardausgabe.
 * @param img das Bild, in welchem nach dem Icon gesucht wird
 * @param icon das zu suchende Icon
*/
void iconFinder(cv::Mat img, cv::Mat icon);