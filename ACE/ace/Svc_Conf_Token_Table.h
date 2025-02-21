/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with ACE_YY_ or ace_yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef ACE_YY_ACE_YY_SVC_CONF_TOKEN_TABLE_H_INCLUDED
# define ACE_YY_ACE_YY_SVC_CONF_TOKEN_TABLE_H_INCLUDED
/* Debug traces.  */
#ifndef ACE_YYDEBUG
# define ACE_YYDEBUG 0
#endif
#if ACE_YYDEBUG
extern int ace_yydebug;
#endif

/* Token kinds.  */
#ifndef ACE_YYTOKENTYPE
# define ACE_YYTOKENTYPE
  enum ace_yytokentype
  {
    ACE_YYEMPTY = -2,
    ACE_YYEOF = 0,                     /* "end of file"  */
    ACE_YYerror = 256,                 /* error  */
    ACE_YYUNDEF = 257,                 /* "invalid token"  */
    ACE_DYNAMIC = 258,             /* ACE_DYNAMIC  */
    ACE_STATIC = 259,              /* ACE_STATIC  */
    ACE_SUSPEND = 260,             /* ACE_SUSPEND  */
    ACE_RESUME = 261,              /* ACE_RESUME  */
    ACE_REMOVE = 262,              /* ACE_REMOVE  */
    ACE_USTREAM = 263,             /* ACE_USTREAM  */
    ACE_MODULE_T = 264,            /* ACE_MODULE_T  */
    ACE_STREAM_T = 265,            /* ACE_STREAM_T  */
    ACE_SVC_OBJ_T = 266,           /* ACE_SVC_OBJ_T  */
    ACE_ACTIVE = 267,              /* ACE_ACTIVE  */
    ACE_INACTIVE = 268,            /* ACE_INACTIVE  */
    ACE_PATHNAME = 269,            /* ACE_PATHNAME  */
    ACE_IDENT = 270,               /* ACE_IDENT  */
    ACE_STRING = 271               /* ACE_STRING  */
  };
  typedef enum ace_yytokentype ace_yytoken_kind_t;
#endif

/* Value type.  */




int ace_yyparse (void *ACE_YYLEX_PARAM);


#endif /* !ACE_YY_ACE_YY_SVC_CONF_TOKEN_TABLE_H_INCLUDED  */
