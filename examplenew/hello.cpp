#include "say.h"
#include "tp_examplenew_hello.h"

int main(int argc, const char* argv[]) {
    tracepoint(qi_probes_examplenew_hello, start);
    for (int i; i<10; ++i)
    {
        say(i);
    }
    tracepoint(qi_probes_examplenew_hello, stop);
}
