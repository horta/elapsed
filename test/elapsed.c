#include "elapsed/elapsed.h"
#include "hope/hope.h"

static void test_elapsed(void)
{
    struct elapsed elapsed = ELAPSED_INIT;
    EQ(elapsed_start(&elapsed), 0);
    EQ(elapsed_sleep(300), 0);
    EQ(elapsed_stop(&elapsed), 0);
    COND(elapsed_milliseconds(&elapsed) >= 250);

    EQ(elapsed_start(&elapsed), 0);
    EQ(elapsed_sleep(800), 0);
    EQ(elapsed_stop(&elapsed), 0);
    COND(elapsed_milliseconds(&elapsed) >= 750);

    EQ(elapsed_start(&elapsed), 0);
    EQ(elapsed_stop(&elapsed), 0);
    COND(elapsed_milliseconds(&elapsed) <= 500);
}

int main(void)
{
    test_elapsed();
    return hope_status();
}
