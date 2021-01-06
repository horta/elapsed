#ifndef ELAPSED_ELAPSED_H
#define ELAPSED_ELAPSED_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ELAPSED_VERSION "1.0.0"

#define INPLACE static inline

struct elapsed;

INPLACE struct elapsed* elapsed_create(void);
INPLACE void            elapsed_start(struct elapsed* elapsed);
INPLACE double          elapsed_end(struct elapsed* elapsed);
INPLACE double          elapsed_seconds(struct elapsed const* elapsed);
INPLACE void            elapsed_destroy(struct elapsed const* elapsed);

struct elapsed_perf;

INPLACE struct elapsed_perf* elapsed_perf_create(unsigned ntrials);
INPLACE void                 elapsed_perf_start(struct elapsed_perf* perf);
INPLACE void                 elapsed_perf_end(struct elapsed_perf* perf);
INPLACE void                 elapsed_perf_destroy(struct elapsed_perf const* perf);

struct elapsed_stats
{
    double mean; /**< Arithmetic mean. */
    double sem;  /**< Standard error of the mean. */
};

INPLACE struct elapsed_stats elapsed_stats(struct elapsed_perf const* perf);

/* -------------------- Elapsed implementation -------------------- */

struct elapsed
{
    clock_t  start;
    clock_t  end;
    double*  seconds;
    unsigned ntrials;
};

INPLACE struct elapsed* elapsed_create(void)
{
    return (struct elapsed*)malloc(sizeof(struct elapsed));
}

INPLACE void elapsed_start(struct elapsed* elapsed) { elapsed->start = clock(); }

INPLACE double elapsed_end(struct elapsed* elapsed)
{
    elapsed->end = clock();
    return elapsed_seconds(elapsed);
}

INPLACE double elapsed_seconds(struct elapsed const* elapsed)
{
    return ((double)(elapsed->end - elapsed->start)) / CLOCKS_PER_SEC;
}

INPLACE void elapsed_destroy(struct elapsed const* elapsed) { free((void*)elapsed); }

/* -------------------- Elapsed perf implementation -------------------- */

struct elapsed_perf
{
    struct elapsed clock;
    double*        elapsed_time;
    unsigned       trial;
    unsigned       ntrials;
};

INPLACE struct elapsed_perf* elapsed_perf_create(unsigned ntrials)
{
    struct elapsed_perf* perf = (struct elapsed_perf*)malloc(sizeof(*perf));
    perf->elapsed_time = (double*)malloc(sizeof(*perf->elapsed_time) * ntrials);
    perf->trial = 0;
    perf->ntrials = ntrials;
    return perf;
}

INPLACE void elapsed_perf_start(struct elapsed_perf* perf) { elapsed_start(&perf->clock); }

INPLACE void elapsed_perf_end(struct elapsed_perf* perf)
{
    perf->elapsed_time[perf->trial] = elapsed_end(&perf->clock);
    perf->trial += 1;
}

INPLACE void elapsed_perf_destroy(struct elapsed_perf const* perf)
{
    free((void*)perf->elapsed_time);
    free((void*)perf);
}

/* -------------------- Elapsed stats implementation -------------------- */

INPLACE double __elapsed_mean(double const* arr, unsigned n);
INPLACE double __elapsed_sample_std(double const* arr, unsigned n, double mean);

INPLACE struct elapsed_stats elapsed_stats(struct elapsed_perf const* perf)
{
    double mean = __elapsed_mean(perf->elapsed_time, perf->ntrials);
    double std = __elapsed_sample_std(perf->elapsed_time, perf->ntrials, mean);
    double sem = std / sqrt(perf->ntrials);
    return (struct elapsed_stats){mean, sem};
}

INPLACE double __elapsed_mean(double const* arr, unsigned n)
{
    if (n == 0)
        return NAN;

    double total = arr[0];

    for (unsigned i = 1; i < n; ++i)
        total += arr[i];

    return total / n;
}

INPLACE double __elapsed_sample_std(double const* arr, unsigned n, double mean)
{
    if (n <= 1)
        return NAN;

    double var = 0.0;

    for (unsigned i = 0; i < n; ++i)
        var += (arr[i] - mean) * (arr[i] - mean);

    return sqrt(var / (n - 1));
}

#undef INPLACE

#endif
