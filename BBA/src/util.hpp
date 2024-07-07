/**
 *  @file
 *  Schnittstellen-Modul für ungebundene Hilfsfunktionen.
 * 
 *  @author Ilana Schmara, Joshua-Scott Schöttke, Gruppe 14
 */

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <functional>

/**
 *  Führt den uebergebenen Funktor auf allen Pixel eines Bildes aus.
 *  @param img das Bild, auf dessen Pixeln gearbeitet wird
 *  @param filter der Funktor, welcher auf allen Pixeln angewendet wird
 */
void for_each_pixel(cv::Mat &img, std::function<void(cv::Mat &, int, int)> filter);

/**
 * Erstellt einen quadratischen Kernel der ueber ein gegebenes Bild erstellt wird.
 * @param kernelImg der Kernel der bearbeitet und verändert wird
 * @param y die y-Achse des original Bildes
 * @param x die x-Achse des original Bildes
 * @param length die Laenge und Breite des Kernel. Soll nur ungerade sein.
 * @param floatImg das original Bild als Float Bild
 * @return die positiven Werte des Kernel in einem Vector
*/
std::vector<int> createKernelVector(cv::Mat kernelImg, int y, int x, int length, cv::Mat floatImg);

/**
 *
 */
cv::Mat newFinalImage(cv::Mat img, cv::Mat matrix);