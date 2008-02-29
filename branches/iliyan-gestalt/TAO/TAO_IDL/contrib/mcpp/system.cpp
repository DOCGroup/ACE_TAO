/*- $Id$
 * Copyright (c) 1998, 2002-2007 Kiyoshi Matsui <kmatsui@t3.rim.or.jp>
 * All rights reserved.
 *
 * Some parts of this code are derived from the public domain software
 * DECUS cpp (1984,1985) written by Martin Minow.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 *                          S Y S T E M . C
 *          S y s t e m   D e p e n d e n t   R o u t i n e s
 *
 * Routines dependent on character set, O.S., compiler or compiler-driver.
 * To implement MCPP for the systems not yet implemented, you must
 *      1. specify the constants in "configed.H" or "noconfig.H",
 *      2. append the system-dependent routines in this file.
 */
#if PREPROCESSED
#include    "mcpp.H"
#else
#include    "system.H"
#include    "internal.H"
#include    "mcpp_lib.h"
#endif

#include "ace/OS_NS_unistd.h"              /* For getcwd(), readlink(), getopt()   */
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_ctype.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_stdlib.h"

#undef OUT

/* Functions other than standard.   */
#if 1 //    HOST_SYS_FAMILY != SYS_UNIX     /* On UNIX "unistd.h" will suffice  */
//FUZZ: disable check_for_lack_ACE_OS
extern int      getopt( int argc, char * const * argv, const char * opts);
extern int      optind;
extern char *   optarg;
//FUZZ: enable check_for_lack_ACE_OS
#endif

/*
 * PATH_DELIM is defined for the O.S. which has single byte path-delimiter.
 * Note: '\\' or any other character identical to second byte of MBCHAR should
 * not be used for PATH_DELIM for convenience of path-list parsing.
 */
#if SYS_FAMILY == SYS_UNIX || SYS_FAMILY == SYS_WIN || SYSTEM == SYS_UNKNOWN
#define PATH_DELIM      '/'
#define SPECIAL_PATH_DELIM  FALSE
#else
#if SYSTEM == SYS_MAC
#define PATH_DELIM      ':'         /* ?? I don't know  */
#else   /* Any other path-delimiter, define by yourself */
#endif
#define SPECIAL_PATH_DELIM  TRUE    /* Any path-delimiter other than '/'    */
#endif

/*
 * OBJEXT is the suffix to denote "object" file.
 */
#ifndef OBJEXT
#if     SYS_FAMILY == SYS_UNIX
#define OBJEXT     "o"
#elif   SYS_FAMILY == SYS_WIN
#define OBJEXT     "obj"
#elif   1
/* Add here appropriate definitions for other systems.  */
#endif
#endif

static void     version( void);
                /* Print version message            */
static void     usage( int opt);
                /* Putout usage of MCPP             */
static void     set_opt_list( char * optlist);
                /* Set list of legal option chars   */
static int      parse_warn_level( const char * optarg, int opt);
                /* Parse warning level option       */
static void     def_a_macro( int opt, char * def);
                /* Do a -D option                   */
static void     chk_opts( int sflag, long std_val, int ansi, int trad);
                /* Check consistency of options     */
static void     init_predefines( int nflag, long std_val);
                /* Set and unset predefined macros  */
static void     init_std_defines( void);
                /* Predefine Standard macros        */
static void     set_limit( void);
                /* Set minimum translation limits   */
static void     set_pragma_op( void);
                /* Set the _Pragma() operator       */
static char *   set_files( int argc, char ** argv, char ** in_pp
        , char ** out_pp);
                /* Set input, output, diagnostic    */
static void     set_sys_dirs( int set_cplus_dir);
                /* Set system-specific include dirs */
static void     set_env_dirs( void);
                /* Set user-defined include dirs    */
static void     parse_env( const char * env);
                /* Parse environment variables      */
static void     set_a_dir( const char * dirname);
                /* Append an include directory      */
static char *   norm_path( const char * dir, const char * fname);
                /* Normalize pathname to compare    */
#if SYS_FAMILY == SYS_UNIX
static void     deref_syml( char * slbuf1, char * slbuf2, char * chk_start);
                /* Dereference symbolic linked dir  */
#endif
#if COMPILER == GNUC
static void     init_gcc_macro( int gcc_maj_ver, int gcc_min_ver);
                /* Predefine GCC macros             */
static void     undef_gcc_macro( int clearall);
                /* Undefine GCC predef-macros       */
static void     chk_env( void);
                /* Check the environment variables  */
#elif   COMPILER == MSC
static void     init_msc_macro( int wchar_t_modified);
                /* Predefine Visual C macros        */
#endif
static char *   md_init( const char * filename, char * output);
                /* Initialize makefile dependency   */
static char *   md_quote( char * output);
                /* 'Quote' special characters       */
static int      open_include( char * filename, int searchlocal, int next);
                /* Open the file to include         */
static int      has_directory( const char * source, char * directory);
                /* Get directory part of fname      */
static int      search_dir( char * filename, int searchlocal, int next);
                /* Search the include directories   */
static int      open_file( const char ** dirp, const char * filename
        , int local);
                /* Open a source file               */
static const char *     set_fname( const char * filename);
                /* Remember the source filename     */
static int      is_junk( void);
                /* The directive has trailing junk? */
static void     do_once( const char * dir, const char * filename);
                /* Process #pragma once             */
static int      included( const char * dir, const char * filename);
                /* The file has been once included? */
static void     push_or_pop( int direction);
                /* Push or pop a macro definition   */
static int      do_prestd_directive( void);
                /* Process pre-Standard directives  */
static void     do_preprocessed( void);
                /* Process preprocessed file        */
static int      do_debug( int set);
                /* #pragma MCPP debug, #debug       */
static void     dump_path( void);
                /* Print include search path        */
static void     do_asm( int asm_start);
                /* Process #asm, #endasm            */

#if SYS_FAMILY == SYS_WIN
static char *   bsl2sl( char * filename);       /* Convert \ to /   */
#endif

static int      mb_changed = FALSE;     /* Flag of -e option        */
static char     cur_work_dir[ FILENAMEMAX + 1]; /* Current working directory*/

/*
 * incdir[] stores the -I directories (and the system-specific #include <...>
 * directories).  This is set by set_a_dir().  A trailing PATH_DELIM is
 * appended if absent.
 */
static const char *     incdir[ NINCLUDE];      /* Include directories      */
static const char **    incend = incdir;        /* -> active end of incdir  */

/*
 * fnamelist[] stores the souce file names opened by #include directive for
 * debugging information.
 */
#define FNAMELIST   (NINCLUDE * 4)
static const char *     fnamelist[ FNAMELIST];  /* Source file names        */
static const char **    fname_end = fnamelist;
                                            /* -> active end of fnamelist   */

typedef struct inc_list {               /* List of #pragma once file*/
    struct inc_list *   next;           /* Next file                */
    char            fname[ 1];          /* Filename                 */
} INC_LIST;

static INC_LIST *   start_inc = NULL;   /* The first file in list   */
static INC_LIST *   last_inc;           /* The last file in list    */

/*
 * 'search_rule' holds searching rule of #include "header.h" to search first
 * before searching user specified or system-specific include directories.
 * 'search_rule' is initialized to SEARCH_INIT.  It can be changed by -I1, -I2
 * or -I3 option.  -I1 specifies CURRENT, -I2 SOURCE and -I3 both.
 */

static int      search_rule = SEARCH_INIT;  /* Rule to search include file  */

static int      dDflag = FALSE;         /* Flag of -dD option (for GCC)     */
static int      nflag = FALSE;          /* Flag of -N (-undef) option       */

/* Values of mkdep. */
#define MD_MKDEP        1   /* Output source file dependency line   */
#define MD_SYSHEADER    2   /* Print also system-header names       */
#define MD_FILE         4   /* Output to the file named *.d         */
#define MD_PHONY        8   /* Print also phony targets for each header */
#define MD_QUOTE        16  /* 'Quote' $ and space in target name   */

static FILE *   mkdep_fp;                       /* For -Mx option   */
static char *   mkdep_target;
    /* For -MT TARGET option and for GCC's queer environment variables.     */
static char *   mkdep_mf;               /* Argument of -MF option   */
static char *   mkdep_md;               /* Argument of -MD option   */
static char *   mkdep_mq;               /* Argument of -MQ option   */
static char *   mkdep_mt;               /* Argument of -MT option   */

/* sharp_filename is filename for #line line, used only in cur_file()   */
static char *   sharp_filename = NULL;

static char *   argv0;   /* argv[ 0] for usage() and version()   */

#if COMPILER == GNUC
/* sys_dirp indicates the first directory to search for system headers  */
static const char **    sys_dirp = incdir;      /* For -I- option   */
static int      gcc_work_dir = FALSE;           /* For -fworking-directory  */
static int      put_info_done = FALSE;  /* put_info() has been executed     */
static int      no_exceptions = FALSE;  /* For -fno-deprecated option       */
#endif

#if COMPILER == GNUC || COMPILER == MSC
/*
 * preinclude points to the file specified by -include (-Fl for MSC) option,
 * which is included prior to the main input file.
 */
#define         NPREINCLUDE 8
static char *   preinclude[ NPREINCLUDE];       /* File to pre-include      */
static char **  preinc_end = preinclude;    /* -> active end of preinclude  */
static int      dMflag = FALSE;                 /* Flag of -dM option       */
#endif

#if COMPILER == LCC
static const char *     optim_name = "__LCCOPTIMLEVEL";
#endif

#if SYSTEM == SYS_CYGWIN
static int      no_cygwin = FALSE;          /* -mno-cygwin          */
#endif

#if MCPP_LIB
void    init_system( void)
/* Initialize static variables  */
{
    if (sharp_filename)
        ACE_OS::free( sharp_filename);
    sharp_filename = 0;
    incend = incdir;
    fname_end = fnamelist;
    start_inc = 0;
    search_rule = SEARCH_INIT;
    mb_changed = dDflag = nflag = FALSE;
    mkdep_fp = 0;
    mkdep_target = mkdep_mf = mkdep_md = mkdep_mq = mkdep_mt = 0;
#if COMPILER == GNUC
    sys_dirp = incdir;
    gcc_work_dir = put_info_done = no_exceptions = FALSE;
#endif
#if COMPILER == GNUC || COMPILER == MSC
    preinc_end = preinclude;
    dMflag = FALSE;
#endif
#if SYSTEM == SYS_CYGWIN
    no_cygwin = FALSE;
#endif
}
#endif

#define LINE90LIMIT         32767
#define LINE_CPLUS_LIMIT    32767

#define OPTLISTLEN  80

void    do_options(
    int         argc,
    char **     argv,
    char **     in_pp,                      /* Input file name      */
    char **     out_pp                      /* Output file name     */
)
/*
 * Process command line arguments (-D, etc.), called only at MCPP startup.
 */
{
    char        optlist[ OPTLISTLEN];       /* List of option letter*/
    const char *    warning = "warning: -%c%s option is ignored\n";
    int         vflag;                      /* -v option            */
    int         unset_sys_dirs;
    /* Unset system-specific and site-specific include directories ?    */
    int         set_cplus_dir;  /* Set C++ include directory ? (for GCC)*/
    int         show_path;          /* Show include directory list  */
    DEFBUF *    defp;
    int         i;
    int         opt;
    char *      cp;

    long        std_val;        /* Value of __STDC_VERSION__ or __cplusplus */
    VAL_SIGN    *valp;
    int         sflag;                      /* -S option or similar */
    int         ansi, trad;                 /* -ansi, -traditional  */
    int         old_mode;                   /* backup of 'mcpp_mode'*/
#if COMPILER == GNUC
#define NSYSDIR   8
    /* System include directory specified by -isystem   */
    char *      sysdir[ NSYSDIR] = { 0, };
    char **     sysdir_end = sysdir;
    int         integrated_cpp; /* Flag of cc1 which integrates cpp in it   */
    int         gcc_maj_ver, gcc_min_ver;       /* __GNUC__, __GNUC_MINOR__ */
#elif   COMPILER == MSC
    int         wchar_t_modified = FALSE;   /* -Zc:wchar_t flag     */
#elif   COMPILER == LCC
    const char *    debug_name = "__LCCDEBUGLEVEL";
#endif

    argv0 = argv[ 0];
    vflag = nflag = unset_sys_dirs = show_path = FALSE;
    set_cplus_dir = TRUE;
    sflag = ansi = trad = FALSE;
    std_val = -1L;

    /* Get current directory for -I option and #pragma once */
        ACE_OS::getcwd( cur_work_dir, FILENAMEMAX);
#if FNAME_FOLD
    conv_case( cur_work_dir, cur_work_dir + ACE_OS::strlen( cur_work_dir), LOWER);
#endif
#if SYS_FAMILY == SYS_WIN
    bsl2sl( cur_work_dir);
#endif
    ACE_OS::sprintf( cur_work_dir + ACE_OS::strlen( cur_work_dir), "%c%c", PATH_DELIM, EOS);
        /* Append trailing path-delimiter   */

#if COMPILER == GNUC
    defp = look_id( "__GNUC__");
    gcc_maj_ver = ACE_OS::atoi( defp->repl);
    defp = look_id( "__GNUC_MINOR__");
    gcc_min_ver = ACE_OS::atoi( defp->repl);
    integrated_cpp = ((gcc_maj_ver == 3 && gcc_min_ver >= 3)
            || gcc_maj_ver == 4);
#endif

    set_opt_list( optlist);
    //FUZZ: disable check_for_lack_ACE_OS
opt_search: ;
        while (optind < argc
            && (opt = getopt( argc, argv, optlist)) != EOF) {
      //FUZZ: enable check_for_lack_ACE_OS
        switch (opt) {          /* Command line option character    */

#if COMPILER == GNUC && ! DOLLAR_IN_NAME
        case '$':                       /* Forbid '$' in identifier */
            break;                          /* Ignore this option   */
#endif

        case '+':
#if COMPILER == GNUC
plus:
#endif
            if (cplus_val || sflag) {
                mcpp_fputs( "warning: -+ option is ignored\n", ERR);
                break;
            }
            cplus_val = CPLUS;
            break;
        case '2':                   /* Revert digraphs recognition  */
            dig_flag = ! dig_flag;
            break;
        case '3':                   /* Revert trigraph recogniion   */
            trig_flag = ! trig_flag;
            break;

        case '@':                   /* Special preprocessing mode   */
            old_mode = mcpp_mode;
            if (str_eq( optarg, "post") || str_eq( optarg, "poststd"))
                mcpp_mode = POST_STD;   /* 'post-Standard' mode     */
            else if (str_eq( optarg, "old") || str_eq( optarg, "oldprep"))
                mcpp_mode = OLD_PREP;   /* 'old-Preprocessor' mode  */
            else if (str_eq( optarg, "kr"))
                mcpp_mode = KR;         /* 'K&R 1st' mode           */
            else if (str_eq( optarg, "std"))
                mcpp_mode = STD;        /* 'Standard' mode (default)*/
            else if (str_eq( optarg, "compat")) {
                compat_mode = TRUE;     /* 'compatible' mode        */
                mcpp_mode = STD;
            }
            else 
                usage( opt);
            standard = (mcpp_mode == STD || mcpp_mode == POST_STD);
            if (old_mode != STD && old_mode != mcpp_mode)
                mcpp_fprintf( ERR, "Mode is redefined to: %s\n", optarg);
            break;

#if COMPILER == GNUC
        case 'A':       /* Ignore -A system(gnu), -A cpu(vax) or so */
            break;
        case 'a':
            if (str_eq( optarg, "nsi")) {   /* -ansi                */
                look_and_install( "__STRICT_ANSI__", DEF_NOARGS, "", "1");
                ansi = TRUE;
                break;
            }
            usage( opt);
#elif   COMPILER == MSC
        case 'a':
            if (ACE_OS::memcmp( optarg, "rch", 3) == 0) {
                if (str_eq( optarg + 3, ":SSE")     /* -arch:SSE    */
                        || str_eq( optarg + 3, ":sse"))
                    look_and_install( "_M_IX86_FP", DEF_NOARGS, null, "1");
                else if (str_eq( optarg + 3, ":SSE2")       /* -arch:SSE2   */
                        || str_eq( optarg + 3, ":sse2"))
                    look_and_install( "_M_IX86_FP", DEF_NOARGS, null, "2");
                /* Else ignore  */
            } else {
                usage( opt);
            }
            break;

        case 'A':
            lang_asm = TRUE;                /* "assembler" source   */
            break;
#else
        case 'a':
            lang_asm = TRUE;                /* "assembler" source   */
            break;
#endif

#if ! STD_LINE_PREFIX
        case 'b':
            std_line_prefix = TRUE; /* Putout line and file infor-  */
            break;                  /*   mation in C source style.  */
#endif

        case 'C':                           /* Keep comments        */
            cflag = TRUE;
            break;

#if COMPILER == GNUC
        case 'c':
            if (! integrated_cpp)
                usage( opt);
            break;                  /* Else ignore this option      */
        case 'd':
            if (str_eq( optarg, "M"))       {       /* -dM          */
                dMflag = TRUE;
                no_output++;
            } else if (str_eq( optarg, "D"))  {     /* -dD          */
                dDflag = TRUE;
            } else if (str_eq( optarg, "igraphs")) {        /* -digraphs    */
                dig_flag = TRUE;
            } else if (str_eq( optarg, "umpbase")) {        /* -dumpbase    */
                ;                                   /* Ignore       */
            } else {
                usage( opt);
            }
            break;
#endif  /* COMPILER == GNUC */

        case 'D':                           /* Define symbol        */
            def_a_macro( opt, optarg);
            break;

        case 'e':
            /* Change the default MBCHAR encoding   */
            if (set_encoding( optarg, FALSE, 0) == 0)
                usage( opt);
            mb_changed = TRUE;
            break;

#if COMPILER == GNUC
        case 'E':
            if (! integrated_cpp)
                usage( opt);
            break;                          /* Ignore this option   */
        case 'f':
            if (ACE_OS::memcmp( optarg, "input-charset=", 14) == 0) {
                /* Treat -finput-charset= as the same option as -e  */
                if (set_encoding( optarg + 14, FALSE, 0) == 0)
                    usage( opt);
                mb_changed = TRUE;
                break;
            } else if (str_eq( optarg, "working-directory")) {
                gcc_work_dir = TRUE;
                break;
            } else if (str_eq( optarg, "no-working-directory")) {
                gcc_work_dir = FALSE;
                break;
            } else if (str_eq( optarg, "stack-protector")) {
                look_and_install( "__SSP__", DEF_NOARGS, null, "1");
                break;
            } else if (str_eq( optarg, "stack-protector-all")) {
                look_and_install( "__SSP_ALL__", DEF_NOARGS, null, "2");
                break;
            } else if (str_eq( optarg, "exceptions")) {
                look_and_install( "__EXCEPTIONS", DEF_NOARGS, null, "1");
                break;
            } else if (str_eq( optarg, "no-exceptions")) {
                no_exceptions = TRUE;
                break;
            } else if (str_eq( optarg, "PIC") || str_eq( optarg, "pic")
                    || str_eq( optarg, "PIE") || str_eq( optarg, "pie")) {
                look_and_install( "__PIC__", DEF_NOARGS, null, "1");
                look_and_install( "__pic__", DEF_NOARGS, null, "1");
                break;
            } else if (str_eq( optarg, "no-show-column")) {
                break;                      /* Ignore this option   */
            } else if (! integrated_cpp) {
                usage( opt);
            }
            break;

        case 'g':
            if (!ACE_OS::ace_isdigit( *optarg) && str_eq( argv[ optind - 2], "-g"))
                optind--;   /* Neither '-g 0' nor '-ggdb' -- No argument    */
            break;                          /* Ignore the option    */
#elif COMPILER == LCC
        case 'g':               /* Define __LCCDEBUGLEVEL as <n>    */
            if (*(optarg + 1) == EOS && ACE_OS::ace_isdigit( *optarg)) {
                defp = look_id( debug_name);
                ACE_OS::strcpy( defp->repl, optarg);
            } else {
                usage( opt);
            }
            break;
#elif COMPILER == MSC
        case 'G':
            if (*(optarg + 1) == EOS) {     /* -Gx                  */
                char    val[ 4] = "000";

                switch (*optarg) {
                case '3':   case '4':   case '5':   case '6':
                    *val = *optarg; /* "300", "400", "500", "600"   */
                    break;
                case 'B':                   /* -GB                  */
                    *val = '6';
                    break;
                case 'R':
                    look_and_install( "_CPPRTTI", DEF_NOARGS, null, "1");
                    break;
                case 'X':
                    look_and_install( "_CPPUNWIND", DEF_NOARGS, null, "1");
                    break;
                case 'Z':
                    look_and_install( "__MSVC_RUNTIME_CHECKS", DEF_NOARGS
                            , null, "1");
                    break;
                default :
                    mcpp_fprintf( ERR, warning, opt, optarg);
                }
                if (*val)
                    look_and_install( COMPILER_SP2, DEF_NOARGS, null, val);
            } else {
                usage( opt);
            }
            break;
#endif

        case 'h':
            if (*(optarg + 1) == EOS && ACE_OS::ace_isdigit( *optarg))      /* a digit  */
                look_and_install( "__STDC_HOSTED__", DEF_NOARGS - 1, null
                        , optarg);
            else
                usage( opt);
            break;

#if COMPILER == MSC
        case 'X':
                unset_sys_dirs = TRUE;
                break;
#endif
        case 'I':                           /* Include directory    */
            if (str_eq( optarg, "-")) {     /* -I-                  */
#if COMPILER == GNUC
                sys_dirp = incend;  /* Split include directories    */
#else
                unset_sys_dirs = TRUE;      /* Unset pre-specified  */
                break;                      /*   include directories*/
#endif
            }
            if (*(optarg + 1) == EOS && ACE_OS::ace_isdigit( *optarg)
                    && (i = *optarg - '0') != 0
                    && (i & ~(CURRENT | SOURCE)) == 0) {
                search_rule = i;            /* -I1, -I2 or -I3      */
                break;
            }
            set_a_dir( optarg);             /* User-defined direct. */
            break;

#if COMPILER == MSC
        case 'F':
            if (str_eq( optarg, "l")) {             /* -Fl          */
                if (preinc_end >= &preinclude[ NPREINCLUDE]) {
                    mcpp_fputs( "Too many -Fl options.\n", ERR);
                    longjmp( error_exit, -1);
                }
                *preinc_end++ = argv[ optind++];
            } else {
                usage( opt);
            }
            break;
#endif

#if COMPILER == GNUC
        case 'i':
            if (str_eq( optarg, "nclude")) {        /* -include     */
                if (preinc_end >= &preinclude[ NPREINCLUDE]) {
                    mcpp_fputs( "Too many -include options.\n", ERR);
                    longjmp( error_exit, -1);
                }
                *preinc_end++ = argv[ optind++];
            } else if (str_eq( optarg, "system")) { /* -isystem     */
                if (sysdir_end >= &sysdir[ NSYSDIR]) {
                    mcpp_fputs( "Too many -isystem options.\n", ERR);
                    longjmp( error_exit, -1);
                }
                *sysdir_end++ = argv[ optind++];
                /* Add the directory before system include directory*/
            } else if (str_eq( optarg, "prefix")        /* -iprefix */
                    || str_eq( optarg, "withprefix")    /* -iwithprefix     */
                    || str_eq( optarg, "withprefixbefore")
                                            /* -iwithprefixbefore   */
                    || str_eq( optarg, "dirafter")) {   /* -idirafter       */
                optind++;                   /* Skip the argument    */
                /* Ignore these options */
            } else {
                usage( opt);
            }
            break;
#endif

        case 'j':
            no_source_line = TRUE;
            break;  /* Do not output the source line in diagnostics */

#if COMPILER == MSC
        case 'J':
            look_and_install( "_CHAR_UNSIGNED", DEF_NOARGS, null, "1");
            break;
#endif

#if COMPILER == GNUC
        case 'l':
            if (ACE_OS::memcmp( optarg, "ang-", 4) != 0) {
                usage( opt);
            } else if (str_eq( optarg + 4, "c")) {      /* -lang-c          */
                break;                      /* Ignore this option   */
            } else if (str_eq( optarg + 4, "c99")       /* -lang-c99*/
                        || str_eq( optarg + 4, "c9x")) {    /* -lang-c9x    */
                if (! sflag) {
                    look_and_install( "__STRICT_ANSI__", DEF_NOARGS, "", "1");
                    stdc_val = 1;           /* Define __STDC__ to 1 */
                    std_val = 199901L;
                    sflag = TRUE;
                }
            } else if (str_eq( optarg + 4, "c89")) {    /* -lang-c89*/
                if (! sflag) {
                    look_and_install( "__STRICT_ANSI__", DEF_NOARGS, "", "1");
                    stdc_val = 1;           /* Define __STDC__ to 1 */
                    sflag = TRUE;
                }
            } else if (str_eq( optarg + 4, "c++")) {    /* -lang-c++*/
                goto  plus;
            } else if (str_eq( optarg + 4, "asm")) {    /* -lang-asm*/
                lang_asm = TRUE;
                break;
            } else {
                usage( opt);
            }
            break;
#endif  /* COMPILER == GNUC */

        case 'M':           /* Output source file dependency line   */
            if (str_eq( optarg, "M")) {                     /* -MM  */
                ;
            } else if (str_eq( optarg, "D")) {              /* -MD  */
                mkdep |= (MD_SYSHEADER | MD_FILE);
            } else if (str_eq( optarg, "MD")) {             /* -MMD */
                mkdep |= MD_FILE;
            } else if (str_eq( optarg, "P")) {              /* -MP  */
                mkdep |= MD_PHONY;
            } else if (str_eq( optarg, "Q")) {      /* -MQ target   */
                mkdep |= MD_QUOTE;
                mkdep_mq = argv[ optind++];
            } else if (str_eq( optarg, "T")) {      /* -MT target   */
                mkdep_mt = argv[ optind++];
            } else if (str_eq( optarg, "F")) {      /* -MF file     */
                mkdep_mf = argv[ optind++];
            } else if (argv[ optind - 1] == optarg) {       /* -M   */
                mkdep |= MD_SYSHEADER;
                optind--;
            } else {
                usage( opt);
            }
            if (str_eq( optarg, "D") || str_eq( optarg, "MD")) {
                cp = argv[ optind];
                if (cp && *cp != '-')           /* -MD (-MMD) file  */
                    mkdep_md = argv[ optind++];
            }
            mkdep |= MD_MKDEP;
            break;

#if COMPILER == GNUC
        case 'm':
#if SYSTEM == SYS_CYGWIN
            if (str_eq( optarg, "no-cygwin")) { /* -mno-cygwin      */
                no_cygwin = TRUE;
                break;
            }
#endif
            if (! integrated_cpp)
                usage( opt);
            break;
#endif

#if COMPILER == GNUC
        case 'u':
            if (! str_eq( optarg, "ndef"))  /* -undef               */
                usage( opt);                /* Else fall through    */
#endif

#if COMPILER == MSC
        case 'u':
#endif
        case 'N':
            /* No predefines:   remove "vax", "__VAX" and friends.  */
            nflag = TRUE;
            break;

#if COMPILER == GNUC
        case 'n':
            if (str_eq( optarg, "ostdinc")) {               /* -nostdinc    */
                unset_sys_dirs = TRUE;  /* Unset pre-specified directories  */
            } else if (str_eq( optarg, "ostdinc++")) {      /* -nostdinc++  */
                set_cplus_dir = FALSE;  /* Unset C++-specific directories   */
            } else if (str_eq( optarg, "oprecomp")) {       /* -noprecomp   */
                mcpp_fprintf( ERR, warning, opt, optarg);
                break;
            } else {
                usage( opt);
            }
            break;
#endif

#if COMPILER == GNUC
        case 'O':
            if (integrated_cpp) {
                if (*optarg == '-')                 /* No argument  */
                    optind--;
                else if (! ACE_OS::ace_isdigit( *optarg))
                    usage( opt);
                else if (*optarg != '0')
                    look_and_install( "__OPTIMIZE__", DEF_NOARGS, "", "1");
            } else {
                usage( opt);
            }
            break;                  /* Else ignore -Ox option       */
#elif COMPILER == LCC
        case 'O':                   /* Define __LCCOPTIMLEVEL as 1  */
            defp = look_id( optim_name);
            ACE_OS::strcpy( defp->repl, "1");
            break;
#endif

        case 'o':
            *out_pp = optarg;               /* Output file name     */
            break;

        case 'P':                           /* No #line output      */
            pflag = TRUE;
            break;

#if COMPILER == GNUC
        case 'p':
            if (str_eq( optarg, "edantic")          /* -pedantic    */
                    || str_eq( optarg, "edantic-errors")) {
                                            /* -pedantic-errors     */
                if (warn_level == -1)
                    warn_level = 0;
                warn_level |= (1 | 2 | 4);
                if (! sflag && ! cplus_val) {
                    stdc_val = 1;
                    sflag = TRUE;
                }
            } else {
                usage( opt);
            }
            break;
        case 'q':
            if (str_eq( optarg, "uiet"))            /* -quiet       */
                break;                      /* Ignore the option    */
            else
                usage( opt);
            break;
#endif  /* COMPILER == GNUC */

        case 'Q':
            qflag = TRUE;
            break;

#if COMPILER == MSC
        case 'R':               /* -RTC1, -RTCc, -RTCs, -RTCu, etc. */
            if (ACE_OS::memcmp( optarg, "TC", 2) == 0 && *(optarg + 2) != EOS)
                look_and_install( "__MSVC_RUNTIME_CHECKS", DEF_NOARGS, null
                        , "1");
            else
                usage( opt);
            break;
#endif

        case 'S':
            if (cplus_val || sflag) {   /* C++ or the second time   */
                mcpp_fprintf( ERR, warning, opt, optarg);
                break;
            }
            i = *optarg;
            if (! ACE_OS::ace_isdigit( i) || *(optarg + 1) != EOS)
                usage( opt);
            stdc_val = i - '0';
            sflag = TRUE;
            break;

#if COMPILER == GNUC
        case 'r':
            if (str_eq( optarg, "emap"))
                mcpp_fprintf( ERR, warning, opt, optarg);
                                            /* Ignore -remap option */
            else
                usage( opt);
            break;

        case 's':
            if (ACE_OS::memcmp( optarg, "td=", 3) == 0 && ACE_OS::strlen( optarg) > 3) {
                /* -std=STANDARD    */
                cp = optarg + 3;
                if (str_eq( cp, "c89")              /* std=c89      */
                        || str_eq( cp, "c90")       /* std=c90      */
                        || str_eq( cp, "gnu89")     /* std=gnu89    */
                        || str_eq( cp, "iso9899:1990")) {
                    std_val = 0L;               /* C90 + extensions */
                } else if (str_eq( cp, "c99")       /* std=c99      */
                        || str_eq( cp, "c9x")       /* std=c9x      */
                        || str_eq( cp, "gnu99")     /* std=gnu99    */
                        || str_eq( cp, "gnu9x")     /* std=gnu9x    */
                        || str_eq( cp, "iso9899:1999")
                        || str_eq( cp, "iso9899:199x")) {
                    std_val = 199901L;
                } else if (str_eq( cp, "c++98")) {  /* std=c++98    */
                    cplus_val = std_val = 199711L;
                } else if (ACE_OS::memcmp( cp, "iso9899:", 8) == 0
                        && ACE_OS::strlen( cp) >= 14) { /* std=iso9899:199409, etc. */
                    optarg = cp + 8;
                    goto Version;
                } else if (ACE_OS::memcmp( cp, "iso14882", 8) == 0) {
                    cp += 8;
                    if (cp && *cp == ':' && ACE_OS::strlen( cp) >= 7) {
                                    /* std=iso14882:199711, etc.    */
                        cplus_val = CPLUS;
                        optarg = cp + 1;
                        goto Version;
                    } else {
                        goto plus;
                    }
                } else {
                    usage( opt);
                }
                if (! cplus_val && ACE_OS::memcmp( cp, "gnu", 3) != 0)
                    look_and_install( "__STRICT_ANSI__", DEF_NOARGS, "", "1");
                stdc_val = 1;
                sflag = TRUE;
            } else {
                usage( opt);
            }
            break;

        case 't':
            if (str_eq( optarg, "raditional")
                    || str_eq( optarg, "raditional-cpp")) {
                                /* -traditional, -traditional-cpp   */
                trad = TRUE;
                mcpp_mode = OLD_PREP;
            } else if (str_eq( optarg, "rigraphs")) {
                trig_flag = TRUE;                   /* -trigraphs   */
            } else {
                usage( opt);
            }
            break;
#endif  /* COMPILER == GNUC */

#if COMPILER == MSC
        case 'T':
            if (ACE_OS::strlen( optarg) > 1)
                usage( opt);
            i = ACE_OS::tolower( *optarg);                  /* Fold case    */
            if (i == 'c') {
                break;                      /* Ignore this option   */
            } else if (i == 'p') {
                cplus_val = CPLUS;
                break;
            } else {
                usage( opt);
            }
#endif

        case 'U':                           /* Undefine symbol      */
            /*
             * We don't need to map trigraphs as they can't be part of a
             * symbol name. (_ isn't trigraphable).
             */
            if ((defp = look_id( optarg)) != 0) {
                if (defp->nargs == DEF_NOARGS - 1) {
                    undef_a_predef( optarg);
                }
                undefine( optarg);
            } else {
                mcpp_fprintf( ERR, "\"%s\" wasn't defined\n", optarg);
            }
            break;

        case 'V':
#if COMPILER == GNUC
Version:
#endif
            valp = eval_num( optarg);
            if (valp->sign == VAL_ERROR)
                usage( opt);
            std_val = (long) valp->val;
            break;

        case 'v':
            vflag = TRUE;
            show_path = TRUE;
            break;

        case 'W':                           /* warning level        */
            if (warn_level == -1)           /* Have to initialize   */
                warn_level = 0;
#if COMPILER == GNUC
            if (argv[ optind - 1] == optarg) {      /* No argument  */
                /*
                 * Note: -W without argument is not officially supported.
                 *  It may cause an error.
                 */
                warn_level |= (1 | 2 | 4 | 16);
                optind--;
                break;
            } else if (str_eq( optarg, "comment")
                        || str_eq( optarg, "comments")
                        || str_eq( optarg, "sign-compare")) {
                warn_level |= 1;
                break;
            } else if (str_eq( optarg, "undef")) {
                warn_level |= 4;
                break;
            } else if (str_eq( optarg, "all")) {
                warn_level |= (1 | 16);     /* Convert -Wall to -W17*/
                break;
            } else if (str_eq( optarg, "trigraphs")) {
                warn_level |= 16;
                break;
            }
#endif  /* COMPILER == GNUC */
#if COMPILER == MSC
            if (str_eq( optarg, "all")) {
                warn_level |= (1 | 16);     /* Convert -Wall to -W17*/
                break;
            } else if (str_eq( optarg, "L")) {
                no_source_line = TRUE;  /* Single-line diagnostic   */
                break;
            }
#endif
            if (ACE_OS::ace_isdigit( *optarg)) {
                warn_level |= parse_warn_level( optarg, opt);
                if (warn_level > 31 || warn_level < 0)
                    usage( opt);
            }
            if (warn_level == 0)
                warn_level = 0xFF;          /* Remember this option */
            /* Else ignore the option   */
            break;

#if COMPILER == GNUC || COMPILER == MSC
        case 'w':                           /* Same as -W0          */
            warn_level = 0xFF;              /* Remenber this option */
            break;
#endif

#if COMPILER == GNUC
        case 'x':
            if (str_eq( optarg, "c")) {
                break;                      /* -x c -- ignore this  */
            } else if (str_eq( optarg, "c++")) {
                goto plus;
            } else if (str_eq( optarg, "assembler-with-cpp")) {
                lang_asm = TRUE;
                break;
            } else {
                usage( opt);
            }
            break;
#endif

#if COMPILER == MSC
        case 'Z':
            if (str_eq( optarg, "c:wchar_t")) {     /* -Zc:wchar_t  */
                look_and_install( "_NATIVE_WCHAR_T_DEFINED", DEF_NOARGS, null
                        , "1");
                look_and_install( "_WCHAR_T_DEFINED", DEF_NOARGS, null, "1");
                wchar_t_modified = TRUE;
            } else if (str_eq( optarg, "c:wchar_t-")) {     /* -Zc:wchar_t- */
                wchar_t_modified = TRUE;        /* Do not define the macros */
            } else if (str_eq( optarg, "l")) {
                look_and_install( "_VC_NODEFAULTLIB", DEF_NOARGS, null, "1");
            } else if (str_eq( optarg, "a") || str_eq( optarg, "e")) {
                /* Ignore -Za and -Ze silently  */
                break;
            } else if (*(optarg + 1) == EOS) {  /* -Z followed by one char  */
                mcpp_fprintf( ERR, warning, opt, optarg);
                /* Ignore the option with warning   */
            } else {
                usage( opt);
            }
            break;
#endif

        case 'z':
            zflag = TRUE;           /* No output of included file   */
            break;

        default:                            /* What is this one?    */
            usage( opt);
            break;
        }                               /* Switch on all options    */

    }                                   /* For all arguments        */

    if (optind < argc && set_files( argc, argv, in_pp, out_pp) != 0)
        goto  opt_search;       /* More options after the filename  */

    /* Check consistency of specified options, set some variables   */
    chk_opts( sflag, std_val, ansi, trad);

    if (warn_level == -1)               /* No -W option             */
        warn_level = 1;                 /* Default warning level    */
    else if (warn_level == 0xFF)
        warn_level = 0;                 /* -W0 has high precedence  */

#if COMPILER == GNUC
    if (sysdir < sysdir_end) {
        char **     dp = sysdir;
        while (dp < sysdir_end)
            set_a_dir( *dp++);
    }
    if (*in_pp && str_eq( (*in_pp) + ACE_OS::strlen( *in_pp) - 2, ".S"))
        lang_asm = TRUE;                /* Input file name is *.S   */
    if (lang_asm)
        look_and_install( "__ASSEMBLER__", DEF_NOARGS, null, "1");
#endif
    set_env_dirs();
    if (! unset_sys_dirs)
        set_sys_dirs( set_cplus_dir);

    if (mkdep_mf) {                         /* -MF overrides -MD    */
        mkdep_fp = ACE_OS::fopen( mkdep_mf, "w");
    } else if (mkdep_md) {
        mkdep_fp = ACE_OS::fopen( mkdep_md, "w");
    }
    if (mkdep_mq)                           /* -MQ overrides -MT    */
        mkdep_target = mkdep_mq;
    else if (mkdep_mt)
        mkdep_target = mkdep_mt;

#if COMPILER == GNUC
    init_gcc_macro( gcc_maj_ver, gcc_min_ver);
    chk_env();  /* Check the env-vars to specify version and dependency line*/
#elif   COMPILER == MSC
    init_msc_macro( wchar_t_modified);
#endif

    init_predefines( nflag, std_val);

    if (vflag)
        version();
    if (show_path) {
        fp_debug = stderr;
        dump_path();
        fp_debug = stdout;
    }
}

static void version( void)
/*
 * Print version message.
 */
{
    const char *    mes[] = {

#if     MCPP_LIB
/* Write messages here. */
        0, " with ",
#endif

#ifdef  VERSION_MSG
        "MCPP V.2.6.4 (2007/05) "
#else
        "MCPP V.", VERSION, " (", DATE, ") "
#endif
#if     COMPILER == INDEPENDENT
            , "compiler-independent-build "
#else
#ifdef  CMP_NAME
            , "for ", CMP_NAME, " "
#endif
#endif
            , "compiled by "
#ifdef  VERSION_MSG
            , VERSION_MSG
#else
#ifdef  HOST_CMP_NAME
            , HOST_CMP_NAME
#if     HOST_COMPILER == GNUC
            , " V.", GCC_MAJOR_VERSION, ".", GCC_MINOR_VERSION
#endif
#endif
#endif
            , "\n",
            0
        };

    const char **   mpp = mes;
#if     MCPP_LIB
    mes[ 0] = argv0;
#endif
    while (*mpp)
        mcpp_fputs( *mpp++, ERR);
}

static void usage(
    int     opt
)
/*
 * Print usage.
 */
{
    const char *     mes[] = {

"Usage:  ",
"mcpp",
" [-<opts> [-<opts>]] [<infile> [-<opts>] [<outfile>] [-<opts>]]\n",
"    <infile> defaults to stdin and <outfile> defaults to stdout.\n",

"\nCommonly used options:\n",

"-@MODE      Specify preprocessing mode. MODE should be one of these 4:\n",
"    -@std               Standard conforming mode. (default)\n",
"    -@poststd, -@post   special 'post-Standard' mode.\n",
"    -@kr                K&R 1st mode.\n",
"    -@oldprep, -@old    'old_preprocessor' mode (i.e. 'Reiser model' cpp).\n",

#if COMPILER == MSC
"-arch:SSE, -arch:SSE2   Define the macro _M_IX86_FP as 1, 2 respectively.\n",
#endif

#if ! STD_LINE_PREFIX
"-b          Output #line lines in C source style.\n",
#endif

"-C          Output also comments.\n",
"-D <macro>[=<value>]    Define <macro> as <value> (default:1).\n",
"-D <macro(args)>[=<replace>]    Define <macro(args)> as <replace>.\n",
"-e <encoding>   Change the default multi-byte character encoding to one of:\n",
"            euc_jp, gb2312, ksc5601, big5, sjis, iso2022_jp, utf8.\n",

#if COMPILER == GNUC
"-finput-charset=<encoding>      Same as -e <encoding>.\n",
"            (Don't insert spaces around '=').\n",
#endif
#if COMPILER == MSC
"-Fl <file>  Include the <file> prior to the main input file.\n",
"-G<n>       Define the macro _M_IX86 according to <n>.\n",
#endif
#if COMPILER == LCC
"-g <n>      Define the macro __LCCDEBUGLEVEL as <n>.\n",
#endif

"-I <directory>      Add <directory> to the #include search list.\n",
"-I-         Unset system or site specific include directories.\n",

#if COMPILER == GNUC
"-include <file>     Include the <file> prior to the main input file.\n",
#endif
#if COMPILER == MSC
"-J          Define the macro _CHAR_UNSIGNED as 1.\n",
#endif

"-j          Don't output the source line in diagnostics.\n",
"-M, -MM, -MD, -MMD, -MP, -MQ target, -MT target, -MF file\n",
"            Output source file dependency line for makefile.\n",
"-N          Don't predefine any non-standard macros.\n",

#if COMPILER == GNUC
"-nostdinc   Unset system or site specific include directories.\n",
#endif
#if COMPILER == LCC
"-O          Define the macro __LCCOPTIMLEVEL as 1.\n",
#endif

"-o <file>   Output to <file>.\n",
"-P          Don't output #line lines.\n",
"-Q          Output diagnostics to \"mcpp.err\" (default:stderr).\n",
#if COMPILER == MSC
"-RTC*       Define the macro __MSVC_RUNTIME_CHECKS as 1.\n",
#endif
#if COMPILER == GNUC
"-traditional, -traditional-cpp      Same as -@oldprep.\n",
#endif
"-U <macro>  Undefine <macro>.\n",

#if COMPILER == GNUC
"-undef      Same as -N.\n",
#endif
#if COMPILER == MSC
"-u          Same as -N.\n",
#endif

"-v          Show version and include directories of mcpp.\n",
"-W <level>  Set warning level to <level> (OR of {0,1,2,4,8,16}, default:1).\n",

#if COMPILER == MSC
"-WL         Same as -j.\n",
#endif
#if COMPILER == MSC || COMPILER == GNUC
"-w          Same as -W0.\n",
#endif
#if COMPILER == MSC
"-X          Same as -I-.\n",
"-Zc:wchar_t     Define _NATIVE_WCHAR_T_DEFINED and _WCHAR_T_DEFINED as 1.\n",
"-Zl         Define the macro _VC_NODEFAULTLIB as 1.\n",
#endif

"-z          Don't output the included file, only defining macros.\n",

"\nOptions available with -@std (default) or -@poststd options:\n",

"-+          Process C++ source.\n",

#if DIGRAPHS_INIT
"-2          Disable digraphs.\n",
#else
"-2          Enable digraphs.\n",
#endif
#if COMPILER == GNUC
"-digraphs   Enable digraphs.\n",
#endif

"-h <n>      Re-define the pre-defined macro __STDC_HOSTED__ as <n>.\n",

#if COMPILER == GNUC
"-lang-c89   Same as -S1.\n",
"-lang-c++   Same as -+.\n",
"-pedantic, -pedantic-errors     Same as -W7.\n",
#endif

"-S <n>      Redefine __STDC__ to <n>, undefine old style macros.\n",

#if COMPILER == GNUC
"-std=<STANDARD>     Specify the standard to which the code should conform.\n",
"            <STANDARD> may be one of: c90, c99, iso9899:1990, iso14882, etc.\n",
"            iso9899:<n>, iso14882:<n> : Same as -V <n> (long in decimals).\n",
#endif
#if COMPILER == MSC
"-Tp         Same as -+.\n",
#endif

"-V <n>      Redefine __STDC_VERSION__ or __cplusplus to <n>.\n",
"            C with -V199901L specifies C99 specs.\n",
"            C++ with -V199901L specifies C99 compatible specs.\n",

#if COMPILER == GNUC
"-x c++      Same as -+.\n",
#endif

"\nOptions available with only -@std (default) option:\n",

"-@compat    Expand recursive macro more than Standard.\n",
#if TRIGRAPHS_INIT
"-3          Disable trigraphs.\n",
#else
"-3          Enable trigraphs.\n",
#endif
#if COMPILER == GNUC
"-trigraphs  Enable trigraphs.\n",
#endif

"\nOptions available with -@std (default), -@kr or -@oldprep options:\n",

#if COMPILER == GNUC
"-lang-asm   Same as -x assembler-with-cpp.\n",
"-x assembler-with-cpp   Process \"assembler\" source.\n",
#elif   COMPILER == MSC
"-A          Process \"assembler\" source.\n",
#else
"-a          Process \"assembler\" source.\n",
#endif

"\nFor further details see mcpp-manual.html.\n",
        0,
    };

    const char *    illegopt = "Incorrect option -%c%s\n";
    const char * const *    mpp = mes;

    if (opt != '?')
        mcpp_fprintf( ERR, illegopt, opt, optarg ? optarg : "");
    version();
#if MCPP_LIB
    mes[ 1] = argv0;
#endif
    while (*mpp)
        mcpp_fputs( *mpp++, ERR);
    longjmp( error_exit, -1);
}

static void set_opt_list(
    char *  optlist
)
/*
 * Set list of legal option characters.
 */
{
    const char *    list[] = {

#if ! STD_LINE_PREFIX
    "b",
#endif

#if COMPILER == GNUC
    "$A:a:cd:Ef:g:i:l:m:n:r:s:t:u:O:p:q:wx:",
#elif   COMPILER != MSC
    "a",
#endif

#if COMPILER == MSC
    "Aa:F:G:JR:T:XZ:uw",
#elif   COMPILER == LCC
    "g:O",
#endif
    0
    };

    const char * const *    lp = & list[ 0];

    ACE_OS::strcpy( optlist, "23+@:e:h:jo:vzCD:I:M:NPQS:U:V:W:");
                                                /* Default options  */
    while (*lp)
        ACE_OS::strcat( optlist, *lp++);
    if (ACE_OS::strlen( optlist) >= OPTLISTLEN)
        cfatal( "Bug: Too long option list", 0, 0L, 0);       /* _F_  */
}

static int  parse_warn_level(
    const char *    optarg,
    int     opt
)
/*
 * Parse warn level option.
 * Warning level option is specified as '19' or '1|2|16' or even '3|16'.
 * Even spaces are allowed as ' 1 | 2|16 '.
 */
{
    const char *    cp = optarg;
    int             w, i;

    w = i = 0;
    while( *cp != EOS) {
        while( *cp == ' ')
            cp++;                           /* Skip spaces          */
        if (! ACE_OS::ace_isdigit( *cp))
            break;                          /* Error    */
        while (ACE_OS::ace_isdigit( *cp)) {
            i *= 10;
            i += (*cp++ - '0');
        }
        while (*cp == ' ')
            cp++;
        if (*cp == '|') {       /* Only digits or '|' are allowed   */
            w |= i;                         /* Take OR of the args  */
            i = 0;
            cp++;
        }
    }
    if (*cp != EOS) {               /* Not ending with digit        */
        mcpp_fprintf( ERR, "Illegal warning level option \"%s\"\n", optarg);
        usage( opt);
    }
    w |= i;                                 /* Take the last arg    */
    return  w;
}

static void def_a_macro(
    int     opt,
    char *  def
)
/*
 * Define a macro specified by -D option.
 * The macro maybe either object-like or function-like (with parameter).
 */
{
    DEFBUF *    defp;
    char *      definition;                 /* Argument of -D option*/
    char *      cp;
    int         i;

    /* Convert trigraphs for the environment which need trigraphs   */
    if (mcpp_mode == STD && trig_flag)
        cnv_trigraph( def);
    if (mcpp_mode == POST_STD && dig_flag)
        cnv_digraph( def);  /* Convert prior to installing macro    */
    definition = xmalloc( ACE_OS::strlen( def) + 4);
    ACE_OS::strcpy( definition, def);
    if ((cp = ACE_OS::strchr( definition, '=')) != 0) {
        *cp = ' ';                          /* Remove the '='       */
        cp = "\n";                          /* Append <newline>     */
    } else {
        cp = " 1\n";                        /* With definition "1"  */
    }
    ACE_OS::strcat( definition, cp);
    cp = definition;
    while ((char_type[ *cp & UCHARMAX] & SPA) == 0)
        cp++;
    i = *cp;
    *cp = EOS;
    if ((defp = look_id( definition)) != 0) {    /* Pre-defined  */
        if (defp->nargs == DEF_NOARGS - 1) {
            undef_a_predef( definition);
            /* Remove the name from the table of pre-defined-macros.*/
        }
        undefine( definition);
    }
    *cp = i;
    /* Now, save the definition.    */
    unget_string( definition, 0);
    if (do_define( FALSE) == 0)          /* Define a macro       */
        usage( opt);
    *cp = EOS;
    if (str_eq( definition, "__STDC__")) {
        defp = look_id( definition);
        defp->nargs = DEF_NOARGS - 2;
                                /* Restore Standard-predefinedness  */
    }
    ACE_OS::free( definition);
    skip_nl();                      /* Clear the appended <newline> */
}

static void     chk_opts( 
    int     sflag,      /* Flag of Standard or post-Standard mode   */
    long    std_val,                /* Value of __STDC_VERSION__    */
    int     ansi,                   /* -ansi (GCC only)             */
    int     trad                    /* -traditional (GCC only)      */
)
/*
 * Check consistency between the specified options.
 * Set default value of some variables for each 'mcpp_mode'.
 */
{
    int     incompat = FALSE;

    switch (mcpp_mode) {
    case STD    :
    case POST_STD   :
        if (trad)
            incompat = TRUE;
        if (! stdc_val)
            stdc_val = STDC;
        break;
    case KR :
    case OLD_PREP   :
        if (sflag || cplus_val || ansi || std_val != -1L)
            incompat = TRUE;
        if (dig_flag) {
            if (dig_flag != DIGRAPHS_INIT)
                incompat = TRUE;
            else
                dig_flag = 0;
        }
        break;
    }
    if (mcpp_mode == POST_STD && (lang_asm || compat_mode))
        incompat = TRUE;
    if (mcpp_mode != STD && trig_flag) {
        if (trig_flag != TRIGRAPHS_INIT)
            incompat = TRUE;
        else
            trig_flag = FALSE;
    }
    if (incompat) {
        mcpp_fputs( "Incompatible options are specified.\n", ERR);
        usage( '?');
    }

    standard = (mcpp_mode == STD || mcpp_mode == POST_STD);
    /* Modify magic characters in character type table. */
    if (! standard)
        char_type[ DEF_MAGIC] = 0;
    if (mcpp_mode != STD)
        char_type[ IN_SRC] = 0;
    if (mcpp_mode == POST_STD || mcpp_mode == KR)
        char_type[ TOK_SEP] = 0;    /* TOK_SEP equals to COM_SEP    */

    expand_init();
                /* Set function pointer to macro expansion routine  */
}

static void init_predefines(
    int     nflag,                  /* -N option                    */
    long    std_val                 /* Value of __STDC_VERSION__    */
)
/*
 * Set or unset predefined macros.
 * This routine should be called after init_gcc_macro().
 */
{
    char    tmp[ 16];

    if (std_val != -1L) {               /* Version is specified     */
        if (cplus_val)
            cplus_val = std_val;        /* Value of __cplusplus     */
        else
            stdc_ver = std_val;     /* Value of __STDC_VERSION__    */
    } else {
        if (! cplus_val)
            stdc_ver = stdc_val ? STDC_VERSION : 0L;
    }

    if (nflag) {
        un_predefine( TRUE);
#if COMPILER == GNUC
        undef_gcc_macro( TRUE);
#endif
    } else if (stdc_val || cplus_val) {
        un_predefine( FALSE);           /* Undefine "unix" or so    */
#if COMPILER == GNUC
        undef_gcc_macro( FALSE);
#endif
    }
    ACE_OS::sprintf( tmp, "%ldL", cplus_val ? cplus_val : stdc_ver);
    if (cplus_val) {
        look_and_install( "__cplusplus", DEF_NOARGS - 2, null, tmp);
    } else {
        if (stdc_ver)
            look_and_install( "__STDC_VERSION__", DEF_NOARGS - 2, null, tmp);
#ifdef  COMPILER_CPLUS
        if (! nflag)        /* Undefine pre-defined macro for C++   */
            undefine( COMPILER_CPLUS);
#endif
    }
    set_limit();
    stdc2 = cplus_val || stdc_ver >= 199901L;
    stdc3 = (cplus_val >= 199901L) || (stdc_ver >= 199901L);
            /* (cplus_val >= 199901L) makes C++ C99-compatible specs    */
    if (standard)
        init_std_defines();
    if (stdc3)
        set_pragma_op();
}

static void init_std_defines( void)
/*
 * For STD and POST_STD modes.
 * The magic pre-defines (Standard predefined macros) are initialized with
 * negative argument counts.  expand_macro() notices this and calls the
 * appropriate routine.  DEF_NOARGS is one greater than the first "magic"
 * definition.  'DEF_NOARGS - n' is reserved for pre-defined macros.
 * __STDC_VERSION__ and __cplusplus are defined by chk_opts() and set_cplus().
 */
{
    char    tmp[ 16];
    char    timestr[ 14];
    time_t  tvec;
    char *  tstring;

    look_and_install( "__LINE__", DEF_NOARGS - 3, null, "-1234567890");
    /* Room for 11 chars (10 for long and 1 for '-' in case of wrap round.  */
    look_and_install( "__FILE__", DEF_NOARGS - 4, null, null);
                                            /* Should be stuffed    */

    /* Define __DATE__, __TIME__ as present date and time.          */
    ACE_OS::time( &tvec);
    tstring = ACE_OS::ctime( &tvec);
    ACE_OS::sprintf( timestr, "\"%.3s %c%c %.4s\"",
        tstring + 4,
        *(tstring + 8) == '0' ? ' ' : *(tstring + 8),
        *(tstring + 9),
        tstring + 20);
    look_and_install( "__DATE__", DEF_NOARGS - 2, null, timestr);
    ACE_OS::sprintf( timestr, "\"%.8s\"", tstring + 11);
    look_and_install( "__TIME__", DEF_NOARGS - 2, null, timestr);

    if (! look_id( "__STDC_HOSTED__")) {
        /*
         * Some compilers, e.g. GCC older than 3.3, define this macro by
         * -D option.
         */
        ACE_OS::sprintf( tmp, "%d", STDC_HOSTED);
        look_and_install( "__STDC_HOSTED__", DEF_NOARGS - 1, null, tmp);
    }
#if COMPILER != GNUC        /* GCC do not undefine __STDC__ on C++  */
    if (cplus_val)
        return;
#endif
    /* Define __STDC__ as 1 or such for Standard conforming compiler.   */
    if (! look_id( "__STDC__")) {
        ACE_OS::sprintf( tmp, "%d", stdc_val);
        look_and_install( "__STDC__", DEF_NOARGS - 2, null, tmp);
    }
}

static void set_limit( void)
/*
 * Set the minimum translation limits specified by the Standards.
 */
{
    if (cplus_val) {            /* Specified by C++ 1998 Standard   */
        str_len_min = SLEN_CPLUS_MIN;
        id_len_min = IDLEN_CPLUS_MIN;
        n_mac_pars_min = NMACPARS_CPLUS_MIN;
        exp_nest_min = EXP_NEST_CPLUS_MIN;
        blk_nest_min = BLK_NEST_CPLUS_MIN;
        inc_nest_min = INCLUDE_NEST_CPLUS_MIN;
        n_macro_min = NMACRO_CPLUS_MIN;
        line_limit = LINE_CPLUS_LIMIT;
    } else if (stdc_ver >= 199901L) {       /* Specified by C 1999 Standard */
        str_len_min = SLEN99MIN;
        id_len_min = IDLEN99MIN;
        n_mac_pars_min = NMACPARS99MIN;
        exp_nest_min = EXP_NEST99MIN;
        blk_nest_min = BLK_NEST99MIN;
        inc_nest_min = INCLUDE_NEST99MIN;
        n_macro_min = NMACRO99MIN;
        line_limit = LINE99LIMIT;
    } else if (standard) {                  /* Specified by C 1990 Standard */
        str_len_min = SLEN90MIN;
        id_len_min = IDLEN90MIN;
        n_mac_pars_min = NMACPARS90MIN;
        exp_nest_min = EXP_NEST90MIN;
        blk_nest_min = BLK_NEST90MIN;
        inc_nest_min = INCLUDE_NEST90MIN;
        n_macro_min = NMACRO90MIN;
        line_limit = LINE90LIMIT;
    }
    /* Else pre-Standard mode   */
}

static void set_pragma_op( void)
/*
 *      #define _Pragma(a)  _Pragma ( a )
 * Define _Pragma() operator as a special macro so as to be searched
 * easily.  The unusual 'DEF_PRAGMA' is a marker of this psuedo
 * macro.
 */
{
    char *  name = "_Pragma";
    char    tmp[ 16];

    ACE_OS::sprintf( tmp, "%c%s ( %c%c )", DEF_MAGIC, name, MAC_PARM, 1);
                                                /* Replacement text */
    look_and_install( name, DEF_PRAGMA, "a", tmp);
}

void    at_start( void)
/*
 * Do the commands prior to processing main source file.
 */
{
    /*
     * Set multi-byte character encoding according to environment variables
     * LC_ALL, LC_CTYPE and LANG -- with preference in this order.
     */
    char *  env;

    if (mb_changed)
        return;                             /* -m option precedes   */
    if ((env = ACE_OS::getenv( "LC_ALL")) != 0)
        set_encoding( env, "LC_ALL", 0);
    else if ((env = ACE_OS::getenv( "LC_CTYPE")) != 0)
        set_encoding( env, "LC_CTYPE", 0);
    else if ((env = ACE_OS::getenv( "LANG")) != 0)
        set_encoding( env, "LANG", 0);

#if COMPILER == GNUC || COMPILER == MSC
#if COMPILER == GNUC
    /* -fworking-directory  */
    if (gcc_work_dir && (preinclude <= --preinc_end && *preinc_end != 0)) {
        open_include( *preinc_end, TRUE, FALSE);
        src_line++;
        put_info();                 /* Putout the current directory */
        src_line--;
    }
#endif
    /*
     * Do the -include (-Fl for MSC) options in the specified order.
     * Note: This functionality is implemented as nested #includes
     *   which results the same effect as sequential #includes.
     */
    while (preinclude <= --preinc_end && *preinc_end != 0)
        open_include( *preinc_end, TRUE, FALSE);
#endif
}

static char *   set_files(
    int     argc,
    char ** argv,
    char ** in_pp,
    char ** out_pp
)
/*
 * Set input and/or output files.
 */
{
    char *      cp;

    if (*in_pp == 0) {                           /* Input file   */
        cp = argv[ optind++];
#if SYS_FAMILY == SYS_WIN
        cp = bsl2sl( cp);
#endif
        *in_pp = cp;
    }
    if (optind < argc && argv[ optind][ 0] != '-' && *out_pp == 0) {
        cp = argv[ optind++];
#if SYS_FAMILY == SYS_WIN
        cp = bsl2sl( cp);
#endif
        *out_pp = cp;                               /* Output file  */
    }
    if (optind >= argc)
        return  0;           /* Exhausted command line arguments */
    if (argv[ optind][ 0] == '-')
        return  argv[ optind];                      /* More options */
    cfatal( "Excessive file argument \"%s\"", argv[ optind], 0L , 0);
    return  0;
}

static void set_env_dirs( void)
/*
 * Add to include path those specified by environment variables.
 */
{
    const char *    env;

    if (cplus_val) {
        if ((env = ACE_OS::getenv( ENV_CPLUS_INCLUDE_DIR)) != 0)
            parse_env( env);
    }
    if ((env = ACE_OS::getenv( ENV_C_INCLUDE_DIR)) != 0)
        parse_env( env);
}

static void parse_env(
    const char *    env
)
/*
 * Parse environmental variable and append the path to include-dir-list.
 */
{
    char *  save;
    char *  save_start;
    char *  p;
    int     sep;

    save = save_start = save_string( env);
    while (*save) {
        p = save;
        while (*p && *p != ENV_SEP)
            p++;
        if (p != save)  {                   /* Variable separator   */
            sep = *p;
            *p = EOS;
            set_a_dir( save);
            if (sep == EOS)
                break;
            save = ++p;
        }
        while (*save == ENV_SEP)
            ++save;
    }
    ACE_OS::free( save_start);
}

static void set_sys_dirs(
    int     set_cplus_dir       /* Set C++ include-directory too    */
)
/*
 * Set site-specific and system-specific directories to the include directory
 * list.
 */
{
    if (cplus_val && set_cplus_dir) {
#ifdef  CPLUS_INCLUDE_DIR1
        set_a_dir( CPLUS_INCLUDE_DIR1);
#endif
#ifdef  CPLUS_INCLUDE_DIR2
        set_a_dir( CPLUS_INCLUDE_DIR2);
#endif
#ifdef  CPLUS_INCLUDE_DIR3
        set_a_dir( CPLUS_INCLUDE_DIR3);
#endif
#ifdef  CPLUS_INCLUDE_DIR4
        set_a_dir( CPLUS_INCLUDE_DIR4);
#endif
    }

#if SYS_FAMILY == SYS_UNIX
    set_a_dir( "/usr/local/include");
#endif

#ifdef  C_INCLUDE_DIR1 
    set_a_dir( C_INCLUDE_DIR1);
#endif
#ifdef  C_INCLUDE_DIR2
    set_a_dir( C_INCLUDE_DIR2);
#endif

#if SYS_FAMILY == SYS_UNIX
#if SYSTEM == SYS_CYGWIN
    if (no_cygwin)                          /* -mno-cygwin          */
        set_a_dir( "/usr/include/mingw");
    else
        set_a_dir( "/usr/include");
#else
    set_a_dir( "/usr/include"); /* Should be placed after C_INCLUDE_DIR?    */
#endif
#endif
}

static void set_a_dir(
    const char *    dirname                 /* The path-name        */
)
/*
 * Append an include directory, checking array boundary.
 * This routine is called from the following routines (in this order).
 * 1. do_options() by -I option.
 * 2. do_options() by -isystem option (for GNUC).
 * 3. set_env_dirs() by environment variables.
 * 4. set_sys_dirs() by CPLUS_INCLUDE_DIR?, C_INCLUDE_DIR? and system-
 *    specifics (unless -I- or -nostdinc option is specified).
 * Note: a trailing PATH-DELIM is appended by norm_path().
 */
{
    char *  norm_name;
    const char **   ip;

    norm_name = norm_path( dirname, 0);
                            /* Normalize the pathname to compare    */
    for (ip = incdir; ip < incend; ip++) {
        if (str_eq( *ip, norm_name)) {
            ACE_OS::free( norm_name);
            return;
        }
    }
    if (& incdir[ NINCLUDE - 1] < incend)
        cfatal( "Too many include directories %s"           /* _F_  */
                , norm_name, 0L, 0);
    *incend++ = norm_name;
}

static char *   norm_path(
    const char *    dir,    /* Include directory (maybe a pointer to "")    */
    const char *    fname   /* Filename (possibly has directory part)       */
)
/*
 * Normalize the pathname removing redundant components such as
 * "foo/../", "./" and trailing "/.".
 * Append trailing "/" if 'fname' is 0.
 * Change relative path to absolute path.
 * Dereference a symbolic linked file (or directory) to a real directory/file.
 * Returns a malloc'ed buffer.
 * This routine is called from set_a_dir(), init_gcc_macro(), do_once() and
 * open_file().
 */
{
    char *  norm_name;
    char *  start;
    char *  cp1;
    char *  cp2;
    char *  abs_path;
    int     len;                            /* Should not be size_t */
    size_t  start_pos = 0;
    char    slbuf1[ FILENAMEMAX+1];
#if SYS_FAMILY == SYS_UNIX
    char    slbuf2[ FILENAMEMAX+1];
#endif
#if SYSTEM == SYS_CYGWIN || SYSTEM == SYS_MINGW
    static char *   root_dir;
    static size_t   root_dir_len;
#if SYSTEM == SYS_CYGWIN
    static char *   cygdrive = "/cygdrive/";
#else
    static char *   mingw_dir;
    static size_t   mingw_dir_len;
#endif
#endif

    ACE_OS::strcpy( slbuf1, dir);                   /* Include directory    */
    len = ACE_OS::strlen( slbuf1);
    if (len && slbuf1[ len - 1] != PATH_DELIM) {
        slbuf1[ len] = PATH_DELIM;          /* Append PATH_DELIM    */
        slbuf1[ len + 1] = EOS;
    }
#if SYS_FAMILY == SYS_UNIX
    /* Dereference symbolic linked directory or file, if any    */
    slbuf2[ 0] = EOS;
    if (*dir && ! fname) {      /* Registering include directory    */
        /* Symbolic link check of directories are required  */
        deref_syml( slbuf1, slbuf2, slbuf1);
    }
    if (fname) {
        len = ACE_OS::strlen( slbuf1);
        ACE_OS::strcat( slbuf1, fname);
        deref_syml( slbuf1, slbuf2, slbuf1 + len);
                                /* Symbolic link check of directory */
                if ((len = ACE_OS::readlink( slbuf1, slbuf2, FILENAMEMAX)) > 0) {
            /* Dereference symbolic linked file (not directory) */
            *(slbuf2 + len) = EOS;
            ACE_OS::strcpy( slbuf1, slbuf2);
        }
    }
    if (mcpp_debug & PATH) {
        if (slbuf2[ 0])
            mcpp_fprintf( DBG, "Dereferenced \"%s%s\" to \"%s\"\n"
                    , dir, fname ? fname : "", slbuf1);
    }
#else
    if (fname)
        ACE_OS::strcat( slbuf1, fname);
#endif
    len = ACE_OS::strlen( slbuf1);
    start = norm_name = xmalloc( len + 1);  /* Need a new buffer    */
    ACE_OS::strcpy( norm_name, slbuf1);
#if SYS_FAMILY == SYS_WIN
    bsl2sl( norm_name);
#endif
#if FNAME_FOLD
    conv_case( norm_name, norm_name + len, LOWER);
#endif
#if SPECIAL_PATH_DELIM                  /* ':' ?    */
    for (cp1 = norm_name; *cp1 != EOS; cp1++) {
        if (*cp1 == PATH_DELIM)
            *cp1 = '/';
    }
#endif
    cp1 = norm_name;

#if SYSTEM == SYS_CYGWIN
    /* Convert to "/cygdirve/x/dir" style of absolute path-list     */
    if (ACE_OS::memcmp( cp1, "/usr/bin", 8) == 0 || ACE_OS::memcmp( cp1, "/usr/lib", 8) == 0) {
        ACE_OS::memmove( cp1, cp1 + 4, len - 4 + 1);    /* Remove "/usr"    */
        len -= 4;
    }
    if (*cp1 == '/' && ACE_OS::memcmp( cp1, cygdrive, 10) != 0) {
        /* /dir, not /cygdrive/     */
        if (! root_dir_len) {           /* Should be initialized    */
            /* Convert "X:\DIR-list" to "/cygdrive/x/dir-list"      */
            root_dir = xmalloc( ACE_OS::strlen( CYGWIN_ROOT_DIRECTORY) + 1);
            ACE_OS::strcpy( root_dir, CYGWIN_ROOT_DIRECTORY);
            conv_case( root_dir, root_dir + ACE_OS::strlen( root_dir), LOWER);
            *(root_dir + 1) = *root_dir;        /* "x:/" to " x/"   */
            cp1 = xmalloc( ACE_OS::strlen( cygdrive) + ACE_OS::strlen( root_dir));
            ACE_OS::strcpy( cp1, cygdrive);
            ACE_OS::strcat( cp1, root_dir + 1);
            ACE_OS::free( root_dir);
            root_dir = cp1;
            root_dir_len = ACE_OS::strlen( root_dir);
        }
        cp1 = xmalloc( root_dir_len + len + 1);
        ACE_OS::strcpy( cp1, root_dir);
        ACE_OS::strcat( cp1, norm_name);        /* Convert to absolute path */
        ACE_OS::free( norm_name);
        norm_name = start = cp1;
        len += root_dir_len;
    }
#endif

#if SYSTEM == SYS_MINGW
    /* Handle the mess of MinGW's path-list */
    /* Convert to "x:/dir" style of absolute path-list  */
    if (*cp1 == PATH_DELIM && ACE_OS::isalpha( *(cp1 + 1))
            && *(cp1 + 2) == PATH_DELIM) {          /* /c/, /d/, etc*/
        *cp1 = *(cp1 + 1);
        *(cp1 + 1) = ':';               /* Convert to c:/, d:/, etc */
    } else if (ACE_OS::memcmp( cp1, "/mingw", 6) == 0) {
        if (! mingw_dir_len) {          /* Should be initialized    */
            mingw_dir_len = ACE_OS::strlen( MINGW_DIRECTORY);
            mingw_dir = xmalloc( mingw_dir_len + 1);
            ACE_OS::strcpy( mingw_dir, MINGW_DIRECTORY);
            conv_case( mingw_dir, mingw_dir + mingw_dir_len, LOWER);
        }
        cp1 = xmalloc( mingw_dir_len + len);
        ACE_OS::strcpy( cp1, mingw_dir);
        ACE_OS::strcat( cp1, norm_name + 6);    /* Convert to absolute path */
        ACE_OS::free( norm_name);
        norm_name = start = cp1;
        len += mingw_dir_len;
    } else if (ACE_OS::memcmp( cp1, "/usr", 4) == 0) {
        ACE_OS::memmove( cp1, cp1 + 4, len - 4 + 1);    /* Remove "/usr"    */
        len -= 4;
    }
    if (*cp1 == '/') {                  /* /dir or /                */
        if (! root_dir_len) {           /* Should be initialized    */
            root_dir_len = ACE_OS::strlen( MSYS_ROOT_DIRECTORY);
            root_dir = xmalloc( root_dir_len + 1);
            ACE_OS::strcpy( root_dir, MSYS_ROOT_DIRECTORY);
            conv_case( root_dir, root_dir + root_dir_len, LOWER);
        }
        cp1 = xmalloc( root_dir_len + len + 1);
        ACE_OS::strcpy( cp1, root_dir);
        ACE_OS::strcat( cp1, norm_name);        /* Convert to absolute path */
        ACE_OS::free( norm_name);
        norm_name = start = cp1;
        len += root_dir_len;
    }
#endif

#if SYS_FAMILY == SYS_WIN
    if (*(cp1 + 1) == ':')
        start = cp1 += 2;               /* Skip the drive letter    */
    start_pos = 2;
#endif
    if (len == 1 && *norm_name == '/')              /* Only "/"     */
        return  norm_name;

    if (ACE_OS::strncmp( cp1, "./", 2) == 0)    /* Remove beginning "./"    */
        ACE_OS::memmove( cp1, cp1 + 2, ACE_OS::strlen( cp1 + 2) + 1);       /* +1 for EOS   */
    if (*start != '/') {    /* Relative path to current directory   */
        /* Make absolute path   */
        abs_path = xmalloc( len + ACE_OS::strlen( cur_work_dir) + 1);
        cp1 = stpcpy( abs_path, cur_work_dir);
        ACE_OS::strcpy( cp1, start);
        ACE_OS::free( norm_name);
        norm_name = abs_path;
        start = cp1 = norm_name + start_pos;
    }

    while ((cp1 = ACE_OS::strstr( cp1, "/./")) != 0)
        ACE_OS::memmove( cp1, cp1 + 2, ACE_OS::strlen( cp1 + 2) + 1);
                                        /* Remove "/." of "/./"     */
    cp1 = start;
    /* Remove redundant "foo/../"   */
    while ((cp1 = ACE_OS::strstr( cp1, "/../")) != 0) {
        *cp1 = EOS;
        if ((cp2 = ACE_OS::strrchr( start, '/')) != 0) {
            if (*(cp1 - 1) != '.') {
                ACE_OS::memmove( cp2 + 1, cp1 + 4, ACE_OS::strlen( cp1 + 4) + 1);
                                        /* Remove "foo/../"         */
                cp1 = cp2;
            } else {                                /* Impossible   */
                break;
            }
        } else {                                    /* Impossible   */ 
            break;
        }
    }

#if SPECIAL_PATH_DELIM
    for (cp1 = start; *cp1 != EOS; cp1++) {
        if (*cp1 == '/')
            *cp1 = PATH_DELIM;
    }
#endif
    if (mcpp_debug & PATH) {
        char    debug_buf[ FILENAMEMAX+1];
        ACE_OS::strcpy( debug_buf, dir);
        ACE_OS::strcat( debug_buf, fname ? fname : "");
#if SYS_FAMILY == SYS_WIN
        bsl2sl( debug_buf);
#endif
#if FNAME_FOLD
        conv_case( debug_buf, debug_buf + ACE_OS::strlen( debug_buf), LOWER);
#endif
        if (! str_eq( debug_buf, norm_name))
            mcpp_fprintf( DBG, "Normalized the path \"%s\" to \"%s\"\n"
                    , debug_buf, norm_name);
    }

    return  norm_name;
}

#if SYS_FAMILY == SYS_UNIX

static void     deref_syml(
    char *      slbuf1,
    char *      slbuf2,
    char *      chk_start
)
/* Dereference symbolic linked directory    */
{
    char *      cp2;
    int         len;            /* Should be int, not size_t    */

    while ((chk_start = ACE_OS::strchr( chk_start, PATH_DELIM)) != 0) {
        *chk_start = EOS;
                if ((len = ACE_OS::readlink( slbuf1, slbuf2, FILENAMEMAX)) > 0) {
            /* Dereference symbolic linked directory    */
            cp2 = ACE_OS::strrchr( slbuf1, PATH_DELIM); /* Previous delimiter       */
            *chk_start = PATH_DELIM;
            ACE_OS::strcpy( slbuf2 + len, chk_start);
            if (slbuf2[ 0] == PATH_DELIM) {     /* Absolute path    */
                ACE_OS::strcpy( slbuf1, slbuf2);
                chk_start = slbuf1 + len + 1;
            } else {
                if (cp2)
                    chk_start = cp2 + 1;
                else
                    chk_start = slbuf1;
                ACE_OS::strcpy( chk_start, slbuf2);     /* Rewrite the path */
                chk_start += len;
            }
        } else {
            *chk_start++ = PATH_DELIM;
        }
    }
}
#endif

void    conv_case(
    char *  name,                       /* (diretory) Name          */
    char *  lim,                        /* End of (directory) name  */
    int     upper                       /* TRUE if to upper         */
)
/* Convert a string to upper-case letters or lower-case letters in-place    */
{
    int     c;
    char *  sp;

    for (sp = name; sp < lim; sp++) {
        c = *sp & UCHARMAX;
#if MBCHAR
        if ((char_type[ c] & mbstart)) {
            char    tmp[ FILENAMEMAX+1];
            char *  tp = tmp;
            *tp++ = *sp++;
            mb_read( c, &sp, &tp);
        } else
#endif
        {
            if (upper)
                *sp = ACE_OS::ace_toupper( c);
            else
                *sp = ACE_OS::ace_tolower( c);
        }
    }
}

void    put_info( void)
/* Putout compiler-specific information */
{
#if COMPILER == GNUC
    /*
     * Putout the current directory as a #line line as:
     * '# 1 "/abs-path/cur_dir//"'.
     */
    if (! put_info_done && ! no_output && gcc_work_dir)
        mcpp_fprintf( OUT, "%s%ld \"%s%c\"\n"
                , std_line_prefix ? "#line " : LINE_PREFIX
                , src_line, cur_work_dir, '/');
    put_info_done = TRUE;
#endif
}

#if COMPILER == GNUC

static DEFBUF * gcc_predef_std[ 128];
static DEFBUF * gcc_predef_old[ 16];

static void init_gcc_macro(
    int     gcc_maj_ver,        /* __GNUC__         */
    int     gcc_min_ver         /* __GNUC_MINOR__   */
)
/*
 * Predefine GCC macros.
 */
{
    char        fname[ 256];
    char *      include_dir;    /* The version-specific include directory   */
    char        lbuf[ BUFSIZ];
    FILE *      fp;
    DEFBUF **   predef;
    DEFBUF *    defp;
    const char *    cp;
    char *      tp;
    int         i;

#if     SYSTEM == SYS_CYGWIN
    char        mingw_dir[ FILENAMEMAX];

    if (no_cygwin) {
        include_dir = mingw_dir;
        ACE_OS::sprintf( include_dir, "%s/%s", C_INCLUDE_DIR1, "mingw");
    } else {
        include_dir = C_INCLUDE_DIR1;
    }
#elif   SYSTEM == SYS_MINGW
    include_dir = C_INCLUDE_DIR2;
            /* MinGW's 2nd directory is the version-specific one    */
#else
#ifdef  C_INCLUDE_DIR1
    include_dir = C_INCLUDE_DIR1;
#else
    include_dir = "/usr/local/include";
#endif
#endif
    include_dir = norm_path( include_dir, 0);

    for (i = 0; i <= 1; i++) {
        /* The predefined macro file    */
        cp = i ? "std" : "old";
        ACE_OS::sprintf( fname, "%smcpp_g%s%d%d_predef_%s.h"
                , include_dir, cplus_val ? "xx" : "cc"
                , gcc_maj_ver, gcc_min_ver, cp);
            /* Note that norm_path() append a PATH_DELIM.   */
        if ((fp = ACE_OS::fopen( fname, "r")) == 0) {
            mcpp_fprintf( ERR, "Predefined macro file '%s' is not found\n"
                    , fname);
            continue;
        }
        predef = i ? gcc_predef_std : gcc_predef_old;
        while (ACE_OS::fgets( lbuf, BUFSIZ, fp) != 0) {
            unget_string( lbuf, "gcc_predefine");
            if (skip_ws() == '#'
                && scan_token( skip_ws(), (tp = work_buf, &tp), work_end)
                        == NAM
                    && str_eq( work_buf, "define")) {
                defp = do_define( TRUE);    /* Ignore re-definition */ 
                if (defp->nargs >= DEF_NOARGS - 1)
                    *predef++ = defp;   /* Register only non-Standard macros*/
            }
            skip_nl();
        }
        *predef = 0;                     /* Terminate the array  */
    }

    if (look_id( "__OPTIMIZE__"))       /* -O option is specified   */
        undefine( "__NO_INLINE__");
    if (no_exceptions)                  /* -fno-exceptions option   */
        undefine( "__EXCEPTIONS");
}

static void undef_gcc_macro(
    int     clearall
)
/*
 * Undefine GCC predefined macros.
 */
{
    DEFBUF **   predef;

    predef = gcc_predef_old;
    while (*predef)
        undefine( (*predef++)->name);
    gcc_predef_old[ 0] = 0;
    if (clearall) {
        predef = gcc_predef_std;
        while (*predef)
            undefine( (*predef++)->name);
        gcc_predef_std[ 0] = 0;
    }
}

static void chk_env( void)
/*
 * Check the environment variables to specify output of dependency lines.
 */
{
    char *  env;
    char *  cp;

    /* Output of dependency lines   */
    if ((env = ACE_OS::getenv( "DEPENDENCIES_OUTPUT")) == 0) {
        if ((env = ACE_OS::getenv( "SUNPRO_DEPENDENCIES")) == 0)
            return;
        else
            mkdep |= MD_SYSHEADER;
    }
    mkdep |= MD_MKDEP;
    if ((cp = ACE_OS::ACE_OS::strchr( env, ' ')) != 0) {
        *cp++ = EOS;
        while (*cp == ' ')
            cp++;
    }
    if (! mkdep_fp)                 /* Command line option precedes */
        mkdep_fp = ACE_OS::ACE_OS::fopen( env, "a");
    if (! mkdep_target)
        mkdep_target = cp;
}

#elif   COMPILER == MSC

static void init_msc_macro(
    int     wchar_t_modified
)
{
    DEFBUF *    defp;
    int         i;

    defp = look_id( "_MSC_VER");
    i = ACE_OS::atoi( defp->repl);
    if (i >= 1400) {                        /* _MSC_VER >= 1400     */
        look_and_install( "_MT", DEF_NOARGS - 1, null, "1");
        if (cplus_val && ! wchar_t_modified) {
            /* -Zc:wchar_t- was not specified   */
            look_and_install( "_NATIVE_WCHAR_T_DEFINED", DEF_NOARGS - 1, null
                    , "1");
            look_and_install( "_WCHAR_T_DEFINED", DEF_NOARGS - 1, null, "1");
        }
    }
}

#endif

void    put_depend(
    const char *    filename
)
/*
 * Append a header name to the source file dependency line.
 */
{
#define MAX_OUT_LEN     76      /* Maximum length of output line    */
#define MKDEP_MAXLEN    (MKDEP_MAX * 0x200)
    static char     output[ MKDEP_MAXLEN];          /* File names   */
    static char *   pos[ MKDEP_MAX];      /* Pointers to filenames  */
    static int      pos_num;              /* Index of pos[]         */
    static char *   out_p;                /* Pointer to output[]    */
    static FILE *   fp;         /* Path to output dependency line   */
    static size_t   llen;       /* Length of current physical output line   */
    char **         pos_pp;               /* Pointer to pos         */
    size_t          fnamlen;              /* Length of filename     */

    if (fp == 0) {   /* Main source file.  Have to initialize.   */
        out_p = md_init( filename, output);
        fp = mkdep_fp;
        llen = ACE_OS::strlen( output);
        pos_num = 0;            /* Initialize for MCPP_LIB build    */
    } else if (filename == 0) {              /* End of input     */
        out_p = stpcpy( out_p, "\n\n");
        if (mkdep & MD_PHONY) {
            /* Output the phony target line for each recorded header files. */
            char *  cp;
            int     c;

            if (ACE_OS::strlen( output) * 2 + (pos_num * 2) >= MKDEP_MAXLEN) {
                cerror( "Too long dependency line\n"        /* _E_  */
                        , 0, 0L, 0);
                if (fp == fp_out)
                    mcpp_fputs( output, OUT );
                else
                    ACE_OS::fputs( output, fp);
                return;
            }
            pos_num--;
            for (pos_pp = &pos[ 0]; pos_pp <= &pos[ pos_num]; pos_pp++) {
                if (pos_pp == &pos[ pos_num]) {
                    for (cp = *pos_pp; *cp != '\n'; cp++)
                        ;
                    c = '\n';
                } else {
                    cp = *(pos_pp + 1) - 1;
                    while( *cp == ' ' || *cp == '\\' || *cp == '\n')
                        cp--;
                    c = *(++cp);
                }
                *cp = EOS;
                out_p = stpcpy( out_p, *pos_pp);
                out_p = stpcpy( out_p, ":\n\n");
                *cp = c;
            }
        }
        if (fp == fp_out)   /* To the same path with normal preprocessing   */
            mcpp_fputs( output, OUT);
        else            /* To the file specified by -MF, -MD, -MMD options  */
            ACE_OS::fputs( output, fp);
        fp = 0;      /* Clear for the next call in MCPP_LIB build        */
        return;
    }

    fnamlen = ACE_OS::strlen( filename);
    /* Check the recorded filename  */
    for (pos_pp = pos; pos_pp < &pos[ pos_num]; pos_pp++) {
        if (ACE_OS::memcmp( *pos_pp, filename, fnamlen) == 0)
            return;                 /* Already recorded filename    */
    }
    /* Any new header.  Append its name to output.  */
    if (llen + fnamlen > MAX_OUT_LEN) {         /* Line is long     */
        out_p = stpcpy( out_p, " \\\n ");       /* Fold it          */
        llen = 1;
    }
    llen += fnamlen + 1;
    if (pos_num >= MKDEP_MAX
            || out_p + fnamlen + 1 >= output + MKDEP_MAXLEN)
        cfatal( "Too long dependency line: %s", output, 0L, 0);
    *out_p++ = ' ';
    pos[ pos_num++] = out_p;
    out_p = stpcpy( out_p, filename);
}

static char *   md_init(
    const char *    filename,   /* The source file name             */ 
    char *  output              /* Output to dependency file        */
)
/*
 * Initialize output file and target.
 */
{
    char    prefix[ FILENAMEMAX];
    char *  cp = 0;
    size_t  len;
    char *  out_p;
    const char *    target = filename;
    const char *    cp0;

    if (! mkdep_target || ! mkdep_fp) {         /* Make target name */
#ifdef  PATH_DELIM
        if ((cp0 = ACE_OS::strrchr( target, PATH_DELIM)) != 0)
            target = cp0 + 1;
#endif
        if ((cp0 = ACE_OS::strrchr( target, '.')) == 0)
            len = ACE_OS::strlen( target);
        else
            len = (size_t) (cp0 - target);
        ACE_OS::memcpy( prefix, target, len);
        cp = prefix + len;
        *cp++ = '.';
    }

    if (! mkdep_fp) {   /* Unless already opened by -MF, -MD, -MMD options  */
        if (mkdep & MD_FILE) {
            ACE_OS::strcpy( cp, "d");
            mkdep_fp = ACE_OS::fopen( prefix, "w");
        } else {
            mkdep_fp = fp_out;  /* Output dependency line to normal output  */
            no_output++;                /* Without normal output    */
        }
    }

    if (mkdep_target) {         /* -MT or -MQ option is specified   */
        if (mkdep & MD_QUOTE) {         /* 'Quote' $, \t and space  */
            out_p = md_quote( output);
        } else {
            out_p = stpcpy( output, mkdep_target);
        }
    } else {
        ACE_OS::strcpy( cp, OBJEXT);
        out_p = stpcpy( output, prefix);
    }

    *out_p++ = ':';
    return  out_p;
}

static char *   md_quote(
    char *  output
)
/*
 * 'Quote' $, tab and space.
 * This function was wretten referring to GCC V.3.2 source.
 */
{
    char *  p;
    char *  q;

    for (p = mkdep_target; *p; p++, output++) {
        switch (*p) {
        case ' ':
        case '\t':
            /* GNU-make treats backslash-space sequence peculiarly  */
            for (q = p - 1; mkdep_target <= q && *q == '\\'; q--)
                *output++ = '\\';
            *output++ = '\\';
            break;
        case '$':
            *output++ = '$';
            break;
        default:
            break;
        }
        *output = *p;
    }
    *output = EOS;
    return  output;
}

static const char *     toolong_fname =
        "Too long header name \"%s%.0ld%s\"";               /* _F_  */
static const char *     excess_token =
        "Excessive token sequence \"%s\"";          /* _E_, _W1_    */

int     do_include(
    int     next        /* TRUE if the directive is #include_next   */
)
/*
 * Process the #include line.
 * There are three variations:
 *      #include "file"         search somewhere relative to the
 *                              current (or source) directory, if not
 *                              found, treat as #include <file>.
 *      #include <file>         Search in an implementation-dependent
 *                              list of places.
 *      #include macro-call     Expand the macro call, it must be one of
 *                              "file" or <file>, process as such.
 * On success : return TRUE;
 * On failure of syntax : return FALSE;
 * On failure of file opening : return FALSE.
 * do_include() always absorbs the line (including the <newline>).
 */
{
    const char * const  no_name = "No header name";         /* _E_  */
    char    header[ FILENAMEMAX + 16];
    int     token_type;
    char *  fname;
    int     delim;                          /* " or <, >            */

    if ((delim = skip_ws()) == '\n') {      /* No argument          */
        cerror( no_name, 0, 0L, 0);
        return  FALSE;
    }
    fname = infile->bptr - 1;       /* Current token for diagnosis  */

    if (standard && char_type[ delim] & LET) {  /* Maybe a macro    */
        int     c;
        char    *hp;

        hp = header;
        c = delim;
        while (get_unexpandable( c, FALSE) != NO_TOKEN) {
                                /* Expand any macros in the line    */
            if (header + FILENAMEMAX < hp + (int) (workp - work_buf))
                cfatal( toolong_fname, header, 0L, work_buf);
            hp = stpcpy( hp, work_buf);
            while ((c = get_ch()) == ' ')
                *hp++ = ' ';
        }
        *hp = EOS;                          /* Ensure to terminate  */
        if (macro_line == MACRO_ERROR)      /* Unterminated macro   */
            return  FALSE;                  /*   already diagnosed. */
        unget_string( header, 0);        /* To re-read           */
        delim = skip_ws();
        if (delim == '\n') {
            cerror( no_name, 0, 0L, 0);       /* Expanded to  */
            return  FALSE;                          /*   0 token.   */
        }
    }

    token_type = scan_token( delim, (workp = work_buf, &workp)
            , work_buf + FILENAMEMAX);
    if (token_type == STR)                  /* String literal form  */
        goto  found_name;
    else if (token_type == OPE && openum == OP_LT)          /* '<'  */
        workp = scan_quote( delim, work_buf, work_buf + FILENAMEMAX, TRUE);
                                        /* Re-construct or diagnose */
    else                                    /* Any other token in-  */
        goto  not_header;                   /*   cluding <=, <<, <% */

    if (workp == 0)                      /* Missing closing '>'  */
        goto  syntax_error;

found_name:
    *--workp = EOS;                         /* Remove the closing and   */
    fname = save_string( &work_buf[ 1]);    /*  the starting delimiter. */

    if (skip_ws() != '\n') {
        if (standard) {
            cerror( excess_token, infile->bptr-1, 0L, 0);
            skip_nl();
            goto  error;
        } else if (mcpp_mode == OLD_PREP) {
            skip_nl();
        } else {
            if (warn_level & 1)
                cwarn( excess_token, infile->bptr-1, 0L, 0);
            skip_nl();
        }
    }

    if (open_include( fname, (delim == '"'), next)) {
        goto opened;
    }

    cerror( "Can't open include file \"%s\"", fname, 0L, 0);     /* _E_  */
    goto error;

not_header:
    cerror( "Not a header name \"%s\"", fname, 0L, 0);   /* _E_  */
syntax_error:
    skip_nl();
    return  FALSE;
error:
    ACE_OS::free( fname);
    return  FALSE;
opened:
    ACE_OS::free( fname);
    errno = 0;      /* Clear errno possibly set by path searching   */
    return  TRUE;
}

static int  open_include(
    char *  filename,               /* File name to include         */
    int     searchlocal,            /* TRUE if #include "file"      */
    int     next                    /* TRUE if #include_next        */
)
/*
 * Open an include file.  This routine is only called from do_include()
 * above, but was written as a separate subroutine for portability.
 * It searches the list of directories via search_dir() and opens the file
 * via open_file(), linking it into the list of active files.
 * Returns TRUE if the file was opened, FALSE if open_file() fails.
 */
{
    int     full_path;              /* Filename is full-path-list   */
    int     has_dir = FALSE;        /* Includer has directory part  */
    char    dir[ FILENAMEMAX] = { EOS, };   /* Directory part of includer   */

#if SYS_FAMILY == SYS_WIN
    bsl2sl( filename);
#endif
#if FNAME_FOLD  /* If O.S. folds upper and lower cases of file-name */
    /* Convert filename to lower-case-letters   */
    conv_case( filename, filename + ACE_OS::strlen( filename), LOWER);
#endif

#if SYS_FAMILY == SYS_UNIX
    if (filename[0] == PATH_DELIM)
#elif   SYS_FAMILY == SYS_WIN
    if (filename[1] == ':' && filename[2] == PATH_DELIM)
#elif   SYSTEM == SYS_MAC         /* I don't know.  Write by yourself */
    if (filename[0] != PATH_DELIM && ACE_OS::strchr( filename, PATH_DELIM))
#elif   1
/* For other systems you should write code here.    */
    if (filename[0] == PATH_DELIM)
#endif
        full_path = TRUE;
    else
        full_path = FALSE;

    if (!full_path && searchlocal && (search_rule & SOURCE))
        has_dir = has_directory( infile->real_fname, dir)
                || (**(infile->dirp) != EOS);

#if COMPILER == GNUC
    if (!full_path) {
        if ((!searchlocal && incdir < sys_dirp) /* -I- option is specified  */
                || next)                        /* #include_next    */
        goto  search_dirs;
    }
#endif

    if (mcpp_debug & PATH)
        mcpp_fprintf( DBG, "filename:%s\n", filename);
    if ((searchlocal && ((search_rule & CURRENT) || !has_dir)) || full_path) {
        /*
         * Look in local directory first.
         * Try to open filename relative to the "current directory".
         */
        if (open_file( &null, filename, searchlocal && !full_path))
            return  TRUE;
    }
    if (full_path)
        return  FALSE;

    if (searchlocal && (search_rule & SOURCE) && has_dir) {
        /*
         * Look in local directory of source file.
         * Try to open filename relative to the "source directory".
         */
        ACE_OS::strcat( dir, filename);
        if (open_file( infile->dirp, dir, TRUE))
            return  TRUE;
    }

    /* Search the system include directories    */
#if COMPILER == GNUC
search_dirs:
#endif
    if (search_dir( filename, searchlocal, next))
        return  TRUE;

    return  FALSE;
}

static int  has_directory(
    const char *    source,         /* Filename to examine          */
    char *  directory               /* Put directory stuff here     */
)
/*
 * If a directory is found in the 'source' filename string (i.e. "includer"),
 * the directory part of the string is copied to 'directory' and 
 * has_directory() returns TRUE.
 * Else, nothing is copied and it returns FALSE.
 */
{
    const char *    sp;
    size_t  len;

    if ((sp = ACE_OS::strrchr( source, PATH_DELIM)) == 0) {
        return  FALSE;
    } else {
        len = (size_t)(sp - source) + 1;
        ACE_OS::memcpy( directory, source, len);
        directory[ len] = EOS;
        return  TRUE;
    }
}

static int  search_dir(
    char *  filename,               /* File name to include         */
    int     ,            /* #include "header.h" for GNUC */
    int                       /* TRUE if #include_next        */
)
/*
 * Look in any directories specified by -I command line arguments,
 * specified by environment variable, then in the builtin search list.
 */
{
    const char **   incptr;                 /* -> inlcude directory */

#if COMPILER == GNUC
    if (next && **inc_dirp != EOS) {
        incptr = inc_dirp + 1;
        /* In case of include_next search after the includer's directory    */
    } else {
    /* If (next && **inc_dirp == EOS), it should be #include_next "header.h"*/
        if (searchlocal || next)
            /* #include_next does not distinguish "header.h" and <header.h> */
            incptr = incdir;
        else
            incptr = sys_dirp;
    }
#else
    incptr = incdir;
#endif

    for ( ; incptr < incend; incptr++) {
        if (ACE_OS::strlen( *incptr) + ACE_OS::strlen( filename) >= FILENAMEMAX) {
            char *  cp;
            cp = stpcpy( work_buf, *incptr);
            ACE_OS::strcpy( cp, filename);
            cfatal( toolong_fname, work_buf, 0L, "");       /* _F_  */
        }
        if (open_file( incptr, filename, FALSE))
            /* Now infile has been renewed  */
            return  TRUE;
    }

    return  FALSE;
}

static int  open_file(
    const char **   dirp,           /* Pointer to directory */
    const char *    filename,       /* The filename         */
    int         local               /* #include "file"      */
)
/*
 * Open a file, add it to the linked list of open files, close the includer
 * if nessesary and truncate the includer's buffer.
 * This is called from open_include() and at_start().
 */
{
#if HOST_COMPILER == BORLANDC
    /* Borland's ACE_OS::fopen() fails to set errno to EMFILE.  */
    static int  max_open = ACE_OS::FOPEN_MAX - 5;
#else
    static int  max_open;
#endif
    int         len;
    FILEINFO *  file = infile;
    FILE *      fp;
    char *      cp;
    char        fullname[ FILENAMEMAX + 1];

    if (mcpp_debug & PATH)
        mcpp_fprintf( DBG, "Searching %s\n", **dirp == EOS ? "." : *dirp);
    if (standard && included( *dirp, filename)) /* Once included    */
        return  TRUE;
    cp = stpcpy( fullname, *dirp);
    ACE_OS::strcat( cp, filename);

    if ((max_open != 0 && max_open <= include_nest)
                            /* Exceed the known limit of open files */
            || ((fp = ACE_OS::fopen( fullname, "r")) == 0 && errno == EMFILE)) {
                            /* Reached the limit for the first time */
        /*
         * Table of open files is full.
         * Remember the file position and close the includer.
         * The state will be restored by get_line() on end of the included.
         */
        file->pos = ACE_OS::ftell( file->fp);
        ACE_OS::fclose( file->fp);
        /* In case of failure, re-open the includer */
        if ((fp = ACE_OS::fopen( fullname, "r")) == 0) {
            file->fp = ACE_OS::fopen( cur_fullname, "r");
            ACE_OS::fseek( file->fp, file->pos, SEEK_SET);
            return  FALSE;
        }
        if (max_open == 0)      /* Remember the limit of the system */
            max_open = include_nest;
    } else if (fp == 0) {    /* No file, illegal path name or so */
        return  FALSE;
    }
    /* Truncate buffer of the includer to save memory   */
    len = (int) (file->bptr - file->buffer);
    if (len) {
        file->buffer = xrealloc( file->buffer, len + 1);
        file->bptr = file->buffer + len;
    }

    if (mkdep && ((mkdep & MD_SYSHEADER) || local))
        put_depend( fullname);          /* Output dependency line   */

    add_file( fp, filename);    /* Add file-info to the linked list */
    /*
     * Remember the directory for #include_next.
     * Note: inc_dirp is restored to the parent includer's directory
     *   by get_ch() when the current includer is finished.
     */
    infile->dirp = inc_dirp = dirp;
    ACE_OS::strcpy( cur_fullname, fullname);

    if (zflag) {
        no_output++;        /* Don't output the included file       */
    } else {
        src_line = 1;                       /* Working on line 1 now    */
        sharp();            /* Print out the included file name     */
    }
    src_line = 0;                           /* To read the first line   */
    return  TRUE;
}

void    add_file(
    FILE *      fp,                         /* Open file pointer    */
    const char *    filename                /* Name of the file     */
)
/*
 * Initialize tables for this open file.  This is called from open_file()
 * (for #include files), and from the entry to MCPP to open the main input
 * file.  It calls a common routine get_file() to build the FILEINFO
 * structure which is used to read characters.
 */
{
    FILEINFO *      file;
    const char *    too_many_include_nest =
            "More than %.0s%ld nesting of #include";    /* _F_ _W4_ */

    filename = set_fname( filename);    /* Search or append to fnamelist[]  */
    file = get_file( filename, (size_t) NBUFF); /* file == infile   */
    file->fp = fp;                      /* Better remember FILE *   */
    cur_fname = filename;

    if (include_nest >= INCLUDE_NEST)   /* Probably recursive #include      */
        cfatal( too_many_include_nest, 0, (long) INCLUDE_NEST, 0);
    if (standard && (warn_level & 4) && include_nest == inc_nest_min + 1)
        cwarn( too_many_include_nest, 0, (long) inc_nest_min, 0);
    include_nest++;
}

static const char *     set_fname(
    const char *    filename
)
/*
 * Register the source filename to fnamelist[].
 * Search fnamelist[] for filename or append filename to fnamelist[].
 * Returns the pointer.
 */
{
    const char **     fnamep;

    /* Register the filename in fnamelist[] */
    fnamep = fnamelist;
    while (fnamep < fname_end) {
        if (str_eq( *fnamep, filename)) /* Already registered       */
            break;
        fnamep++;
    }
    if (fnamep < fname_end) {
        filename = *fnamep;
    } else {
        if (fname_end - fnamelist >= FNAMELIST)
            cfatal( "Too many include files", 0, 0L, 0);      /* _F_  */
        *fname_end = xmalloc( ACE_OS::strlen( filename) + 1);
        filename = ACE_OS::strcpy( *(char **)fname_end++, filename);
                                /* Global pointer for get_file()    */
    }
    return  filename;
}

void cur_file( void)
/*
 * Output current source file name.
 */
{
    FILEINFO *      file = infile;
    const char *    name;
    char *  cp;

    while (file->fp == 0)
        file = file->parent;
    cp = stpcpy( work_buf, *(file->dirp));
    ACE_OS::strcpy( cp, file->filename);
    name = work_buf;
    if (sharp_filename == 0 || ! str_eq( name, sharp_filename)) {
        if (sharp_filename != 0)
            ACE_OS::free( sharp_filename);
        sharp_filename = save_string( name);
    }
    if (! no_output)
        mcpp_fprintf( OUT, " \"%s\"", name);
#if COMPILER == GNUC
    if (sys_dirp <= file->dirp && file->dirp <= incend)
        mcpp_fputs( " 3", OUT);
#endif
}

#if SYS_FAMILY == SYS_WIN

static char *   bsl2sl(
    char * filename
)
/*
 * Convert '\\' in the path-list to '/'.
 */
{
    static int  diagnosed = FALSE;
    char *  cp;

    cp = filename;

    while (*cp) {
        if (bsl_in_mbchar) {
            int     c;
            c = *cp & UCHARMAX;
            if (char_type[ c] & mbstart) {  /* First byte of MBCHAR */
                char    tmp[ FILENAMEMAX];
                char *  tp = tmp;
                *tp++ = *cp++;
                mb_read( c, &cp, &tp);
                            /* Read over the multi-byte characters  */
                continue;
            }
        }
        if (*cp == '\\') {
            *cp++ = PATH_DELIM;
            if (!diagnosed && (warn_level & 2) && (warn_level != -1)) {
                            /* Backslash in source program          */
                cwarn(
        "Converted \\ to %s", "/", 0L, 0);               /* _W2_ */
                    diagnosed = TRUE;       /* Diagnose only once   */
            }
        } else {
            cp++;
        }
    }

    return  filename;
}

#endif  /* SYS_FAMILY == SYS_WIN    */

static const char * const   unknown_arg =
        "Unknown argument \"%s\"";      /*_W1_*/
static const char * const   not_ident =
        "Not an identifier \"%s\"";     /*_W1_*/

static int  is_junk( void)
/*
 * Check the trailing junk in a directive line.
 * This routine is never called in OLD_PREP mode.
 */
{
    int     c;

    c = skip_ws();
    unget_ch();
    if (c != '\n') {                        /* Trailing junk        */
        if (warn_level & 1)
            cwarn( unknown_arg, infile->bptr, 0L, 0);
        return TRUE;
    } else {
        return FALSE;
    }
}

#define PUSH    1
#define POP    -1

#define __SETLOCALE     1       /* #pragma __setlocale( "encoding") */
#define SETLOCALE       2       /* #pragma setlocale( "encoding")   */

void    do_pragma( void)
/*
 * Process the #pragma lines.
 * 1. Process the sub-directive for MCPP.
 * 2. Pass the line to the compiler-proper.
 *      #pragma MCPP put_defines, #pragma MCPP preprocess,
 *      #pragma MCPP preprocessed and #pragma once are, however, not put
 *      out so as not to duplicate output when re-preprocessed.
 * When EXPAND_PRAGMA == TRUE and (__STDC_VERSION__ >= 199901L or
 * __cplusplus >= 199901L), the line is subject to macro expansion unless
 * the next to 'pragma' token is one of 'STDC', 'GCC' or 'MCPP'.
 */
{
    int         c;
    int         warn = FALSE;               /* Necessity of warning */
    int         token_type;
    char *      bp;                         /* Pointer to argument  */
    char *      tp;
    FILEINFO *  file;

    wrong_line = TRUE;                      /* In case of error     */
    c = skip_ws();
    bp = infile->bptr - 1;  /* Remember token to pass to compiler   */
    if (c == '\n') {
        if (warn_level & 1)
            cwarn( "No sub-directive", 0, 0L, 0);     /* _W1_ */
        unget_ch();
        return;
    }
    token_type = scan_token( c, (tp = work_buf, &tp), work_end);
#if EXPAND_PRAGMA
#if COMPILER == MSC
    if (token_type == NAM
            && !str_eq( identifier, "STDC") && !str_eq( identifier, "MCPP")) {
#else
    if (stdc3 && token_type == NAM
            && !str_eq( identifier, "STDC") && !str_eq( identifier, "MCPP")) {
#endif
        DEFBUF *        defp;
        char *          mp;
        char *          mp_end;

        bp = mp = xmalloc( (size_t)(NMACWORK + IDMAX));
                                    /* Buffer for macro expansion   */
        mp_end = mp + NMACWORK;
        tp = stpcpy( mp, identifier);
        do {                /* Expand all the macros in the line    */
            if (token_type == NAM && (defp = is_macro( &tp)) != 0) {
                tp = expand_macro( defp, bp, mp_end);
                if (! stdc3 && (warn_level & 2))
                    cwarn(
                "\"%s\" is macro expanded in other than C99 mode"   /* _W2_ */
                            , identifier, 0L, 0);
            }
            token_type = scan_token( c = get_ch(), (bp = tp, &tp), mp_end);
        } while (c != '\n');
        unget_string( mp, 0);                    /* To re-read   */
        ACE_OS::free( mp);
        c = skip_ws();
        bp = infile->bptr - 1;
        token_type = scan_token( c, (tp = work_buf, &tp), work_end);
    }
#endif
    if (token_type != NAM) {
        if (warn_level & 1)
            cwarn( not_ident, work_buf, 0L, 0);
        goto  skip_nl;
    } else if (str_eq( identifier, "once")) {   /* #pragma once     */
       if (! is_junk()) {
            file = infile;
            while (file->fp == 0)
                file = file->parent;
            do_once( *(file->dirp), file->real_fname);
            goto  skip_nl;
        }
    } else if (str_eq( identifier, "MCPP")) {
        if (scan_token( skip_ws(), (tp = work_buf, &tp), work_end) != NAM) {
            if (warn_level & 1)
                cwarn( not_ident, work_buf, 0L, 0);
        }
        if (str_eq( identifier, "put_defines")) {
            if (! is_junk())
                dump_def( dDflag, TRUE);        /* #pragma MCPP put_defines */
        } else if (str_eq( identifier, "preprocess")) {
            if (! is_junk())            /* #pragma MCPP preprocess  */
                mcpp_fputs( "#pragma MCPP preprocessed\n", OUT);
                    /* Just putout the directive    */
        } else if (str_eq( identifier, "preprocessed")) {
            if (! is_junk()) {          /* #pragma MCPP preprocessed*/
                skip_nl();
                do_preprocessed();
                return;
            }
        } else if (str_eq( identifier, "warning")) {
                                        /* #pragma MCPP warning     */
            cwarn( infile->buffer, 0, 0L, 0);
        } else if (str_eq( identifier, "push_macro")) {
            push_or_pop( PUSH);         /* #pragma MCPP push_macro  */
        } else if (str_eq( identifier, "pop_macro")) {
            push_or_pop( POP);          /* #pragma MCPP pop_macro   */
        } else if (str_eq( identifier, "debug")) {
            do_debug( TRUE);            /* #pragma MCPP debug       */
        } else if (str_eq( identifier, "end_debug")) {
            do_debug( FALSE);           /* #pragma MCPP end_debug   */
        } else {
            warn = TRUE;
        }
        if (warn && (warn_level & 1))
            cwarn( unknown_arg, identifier, 0L, 0);
        goto  skip_nl;                  /* Do not putout the line   */
#if COMPILER == GNUC
    /* The #pragma lines for GCC is skipped not to confuse cc1.     */
    } else if (str_eq( identifier, "GCC")) {    /* #pragma GCC *    */
        if ((scan_token( skip_ws(), (tp = work_buf, &tp), work_end) == NAM)
                && (str_eq( identifier, "poison")
                    || str_eq( identifier, "dependency")
                    || str_eq( identifier, "system_header"))) {
            if (warn_level & 2)
                cwarn( "Skipped the #pragma line"           /*_W2_  */
                        , 0, 0L, 0);
            goto skip_nl;
        }
#endif

#if COMPILER == MSC
    } else if (str_eq( identifier, "setlocale")) {
        if (skip_ws() == '('
                && scan_token( skip_ws(), (tp = work_buf, &tp), work_end)
                    == STR
                && skip_ws() == ')') {
            if (! is_junk()) {
                work_buf[ 0] = *(tp - 1) = '\0';
                set_encoding( work_buf + 1, 0, SETLOCALE);
                work_buf[ 0] = *(tp - 1) = '"';
            }   /* else warned by is_junk() */
        } else {
            warn = TRUE;
        }
#else   /* COMPILER != MSC  */
    } else if (str_eq( identifier, "__setlocale")) {
        if (skip_ws() == '('
                && scan_token( skip_ws(), (tp = work_buf, &tp), work_end)
                        == STR
                && skip_ws() == ')') {
            if (! is_junk()) {              /* #pragma __setlocale  */
                work_buf[ 0] = *(tp - 1) = '\0';
                set_encoding( work_buf + 1, 0, __SETLOCALE);
                work_buf[ 0] = *(tp - 1) = '"';
            }   /* else warned by is_junk() */
        } else {
            warn = TRUE;
        }
#endif

#if COMPILER == MSC
    } else if (str_eq( identifier, "push_macro")) {
        push_or_pop( PUSH);
        goto  skip_nl;
    } else if (str_eq( identifier, "pop_macro")) {
        push_or_pop( POP);
        goto  skip_nl;
#endif

#if COMPILER == LCC
    } else if (str_eq( identifier, "optimize")
                && (skip_ws() == '(')
                && (char_type[ (c = skip_ws()) & UCHARMAX] == DIG)
                && (skip_ws() == ')')) {
        char    tmp[ 2];

        tmp[ 0] = c;
        tmp[ 1] = EOS;
        look_and_install( optim_name, DEF_NOARGS - 1, null, tmp);
#endif

#if COMPILER == COMPILER_UNKNOWN
    /*
     * Write here any compiler-specific #pragma sub-directive which should
     * be processed by preprocessor.
     */
#endif
    }

    if (warn) {
        if (warn_level & 1)
            cwarn( unknown_arg, identifier, 0L, 0);
        goto  skip_nl;                  /* Do not putout the line   */
    }

    sharp();            /* Synchronize line number before output    */
    if (! no_output) {
        mcpp_fputs( "#pragma ", OUT);
        mcpp_fputs( bp, OUT);           /* Line is put out          */
    }
skip_nl: /* Don't use skip_nl() which skips to the newline in source file */
    while (get_ch() != '\n')
        ;
}

static void do_once(
    const char *    dir,
    const char *    filename
)
/*
 * Process #pragma MCPP once or #pragma once so as not to re-include the file
 * in future.
 * This directive has been imported from GCC V.1.* / cpp as an extension.
 */
{
    INC_LIST *  inc;
    size_t      fnamlen;

    filename = norm_path( dir, filename);   /* Normalize path name  */
    fnamlen = ACE_OS::strlen( filename);
    inc = (INC_LIST *) xmalloc( sizeof (INC_LIST) + fnamlen + 1);
    ACE_OS::memcpy( inc->fname, filename, fnamlen + 1);
    ACE_OS::free( (void *) filename);
    inc->next = 0;
    if (start_inc == 0)
        start_inc = last_inc = inc;         /* The first file       */
    else
        last_inc = last_inc->next = inc;    /* Append the file to the list  */
}

static int  included(
    const char *    dir,
    const char *    filename
)
/*
 * Has the file been once included ?
 * This routine is called only from open_file().
 */
{
    INC_LIST *  inc;
    char *      fname;

    fname = norm_path( dir, filename);
    for (inc = start_inc; inc; inc = inc->next) {
        if (str_eq( inc->fname, fname)) {
            /* Already included */
            if (mcpp_debug & PATH)
                mcpp_fprintf( DBG, "Once included \"%s\"\n", fname);
            ACE_OS::free( fname);
            return  TRUE;
        }
    }
    ACE_OS::free( fname);
    return  FALSE;                          /* Not yet included     */
}

static void push_or_pop(
    int     direction
)
/* Process #pragma MCPP push_macro( "MACRO"),
 * #pragma MCPP pop_macro( "MACRO") for other compilers than Visual C,
 * and #pragma push_macro( "MACRO"), #pragma pop_macro( "MACRO") for Visual C.
 * Note:1. "push" count is set in defp->push.
 *      2. pushed definitions are inserted immediatly after the current
 *          definition of the same name.
 *      3. the definitions of a same name macro can be pushed multiple times.
 */
{
    char *          tp;
    DEFBUF **       prevp;
    DEFBUF *        defp;
    DEFBUF *        dp;
    int             cmp;
    size_t          s_name, s_def;

    if (skip_ws() == '('
            && scan_token( skip_ws(), (tp = work_buf, &tp), work_end) == STR
            && skip_ws() == ')') {          /* Correct syntax       */

        if (is_junk())
            return;
        s_name = ACE_OS::strlen( work_buf) - 2;
        *(work_buf + s_name + 1) = '\0';
        ACE_OS::memcpy( identifier, work_buf + 1, s_name + 1);
                                            /* Remove enclosing '"' */
        prevp = look_prev( identifier, &cmp);
        if (cmp == 0) { /* Current definition or pushed definition exists   */
            defp = *prevp;
            if (direction == PUSH) {/* #pragma push_macro( "MACRO") */
                if (defp->push) {           /* No current definition*/
                    if (warn_level & 1)
                        cwarn( "\"%s\" is already pushed"   /* _W1_ */
                                , identifier, 0L, 0);
                    return;
                }
                /* Else the current definition exists.  Push it     */
                s_def = sizeof (DEFBUF) + 3 + s_name
                        + ACE_OS::strlen( defp->repl) + ACE_OS::strlen( defp->fname);
                if (mcpp_mode == STD)
                    s_def += ACE_OS::strlen( defp->parmnames);
                dp = (DEFBUF *) xmalloc( s_def);
                ACE_OS::memcpy( dp, defp, s_def);   /* Copy the definition  */
                dp->link = *prevp;          /* Insert to linked-list*/
                *prevp = dp;                /*      the pushed def  */
                prevp = &dp->link;          /* Next link to search  */
            } else {                /* #pragma pop_macro( "MACRO")  */
                if (defp->push == 0) {      /* Current definition   */
                    if (defp->link == 0
                            || ! str_eq( identifier, defp->link->name)) {
                        if (warn_level & 1)
                            cwarn( "\"%s\" has not been pushed"     /* _W1_ */
                                    , identifier, 0L, 0);
                        return;
                    } else {
                        *prevp = defp->link;
                                /* Link the previous and the next   */
                        ACE_OS::free( defp);
                            /* Delete the definition to enable popped def   */
                    }
                }   /* Else no current definition exists    */
            }
            while ((defp = *prevp) != 0) {
                /* Increment or decrement "push" count of all pushed defs   */
                if ((cmp = ACE_OS::memcmp( defp->name, identifier, s_name)) > 0)
                    break;
                defp->push += direction;        /* Increment or decrement   */
                prevp = &defp->link;
            }
        } else {    /* No current definition nor pushed definition  */
            if (warn_level & 1)
                cwarn( "\"%s\" has not been defined"        /* _W1_ */
                        , identifier, 0L, 0);
        }
    } else {        /* Wrong syntax */
        if (warn_level & 1)
            cwarn( "Bad %s syntax", direction == PUSH       /* _W1_ */
                    ? "push_macro" : "pop_macro", 0L, 0);
    }
}

static void do_asm(
    int     asm_start                       /* #asm ?               */
)
/*
 * #asm, #endasm
 * Originally written for OS-9/09 Microware C.
 */
{
    if (! compiling)
        return;
    if (asm_start == (in_asm != 0L)) {
        if (in_asm)
            cerror( "In #asm block started at line %.0s%ld" /* _E_  */
                    , 0, in_asm, 0);
        else
            cerror( "Without #asm", 0, 0L, 0);        /* _E_  */
        skip_nl();
        unget_ch();
        return;
    }
    in_asm = asm_start ? src_line : 0L;
}

void    do_old( void)
/*
 * Process the out-of-standard directives.
 * GCC permits #include_next and #warning even in STANDARD mode.
 */
{
    static const char * const   unknown
            = "Unknown #directive \"%s\"%.0ld%s";       /* _E_ _W8_ */

#if COMPILER == GNUC
    static const char * const   gnu_ext
            = "%s is not allowed by Standard%.0ld%s";   /* _W2_ _W8_*/

    if (str_eq( identifier, "include_next")) {
        if ((compiling && (warn_level & 2))
                || (! compiling && warn_level & 8))
            cwarn( gnu_ext, "#include_next", 0L
                    , compiling ? 0 : " (in skipped block)");
        if (! compiling)
            return;
        in_include = TRUE;
        do_include( TRUE);
        in_include = FALSE;
        return;
    } else if (str_eq( identifier, "warning")) {
        if ((compiling && (warn_level & 2))
                || (! compiling && warn_level & 8))
            cwarn( gnu_ext, "#warning", 0L
                    , compiling ? 0 : " (in skipped block)");
        if (! compiling)
            return;
        cwarn( infile->buffer, 0, 0L, 0);
                                    /* Always output the warning    */
        skip_nl();
        unget_ch();
        return;
    } else if (str_eq( identifier, "ident") || str_eq( identifier, "sccs")) {
        if ((compiling && (warn_level & 1))
                || (! compiling && warn_level & 8)) {
            if (str_eq( identifier, "ident"))
                cwarn(
    compiling ? "Ignored #ident" : "#ident (in skipped block)"  /* _W1_ _W8_*/
                        , 0, 0L, 0);
            else
                cwarn(
    compiling ? "Ignored #sccs" : "#sccs (in skipped block)"    /* _W1_ _W8_*/
                        , 0, 0L, 0);
        }
        if (! compiling)
            return;
        skip_nl();
        unget_ch();
        return;
    }
#endif  /* COMPILER == GNUC */

#if COMPILER == MSC
    if (str_eq( identifier, "using") || str_eq( identifier, "import")) {
                                            /* #using or #import    */
        if (! compiling)
            return;
        mcpp_fputs( infile->buffer, OUT);   /* Putout the line as is*/
        skip_nl();
        unget_ch();
        return;
    }
#endif

    if (! standard && do_prestd_directive())
        return;

    if (compiling) {
        if (lang_asm) {                     /* "Assembler" source   */
            if (warn_level & 1)
                cwarn( unknown, identifier, 0L, 0);
            mcpp_fputs( infile->buffer, OUT);   /* Putout the line  */
        } else {
            cerror( unknown, identifier, 0L, 0);
        }
    } else if (warn_level & 8) {
        cwarn( unknown, identifier, 0L, " (in skipped block)");
    }
    skip_nl();
    unget_ch();
    return;
}

static int  do_prestd_directive( void)
/*
 * Process directives for pre-Standard mode.
 */
{
#if COMPILER != GNUC
    if (str_eq( identifier, "assert")) {    /* #assert              */
        if (! compiling)                    /* Only validity check  */
            return  TRUE;
        if (eval_if() == 0L) {              /* Assert expression    */
            cerror( "Preprocessing assertion failed"        /* _E_  */
                    , 0, 0L, 0);
            skip_nl();
            unget_ch();
        }
        return  TRUE;
    } else
#endif
    if (str_eq( identifier, "put_defines")) {
        if (! compiling)                    /* Only validity check  */
            return  TRUE;
        if (mcpp_mode != OLD_PREP && ! is_junk())
            dump_def( dDflag, TRUE);        /* #put_defines         */
        skip_nl();
        unget_ch();
        return  TRUE;
    } else if (str_eq( identifier, "preprocess")) {
        if (! compiling)                    /* Only validity check  */
            return  TRUE;
        if (mcpp_mode != OLD_PREP && ! is_junk())
        /* Just putout the directive for the succeding preprocessor */
            mcpp_fputs( "#preprocessed\n", OUT);
        skip_nl();
        unget_ch();
        return  TRUE;
    } else if (str_eq( identifier, "preprocessed")) {
        if (! compiling)                    /* Only validity check  */
            return  TRUE;
        if (mcpp_mode != OLD_PREP && ! is_junk()) {
            skip_nl();
            do_preprocessed();              /* #preprocessed        */
            return  TRUE;
        }
        skip_nl();
        unget_ch();
        return  TRUE;
    }

    if (str_eq( identifier, "debug")) {     /* #debug <args>        */
        if (! compiling)                    /* Only validity check  */
            return  TRUE;
        do_debug( TRUE);
        return  TRUE;
    } else if (str_eq( identifier, "end_debug")) {
        if (! compiling)
            return  TRUE;
        do_debug( FALSE);                   /* #end_debug <args>    */
        return  TRUE;
    }

    if (str_eq( identifier, "asm")) {       /* #asm                 */
        do_asm( TRUE);
        return  TRUE;
    }
    if (str_eq( identifier, "endasm")) {    /* #endasm              */
        do_asm( FALSE);
        skip_nl();                          /* Skip comments, etc.  */
        unget_ch();
        return  TRUE;
    }

    return  FALSE;                          /* Unknown directive    */
}

static void do_preprocessed( void)
/*
 * The source file has been already preprocessed.
 * Copy the lines to output.
 * Install macros according the #define directives.
 */
{
    const char *    corrupted =
            "This preprocessed file is corrupted";          /* _F_  */
    FILEINFO *      file;
    char *          lbuf;
    char *          cp;
    const char **   incptr;
    char *          comment = 0;
    char *          colon = 0;
    const char *    dir;
#if STD_LINE_PREFIX == FALSE
    char            conv[ NBUFF];
    char *          arg;

    /*
     * Compiler cannot accept C source style #line.
     * Convert it to the compiler-specific format.
     */
    ACE_OS::strcpy( conv, LINE_PREFIX);
    arg = conv + ACE_OS::strlen( conv);
#endif
    file = infile;
    lbuf = file->bptr = file->buffer;           /* Reset file->bptr */

    /* Copy the input to output until a comment line appears.       */
    while (ACE_OS::fgets( lbuf, NBUFF, file->fp) != 0
            && ACE_OS::memcmp( lbuf, "/*", 2) != 0) {
#if STD_LINE_PREFIX == FALSE
        if (ACE_OS::memcmp( lbuf, "#line ", 6) == 0) {
            ACE_OS::strcpy( arg, lbuf + 6);
            mcpp_fputs( conv, OUT);
        } else
#endif
        {
            mcpp_fputs( lbuf, OUT);
        }
    }
    if (! str_eq( lbuf, "/* Currently defined macros. */\n"))
        cfatal( "This is not a preprocessed source"         /* _F_  */
                , 0, 0L, 0);

    /* Define macros according to the #define lines.    */
    while (ACE_OS::fgets( lbuf, NWORK, file->fp) != 0) {
        if (ACE_OS::memcmp( lbuf, "/*", 2) == 0) {
                                    /* Standard predefined macro    */
            continue;
        } else {
            if (ACE_OS::memcmp( lbuf, "#define ", 8) != 0) {
                if (ACE_OS::memcmp( lbuf, "#line", 5) == 0)
                    continue;
                else
                    cfatal( corrupted, 0, 0L, 0);
            }
            /* Filename and line-number information in comment as:  */
            /* dir/fname:1234\t*/
            cp = lbuf + ACE_OS::strlen( lbuf);
            if ((ACE_OS::memcmp( cp - 4, "\t*/\n", 4) != 0)
                    || (*(cp - 4) = EOS
                            , (comment = ACE_OS::strrchr( lbuf, '*')) == 0)
                    || (ACE_OS::memcmp( --comment, "/* ", 3) != 0)
                    || ((colon = ACE_OS::strrchr( comment, ':')) == 0))
                cfatal( corrupted, 0, 0L, 0);
            src_line = atol( colon + 1);    /* Pseudo line number   */
            *colon = EOS;
            dir = comment + 3;
            inc_dirp = &null;
            /* Search the include directory list    */
            for (incptr = incdir ; incptr < incend; incptr++) {
                if (ACE_OS::memcmp( *incptr, dir, ACE_OS::strlen( *incptr)) == 0) {
                    inc_dirp = incptr;
                    break;
                }
            }
            /* Register the filename to fnamelist[] */
            /* inc_dirp may be 0, and cur_fname may be "(predefined)"    */
            cur_fname = set_fname( dir + ACE_OS::strlen( *inc_dirp));
            ACE_OS::strcpy( comment - 2, "\n");     /* Remove the comment   */
            unget_string( lbuf + 8, 0);
            do_define( FALSE);
            get_ch();                           /* '\n' */
            get_ch();                           /* Clear the "file" */
            unget_ch();                         /* infile == file   */
        }
    }
    file->bptr = file->buffer + ACE_OS::strlen( file->buffer);
}

static int  do_debug(
    int     set                         /* TRUE to set debugging    */
)
/*
 * #pragma MCPP debug, #pragma MCPP end_debug, #debug, #end_debug
 * Return TRUE when diagnostic is issued else return FALSE.
 */
{
    struct Debug_arg {
        const char *    arg_name;               /* Name of option   */
        int     arg_num;                        /* Value of 'debug' */
    };
    static struct Debug_arg     debug_args[] = {
        { "path",   PATH    },
        { "token",  TOKEN   },
        { "expand", EXPAND  },
        { "if",     IF      },
        { "expression", EXPRESSION  },
        { "getc",   GETC    },
        { "memory", MEMORY  },
        { 0,     0       },
    };
    struct Debug_arg    *argp;
    int     num;
    int     c;

    c = skip_ws();
    if (c == '\n') {
        unget_ch();
        if (set) {
            if (warn_level & 1)
                cwarn( "No argument", 0, 0L, 0);      /* _W1_ */
            return TRUE;
        } else {
            mcpp_debug = 0;                 /* Clear all the flags  */
            return FALSE;
        }
    }
    while (scan_token( c, (workp = work_buf, &workp), work_end) == NAM) {
        argp = debug_args;
        while (argp->arg_name) {
            if (str_eq( argp->arg_name, work_buf))
                break;
            argp++;
        }
        if (argp->arg_name == 0) {
            if (warn_level & 1)
                cwarn( unknown_arg, work_buf, 0L, 0);
            goto  diagnosed;
        } else {
            num = argp->arg_num;
            if (set) {
                mcpp_debug |= num;
                if (num == PATH)
                    dump_path();
                else if (num == MEMORY)
                    print_heap();
            } else {
                mcpp_debug &= ~num;
            }
        }
        c = skip_ws();
    }
    if (c != '\n') {
        if (warn_level & 1)
            cwarn( not_ident, work_buf, 0L, 0);
        skip_nl();
        unget_ch();
        goto  diagnosed;
    }
    unget_ch();
    return FALSE;
diagnosed:
    return TRUE;
}

void    put_asm( void)
/*
 * Put out source line as it is.
 */
{
#if 0
    mcpp_fputs( "#2\n", OUT);
    mcpp_fputs( infile->buffer, OUT);
    skip_nl();
#endif
}

static void dump_path( void)
/*
 * Show the include directories.
 */
{
    const char **   incptr;
    const char *    inc_dir;
    const char *    dir = "./";

    mcpp_fputs( "Include paths are as follows --\n", DBG);
    for (incptr = incdir; incptr < incend; incptr++) {
        inc_dir = *incptr;
        if (*inc_dir == '\0')
            inc_dir = dir;
        mcpp_fprintf( DBG, "    %s\n", inc_dir);
    }
    mcpp_fputs( "End of include path list.\n", DBG);
}

/*
 * list_heap() is a function to print out information of heap-memory.
 * See "kmmalloc-2.5.1.lzh" by kmatsui.
 */
#if     KMMALLOC
    int     list_heap( int);
#elif   BSD_MALLOC
    int     list_heap( char *);
#elif   DB_MALLOC || DMALLOC
    int     list_heap( FILE *);
#endif

void    print_heap( void)
{
#if     KMMALLOC
    list_heap( 1);
#elif   BSD_MALLOC
    list_heap( ":cpp");
#elif   DB_MALLOC || DMALLOC || PHK_MALLOC || DLMALLOC
    list_heap( fp_debug);
#endif
}

void    at_end( void)
/*
 * Handle the commands to be executed at the end of processing.
 */
{
#if COMPILER == GNUC
    if (dMflag || dDflag) {
        dump_def( dDflag, FALSE);
    }
#endif
}

#if MCPP_LIB
void    clear_filelist( void)
/*
 * Free malloced memory for filename-list and directory-list.
 */
{
    const char **   namep;
    INC_LIST *  inc;
    INC_LIST *  pnext;

    for (namep = fnamelist; namep < fname_end; namep++)
        ACE_OS::free( (void*) *namep);

    if (standard) {
        for (inc = start_inc; inc != 0; inc = pnext) {
            pnext = inc->next;
            ACE_OS::free( inc);
        }
        start_inc = 0;
    }

    for (namep = incdir; namep < incend; namep++)
        ACE_OS::free( (void *) *namep);
}
#endif

