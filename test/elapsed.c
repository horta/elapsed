#include "elapsed/elapsed.h"
#include "cass.h"

int main(void)
{
    struct elapsed elapsed = elapsed_init();

    elapsed_start(&elapsed);
    elapsed_end(&elapsed);
    cass_cond(elapsed_seconds(&elapsed) < 1.0);

    return cass_status();
}
