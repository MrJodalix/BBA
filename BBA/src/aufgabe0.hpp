/**
 *  @file
 *  Schnittstellen-Modul, welches einen Teil der geforderten Funktionalitäten
 *  aus Aufgabe 0 kapselt.
 * 
 *  Fasst die PNM Ausgabe und den Filter Gradient in Funktionen zusammen.
 * 
 *  @author Ilana Schmara, Joshua-Scott Schöttke, Gruppe 14
 */

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

/**
 *  Gibt ein Bild im Portable-Anymap-Format auf der Konsole aus.
 *  @param img das Bild, welchen als PNM dargestellt wird
 */
void printpnm(cv::Mat img);

/**
 *  Filter, der aus einem Leerbild ein Bild mit einem weichen Farb-Verlauf
 *  von schwarz nach weiss erzeugt.
 *  @param img das Leerbild
 *  @param s Summand s für die Berechnung
 *  @param t Summand t für die Berechnung
 *  @param u Faktor u für die Berechnung
 *  @param v Faktor v für die Berechnung
 */
void gradient(cv::Mat img, float s, float t, float u, float v);