#include "tp_examplenew_say.h"
#include <iostream>

void say(int i) {
    tracepoint(qi_probes_examplenew_say, saying, i);
    std::cout << i << std::endl;
    tracepoint(qi_probes_examplenew_say, done_saying);
}
