/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_TAO_YY_FE_IDL_TAB_HPP_INCLUDED
# define YY_TAO_YY_FE_IDL_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int tao_yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IDENTIFIER = 258,
    IDL_CONST = 259,
    IDL_MODULE = 260,
    IDL_INTERFACE = 261,
    IDL_TYPEDEF = 262,
    IDL_LONG = 263,
    IDL_SHORT = 264,
    IDL_UNSIGNED = 265,
    IDL_DOUBLE = 266,
    IDL_FLOAT = 267,
    IDL_CHAR = 268,
    IDL_WCHAR = 269,
    IDL_OCTET = 270,
    IDL_BOOLEAN = 271,
    IDL_FIXED = 272,
    IDL_ANY = 273,
    IDL_OBJECT = 274,
    IDL_STRUCT = 275,
    IDL_UNION = 276,
    IDL_SWITCH = 277,
    IDL_ENUM = 278,
    IDL_SEQUENCE = 279,
    IDL_STRING = 280,
    IDL_WSTRING = 281,
    IDL_EXCEPTION = 282,
    IDL_CASE = 283,
    IDL_DEFAULT = 284,
    IDL_READONLY = 285,
    IDL_ATTRIBUTE = 286,
    IDL_ONEWAY = 287,
    IDL_IDEMPOTENT = 288,
    IDL_VOID = 289,
    IDL_IN = 290,
    IDL_OUT = 291,
    IDL_INOUT = 292,
    IDL_RAISES = 293,
    IDL_CONTEXT = 294,
    IDL_NATIVE = 295,
    IDL_LOCAL = 296,
    IDL_ABSTRACT = 297,
    IDL_CUSTOM = 298,
    IDL_FACTORY = 299,
    IDL_PRIVATE = 300,
    IDL_PUBLIC = 301,
    IDL_SUPPORTS = 302,
    IDL_TRUNCATABLE = 303,
    IDL_VALUETYPE = 304,
    IDL_COMPONENT = 305,
    IDL_CONSUMES = 306,
    IDL_EMITS = 307,
    IDL_EVENTTYPE = 308,
    IDL_FINDER = 309,
    IDL_GETRAISES = 310,
    IDL_HOME = 311,
    IDL_IMPORT = 312,
    IDL_MULTIPLE = 313,
    IDL_PRIMARYKEY = 314,
    IDL_PROVIDES = 315,
    IDL_PUBLISHES = 316,
    IDL_SETRAISES = 317,
    IDL_TYPEID = 318,
    IDL_TYPEPREFIX = 319,
    IDL_USES = 320,
    IDL_MANAGES = 321,
    IDL_TYPENAME = 322,
    IDL_PORT = 323,
    IDL_MIRRORPORT = 324,
    IDL_PORTTYPE = 325,
    IDL_CONNECTOR = 326,
    IDL_ALIAS = 327,
    IDL_INTEGER_LITERAL = 328,
    IDL_UINTEGER_LITERAL = 329,
    IDL_STRING_LITERAL = 330,
    IDL_CHARACTER_LITERAL = 331,
    IDL_FLOATING_PT_LITERAL = 332,
    IDL_FIXED_PT_LITERAL = 333,
    IDL_TRUETOK = 334,
    IDL_FALSETOK = 335,
    IDL_SCOPE_DELIMITOR = 336,
    IDL_LEFT_SHIFT = 337,
    IDL_RIGHT_SHIFT = 338,
    IDL_WCHAR_LITERAL = 339,
    IDL_WSTRING_LITERAL = 340
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 152 "fe/idl.ypp" /* yacc.c:1909  */

  AST_Decl                      *dcval;         /* Decl value           */
  UTL_StrList                   *slval;         /* String list          */
  UTL_NameList                  *nlval;         /* Name list            */
  UTL_ExprList                  *elval;         /* Expression list      */
  UTL_LabelList                 *llval;         /* Label list           */
  UTL_DeclList                  *dlval;         /* Declaration list     */
  FE_InterfaceHeader            *ihval;         /* Interface header     */
  FE_OBVHeader                  *vhval;         /* Valuetype header     */
  FE_ComponentHeader            *chval;         /* Component header     */
  FE_HomeHeader                 *hhval;         /* Home header          */
  AST_Expression                *exval;         /* Expression value     */
  AST_UnionLabel                *ulval;         /* Union label          */
  AST_Field                     *ffval;         /* Field value          */
  AST_Field::Visibility         vival;          /* N/A, pub or priv     */
  AST_Expression::ExprType      etval;          /* Expression type      */
  AST_Argument::Direction       dival;          /* Argument direction   */
  AST_Operation::Flags          ofval;          /* Operation flags      */
  FE_Declarator                 *deval;         /* Declarator value     */
  ACE_CDR::Boolean              bval;           /* Boolean value        */
  ACE_CDR::LongLong             ival;           /* Long Long value      */
  ACE_CDR::ULongLong            uival;          /* Unsigned long long   */
  ACE_CDR::Double               dval;           /* Double value         */
  ACE_CDR::Float                fval;           /* Float value          */
  ACE_CDR::Char                 cval;           /* Char value           */
  ACE_CDR::WChar                wcval;          /* WChar value          */
  ACE_CDR::Fixed                fixval;         /* Fixed point value    */
  UTL_String                    *sval;          /* String value         */
  char                          *wsval;         /* WString value        */
  char                          *strval;        /* char * value         */
  Identifier                    *idval;         /* Identifier           */
  UTL_IdList                    *idlist;        /* Identifier list      */
  AST_Decl::NodeType            ntval;          /* Node type value      */
  FE_Utils::T_Param_Info        *pival;         /* Template interface param */
  FE_Utils::T_PARAMLIST_INFO    *plval;         /* List of template params */
  FE_Utils::T_ARGLIST           *alval;         /* List of template args */

#line 178 "fe/idl.tab.hpp" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE tao_yylval;

int tao_yyparse (void);

#endif /* !YY_TAO_YY_FE_IDL_TAB_HPP_INCLUDED  */
