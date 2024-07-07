/**
 *  @file
 *  Schnittstellen-Modul, welches einen Teil der geforderten Funktionalitäten
 *  aus Aufgabe 1 kapselt.
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
 *  Manipuliert die Helligkeit aller Pixel eines Bildes um
 *  einen Wert, der pro Pixel addiert wird.
 *  @param img das Bild, welches verändert wird
 *  @param add der pro Pixel addierte Wert
 */
void brightness(cv::Mat img, float add);

/**
 *  Manipuliert die Helligkeit aller Pixel eines Bildes um
 *  einen Wert, der pro Pixel multipliziert wird.
 *  @param img das Bild, welches verändert wird
 *  @param fact der pro Pixel multiplizierte Wert
 */
void factor(cv::Mat img, float fact);

/**
 *  Manipuliert die Helligkeit aller Pixel eines Bildes mit
 *  Hilfe einer linearen Funktion, die für jeden Pixel
 *  angewendet wird.
 *  @param img das Bild, welches verändert wird
 *  @param gradient die Steigung der Funktion
 *  @param axis der y-Achsenabschnitt
 */
void linear(cv::Mat img, float gradient, int axis);

/**
 *  Manipuliert die Helligkeit aller Pixel eines Bildes mit
 *  Hilfe einer linearen Funktion, die den Kontrast des Bildes aendert.
 *  @param img das Bild, welches verändert wird
 *  @param intensity Wert, um welche die Intensitaet verändert wird
 */
void contrast(cv::Mat img, float intensity);

/**
 * Konvertiert das uebergebene Bild in ein Float-Wert Bild.
 * @param img Das Bild das ubergeben wird
 * @return das konvertierte Bild
 */
cv::Mat
toFloat(cv::Mat img);

/**
 * Konvertiert das uebergebene Bild in ein unsigned-Char-Bild.
 * @param img Das Bild das ubergeben wird
 * @return das konvertierte Bild
 */
cv::Mat
toUChar(cv::Mat img);

/**
 * Bildet die absolute Differenz von zwei Bildern und liefert das Ergebnis als Bild zurueck.
 * @param img das erste Bild
 * @param secImg das zweite Bild
 * @return das Ergebnisbild
 */
cv::Mat
aDiff(cv::Mat img, cv::Mat secImg);

/**
 * Legt ein Bild mit Alphakanal auf ein RGB-Bild ohne Alphakanal.
 * Die Bilder muessen gleich groß sein.
 * @param topImg das Bild mit Alphakanal
 * @param bottomImg das RGB Bild
 * @return das Ergebnisbild
 */
cv::Mat
overlay(cv::Mat bottomImg, cv::Mat topImg);

/**
 * Erstellt das Histogramm eines Grauwertbildes mit Gruppierung.
 * @param img das Bild
 * @param sections die Anzahl an Abschnitten, in die das Histogramm aufgeteilt wird
 * @return das Histogramm
 */
cv::Mat
histogram(cv::Mat img, int sections);

/**
 * Eine Histogramm-Matrix wird als Bild ausgegeben.
 * @param hist die Histogramm-Matrix
 * @param width die Bildbreite fuer das Histogramm
 * @param heigth die Bildhoehe fuer das Histogramm
 * @return das Histogramm als Bild
 */
cv::Mat
histogramDraw(cv::Mat hist, int width, int height);

/**
 * Aequilibriert das Bild anhand des uebergebenen Histogramms.
 * @param hist das Histogramm
 * @param img das Bild
 * @return das aequilibrierte Bild
 */
cv::Mat
equalize(cv::Mat img, cv::Mat hist);

/**
 * Splittet ein Bild in seine drei Farbkanaele.
 * @param img das Bild
 * @return ein Vektor, mit den drei Farbkanaelen
 */
std::vector<cv::Mat>
split(cv::Mat img);

/**
 * Konvertiert den Farbraum des BGR-Bildes in den HSV-Farbraums.
 * @param img das Bild
 */
void bgr2hsv(cv::Mat img);

/**
 * Wertet prozentual die Farbwerte des Bildes aus.
 * Dabei werden Farbwerte die im gruenen Bereich liegen
 * ausgelassen um nur rote und gelbe Werte einzufangen.
 * Wir erwarten dabei nur Bilder die rote/gelbe und gruene Farbwerte aufweisen.
 * @param img das Bild
 * @return den Prozentwert an roten/gelben Pixeln
 */
int leavesDetect(cv::Mat img);