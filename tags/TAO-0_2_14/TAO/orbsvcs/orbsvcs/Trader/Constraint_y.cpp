
# line 2 "Trader/constraint.y"
  // $Id$
// ========================================================================
//
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    constraint.y
//
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
//
// ========================================================================

#include "ace/OS.h"
#include "Constraint_Interpreter.h"
#include "Constraint_Nodes.h"

//#define YYDEBUG 1
# define TAO_GT 257
# define TAO_GE 258
# define TAO_LT 259
# define TAO_LE 260
# define TAO_EQ 261
# define TAO_NE 262
# define TAO_EXIST 263
# define TAO_AND 264
# define TAO_OR 265
# define TAO_NOT 266
# define TAO_IN 267
# define TAO_TWIDDLE 268
# define TAO_BOOLEAN 269
# define TAO_PLUS 270
# define TAO_MINUS 271
# define TAO_MULT 272
# define TAO_DIV 273
# define TAO_UMINUS 274
# define TAO_NUMBER 275
# define TAO_RPAREN 276
# define TAO_LPAREN 277
# define TAO_IDENT 278
# define TAO_STRING 279
# define TAO_UNKNOWN 280
# define TAO_UNSIGNED 281
# define TAO_SIGNED 282
# define TAO_DOUBLE 283
# define TAO_CONSTRAINT 284
# define TAO_SEQUENCE 285
# define TAO_WITH 286
# define TAO_MAX 287
# define TAO_MIN 288
# define TAO_FIRST 289
# define TAO_RANDOM 290

#ifdef __cplusplus

#ifndef yyerror
	void yyerror(const char *);
#endif

#ifndef yylex
#ifdef __EXTERN_C__
	extern "C" { int yylex(void); }
#else
	int yylex(void);
#endif
#endif
	int yyparse(void);

#endif
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
YYSTYPE yylval;
YYSTYPE yyval;
typedef int yytabelem;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#if YYMAXDEPTH > 0
int yy_yys[YYMAXDEPTH], *yys = yy_yys;
YYSTYPE yy_yyv[YYMAXDEPTH], *yyv = yy_yyv;
#else	/* user does initial allocation */
int *yys;
YYSTYPE *yyv;
#endif
static int yymaxdepth = YYMAXDEPTH;
# define YYERRCODE 256

# line 133 "Trader/constraint.y"


//extern int yydebug = 1;
yytabelem yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 38
# define YYLAST 93
yytabelem yyact[]={

    19,    54,    44,    16,    45,    19,    24,    37,    22,    38,
    39,    24,    21,    22,    18,    20,    23,    21,    25,    18,
    20,    23,    36,     7,     6,     5,     8,     9,    19,    60,
    26,    16,    40,    41,    24,    25,    22,    38,    39,    15,
    21,    11,    18,    20,    23,    32,    33,    34,    35,    30,
    31,    14,    10,    17,    13,    12,     4,     3,     1,     2,
     0,     0,     0,     0,     0,    27,    28,    29,     0,     0,
    42,     0,    48,    49,    50,    51,    52,    53,    43,    47,
    58,    59,    46,     0,     0,     0,     0,     0,     0,     0,
    56,    57,    55 };
yytabelem yypact[]={

  -263,-10000000,  -230,-10000000,  -234,  -235,  -235,  -235,-10000000,-10000000,
-10000000,  -212,  -245,  -261,  -240,-10000000,  -258,-10000000,  -235,  -276,
-10000000,-10000000,  -271,-10000000,-10000000,  -235,  -235,  -230,  -230,  -230,
  -235,  -235,  -235,  -235,  -235,  -235,  -277,  -235,  -235,  -235,
  -235,  -235,-10000000,  -247,-10000000,-10000000,  -234,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,  -233,  -240,  -240,-10000000,-10000000,
-10000000 };
yytabelem yypgo[]={

     0,    58,    57,    59,    56,    52,    41,    55,    54,    51,
    39,    53 };
yytabelem yyr1[]={

     0,     1,     1,     2,     2,     2,     2,     2,     3,     3,
     4,     4,     5,     5,     5,     5,     5,     5,     5,     6,
     6,     7,     7,     8,     8,     8,     9,     9,     9,    10,
    10,    11,    11,    11,    11,    11,    11,    11 };
yytabelem yyr2[]={

     0,     3,     3,     5,     5,     5,     3,     3,     7,     3,
     7,     3,     7,     7,     7,     7,     7,     7,     3,     7,
     3,     7,     3,     7,     7,     3,     7,     7,     3,     5,
     3,     7,     5,     3,     3,     5,     3,     3 };
yytabelem yychk[]={

-10000000,    -1,    -3,    -2,    -4,   288,   287,   286,   289,   290,
    -5,    -6,    -7,    -8,    -9,   -10,   266,   -11,   277,   263,
   278,   275,   271,   279,   269,   265,   264,    -3,    -3,    -3,
   261,   262,   257,   258,   259,   260,   267,   268,   270,   271,
   272,   273,   -11,    -3,   278,   275,    -4,    -5,    -6,    -6,
    -6,    -6,    -6,    -6,   278,    -8,    -9,    -9,   -10,   -10,
   276 };
yytabelem yydef[]={

     0,    -2,     1,     2,     9,     0,     0,     0,     6,     7,
    11,    18,    20,    22,    25,    28,     0,    30,     0,     0,
    33,    34,     0,    36,    37,     0,     0,     3,     4,     5,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    29,     0,    32,    35,     8,    10,    12,    13,
    14,    15,    16,    17,    19,    21,    23,    24,    26,    27,
    31 };
typedef struct
#ifdef __cplusplus
	yytoktype
#endif
{ char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
{
	"TAO_GT",	257,
	"TAO_GE",	258,
	"TAO_LT",	259,
	"TAO_LE",	260,
	"TAO_EQ",	261,
	"TAO_NE",	262,
	"TAO_EXIST",	263,
	"TAO_AND",	264,
	"TAO_OR",	265,
	"TAO_NOT",	266,
	"TAO_IN",	267,
	"TAO_TWIDDLE",	268,
	"TAO_BOOLEAN",	269,
	"TAO_PLUS",	270,
	"TAO_MINUS",	271,
	"TAO_MULT",	272,
	"TAO_DIV",	273,
	"TAO_UMINUS",	274,
	"TAO_NUMBER",	275,
	"TAO_RPAREN",	276,
	"TAO_LPAREN",	277,
	"TAO_IDENT",	278,
	"TAO_STRING",	279,
	"TAO_UNKNOWN",	280,
	"TAO_UNSIGNED",	281,
	"TAO_SIGNED",	282,
	"TAO_DOUBLE",	283,
	"TAO_CONSTRAINT",	284,
	"TAO_SEQUENCE",	285,
	"TAO_WITH",	286,
	"TAO_MAX",	287,
	"TAO_MIN",	288,
	"TAO_FIRST",	289,
	"TAO_RANDOM",	290,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"constraint : bool_or",
	"constraint : preference",
	"preference : TAO_MIN bool_or",
	"preference : TAO_MAX bool_or",
	"preference : TAO_WITH bool_or",
	"preference : TAO_FIRST",
	"preference : TAO_RANDOM",
	"bool_or : bool_or TAO_OR bool_and",
	"bool_or : bool_and",
	"bool_and : bool_and TAO_AND bool_compare",
	"bool_and : bool_compare",
	"bool_compare : expr_in TAO_EQ expr_in",
	"bool_compare : expr_in TAO_NE expr_in",
	"bool_compare : expr_in TAO_GT expr_in",
	"bool_compare : expr_in TAO_GE expr_in",
	"bool_compare : expr_in TAO_LT expr_in",
	"bool_compare : expr_in TAO_LE expr_in",
	"bool_compare : expr_in",
	"expr_in : expr_twiddle TAO_IN TAO_IDENT",
	"expr_in : expr_twiddle",
	"expr_twiddle : expr TAO_TWIDDLE expr",
	"expr_twiddle : expr",
	"expr : expr TAO_PLUS term",
	"expr : expr TAO_MINUS term",
	"expr : term",
	"term : term TAO_MULT factor_not",
	"term : term TAO_DIV factor_not",
	"term : factor_not",
	"factor_not : TAO_NOT factor",
	"factor_not : factor",
	"factor : TAO_LPAREN bool_or TAO_RPAREN",
	"factor : TAO_EXIST TAO_IDENT",
	"factor : TAO_IDENT",
	"factor : TAO_NUMBER",
	"factor : TAO_MINUS TAO_NUMBER",
	"factor : TAO_STRING",
	"factor : TAO_BOOLEAN",
};
#endif /* YYDEBUG */
# line	1 "/usr/ccs/bin/yaccpar"
/*
 * Copyright (c) 1993 by Sun Microsystems, Inc.
 */

//#pragma ident	"@(#)yaccpar	6.12	93/06/07 SMI"

/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define YYERROR		goto yyerrlab
#define YYACCEPT	return(0)
#define YYABORT		return(1)
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
	{\
		yyerror( "syntax error - cannot backup" );\
		goto yyerrlab;\
	}\
	yychar = newtoken;\
	yystate = *yyps;\
	yylval = newvalue;\
	goto yynewstate;\
}
#define YYRECOVERING()	(!!yyerrflag)
#define YYNEW(type)	malloc(sizeof(type) * yynewmax)
#define YYCOPY(to, from, type) \
	(type *) memcpy(to, (char *) from, yynewmax * sizeof(type))
#define YYENLARGE( from, type) \
	(type *) realloc((char *) from, yynewmax * sizeof(type))
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int yydebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-10000000)

/*
** global variables used by the parser
*/
YYSTYPE *yypv;			/* top of value stack */
int *yyps;			/* top of state stack */

int yystate;			/* current state */
int yytmp;			/* extra var (lasts between blocks) */

int yynerrs;			/* number of errors */
int yyerrflag;			/* error recovery flag */
int yychar;			/* current input token number */



#ifdef YYNMBCHARS
#define YYLEX()		yycvtok(yylex())
/*
** yycvtok - return a token if i is a wchar_t value that exceeds 255.
**	If i<255, i itself is the token.  If i>255 but the neither 
**	of the 30th or 31st bit is on, i is already a token.
*/
#if defined(__STDC__) || defined(__cplusplus)
int yycvtok(int i)
#else
int yycvtok(i) int i;
#endif
{
	int first = 0;
	int last = YYNMBCHARS - 1;
	int mid;
	wchar_t j;

	if(i&0x60000000){/*Must convert to a token. */
		if( yymbchars[last].character < i ){
			return i;/*Giving up*/
		}
		while ((last>=first)&&(first>=0)) {/*Binary search loop*/
			mid = (first+last)/2;
			j = yymbchars[mid].character;
			if( j==i ){/*Found*/ 
				return yymbchars[mid].tvalue;
			}else if( j<i ){
				first = mid + 1;
			}else{
				last = mid -1;
			}
		}
		/*No entry in the table.*/
		return i;/* Giving up.*/
	}else{/* i is already a token. */
		return i;
	}
}
#else/*!YYNMBCHARS*/
#define YYLEX()		yylex()
#endif/*!YYNMBCHARS*/

/*
** yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
#if defined(__STDC__) || defined(__cplusplus)
int yyparse(void)
#else
int yyparse()
#endif
{
	register YYSTYPE *yypvt;	/* top of value stack for $vars */

#if defined(__cplusplus) || defined(lint)
/*
	hacks to please C++ and lint - goto's inside switch should never be
	executed; yypvt is set to 0 to avoid "used before set" warning.
*/
	static int __yaccpar_lint_hack__ = 0;
	switch (__yaccpar_lint_hack__)
	{
		case 1: goto yyerrlab;
		case 2: goto yynewstate;
	}
	yypvt = 0;
#endif

	/*
	** Initialize externals - yyparse may be called more than once
	*/
	yypv = &yyv[-1];
	yyps = &yys[-1];
	yystate = 0;
	yytmp = 0;
	yynerrs = 0;
	yyerrflag = 0;
	yychar = -1;

#if YYMAXDEPTH <= 0
	if (yymaxdepth <= 0)
	{
		if ((yymaxdepth = YYEXPAND(0)) <= 0)
		{
			yyerror("yacc initialization error");
			YYABORT;
		}
	}
#endif

	{
		register YYSTYPE *yy_pv;	/* top of value stack */
		register int *yy_ps;		/* top of state stack */
		register int yy_state;		/* current state */
		register int  yy_n;		/* internal state number info */
	goto yystack;	/* moved from 6 lines above to here to please C++ */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	yynewstate:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;
		goto yy_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	yystack:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	yy_stack:
		/*
		** put a state and value onto the stacks
		*/
#if YYDEBUG
		/*
		** if debugging, look up token value in list of value vs.
		** name pairs.  0 and negative (-1) are special values.
		** Note: linear search is used since time is not a real
		** consideration while debugging.
		*/
		if ( yydebug )
		{
			register int yy_i;

			printf( "State %d, token ", yy_state );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++yy_ps >= &yys[ yymaxdepth ] )	/* room on stack? */
		{
			/*
			** reallocate and recover.  Note that pointers
			** have to be reset, or bad things will happen
			*/
			int yyps_index = (yy_ps - yys);
			int yypv_index = (yy_pv - yyv);
			int yypvt_index = (yypvt - yyv);
			int yynewmax;
#ifdef YYEXPAND
			yynewmax = YYEXPAND(yymaxdepth);
#else
			yynewmax = 2 * yymaxdepth;	/* double table size */
			if (yymaxdepth == YYMAXDEPTH)	/* first time growth */
			{
				char *newyys = (char *)YYNEW(int);
				char *newyyv = (char *)YYNEW(YYSTYPE);
				if (newyys != 0 && newyyv != 0)
				{
					yys = YYCOPY(newyys, yys, int);
					yyv = YYCOPY(newyyv, yyv, YYSTYPE);
				}
				else
					yynewmax = 0;	/* failed */
			}
			else				/* not first time */
			{
				yys = YYENLARGE(yys, int);
				yyv = YYENLARGE(yyv, YYSTYPE);
				if (yys == 0 || yyv == 0)
					yynewmax = 0;	/* failed */
			}
#endif
			if (yynewmax <= yymaxdepth)	/* tables not expanded */
			{
				yyerror( "yacc stack overflow" );
				YYABORT;
			}
			yymaxdepth = yynewmax;

			yy_ps = yys + yyps_index;
			yy_pv = yyv + yypv_index;
			yypvt = yyv + yypvt_index;
		}
		*yy_ps = yy_state;
		*++yy_pv = yyval;

		/*
		** we have a new state - find out what to do
		*/
	yy_newstate:
		if ( ( yy_n = yypact[ yy_state ] ) <= YYFLAG )
			goto yydefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		yytmp = yychar < 0;
#endif
		if ( ( yychar < 0 ) && ( ( yychar = YYLEX() ) < 0 ) )
			yychar = 0;		/* reached EOF */
#if YYDEBUG
		if ( yydebug && yytmp )
		{
			register int yy_i;

			printf( "Received token " );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( yy_n += yychar ) < 0 ) || ( yy_n >= YYLAST ) )
			goto yydefault;
		if ( yychk[ yy_n = yyact[ yy_n ] ] == yychar )	/*valid shift*/
		{
			yychar = -1;
			yyval = yylval;
			yy_state = yy_n;
			if ( yyerrflag > 0 )
				yyerrflag--;
			goto yy_stack;
		}

	yydefault:
		if ( ( yy_n = yydef[ yy_state ] ) == -2 )
		{
#if YYDEBUG
			yytmp = yychar < 0;
#endif
			if ( ( yychar < 0 ) && ( ( yychar = YYLEX() ) < 0 ) )
				yychar = 0;		/* reached EOF */
#if YYDEBUG
			if ( yydebug && yytmp )
			{
				register int yy_i;

				printf( "Received token " );
				if ( yychar == 0 )
					printf( "end-of-file\n" );
				else if ( yychar < 0 )
					printf( "-none-\n" );
				else
				{
					for ( yy_i = 0;
						yytoks[yy_i].t_val >= 0;
						yy_i++ )
					{
						if ( yytoks[yy_i].t_val
							== yychar )
						{
							break;
						}
					}
					printf( "%s\n", yytoks[yy_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register int *yyxi = yyexca;

				while ( ( *yyxi != -1 ) ||
					( yyxi[1] != yy_state ) )
				{
					yyxi += 2;
				}
				while ( ( *(yyxi += 2) >= 0 ) &&
					( *yyxi != yychar ) )
					;
				if ( ( yy_n = yyxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( yy_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( yyerrflag )
			{
			case 0:		/* new error */
				yyerror( "syntax error" );
				goto skip_init;
			yyerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				yy_pv = yypv;
				yy_ps = yyps;
				yy_state = yystate;
			skip_init:
				yynerrs++;
				/* FALLTHRU */
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				yyerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( yy_ps >= yys )
				{
					yy_n = yypact[ *yy_ps ] + YYERRCODE;
					if ( yy_n >= 0 && yy_n < YYLAST &&
						yychk[yyact[yy_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						yy_state = yyact[ yy_n ];
						goto yy_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( yydebug )
						printf( _POP_, *yy_ps,
							yy_ps[-1] );
#	undef _POP_
#endif
					yy_ps--;
					yy_pv--;
				}
				/*
				** there is no state on stack with "error" as
				** a valid shift.  give up.
				*/
				YYABORT;
			case 3:		/* no shift yet; eat a token */
#if YYDEBUG
				/*
				** if debugging, look up token in list of
				** pairs.  0 and negative shouldn't occur,
				** but since timing doesn't matter when
				** debugging, it doesn't hurt to leave the
				** tests here.
				*/
				if ( yydebug )
				{
					register int yy_i;

					printf( "Error recovery discards " );
					if ( yychar == 0 )
						printf( "token end-of-file\n" );
					else if ( yychar < 0 )
						printf( "token -none-\n" );
					else
					{
						for ( yy_i = 0;
							yytoks[yy_i].t_val >= 0;
							yy_i++ )
						{
							if ( yytoks[yy_i].t_val
								== yychar )
							{
								break;
							}
						}
						printf( "token %s\n",
							yytoks[yy_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( yychar == 0 )	/* reached EOF. quit */
					YYABORT;
				yychar = -1;
				goto yy_newstate;
			}
		}/* end if ( yy_n == 0 ) */
		/*
		** reduction by production yy_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( yydebug )
			printf( "Reduce by (%d) \"%s\"\n",
				yy_n, yyreds[ yy_n ] );
#endif
		yytmp = yy_n;			/* value to switch over */
		yypvt = yy_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using yy_state here as temporary
		** register variable, but why not, if it works...
		** If yyr2[ yy_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto yy_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int yy_len = yyr2[ yy_n ];

			if ( !( yy_len & 01 ) )
			{
				yy_len >>= 1;
				yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
				yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
					*( yy_ps -= yy_len ) + 1;
				if ( yy_state >= YYLAST ||
					yychk[ yy_state =
					yyact[ yy_state ] ] != -yy_n )
				{
					yy_state = yyact[ yypgo[ yy_n ] ];
				}
				goto yy_stack;
			}
			yy_len >>= 1;
			yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
			yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
				*( yy_ps -= yy_len ) + 1;
			if ( yy_state >= YYLAST ||
				yychk[ yy_state = yyact[ yy_state ] ] != -yy_n )
			{
				yy_state = yyact[ yypgo[ yy_n ] ];
			}
		}
					/* save until reenter driver code */
		yystate = yy_state;
		yyps = yy_ps;
		yypv = yy_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( yytmp )
	{
		
case 1:
# line 38 "Trader/constraint.y"
{ yyval.constraint_ = new TAO_Unary_Constraint(TAO_CONSTRAINT, yypvt[-0].constraint_); } break;
case 2:
# line 40 "Trader/constraint.y"
{ yyval.constraint_ = yypvt[-0].constraint_; } break;
case 3:
# line 44 "Trader/constraint.y"
{ yyval.constraint_ = new TAO_Unary_Constraint(TAO_MIN, yypvt[-0].constraint_); } break;
case 4:
# line 46 "Trader/constraint.y"
{ yyval.constraint_ = new TAO_Unary_Constraint(TAO_MAX, yypvt[-0].constraint_); } break;
case 5:
# line 48 "Trader/constraint.y"
{ yyval.constraint_ = new TAO_Unary_Constraint(TAO_WITH, yypvt[-0].constraint_); } break;
case 6:
# line 50 "Trader/constraint.y"
{ yyval.constraint_ = new TAO_Noop_Constraint(TAO_FIRST); } break;
case 7:
# line 52 "Trader/constraint.y"
{ yyval.constraint_ = new TAO_Noop_Constraint(TAO_RANDOM); } break;
case 8:
# line 56 "Trader/constraint.y"
{ yyval.constraint_ = new TAO_Binary_Constraint(TAO_OR, yypvt[-2].constraint_, yypvt[-0].constraint_); } break;
case 9:
# line 58 "Trader/constraint.y"
{ yyval.constraint_ = yypvt[-0].constraint_; } break;
case 10:
# line 62 "Trader/constraint.y"
{ yyval.constraint_ = new TAO_Binary_Constraint(TAO_AND, yypvt[-2].constraint_, yypvt[-0].constraint_); } break;
case 11:
# line 64 "Trader/constraint.y"
{ yyval.constraint_ = yypvt[-0].constraint_; } break;
case 12:
# line 68 "Trader/constraint.y"
{ yyval.constraint_ = new TAO_Binary_Constraint(TAO_EQ, yypvt[-2].constraint_, yypvt[-0].constraint_); } break;
case 13:
# line 70 "Trader/constraint.y"
{ yyval.constraint_ = new TAO_Binary_Constraint(TAO_NE, yypvt[-2].constraint_, yypvt[-0].constraint_); } break;
case 14:
# line 72 "Trader/constraint.y"
{ yyval.constraint_ = new TAO_Binary_Constraint(TAO_GT, yypvt[-2].constraint_, yypvt[-0].constraint_); } break;
case 15:
# line 74 "Trader/constraint.y"
{ yyval.constraint_ = new TAO_Binary_Constraint(TAO_GE, yypvt[-2].constraint_, yypvt[-0].constraint_); } break;
case 16:
# line 76 "Trader/constraint.y"
{ yyval.constraint_ = new TAO_Binary_Constraint(TAO_LT, yypvt[-2].constraint_, yypvt[-0].constraint_); } break;
case 17:
# line 78 "Trader/constraint.y"
{ yyval.constraint_ = new TAO_Binary_Constraint(TAO_LE, yypvt[-2].constraint_, yypvt[-0].constraint_); } break;
case 18:
# line 80 "Trader/constraint.y"
{ yyval.constraint_ = yypvt[-0].constraint_; } break;
case 19:
# line 84 "Trader/constraint.y"
{ yyval.constraint_ = new TAO_Binary_Constraint(TAO_IN, yypvt[-2].constraint_, yypvt[-0].constraint_); } break;
case 20:
# line 86 "Trader/constraint.y"
{ yyval.constraint_ = yypvt[-0].constraint_; } break;
case 21:
# line 90 "Trader/constraint.y"
{ yyval.constraint_ = new TAO_Binary_Constraint(TAO_TWIDDLE, yypvt[-2].constraint_, yypvt[-0].constraint_); } break;
case 22:
# line 92 "Trader/constraint.y"
{ yyval.constraint_ = yypvt[-0].constraint_; } break;
case 23:
# line 96 "Trader/constraint.y"
{ yyval.constraint_ = new TAO_Binary_Constraint(TAO_PLUS, yypvt[-2].constraint_, yypvt[-0].constraint_); } break;
case 24:
# line 98 "Trader/constraint.y"
{ yyval.constraint_ = new TAO_Binary_Constraint(TAO_MINUS, yypvt[-2].constraint_, yypvt[-0].constraint_); } break;
case 25:
# line 100 "Trader/constraint.y"
{ yyval.constraint_ = yypvt[-0].constraint_; } break;
case 26:
# line 104 "Trader/constraint.y"
{ yyval.constraint_ = new TAO_Binary_Constraint(TAO_MULT, yypvt[-2].constraint_, yypvt[-0].constraint_); } break;
case 27:
# line 106 "Trader/constraint.y"
{ yyval.constraint_ = new TAO_Binary_Constraint(TAO_DIV, yypvt[-2].constraint_, yypvt[-0].constraint_); } break;
case 28:
# line 108 "Trader/constraint.y"
{ yyval.constraint_ = yypvt[-0].constraint_; } break;
case 29:
# line 112 "Trader/constraint.y"
{ yyval.constraint_ = new TAO_Unary_Constraint(TAO_NOT, yypvt[-0].constraint_); } break;
case 30:
# line 114 "Trader/constraint.y"
{ yyval.constraint_ = yypvt[-0].constraint_; } break;
case 31:
# line 118 "Trader/constraint.y"
{ yyval.constraint_ = yypvt[-1].constraint_; } break;
case 32:
# line 120 "Trader/constraint.y"
{ yyval.constraint_ = new TAO_Unary_Constraint(TAO_EXIST, yypvt[-0].constraint_); } break;
case 33:
# line 122 "Trader/constraint.y"
{ yyval.constraint_ = yypvt[-0].constraint_; } break;
case 34:
# line 124 "Trader/constraint.y"
{ yyval.constraint_ = yypvt[-0].constraint_; } break;
case 35:
# line 126 "Trader/constraint.y"
{ yyval.constraint_ = new TAO_Unary_Constraint(TAO_UMINUS, yypvt[-0].constraint_); } break;
case 36:
# line 128 "Trader/constraint.y"
{ yyval.constraint_ = yypvt[-0].constraint_; } break;
case 37:
# line 130 "Trader/constraint.y"
{ yyval.constraint_ = yypvt[-0].constraint_; } break;
# line	532 "/usr/ccs/bin/yaccpar"
	}
	goto yystack;		/* reset registers in driver code */
}

