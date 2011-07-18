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
 *                          D I R E C T I V E . C
 *              P r o c e s s   D i r e c t i v e  L i n e s
 *
 * The routines to handle directives other than #include and #pragma
 * are placed here.
 */

#if PREPROCESSED
#include    "mcpp.H"
#else
#include    "system.H"
#include    "internal.H"
#endif

#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_ctype.h"

static int      do_if( int hash);
                /* #if, #elif, #ifdef, #ifndef      */
static long     do_line( void);
                /* Process #line directive          */
static int      get_parm( void);
                /* Get parm., its nargs, names, lens*/
static int      get_repl( const char * macroname);
                /* Get replacement embedded parm-no.*/
static char *   is_formal( const char * name, int conv);
                /* If formal param., save the num.  */
static char *   def_stringization( char * repl_cur);
                /* Define stringization     */
static char *   mgtoken_save( const char * macroname);
                /* Prefix DEF_MAGIC to macro name   */
static char *   str_parm_scan( char * string_end);
                /* Scan the param. in string        */
static void     do_undef( void);
                /* Process #undef directive         */
static void     dump_repl( const DEFBUF * dp, FILE * fp);
                /* Dump replacement text            */

/*
 * Generate (by hand-inspection) a set of unique values for each directive.
 * MCPP won't compile if there are hash conflicts.
 */

#define L_if            ('i' ^ (EOS << 1))
#define L_ifdef         ('i' ^ ('e' << 1))
#define L_ifndef        ('i' ^ ('d' << 1))
#define L_elif          ('e' ^ ('f' << 1))
#define L_else          ('e' ^ ('e' << 1))
#define L_endif         ('e' ^ ('i' << 1))
#define L_define        ('d' ^ ('i' << 1))
#define L_undef         ('u' ^ ('e' << 1))
#define L_line          ('l' ^ ('e' << 1))
#define L_include       ('i' ^ ('l' << 1))
#if COMPILER == GNUC
#define L_include_next  ('i' ^ ('l' << 1) ^ ('_' << 1))
#endif
#define L_error         ('e' ^ ('o' << 1))
#define L_pragma        ('p' ^ ('g' << 1))

static const char * const   not_ident
            = "Not an identifier \"%s\"";               /* _E_      */
static const char * const   no_arg = "No argument";     /* _E_      */
static const char * const   excess
            = "Excessive token sequence \"%s\"";        /* _E_ _W1_ */

void    directive( void)
/*
 * Process #directive lines.  Each directive have their own subroutines.
 */
{
    const char * const  many_nesting =
"More than %.0s%ld nesting of #if (#ifdef) sections%s"; /* _F_ _W4_ _W8_    */
    const char * const  not_in_section
    = "Not in a #if (#ifdef) section in a source file"; /* _E_ _W1_ */
    const char * const  illeg_dir
            = "Illegal #directive \"%s%.0ld%s\"";       /* _E_ _W1_ _W8_    */
    const char * const  in_skipped = " (in skipped block)"; /* _W8_ */
    FILEINFO *  file;
    int     token_type;
    int     hash;
    int     c;
    const char *  tp;

    in_directive = TRUE;
    if (keep_comments) {
        mcpp_fputc( '\n', OUT);     /* Possibly flush out comments  */
        newlines--;
    }
    c = skip_ws();
    if (c == '\n')                              /* 'null' directive */
        goto  ret;
    token_type = scan_token( c, (workp = work_buf, &workp), work_end);
    if (in_asm && (token_type != NAM            /* In #asm block    */
            || (! str_eq( identifier, "asm")    /* Ignore #anything */
                && ! str_eq( identifier, "endasm"))))   /*   other  */
        goto  skip_line;                /*    than #asm or #endasm  */
    if (token_type != NAM) {
        if (mcpp_mode == OLD_PREP && token_type == NUM) {   /* # 123 [fname]*/
            ACE_OS::strcpy( identifier, "line");
        } else {
            if (compiling) {
                if (lang_asm) {
                    if (warn_level & 1)
                        cwarn( illeg_dir, work_buf, 0L, 0);
                } else {
                    cerror( illeg_dir, work_buf, 0L, 0);
                }
            } else if (warn_level & 8) {
                cwarn( illeg_dir, work_buf, 0L, in_skipped);
            }
            goto  skip_line;
        }
    }
    if (identifier[ 2] == EOS)
        identifier[ 3] = EOS;                   /* Diddle           */
    hash = (identifier[ 1] == EOS) ? identifier[ 0]
            : (identifier[ 0] ^ (identifier[ 3] << 1));
    if (ACE_OS::strlen( identifier) > 7)
        hash ^= (identifier[ 7] << 1);

    switch (hash) {
    case L_if:      tp = "if";      break;
    case L_ifdef:   tp = "ifdef";   break;
    case L_ifndef:  tp = "ifndef";  break;
    case L_elif:    tp = "elif";    break;
    case L_else:    tp = "else";    break;
    case L_endif:   tp = "endif";   break;
    case L_define:  tp = "define";  break;
    case L_undef:   tp = "undef";   break;
    case L_line:    tp = "line";    break;
    case L_include: tp = "include"; break;
#if COMPILER == GNUC
    case L_include_next:    tp = "include_next";    break;
#endif
    case L_error:   tp = "error";   break;
    case L_pragma:  tp = "pragma";  break;
    default:        tp = 0;      break;
    }

    if (tp != 0 && ! str_eq( identifier, tp)) {  /* Hash conflict*/
        hash = 0;                       /* Unknown directive, will  */
        tp = 0;                      /*   be handled by do_old() */
    }

    /*
     * hash is set to a unique value corresponding to the directive.
     */
    if (! compiling) {                      /* Not compiling now    */
        switch (hash) {
        case L_elif :
            if (! standard) {
                if (warn_level & 8)
                    do_old();               /* Unknown directive    */
                goto  skip_line;            /* Skip the line        */
            }   /* Else fall through    */
        case L_else :       /* Test the #if's nest, if 0, compile   */
        case L_endif:                       /* Un-nest #if          */
            break;
        case L_if   :                       /* These can't turn     */
        case L_ifdef:                       /*  compilation on, but */
        case L_ifndef:                      /*   we must nest #if's.*/
            if (&ifstack[ BLK_NEST] < ++ifptr)
                goto  if_nest_err;
            if (standard && (warn_level & 8)
                    && &ifstack[ blk_nest_min + 1] == ifptr)
                cwarn( many_nesting, 0, (long) blk_nest_min, in_skipped);
            ifptr->stat = 0;                /* !WAS_COMPILING       */
            ifptr->ifline = src_line;       /* Line at section start*/
            goto  skip_line;
        default     :                       /* Other directives     */
            if (tp == 0 && (warn_level & 8))
                do_old();                   /* Unknown directive ?  */
            goto  skip_line;                /* Skip the line        */
        }
    }
    macro_line = 0;                         /* Reset error flag     */
    file = infile;                  /* Remember the current file    */

    switch (hash) {

    case L_if:
    case L_ifdef:
    case L_ifndef:
        if (&ifstack[ BLK_NEST] < ++ifptr)
            goto  if_nest_err;
        if (standard && (warn_level & 4) &&
                &ifstack[ blk_nest_min + 1] == ifptr)
            cwarn( many_nesting, 0 , (long) blk_nest_min, 0);
        ifptr->stat = WAS_COMPILING;
        ifptr->ifline = src_line;
        goto  ifdo;

    case L_elif:
        if (! standard) {
            do_old();               /* Unrecognized directive       */
            break;
        }
        if (ifptr == &ifstack[0])
            goto  nest_err;
        if (ifptr == infile->initif) {
            goto  in_file_nest_err;
        }
        if (ifptr->stat & ELSE_SEEN)
            goto  else_seen_err;
        if ((ifptr->stat & (WAS_COMPILING | TRUE_SEEN)) != WAS_COMPILING) {
            compiling = FALSE;              /* Done compiling stuff */
            goto  skip_line;                /* Skip this group      */
        }
        hash = L_if;
ifdo:
        c = do_if( hash);
        if (mcpp_debug & IF) {
            mcpp_fprintf( DBG
                    , "#if (#elif, #ifdef, #ifndef) evaluate to %s.\n"
                    , compiling ? "TRUE" : "FALSE");
            mcpp_fprintf( DBG, "line %ld: %s", src_line, infile->buffer);
        }
        if (c == FALSE) {                   /* Error                */
            compiling = FALSE;              /* Skip this group      */
            goto  skip_line;    /* Prevent an extra error message   */
        }
        break;

    case L_else:
        if (ifptr == &ifstack[0])
            goto  nest_err;
        if (ifptr == infile->initif) {
            if (standard)
                goto  in_file_nest_err;
            else if (warn_level & 1)
                cwarn( not_in_section, 0, 0L, 0);
        }
        if (ifptr->stat & ELSE_SEEN)
            goto  else_seen_err;
        ifptr->stat |= ELSE_SEEN;
        ifptr->elseline = src_line;
        if (ifptr->stat & WAS_COMPILING) {
            if (compiling || (ifptr->stat & TRUE_SEEN) != 0)
                compiling = FALSE;
            else
                compiling = TRUE;
        }
        break;

    case L_endif:
        if (ifptr == &ifstack[0])
            goto  nest_err;
        if (ifptr <= infile->initif) {
            if (standard)
                goto  in_file_nest_err;
            else if (warn_level & 1)
                cwarn( not_in_section, 0, 0L, 0);
        }
        if (! compiling && (ifptr->stat & WAS_COMPILING))
            wrong_line = TRUE;
        compiling = (ifptr->stat & WAS_COMPILING);
        --ifptr;
        break;

    case L_define:
        do_define( FALSE);
        break;

    case L_undef:
        do_undef();
        break;

    case L_line:
        if ((c = do_line()) > 0) {
            src_line = c;
            sharp();    /* Putout the new line number and file name */
            infile->line = --src_line;  /* Next line number is 'src_line'   */
            newlines = -1;
        } else {            /* Error already diagnosed by do_line()  */
            skip_nl();
        }
        break;

    case L_include:
        in_include = TRUE;
        if (do_include( FALSE) == TRUE && file != infile)
            newlines = -1;  /* File has been included. Clear blank lines    */
        in_include = FALSE;
        break;

    case L_error:
        if (! standard) {
            do_old();               /* Unrecognized directive       */
            break;
        }
        cerror( infile->buffer, 0, 0L, 0);            /* _E_  */
        break;

    case L_pragma:
        if (! standard) {
            do_old();               /* Unrecognized directive       */
            break;
        }
        do_pragma();
        newlines = -1;              /* Do not putout excessive '\n' */
        break;

    default:                /* Non-Standard or unknown directives   */
        do_old();
        break;
    }

    switch (hash) {
    case L_if       :
    case L_define   :
    case L_line     :
        goto  skip_line;    /* To prevent duplicate error message   */
#if COMPILER == GNUC
    case L_include_next  :
        if (file != infile)             /* File has been included   */
            newlines = -1;
#endif
    case L_error    :
        if (standard)
            goto  skip_line;
        /* Else fall through    */
    case L_include  :
    case L_pragma   :
        if (standard)
            break;          /* Already read over the line           */
        /* Else fall through    */
    default :               /* L_else, L_endif, L_undef, etc.       */
        if (mcpp_mode == OLD_PREP) {
        /*
         * Ignore the rest of the #directive line so you can write
         *          #if     foo
         *          #endif  foo
         */
            ;
        } else if (skip_ws() != '\n') {
            if (standard)
                cerror( excess, infile->bptr-1, 0L, 0);
            else if (warn_level & 1)
                cwarn( excess, infile->bptr-1, 0L, 0);
        }
        skip_nl();
    }
    goto  ret;

in_file_nest_err:
    cerror( not_in_section, 0, 0L, 0);
    goto  skip_line;
nest_err:
    cerror( "Not in a #if (#ifdef) section", 0, 0L, 0);       /* _E_  */
    goto  skip_line;
else_seen_err:
    cerror( "Already seen #else at line %.0s%ld"            /* _E_  */
            , 0, ifptr->elseline, 0);
skip_line:
    skip_nl();                              /* Ignore rest of line  */
    goto  ret;

if_nest_err:
    cfatal( many_nesting, 0, (long) BLK_NEST, 0);

ret:
    in_directive = FALSE;
    keep_comments = cflag && compiling && !no_output;
    if (! wrong_line)
        newlines++;
}

static int  do_if( int hash)
/*
 * Process an #if (#elif), #ifdef or #ifndef.  The latter two are straight-
 * forward, while #if needs a subroutine of its own to evaluate the
 * expression.
 * do_if() is called only if compiling is TRUE.  If false, compilation is
 * always supressed, so we don't need to evaluate anything.  This supresses
 * unnecessary warnings.
 */
{
    int     c;
    int     found;

    if ((c = skip_ws()) == '\n') {
        unget_ch();
        cerror( no_arg, 0, 0L, 0);
        return  FALSE;
    }
    if (hash == L_if) {                 /* #if or #elif             */
        unget_ch();
        found = (eval_if() != 0L);      /* Evaluate expression      */
        hash = L_ifdef;                 /* #if is now like #ifdef   */
    } else {                            /* #ifdef or #ifndef        */
        if (scan_token( c, (workp = work_buf, &workp), work_end) != NAM) {
            cerror( not_ident, work_buf, 0L, 0);
            return  FALSE;      /* Next token is not an identifier  */
        }
        found = (look_id( identifier) != 0); /* Look in table    */
    }
    if (found == (hash == L_ifdef)) {
        compiling = TRUE;
        ifptr->stat |= TRUE_SEEN;
    } else {
        compiling = FALSE;
    }
    return  TRUE;
}

static long do_line( void)
/*
 * Parse the line to update the line number and "filename" field for the next
 * input line.
 * Values returned are as follows:
 *  -1:     syntax error or out-of-range error (diagnosed by do_line(),
 *          eval_num()).
 *  [1,32767]:  legal line number for C90, [1,2147483647] for C99.
 * Line number [32768,2147483647] in C90 mode is only warned (not an error).
 * do_line() always absorbs the line (except the <newline>).
 */
{
    const char * const  not_digits
        = "Line number \"%s\" isn't a decimal digits sequence"; /* _E_ _W1_ */
    const char * const  out_of_range
        = "Line number \"%s\" is out of range of [1,%ld]";      /* _E_ _W1_ */
    int     token_type;
    VAL_SIGN *      valp;
    char *  save;
    int     c;

    if ((c = skip_ws()) == '\n') {
        cerror( no_arg, 0, 0L, 0);
        unget_ch();                         /* Push back <newline>  */
        return  -1L;                /* Line number is not changed   */
    }

    if (standard) {
        token_type = get_unexpandable( c, FALSE);
        if (macro_line == MACRO_ERROR)      /* Unterminated macro   */
            return  -1L;                    /*   already diagnosed. */
        if (token_type == NO_TOKEN) /* Macro expanded to 0 token    */
            goto  no_num;
        if (token_type != NUM)
            goto  illeg_num;
    } else if (scan_token( c, (workp = work_buf, &workp), work_end) != NUM) {
        goto  illeg_num;
    }
    for (workp = work_buf; *workp != EOS; workp++) {
      if (! ACE_OS::ace_isdigit( *workp & UCHARMAX)) {
            if (standard) {
                cerror( not_digits, work_buf, 0L, 0);
                return  -1L;
            } else if (warn_level & 1) {
                cwarn( not_digits, work_buf, 0L, 0);
            }
        }
    }
    valp = eval_num( work_buf);             /* Evaluate number      */
    if (valp->sign == VAL_ERROR) {  /* Error diagnosed by eval_num()*/
        return  -1;
    } else if (standard && (line_limit < valp->val || valp->val <= 0L)) {
        if (valp->val < LINE99LIMIT && valp->val > 0L) {
            if (warn_level & 1)
                cwarn( out_of_range, work_buf, line_limit, 0);
        } else {
            cerror( out_of_range, work_buf, line_limit, 0);
            return  -1L;
        }
    }

    if (standard) {
        token_type = get_unexpandable( skip_ws(), FALSE);
        if (macro_line == MACRO_ERROR)
            return  -1L;
        if (token_type != STR) {
            if (token_type == NO_TOKEN) {   /* Filename is absent   */
                return  (long) valp->val;
            } else {    /* Expanded macro should be a quoted string */
                goto  not_fname;
            }
        }
    } else {
        if ((c = skip_ws()) == '\n') {
            unget_ch();
            return  (long) valp->val;
        }
        if (scan_token( c, (workp = work_buf, &workp), work_end) != STR)
            goto  not_fname;
    }
#if COMPILER == GNUC
    if (ACE_OS::memcmp( workp - 3, "//", 2) == 0) { /* "/cur-dir//"         */
        save = infile->filename;    /* Do not change the file name  */
    } else
#endif
    {
        *(workp - 1) = EOS;                 /* Ignore right '"'     */
        save = save_string( &work_buf[ 1]); /* Ignore left '"'      */
    }

    if (standard) {
        if (get_unexpandable( skip_ws(), FALSE) != NO_TOKEN) {
            cerror( excess, work_buf, 0L, 0);
            ACE_OS::free( save);
            return  -1L;
        }
    } else if (mcpp_mode == OLD_PREP) {
        skip_nl();
        unget_ch();
    } else if ((c = skip_ws()) == '\n') {
        unget_ch();
    } else {
        if (warn_level & 1) {
            scan_token( c, (workp = work_buf, &workp), work_end);
            cwarn( excess, work_buf, 0, 0);
        }
        skip_nl();
        unget_ch();
    }

    if (infile->filename)
        ACE_OS::free( infile->filename);
    infile->filename = save;                /* New file name        */
            /* Note that this does not change infile->real_fname    */
    return  (long) valp->val;               /* New line number      */

no_num:
    cerror( "No line number", 0, 0L, 0);              /* _E_  */
    return  -1L;
illeg_num:
    cerror( "Not a line number \"%s\"", work_buf, 0L, 0);        /* _E_  */
    return  -1L;
not_fname:
    cerror( "Not a file name \"%s\"", work_buf, 0L, 0);  /* _E_  */
    return  -1L;
}

/*
 *                  M a c r o  D e f i n i t i o n s
 *
 * Edit History (cpp4.c) of original version
 * 31-Aug-84    MM      USENET net.sources release
 *  2-May-85    MM      Latest revision
 */

/*
 * look_id()    Looks for the name in the defined symbol table.  Returns a
 *              pointer to the definition if found, or 0 if not present.
 * install_macro()  Installs the definition.  Updates the symbol table.
 * undefine()   Deletes the definition from the symbol table.
 */

/*
 * Global work_buf[] are used to store #define parameter lists and parlist[]
 * point to them.
 * 'nargs' contains the actual number of parameters stored.
 */
static char *   parlist[ NMACPARS]; /* -> Start of each parameter   */
static size_t   parlen[ NMACPARS];  /* Length of parameter name     */
static int      nargs;          /* Number of parameters         */
static char *   token_p;        /* Pointer to the token scanned */
static char *   repl_base;      /* Base of buffer for repl-text */
static char *   repl_end;       /* End of buffer for repl-text  */
static const char * const   no_ident = "No identifier";     /* _E_  */

DEFBUF *    do_define(
    int     ignore_redef        /* Do not redefine   */
)
/*
 * Called from directive() when a #define is scanned or called from
 *      do_options() when a -D option is scanned.  This module parses formal
 *      parameters by get_parm() and the replacement text by get_repl().
 *
 * There is some special case code to distinguish
 *      #define foo     bar     --  object-like macro
 * from #define foo()   bar     --  function-like macro with no parameter
 *
 * Also, we make sure that
 *      #define foo     foo
 * expands to "foo" but doesn't put MCPP into an infinite loop.
 *
 * A warning is printed if you redefine a symbol with a non-identical
 * text.  I.e,
 *      #define foo     123
 *      #define foo     123
 * is ok, but
 *      #define foo     123
 *      #define foo     +123
 * is not.
 *
 * The following subroutines are called from do_define():
 * get_parm()   parsing and remembering parameter names.
 * get_repl()   parsing and remembering replacement text.
 *
 * The following subroutines are called from get_repl():
 * is_formal()  is called when an identifier is scanned.  It checks through
 *              the array of formal parameters.  If a match is found, the
 *              identifier is replaced by a control byte which will be used
 *              to locate the parameter when the macro is expanded.
 * def_stringization()  is called when '#' operator is scanned.  It surrounds
 *              the token to stringize with magic-codes.
 *
 * modes other than STD ignore difference of parameter names in macro
 * redefinition.
 */
{
    const char * const  predef = "\"%s\" shouldn't be redefined";   /* _E_  */
    char    repl_list[ NMACWORK + IDMAX];   /* Replacement text     */
    char    macroname[ IDMAX + 1];  /* Name of the macro defining   */
    DEFBUF *    defp;               /* -> Old definition            */
    DEFBUF **   prevp;      /* -> Pointer to previous def in list   */
    int     c;
    int     redefined;                      /* TRUE if redefined    */
    int     dnargs = 0;                     /* defp->nargs          */
    int     cmp;                    /* Result of name comparison    */

    repl_base = repl_list;
    repl_end = & repl_list[ NMACWORK];
    if ((c = skip_ws()) == '\n') {
        cerror( no_ident, 0, 0L, 0);
        unget_ch();
        return  0;
    } else if (scan_token( c, (workp = work_buf, &workp), work_end) != NAM) {
        cerror( not_ident, work_buf, 0L, 0);
        return  0;
    } else {
        prevp = look_prev( identifier, &cmp);
        defp = *prevp;
        if (standard) {
            if (cmp || defp->push) {    /* Not known or 'pushed' macro      */
                if (str_eq( identifier, "defined")
                        || ((stdc_val || cplus_val)
                            &&  str_eq( identifier, "__VA_ARGS__"))) {
                    cerror(
            "\"%s\" shouldn't be defined", identifier, 0L, 0);   /* _E_  */
                    return  0;
                }
                redefined = FALSE;          /* Quite new definition */
            } else {                        /* It's known:          */
                if (ignore_redef)
                    return  defp;
                dnargs = (defp->nargs == DEF_NOARGS-1) ? DEF_NOARGS
                        : defp->nargs;
                if (dnargs < DEF_NOARGS - 1 /* Standard predefined  */
                        || dnargs == DEF_PRAGMA /* _Pragma() pseudo-macro   */
                        ) {
                    cerror( predef, identifier, 0L, 0);
                    return  0;
                } else {
                    redefined = TRUE;       /* Remember this fact   */
                }
            }
        } else {
            if (cmp) {
                redefined = FALSE;          /* Quite new definition */
            } else {                        /* It's known:          */
                if (ignore_redef)
                    return  defp;
                dnargs = (defp->nargs == DEF_NOARGS-1) ? DEF_NOARGS
                        : defp->nargs;
                redefined = TRUE;
            }
        }
    }
    ACE_OS::strcpy( macroname, identifier);         /* Remember the name    */

    in_define = TRUE;                       /* Recognize '#', '##'  */
    if (get_parm() == FALSE) {              /* Get parameter list   */
        in_define = FALSE;
        return  0;                       /* Syntax error         */
    }
    if (get_repl( macroname) == FALSE) {    /* Get replacement text */
        in_define = FALSE;
        return  0;                       /* Syntax error         */
    }

    in_define = FALSE;
    if (redefined) {
        if (dnargs != nargs || ! str_eq( defp->repl, repl_list)
                || (mcpp_mode == STD && ! str_eq( defp->parmnames, work_buf))
                ) {             /* Warn if differently redefined    */
            if (warn_level & 1) {
                cwarn(
            "The macro is redefined", 0, 0L, 0);      /* _W1_ */
                if (! no_source_line)
                    dump_a_def( "    previously macro", defp, FALSE, FALSE
                            , TRUE, fp_err);
            }
        } else {                        /* Identical redefinition   */
            return  defp;
        }
    }                                   /* Else new or re-definition*/
    defp = install_macro( macroname, nargs, work_buf, repl_list, prevp, cmp);
    if (mcpp_mode == STD && cplus_val && id_operator( macroname)
            && (warn_level & 1))
        /* These are operators, not identifiers, in C++98   */
        cwarn( "\"%s\" is defined as macro", macroname      /* _W1_ */
                , 0L, 0);
    return  defp;
}

static int  get_parm( void)
/*
 *   Get parameters i.e. numbers into nargs, name into work_buf[], name-length
 * into parlen[].
 *   Return TRUE if the parameters are legal, else return FALSE.
 *   In STD mode preprocessor must remember the parameter names, only for
 * checking the validity of macro redefinitions.  This is required by the
 * Standard (what an overhead !).
 */
{
    const char * const  many_parms
            = "More than %.0s%ld parameters";       /* _E_ _W4_     */
    const char * const  illeg_parm
            = "Illegal parameter \"%s\"";           /* _E_          */
    int     token_type;
    int     c;

    parlist[ 0] = workp = work_buf;
    work_buf[ 0] = EOS;

    /* POST_STD mode    */
    insert_sep = NO_SEP;    /* Clear the inserted token separator   */
    c = get_ch();

    if (c == '(') {                         /* With arguments?      */
        nargs = 0;                          /* Init parms counter   */
        if (skip_ws() == ')')
            return  TRUE;                   /* Macro with 0 parm    */
        else
            unget_ch();

        do {                                /* Collect parameters   */
            if (nargs >= NMACPARS) {
                cerror( many_parms, 0, (long) NMACPARS, 0);
                return  FALSE;
            }
            parlist[ nargs] = workp;        /* Save its start       */
            if ((token_type = scan_token( c = skip_ws(), &workp, work_end))
                    != NAM) {
                if (c == '\n') {
                    break;
                } else if (c == ',' || c == ')') {
                    cerror( "Empty parameter", 0, 0L, 0);     /* _E_  */
                    return  FALSE;
                } else if (standard && (stdc_val || cplus_val)
                        && token_type == OPE && openum == OP_ELL) {
                    /*
                     * Enable variable argument macro which is a feature of
                     * C99.  We enable this even on C90 or C++ for GCC
                     * compatibility.
                     */
                    if (skip_ws() != ')') {
                        cerror( "\"...\" isn't the last parameter", /* _E_  */
                                0, 0L, 0);
                        return  FALSE;
                    }
                    parlen[ nargs++] = 3;
                    nargs |= VA_ARGS;
                    goto  ret;
                } else {
                    cerror( illeg_parm , parlist[ nargs], 0L, 0);
                    return  FALSE;          /* Bad parameter syntax */
                }
            }
            if (standard && (stdc_val || cplus_val)
                    && str_eq( identifier, "__VA_ARGS__")) {
                cerror( illeg_parm, parlist[ nargs], 0L, 0);
                return  FALSE;
                /* __VA_ARGS__ should not be used as a parameter    */
            }
            if (is_formal( parlist[ nargs], FALSE)) {
                cerror( "Duplicate parameter name \"%s\""   /* _E_  */
                        , parlist[ nargs], 0L, 0);
                return  FALSE;
            }
            parlen[ nargs] = (size_t) (workp - parlist[ nargs]);
                                            /* Save length of param */
            *workp++ = ',';
            nargs++;
        } while ((c = skip_ws()) == ',');   /* Get another parameter*/

        *--workp = EOS;                     /* Remove excessive ',' */
        if (c != ')') {                     /* Must end at )        */
            unget_ch();                     /* Push back '\n'       */
            cerror(
        "Missing \",\" or \")\" in parameter list \"(%s\""  /* _E_  */
                    , work_buf, 0L, 0);
            return  FALSE;
        }
    } else {
        /*
         * DEF_NOARGS is needed to distinguish between
         * "#define foo" and "#define foo()".
         */
        nargs = DEF_NOARGS;                 /* Object-like macro    */
        unget_ch();
    }
ret:
#if NMACPARS > NMACPARS90MIN
    if ((warn_level & 4) && (nargs & ~AVA_ARGS) > n_mac_pars_min)
        cwarn( many_parms, 0 , (long) n_mac_pars_min , 0);
#endif
    return  TRUE;
}

static int  get_repl( const char * macroname)
/*
 *   Get replacement text i.e. names of formal parameters are converted to
 * the magic numbers, and operators #, ## is converted to magic characters.
 *   Return TRUE if replacement list is legal, else return FALSE.
 *   Any token separator in the text is converted to a single space, no token
 * sepatator is inserted by MCPP.  Those are required by the Standard for
 * stringizing of an argument by # operator.
 *   In POST_STD mode, inserts a space between any tokens in source (except a
 * macro name and the next '(' in macro definition), hence presence or absence
 * of token separator makes no difference.
 */
{
    const char * const  mixed_ops
    = "Macro with mixing of ## and # operators isn't portable";     /* _W4_ */
    const char * const  multiple_cats
    = "Macro with multiple ## operators isn't portable";    /* _W4_ */
    char *  prev_token = 0;              /* Preceding token      */
    char *  prev_prev_token = 0;         /* Pre-preceding token  */
    int     multi_cats = FALSE;             /* Multiple ## operators*/
    int     c;
    int     token_type;                     /* Type of token        */
    char *  temp;
    char *  repl_cur = repl_base;   /* Pointer into repl-text buffer*/

    *repl_cur = EOS;
    token_p = 0;
    if (mcpp_mode == STD) {
        c = get_ch();
        unget_ch();
        if (((char_type[ c] & SPA) == 0) && (nargs < 0) && (warn_level & 1))
            cwarn( "No space between macro name \"%s\" and repl-text"/* _W1_ */
                , macroname, 0L, 0);
    }
    c = skip_ws();                           /* Get to the body      */

    while (c != CHAR_EOF && c != '\n') {
        if (standard) {
            prev_prev_token = prev_token;
            prev_token = token_p;
        }
        token_p = repl_cur;                 /* Remember the pointer */
        token_type = scan_token( c, &repl_cur, repl_end);

        switch (token_type) {
        case OPE:                   /* Operator or punctuator       */
            if (! standard)
                break;
            switch (openum) {
            case OP_CAT:                    /* ##                   */
                if (prev_token == 0) {
                    cerror( "No token before ##"            /* _E_  */
                            , 0, 0L, 0);
                    return  FALSE;
                } else if (*prev_token == CAT) {
                    cerror( "## after ##", 0, 0L, 0); /* _E_  */
                    return  FALSE;
                } else if (prev_prev_token && *prev_prev_token == CAT) {
                    multi_cats = TRUE;
                } else if (prev_prev_token && *prev_prev_token == ST_QUOTE
                        && (warn_level & 4)) {      /* # parm ##    */
                    cwarn( mixed_ops, 0, 0L, 0);
                }
                repl_cur = token_p;
                *repl_cur++ = CAT;          /* Convert to CAT       */
                break;
            case OP_STR:                    /* #                    */
                if (nargs < 0)              /* In object-like macro */
                    break;                  /* '#' is an usual char */
                if (prev_token && *prev_token == CAT
                        && (warn_level & 4))        /* ## #         */
                    cwarn( mixed_ops, 0, 0L, 0);
                repl_cur = token_p;         /* Overwrite on #       */
                if ((temp = def_stringization( repl_cur)) == 0) {
                    return  FALSE;          /* Error                */
                } else {
                    repl_cur = temp;
                }
                break;
            default:                    /* Any operator as it is    */
                break;
            }
            break;
        case NAM:
        /*
         * Replace this name if it's a parm.  Note that the macro name is a
         * possible replacement token.  We stuff DEF_MAGIC in front of the
         * token which is treated as a LETTER by the token scanner and eaten
         * by the macro expanding routine.  This prevents the macro expander
         * from looping if someone writes "#define foo foo".
         */
            temp = is_formal( identifier, TRUE);
            if (temp == 0) {             /* Not a parameter name */
                if (! standard)
                    break;
                if ((stdc_val || cplus_val)
                            && str_eq( identifier, "__VA_ARGS__")) {
                    cerror( "\"%s\" without corresponding \"...\""  /* _E_  */
                            , identifier, 0L, 0);
                    return  FALSE;
                }
                if ((temp = mgtoken_save( macroname)) != 0)
                    repl_cur = temp;        /* Macro name           */
            } else {                        /* Parameter name       */
                repl_cur = temp;
#if COMPILER == GNUC
                if (mcpp_mode == STD && str_eq( identifier, "__VA_ARGS__")
                        && prev_token && *prev_token == CAT
                        && prev_prev_token && *prev_prev_token == ',')
                        /* ", ## __VA_ARGS__" sequence              */
                        /* This is a GCC3-specific variadic macro   */
                    nargs |= GVA_ARGS;      /* Mark as GCC3 variadic*/
#endif
            }
            break;

        case STR:                           /* String in mac. body  */
        case CHR:                           /* Character constant   */
            if (mcpp_mode == OLD_PREP)
                repl_cur = str_parm_scan( repl_cur);
            break;
        case SEP:
            if (mcpp_mode == OLD_PREP && c == COM_SEP)
                repl_cur--;                 /* Skip comment now     */
            break;
        default:                            /* Any token as it is   */
            break;
        }

        c = get_ch();
        if (c == ' ') {
            *repl_cur++ = ' ';
            c = get_ch();
        }
    }

    unget_ch();                             /* For syntax check     */
    if (standard) {
        if (token_p && *token_p == CAT) {
            cerror( "No token after ##", 0, 0L, 0);   /* _E_  */
            return  FALSE;
        }
        if (multi_cats && (warn_level & 4))
            cwarn( multiple_cats, 0, 0L, 0);
        if ((nargs & VA_ARGS) && stdc_ver < 199901L && (warn_level & 2))
            /* Variable arg macro is the spec of C99, not C90 nor C++98     */
            cwarn( "Variable argument macro is defined",    /* _W2_ */
                    0, 0L, 0);
    }
    *repl_cur = EOS;                        /* Terminate work       */

    return  TRUE;
}

static char *   is_formal(
    const char *    name,
    int         conv                    /* Convert to magic number? */
)
/*
 * If the identifier is a formal parameter, save the MAC_PARM and formal
 * offset, returning the advanced pointer into the replacement text.
 * Else, return 0.
 */
{
    char *  repl_cur;
    size_t  len;
    int     i;

    len = ACE_OS::strlen( name);
    for (i = 0; i < (nargs & ~AVA_ARGS); i++) {     /* For each parameter   */
        if ((len == parlen[ i]      /* Note: parlist[] are comma separated  */
                    && ACE_OS::memcmp( name, parlist[ i], parlen[ i]) == 0)
                || (standard && (nargs & VA_ARGS)
                    && i == (nargs & ~AVA_ARGS) - 1 && conv
                    && ACE_OS::memcmp( name, "__VA_ARGS__", 12) == 0)) {
                                            /* If it's known        */
            if (conv) {
                repl_cur = token_p;         /* Overwrite on the name*/
                *repl_cur++ = MAC_PARM;     /* Save the signal      */
                *repl_cur++ = i + 1;        /* Save the parm number */
                return  repl_cur;           /* Return "gotcha"      */
            } else {
                return  parlist[ i];        /* Duplicate parm name  */
            }
        }
    }

    return  0;                           /* Not a formal param   */
}

static char *   def_stringization( char * repl_cur)
/*
 * Define token stringization.
 * We store a magic cookie (which becomes " on output) preceding the
 * parameter as an operand of # operator.
 * Return the current pointer into replacement text if the token following #
 * is a parameter name, else return 0.
 */
{
    int     c;
    char *  temp;

    *repl_cur++ = ST_QUOTE;                 /* prefix               */
    if ((c = get_ch()) == ' ') {
        *repl_cur++ = ' ';
        c = get_ch();
    }
    token_p = repl_cur;                     /* Remember the pointer */
    if (scan_token( c, &repl_cur, repl_end) == NAM) {
        if ((temp = is_formal( identifier, TRUE)) != 0) {
            repl_cur = temp;
            return  repl_cur;
        }
    }
    cerror( "Not a formal parameter \"%s\"", token_p, 0L, 0);    /* _E_  */
    return  0;
}

static char *   mgtoken_save( const char * macroname)
/*
 * A magic cookie is inserted if the token is identical to the macro name,
 * so the expansion doesn't recurse.
 * Return the advanced pointer into the replacement text or 0.
 */
{
    char *   repl_cur;

    if (str_eq( macroname, identifier)) {   /* Macro name in body   */
        repl_cur = token_p;                 /* Overwrite on token   */
        *repl_cur++ = DEF_MAGIC;            /* Save magic marker    */
        repl_cur = mcpp_stpcpy( repl_cur, identifier);
                                            /* And save the token   */
        return  repl_cur;
    } else {
        return  0;
    }
}

static char *   str_parm_scan( char * string_end)
/*       FUZZ: disable check_for_lack_ACE_OS
 * String parameter scan.
 * This code -- if enabled -- recognizes a formal parameter in a string
 * literal or in a character constant.
 *      #define foo(bar, v) printf("%bar\n", v)
 *      foo( d, i)
 * expands to:
 *      printf("%d\n", i)
 * str_parm_scan() return the advanced pointer into the replacement text.
 * This has been superceded by # stringizing and string concatenation.
 * This routine is called only in OLD_PREP mode.
 */       //FUZZ: enable check_for_lack_ACE_OS
{
    int     delim;
    int     c;
    char *  tp;
    char *  wp;             /* Pointer into the quoted literal  */

    delim = *token_p;
    unget_string( ++token_p, 0);
    /* Pseudo-token-parsing in a string literal */
    wp = token_p;
    while ((c = get_ch()) != delim) {
        token_p = wp;
        if (scan_token( c, &wp, string_end) != NAM)
            continue;
        if ((tp = is_formal( token_p, TRUE)) != 0)
            wp = tp;
    }
    *wp++ = delim;
    return  wp;
}

static void do_undef( void)
/*
 * Remove the symbol from the defined list.
 * Called from directive().
 */
{
    DEFBUF *    defp;
    int     c;

    if ((c = skip_ws()) == '\n') {
        cerror( no_ident, 0, 0L, 0);
        unget_ch();
        return;
    }
    if (scan_token( c, (workp = work_buf, &workp), work_end) != NAM) {
        cerror( not_ident, work_buf, 0L, 0);
        skip_nl();
        unget_ch();
    } else {
        if ((defp = look_id( identifier)) == 0) {
            if (warn_level & 8)
                cwarn( "\"%s\" wasn't defined"              /* _W8_ */
                        , identifier, 0L, 0);
        } else if (standard 
                && (defp->nargs < DEF_NOARGS - 1        /* Standard predef  */
                    || defp->nargs == DEF_PRAGMA)) {
                                        /* _Pragma() pseudo-macro   */
            cerror( "\"%s\" shouldn't be undefined"         /* _E_  */
                    , identifier, 0L, 0);
        } else if (standard) {
            c = skip_ws();
            unget_ch();
            if (c != '\n')                      /* Trailing junk    */
                return;
            else
                undefine( identifier);
        } else {
            undefine( identifier);
        }
    }
}

/*
 *                  C P P   S y m b o l   T a b l e s
 *
 * SBSIZE defines the number of hash-table slots for the symbol table.
 * It must be a power of 2.
 */

/* Symbol table queue headers.  */
static DEFBUF *     symtab[ SBSIZE];
static long         num_of_macro = 0;

#if MCPP_LIB
void    init_directive( void)
/* Initialize static variables. */
{
    num_of_macro = 0;
}
#endif

DEFBUF *    look_id( const char * name)
/*
 * Look for the identifier in the symbol table.
 * If found, return the table pointer;  Else return 0.
 */
{
    DEFBUF **   prevp;
    int         cmp;

    prevp = look_prev( name, &cmp);

    if (standard)
        return ((cmp == 0 && (*prevp)->push == 0) ? *prevp : 0);
    else
        return ((cmp == 0) ? *prevp : 0);
}

DEFBUF **   look_prev(
    const char *    name,                   /* Name of the macro    */
    int *   cmp                             /* Result of comparison */
)
/*
 * Look for the place to insert the macro definition.
 * Return a pointer to the previous member in the linked list.
 */
{
    const char *    np;
    DEFBUF **   prevp;
    DEFBUF *    dp;
    size_t      s_name;
    int         hash;

    for (hash = 0, np = name; *np != EOS; )
        hash += *np++;
    hash += s_name = (size_t)(np - name);
    s_name++;
    prevp = & symtab[ hash & SBMASK];
    *cmp = -1;                              /* Initialize           */

    while ((dp = *prevp) != 0) {
        if ((*cmp = ACE_OS::memcmp( dp->name, name, s_name)) >= 0)
            break;
        prevp = &dp->link;
    }

    return  prevp;
}

DEFBUF *    look_and_install(
    const char *    name,                   /* Name of the macro    */
    int     numargs,                        /* The numbers of parms */
    const char *    parmnames,  /* Names of parameters concatenated */
    const char *    repl                    /* Replacement text     */
)
/*
 * Look for the name and (re)define it.
 * Returns a pointer to the definition block.
 * Returns 0 if the symbol was Standard-predefined.
 */
{
    DEFBUF **   prevp;          /* Place to insert definition       */
    DEFBUF *    defp;                       /* New definition block */
    int         cmp;    /* Result of comparison of new name and old */

    prevp = look_prev( name, &cmp);
    defp = install_macro( name, numargs, parmnames, repl, prevp, cmp);
    return  defp;
}

DEFBUF *    install_macro(
    const char *    name,                   /* Name of the macro    */
    int     numargs,                        /* The numbers of parms */
    const char *    parmnames,  /* Names of parameters concatenated */
    const char *    repl,                   /* Replacement text     */
    DEFBUF **  prevp,           /* The place to insert definition   */
    int     cmp         /* Result of comparison of new name and old */
)
/*
 * Enter this name in the lookup table.
 * Returns a pointer to the definition block.
 * Returns 0 if the symbol was Standard-predefined.
 */
{
    DEFBUF *    dp;
    DEFBUF *    defp;
    size_t      s_name, s_parmnames, s_repl;

    defp = *prevp;
    if (cmp == 0 && defp->nargs < DEF_NOARGS - 1)
        return  0;                       /* Standard predefined  */
    s_parmnames = 0;
    if (parmnames == 0 || repl == 0)      /* Shouldn't happen */
        cfatal( "Bug: Illegal macro installation of \"%s\"" /* _F_  */
                , name, 0L, 0);      /* Use "" instead of 0   */
    s_name = ACE_OS::strlen( name);
    if (mcpp_mode == STD)
        s_parmnames = ACE_OS::strlen( parmnames) + 1;
    s_repl = ACE_OS::strlen( repl) + 1;
    dp = (DEFBUF *)
        xmalloc( sizeof (DEFBUF) + s_name + s_parmnames + s_repl);
    if (cmp || (standard && (*prevp)->push)) {  /* New definition   */
        dp->link = defp;                /* Insert to linked list    */
        *prevp = dp;
    } else {                            /* Redefinition             */
        dp->link = defp->link;          /* Replace old def with new */
        *prevp = dp;
        ACE_OS::free( defp);
    }
    dp->nargs = numargs;
    if (standard) {
        dp->push = 0;
        dp->parmnames = (char *)dp + sizeof (DEFBUF) + s_name;
        dp->repl = dp->parmnames + s_parmnames;
        if (mcpp_mode == STD)
          ACE_OS::memcpy( dp->parmnames, parmnames, s_parmnames);
    } else {
        dp->repl = (char *)dp + sizeof (DEFBUF) + s_name;
    }
    ACE_OS::memcpy( dp->name, name, s_name + 1);
    ACE_OS::memcpy( dp->repl, repl, s_repl);
    /* Remember where the macro is defined  */
    dp->dir = *inc_dirp;
    dp->fname = cur_fname;
    dp->mline = src_line;
    if (standard && cmp && ++num_of_macro == n_macro_min + 1 && n_macro_min
            && (warn_level & 4))
        cwarn( "More than %.0s%ld macros defined"           /* _W4_ */
                , 0 , n_macro_min , 0);
    return  dp;
}

int undefine(
    const char *  name                      /* Name of the macro    */
)
/*
 * Delete the macro definition from the symbol table.
 * Returns TRUE, if deleted;
 * Else returns FALSE (when the macro was not defined or was predefined).
 */
{
    DEFBUF **   prevp;          /* Preceding definition in list     */
    DEFBUF *    dp;                     /* Definition to delete     */
    int         cmp;

    prevp = look_prev( name, &cmp);
    dp = *prevp;                        /* Definition to delete     */
    if (cmp || dp->nargs < DEF_NOARGS - 1)
        return  FALSE;                      /* Standard predefined  */
    if (standard && dp->push)
        return  FALSE;                  /* 'Pushed' macro           */
    *prevp = dp->link;          /* Link the previous and the next   */
    ACE_OS::free( dp);                          /* Delete the definition    */
    if (standard)
        num_of_macro--;
    return  TRUE;
}

static void dump_repl( 
    const DEFBUF *  dp,
    FILE *  fp
)
/*
 * Dump replacement text.
 */
{
    int     numargs = dp->nargs;
    char *  cp1;
    size_t  i;
    int     c;
    const char *    cp;

    for (cp = dp->repl; (c = *cp++ & UCHARMAX) != EOS; ) {

        switch (c) {
        case MAC_PARM:                              /* Parameter    */
            c = (*cp++ & UCHARMAX) - 1;
            if (standard) {
                if ((numargs & VA_ARGS) && c == (numargs & ~AVA_ARGS) - 1) {
                    mcpp_fputs( "__VA_ARGS__", FP2DEST( fp));
                } else {
                    if (mcpp_mode == STD) {
                        for (i = 0, cp1 = parlist[ c]; i < parlen[ c]; i++)
                            mcpp_fputc( *cp1++, FP2DEST( fp));
                    } else {
                        mcpp_fputc( 'a' + c % 26, FP2DEST( fp));
                        if (c > 26)
                            mcpp_fputc( '0' + c / 26, FP2DEST( fp));
                    }
                }
            } else {
                mcpp_fputc( 'a' + c % 26, FP2DEST( fp));
                if (c > 26)
                    mcpp_fputc( '0' + c / 26, FP2DEST( fp));
            }
            break;
        case DEF_MAGIC:
            if (! standard)
                mcpp_fputc( c, FP2DEST( fp));
            /* Else skip    */
            break;
        case CAT:
            if (standard)
                mcpp_fputs( "##", FP2DEST( fp));
            else
                mcpp_fputc( c, FP2DEST( fp));
            break;
        case ST_QUOTE:
            if (standard)
                mcpp_fputs( "#", FP2DEST( fp));
            else
                mcpp_fputc( c, FP2DEST( fp));
            break;
        case COM_SEP:
            /*
             * Though TOK_SEP coincides to COM_SEP, this cannot appear in
             * Standard mode.
             */
            if (mcpp_mode == OLD_PREP)
                mcpp_fputs( "/**/", FP2DEST( fp));
            break;
        default:
            mcpp_fputc( c, FP2DEST( fp));
            break;
        }
    }
}

/*
 * If the compiler is so-called "one-pass" compiler, compiler-predefined
 * macros are commented out to avoid redefinition.
 */
#if ONE_PASS
#define CAN_REDEF   DEF_NOARGS
#else
#define CAN_REDEF   (DEF_NOARGS - 1)
#endif

void    dump_a_def(
    const char *    why,
    const DEFBUF *  dp,
    int     newdef,         /* TRUE if parmnames are currently in parlist[] */
    int     dDflag,         /* TRUE if -dD option is used (for GCC)         */
    int     comment,        /* Show location of the definition in comment   */
    FILE *  fp
)
/*
 * Dump a macro definition.
 */
{
    char *  cp, * cp1;
    int     numargs = dp->nargs & ~AVA_ARGS;
    int     commented;                      /* To be commented out  */
    int     i;

    if (standard && numargs == DEF_PRAGMA)  /* _Pragma pseudo-macro */
        return;
    if ((numargs < CAN_REDEF) || (dDflag && (numargs == DEF_NOARGS - 1))
            || (standard && dp->push))
        commented = TRUE;
    else
        commented = FALSE;
    if (! comment && commented)             /* For -dM option       */
        return;
    if (why)
        mcpp_fprintf( FP2DEST( fp), "%s \"%s\" defined as: ", why, dp->name);
    mcpp_fprintf( FP2DEST( fp), "%s#define %s", commented ? "/* " : "",
            dp->name);                      /* Macro name           */
    if (numargs >= 0) {                     /* Parameter list       */
        if (mcpp_mode == STD) {
            mcpp_fprintf( FP2DEST( fp), "(%s)", dp->parmnames);
            if (! newdef) {
                for (i = 0, cp = dp->parmnames; i < numargs;
                        i++, cp = cp1 + 1) {
                    if ((cp1 = ACE_OS::strchr( cp, ',')) == 0)   /* The last arg */
                        parlen[ i] = ACE_OS::strlen( cp);
                    else
                        parlen[ i] = (size_t) (cp1 - cp);
                    parlist[ i] = cp;
                }
            }
        } else {
            if (newdef) {
                mcpp_fprintf( FP2DEST( fp), "(%s)", parlist[0]);
            } else if (numargs == 0) {
                mcpp_fputs( "()", FP2DEST( fp));
            } else {
                mcpp_fputc( '(', FP2DEST( fp));
                for (i = 0; i < numargs; i++) {     /* Make parameter list  */
                    mcpp_fputc( 'a' + i % 26, FP2DEST( fp));
                    if (i >= 26)
                        mcpp_fputc( '0' + i / 26, FP2DEST( fp));
                    if (i + 1 < numargs)
                        mcpp_fputc( ',', FP2DEST( fp));
                }
                mcpp_fputc( ')', FP2DEST( fp));
            }
        }
    }
    if (*dp->repl) {
        mcpp_fputc( ' ', FP2DEST( fp));
        dump_repl( dp, fp);                 /* Replacement text     */
    }
    if (commented)
            /* Standard predefined or one-pass-compiler-predefined  */
        mcpp_fputs( " */", FP2DEST( fp));
    if (comment)                            /* Not -dM option       */
        mcpp_fprintf( FP2DEST( fp), " \t/* %s%s:%ld\t*/", dp->dir, dp->fname
                , dp->mline);
    mcpp_fputc( '\n', FP2DEST( fp));
}

void    dump_def(
    int     dDflag,                 /* -dD option (for GCC)         */
    int     comment         /* Location of definition in comment    */
)
/*
 * Dump all the current macro definitions to output stream.
 */
{
    DEFBUF *    dp;
    DEFBUF **   symp;

    sharp();            /* Report the current source file & line    */
    if (comment)
        mcpp_fputs( "/* Currently defined macros. */\n", OUT);
    for (symp = symtab; symp < &symtab[ SBSIZE]; symp++) {
        if ((dp = *symp) != 0) {
            do {
                dump_a_def( 0, dp, FALSE, dDflag, comment, fp_out);
            } while ((dp = dp->link) != 0);
        }
    }
    wrong_line = TRUE;               /* Line number is out of sync  */
}

#if MCPP_LIB
void    clear_symtable( void)
/*
 * ACE_OS::Free all the macro definitions.
 */
{
    DEFBUF *    next;
    DEFBUF *    dp;
    DEFBUF **   symp;

    for (symp = symtab; symp < &symtab[ SBSIZE]; symp++) {
        for (next = *symp; next != 0; ) {
            dp = next;
            next = dp->link;
            ACE_OS::free( dp);                      /* free the symbol      */
        }
        *symp = 0;
    }
}
#endif

