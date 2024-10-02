/* SPDX-License-Identifier: Unlicense */

#ifndef SMOL_ARG_H
#define SMOL_ARG_H

#define ARGBEGIN for ((void)(*argv && (--argc, ++argv));              \
                    *argv && ((*argv)[0] == '-' && (*argv)[1]);) do { \
                        int _i, _j;                                   \
                        if ((*argv)[1] == '-' && *argv[2] == '\0') {  \
                            --argc, ++argv;                           \
                            break;                                    \
                        }                                             \
                        for (_i = 1, _j = 1; (*argv)[_i]; ++_i) {     \
                            switch ((*argv)[_i])
#define ARGEND          }                                             \
                        argc -= _j, argv += _j;                       \
                    } while (0)

#define ARGF(x) (argv[_j] ? argv[_j++] : (x))
#define EARGF(e) ARGF(((e), (void *)0))

#endif /* SMOL_ARG_H */
