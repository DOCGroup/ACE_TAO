// $Id$
#include "CC_command.h"
#include "ace/ACE.h"
#include "orbsvcs/CosConcurrencyControlC.h"

void ace_cc_yyerror (const char* s);
int ace_cc_yylex(void);

extern int line_no;
extern char line_buf[500];
extern char *ace_cc_yytext;
extern CC_CommandList *cmdlist;
typedef union
#ifdef __cplusplus
	ACE_CC_YYSTYPE
#endif
 {
  CC_Command *command;
  CosConcurrencyControl::lock_mode lock_mode;
  char *id;
  int num;
} ACE_CC_YYSTYPE;
# define T_START_CMD 257
# define T_CREATE_CMD 258
# define T_LOCK_CMD 259
# define T_UNLOCK_CMD 260
# define T_TRYLOCK_CMD 261
# define T_CHANGEMODE_CMD 262
# define T_SLEEP_CMD 263
# define T_WAIT_CMD 264
# define T_REPEAT_CMD 265
# define T_EXCEP_CMD 266
# define T_PRINT_CMD 267
# define T_LOOKUP_CMD 268
# define T_TERM 269
# define T_READ 270
# define T_IREAD 271
# define T_UPGRADE 272
# define T_WRITE 273
# define T_IWRITE 274
# define T_IDENT 275
# define Q_STRING 276
# define T_NUM 277

#include <inttypes.h>

#ifdef __STDC__
#include <stdlib.h>
#include <string.h>
#else
#include <malloc.h>
#include <memory.h>
#endif

#include <values.h>

#if defined(__cplusplus) || defined(__STDC__)

#if defined(__cplusplus) && defined(__EXTERN_C__)
extern "C" {
#endif
#ifndef ace_cc_yyerror
#if defined(__cplusplus)
	void ace_cc_yyerror(const char *);
#endif
#endif
#ifndef ace_cc_yylex
	int ace_cc_yylex(void);
#endif
	int ace_cc_yyparse(void);
#if defined(__cplusplus) && defined(__EXTERN_C__)
}
#endif

#endif

#define ace_cc_yyclearin ace_cc_yychar = -1
#define ace_cc_yyerrok ace_cc_yyerrflag = 0
extern int ace_cc_yychar;
extern int ace_cc_yyerrflag;
ACE_CC_YYSTYPE ace_cc_yylval;
ACE_CC_YYSTYPE ace_cc_yyval;
typedef int ace_cc_yytabelem;
#ifndef ACE_CC_YYMAXDEPTH
#define ACE_CC_YYMAXDEPTH 150
#endif
#if ACE_CC_YYMAXDEPTH > 0
int ace_cc_yy_ace_cc_yys[ACE_CC_YYMAXDEPTH], *ace_cc_yys = ace_cc_yy_ace_cc_yys;
ACE_CC_YYSTYPE ace_cc_yy_ace_cc_yyv[ACE_CC_YYMAXDEPTH], *ace_cc_yyv = ace_cc_yy_ace_cc_yyv;
#else	/* user does initial allocation */
int *ace_cc_yys;
ACE_CC_YYSTYPE *ace_cc_yyv;
#endif
static int ace_cc_yymaxdepth = ACE_CC_YYMAXDEPTH;
# define ACE_CC_YYERRCODE 256


void
ace_cc_yyerror (const char *s)
{
  ACE_OS::printf ("%d: %s at %s in:\n%s\n",
                  line_no,
                  s,
                  ace_cc_yytext,
                  line_buf);
  ACE_OS::exit (-1);
}
static const ace_cc_yytabelem ace_cc_yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define ACE_CC_YYNPROD 31
# define ACE_CC_YYLAST 102
static const ace_cc_yytabelem ace_cc_yyact[]={

    24,    25,    26,    27,    28,    21,     4,     5,     6,     7,
     8,     9,    10,    11,    14,    15,    12,    13,    24,    25,
    26,    27,    28,    40,    36,    64,    20,    35,    38,    42,
    63,    37,    21,    21,    18,    61,    60,    59,    58,    57,
    56,    55,    54,    53,    50,    48,    46,    44,    43,    23,
     2,     3,     1,    19,    16,    41,    17,    30,    32,    34,
    22,    29,    31,    33,     0,     0,     0,    39,     0,     0,
     0,     0,    45,     0,     0,     0,     0,     0,     0,    47,
     0,    49,     0,    51,    52,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    62 };
static const ace_cc_yytabelem ace_cc_yypact[]={

  -251,-10000000,  -251,-10000000,  -241,  -243,  -270,  -270,  -270,  -270,
  -250,  -245,  -248,  -242,  -254,  -246,-10000000,  -221,-10000000,  -222,
-10000000,-10000000,  -252,  -223,-10000000,-10000000,-10000000,-10000000,-10000000,  -252,
  -224,  -252,  -225,  -252,  -252,  -226,-10000000,  -227,  -228,  -229,
  -230,  -231,-10000000,-10000000,-10000000,  -232,-10000000,  -233,-10000000,  -234,
-10000000,  -252,  -239,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,  -244,-10000000,-10000000 };
static const ace_cc_yytabelem ace_cc_yypgo[]={

     0,    51,    56,    53,    55,    49,    52,    50 };
static const ace_cc_yytabelem ace_cc_yyr1[]={

     0,     6,     6,     7,     7,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     2,     3,     4,     5,     5,     5,     5,
     5 };
static const ace_cc_yytabelem ace_cc_yyr2[]={

     0,     0,     3,     5,     3,     7,     7,     5,     9,     7,
     9,     7,     9,     7,    11,     9,     7,     5,     7,     7,
     7,     7,     7,     3,     3,     3,     3,     3,     3,     3,
     3 };
static const ace_cc_yytabelem ace_cc_yychk[]={

-10000000,    -6,    -7,    -1,   257,   258,   259,   260,   261,   262,
   263,   264,   267,   268,   265,   266,    -1,    -2,   275,    -3,
   269,   275,    -3,    -5,   270,   271,   272,   273,   274,    -3,
    -5,    -3,    -5,    -3,    -5,   277,   269,   276,   276,    -3,
   277,    -4,   275,   269,   269,    -5,   269,    -5,   269,    -5,
   269,    -5,    -5,   269,   269,   269,   269,   269,   269,   269,
   269,   269,    -5,   269,   269 };
static const ace_cc_yytabelem ace_cc_yydef[]={

     1,    -2,     2,     4,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     3,     0,    23,     0,
     7,    24,     0,     0,    26,    27,    28,    29,    30,     0,
     0,     0,     0,     0,     0,     0,    17,     0,     0,     0,
     0,     0,    25,     5,     6,     0,     9,     0,    11,     0,
    13,     0,     0,    16,    18,    19,    20,    21,    22,     8,
    10,    12,     0,    15,    14 };
typedef struct
#ifdef __cplusplus
	ace_cc_yytoktype
#endif
{ char *t_name; int t_val; } ace_cc_yytoktype;
#ifndef ACE_CC_YYDEBUG
#	define ACE_CC_YYDEBUG	0	/* don't allow debugging */
#endif

#if ACE_CC_YYDEBUG

ace_cc_yytoktype ace_cc_yytoks[] =
{
	"T_START_CMD",	257,
	"T_CREATE_CMD",	258,
	"T_LOCK_CMD",	259,
	"T_UNLOCK_CMD",	260,
	"T_TRYLOCK_CMD",	261,
	"T_CHANGEMODE_CMD",	262,
	"T_SLEEP_CMD",	263,
	"T_WAIT_CMD",	264,
	"T_REPEAT_CMD",	265,
	"T_EXCEP_CMD",	266,
	"T_PRINT_CMD",	267,
	"T_LOOKUP_CMD",	268,
	"T_TERM",	269,
	"T_READ",	270,
	"T_IREAD",	271,
	"T_UPGRADE",	272,
	"T_WRITE",	273,
	"T_IWRITE",	274,
	"T_IDENT",	275,
	"Q_STRING",	276,
	"T_NUM",	277,
	"-unknown-",	-1	/* ends search */
};

char * ace_cc_yyreds[] =
{
	"-no such reduction-",
	"start : /* empty */",
	"start : cmd_list",
	"cmd_list : cmd_list cmd",
	"cmd_list : cmd",
	"cmd : T_START_CMD config_file_name T_TERM",
	"cmd : T_CREATE_CMD lock_set_name T_TERM",
	"cmd : T_CREATE_CMD T_TERM",
	"cmd : T_LOCK_CMD lock_set_name lock_mode T_TERM",
	"cmd : T_LOCK_CMD lock_mode T_TERM",
	"cmd : T_UNLOCK_CMD lock_set_name lock_mode T_TERM",
	"cmd : T_UNLOCK_CMD lock_mode T_TERM",
	"cmd : T_TRYLOCK_CMD lock_set_name lock_mode T_TERM",
	"cmd : T_TRYLOCK_CMD lock_mode T_TERM",
	"cmd : T_CHANGEMODE_CMD lock_set_name lock_mode lock_mode T_TERM",
	"cmd : T_CHANGEMODE_CMD lock_mode lock_mode T_TERM",
	"cmd : T_SLEEP_CMD T_NUM T_TERM",
	"cmd : T_WAIT_CMD T_TERM",
	"cmd : T_WAIT_CMD Q_STRING T_TERM",
	"cmd : T_PRINT_CMD Q_STRING T_TERM",
	"cmd : T_LOOKUP_CMD lock_set_name T_TERM",
	"cmd : T_REPEAT_CMD T_NUM T_TERM",
	"cmd : T_EXCEP_CMD exception_name T_TERM",
	"config_file_name : T_IDENT",
	"lock_set_name : T_IDENT",
	"exception_name : T_IDENT",
	"lock_mode : T_READ",
	"lock_mode : T_IREAD",
	"lock_mode : T_UPGRADE",
	"lock_mode : T_WRITE",
	"lock_mode : T_IWRITE",
};
#endif /* ACE_CC_YYDEBUG */
/*
 * Copyright (c) 1993 by Sun Microsystems, Inc.
 */

#pragma ident	"@(#)yaccpar	6.15	97/12/08 SMI"

/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define ACE_CC_YYERROR		goto ace_cc_yyerrlab
#define ACE_CC_YYACCEPT	return(0)
#define ACE_CC_YYABORT		return(1)
#define ACE_CC_YYBACKUP( newtoken, newvalue )\
{\
	if ( ace_cc_yychar >= 0 || ( ace_cc_yyr2[ ace_cc_yytmp ] >> 1 ) != 1 )\
	{\
		ace_cc_yyerror( "syntax error - cannot backup" );\
		goto ace_cc_yyerrlab;\
	}\
	ace_cc_yychar = newtoken;\
	ace_cc_yystate = *ace_cc_yyps;\
	ace_cc_yylval = newvalue;\
	goto ace_cc_yynewstate;\
}
#define ACE_CC_YYRECOVERING()	(!!ace_cc_yyerrflag)
#define ACE_CC_YYNEW(type)	malloc(sizeof(type) * ace_cc_yynewmax)
#define ACE_CC_YYCOPY(to, from, type) \
	(type *) memcpy(to, (char *) from, ace_cc_yymaxdepth * sizeof (type))
#define ACE_CC_YYENLARGE( from, type) \
	(type *) realloc((char *) from, ace_cc_yynewmax * sizeof(type))
#ifndef ACE_CC_YYDEBUG
#	define ACE_CC_YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int ace_cc_yydebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define ACE_CC_YYFLAG		(-10000000)

/*
** global variables used by the parser
*/
ACE_CC_YYSTYPE *ace_cc_yypv;			/* top of value stack */
int *ace_cc_yyps;			/* top of state stack */

int ace_cc_yystate;			/* current state */
int ace_cc_yytmp;			/* extra var (lasts between blocks) */

int ace_cc_yynerrs;			/* number of errors */
int ace_cc_yyerrflag;			/* error recovery flag */
int ace_cc_yychar;			/* current input token number */



#ifdef ACE_CC_YYNMBCHARS
#define ACE_CC_YYLEX()		ace_cc_yycvtok(ace_cc_yylex())
/*
** ace_cc_yycvtok - return a token if i is a wchar_t value that exceeds 255.
**	If i<255, i itself is the token.  If i>255 but the neither 
**	of the 30th or 31st bit is on, i is already a token.
*/
#if defined(__STDC__) || defined(__cplusplus)
int ace_cc_yycvtok(int i)
#else
int ace_cc_yycvtok(i) int i;
#endif
{
	int first = 0;
	int last = ACE_CC_YYNMBCHARS - 1;
	int mid;
	wchar_t j;

	if(i&0x60000000){/*Must convert to a token. */
		if( ace_cc_yymbchars[last].character < i ){
			return i;/*Giving up*/
		}
		while ((last>=first)&&(first>=0)) {/*Binary search loop*/
			mid = (first+last)/2;
			j = ace_cc_yymbchars[mid].character;
			if( j==i ){/*Found*/ 
				return ace_cc_yymbchars[mid].tvalue;
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
#else/*!ACE_CC_YYNMBCHARS*/
#define ACE_CC_YYLEX()		ace_cc_yylex()
#endif/*!ACE_CC_YYNMBCHARS*/

/*
** ace_cc_yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
#if defined(__STDC__) || defined(__cplusplus)
int ace_cc_yyparse(void)
#else
int ace_cc_yyparse()
#endif
{
	register ACE_CC_YYSTYPE *ace_cc_yypvt = 0;	/* top of value stack for $vars */

#if defined(__cplusplus) || defined(lint)
/*
	hacks to please C++ and lint - goto's inside
	switch should never be executed
*/
	static int __yaccpar_lint_hack__ = 0;
	switch (__yaccpar_lint_hack__)
	{
		case 1: goto ace_cc_yyerrlab;
		case 2: goto ace_cc_yynewstate;
	}
#endif

	/*
	** Initialize externals - ace_cc_yyparse may be called more than once
	*/
	ace_cc_yypv = &ace_cc_yyv[-1];
	ace_cc_yyps = &ace_cc_yys[-1];
	ace_cc_yystate = 0;
	ace_cc_yytmp = 0;
	ace_cc_yynerrs = 0;
	ace_cc_yyerrflag = 0;
	ace_cc_yychar = -1;

#if ACE_CC_YYMAXDEPTH <= 0
	if (ace_cc_yymaxdepth <= 0)
	{
		if ((ace_cc_yymaxdepth = ACE_CC_YYEXPAND(0)) <= 0)
		{
			ace_cc_yyerror("yacc initialization error");
			ACE_CC_YYABORT;
		}
	}
#endif

	{
		register ACE_CC_YYSTYPE *ace_cc_yy_pv;	/* top of value stack */
		register int *ace_cc_yy_ps;		/* top of state stack */
		register int ace_cc_yy_state;		/* current state */
		register int  ace_cc_yy_n;		/* internal state number info */
	goto ace_cc_yystack;	/* moved from 6 lines above to here to please C++ */

		/*
		** get globals into registers.
		** branch to here only if ACE_CC_YYBACKUP was called.
		*/
	ace_cc_yynewstate:
		ace_cc_yy_pv = ace_cc_yypv;
		ace_cc_yy_ps = ace_cc_yyps;
		ace_cc_yy_state = ace_cc_yystate;
		goto ace_cc_yy_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	ace_cc_yystack:
		ace_cc_yy_pv = ace_cc_yypv;
		ace_cc_yy_ps = ace_cc_yyps;
		ace_cc_yy_state = ace_cc_yystate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	ace_cc_yy_stack:
		/*
		** put a state and value onto the stacks
		*/
#if ACE_CC_YYDEBUG
		/*
		** if debugging, look up token value in list of value vs.
		** name pairs.  0 and negative (-1) are special values.
		** Note: linear search is used since time is not a real
		** consideration while debugging.
		*/
		if ( ace_cc_yydebug )
		{
			register int ace_cc_yy_i;

			printf( "State %d, token ", ace_cc_yy_state );
			if ( ace_cc_yychar == 0 )
				printf( "end-of-file\n" );
			else if ( ace_cc_yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( ace_cc_yy_i = 0; ace_cc_yytoks[ace_cc_yy_i].t_val >= 0;
					ace_cc_yy_i++ )
				{
					if ( ace_cc_yytoks[ace_cc_yy_i].t_val == ace_cc_yychar )
						break;
				}
				printf( "%s\n", ace_cc_yytoks[ace_cc_yy_i].t_name );
			}
		}
#endif /* ACE_CC_YYDEBUG */
		if ( ++ace_cc_yy_ps >= &ace_cc_yys[ ace_cc_yymaxdepth ] )	/* room on stack? */
		{
			/*
			** reallocate and recover.  Note that pointers
			** have to be reset, or bad things will happen
			*/
			long ace_cc_yyps_index = (ace_cc_yy_ps - ace_cc_yys);
			long ace_cc_yypv_index = (ace_cc_yy_pv - ace_cc_yyv);
			long ace_cc_yypvt_index = (ace_cc_yypvt - ace_cc_yyv);
			int ace_cc_yynewmax;
#ifdef ACE_CC_YYEXPAND
			ace_cc_yynewmax = ACE_CC_YYEXPAND(ace_cc_yymaxdepth);
#else
			ace_cc_yynewmax = 2 * ace_cc_yymaxdepth;	/* double table size */
			if (ace_cc_yymaxdepth == ACE_CC_YYMAXDEPTH)	/* first time growth */
			{
				char *newace_cc_yys = (char *)ACE_CC_YYNEW(int);
				char *newace_cc_yyv = (char *)ACE_CC_YYNEW(ACE_CC_YYSTYPE);
				if (newace_cc_yys != 0 && newace_cc_yyv != 0)
				{
					ace_cc_yys = ACE_CC_YYCOPY(newace_cc_yys, ace_cc_yys, int);
					ace_cc_yyv = ACE_CC_YYCOPY(newace_cc_yyv, ace_cc_yyv, ACE_CC_YYSTYPE);
				}
				else
					ace_cc_yynewmax = 0;	/* failed */
			}
			else				/* not first time */
			{
				ace_cc_yys = ACE_CC_YYENLARGE(ace_cc_yys, int);
				ace_cc_yyv = ACE_CC_YYENLARGE(ace_cc_yyv, ACE_CC_YYSTYPE);
				if (ace_cc_yys == 0 || ace_cc_yyv == 0)
					ace_cc_yynewmax = 0;	/* failed */
			}
#endif
			if (ace_cc_yynewmax <= ace_cc_yymaxdepth)	/* tables not expanded */
			{
				ace_cc_yyerror( "yacc stack overflow" );
				ACE_CC_YYABORT;
			}
			ace_cc_yymaxdepth = ace_cc_yynewmax;

			ace_cc_yy_ps = ace_cc_yys + ace_cc_yyps_index;
			ace_cc_yy_pv = ace_cc_yyv + ace_cc_yypv_index;
			ace_cc_yypvt = ace_cc_yyv + ace_cc_yypvt_index;
		}
		*ace_cc_yy_ps = ace_cc_yy_state;
		*++ace_cc_yy_pv = ace_cc_yyval;

		/*
		** we have a new state - find out what to do
		*/
	ace_cc_yy_newstate:
		if ( ( ace_cc_yy_n = ace_cc_yypact[ ace_cc_yy_state ] ) <= ACE_CC_YYFLAG )
			goto ace_cc_yydefault;		/* simple state */
#if ACE_CC_YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		ace_cc_yytmp = ace_cc_yychar < 0;
#endif
		if ( ( ace_cc_yychar < 0 ) && ( ( ace_cc_yychar = ACE_CC_YYLEX() ) < 0 ) )
			ace_cc_yychar = 0;		/* reached EOF */
#if ACE_CC_YYDEBUG
		if ( ace_cc_yydebug && ace_cc_yytmp )
		{
			register int ace_cc_yy_i;

			printf( "Received token " );
			if ( ace_cc_yychar == 0 )
				printf( "end-of-file\n" );
			else if ( ace_cc_yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( ace_cc_yy_i = 0; ace_cc_yytoks[ace_cc_yy_i].t_val >= 0;
					ace_cc_yy_i++ )
				{
					if ( ace_cc_yytoks[ace_cc_yy_i].t_val == ace_cc_yychar )
						break;
				}
				printf( "%s\n", ace_cc_yytoks[ace_cc_yy_i].t_name );
			}
		}
#endif /* ACE_CC_YYDEBUG */
		if ( ( ( ace_cc_yy_n += ace_cc_yychar ) < 0 ) || ( ace_cc_yy_n >= ACE_CC_YYLAST ) )
			goto ace_cc_yydefault;
		if ( ace_cc_yychk[ ace_cc_yy_n = ace_cc_yyact[ ace_cc_yy_n ] ] == ace_cc_yychar )	/*valid shift*/
		{
			ace_cc_yychar = -1;
			ace_cc_yyval = ace_cc_yylval;
			ace_cc_yy_state = ace_cc_yy_n;
			if ( ace_cc_yyerrflag > 0 )
				ace_cc_yyerrflag--;
			goto ace_cc_yy_stack;
		}

	ace_cc_yydefault:
		if ( ( ace_cc_yy_n = ace_cc_yydef[ ace_cc_yy_state ] ) == -2 )
		{
#if ACE_CC_YYDEBUG
			ace_cc_yytmp = ace_cc_yychar < 0;
#endif
			if ( ( ace_cc_yychar < 0 ) && ( ( ace_cc_yychar = ACE_CC_YYLEX() ) < 0 ) )
				ace_cc_yychar = 0;		/* reached EOF */
#if ACE_CC_YYDEBUG
			if ( ace_cc_yydebug && ace_cc_yytmp )
			{
				register int ace_cc_yy_i;

				printf( "Received token " );
				if ( ace_cc_yychar == 0 )
					printf( "end-of-file\n" );
				else if ( ace_cc_yychar < 0 )
					printf( "-none-\n" );
				else
				{
					for ( ace_cc_yy_i = 0;
						ace_cc_yytoks[ace_cc_yy_i].t_val >= 0;
						ace_cc_yy_i++ )
					{
						if ( ace_cc_yytoks[ace_cc_yy_i].t_val
							== ace_cc_yychar )
						{
							break;
						}
					}
					printf( "%s\n", ace_cc_yytoks[ace_cc_yy_i].t_name );
				}
			}
#endif /* ACE_CC_YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register const int *ace_cc_yyxi = ace_cc_yyexca;

				while ( ( *ace_cc_yyxi != -1 ) ||
					( ace_cc_yyxi[1] != ace_cc_yy_state ) )
				{
					ace_cc_yyxi += 2;
				}
				while ( ( *(ace_cc_yyxi += 2) >= 0 ) &&
					( *ace_cc_yyxi != ace_cc_yychar ) )
					;
				if ( ( ace_cc_yy_n = ace_cc_yyxi[1] ) < 0 )
					ACE_CC_YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( ace_cc_yy_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( ace_cc_yyerrflag )
			{
			case 0:		/* new error */
				ace_cc_yyerror( "syntax error" );
				goto skip_init;
			ace_cc_yyerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				ace_cc_yy_pv = ace_cc_yypv;
				ace_cc_yy_ps = ace_cc_yyps;
				ace_cc_yy_state = ace_cc_yystate;
			skip_init:
				ace_cc_yynerrs++;
				/* FALLTHRU */
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				ace_cc_yyerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( ace_cc_yy_ps >= ace_cc_yys )
				{
					ace_cc_yy_n = ace_cc_yypact[ *ace_cc_yy_ps ] + ACE_CC_YYERRCODE;
					if ( ace_cc_yy_n >= 0 && ace_cc_yy_n < ACE_CC_YYLAST &&
						ace_cc_yychk[ace_cc_yyact[ace_cc_yy_n]] == ACE_CC_YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						ace_cc_yy_state = ace_cc_yyact[ ace_cc_yy_n ];
						goto ace_cc_yy_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if ACE_CC_YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( ace_cc_yydebug )
						printf( _POP_, *ace_cc_yy_ps,
							ace_cc_yy_ps[-1] );
#	undef _POP_
#endif
					ace_cc_yy_ps--;
					ace_cc_yy_pv--;
				}
				/*
				** there is no state on stack with "error" as
				** a valid shift.  give up.
				*/
				ACE_CC_YYABORT;
			case 3:		/* no shift yet; eat a token */
#if ACE_CC_YYDEBUG
				/*
				** if debugging, look up token in list of
				** pairs.  0 and negative shouldn't occur,
				** but since timing doesn't matter when
				** debugging, it doesn't hurt to leave the
				** tests here.
				*/
				if ( ace_cc_yydebug )
				{
					register int ace_cc_yy_i;

					printf( "Error recovery discards " );
					if ( ace_cc_yychar == 0 )
						printf( "token end-of-file\n" );
					else if ( ace_cc_yychar < 0 )
						printf( "token -none-\n" );
					else
					{
						for ( ace_cc_yy_i = 0;
							ace_cc_yytoks[ace_cc_yy_i].t_val >= 0;
							ace_cc_yy_i++ )
						{
							if ( ace_cc_yytoks[ace_cc_yy_i].t_val
								== ace_cc_yychar )
							{
								break;
							}
						}
						printf( "token %s\n",
							ace_cc_yytoks[ace_cc_yy_i].t_name );
					}
				}
#endif /* ACE_CC_YYDEBUG */
				if ( ace_cc_yychar == 0 )	/* reached EOF. quit */
					ACE_CC_YYABORT;
				ace_cc_yychar = -1;
				goto ace_cc_yy_newstate;
			}
		}/* end if ( ace_cc_yy_n == 0 ) */
		/*
		** reduction by production ace_cc_yy_n
		** put stack tops, etc. so things right after switch
		*/
#if ACE_CC_YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( ace_cc_yydebug )
			printf( "Reduce by (%d) \"%s\"\n",
				ace_cc_yy_n, ace_cc_yyreds[ ace_cc_yy_n ] );
#endif
		ace_cc_yytmp = ace_cc_yy_n;			/* value to switch over */
		ace_cc_yypvt = ace_cc_yy_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using ace_cc_yy_state here as temporary
		** register variable, but why not, if it works...
		** If ace_cc_yyr2[ ace_cc_yy_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto ace_cc_yy_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int ace_cc_yy_len = ace_cc_yyr2[ ace_cc_yy_n ];

			if ( !( ace_cc_yy_len & 01 ) )
			{
				ace_cc_yy_len >>= 1;
				ace_cc_yyval = ( ace_cc_yy_pv -= ace_cc_yy_len )[1];	/* $$ = $1 */
				ace_cc_yy_state = ace_cc_yypgo[ ace_cc_yy_n = ace_cc_yyr1[ ace_cc_yy_n ] ] +
					*( ace_cc_yy_ps -= ace_cc_yy_len ) + 1;
				if ( ace_cc_yy_state >= ACE_CC_YYLAST ||
					ace_cc_yychk[ ace_cc_yy_state =
					ace_cc_yyact[ ace_cc_yy_state ] ] != -ace_cc_yy_n )
				{
					ace_cc_yy_state = ace_cc_yyact[ ace_cc_yypgo[ ace_cc_yy_n ] ];
				}
				goto ace_cc_yy_stack;
			}
			ace_cc_yy_len >>= 1;
			ace_cc_yyval = ( ace_cc_yy_pv -= ace_cc_yy_len )[1];	/* $$ = $1 */
			ace_cc_yy_state = ace_cc_yypgo[ ace_cc_yy_n = ace_cc_yyr1[ ace_cc_yy_n ] ] +
				*( ace_cc_yy_ps -= ace_cc_yy_len ) + 1;
			if ( ace_cc_yy_state >= ACE_CC_YYLAST ||
				ace_cc_yychk[ ace_cc_yy_state = ace_cc_yyact[ ace_cc_yy_state ] ] != -ace_cc_yy_n )
			{
				ace_cc_yy_state = ace_cc_yyact[ ace_cc_yypgo[ ace_cc_yy_n ] ];
			}
		}
					/* save until reenter driver code */
		ace_cc_yystate = ace_cc_yy_state;
		ace_cc_yyps = ace_cc_yy_ps;
		ace_cc_yypv = ace_cc_yy_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( ace_cc_yytmp )
	{
		
case 2:{ cmdlist->add(new CC_Sleep_Cmd(0));
			   /* dummy to check exeption in the last command */
		  if(cmdlist->execute()!=0) //CC_FAIL
                    { printf(" ** Test succeded!!\n"); }
                  else
                    { printf(" ** Test FAILED!!\n"); } } break;
case 3:{ cmdlist->add(ace_cc_yypvt[-0].command); } break;
case 4:{ cmdlist->add(ace_cc_yypvt[-0].command); } break;
case 5:{ ace_cc_yyval.command = new CC_Start_Cmd(ace_cc_yypvt[-1].id); } break;
case 6:{ ace_cc_yyval.command = new CC_CreateLockSet_Cmd(ace_cc_yypvt[-1].id); } break;
case 7:{ ace_cc_yyval.command = new CC_CreateLockSet_Cmd(""); } break;
case 8:{ ace_cc_yyval.command = new CC_Lock_Cmd(ace_cc_yypvt[-2].id, ace_cc_yypvt[-1].lock_mode); } break;
case 9:{ ace_cc_yyval.command = new CC_Lock_Cmd("", ace_cc_yypvt[-1].lock_mode); } break;
case 10:{ ace_cc_yyval.command = new CC_UnLock_Cmd(ace_cc_yypvt[-2].id, ace_cc_yypvt[-1].lock_mode); } break;
case 11:{ ace_cc_yyval.command = new CC_UnLock_Cmd("", ace_cc_yypvt[-1].lock_mode); } break;
case 12:{ ace_cc_yyval.command = new CC_TryLock_Cmd(ace_cc_yypvt[-2].id, ace_cc_yypvt[-1].lock_mode); } break;
case 13:{ ace_cc_yyval.command = new CC_TryLock_Cmd("", ace_cc_yypvt[-1].lock_mode); } break;
case 14:{ ace_cc_yyval.command = new CC_ChangeMode_Cmd(ace_cc_yypvt[-3].id, ace_cc_yypvt[-2].lock_mode, ace_cc_yypvt[-1].lock_mode); } break;
case 15:{ ace_cc_yyval.command = new CC_ChangeMode_Cmd("", ace_cc_yypvt[-2].lock_mode, ace_cc_yypvt[-1].lock_mode); } break;
case 16:{ ace_cc_yyval.command = new CC_Sleep_Cmd(ace_cc_yypvt[-1].num); } break;
case 17:{ ace_cc_yyval.command = new CC_Wait_Cmd(""); } break;
case 18:{ ace_cc_yyval.command = new CC_Wait_Cmd(ace_cc_yypvt[-1].id); } break;
case 19:{ ace_cc_yyval.command = new CC_Print_Cmd(ace_cc_yypvt[-1].id); } break;
case 20:{ ace_cc_yyval.command = new CC_Lookup_Cmd(ace_cc_yypvt[-1].id); } break;
case 21:{ cmdlist->setrepeat(ace_cc_yypvt[-1].num);
                          ace_cc_yyval.command = new CC_Dummy_Cmd();} break;
case 22:{ ace_cc_yyval.command = new CC_Excep_Cmd(ace_cc_yypvt[-1].id); } break;
case 23:{ace_cc_yyval.id = ace_cc_yypvt[-0].id; } break;
case 24:{ace_cc_yyval.id = ace_cc_yypvt[-0].id; } break;
case 25:{ace_cc_yyval.id = ace_cc_yypvt[-0].id; } break;
case 26:{ ace_cc_yyval.lock_mode = CosConcurrencyControl::read; } break;
case 27:{ ace_cc_yyval.lock_mode = CosConcurrencyControl::intention_read; } break;
case 28:{ ace_cc_yyval.lock_mode = CosConcurrencyControl::upgrade; } break;
case 29:{ ace_cc_yyval.lock_mode = CosConcurrencyControl::write; } break;
case 30:{ ace_cc_yyval.lock_mode = CosConcurrencyControl::intention_write; } break;
	}
	goto ace_cc_yystack;		/* reset registers in driver code */
}

