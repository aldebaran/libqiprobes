#include "tp_hello.h"
#include <iostream>

void say(int i) {
    tracepoint(qi_probes_test_hello, saying, i);
    std::cout << i << std::endl;
}
