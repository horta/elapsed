#include "cass.h"
#include "elapsed/elapsed.h"

int main(void)
{
    struct elapsed_perf* perf = elapsed_perf_create(4);

    elapsed_perf_start(perf);
    elapsed_perf_end(perf);

    elapsed_perf_start(perf);
    elapsed_perf_end(perf);

    elapsed_perf_start(perf);
    elapsed_perf_end(perf);

    elapsed_perf_start(perf);
    elapsed_perf_end(perf);

    struct elapsed_stats stats = elapsed_stats(perf);
    cass_cond(stats.mean < 1.0);
    cass_cond(stats.sem < 1.0 || isnan(stats.sem));

    elapsed_perf_destroy(perf);

    return cass_status();
}
