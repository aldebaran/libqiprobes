#include "tp_subdirhello.h"
#include <iostream>

void say(int i) {
    tracepoint(qi_probes_tests_subdirhello, saying, i);
    std::cout << i << std::endl;
}
