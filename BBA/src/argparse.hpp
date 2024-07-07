/**
 *  @file
 *  Kommandozeilenparamter-Parser-Schnittstellen-Modul.
 *  Empfängt die Kommandozeilenparameter aus dem Main-Modul und fügt diese
 *  in einen Vektor ein. Mit diesem Vektor kann dann einfacher auf den Parametern
 *  gearbeitet werden.
 *
 *  @author Ilana Schmara, Joshua-Scott Schöttke, Gruppe 14
 */

#pragma once

#include <vector>
#include <string>

namespace bba
{

  enum Action
  {
    ACTION_NONE,
    ACTION_DROP,
    ACTION_INPUT,
    ACTION_PRINT_PNM,
    ACTION_PRINT_RAW,
    ACTION_PRINT_INFO,
    ACTION_OUTPUT,
    ACTION_SHOW,
    ACTION_ZERO,
    // Aufgabe 1
    ACTION_FILTER_IDENTITY,
    // Aufgabe 2
    ACTION_KERNEL_IDENTITY,
    ACTION_EDGE,
    // Aufgabe 3
    ACTION_INTERPOLATION
  };

  enum Filter
  {
    // Filter Aufgabe 1
    FILTER_GRADIENT,
    FILTER_BRIGHTNESS,
    FILTER_FACTOR,
    FILTER_LINEAR,
    FILTER_CONTRAST,
    FILTER_TO_FLOAT,
    FILTER_TO_UCHAR,
    FILTER_A_DIFF,
    FILTER_OVERLAY,
    FILTER_HISTOGRAM,
    FILTER_HISTOGRAM_DRAW,
    FILTER_EQUALIZE,
    FILTER_SPLIT,
    FILTER_BGR_2_HSV,
    FILTER_LEAVES_DETECT,
    // Filter Aufgabe 2
    FILTER_FOLD,
    FILTER_CORRELATE,
    FILTER_MEDIAN,
    FILTER_ERODE,
    FILTER_DILATE,
    FILTER_STAIRTREEDECIDER,
    FILTER_ICONFINDER,
    // Filter Aufgabe 3
    FILTER_TRANSFORM,
    FILTER_TINYPLANET
  };

  enum Kernel
  {
    KERNEL_BOX,
    KERNEL_LAPLACE,
    KERNEL_GAUSSIAN,
    KERNEL_ROTATION,
    KERNEL_SCALING,
    KERNEL_AFFINEFIT
  };

  enum Edge
  {
    EDGE_NULL,
    EDGE_REFLECT,
    EDGE_REPLICATE
  };

  enum Interpolation
  {
    INTERPOLATION_NEAREST,
    INTERPOLATION_BILINEAR
  };

  struct Operation
  {
    Action action;
    Filter filter;
    Edge edge;
    Kernel kernel;
    Interpolation interpolation;
    std::string filename;
    std::vector<int> zero;
    std::vector<int> size;
    std::vector<float> gradient;
    int axis;
    int sections;
    float operand;
    int length;
    float factor;
    std::string edgeParam;
    int medianWidth;
    int min;
    int max;
    double degree;
    std::vector<float> scaling;
    std::vector<float> affinefit;
    std::string interpolationParam;
  };

  /**
   *  Vektor aus Operationen
   */
  typedef std::vector<Operation> Operations;

  /**
   *  Parameter Parser für Kommandozeilen-Argumente
   *  @param argc Argument Count
   *  @param argv Argument Vektor
   *  @return einen Vektor mit den eingegebenen Kommandozeilenparametern
   */
  Operations parse_arguments(int argc, char *argv[]);

}
