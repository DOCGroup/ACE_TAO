// $Id$
/*  A Bison parser, made from CC_command.y with Bison version GNU Bison version 1.21
  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse CC_commandparse
#define yylex CC_commandlex
#define yyerror CC_commanderror
#define yylval CC_commandlval
#define yychar CC_commandchar
#define yydebug CC_commanddebug
#define yynerrs CC_commandnerrs
#define	T_START_CMD	258
#define	T_CREATE_CMD	259
#define	T_LOCK_CMD	260
#define	T_UNLOCK_CMD	261
#define	T_TRYLOCK_CMD	262
#define	T_CHANGEMODE_CMD	263
#define	T_SLEEP_CMD	264
#define	T_WAIT_CMD	265
#define	T_REPEAT_CMD	266
#define	T_EXCEP_CMD	267
#define	T_PRINT_CMD	268
#define	T_LOOKUP_CMD	269
#define	T_TERM	270
#define	T_READ	271
#define	T_IREAD	272
#define	T_UPGRADE	273
#define	T_WRITE	274
#define	T_IWRITE	275
#define	T_IDENT	276
#define	Q_STRING	277
#define	T_NUM	278

#line 18 "CC_command.y"

#include "CC_command.h"
#include "ace/ACE.h"
#include "orbsvcs/CosConcurrencyControlC.h"

int CC_commanderror(char *s);
int CC_commandlex(void);

extern int line_no;
extern char line_buf[500];
extern char *CC_commandtext;
extern CC_CommandList *cmdlist;

#line 32 "CC_command.y"
typedef union {
  CC_Command *command;
  CosConcurrencyControl::lock_mode lock_mode;
  char *id;
  int num;
} YYSTYPE;

#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		66
#define	YYFLAG		-32768
#define	YYNTBASE	24

#define YYTRANSLATE(x) ((unsigned)(x) <= 278 ? yytranslate[x] : 31)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     3,     6,     8,    12,    16,    19,    24,    28,
    33,    37,    42,    46,    52,    57,    61,    64,    68,    72,
    76,    80,    84,    86,    88,    90,    92,    94,    96,    98
};

static const short yyrhs[] = {    -1,
    25,     0,    25,    26,     0,    26,     0,     3,    27,    15,
     0,     4,    28,    15,     0,     4,    15,     0,     5,    28,
    30,    15,     0,     5,    30,    15,     0,     6,    28,    30,
    15,     0,     6,    30,    15,     0,     7,    28,    30,    15,
     0,     7,    30,    15,     0,     8,    28,    30,    30,    15,
     0,     8,    30,    30,    15,     0,     9,    23,    15,     0,
    10,    15,     0,    10,    22,    15,     0,    13,    22,    15,
     0,    14,    28,    15,     0,    11,    23,    15,     0,    12,
    29,    15,     0,    21,     0,    21,     0,    21,     0,    16,
     0,    17,     0,    18,     0,    19,     0,    20,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    51,    52,    60,    61,    64,    66,    68,    70,    72,    74,
    76,    78,    80,    82,    84,    86,    88,    90,    92,    94,
    96,    99,   103,   105,   107,   109,   110,   111,   112,   113
};

static const char * const yytname[] = {   "$","error","$illegal.","T_START_CMD",
"T_CREATE_CMD","T_LOCK_CMD","T_UNLOCK_CMD","T_TRYLOCK_CMD","T_CHANGEMODE_CMD",
"T_SLEEP_CMD","T_WAIT_CMD","T_REPEAT_CMD","T_EXCEP_CMD","T_PRINT_CMD","T_LOOKUP_CMD",
"T_TERM","T_READ","T_IREAD","T_UPGRADE","T_WRITE","T_IWRITE","T_IDENT","Q_STRING",
"T_NUM","start","cmd_list","cmd","config_file_name","lock_set_name","exception_name",
"lock_mode",""
};
#endif

static const short yyr1[] = {     0,
    24,    24,    25,    25,    26,    26,    26,    26,    26,    26,
    26,    26,    26,    26,    26,    26,    26,    26,    26,    26,
    26,    26,    27,    28,    29,    30,    30,    30,    30,    30
};

static const short yyr2[] = {     0,
     0,     1,     2,     1,     3,     3,     2,     4,     3,     4,
     3,     4,     3,     5,     4,     3,     2,     3,     3,     3,
     3,     3,     1,     1,     1,     1,     1,     1,     1,     1
};

static const short yydefact[] = {     1,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     2,     4,    23,     0,     7,    24,     0,    26,
    27,    28,    29,    30,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    17,     0,     0,    25,     0,     0,     0,
     3,     5,     6,     0,     9,     0,    11,     0,    13,     0,
     0,    16,    18,    21,    22,    19,    20,     8,    10,    12,
     0,    15,    14,     0,     0,     0
};

static const short yydefgoto[] = {    64,
    13,    14,    16,    19,    38,    26
};

static const short yypact[] = {     0,
    -1,    14,    20,    20,    20,    20,     7,     4,     8,     1,
    10,    13,     0,-32768,-32768,    18,-32768,-32768,    27,-32768,
-32768,-32768,-32768,-32768,    31,    28,    31,    29,    31,    30,
    31,    31,    37,-32768,    38,    39,-32768,    40,    41,    42,
-32768,-32768,-32768,    43,-32768,    44,-32768,    45,-32768,    31,
    46,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    47,-32768,-32768,    63,    64,-32768
};

static const short yypgoto[] = {-32768,
-32768,    52,-32768,    12,-32768,    -4
};


#define	YYLAST		65


static const short yytable[] = {    28,
    30,    32,     1,     2,     3,     4,     5,     6,     7,     8,
     9,    10,    11,    12,    25,    27,    29,    31,    34,    15,
    44,    37,    46,    40,    48,    35,    50,    51,    17,    33,
    36,    39,    42,    18,    18,    20,    21,    22,    23,    24,
    18,    43,    45,    47,    49,    61,    20,    21,    22,    23,
    24,    52,    53,    54,    55,    56,    57,    58,    59,    60,
    62,    63,    65,    66,    41
};

static const short yycheck[] = {     4,
     5,     6,     3,     4,     5,     6,     7,     8,     9,    10,
    11,    12,    13,    14,     3,     4,     5,     6,    15,    21,
    25,    21,    27,    12,    29,    22,    31,    32,    15,    23,
    23,    22,    15,    21,    21,    16,    17,    18,    19,    20,
    21,    15,    15,    15,    15,    50,    16,    17,    18,    19,
    20,    15,    15,    15,    15,    15,    15,    15,    15,    15,
    15,    15,     0,     0,    13
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/pkg/gnu/lib/bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Bob Corbett and Richard Stallman

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 1, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */


#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || defined (__DECCXX_VER)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca (unsigned int);
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#define YYLEX		yylex(&yylval, &yylloc)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_bcopy(FROM,TO,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_bcopy (from, to, count)
     char *from;
     char *to;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_bcopy (char *from, char *to, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 184 "/pkg/gnu/lib/bison.simple"
int
yyparse()
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
#ifdef YYLSP_NEEDED
		 &yyls1, size * sizeof (*yylsp),
#endif
		 &yystacksize);

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_bcopy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_bcopy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_bcopy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 2:
#line 52 "CC_command.y"
{ cmdlist->add(new CC_Sleep_Cmd(0));
			   /* dummy to check exeption in the last command */
		  if(cmdlist->execute()!=0) //CC_FAIL
                    { printf(" ** Test succeded!!\n"); }
                  else
                    { printf(" ** Test FAILED!!\n"); } ;
    break;}
case 3:
#line 60 "CC_command.y"
{ cmdlist->add(yyvsp[0].command); ;
    break;}
case 4:
#line 61 "CC_command.y"
{ cmdlist->add(yyvsp[0].command); ;
    break;}
case 5:
#line 65 "CC_command.y"
{ yyval.command = new CC_Start_Cmd(yyvsp[-1].id); ;
    break;}
case 6:
#line 67 "CC_command.y"
{ yyval.command = new CC_CreateLockSet_Cmd(yyvsp[-1].id); ;
    break;}
case 7:
#line 69 "CC_command.y"
{ yyval.command = new CC_CreateLockSet_Cmd(""); ;
    break;}
case 8:
#line 71 "CC_command.y"
{ yyval.command = new CC_Lock_Cmd(yyvsp[-2].id, yyvsp[-1].lock_mode); ;
    break;}
case 9:
#line 73 "CC_command.y"
{ yyval.command = new CC_Lock_Cmd("", yyvsp[-1].lock_mode); ;
    break;}
case 10:
#line 75 "CC_command.y"
{ yyval.command = new CC_UnLock_Cmd(yyvsp[-2].id, yyvsp[-1].lock_mode); ;
    break;}
case 11:
#line 77 "CC_command.y"
{ yyval.command = new CC_UnLock_Cmd("", yyvsp[-1].lock_mode); ;
    break;}
case 12:
#line 79 "CC_command.y"
{ yyval.command = new CC_TryLock_Cmd(yyvsp[-2].id, yyvsp[-1].lock_mode); ;
    break;}
case 13:
#line 81 "CC_command.y"
{ yyval.command = new CC_TryLock_Cmd("", yyvsp[-1].lock_mode); ;
    break;}
case 14:
#line 83 "CC_command.y"
{ yyval.command = new CC_ChangeMode_Cmd(yyvsp[-3].id, yyvsp[-2].lock_mode, yyvsp[-1].lock_mode); ;
    break;}
case 15:
#line 85 "CC_command.y"
{ yyval.command = new CC_ChangeMode_Cmd("", yyvsp[-2].lock_mode, yyvsp[-1].lock_mode); ;
    break;}
case 16:
#line 87 "CC_command.y"
{ yyval.command = new CC_Sleep_Cmd(yyvsp[-1].num); ;
    break;}
case 17:
#line 89 "CC_command.y"
{ yyval.command = new CC_Wait_Cmd(""); ;
    break;}
case 18:
#line 91 "CC_command.y"
{ yyval.command = new CC_Wait_Cmd(yyvsp[-1].id); ;
    break;}
case 19:
#line 93 "CC_command.y"
{ yyval.command = new CC_Print_Cmd(yyvsp[-1].id); ;
    break;}
case 20:
#line 95 "CC_command.y"
{ yyval.command = new CC_Lookup_Cmd(yyvsp[-1].id); ;
    break;}
case 21:
#line 97 "CC_command.y"
{ cmdlist->setrepeat(yyvsp[-1].num);
                          yyval.command = new CC_Dummy_Cmd();;
    break;}
case 22:
#line 100 "CC_command.y"
{ yyval.command = new CC_Excep_Cmd(yyvsp[-1].id); ;
    break;}
case 23:
#line 103 "CC_command.y"
{yyval.id = yyvsp[0].id; ;
    break;}
case 24:
#line 105 "CC_command.y"
{yyval.id = yyvsp[0].id; ;
    break;}
case 25:
#line 107 "CC_command.y"
{yyval.id = yyvsp[0].id; ;
    break;}
case 26:
#line 109 "CC_command.y"
{ yyval.lock_mode = CosConcurrencyControl::read; ;
    break;}
case 27:
#line 110 "CC_command.y"
{ yyval.lock_mode = CosConcurrencyControl::intention_read; ;
    break;}
case 28:
#line 111 "CC_command.y"
{ yyval.lock_mode = CosConcurrencyControl::upgrade; ;
    break;}
case 29:
#line 112 "CC_command.y"
{ yyval.lock_mode = CosConcurrencyControl::write; ;
    break;}
case 30:
#line 113 "CC_command.y"
{ yyval.lock_mode = CosConcurrencyControl::intention_write; ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 457 "/pkg/gnu/lib/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 115 "CC_command.y"


int CC_commanderror(char *s)
{
  printf("%d: %s at %s in:\n%s\n",
         line_no, s, CC_commandtext, line_buf);
  ACE_OS::exit(-1);
  return 0;
}
