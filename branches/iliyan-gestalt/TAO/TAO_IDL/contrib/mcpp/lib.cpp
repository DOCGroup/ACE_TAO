/* $Id$
 * Redistribution and use of this code in source and binary forms, with or
 * without modification, are freely permitted.              [Kiyoshi Matsui]
 */

/*
 *                              L I B . C
 *                  L i b r a r y   R o u t i n e s
 *
 * Some variation of standard library functions.
 * Some standard functions for the library which has not those or the library
 *      which has only non-conforming ones.
 */

#if     HAVE_CONFIG_H
#include    "configed.H"
#else
#include    "noconfig.H"
#endif

#if NEED_GETOPT

#include "system.H"
#include "internal.H"

/*
 * Note: The getopt() of glibc should not be used since the specification
 *  differs from the standard one.
 *  Use this getopt() for this cpp.
 */

/* Based on the public-domain-software released by AT&T.    */

#define OPTERR( s, c)   if (opterr) {   \
    mcpp_fputs( argv[0], ERR);  \
    mcpp_fputs( s, ERR);        \
    mcpp_fputc( c, ERR);        \
    mcpp_fputc( '\n', ERR);     \
    }

int     optind = 1;
int     opterr = 1;
int     optopt;
char *  optarg;

#if MCPP_LIB
void    init_lib( void)
{
    optind = 1;
    opterr = 1;
}
#endif
//FUZZ: disable check_for_lack_ACE_OS
int     getopt(
    int         argc,
    char * const *  argv,
    const char *    opts
)
//FUZZ: enable check_for_lack_ACE_OS
/*
 * Get the next option (and it's argument) from the command line.
 */
{
    const char * const   error1 = ": option requires an argument --";
    const char * const   error2 = ": illegal option --";
    static int      sp = 1;
    int             c;
    const char *    cp;

    if (sp == 1) {
        if (argc <= optind ||
                argv[ optind][ 0] != '-' || argv[ optind][ 1] == '\0') {
            return  EOF;
        } else if (ACE_OS::strcmp( argv[ optind], "--") == 0) {
            optind++;
            return  EOF;
        }
    }
/*  optopt = c = (unsigned char) argv[ optind][ sp];    */
    optopt = c = argv[ optind][ sp] & UCHARMAX;
    if (c == ':' || (cp = ACE_OS::strchr( opts, c)) == 0) {
        OPTERR( error2, c)
        if (argv[ optind][ ++sp] == '\0') {
            optind++;
            sp = 1;
        }
        return  '?';
    }
    if (*++cp == ':') {
        if (argv[ optind][ sp+1] != '\0') {
            optarg = &argv[ optind++][ sp+1];
        } else if (argc <= ++optind) {
            OPTERR( error1, c)
            sp = 1;
            return  '?';
        } else {
            optarg = argv[ optind++];
        }
        sp = 1;
    } else {
        if (argv[ optind][ ++sp] == '\0') {
            sp = 1;
            optind++;
        }
        optarg = 0;
    }
    return  c;
}

#endif

#if ! HOST_HAVE_STPCPY

char *  stpcpy(
    char *          dest,
    const char *    src
)
/*
 * Copy the string and return the advanced pointer.
 */
{
    const char * s;
    char *  d;

    for (s = src, d = dest; (*d++ = *s++) != '\0'; )
        ;
    return  d - 1;
}

#endif
