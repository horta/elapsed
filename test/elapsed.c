#include "elapsed/elapsed.h"
#include "cass.h"

int main(void)
{
    struct elapsed* elapsed = elapsed_create();

    elapsed_start(elapsed);
    elapsed_end(elapsed);
    cass_cond(elapsed_seconds(elapsed) < 1.0);

    elapsed_destroy(elapsed);

    return cass_status();
}
