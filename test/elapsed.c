#include "elapsed/elapsed.h"
#include "hope/hope.h"

static void test_elapsed(void)
{
    struct elapsed elapsed = ELAPSED_INIT;
    elapsed_start(&elapsed);
    elapsed_sleep(0.3);
    elapsed_stop(&elapsed);
    COND(elapsed_seconds(&elapsed) >= 0.3);
}

static void test_elapsed_clock(void)
{
    struct elapsed_clock elapsed = elapsed_clock_init();
    elapsed_clock_start(&elapsed);
    elapsed_sleep(0.3);
    elapsed_clock_stop(&elapsed);
    COND(elapsed_clock_seconds(&elapsed) < 1.0);
}

int main(void)
{
    test_elapsed();
    test_elapsed_clock();
    return hope_status();
}
