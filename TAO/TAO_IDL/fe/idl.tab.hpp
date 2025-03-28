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
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_TAO_YY_FE_IDL_TAB_HPP_INCLUDED
# define YY_TAO_YY_FE_IDL_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int tao_yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IDENTIFIER = 258,              /* IDENTIFIER  */
    IDL_CONST = 259,               /* IDL_CONST  */
    IDL_MODULE = 260,              /* IDL_MODULE  */
    IDL_INTERFACE = 261,           /* IDL_INTERFACE  */
    IDL_TYPEDEF = 262,             /* IDL_TYPEDEF  */
    IDL_LONG = 263,                /* IDL_LONG  */
    IDL_SHORT = 264,               /* IDL_SHORT  */
    IDL_UNSIGNED = 265,            /* IDL_UNSIGNED  */
    IDL_DOUBLE = 266,              /* IDL_DOUBLE  */
    IDL_FLOAT = 267,               /* IDL_FLOAT  */
    IDL_CHAR = 268,                /* IDL_CHAR  */
    IDL_WCHAR = 269,               /* IDL_WCHAR  */
    IDL_OCTET = 270,               /* IDL_OCTET  */
    IDL_BOOLEAN = 271,             /* IDL_BOOLEAN  */
    IDL_FIXED = 272,               /* IDL_FIXED  */
    IDL_ANY = 273,                 /* IDL_ANY  */
    IDL_OBJECT = 274,              /* IDL_OBJECT  */
    IDL_STRUCT = 275,              /* IDL_STRUCT  */
    IDL_UNION = 276,               /* IDL_UNION  */
    IDL_SWITCH = 277,              /* IDL_SWITCH  */
    IDL_ENUM = 278,                /* IDL_ENUM  */
    IDL_SEQUENCE = 279,            /* IDL_SEQUENCE  */
    IDL_MAP = 280,                 /* IDL_MAP  */
    IDL_STRING = 281,              /* IDL_STRING  */
    IDL_WSTRING = 282,             /* IDL_WSTRING  */
    IDL_EXCEPTION = 283,           /* IDL_EXCEPTION  */
    IDL_CASE = 284,                /* IDL_CASE  */
    IDL_DEFAULT = 285,             /* IDL_DEFAULT  */
    IDL_READONLY = 286,            /* IDL_READONLY  */
    IDL_ATTRIBUTE = 287,           /* IDL_ATTRIBUTE  */
    IDL_ONEWAY = 288,              /* IDL_ONEWAY  */
    IDL_IDEMPOTENT = 289,          /* IDL_IDEMPOTENT  */
    IDL_VOID = 290,                /* IDL_VOID  */
    IDL_IN = 291,                  /* IDL_IN  */
    IDL_OUT = 292,                 /* IDL_OUT  */
    IDL_INOUT = 293,               /* IDL_INOUT  */
    IDL_RAISES = 294,              /* IDL_RAISES  */
    IDL_CONTEXT = 295,             /* IDL_CONTEXT  */
    IDL_NATIVE = 296,              /* IDL_NATIVE  */
    IDL_LOCAL = 297,               /* IDL_LOCAL  */
    IDL_ABSTRACT = 298,            /* IDL_ABSTRACT  */
    IDL_CUSTOM = 299,              /* IDL_CUSTOM  */
    IDL_FACTORY = 300,             /* IDL_FACTORY  */
    IDL_PRIVATE = 301,             /* IDL_PRIVATE  */
    IDL_PUBLIC = 302,              /* IDL_PUBLIC  */
    IDL_SUPPORTS = 303,            /* IDL_SUPPORTS  */
    IDL_TRUNCATABLE = 304,         /* IDL_TRUNCATABLE  */
    IDL_VALUETYPE = 305,           /* IDL_VALUETYPE  */
    IDL_COMPONENT = 306,           /* IDL_COMPONENT  */
    IDL_CONSUMES = 307,            /* IDL_CONSUMES  */
    IDL_EMITS = 308,               /* IDL_EMITS  */
    IDL_EVENTTYPE = 309,           /* IDL_EVENTTYPE  */
    IDL_FINDER = 310,              /* IDL_FINDER  */
    IDL_GETRAISES = 311,           /* IDL_GETRAISES  */
    IDL_HOME = 312,                /* IDL_HOME  */
    IDL_IMPORT = 313,              /* IDL_IMPORT  */
    IDL_MULTIPLE = 314,            /* IDL_MULTIPLE  */
    IDL_PRIMARYKEY = 315,          /* IDL_PRIMARYKEY  */
    IDL_PROVIDES = 316,            /* IDL_PROVIDES  */
    IDL_PUBLISHES = 317,           /* IDL_PUBLISHES  */
    IDL_SETRAISES = 318,           /* IDL_SETRAISES  */
    IDL_TYPEID = 319,              /* IDL_TYPEID  */
    IDL_TYPEPREFIX = 320,          /* IDL_TYPEPREFIX  */
    IDL_USES = 321,                /* IDL_USES  */
    IDL_MANAGES = 322,             /* IDL_MANAGES  */
    IDL_TYPENAME = 323,            /* IDL_TYPENAME  */
    IDL_PORT = 324,                /* IDL_PORT  */
    IDL_MIRRORPORT = 325,          /* IDL_MIRRORPORT  */
    IDL_PORTTYPE = 326,            /* IDL_PORTTYPE  */
    IDL_CONNECTOR = 327,           /* IDL_CONNECTOR  */
    IDL_ALIAS = 328,               /* IDL_ALIAS  */
    IDL_INTEGER_LITERAL = 329,     /* IDL_INTEGER_LITERAL  */
    IDL_UINTEGER_LITERAL = 330,    /* IDL_UINTEGER_LITERAL  */
    IDL_STRING_LITERAL = 331,      /* IDL_STRING_LITERAL  */
    IDL_CHARACTER_LITERAL = 332,   /* IDL_CHARACTER_LITERAL  */
    IDL_FLOATING_PT_LITERAL = 333, /* IDL_FLOATING_PT_LITERAL  */
    IDL_FIXED_PT_LITERAL = 334,    /* IDL_FIXED_PT_LITERAL  */
    IDL_TRUETOK = 335,             /* IDL_TRUETOK  */
    IDL_FALSETOK = 336,            /* IDL_FALSETOK  */
    IDL_INT8 = 337,                /* IDL_INT8  */
    IDL_UINT8 = 338,               /* IDL_UINT8  */
    IDL_INT16 = 339,               /* IDL_INT16  */
    IDL_UINT16 = 340,              /* IDL_UINT16  */
    IDL_INT32 = 341,               /* IDL_INT32  */
    IDL_UINT32 = 342,              /* IDL_UINT32  */
    IDL_INT64 = 343,               /* IDL_INT64  */
    IDL_UINT64 = 344,              /* IDL_UINT64  */
    IDL_SCOPE_DELIMITOR = 345,     /* IDL_SCOPE_DELIMITOR  */
    IDL_LEFT_SHIFT = 346,          /* IDL_LEFT_SHIFT  */
    IDL_RIGHT_SHIFT = 347,         /* IDL_RIGHT_SHIFT  */
    IDL_WCHAR_LITERAL = 348,       /* IDL_WCHAR_LITERAL  */
    IDL_WSTRING_LITERAL = 349,     /* IDL_WSTRING_LITERAL  */
    IDL_ANNOTATION_DECL = 350,     /* IDL_ANNOTATION_DECL  */
    IDL_ANNOTATION_SYMBOL = 351    /* IDL_ANNOTATION_SYMBOL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 171 "fe/idl.ypp"

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
  AST_Annotation_Appl *annotation_val;
  AST_Annotation_Appls *annotations_val;
  AST_Annotation_Appl::Param *annotation_param_val;
  AST_Annotation_Appl::Params *annotation_params_val;
  AST_Annotation_Decl *annotation_decl_val;
  AST_Decls *decls_val;
  Decl_Annotations_Pair *decl_annotations_pair_val;
  Decl_Annotations_Pair_Pair* decl_annotations_pair_val_pair;

#line 206 "fe/idl.tab.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE tao_yylval;


int tao_yyparse (void);


#endif /* !YY_TAO_YY_FE_IDL_TAB_HPP_INCLUDED  */
