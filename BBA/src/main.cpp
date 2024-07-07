#include "argparse.hpp" // soll vom Teilnehmer erstellt werden
#include <opencv2/opencv.hpp>
#include <stack>

int main(int argc, char **argv) {
  bba::Operations ops = bba::parse_arguments(argc, argv); // soll vom Teilnehmer erstellt werden
  bba::optimize_operations(ops); // ab Aufgabe 3 (optional)
  
  // Operationen an Kontext binden (optional, in Aufgabe 4 hilfreich)
  int some_setting = 0;
  std::vector<std::function<…>> stack_operations;
  for (const bba::Operation & op : ops) {
    switch (op.action) {
      case bba::WHATEVER: {
        bound_operations.push([&some_setting](std::stack<cv::Mat> &s){…});
      } break;
    }
  }
  
  // Operationen ausführen
  std::stack<cv::Mat> stack;
  bool ok = true;
  for (int i = 0; i < stack_operations.size() && ok ; i++) {
    auto & op = stack_operations[i];
    ok = op(stack);
  }

  return 0;
}
