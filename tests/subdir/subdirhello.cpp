#include "tp_subdirhello.h"
void say(int);

int main(int argc, const char* argv[]) {
    for (int i=0; i<20; ++i)
    {
        tracepoint(qi_probes_tests_subdirhello, counting, i);
        say(i);
    }
}
