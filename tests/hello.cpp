#include "tp_hello.h"
void say(int);

int main(int argc, const char* argv[]) {
    for (int i; i<20; ++i)
    {
        tracepoint(qi_probes_test_hello, counting, i);
        say(i);
    }
}
