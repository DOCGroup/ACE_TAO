#include <stdio.h>
# define U(x) x
# define NLSTATE yyprevious=YYNEWLINE
# define BEGIN yybgin = yysvec + 1 +
# define INITIAL 0
# define YYLERR yysvec
# define YYSTATE (yyestate-yysvec-1)
# define YYOPTIM 1
# define YYLMAX BUFSIZ
#ifndef __cplusplus
# define output(c) (void)putc(c,yyout)
#else
# define lex_output(c) (void)putc(c,yyout)
#endif

#if defined(__cplusplus) || defined(__STDC__)

#if defined(__cplusplus) && defined(__EXTERN_C__)
extern "C" {
#endif
	int yyback(int *, int);
	int yyinput(void);
	int yylook(void);
	void yyoutput(int);
	int yyracc(int);
	int yyreject(void);
	void yyunput(int);
	int yylex(void);
#ifdef YYLEX_E
	void yywoutput(wchar_t);
	wchar_t yywinput(void);
#endif
#ifndef yyless
	int yyless(int);
#endif
#ifndef yywrap
	int yywrap(void);
#endif
#ifdef LEXDEBUG
	void allprint(char);
	void sprint(char *);
#endif
#if defined(__cplusplus) && defined(__EXTERN_C__)
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
	void exit(int);
#ifdef __cplusplus
}
#endif

#endif
# define unput(c) {yytchar= (c);if(yytchar=='\n')yylineno--;*yysptr++=yytchar;}
# define yymore() (yymorfg=1)
#ifndef __cplusplus
# define input() (((yytchar=yysptr>yysbuf?U(*--yysptr):getc(yyin))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
#else
# define lex_input() (((yytchar=yysptr>yysbuf?U(*--yysptr):getc(yyin))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
#endif
#define ECHO fprintf(yyout, "%s",yytext)
# define REJECT { nstr = yyreject(); goto yyfussy;}
int yyleng;
char yytext[YYLMAX];
int yymorfg;
extern char *yysptr, yysbuf[];
int yytchar;
FILE *yyin = {stdin}, *yyout = {stdout};
extern int yylineno;
struct yysvf { 
	struct yywork *yystoff;
	struct yysvf *yyother;
	int *yystops;};
struct yysvf *yyestate;
extern struct yysvf yysvec[], *yybgin;

# line 4 "idl.ll"
/*

COPYRIGHT

Copyright 1992, 1993, 1994 Sun Microsystems, Inc.  Printed in the United
States of America.  All Rights Reserved.

This product is protected by copyright and distributed under the following
license restricting its use.

The Interface Definition Language Compiler Front End (CFE) is made
available for your use provided that you include this license and copyright
notice on all media and documentation and the software program in which
this product is incorporated in whole or part. You may copy and extend
functionality (but may not remove functionality) of the Interface
Definition Language CFE without charge, but you are not authorized to
license or distribute it to anyone else except as part of a product or
program developed by you or with the express written consent of Sun
Microsystems, Inc. ("Sun").

The names of Sun Microsystems, Inc. and any of its subsidiaries or
affiliates may not be used in advertising or publicity pertaining to
distribution of Interface Definition Language CFE as permitted herein.

This license is effective until terminated by Sun for failure to comply
with this license.  Upon termination, you shall destroy or return all code
and documentation for the Interface Definition Language CFE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED AS IS WITH NO WARRANTIES OF
ANY KIND INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS
FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR ARISING FROM A COURSE OF
DEALING, USAGE OR TRADE PRACTICE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED WITH NO SUPPORT AND WITHOUT
ANY OBLIGATION ON THE PART OF Sun OR ANY OF ITS SUBSIDIARIES OR AFFILIATES
TO ASSIST IN ITS USE, CORRECTION, MODIFICATION OR ENHANCEMENT.

SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL HAVE NO LIABILITY WITH
RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY
INTERFACE DEFINITION LANGUAGE CFE OR ANY PART THEREOF.

IN NO EVENT WILL SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES BE LIABLE FOR
ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
DAMAGES, EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

Use, duplication, or disclosure by the government is subject to
restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
Technical Data and Computer Software clause at DFARS 252.227-7013 and FAR
52.227-19.

Sun, Sun Microsystems and the Sun logo are trademarks or registered
trademarks of Sun Microsystems, Inc.

SunSoft, Inc.  
2550 Garcia Avenue 
Mountain View, California  94043

NOTE:

SunOS, SunSoft, Sun, Solaris, Sun Microsystems or the Sun logo are
trademarks or registered trademarks of Sun Microsystems, Inc.

 */


# line 68 "idl.ll"
/*
 * idl.ll - Lexical scanner for IDL 1.1
 */

#include "idl.h"
#include "idl_extern.h"

#include "fe_private.h"

#include <y.tab.hh>

#include <string.h>

static char	idl_escape_reader(char *);
static double	idl_atof(char *);
static long	idl_atoi(char *, long);
static void	idl_parse_line_and_file(char *);
static void	idl_store_pragma(char *);

// HPUX has yytext typed to unsigned char *. We make sure here that
// we'll always use char *
static char	*__yytext = (char *) yytext;

# define YYNEWLINE 10
yylex(){
int nstr; extern int yyprevious;
#ifdef __cplusplus
/* to avoid CC and lint complaining yyfussy not being used ...*/
static int __lex_hack = 0;
if (__lex_hack) goto yyfussy;
#endif
while((nstr = yylook()) >= 0)
yyfussy: switch(nstr){
case 0:
if(yywrap()) return(0); break;
case 1:

# line 94 "idl.ll"
	return MODULE;
break;
case 2:

# line 95 "idl.ll"
	return RAISES;
break;
case 3:

# line 96 "idl.ll"
return READONLY;
break;
case 4:

# line 97 "idl.ll"
return ATTRIBUTE;
break;
case 5:

# line 98 "idl.ll"
return EXCEPTION;
break;
case 6:

# line 99 "idl.ll"
	return CONTEXT;
break;
case 7:

# line 100 "idl.ll"
return INTERFACE;
break;
case 8:

# line 101 "idl.ll"
	return CONST;
break;
case 9:

# line 102 "idl.ll"
	return TYPEDEF;
break;
case 10:

# line 103 "idl.ll"
	return STRUCT;
break;
case 11:

# line 104 "idl.ll"
	return ENUM;
break;
case 12:

# line 105 "idl.ll"
	return STRING;
break;
case 13:

# line 106 "idl.ll"
return WSTRING;
break;
case 14:

# line 107 "idl.ll"
return SEQUENCE;
break;
case 15:

# line 108 "idl.ll"
	return UNION;
break;
case 16:

# line 109 "idl.ll"
	return SWITCH;
break;
case 17:

# line 110 "idl.ll"
	return CASE;
break;
case 18:

# line 111 "idl.ll"
	return DEFAULT;
break;
case 19:

# line 112 "idl.ll"
	return FLOAT;
break;
case 20:

# line 113 "idl.ll"
	return DOUBLE;
break;
case 21:

# line 114 "idl.ll"
	return LONG;
break;
case 22:

# line 115 "idl.ll"
	return SHORT;
break;
case 23:

# line 116 "idl.ll"
return UNSIGNED;
break;
case 24:

# line 117 "idl.ll"
	return CHAR;
break;
case 25:

# line 118 "idl.ll"
	return WCHAR;
break;
case 26:

# line 119 "idl.ll"
	return BOOLEAN;
break;
case 27:

# line 120 "idl.ll"
	return OCTET;
break;
case 28:

# line 121 "idl.ll"
	return VOID;
break;
case 29:

# line 123 "idl.ll"
	return TRUETOK;
break;
case 30:

# line 124 "idl.ll"
	return FALSETOK;
break;
case 31:

# line 126 "idl.ll"
	return INOUT;
break;
case 32:

# line 127 "idl.ll"
	return IN;
break;
case 33:

# line 128 "idl.ll"
	return OUT;
break;
case 34:

# line 129 "idl.ll"
	return ONEWAY;
break;
case 35:

# line 131 "idl.ll"
	return LEFT_SHIFT;
break;
case 36:

# line 132 "idl.ll"
	return RIGHT_SHIFT;
break;
case 37:

# line 133 "idl.ll"
	{
		  yylval.strval = "::";    
		  return SCOPE_DELIMITOR;
		}
break;
case 38:

# line 138 "idl.ll"
{
    char *z = (char *) malloc(strlen(__yytext) + 1);
    strcpy(z, __yytext);
    yylval.strval = z;
    return IDENTIFIER;
}
break;
case 39:

# line 145 "idl.ll"
     {
                  yylval.dval = idl_atof(__yytext);
                  return FLOATING_PT_LITERAL;
                }
break;
case 40:

# line 149 "idl.ll"
 {
                  yylval.dval = idl_atof(__yytext);
                  return FLOATING_PT_LITERAL;
                }
break;
case 41:

# line 154 "idl.ll"
{
		  yylval.ival = idl_atoi(__yytext, 10);
		  return INTEGER_LITERAL;
	        }
break;
case 42:

# line 158 "idl.ll"
{
		  yylval.ival = idl_atoi(__yytext, 16);
		  return INTEGER_LITERAL;
	        }
break;
case 43:

# line 162 "idl.ll"
{
		  yylval.ival = idl_atoi(__yytext, 8);
		  return INTEGER_LITERAL;
	      	}
break;
case 44:

# line 167 "idl.ll"
{
		  __yytext[strlen(__yytext)-1] = '\0';
		  yylval.sval = new String(__yytext + 1);
		  return STRING_LITERAL;
	      	}
break;
case 45:

# line 172 "idl.ll"
	{
		  yylval.cval = __yytext[1];
		  return CHARACTER_LITERAL;
	      	}
break;
case 46:

# line 176 "idl.ll"
{
		  // octal character constant
		  yylval.cval = idl_escape_reader(__yytext + 1);
		  return CHARACTER_LITERAL;
		}
break;
case 47:

# line 181 "idl.ll"
{
		  yylval.cval = idl_escape_reader(__yytext + 1);
		  return CHARACTER_LITERAL;
		}
break;
case 48:

# line 185 "idl.ll"
{/* remember pragma */
  		  idl_global->set_lineno(idl_global->lineno() + 1);
		  idl_store_pragma(__yytext);
		}
break;
case 49:

# line 189 "idl.ll"
	{
		  idl_parse_line_and_file(__yytext);
		}
break;
case 50:

# line 192 "idl.ll"
		{
		  idl_parse_line_and_file(__yytext);
		}
break;
case 51:

# line 195 "idl.ll"
{
		  idl_parse_line_and_file(__yytext);
	        }
break;
case 52:

# line 198 "idl.ll"
{
		  /* ignore cpp ident */
  		  idl_global->set_lineno(idl_global->lineno() + 1);
		}
break;
case 53:

# line 202 "idl.ll"
{
		  /* ignore comments */
  		  idl_global->set_lineno(idl_global->lineno() + 1);
		}
break;
case 54:

# line 206 "idl.ll"
	{
		  for(;;) {
		    char c = yyinput();
		    if (c == '*') {
		      char next = yyinput();
		      if (next == '/')
			break;
		      else
			yyunput(c);
	              if (c == '\n') 
		        idl_global->set_lineno(idl_global->lineno() + 1);
		    }
	          }
	        }
break;
case 55:

# line 220 "idl.ll"
	;
break;
case 56:

# line 221 "idl.ll"
	{
  		  idl_global->set_lineno(idl_global->lineno() + 1);
		}
break;
case 57:

# line 224 "idl.ll"
	return __yytext[0];
break;
case -1:
break;
default:
(void)fprintf(yyout,"bad switch yylook %d",nstr);
} return(0); }
/* end of yylex */
	/* subroutines */

/*
 * Strip down a name to the last component, i.e. everything after the last
 * '/' character
 */
static char *
stripped_name(UTL_String *fn)
{
    char	*n = fn->get_string();
    long	l;

    if (n == NULL)
	return NULL;
    l = strlen(n);
    for (n += l; l > 0 && *n != '/'; l--, n--);
    if (*n == '/') n++;
    return n;
}

/*
 * Parse a #line statement generated by the C preprocessor
 */
static void
idl_parse_line_and_file(char *buf)
{
  char		*r = buf;
  char 		*h;
  UTL_String	*nm;

  /* Skip initial '#' */
  if (*r != '#') {
    return;
  }

  /* Find line number */
  for (r++; *r == ' ' || *r == '\t'; r++);
  h = r;
  for (; *r != '\0' && *r != ' ' && *r != '\t'; r++);
  *r++ = 0;
  idl_global->set_lineno(idl_atoi(h, 10));

  /* Find file name, if present */
  for (; *r != '"'; r++) {
    if (*r == '\n' || *r == '\0')
      return;
  }
  h = ++r;
  for (; *r != '"'; r++);
  *r = 0;
  if (*h == '\0')
    idl_global->set_filename(new String("standard input"));
  else
    idl_global->set_filename(new String(h));

  idl_global->set_in_main_file(
    (idl_global->filename()->compare(idl_global->real_filename())) ?
    I_TRUE :
    I_FALSE
  );
  /*
   * If it's an import file store the stripped name for the BE to use
   */
  if (!(idl_global->in_main_file()) && idl_global->import()) {
    nm = new UTL_String(stripped_name(idl_global->filename()));
    idl_global->store_include_file_name(nm);
  }
}

/*
 * Store a #pragma line into the list of pragmas
 */
static void
idl_store_pragma(char *buf)
{
  char *cp = buf + 1;
  while(*cp != 'p')
    cp++;
  while(*cp != ' ' && *cp != '\t')
    cp++;
  while(*cp == ' ' || *cp == '\t')
    cp++;
  char pragma[80];
  char *pp = pragma;
  while(*cp != '\n') {
    *pp++ = *cp++;
  }
  *pp = 0;
  if (strcmp(pragma, "import") == 0) {
    idl_global->set_import(I_TRUE);
    return;
  }
  if (strcmp(pragma, "include") == 0) {
    idl_global->set_import(I_FALSE);
    return;
  }
  UTL_StrList *p = idl_global->pragmas();
  if (p == NULL)
    idl_global->set_pragmas(new UTL_StrList(new String(buf), NULL));
  else {
    p->nconc(new UTL_StrList(new String(buf), NULL));
    idl_global->set_pragmas(p);
  }
}

/*
 * idl_atoi - Convert a string of digits into an integer according to base b
 */
static long
idl_atoi(char *s, long b)
{
	long	r = 0;
	long	negative = 0;

	if (*s == '-') {
	  negative = 1;
	  s++;
	}
	if (b == 8 && *s == '0')
	  s++;
	else if (b == 16 && *s == '0' && (*(s + 1) == 'x' || *(s + 1) == 'X'))
	  s += 2;

	for (; *s; s++)
	  if (*s <= '9' && *s >= '0')
	    r = (r * b) + (*s - '0');
	  else if (b > 10 && *s <= 'f' && *s >= 'a')
	    r = (r * b) + (*s - 'a' + 10);
	  else if (b > 10 && *s <= 'F' && *s >= 'A')
	    r = (r * b) + (*s - 'A' + 10);
	  else
	    break;

	if (negative)
	  r *= -1;

	return r;
}

/*
 * Convert a string to a float; atof doesn't seem to work, always.
 */
static double
idl_atof(char *s)
{
	char    *h = s;
	double	d = 0.0;
	double	f = 0.0;
	double	e, k;
	long	neg = 0, negexp = 0;

	if (*s == '-') {
	  neg = 1;
	  s++;
	}
	while (*s >= '0' && *s <= '9') {
		d = (d * 10) + *s - '0';
		s++;
	}
	if (*s == '.') {
		s++;
		e = 10;
		while (*s >= '0' && *s <= '9') {
			d += (*s - '0') / (e * 1.0);
			e *= 10;
			s++;
		}
	}
	if (*s == 'e' || *s == 'E') {
		s++;
		if (*s == '-') {
			negexp = 1;
			s++;
		} else if (*s == '+')
			s++;
		e = 0;
		while (*s >= '0' && *s <= '9') {
			e = (e * 10) + *s - '0';
			s++;
		}
		if (e > 0) {
			for (k = 1; e > 0; k *= 10, e--);
			if (negexp)
				d /= k;
			else
				d *= k;
		}
	}

	if (neg) d *= -1.0;

	return d;
}

/*
 * Convert (some) escaped characters into their ascii values
 */
static char
idl_escape_reader(
    char *str
)
{
    if (str[0] != '\\') {
	return str[0];
    }

    switch (str[1]) {
      case 'n':
	return '\n';
      case 't':
	return '\t';
      case 'v':
	return '\v';
      case 'b':
	return '\b';
      case 'r':
	return '\r';
      case 'f':
	return '\f';
      case 'a':
	return '\a';
      case '\\':
	return '\\';
      case '\?':
	return '?';
      case '\'':
	return '\'';
      case '"':
	return '"';
      case 'x':
	{
	    // hex value
	    for (int i = 2; str[i] != '\0' && isxdigit(str[i]); i++) {
		continue;
	    }
	    char save = str[i];
	    str[i] = '\0';
	    char out = (char)idl_atoi(&str[2], 16);
	    str[i] = save;
	    return out;
	}
	break;
      default:
	// check for octal value
	if (str[1] >= '0' && str[1] <= '7') {
	    for (int i = 1; str[i] >= '0' && str[i] <= '7'; i++) {
		continue;
	    }
	    char save = str[i];
	    str[i] = '\0';
	    char out = (char)idl_atoi(&str[1], 8);
	    str[i] = save;
	    return out;
	} else {
	  return str[1] - 'a';
	}
	break;
    }
}
int yyvstop[] = {
0,

55,
0,

55,
0,

57,
0,

55,
57,
0,

56,
0,

57,
0,

57,
0,

57,
0,

57,
0,

43,
57,
0,

41,
57,
0,

57,
0,

57,
0,

57,
0,

38,
57,
0,

38,
57,
0,

38,
57,
0,

38,
57,
0,

38,
57,
0,

38,
57,
0,

38,
57,
0,

38,
57,
0,

38,
57,
0,

38,
57,
0,

38,
57,
0,

38,
57,
0,

38,
57,
0,

38,
57,
0,

38,
57,
0,

38,
57,
0,

38,
57,
0,

38,
57,
0,

38,
57,
0,

57,
0,

55,
0,

44,
0,

43,
0,

41,
0,

54,
0,

39,
0,

43,
0,

37,
0,

35,
0,

36,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

32,
38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

51,
0,

45,
0,

45,
0,

53,
0,

39,
0,

40,
0,

42,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

33,
38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

47,
0,

46,
47,
0,

39,
0,

40,
0,

38,
0,

29,
38,
0,

38,
0,

38,
0,

17,
38,
0,

24,
38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

11,
38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

21,
38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

28,
38,
0,

38,
0,

38,
0,

46,
0,

30,
38,
0,

38,
0,

38,
0,

8,
38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

19,
38,
0,

31,
38,
0,

38,
0,

38,
0,

27,
38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

22,
38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

15,
38,
0,

38,
0,

38,
0,

38,
0,

50,
0,

38,
0,

38,
0,

38,
0,

38,
0,

20,
38,
0,

38,
0,

38,
0,

1,
38,
0,

34,
38,
0,

2,
38,
0,

38,
0,

38,
0,

12,
38,
0,

10,
38,
0,

16,
38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

49,
0,

38,
0,

26,
38,
0,

6,
38,
0,

18,
38,
0,

38,
0,

38,
0,

38,
0,

38,
0,

9,
38,
0,

38,
0,

25,
38,
0,

38,
0,

52,
0,

38,
0,

38,
0,

38,
0,

3,
38,
0,

14,
38,
0,

23,
38,
0,

38,
0,

4,
38,
0,

5,
38,
0,

7,
38,
0,

13,
38,
0,

48,
0,
0};
# define YYTYPE unsigned char
struct yywork { YYTYPE verify, advance; } yycrank[] = {
0,0,	0,0,	1,3,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,4,	1,5,	
0,0,	4,35,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	1,6,	
4,35,	0,0,	0,0,	0,0,	
1,7,	0,0,	0,0,	0,0,	
1,3,	0,0,	1,8,	84,129,	
1,9,	1,10,	1,11,	38,88,	
89,133,	0,0,	0,0,	9,42,	
0,0,	1,11,	0,0,	1,12,	
9,43,	1,13,	0,0,	1,14,	
0,0,	0,0,	1,15,	11,44,	
0,0,	11,41,	1,15,	1,16,	
1,15,	12,49,	13,50,	14,51,	
130,129,	1,15,	16,53,	0,0,	
0,0,	99,140,	0,0,	0,0,	
0,0,	1,17,	0,0,	0,0,	
53,98,	1,15,	11,47,	0,0,	
0,0,	0,0,	17,54,	98,139,	
1,3,	54,99,	1,18,	1,19,	
1,20,	1,21,	1,22,	1,23,	
0,0,	6,36,	1,24,	0,0,	
0,0,	1,25,	1,26,	58,103,	
1,27,	6,36,	6,36,	1,28,	
1,29,	1,30,	1,31,	1,32,	
1,33,	2,34,	11,47,	19,56,	
23,64,	2,7,	21,60,	20,57,	
18,55,	22,62,	24,65,	2,8,	
25,66,	2,9,	20,58,	26,67,	
21,61,	30,77,	6,37,	22,63,	
27,68,	20,59,	31,78,	32,79,	
2,12,	28,71,	2,13,	6,36,	
2,14,	28,72,	33,80,	27,69,	
6,36,	6,36,	41,44,	55,100,	
41,41,	56,101,	27,70,	57,102,	
6,36,	29,73,	59,104,	60,105,	
29,74,	61,106,	33,81,	62,107,	
63,108,	6,36,	2,17,	64,109,	
66,112,	6,36,	6,36,	6,36,	
29,75,	41,47,	67,113,	29,76,	
6,36,	7,38,	68,114,	2,18,	
2,19,	2,20,	2,21,	2,22,	
2,23,	7,38,	7,0,	2,24,	
6,36,	65,110,	2,25,	2,26,	
69,115,	2,27,	65,111,	6,36,	
2,28,	2,29,	2,30,	2,31,	
2,32,	2,33,	70,116,	71,117,	
72,118,	41,47,	73,119,	74,120,	
75,121,	76,122,	7,38,	46,46,	
46,46,	46,46,	46,46,	46,46,	
46,46,	46,46,	46,46,	7,38,	
77,123,	78,124,	79,126,	80,127,	
7,38,	7,38,	81,128,	86,131,	
87,132,	100,141,	101,142,	78,125,	
7,38,	102,143,	103,144,	104,145,	
104,146,	105,147,	106,148,	107,149,	
108,150,	7,38,	109,151,	110,152,	
111,153,	7,38,	7,38,	7,38,	
112,154,	113,155,	114,156,	115,157,	
7,38,	8,40,	8,41,	8,41,	
8,41,	8,41,	8,41,	8,41,	
8,41,	8,41,	8,41,	117,158,	
7,38,	118,159,	119,160,	120,161,	
7,39,	122,164,	10,44,	7,38,	
10,45,	10,45,	10,45,	10,45,	
10,45,	10,45,	10,45,	10,45,	
10,46,	10,46,	123,165,	124,166,	
125,167,	126,168,	127,169,	128,170,	
131,172,	47,95,	132,173,	47,95,	
139,176,	10,47,	47,96,	47,96,	
47,96,	47,96,	47,96,	47,96,	
47,96,	47,96,	47,96,	47,96,	
91,134,	141,177,	142,178,	145,179,	
146,180,	121,162,	147,181,	148,182,	
10,48,	91,135,	91,135,	91,135,	
91,135,	91,135,	91,135,	91,135,	
91,135,	121,163,	150,183,	151,184,	
152,185,	10,47,	153,186,	155,187,	
15,52,	15,52,	15,52,	15,52,	
15,52,	15,52,	15,52,	15,52,	
15,52,	15,52,	156,188,	157,189,	
158,190,	159,191,	160,192,	161,193,	
10,48,	15,52,	15,52,	15,52,	
15,52,	15,52,	15,52,	15,52,	
15,52,	15,52,	15,52,	15,52,	
15,52,	15,52,	15,52,	15,52,	
15,52,	15,52,	15,52,	15,52,	
15,52,	15,52,	15,52,	15,52,	
15,52,	15,52,	15,52,	162,194,	
163,195,	164,196,	165,197,	15,52,	
166,198,	15,52,	15,52,	15,52,	
15,52,	15,52,	15,52,	15,52,	
15,52,	15,52,	15,52,	15,52,	
15,52,	15,52,	15,52,	15,52,	
15,52,	15,52,	15,52,	15,52,	
15,52,	15,52,	15,52,	15,52,	
15,52,	15,52,	15,52,	34,82,	
34,83,	93,136,	167,199,	93,136,	
169,200,	170,201,	93,137,	93,137,	
93,137,	93,137,	93,137,	93,137,	
93,137,	93,137,	93,137,	93,137,	
39,89,	172,204,	173,205,	43,43,	
175,174,	177,206,	34,84,	178,207,	
39,89,	39,0,	171,202,	43,43,	
43,92,	180,208,	96,138,	181,209,	
182,210,	183,211,	186,212,	187,213,	
96,138,	189,214,	34,85,	34,85,	
34,85,	34,85,	34,85,	34,85,	
34,85,	34,85,	34,85,	34,85,	
171,203,	39,89,	190,215,	191,216,	
43,43,	192,217,	39,90,	194,218,	
195,219,	196,220,	39,89,	197,221,	
199,222,	43,43,	96,138,	39,91,	
39,91,	200,223,	43,43,	43,43,	
96,138,	137,94,	201,224,	39,89,	
204,226,	205,227,	43,43,	137,94,	
206,228,	207,229,	208,230,	209,231,	
39,89,	211,232,	212,233,	43,43,	
39,89,	39,89,	39,89,	43,43,	
43,43,	43,43,	216,234,	39,89,	
217,235,	221,236,	43,43,	34,86,	
222,237,	223,238,	224,239,	227,241,	
228,242,	137,94,	34,87,	39,89,	
232,243,	233,244,	43,43,	137,94,	
234,245,	235,246,	39,89,	237,247,	
239,248,	43,43,	44,44,	44,44,	
44,44,	44,44,	44,44,	44,44,	
44,44,	44,44,	44,44,	44,44,	
95,96,	95,96,	95,96,	95,96,	
95,96,	95,96,	95,96,	95,96,	
95,96,	95,96,	242,250,	44,93,	
44,94,	243,251,	244,252,	248,253,	
241,249,	45,44,	44,94,	45,45,	
45,45,	45,45,	45,45,	45,45,	
45,45,	45,45,	45,45,	45,46,	
45,46,	136,137,	136,137,	136,137,	
136,137,	136,137,	136,137,	136,137,	
136,137,	136,137,	136,137,	241,249,	
45,47,	0,0,	0,0,	44,93,	
44,94,	85,83,	0,0,	0,0,	
0,0,	0,0,	44,94,	0,0,	
0,0,	0,0,	0,0,	203,225,	
48,97,	48,97,	48,97,	48,97,	
48,97,	48,97,	48,97,	48,97,	
48,97,	48,97,	0,0,	85,130,	
0,0,	0,0,	0,0,	129,129,	
45,47,	48,97,	48,97,	48,97,	
48,97,	48,97,	48,97,	129,129,	
129,129,	0,0,	0,0,	85,85,	
85,85,	85,85,	85,85,	85,85,	
85,85,	85,85,	85,85,	85,85,	
85,85,	203,203,	203,203,	203,203,	
203,203,	203,203,	203,203,	203,203,	
203,203,	203,203,	203,203,	0,0,	
129,171,	48,97,	48,97,	48,97,	
48,97,	48,97,	48,97,	0,0,	
0,0,	129,129,	0,0,	0,0,	
0,0,	0,0,	129,129,	129,129,	
0,0,	0,0,	226,226,	0,0,	
0,0,	0,0,	129,129,	0,0,	
0,0,	0,0,	226,226,	226,240,	
0,0,	0,0,	0,0,	129,129,	
0,0,	0,0,	135,174,	129,129,	
129,129,	129,129,	0,0,	0,0,	
0,0,	0,0,	129,129,	135,175,	
135,175,	135,175,	135,175,	135,175,	
135,175,	135,175,	135,175,	226,226,	
0,0,	0,0,	129,129,	0,0,	
0,0,	0,0,	0,0,	249,249,	
226,226,	129,129,	0,0,	0,0,	
0,0,	226,226,	226,226,	249,249,	
249,254,	0,0,	0,0,	0,0,	
0,0,	226,226,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	226,226,	0,0,	
0,0,	0,0,	226,226,	226,226,	
226,226,	0,0,	0,0,	0,0,	
249,249,	226,226,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	249,249,	0,0,	0,0,	
0,0,	226,226,	249,249,	249,249,	
0,0,	0,0,	0,0,	0,0,	
226,226,	0,0,	249,249,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	249,249,	
0,0,	0,0,	0,0,	249,249,	
249,249,	249,249,	0,0,	0,0,	
0,0,	0,0,	249,249,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	249,249,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	249,249,	0,0,	0,0,	
0,0};
struct yysvf yysvec[] = {
0,	0,	0,
yycrank+-1,	0,		yyvstop+1,
yycrank+-86,	yysvec+1,	yyvstop+3,
yycrank+0,	0,		yyvstop+5,
yycrank+4,	0,		yyvstop+7,
yycrank+0,	0,		yyvstop+10,
yycrank+-104,	0,		yyvstop+12,
yycrank+-180,	0,		yyvstop+14,
yycrank+209,	0,		yyvstop+16,
yycrank+13,	0,		yyvstop+18,
yycrank+228,	0,		yyvstop+20,
yycrank+21,	yysvec+8,	yyvstop+23,
yycrank+15,	0,		yyvstop+26,
yycrank+14,	0,		yyvstop+28,
yycrank+13,	0,		yyvstop+30,
yycrank+284,	0,		yyvstop+32,
yycrank+13,	yysvec+15,	yyvstop+35,
yycrank+12,	yysvec+15,	yyvstop+38,
yycrank+12,	yysvec+15,	yyvstop+41,
yycrank+12,	yysvec+15,	yyvstop+44,
yycrank+30,	yysvec+15,	yyvstop+47,
yycrank+25,	yysvec+15,	yyvstop+50,
yycrank+19,	yysvec+15,	yyvstop+53,
yycrank+16,	yysvec+15,	yyvstop+56,
yycrank+20,	yysvec+15,	yyvstop+59,
yycrank+21,	yysvec+15,	yyvstop+62,
yycrank+24,	yysvec+15,	yyvstop+65,
yycrank+41,	yysvec+15,	yyvstop+68,
yycrank+48,	yysvec+15,	yyvstop+71,
yycrank+60,	yysvec+15,	yyvstop+74,
yycrank+16,	yysvec+15,	yyvstop+77,
yycrank+32,	yysvec+15,	yyvstop+80,
yycrank+32,	yysvec+15,	yyvstop+83,
yycrank+51,	yysvec+15,	yyvstop+86,
yycrank+398,	0,		yyvstop+89,
yycrank+0,	yysvec+4,	yyvstop+91,
yycrank+0,	yysvec+6,	0,	
yycrank+0,	0,		yyvstop+93,
yycrank+12,	0,		0,	
yycrank+-423,	0,		0,	
yycrank+0,	yysvec+10,	yyvstop+95,
yycrank+108,	yysvec+8,	yyvstop+97,
yycrank+0,	0,		yyvstop+99,
yycrank+-426,	0,		0,	
yycrank+474,	0,		yyvstop+101,
yycrank+503,	0,		yyvstop+103,
yycrank+167,	yysvec+45,	0,	
yycrank+250,	0,		0,	
yycrank+540,	0,		0,	
yycrank+0,	0,		yyvstop+105,
yycrank+0,	0,		yyvstop+107,
yycrank+0,	0,		yyvstop+109,
yycrank+0,	yysvec+15,	yyvstop+111,
yycrank+12,	yysvec+15,	yyvstop+113,
yycrank+12,	yysvec+15,	yyvstop+115,
yycrank+39,	yysvec+15,	yyvstop+117,
yycrank+46,	yysvec+15,	yyvstop+119,
yycrank+44,	yysvec+15,	yyvstop+121,
yycrank+14,	yysvec+15,	yyvstop+123,
yycrank+52,	yysvec+15,	yyvstop+125,
yycrank+61,	yysvec+15,	yyvstop+127,
yycrank+48,	yysvec+15,	yyvstop+129,
yycrank+50,	yysvec+15,	yyvstop+131,
yycrank+69,	yysvec+15,	yyvstop+133,
yycrank+60,	yysvec+15,	yyvstop+135,
yycrank+82,	yysvec+15,	yyvstop+137,
yycrank+62,	yysvec+15,	yyvstop+140,
yycrank+78,	yysvec+15,	yyvstop+142,
yycrank+66,	yysvec+15,	yyvstop+144,
yycrank+95,	yysvec+15,	yyvstop+146,
yycrank+90,	yysvec+15,	yyvstop+148,
yycrank+102,	yysvec+15,	yyvstop+150,
yycrank+111,	yysvec+15,	yyvstop+152,
yycrank+97,	yysvec+15,	yyvstop+154,
yycrank+100,	yysvec+15,	yyvstop+156,
yycrank+98,	yysvec+15,	yyvstop+158,
yycrank+108,	yysvec+15,	yyvstop+160,
yycrank+112,	yysvec+15,	yyvstop+162,
yycrank+120,	yysvec+15,	yyvstop+164,
yycrank+121,	yysvec+15,	yyvstop+166,
yycrank+123,	yysvec+15,	yyvstop+168,
yycrank+114,	yysvec+15,	yyvstop+170,
yycrank+0,	yysvec+34,	0,	
yycrank+0,	0,		yyvstop+172,
yycrank+13,	yysvec+34,	0,	
yycrank+567,	0,		0,	
yycrank+131,	0,		0,	
yycrank+118,	0,		0,	
yycrank+0,	0,		yyvstop+174,
yycrank+13,	0,		0,	
yycrank+0,	yysvec+89,	yyvstop+176,
yycrank+269,	0,		0,	
yycrank+0,	0,		yyvstop+178,
yycrank+366,	0,		0,	
yycrank+0,	0,		yyvstop+180,
yycrank+484,	0,		0,	
yycrank+368,	yysvec+95,	yyvstop+182,
yycrank+0,	yysvec+48,	yyvstop+184,
yycrank+12,	yysvec+15,	yyvstop+186,
yycrank+12,	yysvec+15,	yyvstop+188,
yycrank+119,	yysvec+15,	yyvstop+190,
yycrank+126,	yysvec+15,	yyvstop+192,
yycrank+136,	yysvec+15,	yyvstop+194,
yycrank+124,	yysvec+15,	yyvstop+196,
yycrank+124,	yysvec+15,	yyvstop+198,
yycrank+144,	yysvec+15,	yyvstop+200,
yycrank+144,	yysvec+15,	yyvstop+202,
yycrank+134,	yysvec+15,	yyvstop+204,
yycrank+143,	yysvec+15,	yyvstop+206,
yycrank+149,	yysvec+15,	yyvstop+208,
yycrank+130,	yysvec+15,	yyvstop+210,
yycrank+147,	yysvec+15,	yyvstop+212,
yycrank+149,	yysvec+15,	yyvstop+214,
yycrank+136,	yysvec+15,	yyvstop+216,
yycrank+153,	yysvec+15,	yyvstop+218,
yycrank+136,	yysvec+15,	yyvstop+220,
yycrank+0,	yysvec+15,	yyvstop+222,
yycrank+152,	yysvec+15,	yyvstop+225,
yycrank+169,	yysvec+15,	yyvstop+227,
yycrank+153,	yysvec+15,	yyvstop+229,
yycrank+157,	yysvec+15,	yyvstop+231,
yycrank+208,	yysvec+15,	yyvstop+233,
yycrank+157,	yysvec+15,	yyvstop+235,
yycrank+185,	yysvec+15,	yyvstop+237,
yycrank+176,	yysvec+15,	yyvstop+239,
yycrank+183,	yysvec+15,	yyvstop+241,
yycrank+189,	yysvec+15,	yyvstop+243,
yycrank+193,	yysvec+15,	yyvstop+245,
yycrank+177,	yysvec+15,	yyvstop+247,
yycrank+-602,	0,		0,	
yycrank+42,	0,		0,	
yycrank+191,	0,		0,	
yycrank+197,	0,		0,	
yycrank+0,	0,		yyvstop+249,
yycrank+0,	0,		yyvstop+251,
yycrank+631,	0,		0,	
yycrank+513,	0,		0,	
yycrank+407,	yysvec+136,	yyvstop+254,
yycrank+0,	0,		yyvstop+256,
yycrank+227,	yysvec+15,	yyvstop+258,
yycrank+0,	yysvec+15,	yyvstop+260,
yycrank+204,	yysvec+15,	yyvstop+263,
yycrank+209,	yysvec+15,	yyvstop+265,
yycrank+0,	yysvec+15,	yyvstop+267,
yycrank+0,	yysvec+15,	yyvstop+270,
yycrank+195,	yysvec+15,	yyvstop+273,
yycrank+211,	yysvec+15,	yyvstop+275,
yycrank+197,	yysvec+15,	yyvstop+277,
yycrank+207,	yysvec+15,	yyvstop+279,
yycrank+0,	yysvec+15,	yyvstop+281,
yycrank+214,	yysvec+15,	yyvstop+284,
yycrank+211,	yysvec+15,	yyvstop+286,
yycrank+212,	yysvec+15,	yyvstop+288,
yycrank+216,	yysvec+15,	yyvstop+290,
yycrank+0,	yysvec+15,	yyvstop+292,
yycrank+223,	yysvec+15,	yyvstop+295,
yycrank+226,	yysvec+15,	yyvstop+297,
yycrank+246,	yysvec+15,	yyvstop+299,
yycrank+243,	yysvec+15,	yyvstop+301,
yycrank+234,	yysvec+15,	yyvstop+303,
yycrank+245,	yysvec+15,	yyvstop+305,
yycrank+231,	yysvec+15,	yyvstop+307,
yycrank+265,	yysvec+15,	yyvstop+309,
yycrank+277,	yysvec+15,	yyvstop+311,
yycrank+278,	yysvec+15,	yyvstop+313,
yycrank+278,	yysvec+15,	yyvstop+315,
yycrank+270,	yysvec+15,	yyvstop+317,
yycrank+307,	yysvec+15,	yyvstop+319,
yycrank+0,	yysvec+15,	yyvstop+321,
yycrank+298,	yysvec+15,	yyvstop+324,
yycrank+308,	yysvec+15,	yyvstop+326,
yycrank+424,	0,		0,	
yycrank+315,	0,		0,	
yycrank+323,	0,		0,	
yycrank+0,	0,		yyvstop+328,
yycrank+389,	0,		0,	
yycrank+0,	yysvec+15,	yyvstop+330,
yycrank+331,	yysvec+15,	yyvstop+333,
yycrank+334,	yysvec+15,	yyvstop+335,
yycrank+0,	yysvec+15,	yyvstop+337,
yycrank+317,	yysvec+15,	yyvstop+340,
yycrank+331,	yysvec+15,	yyvstop+342,
yycrank+339,	yysvec+15,	yyvstop+344,
yycrank+325,	yysvec+15,	yyvstop+346,
yycrank+0,	yysvec+15,	yyvstop+348,
yycrank+0,	yysvec+15,	yyvstop+351,
yycrank+340,	yysvec+15,	yyvstop+354,
yycrank+342,	yysvec+15,	yyvstop+356,
yycrank+0,	yysvec+15,	yyvstop+358,
yycrank+324,	yysvec+15,	yyvstop+361,
yycrank+343,	yysvec+15,	yyvstop+363,
yycrank+349,	yysvec+15,	yyvstop+365,
yycrank+351,	yysvec+15,	yyvstop+367,
yycrank+0,	yysvec+15,	yyvstop+369,
yycrank+360,	yysvec+15,	yyvstop+372,
yycrank+348,	yysvec+15,	yyvstop+374,
yycrank+361,	yysvec+15,	yyvstop+376,
yycrank+366,	yysvec+15,	yyvstop+378,
yycrank+0,	yysvec+15,	yyvstop+380,
yycrank+358,	yysvec+15,	yyvstop+383,
yycrank+378,	yysvec+15,	yyvstop+385,
yycrank+368,	yysvec+15,	yyvstop+387,
yycrank+0,	0,		yyvstop+389,
yycrank+577,	0,		0,	
yycrank+364,	0,		0,	
yycrank+372,	0,		0,	
yycrank+367,	yysvec+15,	yyvstop+391,
yycrank+375,	yysvec+15,	yyvstop+393,
yycrank+370,	yysvec+15,	yyvstop+395,
yycrank+371,	yysvec+15,	yyvstop+397,
yycrank+0,	yysvec+15,	yyvstop+399,
yycrank+384,	yysvec+15,	yyvstop+402,
yycrank+393,	yysvec+15,	yyvstop+404,
yycrank+0,	yysvec+15,	yyvstop+406,
yycrank+0,	yysvec+15,	yyvstop+409,
yycrank+0,	yysvec+15,	yyvstop+412,
yycrank+390,	yysvec+15,	yyvstop+415,
yycrank+401,	yysvec+15,	yyvstop+417,
yycrank+0,	yysvec+15,	yyvstop+419,
yycrank+0,	yysvec+15,	yyvstop+422,
yycrank+0,	yysvec+15,	yyvstop+425,
yycrank+399,	yysvec+15,	yyvstop+428,
yycrank+403,	yysvec+15,	yyvstop+430,
yycrank+389,	yysvec+15,	yyvstop+432,
yycrank+403,	yysvec+15,	yyvstop+434,
yycrank+0,	0,		yyvstop+436,
yycrank+-653,	0,		0,	
yycrank+410,	0,		0,	
yycrank+392,	yysvec+15,	yyvstop+438,
yycrank+0,	yysvec+15,	yyvstop+440,
yycrank+0,	yysvec+15,	yyvstop+443,
yycrank+0,	yysvec+15,	yyvstop+446,
yycrank+401,	yysvec+15,	yyvstop+449,
yycrank+414,	yysvec+15,	yyvstop+451,
yycrank+395,	yysvec+15,	yyvstop+453,
yycrank+416,	yysvec+15,	yyvstop+455,
yycrank+0,	yysvec+15,	yyvstop+457,
yycrank+419,	yysvec+15,	yyvstop+460,
yycrank+0,	yysvec+15,	yyvstop+462,
yycrank+425,	yysvec+15,	yyvstop+465,
yycrank+0,	0,		yyvstop+467,
yycrank+539,	0,		0,	
yycrank+441,	yysvec+15,	yyvstop+469,
yycrank+435,	yysvec+15,	yyvstop+471,
yycrank+445,	yysvec+15,	yyvstop+473,
yycrank+0,	yysvec+15,	yyvstop+475,
yycrank+0,	yysvec+15,	yyvstop+478,
yycrank+0,	yysvec+15,	yyvstop+481,
yycrank+431,	yysvec+15,	yyvstop+484,
yycrank+-694,	0,		0,	
yycrank+0,	yysvec+15,	yyvstop+486,
yycrank+0,	yysvec+15,	yyvstop+489,
yycrank+0,	yysvec+15,	yyvstop+492,
yycrank+0,	yysvec+15,	yyvstop+495,
yycrank+0,	0,		yyvstop+498,
0,	0,	0};
struct yywork *yytop = yycrank+789;
struct yysvf *yybgin = yysvec+1;
char yymatch[] = {
  0,   1,   1,   1,   1,   1,   1,   1, 
  1,   9,  10,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  9,   1,  34,   1,   1,   1,   1,   1, 
  1,   1,   1,  43,   1,  43,   1,   1, 
 48,  49,  49,  49,  49,  49,  49,  49, 
 56,  56,   1,   1,   1,   1,   1,   1, 
  1,  65,  65,  65,  65,  69,  70,  71, 
 71,  71,  71,  71,  76,  71,  71,  71, 
 71,  71,  71,  71,  71,  71,  71,  71, 
 88,  71,  71,   1,   1,   1,   1,  95, 
  1,  65,  65,  65,  65,  69,  70,  71, 
 71,  71,  71,  71,  76,  71,  71,  71, 
 71,  71,  71,  71,  71,  71,  71,  71, 
 88,  71,  71,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
0};
char yyextra[] = {
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0};
/*	Copyright (c) 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#pragma ident	"@(#)ncform	6.8	95/02/11 SMI"

int yylineno =1;
# define YYU(x) x
# define NLSTATE yyprevious=YYNEWLINE
struct yysvf *yylstate [YYLMAX], **yylsp, **yyolsp;
char yysbuf[YYLMAX];
char *yysptr = yysbuf;
int *yyfnd;
extern struct yysvf *yyestate;
int yyprevious = YYNEWLINE;
#if defined(__cplusplus) || defined(__STDC__)
int yylook(void)
#else
yylook()
#endif
{
	register struct yysvf *yystate, **lsp;
	register struct yywork *yyt;
	struct yysvf *yyz;
	int yych, yyfirst;
	struct yywork *yyr;
# ifdef LEXDEBUG
	int debug;
# endif
	char *yylastch;
	/* start off machines */
# ifdef LEXDEBUG
	debug = 0;
# endif
	yyfirst=1;
	if (!yymorfg)
		yylastch = yytext;
	else {
		yymorfg=0;
		yylastch = yytext+yyleng;
		}
	for(;;){
		lsp = yylstate;
		yyestate = yystate = yybgin;
		if (yyprevious==YYNEWLINE) yystate++;
		for (;;){
# ifdef LEXDEBUG
			if(debug)fprintf(yyout,"state %d\n",yystate-yysvec-1);
# endif
			yyt = yystate->yystoff;
			if(yyt == yycrank && !yyfirst){  /* may not be any transitions */
				yyz = yystate->yyother;
				if(yyz == 0)break;
				if(yyz->yystoff == yycrank)break;
				}
#ifndef __cplusplus
			*yylastch++ = yych = input();
#else
			*yylastch++ = yych = lex_input();
#endif
			if(yylastch > &yytext[YYLMAX]) {
				fprintf(yyout,"Input string too long, limit %d\n",YYLMAX);
				exit(1);
			}
			yyfirst=0;
		tryagain:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"char ");
				allprint(yych);
				putchar('\n');
				}
# endif
			yyr = yyt;
			if ( (int)yyt > (int)yycrank){
				yyt = yyr + yych;
				if (yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					if(lsp > &yylstate[YYLMAX]) {
						fprintf(yyout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				}
# ifdef YYOPTIM
			else if((int)yyt < (int)yycrank) {		/* r < yycrank */
				yyt = yyr = yycrank+(yycrank-yyt);
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"compressed state\n");
# endif
				yyt = yyt + yych;
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					if(lsp > &yylstate[YYLMAX]) {
						fprintf(yyout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				yyt = yyr + YYU(yymatch[yych]);
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"try fall back character ");
					allprint(YYU(yymatch[yych]));
					putchar('\n');
					}
# endif
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transition */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					if(lsp > &yylstate[YYLMAX]) {
						fprintf(yyout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				}
			if ((yystate = yystate->yyother) && (yyt= yystate->yystoff) != yycrank){
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"fall back to state %d\n",yystate-yysvec-1);
# endif
				goto tryagain;
				}
# endif
			else
				{unput(*--yylastch);break;}
		contin:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"state %d char ",yystate-yysvec-1);
				allprint(yych);
				putchar('\n');
				}
# endif
			;
			}
# ifdef LEXDEBUG
		if(debug){
			fprintf(yyout,"stopped at %d with ",*(lsp-1)-yysvec-1);
			allprint(yych);
			putchar('\n');
			}
# endif
		while (lsp-- > yylstate){
			*yylastch-- = 0;
			if (*lsp != 0 && (yyfnd= (*lsp)->yystops) && *yyfnd > 0){
				yyolsp = lsp;
				if(yyextra[*yyfnd]){		/* must backup */
					while(yyback((*lsp)->yystops,-*yyfnd) != 1 && lsp > yylstate){
						lsp--;
						unput(*yylastch--);
						}
					}
				yyprevious = YYU(*yylastch);
				yylsp = lsp;
				yyleng = yylastch-yytext+1;
				yytext[yyleng] = 0;
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"\nmatch ");
					sprint(yytext);
					fprintf(yyout," action %d\n",*yyfnd);
					}
# endif
				return(*yyfnd++);
				}
			unput(*yylastch);
			}
		if (yytext[0] == 0  /* && feof(yyin) */)
			{
			yysptr=yysbuf;
			return(0);
			}
#ifndef __cplusplus
		yyprevious = yytext[0] = input();
		if (yyprevious>0)
			output(yyprevious);
#else
		yyprevious = yytext[0] = lex_input();
		if (yyprevious>0)
			lex_output(yyprevious);
#endif
		yylastch=yytext;
# ifdef LEXDEBUG
		if(debug)putchar('\n');
# endif
		}
	}
#if defined(__cplusplus) || defined(__STDC__)
int yyback(int *p, int m)
#else
yyback(p, m)
	int *p;
#endif
{
	if (p==0) return(0);
	while (*p) {
		if (*p++ == m)
			return(1);
	}
	return(0);
}
	/* the following are only used in the lex library */
#if defined(__cplusplus) || defined(__STDC__)
int yyinput(void)
#else
yyinput()
#endif
{
#ifndef __cplusplus
	return(input());
#else
	return(lex_input());
#endif
	}
#if defined(__cplusplus) || defined(__STDC__)
void yyoutput(int c)
#else
yyoutput(c)
  int c; 
#endif
{
#ifndef __cplusplus
	output(c);
#else
	lex_output(c);
#endif
	}
#if defined(__cplusplus) || defined(__STDC__)
void yyunput(int c)
#else
yyunput(c)
   int c; 
#endif
{
	unput(c);
	}
