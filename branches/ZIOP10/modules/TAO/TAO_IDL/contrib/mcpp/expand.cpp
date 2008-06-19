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
 *                          E X P A N D . C
 *                  M a c r o   E x p a n s i o n
 *
 * The macro expansion routines are placed here.
 */

#if PREPROCESSED
#include    "mcpp.H"
#else
#include    "system.H"
#include    "internal.H"
#endif

#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_stdio.h"

#define ARG_ERROR   (-255)
#define CERROR      1
#define CWARN       2

static char *   expand_std( DEFBUF * defp, char * out, char * out_end);
                /* Expand a macro completely (for Standard modes)   */
static char *   expand_prestd( DEFBUF * defp, char * out, char * out_end);
                /* Expand a macro completely (for pre-Standard modes)       */
static DEFBUF * is_macro_call( DEFBUF * defp, char ** cp);
                /* Is this really a macro call ?    */
static int      collect_args( const DEFBUF * defp, char ** arglist);
                /* Collect arguments of a macro call*/
static int      get_an_arg( int c, char ** argpp, char * arg_end
        , char ** seqp, int var_arg);
                /* Get an argument                  */
static int      squeeze_ws( char ** out);
                /* Squeeze white spaces to a space  */
static void     skip_macro( void);
                /* Skip the rest of macro call      */
static void     diag_macro( int severity, const char * format
        , const char * arg1, long arg2, const char * arg3, const DEFBUF * defp1
        , const DEFBUF * defp2) ;
                /* Supplement diagnostic information*/
static void     dump_args( const char * why, int nargs, char ** arglist);
                /* Dump arguments list              */

static int      rescan_level;       /* Times of macro rescan    */
static const char * const   macbuf_overflow
        = "Buffer overflow expanding macro \"%s\" at %.0ld\"%s\"";  /* _E_  */
static const char * const   empty_arg
        = "Empty argument in macro call \"%s\"";            /* _W2_ */
static const char * const   unterm_macro
        = "Unterminated macro call \"%s\"";                 /* _E_  */
static const char * const   narg_error
    = "%s than necessary %ld argument(s) in macro call \"%s\""; /* _E_ _W1_ */
static const char * const   only_name
        = "Macro \"%s\" needs arguments";                   /* _W8_ */

void     expand_init( void)
/* Set expand_macro() function  */
{
    expand_macro = standard ? expand_std : expand_prestd;
}

DEFBUF *    is_macro(
    char **     cp
)
/*
 * The name is already in 'identifier', the next token is not yet read.
 * Return TRUE if the name is a macro call, else return FALSE.
 */
{
    DEFBUF *    defp;

    if ((defp = look_id( identifier)) != 0)  /* Is a macro name  */
        return  is_macro_call( defp, cp);
    else
        return  0;
}

static DEFBUF * is_macro_call(
    DEFBUF *    defp,
    char **     cp
)
/*
 * Return TRUE if the defp->name is a macro call, else return FALSE.
 */
{
    int    c;

    if (defp->nargs >= 0                    /* Function-like macro  */
            || defp->nargs == DEF_PRAGMA) { /* _Pragma() pseudo-macro       */
        c = squeeze_ws( cp);                /* See the next char.   */
        if (c == CHAR_EOF) {                /* End of file          */
            unget_string( "\n", 0);      /* Restore skipped '\n' */
        } else if (! standard || c != RT_END) {
                        /* Still in the file and rescan boundary ?  */
            unget_ch();                     /* To see it again      */
        }
        if (c != '(') {     /* Only the name of function-like macro */
            if (! standard && warn_level & 8)
                cwarn( only_name, defp->name, 0L, 0);
            return  0;
        }
    }
    return  defp;                           /* Really a macro call  */
}

/*
 * expand_macro()   expands a macro call completely, and writes out the result
 *      to the specified output buffer and returns the advanced pointer.
 */


/*
 *          T h e   S T A N D A R D   C o n f o r m i n g   M o d e
 *                  o f   M a c r o   E x p a n s i o n
 *
 * 1998/08      First released.     kmatsui
 */

/* Return value of is_able_repl()   */
#define NO          0               /* "Blue-painted"               */
#define YES         1               /* Not blue-painted             */
#define READ_OVER   2
            /* Still "blue-painted", yet has read over repl-list    */

static struct {
    const DEFBUF *  def;            /* Macro definition             */
    int             read_over;      /* Has read over repl-list      */
    /* 'read_over' is never used in POST_STD mode and in compat_mode*/
} replacing[ RESCAN_LIMIT];         /* Macros currently replacing   */

static char *   replace( DEFBUF * defp, char * out, char * out_end
        , const DEFBUF * outer, FILEINFO * rt_file);
                /* Replace a macro recursively      */
static DEFBUF * def_special( DEFBUF * defp);
                /* Re-define __LINE__, __FILE__     */
static int      prescan( const DEFBUF * defp, char ** arglist, char * out
        , char * out_end);
                /* Process #, ## operator           */
static char *   catenate( const DEFBUF * defp, char ** arglist, char * out
        , char * out_end, char ** token_p);
                /* Catenate tokens                  */
static char *   stringize( const DEFBUF * defp, char * argp, char * out);
                /* Stringize an argument            */
static char *   substitute( const DEFBUF * defp, int gvar_arg, char ** arglist
        , const char * in, char * out, char * out_end);
                /* Substitute parms with arguments  */
static char *   rescan( const DEFBUF * outer, const char * in, char * out
        , char * out_end);
                /* Rescan once replaced sequences   */
static int      disable_repl( const DEFBUF * defp);
                /* Disable the macro once replaced  */
static void     enable_repl( const DEFBUF * defp, int done);
                /* Enable the macro for later use   */
static int      is_able_repl( const DEFBUF * defp);
                /* Is the macro allowed to replace? */

static char *   expand_std(
    DEFBUF *    defp,                       /* Macro definition     */
    char *  out,                            /* Output buffer        */
    char *  out_end                         /* End of output buffer */
)
/*
 * Expand a macro call completely, write the results to the specified buffer
 * and return the advanced pointer.
 */
{
    char    macrobuf[ NMACWORK + IDMAX];    /* Buffer for replace() */
    char *  out_p = out;
    size_t  len;
    int     c, c1;
    char *  cp;

    macro_line = src_line;                      /* Line number for diag */
    macro_name = defp->name;
    rescan_level = 0;
    if (replace( defp, macrobuf, macrobuf + NMACWORK, 0
            , infile) == 0) {            /* Illegal macro call   */
        skip_macro();
        macro_line = MACRO_ERROR;
        goto  exp_end;
    }
    len = (size_t) (out_end - out);
    if (ACE_OS::strlen( macrobuf) > len) {
        cerror( macbuf_overflow, macro_name, 0, macrobuf);
        ACE_OS::memcpy( out, macrobuf, len);
        out_p = out + len;
        macro_line = MACRO_ERROR;
        goto  exp_end;
    }

    cp = macrobuf;
    c1 = '\0';
    while ((c = *cp++) != EOS) {
        if (c == DEF_MAGIC)
            continue;                       /* Skip DEF_MAGIC       */
        if (mcpp_mode == STD) {
            if (c == IN_SRC)
                continue;                   /* Skip IN_SRC          */
            else if (c == TOK_SEP) {
                if (c1 == ' ' || in_include || lang_asm)
                    continue;
                    /* Skip separator just after ' ' and in #include line   */
                    /* Also skip this in lang_asm mode              */
                else
                    c = ' ';
            }
        }
        c1 = c;
        *out_p++ = c;
    }

    macro_line = 0;
exp_end:
    *out_p = EOS;
    if (mcpp_debug & EXPAND)
        dump_string( "expand_std exit", out);
    macro_name = 0;
    clear_exp_mac();        /* Clear the information for diagnostic */
    return  out_p;
}

static char *   replace(
    DEFBUF *    defp,                       /* Macro to be replaced */
    char *      out,                        /* Output Buffer        */
    char *      out_end,                    /* End of output buffer */
    const DEFBUF *    outer,                /* Outer macro replacing*/
    FILEINFO *  rt_file                     /* Repl-text "file"     */
)
/*
 * Replace a possibly nested macro recursively.
 * replace() and rescan() call each other recursively.
 * Return the advanced output pointer or 0 on error.
 */
{
    char ** arglist = 0;                 /* Pointers to arguments*/
    int     nargs;                  /* Number of arguments expected */
    char *  catbuf;                         /* Buffer for prescan() */
    char *  expbuf;                 /* Buffer for  substitute()     */
    char *  out_p;                          /* Output pointer       */

    if (mcpp_debug & EXPAND) {
        dump_a_def( "replace entry", defp, FALSE, FALSE, TRUE, fp_debug);
        dump_unget( "replace entry");
    }
    nargs = (defp->nargs == DEF_PRAGMA) ? 1 : (defp->nargs & ~AVA_ARGS);

    if (nargs < DEF_NOARGS - 2) {           /* __FILE__, __LINE__   */
        defp = def_special( defp);
        if (mcpp_mode == STD) {
            *out++ = TOK_SEP;       /* Wrap repl-text with token    */
            out = stpcpy( out, defp->repl); /*   separators to pre- */
            *out++ = TOK_SEP;               /*   vent token merging.*/
            *out = EOS;
        } else {
            out = stpcpy( out, defp->repl);
        }
        return  out;
    } else if (nargs >= 0) {                /* Function-like macro  */
        squeeze_ws( 0);                  /* Skip to '('          */
        arglist = (char **) xmalloc( (nargs + 1) * sizeof (char *));
        arglist[ 0] = xmalloc( (size_t) (NMACWORK + IDMAX * 2));
                            /* Note: arglist[ n] may be reallocated */
                            /*   and re-written by collect_args()   */
        if (collect_args( defp, arglist) == ARG_ERROR) {
            ACE_OS::free( arglist[ 0]);             /* Syntax error         */
            ACE_OS::free( arglist);
            return  0;
        }
        if (mcpp_mode == STD && outer && rt_file != infile) {
                                 /* Has read over replacement-text  */
            if (compat_mode) {
                enable_repl( outer, FALSE); /* Enable re-expansion  */
                if (mcpp_debug & EXPAND)
                    dump_string( "enabled re-expansion"
                            , outer ? outer->name : "<arg>");
            } else {
                replacing[ rescan_level-1].read_over = READ_OVER;
            }
        }
    }

    catbuf = xmalloc( (size_t) (NMACWORK + IDMAX));
    if (mcpp_debug & EXPAND) {
        mcpp_fprintf( DBG, "(%s)", defp->name);
        dump_string( "prescan entry", defp->repl);
    }
    if (prescan( defp, arglist, catbuf, catbuf + NMACWORK) == FALSE) {
                                    /* Process #, ## operators      */
        diag_macro( CERROR, macbuf_overflow, defp->name, 0L, catbuf, defp
                , 0);
        if (nargs >= 0) {
            ACE_OS::free( arglist[ 0]);
            ACE_OS::free( arglist);
        }
        ACE_OS::free( catbuf);
        return  0;
    }
    catbuf = xrealloc( catbuf, ACE_OS::strlen( catbuf) + 1);
                                            /* Use memory sparingly */
    if (mcpp_debug & EXPAND) {
        mcpp_fprintf( DBG, "(%s)", defp->name);
        dump_string( "prescan exit", catbuf);
    }

    if (nargs > 0) {    /* Function-like macro with any argument    */
        int     gvar_arg;
        expbuf = xmalloc( (size_t) (NMACWORK + IDMAX));
        if (mcpp_debug & EXPAND) {
            mcpp_fprintf( DBG, "(%s)", defp->name);
            dump_string( "substitute entry", catbuf);
        }
        gvar_arg = (defp->nargs & GVA_ARGS) ? (defp->nargs & ~AVA_ARGS) : 0;
        out_p = substitute( defp, gvar_arg, arglist, catbuf, expbuf
                , expbuf + NMACWORK);       /* Expand each arguments*/
        ACE_OS::free( arglist[ 0]);
        ACE_OS::free( arglist);
        ACE_OS::free( catbuf);
        expbuf = xrealloc( expbuf, ACE_OS::strlen( expbuf) + 1);
                                            /* Use memory sparingly */
        if (mcpp_debug & EXPAND) {
            mcpp_fprintf( DBG, "(%s)", defp->name);
            dump_string( "substitute exit", expbuf);
        }
    } else {                                /* Object-like macro or */
        if (nargs == 0) {   /* Function-like macro with no argument */
            ACE_OS::free( arglist[ 0]);
            ACE_OS::free( arglist);
        }
        out_p = expbuf = catbuf;
    }

    if (out_p)
        out_p = rescan( defp, expbuf, out, out_end);
    if (out_p && defp->nargs == DEF_PRAGMA)
        has_pragma = TRUE;
        /* Inform mcpp_main() that _Pragma() was found  */
    ACE_OS::free( expbuf);
    return  out_p;
}

static DEFBUF * def_special(
    DEFBUF *    defp                        /* Macro definition     */
)
/*
 * Re-define __LINE__, __FILE__.
 * Return the new definition.
 */
{
    const FILEINFO *    file;
    DEFBUF **   prevp;
    int         cmp;

    switch (defp->nargs) {
    case DEF_NOARGS - 3:                    /* __LINE__             */
        if ((src_line > line_limit || src_line <= 0) && (warn_level & 1))
            diag_macro( CWARN
                    , "Line number %.0s\"%ld\" is out of range"     /* _W1_ */
                    , 0, src_line, 0, defp, 0);
        ACE_OS::sprintf( defp->repl, "%ld", src_line);      /* Re-define    */
        break;
    case DEF_NOARGS - 4:                    /* __FILE__             */
        for (file = infile; file != 0; file = file->parent) {
            if (file->fp != 0) {
                ACE_OS::sprintf( work_buf, "\"%s%s\"", *(file->dirp), file->filename);
                if (str_eq( work_buf, defp->repl))
                    break;                          /* No change    */
                defp->nargs = DEF_NOARGS;   /* Enable to redefine   */
                prevp = look_prev( defp->name, &cmp);
                defp = install_macro( "__FILE__", DEF_NOARGS - 4, "", work_buf
                        , prevp, cmp);      /* Re-define            */
                break;     
            }
        }
        break;
    }
    return  defp;
}

static int  prescan(
    const DEFBUF *  defp,           /* Definition of the macro      */
    char **     arglist,            /* Pointers to actual arguments */
    char *      out,                /* Output buffer                */
    char *      out_end             /* End of output buffer         */
)
/*
 * Concatenate the tokens surounding ## by catenate(), and stringize the
 * argument following # by stringize().
 */
{
    FILEINFO *  file;
    char *      prev_token = 0;  /* Preceding token              */
    int         c;                  /* Value of a character         */
    /*
     * The replacement lists are --
     *          stuff1<SEP>stuff2
     *      or  stuff1<SEP>stuff2<SEP>stuff3...
     * where <SEP> is CAT, maybe with preceding space and following space,
     * stuff might be
     *          ordinary-token
     *          MAC_PARM<n>
     *      or  <QUO>MAC_PARM<n>
     * where <QUO> is ST_QUO, possibly with following space.
     */

    if (mcpp_mode == POST_STD) {
        file = unget_string( defp->repl, defp->name);
    } else {
        *out++ = TOK_SEP;                   /* Wrap replacement     */
        workp = work_buf;                   /*  text with token     */
        workp = stpcpy( workp, defp->repl); /*   separators to      */
        *workp++ = TOK_SEP;                 /*    prevent unintended*/
        *workp = EOS;                       /*     token merging.   */
        file = unget_string( work_buf, defp->name);
    }

    while (c = get_ch(), file == infile) {  /* To the end of repl   */

        switch (c) {
        case ST_QUOTE:
            skip_ws();                      /* Skip space, MAC_PARM */
            c = get_ch() - 1;               /* Parameter number     */
            prev_token = out;               /* Remember the token   */
            out = stringize( defp, arglist[ c], out);
                                    /* Stringize without expansion  */
            break;
        case CAT:
            if (*prev_token == DEF_MAGIC || *prev_token == IN_SRC) {
                ACE_OS::memmove( prev_token, prev_token + 1, ACE_OS::strlen( prev_token + 1));
                *--out = EOS;           /* Remove DEF_MAGIC, IN_SRC */
            }
#if COMPILER == GNUC
            if (*prev_token == ',')
                break;      /* ', ##' sequence (peculiar to GCC)    */
#endif
            out = catenate( defp, arglist, out, out_end, &prev_token);
            break;
        case MAC_PARM:
            prev_token = out;
            *out++ = MAC_PARM;
            *out++ = get_ch();              /* Parameter number     */
            break;
        case TOK_SEP:
        /* Fall through */
        case ' ':
            *out++ = c;
            break;
        default:
            prev_token = out;
            scan_token( c, &out, out_end);  /* Ordinary token       */
            break;
        }

        *out = EOS;                         /* Ensure termination   */
        if (out_end <= out)                 /* Buffer overflow      */
            return  FALSE;
    }

    *out = EOS;         /* Ensure terminatation in case of no token */
    unget_ch();
    return  TRUE;
}

static char *   catenate(
    const DEFBUF *  defp,           /* The macro definition         */
    char ** arglist,                /* Pointers to actual arguments */
    char *  out,                    /* Output buffer                */
    char *  out_end,                /* End of output buffer         */
    char ** token_p         /* Address of preceding token pointer   */
)
/*
 * Concatenate the previous and the following tokens.
 *   Note: The parameter codes may coincide with white spaces or any
 * other characters.
 */
{
    FILEINFO *      file;
    char *  prev_prev_token = 0;
    const char *    invalid_token
    = "Not a valid preprocessing token \"%s\"";     /* _E_ _W2_     */
    const char *    argp;           /* Pointer to an actual argument*/
    char *  prev_token = *token_p;  /* Preceding token              */
    int     in_arg = FALSE;
    int     c;                      /* Value of a character         */

    while ((*(out - 1) == ' ' || *(out - 1) == TOK_SEP)
            && (*(out - 2) != MAC_PARM || prev_token != out - 2))
        out--;          /* Remove spaces and inserted separators    */
    *out = EOS;

    /* Get the previous token   */
    if (*prev_token == MAC_PARM) {          /* Formal parameter     */
        c = (*(prev_token + 1) & UCHARMAX) - 1;     /* Parm number  */
        argp = arglist[ c];                 /* Actual argument      */
        out = prev_token;                   /* To overwrite         */
        if ((mcpp_mode == POST_STD && *argp == EOS)
                || (mcpp_mode == STD && *argp == RT_END)) {
            *out = EOS;                     /* An empty argument    */
        } else {
            if (mcpp_mode == POST_STD) {
                file = unget_string( argp, 0);
                while (c = get_ch(), file == infile) {
                    prev_token = out;   /* Remember the last token      */
                    scan_token( c, &out, out_end);
                }           /* Copy actual argument without expansion   */
                unget_ch();
            } else {
                unget_string( argp, 0);
                while ((c = get_ch()) != RT_END) {
                    prev_prev_token = prev_token;
                    prev_token = out;   /* Remember the last token      */
                    scan_token( c, &out, out_end);
                }           /* Copy actual argument without expansion   */
                if (*prev_token == TOK_SEP) {
                    out = prev_token;
                    prev_token = prev_prev_token;   /* Skip separator   */
                }
            }
            if (*prev_token == DEF_MAGIC 
                    || (mcpp_mode == STD && *prev_token == IN_SRC)) {
                ACE_OS::memmove( prev_token, prev_token + 1
                        , (size_t) (out-- - prev_token));
            /* Remove DEF_MAGIC enabling the name to replace later  */
            }
        }
    }
    c = skip_ws();

    /* Catenate */
    switch (c) {
    case ST_QUOTE:          /* First stringize and then catenate    */
        skip_ws();                  /* Skip MAC_PARM, ST_QUOTE      */
        c = get_ch() - 1;
        out = stringize( defp, arglist[ c], out);
        break;
    case MAC_PARM:
        c = get_ch() - 1;                   /* Parameter number     */
        argp = arglist[ c];                 /* Actual argument      */
        if ((mcpp_mode == POST_STD && *argp == EOS)
                || (mcpp_mode == STD && *argp == RT_END))
            *out = EOS;                     /* An empty argument    */
        else {
            unget_string( argp, 0);
            if ((c = get_ch()) == DEF_MAGIC)    /* Remove DEF_MAGIC */
                c = get_ch();               /*  enabling to replace */
            else if (c == IN_SRC)           /* Remove IN_SRC        */
                c = get_ch();
            scan_token( c, &out, out_end);  /* The first token      */
            if (*infile->bptr)              /* There are more tokens*/
                in_arg = TRUE;
        }
        break;
    case DEF_MAGIC:
    case IN_SRC:
        c = get_ch();                   /* Skip DEF_MAGIC, IN_SRC   */
        /* Fall through */
    default:
        scan_token( c, &out, out_end);      /* Copy the token       */
        break;
    }

    /* The generated sequence is a valid preprocessing-token ?      */
    if (*prev_token) {                      /* There is any token   */
        unget_string( prev_token, 0);    /* Scan once more       */
        c = get_ch();  /* This line should be before the next line. */
        infile->fp = (FILE *)-1;            /* To check token length*/
        if (mcpp_debug & EXPAND)
            dump_string( "checking generated token", infile->buffer);
        scan_token( c, (workp = work_buf, &workp), work_end);
        infile->fp = 0;
        if (*infile->bptr != EOS) {         /* More than a token    */
            if (lang_asm) {                 /* Assembler source     */
                if (warn_level & 2)
                    diag_macro( CWARN, invalid_token, prev_token, 0L, 0
                            , defp, 0);
            } else {
                diag_macro( CERROR, invalid_token, prev_token, 0L, 0, defp
                       , 0);
            }
            infile->bptr += ACE_OS::strlen( infile->bptr);
        }
        get_ch();                           /* To the parent "file" */
        unget_ch();
    }

    if (mcpp_mode == STD && ! lang_asm) {
        *out++ = TOK_SEP;                   /* Prevent token merging*/
        *out = EOS;
    }
    if (in_arg) {
        if (mcpp_mode == POST_STD) {
            file = infile;
            while (c = get_ch(), file == infile) {
                prev_token = out;       /* Remember the last token  */
                scan_token( c, &out, out_end);
            }           /* Copy rest of argument without expansion  */
            unget_ch();
        } else {
            while ((c = get_ch()) != RT_END) {
                prev_token = out;       /* Remember the last token  */
                scan_token( c, &out, out_end);
            }           /* Copy rest of argument without expansion  */
        }
    }
    *token_p = prev_token;      /* Report back the generated token  */

    return  out;
}

static char *   stringize(
    const DEFBUF *  defp,                   /* The macro definition */
    char *      argp,                       /* Pointer to argument  */
    char *      out                         /* Output buffer        */
)
/*
 * Make a string literal from an argument.
 */
{
    FILEINFO *  file;
    int         stray_bsl = FALSE;          /* '\\' not in literal  */
    char *      out_p = out;
    int         token_type;
    int         c;

    *out_p++ = '"';

    file = unget_string( argp, 0);

    while ((c = get_ch()), ((mcpp_mode == POST_STD && file == infile)
            || (mcpp_mode == STD && c != RT_END))) {
        if (c == ' ') {
            *out_p++ = ' ';
            continue;
        }
        if (mcpp_mode == STD && (c == TOK_SEP || c == IN_SRC))
            continue;   /* Skip inserted separator and in-src magic */
        if (c == '\\')
            stray_bsl = TRUE;               /* May cause a trouble  */
        token_type = scan_token( c, (workp = work_buf, &workp), work_end);

        switch (token_type) {
        case WSTR:
        case WCHR:
        case STR:
        case CHR:
            workp = work_buf;
            while ((c = *workp++ & UCHARMAX) != EOS) {
                if (char_type[ c] & mbstart) {      /* Multi-byte character */
                    mb_read( c, &workp, (*out_p++ = c, &out_p));
                                            /* Copy as it is        */
                    continue;
                }
                if (c == '"') {
                    *out_p++ = '\\';        /* Insert '\\'          */
                } else if (c == '\\') {
#if OK_UCN
                    if (mcpp_mode == POST_STD || ! stdc3
                            || (*workp != 'u' && *workp != 'U'))
                                            /* Not UCN              */
#endif
                        *out_p++ = '\\';
                }
                *out_p++ = c;
            }
            *out_p = EOS;
            break;
        default:
            out_p = stpcpy( out_p, work_buf);
            break;
        }
    }

    if (mcpp_mode == POST_STD)
        unget_ch();
    *out_p++ = '"';
    *out_p = EOS;

    if (stray_bsl) {
        int     invalid = FALSE;
        unget_string( out, defp->name);
        if (mcpp_debug & EXPAND)
            dump_string( "checking generated token", infile->buffer);
        scan_quote( get_ch(), work_buf, work_end, TRUE);
            /* Unterminated or too long string will be diagnosed    */
        if (*infile->bptr != EOS)           /* More than a token    */
            invalid = TRUE; /* Diagnose after clearing the "file"   */
        infile->bptr += ACE_OS::strlen( infile->bptr);
        get_ch();                           /* Clear the "file"     */
        unget_ch();
        if (invalid)
            diag_macro( CERROR
                    , "Not a valid string literal %s"       /* _E_  */
                    , out, 0L, 0, defp, 0);
    }
#if NWORK-2 > SLEN90MIN
    else if ((warn_level & 4) && out_p - out > str_len_min)
        diag_macro( CWARN
                , "String literal longer than %.0s%ld bytes %s"     /* _W4_ */
                , 0 , (long) str_len_min, out, defp, 0);
#endif
    return  out_p;
}

static char *   substitute(
    const DEFBUF *  ,           /* The macro getting arguments  */
    int         ,   /* gvar_arg's argument is GCC3 variable argument*/
    char **     arglist,        /* Pointers to actual arguments     */
    const char *    in,                     /* Replacement text     */
    char *      out,                        /* Output buffer        */
    char *      out_end                     /* End of output buffer */
)
/*
 * Replace completely each actual arguments of the macro, and substitute for
 * the formal parameters in the replacement list.
 */
{
  //char *  out_start = out;
    int     c;

    *out = EOS;                             /* Ensure to termanate  */
    while ((c = *in++) != EOS) {
        if (c == MAC_PARM) {                /* Formal parameter     */
            c = *in++ & UCHARMAX;           /* Parameter number     */
            if (mcpp_debug & EXPAND) {
                mcpp_fprintf( DBG, " (expanding arg[%d])", c);
                dump_string( 0, arglist[ c - 1]);
            }
#if COMPILER == GNUC
            if (c == gvar_arg && *(arglist[ c - 1]) == RT_END) {
                /* GCC3 variadic macro and its variable argument is absent  */
                char *  tmp;
                tmp = out - 1;
                while (*tmp == ' ')
                    tmp--;
                if (*tmp == ',') {
                    out = tmp;      /* Remove the immediately preceding ',' */
                    if (warn_level & 1) {
                        *out = EOS;
                        diag_macro( CWARN,
        "Removed ',' preceding the absent variable argument: %s"    /* _W1_ */
                                , out_start, 0L, 0, defp, 0);
                    }
                }
            } else
#endif
            if ((out = rescan( 0, arglist[ c - 1], out, out_end))
                    == 0) {              /* Replace completely   */
                return  0;               /* Error                */
            }
        } else {
            *out++ = c;                     /* Copy the character   */
        }
    }
    *out = EOS;
    return  out;
}

static char *   rescan(
    const DEFBUF *  outer,          /* Outer macro just replacing   */
    const char *    in,             /* Sequences to be rescanned    */
    char *  out,                            /* Output buffer        */
    char *  out_end                         /* End of output buffer */
)
/*
 * Re-scan the once replaced sequences to replace the remaining macros
 * completely.
 * rescan() and replace() call each other recursively.
 *
 * Note: POST_STD mode does not use IN_SRC nor TOK_SEP and seldom uses RT_END.
 * Checking of those are unnecessary overhead for POST_STD mode.  To integrate
 * the code for POST_STD with STD mode, however, we use these checkings
 * commonly.
 * compat_mode also does not use IN_SRC.
 */
{
    char *  cur_cp = 0;
    char *  tp = 0;              /* Temporary pointer into buffer*/
    char *  out_p = out;            /* Current output pointer       */
    FILEINFO *  file;       /* Input sequences stacked on a "file"  */
    DEFBUF *    inner = 0;              /* Inner macro to replace       */
    int     c;                      /* First character of token     */

    if (mcpp_debug & EXPAND) {
        mcpp_fprintf( DBG, "rescan_level--%d (%s) "
                , rescan_level + 1, outer ? outer->name : "<arg>");
        dump_string( "rescan entry", in);
    }
    if (! disable_repl( outer)) /* Don't re-replace replacing macro */
        return  0;               /* Too deeply nested macro call */
    if (mcpp_mode == STD) {
        get_ch();                   /* Clear empty "file"s          */
        unget_ch();                 /*      for diagnostic          */
        cur_cp = infile->bptr;      /* Remember current location    */
    }
    file = unget_string( in, outer ? outer->name : 0);
                                    /* Stack input on a "file"      */

    while ((c = get_ch()), file == infile
        /* Rescanning is limited to the "file"  */
            && c != RT_END) {
            /*
             * This is the trick of STD mode.  collect_args() via replace()
             * may read over to file->parent (provided the "file" is macro)
             * unless stopped by RT_END.
             */

        if (c == ' ' || c == TOK_SEP) {
            *out_p++ = c;
            continue;
        }
        if (scan_token( c, (tp = out_p, &out_p), out_end) == NAM
                && c != DEF_MAGIC && (inner =
                look_id( (mcpp_mode == STD && c == IN_SRC) ? tp+1 : tp))
                        != 0) {          /* A macro name         */
            int     is_able;        /* Macro is not "blue-painted"  */
            char *  inp_save = infile->bptr;        /* Remember current bptr*/

            if (is_macro_call( inner, &out_p)
                    && ((mcpp_mode == POST_STD && is_able_repl( inner))
                        || (mcpp_mode == STD
                            && (((is_able = is_able_repl( inner)) == YES)
                                || (is_able == READ_OVER && c == IN_SRC))))) {
                                            /* Really a macro call  */
                if ((out_p = replace( inner, tp, out_end, outer, file))
                        == 0)            /* Error of macro call  */
                    break;
            } else {
                if (file != infile && infile->bptr != inp_save
                        && *(infile->bptr - 1) == ' ') {
                    /* Has read over spaces into the parent "file"  */
                    infile->bptr--;         /* Pushback forcibly    */
                    out_p--;
                }
                if ((is_able = is_able_repl( inner)) == NO
                        || (mcpp_mode == STD && is_able == READ_OVER
                                && c != IN_SRC)) {
                    if (mcpp_mode == POST_STD || c != IN_SRC)
                        ACE_OS::memmove( tp + 1, tp, (size_t) (out_p++ - tp));
                    *tp = DEF_MAGIC;        /* Mark not to replace  */
                }                           /* Else not a macro call*/
            }
        }
        if (out_end <= out_p) {
            *out_p = EOS;
            diag_macro( CERROR, macbuf_overflow, outer ? outer->name : in, 0L
                    , out, outer, inner);
            out_p = 0;
            break;
        }
    }

    if (out_p) {
        *out_p = EOS;
        if (mcpp_mode == STD) {
            if  (c != RT_END) {
                unget_ch();
                if (outer != 0) {    /* outer isn't a macro in argument  */
                    if (infile && infile->bptr != cur_cp
                                    /* Have overrun replacement list*/
                            && !(tp && *tp == DEF_MAGIC)
                                                /* Macro is enabled */
                            && ((!compat_mode && (warn_level & 1))
                                || (compat_mode && (warn_level & 8)))) {
                        diag_macro( CWARN,
"Replacement text \"%s\" of macro %.0ld\"%s\" involved subsequent text" /* _W1_ */
                            , in, 0L, outer->name, outer, inner);
                    }
                }
            }                       /* Else remove RT_END           */
        } else {
            unget_ch();
        }
    }
    enable_repl( outer, TRUE);      /* Enable macro for later text  */
    if (mcpp_debug & EXPAND) {
        mcpp_fprintf( DBG, "rescan_level--%d (%s) "
                , rescan_level + 1, outer ? outer->name : "<arg>");
        dump_string( "rescan exit", out);
    }
    return  out_p;
}

static int
disable_repl(
    const DEFBUF *  defp
)
/*
 * Register the macro name currently replacing.
 */
{
    if (defp == 0)
        return  TRUE;
    if (rescan_level >= RESCAN_LIMIT) {
        diag_macro( CERROR,
            "Rescanning macro \"%s\" more than %ld times at \"%s\"" /* _E_  */
                , macro_name, (long) RESCAN_LIMIT, defp->name, defp, 0);
        return  FALSE;
    }
    replacing[ rescan_level].def = defp;
    replacing[ rescan_level++].read_over = NO;
    return  TRUE;
}

static void enable_repl(
    const DEFBUF *  defp,
    int         done
)
/*
 * Un-register the macro name just replaced for later text.
 */
{
    if (defp == 0)
        return;
    replacing[ rescan_level - 1].def = 0;
    if (done && rescan_level)
        rescan_level--;
}

static int  is_able_repl(
    const DEFBUF *  defp
)
/*
 * The macro is permitted to replace ?
 */
{
    int     i;

    if (defp == 0)
        return  YES;
    for (i = rescan_level-1; i >= 0; i--) {
        if (defp == replacing[ i].def)
            return  replacing[ i].read_over;
    }
    return  YES;
}


/*
 *  M a c r o   E x p a n s i o n   i n   P R E - S T A N D A R D   M o d e
 */

#include    "setjmp.h"

static jmp_buf  jump;

static char *   arglist_pre[ NMACPARS];     /* Pointers to args     */

static int      rescan_pre( int c, char * mp, char * mac_end);
                /* Replace a macro repeatedly   */
static int      replace_pre( DEFBUF * defp);
                /* Replace a macro once         */
static void     substitute_pre( DEFBUF * defp);
                /* Substitute parms with args   */

static char *   expand_prestd(
    DEFBUF *    defp,                       /* Macro definition     */
    char *  out,                            /* Output buffer        */
    char *  out_end                         /* End of output buffer */
)
/*
 * Expand a macro call completely, write the results to the specified buffer
 * and return the advanced pointer.
 */
{
    char    macrobuf[ NMACWORK + IDMAX];    /* Buffer for rescan_pre()      */
    char *  mac_end = &macrobuf[ NMACWORK]; /* End of macrobuf[]    */
    char *  out_p;                          /* Pointer into out[]   */
    char *  mp = macrobuf;                  /* Pointer into macrobuf*/
    int     len;                            /* Length of a token    */
    int     token_type;                     /* Type of token        */
    int     c;

    macro_line = src_line;                  /* Line number for diag.*/
    unget_string( identifier, identifier);  /* To re-read           */
    macro_name = defp->name;
    rescan_level = 0;
    if (setjmp( jump) == 1) {
        skip_macro();
        mp = macrobuf;
        *mp = EOS;
        macro_line = MACRO_ERROR;
        goto  err_end;
    }

    while ((c = get_ch()) != CHAR_EOF && infile->fp == 0) {
                            /* While the input stream is a macro    */
        while (c == ' ') {                  /* Output the spaces    */
            *mp++ = c;
            c = get_ch();
            if (infile == 0 || infile->fp != 0)
                goto  exp_end;
        }
        token_type = rescan_pre( c, mp, mac_end);   /* Scan token   */
            /*  and expand.  Result of expansion is written at mp.  */

        switch (token_type) {
        case STR:                           /* String literal       */
        case CHR:                           /* Character constant   */
        case NUM:                           /* Number token         */
        case OPE:                           /* Operator or punct.   */
        case NAM:                           /* Identifier           */
            len = ACE_OS::strlen( mp);
            mp += len;
            break;
        case SEP:                           /* Special character    */
            switch( *mp) {
            case COM_SEP:
                if (mcpp_mode == OLD_PREP)
                    break;  /* Zero-length comment is removed now   */
                /* Else fall through    */
            default:                        /* Who knows ?          */
                mp++;                       /* Copy the character   */
                break;
            }
            break;
        case NO_TOKEN:  break;              /* End of file          */
        default:                            /* Unkown token char.   */
            mp++;                           /* Copy the character   */
            break;
        }

        if (mac_end <= mp) {
            *mp = EOS;
            cerror( macbuf_overflow, macro_name, 0L, macrobuf);
            longjmp( jump, 1);
        }
        if (mcpp_debug & GETC) {
            *mp = EOS;
            dump_string( "macrobuf", macrobuf);
        }
    }

exp_end:
    unget_ch();
    while (macrobuf < mp && *(mp - 1) == ' ')
        mp--;                           /* Remove trailing blank    */
    macro_line = 0;
    *mp = EOS;
    if (mp - macrobuf > out_end - out) {
        cerror( macbuf_overflow, macro_name, 0L, macrobuf);
        macro_line = MACRO_ERROR;
    }
err_end:
    out_p = stpcpy( out, macrobuf);
    if (mcpp_debug & EXPAND) {
        dump_string( "expand_prestd exit", out);
    }
    macro_name = 0;
    clear_exp_mac();
    return  out_p;
}

static int  rescan_pre(
    int     c,                      /* First character of token     */
    char *  mp,                     /* Output buffer                */
    char *  mac_end                 /* End of output buffer         */
)
/*
 * If the token is a macro name, replace the macro repeatedly until the first
 * token becomes a non-macro and return the type of token after expansion.
 */
{
    int     token_type;             /* Type of token                */
    char *  cp = mp;        /* Value of mp should not be changed    */
    DEFBUF *    defp;
    FILEINFO *  file;

    while ((token_type = scan_token( c, &cp, mac_end)) == NAM
            && (defp = look_id( identifier)) != 0) { /* Macro    */
        if (replace_pre( defp) == FALSE)
            break;                  /* Macro name with no argument  */
        file = infile;
        c = get_ch();
        if (file != infile) {       /* Replaced to 0 token          */
            unget_ch();
            token_type = NO_TOKEN;
            break;
        }
        cp = mp;                    /* Overwrite on the macro call  */
    }                               /* The macro call is replaced   */
    return  token_type;
}

static int  replace_pre(
    DEFBUF *    defp                /* Definition of the macro      */
)
/*
 * Replace a macro one level.  Called from expand_prestd() (via rescan_pre())
 * when an identifier is found in the symbol table.  It calls collect_args()
 * to parse actual arguments, checking for the correct number.  It then
 * creates a "file" containing single line containing the replacement text
 * with the actual arguments inserted appropriately.  This is "pushed back"
 * onto the input stream.  (When get_ch() routine runs off the end of the macro
 * line, it will dismiss the macro itself.)
 */
{
    int         nargs;                      /* Number of arguments  */
    int         c;

    if (mcpp_debug & EXPAND) {
        dump_a_def( "replace_pre entry", defp, FALSE, FALSE, TRUE, fp_debug);
        dump_unget( "replace_pre entry");
    }
    if (++rescan_level >= PRESTD_RESCAN_LIMIT) {
        diag_macro( CERROR
                , "Recursive macro definition of \"%s\""    /* _E_  */
                , defp->name, 0L, 0, defp, 0);
        longjmp( jump, 1);
    }

    /*
     * Here's a macro to replace.
     */
    switch (defp->nargs) {

    case DEF_NOARGS:                /* No argument just stuffs      */
    case DEF_NOARGS - 1:                    /* Non-standard predef  */
        break;

    default:                                /* defp->nargs >= 0     */
        /*
         * Nothing funny about this macro.
         */
        c = squeeze_ws( 0);              /* Look for and skip '('*/
        if (c != '(') {
            /*
             * If the programmer writes
             *      #define foo() ...
             *      ...
             *      foo [no ()]
             * just write foo to output.
             */
            unget_ch();
            if (warn_level & 8)
                diag_macro( CWARN, only_name, defp->name, 0L, 0, defp, 0);
            return  FALSE;
        } else {
            arglist_pre[ 0] = xmalloc( (size_t) (NMACWORK + IDMAX * 2));
            nargs = collect_args( defp, arglist_pre);
                                            /* Collect arguments    */
            if (nargs == ARG_ERROR) {       /* End of input         */
                ACE_OS::free( arglist_pre[ 0]);
                longjmp( jump, 1);
            }
        }
        break;
    }                                       /* defp->nargs switch   */

    if (defp->nargs > 0)
        substitute_pre( defp);              /* Do actual arguments  */
    else
        unget_string( defp->repl, defp->name);

    if (mcpp_debug & EXPAND)
        dump_unget( "replace_pre exit");
    if (defp->nargs >= 0)
        ACE_OS::free( arglist_pre[ 0]);
    return  TRUE;
}

static void substitute_pre(
    DEFBUF *        defp            /* Current macro being replaced */
)
/*
 * Stuff the macro body, substituting formal parameters with actual arguments.
 */
{
    int         c;                          /* Current character    */
    FILEINFO *  file;                       /* Funny #include       */
    char *      out_end;                    /* -> output buffer end */
    char *      in_p;                       /* -> replacement text  */
    char *      out_p;                      /* -> macro output buff */

    file = get_file( defp->name, (size_t) (NMACWORK + 1));
    in_p = defp->repl;                      /* -> macro replacement */
    out_p = file->buffer;                   /* -> output buffer     */
    out_end = file->buffer + NMACWORK;      /* -> buffer end        */

    while ((c = *in_p++) != EOS) {
        if (c == MAC_PARM) {
            c = (*in_p++ & UCHARMAX) - 1;   /* Parm number          */
            /*
             * Substitute formal parameter with actual argument.
             */
            if (out_end <= (out_p + ACE_OS::strlen( arglist_pre[ c])))
                goto nospace;
            out_p = stpcpy( out_p, arglist_pre[ c]);
        } else {
            *out_p++ = c;
        }
        if (out_end <= out_p)
            goto  nospace;
    }

    *out_p = EOS;
    file->buffer = xrealloc( file->buffer, ACE_OS::strlen( file->buffer) + 1);
    file->bptr = file->buffer;              /* Truncate buffer      */
    if (mcpp_debug & EXPAND)
        dump_string( "substitute_pre macroline", file->buffer);
    return;

nospace:
    *out_p = EOS;
    diag_macro( CERROR, macbuf_overflow, defp->name, 0L, file->buffer, defp
            , 0);
    longjmp( jump, 1);
}


/*
 *                  C O M M O N   R O U T I N E S
 *  f o r   S T A N D A R D   a n d   p r e - S T A N D A R D   M o d e s
 */

static int  collect_args(
    const DEFBUF *  defp,       /* Definition of the macro          */
    char **     arglist         /* Pointers to actual arguments     */
)
/*
 *   Collect the actual arguments for the macro, checking for correct number
 * of arguments.
 *   Variable arguments (on Standard modes) are read as a merged argument.
 *   Return number of arguments, or ARG_ERROR on error of unterminated macro.
 *   collect_args() may read over to the next line unless 'in_directive' is
 * set to TRUE.
 *   collect_args() may read over into file->parent to complete a macro call
 * unless stopped by RT_END (provided the "file" is macro).  This is a key
 * trick of STD mode macro expansion.  Meanwhile, POST_STD mode limits the
 * arguments in the "file" (macro or not).
 *   Note: arglist[ n] may be reallocated by collect_args().
 */
{
    const char *    name = defp->name;
    char *  argp = arglist[ 0];         /* Pointer to an argument   */
    char *  arg_end;                    /* End of arguments buffer  */
    char *  valid_argp = 0;          /* End of valid arguments   */
    char *  sequence;           /* Token sequence for diagnostics   */
    char *  seq;                /* Current pointer into 'sequence'  */
    char *  seq_end;                            /* Limit of buffer  */
    int     args;               /* Number of arguments expected     */
    int     nargs = 0;                  /* Number of collected args */
    int     var_arg = defp->nargs & VA_ARGS;    /* Variable args    */
    int     more_to_come = FALSE;       /* Next argument is expected*/
    int     ret = ARG_ERROR;                    /* Return value     */
    int     c;

    if (mcpp_debug & EXPAND)
        dump_unget( "collect_args entry");
    args = (defp->nargs == DEF_PRAGMA) ? 1 : (defp->nargs & ~AVA_ARGS);
    if (args == 0)                      /* Need no argument         */
        valid_argp = argp;
    *argp = EOS;                        /* Make sure termination    */
    arg_end = argp + NMACWORK/2;
    seq = sequence = arg_end + IDMAX;   /* Use latter half of argp  */
    seq_end = seq + NMACWORK/2;
    seq = stpcpy( seq, name);
    *seq++ = '(';
    /*
     * in_getarg is set TRUE while getting macro arguments, for the sake of
     * diagnostic's convenience.
     * in_getarg is used only in STD mode.
     */
    if (mcpp_mode == STD)
        in_getarg = TRUE;

    while (1) {
        c = squeeze_ws( &seq);          /* Skip white spaces        */
        if (c == ')' || c == ',')
            scan_token( c, &seq, seq_end);  /* Ensure token parsing */
        else
            *seq = EOS;

        switch (c) {                    /* First character of token */
        case ')':
            if (! more_to_come)         /* Zero argument            */
                break;                  /* Else fall through        */
        case ',':                       /* Empty argument           */
            if (warn_level & 2)
                diag_macro( CWARN, empty_arg, sequence, 0L, 0, defp, 0);
            if (standard && var_arg && nargs == args - 1) {
                /* Variable arguments begin with an empty argument  */
                c = get_an_arg( c, &argp, arg_end, &seq, 1);
            } else {
                if (mcpp_mode == STD)
                    *argp++ = RT_END;
                *argp++ = EOS;
            }
            if (++nargs == args)
                valid_argp = argp;
            if (c == ',') {
                more_to_come = TRUE;
                continue;
            } else {                    /* ')'                      */
                break;
            }
        case '\n':      /* Unterminated macro call in control line  */
            unget_ch();                 /* Fall through             */
        case RT_END:                    /* Error of missing ')'     */
            diag_macro( CERROR, unterm_macro, sequence, 0L, 0, defp, 0);
                                        /* Fall through             */
        case CHAR_EOF:                  /* End of file in macro call*/
            goto  arg_ret;              /* Diagnosed by at_eof()    */
        default:                        /* Nomal argument           */
            break;
        }

        if (c == ')')                   /* At end of all args       */
            break;

        c = get_an_arg( c, &argp, arg_end, &seq,
            (var_arg && nargs == args - 1) ? 1 : 0);

        if (++nargs == args)
            valid_argp = argp;          /* End of valid arguments   */
        if (c == ')')
            break;
        if (c == 0)                     /* End of file              */
            goto  arg_ret;              /* Diagnosed by at_eof()    */
        if (c == -1) {                  /* Untermanated macro call  */
            diag_macro( CERROR, unterm_macro, sequence, 0L, 0, defp, 0);
            goto  arg_ret;
        }
        more_to_come = (c == ',');
    }                                   /* Collected all arguments  */

    if (nargs == 0 && args == 1) {      /* Only and empty argument  */
        if (warn_level & 2)
            diag_macro( CWARN, empty_arg, sequence, 0L, 0, defp, 0);
    } else if (nargs != args) {         /* Wrong number of arguments*/
        if (mcpp_mode != OLD_PREP || (warn_level & 1)) {
            if ((standard && var_arg && (nargs == args - 1))
                                /* Absence of variable arguments    */
                        || (mcpp_mode == OLD_PREP)) {
                if (warn_level & 1)
                    diag_macro( CWARN, narg_error, nargs < args ? "Less"
                            : "More", (long) args, sequence, defp, 0);
            } else {
                diag_macro( CERROR, narg_error, nargs < args ? "Less" : "More"
                        , (long) args, sequence, defp, 0);
            }
        }
    }
    if (args < nargs) {
        argp = valid_argp;              /* Truncate excess arguments*/
    } else {
        for (c = nargs; c < args; c++) {
            if (mcpp_mode == STD)
                *argp++ = RT_END;       /* For rescan()             */
            *argp++ = EOS;              /* Missing arguments        */
        }
        if (c == 0)
            argp++;                     /* Ensure positive length   */
    }
    arglist[ 0] = argp
            = xrealloc( arglist[ 0], (size_t)(argp - arglist[ 0]));
                                        /* Use memory sparingly     */
    for (c = 1; c < args; c++)
        arglist[ c] = argp += ACE_OS::strlen( argp) + 1;

    ret = nargs;
arg_ret:
    if (mcpp_debug & EXPAND) {
        if (nargs > 0)
            dump_args( "collect_args exit"
                    , nargs < args ? nargs : args, arglist);
        dump_unget( "collect_args exit");
    }
    if (mcpp_mode == STD)
        in_getarg = FALSE;

    return  ret;
}

static int  get_an_arg(
    int     c,
    char ** argpp,      /* Address of pointer into argument list    */
    char *  arg_end,                /* End of argument list buffer  */
    char ** seqp,                   /* Buffer for diagnostics       */
    int     var_arg                 /* 1 on __VA_ARGS__, 0 on others*/
)
/*
 * Get an argument of macro into '*argpp', return the next punctuator.
 * Variable arguments are read as a merged argument.
 */
{
    int     end_an_arg = FALSE;             /* End-of-an-arg flag   */
    int     paren = var_arg;                /* For embedded ()'s    */
    int     token_type;
    char *  prevp;
    char *  argp = *argpp;

    while (1) {
        if (c == '\n'                       /* In control line      */
                || c == RT_END) {       /* Boundary of rescan (in STD mode) */
            if (c == '\n')
                unget_ch();
            break;
        }
        token_type = scan_token( c, (prevp = argp, &argp), arg_end);
                                            /* Scan the next token  */
        switch (c) {
        case '(' :                          /* Worry about balance  */
            paren++;                        /* To know about commas */
            break;
        case ')' :                          /* Other side too       */
            if (paren-- == var_arg)         /* At the end?          */
                end_an_arg = TRUE;          /* Else more to come    */
            break;
        case ',' :
            if (paren == 0)                 /* Comma delimits arg   */
                end_an_arg = TRUE;
            break;
        case CHAR_EOF:                      /* Unexpected EOF       */
            return  0;
        default :                           /* Any token            */
            if (mcpp_mode == STD && ! compat_mode && token_type == NAM
                    && c != IN_SRC && c != DEF_MAGIC && infile->fp) {
                ACE_OS::memmove( prevp + 1, prevp, (size_t) (argp++ - prevp));
                *prevp = IN_SRC;
            }       /* Mark that the name is read from source file  */
            break;
        }                                   /* End of switch        */

        if (end_an_arg)                     /* End of an argument   */
            break;
        c = squeeze_ws( &argp);             /* To the next token    */
    }                                       /* Collected an argument*/

    *argp = EOS;
    *seqp = stpcpy( *seqp, *argpp);         /* Save the sequence    */
    if (c == '\n' || c == RT_END)
        return  -1;                         /* Unterminated macro   */
    argp--;                                 /* Remove the punctuator*/
    while (*argpp < argp && *(argp - 1) == ' ')
        --argp;                     /* Remove trailing blanks       */
    if (mcpp_mode == STD)
        *argp++ = RT_END;                   /* For rescan()         */
    *argp++ = EOS;                          /* Terminate an argument*/
    *argpp = argp;
    return  c;
}

static int  squeeze_ws(
    char **     out                 /* pointer to output pointer    */
)
/*
 * Squeeze white spaces to one space.
 * White spaces are ' ' ('\t', '\r', '\v', '\f' converted to ' ' by get_ch()),
 * and '\n' unless in_directive is set.
 * COM_SEP is skipped.  TOK_SEPs are squeezed to one TOK_SEP.
 * If white spaces are found and 'out' is not 0, write a space to *out and
 * increment *out.
 * Return the next character.
 */
{
    int     c;
    int     space = 0;
    FILEINFO *      file = infile;
    int     tsep = 0;
    FILE *  fp = infile->fp;

    while ((char_type[ c = get_ch()] & SPA) && (! standard 
            || (mcpp_mode == POST_STD && file == infile)
            || (mcpp_mode == STD
                && ((macro_line != 0 && macro_line != MACRO_ERROR)
                    || file == infile)))) {
        if (c == '\n') {
            if (in_directive)           /* If scanning control line */
                break;                  /*   do not skip newline.   */
            else
                wrong_line = TRUE;
        }
        if (c == TOK_SEP) {
            if (mcpp_mode == STD)
                tsep++;
            continue;           /* Skip COM_SEP in OLD_PREP mode    */
        }
        space++;
    }

    if (out) {
        if (space)              /* Write a space to output pointer  */
            *(*out)++ = ' ';    /*   and increment the pointer.     */
        if (tsep)
            *(*out)++ = TOK_SEP;
    }
    if (mcpp_mode == POST_STD && file != infile) {
        unget_ch();             /* Arguments cannot cross "file"s   */
        c = fp ? CHAR_EOF : RT_END; /* EOF is diagnosed by at_eof() */
    } else if (mcpp_mode == STD && macro_line == MACRO_ERROR
            && file != infile) {            /* EOF                  */
        unget_ch();             /*   diagnosed by at_eof() or only  */
        c = CHAR_EOF;           /*   name of a function-like macro. */
    }                       /* at_eof() resets macro_line on error  */
    return  c;                  /* Return the next character        */
}

static void skip_macro( void)
/*
 * Clear the stacked (i.e. half-expanded) macro, called on macro error.
 */
{
    if (infile == 0)                     /* End of input         */
        return;
    if (infile->fp)                         /* Source file          */
        return;
    while (infile->fp == 0) {            /* Stacked stuff        */
        infile->bptr += ACE_OS::strlen( infile->bptr);
        get_ch();                           /* To the parent "file" */
    }
    unget_ch();
}

static void diag_macro(
    int     severity,                       /* Error or warning     */ 
    const char *    format,
    const char *    arg1,
    long            arg2, 
    const char *    arg3,
    const DEFBUF *  defp1,          /* Macro causing the problem 1  */
    const DEFBUF *  defp2                                   /*   2  */
)
/*
 * Supplement macro information for diagnostic.
 */
{

    if (defp1 && defp1->name != macro_name)
        expanding( defp1->name, FALSE);
                            /* Inform of the problematic macro call */
    if (defp2 && defp2->name != macro_name)
        expanding( defp2->name, FALSE);
    if (severity == CERROR)
        cerror( format, arg1, arg2, arg3);
    else
        cwarn( format, arg1, arg2, arg3);
}

static void dump_args(
    const char *    why,
    int     nargs,
    char ** arglist
)
/*
 * Dump arguments list.
 */
{
    int     i;

    mcpp_fprintf( DBG, "dump of %d actual arguments %s\n", nargs, why);
    for (i = 0; i < nargs; i++) {
        mcpp_fprintf( DBG, "arg[%d]", i + 1);
        dump_string( 0, arglist[ i]);
    }
}

