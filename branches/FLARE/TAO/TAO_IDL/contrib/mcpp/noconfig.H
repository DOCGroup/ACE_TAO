/* $Id$
 * noconfig.H
 * Configurations for MCPP not using config.h.
 *
 * WARNING: These are default settings.  To configure for your system, you
 *      must edit this file here and there.
 */

#define TRUE                1
#define FALSE               0

/*
 *  'Target' means the O.S. and the compiler to which cpp is implemented.
 *  'Host' means the O.S. and the compiler with which cpp is compiled.
 */

/* Define target operating-system.  */
#define SYSTEM              SYS_FREEBSD

/* Define target compiler.          */
#ifndef COMPILER
#define COMPILER            INDEPENDENT /* compiler-independent-build   */
#endif

/* Define host operating-system.    */
#define HOST_SYSTEM         SYSTEM

/* Define host compiler.            */
#define HOST_COMPILER       GNUC

/* Version message.                 */
/* "MCPP V.2.* (200y/mm) compiled by " precedes VERSION_MSG */
#define VERSION_MSG         "GCC 3.4"
#if     0
    "LCC-Win32 2006-03"
    "Visual C 2005"
    "Visual C 2003"
    "BCC V.5.5"
#endif

/*
 *      P A R T   1     Configurations for target-operating-system
 *                      and target-compiler.
 */

/*
 * Names of the SYSTEM (i.e. target operating system).  This is needed so that
 * cpp can use appropriate filename conventions.
 */
#define SYS_UNKNOWN         0
#define SYS_UNIX            0x1000
#define SYS_LINUX           0x1800  /* (SYS_LINUX & 0xF000) == SYS_UNIX     */
#define SYS_FREEBSD         0x1A00  /* (SYS_FREEBSD & 0xF000) == SYS_UNIX   */
#define SYS_CYGWIN          0x1C00  /* (SYS_CYGWIN & 0xF000) == SYS_UNIX    */
#define SYS_MAC             0x6000
#define SYS_WIN             0x7000
#define SYS_WIN32           0x7400  /* (SYS_WIN32 & 0xF000) == SYS_WIN      */
#define SYS_MINGW           0x7C00  /* (SYS_MINGW & 0xF000) == SYS_WIN      */

/* COMPILER */
#define COMPILER_UNKNOWN    0
#define MSC                 0x7400  /* Microsoft C, Visual C++      */
#define BORLANDC            0x7440  /* Borland C    */
#define WIN_SYMANTECC       0x7470  /* Symantec for Windows         */
#define LCC                 0x74C0  /* LCC-Win32    */
#define GNUC                0x00E0  /* GNU C (GCC)  */
#define INDEPENDENT         0xFFFF  /* No target, compiler-independent build*/

#define SYS_FAMILY          (SYSTEM & 0xF000)
#define COMPILER_FAMILY     (COMPILER & 0xF0)
#define HOST_SYS_FAMILY     (HOST_SYSTEM & 0xF000)

/* Default MBCHAR (multi-byte character) encoding.  */
#define EUC_JP          0x10    /* Extended UNIX code of JIS X 0208 */
#define GB2312          0x20    /* EUC-like encoding of Chinese GB 2312-80  */
#define KSC5601         0x30    /* EUC-like encoding of Korean KS C 5601    */
#define SJIS            0x80    /* Shift-JIS encoding of JIS X 0208 */
#define BIGFIVE         0x90    /* Encoding of Taiwanese Big Five   */
#define ISO2022_JP      0x100   /* ISO-2022-JP (ISO-2022-JP1) encoding      */
#define UTF8            0x1000  /* UTF-8 encoding           */

/*
 * MBCHAR means multi-byte character encoding.
 * MBCHAR means the default encoding, and you can change the encoding by
 *      #pragma MCPP setlocale, -e <encoding> option or environment variable
 *      LC_ALL, LC_CTYPE, LANG.
 * MBCHAR == 0 means no multi-byte character encoding.
 */

/*
 * In order to predefine target-system-dependent macros,
 * several macros are defined here:
 * *_OLD define the macro beginning with an alphabetic letter,
 * *_STD, *_STD?, *_EXT, *_EXT2 define the macro beginning with an '_'.
 * *_STD1 define the macro beginning with '__' and ending with an alpha-
 *          numeric letter.
 * *_STD2 define the macro beginning with '__' and ending with '__'.
 * These may not be defined, if they are not needed.
 * They should not be #defined to no token or to "".
 *
 * CPU_OLD, CPU_STD1, CPU_STD2, CPU_SP_OLD, CPU_SP_STD
 *      define the target cpu (by name)
 * SYSTEM_OLD, SYSTEM_STD1, SYSTEM_STD2, SYSTEM_EXT, SYSTEM_EXT2
 *      define the target operating system (by name)
 * SYSTEM_SP_OLD, SYSTEM_SP_STD     define the target-OS specific macro name
 * COMPILER_OLD, COMPILER_STD1, COMPILER_STD2, COMPILER_EXT, COMPILER_EXT2
 *          , COMPILER_SP_OLD, COMPILER_SP_STD
 *      define the target compiler (by name)
 * COMPILER_CPLUS        define the target C++ compiler
 * COMPILER_SP1, COMPILER_SP2, COMPILER_SP3, COMPILER_SP4
 *      define the compiler-specific macros
 *
 * <macro>_VAL      specify the value of the <macro>.
 *      If not specified, these values default to "1".
 *      To define the value of no-token, specify as "" rather than no-token.
 * CPU_*, SYSTEM_OLD, SYSTEM_STD?, COMPILER_OLD have the value of "1".
 */

/*
 * target-compiler-dependent definitions:
 *
 * LINE_PREFIX  defines the output line prefix, if not "#line 123".
 *              This should be defined as "# " to represent "# 123" format
 *              ("#line " represents "#line 123" format).
 *
 * C_INCLUDE_DIR1, C_INCLUDE_DIR2   may be defined if you have a compiler-
 *              specific include directory which is to be searched *before*
 *              the operating-system specific directories (e.g. /usr/include).
 * CPLUS_INCLUDE_DIR1, CPLUS_INCLUDE_DIR2, CPLUS_INCLUDE_DIR3
 *      , CPLUS_INCLUDE_DIR4     are for C++ include directory which exist
 *              other than C include directory.
 * ENV_C_INCLUDE_DIR    may be defined to the name of environment-variable for
 *              C include directory.
 * ENV_CPLUS_INCLUDE_DIR    is name of environment-variable for C++ include
 *              directory which exists other than ENV_C_INCLUDE_DIR.
 * ENV_SEP      is the separator (other than space) of include-paths in an
 *              environment-variable.  e.g. the ':' in
 *              "/usr/abc/include:/usr/xyz/include"
 *
 * EMFILE       should be defined to the macro to represent errno of 'too many
 *              open files' if the macro is different from EMFILE.
 *
 * ONE_PASS     should be set TRUE, if COMPILER is "one pass compiler".
 *
 * FNAME_FOLD   means that target-system folds upper and lower cases of
 *              directory and file-name.
 *
 * SEARCH_INIT  specifies the default value of 'search_rule' (in system.c).
 *              'search_rule' holds searching rule of #include "header.h" to
 *              search first before searching user specified or system-
 *              specific include directories.
 *              CURRENT means to search the directory relative to "current
 *              directory" which is current at cpp invocation.
 *              SOURCE means to search the directory relative to that of the
 *              source file (i.e. "includer").
 *              (CURRENT & SOURCE) means to search current directory first
 *              source directory next.
 *              'search_rule' is initialized to SEARCH_INIT.
 */
#define CURRENT             1
#define SOURCE              2

#if     SYS_FAMILY == SYS_UNIX
#define SYSTEM_OLD          "unix"
#define SYSTEM_STD1         "__unix"
#define SYSTEM_STD2         "__unix__"
#endif

#if     SYSTEM == SYS_FREEBSD || SYSTEM == SYS_LINUX
#define CPU_STD2            "__i386__"
#endif

#if     SYSTEM == SYS_FREEBSD
#define SYSTEM_EXT          "__FreeBSD__"
#define SYSTEM_EXT_VAL      "6"     /* V.5.*: 5, V.6.*:6    */
#endif

#if     SYSTEM == SYS_LINUX
#define SYSTEM_EXT          "__linux__"
#endif

#if     SYSTEM == SYS_CYGWIN
#define CPU_STD2            "__i386__"
#define SYSTEM_EXT          "__CYGWIN__"
#define SYSTEM_EXT2         "__CYGWIN32__"
#define MBCHAR              SJIS
#define FNAME_FOLD          TRUE
#define CYGWIN_ROOT_DIRECTORY   "c:/pub/compilers/cygwin"
#endif  /* SYSTEM == SYS_CYGWIN */

#if     SYSTEM == SYS_MINGW
#define SYSTEM_EXT          "__MINGW__"
#define SYSTEM_EXT2         "__MINGW32__"
#define MSYS_ROOT_DIRECTORY "c:/pub/compilers/msys/1.0"
#define MINGW_DIRECTORY     "c:/pub/compilers/mingw"
#if     COMPILER == GNUC
#define SJIS_IS_ESCAPE_FREE FALSE
#endif
#endif  /* SYSTEM == SYS_MINGW  */

#if     SYS_FAMILY == SYS_UNIX
#ifndef MBCHAR
#define MBCHAR              EUC_JP  /* UTF8 if you like */
#endif
#ifndef FNAME_FOLD
#define FNAME_FOLD          FALSE
#endif
#endif


#if     COMPILER == INDEPENDENT
/* specifications of compiler-independent build  */
#define LINE_PREFIX         "#line "
#define STD_LINE_PREFIX     TRUE    /* Output #line by C source format      */
#define HAVE_DIGRAPHS       TRUE    /* Output digraphs as it is     */
#define SEARCH_INIT         SOURCE  /* Include directory relative to source */
#define SJIS_IS_ESCAPE_FREE         TRUE    /* Do not treat SJIS specially  */
#define BIGFIVE_IS_ESCAPE_FREE      TRUE    /* Do not treat specially       */
#define ISO2022_JP_IS_ESCAPE_FREE   TRUE    /* Do not treat specially       */
#define TARGET_HAVE_LONG_LONG       TRUE    /* dummy    */
#define STDC_VERSION        199409L /* Initial value of __STDC_VERSION__    */
#endif

/*
 * defaults
 */

#ifdef  SYSTEM_EXT
#ifndef SYSTEM_EXT_VAL
#define SYSTEM_EXT_VAL      "1"
#endif
#endif
#ifdef  SYSTEM_EXT2
#ifndef SYSTEM_EXT2_VAL
#define SYSTEM_EXT2_VAL     "1"
#endif
#endif
#ifdef  COMPILER_STD1
#ifndef COMPILER_STD1_VAL
#define COMPILER_STD1_VAL   "1"
#endif
#endif
#ifdef  COMPILER_STD2
#ifndef COMPILER_STD2_VAL
#define COMPILER_STD2_VAL   "1"
#endif
#endif
#ifdef  COMPILER_EXT
#ifndef COMPILER_EXT_VAL
#define COMPILER_EXT_VAL    "1"
#endif
#endif
#ifdef  COMPILER_EXT2
#ifndef COMPILER_EXT2_VAL
#define COMPILER_EXT2_VAL   "1"
#endif
#endif
#ifdef  COMPILER_CPLUS
#ifndef COMPILER_CPLUS_VAL
#define COMPILER_CPLUS_VAL  "1"
#endif
#endif

#ifndef LINE_PREFIX
#define LINE_PREFIX         "#line "
#define STD_LINE_PREFIX     TRUE    /* C source format              */
#else
#ifndef STD_LINE_PREFIX
#define STD_LINE_PREFIX     FALSE   /* Compiler-dependent format    */
#endif
#endif

#ifndef HAVE_DIGRAPHS
#define HAVE_DIGRAPHS       FALSE
#endif

#ifndef ENV_C_INCLUDE_DIR
#define ENV_C_INCLUDE_DIR   "INCLUDE"
#endif
#ifndef ENV_CPLUS_INCLUDE_DIR
#define ENV_CPLUS_INCLUDE_DIR   "CPLUS_INCLUDE"
#endif

#ifndef ENV_SEP
#if     SYS_FAMILY == SYS_WIN
#define ENV_SEP             ';'
#else
#define ENV_SEP             ':'
#endif
#endif

#ifndef ONE_PASS
#define ONE_PASS            FALSE
#endif

#ifndef FNAME_FOLD
#define FNAME_FOLD          TRUE
#endif

#ifndef SEARCH_INIT
#define SEARCH_INIT         CURRENT
#endif

/*
 * CHARBIT, UCHARMAX are respectively CHAR_BIT, UCHAR_MAX of target compiler.
 * CHARBIT should be defined to the number of bits per character.
 * It is needed only for processing of multi-byte character constants.
 * UCHARMAX should be defined to the maximum value of type unsigned char
 * or maximum value of unsigned int which is converted from type (signed)
 * char.
 *
 * LONGMAX      should be defined to the LONG_MAX in <limits.h>.
 * ULONGMAX     should be defined to the ULONG_MAX in <limits.h> or LONG_MAX
 *              for the compiler which lacks of unsigned long.
 */

#ifndef CHARBIT
#define CHARBIT             8
#endif
#ifndef UCHARMAX
#define UCHARMAX            0xFF
#endif
#ifndef LONGMAX
#define LONGMAX             0x7FFFFFFFL
#endif
#ifndef ULONGMAX
#define ULONGMAX            0xFFFFFFFFUL
#endif

/*
 * Define MBCHAR (multi-byte character encoding) to SJIS, EUC_JP or other.
 */
#ifndef MBCHAR
#define MBCHAR              0
#endif

/*
 * SJIS_IS_ESCAPE_FREE means the compiler does not escape '0x5c' ('\\') in
 * shift-JIS encoded multi-byte character.  SJIS_IS_ESCAPE_FREE == FALSE 
 * enables cpp to insert * '\\' before '\\' of the 2nd byte of SJIS code in
 * literal.  This insertion is for the compiler-proper which can't recognize
 * SJIS literal.
 * BIGFIVE_IS_ESCAPE_FREE means similar case on BIGFIVE encoding.
 * ISO2022_JP_IS_ESCAPE_FREE means similar case on ISO2022_JP encoding.
 *
 * GCC can handle these encodings if it has been configured so.
 */
#ifndef SJIS_IS_ESCAPE_FREE
#define SJIS_IS_ESCAPE_FREE     FALSE   /* or TRUE following your compiler  */
#endif
#ifndef BIGFIVE_IS_ESCAPE_FREE
#define BIGFIVE_IS_ESCAPE_FREE  FALSE   /* or TRUE following your compiler  */
#endif
#ifndef ISO2022_JP_IS_ESCAPE_FREE
#define ISO2022_JP_IS_ESCAPE_FREE   FALSE   /* or TRUE following compiler   */
#endif

/*
 *      P A R T   2     Configurations for host-compiler.
 *
 *      WARNING: In case of HOST_COMPILER differs from COMPILER, you must
 *          edit here and there of this part.
 */
#if     HOST_COMPILER == GNUC
#if     __GNUC__ >= 3
#define HAVE_INTMAX_T               TRUE
#define HAVE_INTTYPES_H             TRUE
#endif
#define HOST_HAVE_LONG_LONG         TRUE 
#define HOST_HAVE_GETOPT            TRUE
#if     HOST_SYSTEM == SYS_LINUX
#define HOST_HAVE_STPCPY            TRUE
#define HOST_LIB_IS_GLIBC           TRUE
#endif

#elif   HOST_COMPILER == LCC
#define HOST_HAVE_LONG_LONG         TRUE 

#elif   HOST_COMPILER == MSC
#define HOST_HAVE_LONG_LONG         TRUE 

#elif   HOST_COMPILER == BORLANDC
#if     __BORLANDC__ >= 0x550
#define HOST_HAVE_LONG_LONG         TRUE 
#endif
#define HOST_HAVE_STPCPY            TRUE
#endif

#ifndef HAVE_INTMAX_T
#define HAVE_INTMAX_T               FALSE
#endif

/* The host compilers both have the type long long int ? */
#ifndef HOST_HAVE_LONG_LONG
#define HOST_HAVE_LONG_LONG         FALSE
#endif

/*
 * This definitions should be set TRUE, if *both* of the target
 * and the host compilers have long long type.
 */
#if TARGET_HAVE_LONG_LONG && HOST_HAVE_LONG_LONG
#define HAVE_LONG_LONG      TRUE
#endif

/*
 * Define the format specifier of intmax_t or long long for
 * fprintf( fp_debug,).
 * Both of target COMPILER and HOST_COMPILER should have long long.
 */
#if     HAVE_LONG_LONG
#if     HOST_COMPILER == BORLANDC   \
        || (HOST_COMPILER == MSC && _MSC_VER < 1400)    \
        || HOST_SYSTEM == SYS_MINGW
#define LL_FORM             "I64"   /* Format specifier for __int64 */
#endif
#ifndef LL_FORM
#define LL_FORM             "ll"    /* C99: for long long, "j" for intmax_t */
#endif
#endif

#ifndef HOST_HAVE_STPCPY
#define HOST_HAVE_STPCPY            FALSE
#endif
#ifndef HOST_HAVE_GETOPT
#define HOST_HAVE_GETOPT            FALSE
#endif
#ifndef HOST_LIB_IS_GLIBC
#define HOST_LIB_IS_GLIBC           FALSE
#endif

/*
 * Declaration of standard library functions and macros.
 */

/* stdin, stdout, stderr, FILE, NULL, fgets(), fputs() and other functions. */
#include    "stdio.h"

/* FILENAMEMAX should be defined to FILENAME_MAX of host system.    */
#ifdef  FILENAME_MAX
#define FILENAMEMAX     FILENAME_MAX
#else
#define FILENAMEMAX     BUFSIZ
#endif

/* islower(), isupper(), toupper(), isdigit(), isxdigit(), iscntrl()        */
#include    "ctype.h"

/* errno    */
#include    "errno.h"

#include    "string.h"
#include    "stdlib.h"
#include    "time.h"
#include    "setjmp.h"


#define NEED_GETOPT     TRUE


extern char *   stpcpy( char * dest, const char * src);

/* For debugging malloc systems by kmatsui  */
#if     KMMALLOC && _MEM_DEBUG
#include        "xalloc.h"
#endif

