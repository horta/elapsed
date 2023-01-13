#ifndef SLEEP_H
#define SLEEP_H

struct timespec;

int __elapsed_sleep(const struct timespec *duration,
                    struct timespec *remaining);

#endif
