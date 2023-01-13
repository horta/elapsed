#include "elapsed/elapsed.h"

int main(void)
{
    struct elapsed elapsed = ELAPSED_INIT;
    if (elapsed_start(&elapsed) != 0) return EXIT_FAILURE;
    if (elapsed_sleep(300) != 0) return EXIT_FAILURE;
    if (elapsed_stop(&elapsed) != 0) return EXIT_FAILURE;
    if (elapsed_milliseconds(&elapsed) < 250) return EXIT_FAILURE;

    if (elapsed_start(&elapsed) != 0) return EXIT_FAILURE;
    if (elapsed_sleep(800) != 0) return EXIT_FAILURE;
    if (elapsed_stop(&elapsed), 0 != 0) return EXIT_FAILURE;
    if (elapsed_milliseconds(&elapsed) < 750) return EXIT_FAILURE;

    if (elapsed_start(&elapsed) != 0) return EXIT_FAILURE;
    if (elapsed_stop(&elapsed) != 0) return EXIT_FAILURE;
    if (elapsed_milliseconds(&elapsed) > 500) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
