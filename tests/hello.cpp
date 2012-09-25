#include "tp_hello.h"
void say(int);

int main(int argc, const char* argv[]) {
    for (int i=0; i<10; ++i)
    {
        tracepoint(qi_probes_tests_hello, counting, i);
        say(i);
    }
}
