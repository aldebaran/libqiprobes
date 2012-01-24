#include "tp_hello.h"

int main(int argc, const char* argv[]) {
    for (int i; i<20; ++i)
        tracepoint(qi_probes_test_hello, counting, i);
}
