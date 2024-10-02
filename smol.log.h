/* SPDX-License-Identifier: Unlicense */

#ifndef SMOL_LOG_H
#define SMOL_LOG_H

#include <stdarg.h>
#include <time.h> /* IWYU pragma: keep */

enum log_level {
    log_level_info = 0,
    log_level_warn = 1,
    log_level_err  = 2,
    log_level_die  = 3
};

extern char const *log_level_string[4];
extern char const *log_level_color[4];

struct log_event {
    enum log_level lv;
    char const    *fmt;
    va_list        ap;
    struct tm     *tm;
};

struct log_callback {
    enum log_level lv;
    void          *vp;

    void (*fn)(struct log_event *, void *);
};

int log_add_callback(struct log_callback);

void info(char const *, ...);
void warn(char const *, ...);
void err(char const *, ...);
void die(char const *, ...);

#ifdef SMOL_LOG_IMPL

#include <stdlib.h>

#define MAX_CALLBACKS 8

static struct log_callback callbacks[MAX_CALLBACKS] = {0};

char const *log_level_string[4] = { "info", "warning", "error", "fatal" };
char const *log_level_color[4] = { "\033[32m", "\033[33m", "\033[31m", "\033[36m" };

int
log_add_callback(struct log_callback cb)
{
    int i;

    for (i = 0; callbacks[i].fn; ++i) {
        if (i == MAX_CALLBACKS - 1) {
            return 0;
        }
    }
    
    callbacks[i] = cb;
    return i;
}

#define X(level)                                             \
    struct log_event ev;                                     \
    time_t epoch;                                            \
    int i;                                                   \
                                                             \
    time(&epoch);                                            \
                                                             \
    ev.fmt = fmt;                                            \
    ev.lv  = (level);                                        \
    ev.tm  = localtime(&epoch);                              \
                                                             \
    for (i = 0; i < MAX_CALLBACKS && callbacks[i].fn; ++i) { \
        if (ev.lv >= callbacks[i].lv) {                      \
            va_start(ev.ap, fmt);                            \
            callbacks[i].fn(&ev, callbacks[i].ptr);          \
            va_end(ev.ap);                                   \
        }                                                    \
    }

void
info(char const *fmt, ...)
{
    X(log_level_info)
}

void
warn(char const *fmt, ...)
{
    X(log_level_warn)
}

void
err(char const *fmt, ...)
{
    X(log_level_err)
}

void
die(char const *fmt, ...)
{
    X(log_level_die)
    exit(EXIT_FAILURE);
}

#undef X

#endif /* SMOL_LOG_IMPL */

#endif /* SMOL_LOG_H */
