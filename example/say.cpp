#include "tp_example_say.h"
#include <iostream>

void say(int i) {
    tracepoint(qi_probes_example_say, saying, i);
    std::cout << i << std::endl;
    tracepoint(qi_probes_example_say, done_saying);
}
