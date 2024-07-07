/**
 *  @file
 *  Kommandozeilenparamter-Parser-Modul.
 *  Empfängt die Kommandozeilenparameter aus dem Main-Modul und fügt diese
 *  in einen Vektor ein. Mit diesem Vektor kann dann einfacher auf den Parametern
 *  gearbeitet werden.
 *
 *  @author Ilana Schmara, Joshua-Scott Schöttke, Gruppe 14
 */

#include "argparse.hpp"
#include <iostream>
#include <stdexcept>

/**
 *  Parameter Parser für Kommandozeilen-Argumente
 *  @param argc Argument Count
 *  @param argv Argument Vektor
 *  @return einen Vektor mit den eingegebenen Kommandozeilenparametern
 */
bba::Operations bba::parse_arguments(int argc, char *argv[])
{
  Operations ops;

  const std::vector<std::string> arguments(argv + 1, argv + argc);

  auto consume_argument = [&arguments](size_t &i)
  {
    if (arguments.size() > i + 1)
    {
      return arguments[++i];
    }
    else
    {
      throw std::runtime_error("Nicht genügend Argumente.");
    }
  };

  for (size_t i = 0; i < arguments.size(); i++)
  {
    const std::string &action = arguments[i];
    // std::cerr << action << std::endl;
    if (action == "--drop")
    {
      Operation op;
      op.action = bba::ACTION_DROP;
      ops.push_back(op);
    }
    else if (action == "--input")
    {
      Operation op;
      op.action = bba::ACTION_INPUT;
      op.filename = consume_argument(i);
      ops.push_back(op);
    }
    else if (action == "--printpnm")
    {
      Operation op;
      op.action = bba::ACTION_PRINT_PNM;
      ops.push_back(op);
    }
    else if (action == "--printraw")
    {
      Operation op;
      op.action = bba::ACTION_PRINT_RAW;
      ops.push_back(op);
    }
    else if (action == "--printinfo")
    {
      Operation op;
      op.action = bba::ACTION_PRINT_INFO;
      ops.push_back(op);
    }
    else if (action == "--output")
    {
      Operation op;
      op.action = bba::ACTION_OUTPUT;
      op.filename = consume_argument(i);
      ops.push_back(op);
    }
    else if (action == "--show")
    {
      Operation op;
      op.action = bba::ACTION_SHOW;
      ops.push_back(op);
    }
    else if (action == "--zero")
    {
      Operation op;
      op.action = bba::ACTION_ZERO;
      op.zero.push_back(std::stoi(consume_argument(i)));
      op.zero.push_back(std::stoi(consume_argument(i)));
      op.zero.push_back(std::stoi(consume_argument(i)));
      ops.push_back(op);
    }
    else if (action == "--gradient")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = FILTER_GRADIENT;
      op.gradient.push_back(std::stof(consume_argument(i)));
      op.gradient.push_back(std::stof(consume_argument(i)));
      op.gradient.push_back(std::stof(consume_argument(i)));
      op.gradient.push_back(std::stof(consume_argument(i)));
      ops.push_back(op);
    }
    else if (action == "--brightness")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = bba::FILTER_BRIGHTNESS;
      op.operand = std::stof(consume_argument(i));
      ops.push_back(op);
    }
    else if (action == "--factor")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = bba::FILTER_FACTOR;
      op.operand = std::stof(consume_argument(i));
      ops.push_back(op);
    }
    else if (action == "--linear")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = bba::FILTER_LINEAR;
      op.operand = std::stof(consume_argument(i));
      op.axis = std::stoi(consume_argument(i));
      ops.push_back(op);
    }
    else if (action == "--contrast")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = bba::FILTER_CONTRAST;
      op.operand = std::stof(consume_argument(i));
      ops.push_back(op);
    }
    else if (action == "--tofloat")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = bba::FILTER_TO_FLOAT;
      ops.push_back(op);
    }
    else if (action == "--touchar")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = bba::FILTER_TO_UCHAR;
      ops.push_back(op);
    }
    else if (action == "--adiff")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = bba::FILTER_A_DIFF;
      ops.push_back(op);
    }
    else if (action == "--overlay")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = bba::FILTER_OVERLAY;
      ops.push_back(op);
    }
    else if (action == "--histogram")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = bba::FILTER_HISTOGRAM;
      op.sections = std::stoi(consume_argument(i));
      ops.push_back(op);
    }
    else if (action == "--histogram-draw")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = bba::FILTER_HISTOGRAM_DRAW;
      op.size.push_back(std::stoi(consume_argument(i)));
      op.size.push_back(std::stoi(consume_argument(i)));
      ops.push_back(op);
    }
    else if (action == "--equalize")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = bba::FILTER_EQUALIZE;
      ops.push_back(op);
    }
    else if (action == "--split")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = bba::FILTER_SPLIT;
      ops.push_back(op);
    }
    else if (action == "--bgr2hsv")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = bba::FILTER_BGR_2_HSV;
      ops.push_back(op);
    }
    else if (action == "--leavesdetect")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = bba::FILTER_LEAVES_DETECT;
      ops.push_back(op);
    }
    else if (action == "--fold")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = bba::FILTER_FOLD;
      ops.push_back(op);
    }
    else if (action == "--correlate")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = bba::FILTER_CORRELATE;
      ops.push_back(op);
    }
    else if (action == "--box")
    {
      Operation op;
      op.action = bba::ACTION_KERNEL_IDENTITY;
      op.kernel = bba::KERNEL_BOX;
      op.length = std::stoi(consume_argument(i));
      ops.push_back(op);
    }
    else if (action == "--laplace")
    {
      Operation op;
      op.action = bba::ACTION_KERNEL_IDENTITY;
      op.kernel = bba::KERNEL_LAPLACE;
      ops.push_back(op);
    }
    else if (action == "--gaussian")
    {
      Operation op;
      op.action = bba::ACTION_KERNEL_IDENTITY;
      op.kernel = bba::KERNEL_GAUSSIAN;
      op.length = std::stoi(consume_argument(i));
      op.factor = std::stof(consume_argument(i));
      ops.push_back(op);
    }
    else if (action == "--edge")
    {
      Operation op;
      op.action = bba::ACTION_EDGE;
      op.edgeParam = consume_argument(i);
      ops.push_back(op);
    }
    else if (action == "--median")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = bba::FILTER_MEDIAN;
      op.medianWidth = std::stoi(consume_argument(i));
      ops.push_back(op);
    }
    else if (action == "--erode")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = bba::FILTER_ERODE;
      op.min = std::stoi(consume_argument(i));
      ops.push_back(op);
    }
    else if (action == "--dilate")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = bba::FILTER_DILATE;
      op.max = std::stoi(consume_argument(i));
      ops.push_back(op);
    }
    else if (action == "--stairtreedecider")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = bba::FILTER_STAIRTREEDECIDER;
      ops.push_back(op);
    }
    else if (action == "--iconfinder")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = bba::FILTER_ICONFINDER;
      ops.push_back(op);
    }
    else if (action == "--rotation")
    {
      Operation op;
      op.action = bba::ACTION_KERNEL_IDENTITY;
      op.kernel = bba::KERNEL_ROTATION;
      op.degree = std::stod(consume_argument(i));
      ops.push_back(op);
    }
    else if (action == "--scaling")
    {
      Operation op;
      op.action = bba::ACTION_KERNEL_IDENTITY;
      op.kernel = bba::KERNEL_SCALING;
      op.scaling.push_back(std::stof(consume_argument(i)));
      op.scaling.push_back(std::stof(consume_argument(i)));
      ops.push_back(op);
    }
    else if (action == "--affinefit")
    {
      Operation op;
      op.action = bba::ACTION_KERNEL_IDENTITY;
      op.kernel = bba::KERNEL_AFFINEFIT;
      op.affinefit.push_back(std::stof(consume_argument(i)));
      op.affinefit.push_back(std::stof(consume_argument(i)));
      op.affinefit.push_back(std::stof(consume_argument(i)));
      op.affinefit.push_back(std::stof(consume_argument(i)));
      op.affinefit.push_back(std::stof(consume_argument(i)));
      op.affinefit.push_back(std::stof(consume_argument(i)));
      op.affinefit.push_back(std::stof(consume_argument(i)));
      op.affinefit.push_back(std::stof(consume_argument(i)));
      ops.push_back(op);
    }
    else if (action == "--interpolation")
    {
      Operation op;
      op.action = bba::ACTION_INTERPOLATION;
      op.interpolationParam = consume_argument(i);
      ops.push_back(op);
    }
    else if (action == "--transform")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = bba::FILTER_TRANSFORM;
      ops.push_back(op);
    }
    else if (action == "--tinyplanet")
    {
      Operation op;
      op.action = bba::ACTION_FILTER_IDENTITY;
      op.filter = bba::FILTER_TINYPLANET;
      ops.push_back(op);
    }
  }
  return ops;
}