/* A Bison parser, made by GNU Bison 3.6.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.6.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         tao_yyparse
#define yylex           tao_yylex
#define yyerror         tao_yyerror
#define yydebug         tao_yydebug
#define yynerrs         tao_yynerrs
#define yylval          tao_yylval
#define yychar          tao_yychar

/* First part of user prologue.  */
#line 72 "fe/idl.ypp"

#include <utl_identifier.h>
#include <utl_err.h>
#include <utl_string.h>
#include <utl_strlist.h>
#include <utl_namelist.h>
#include <utl_exprlist.h>
#include <utl_labellist.h>
#include <utl_decllist.h>

#include <global_extern.h>
#include <nr_extern.h>

#include <ast_argument.h>
#include <ast_array.h>
#include <ast_attribute.h>
#include <ast_field.h>
#include <ast_fixed.h>
#include <ast_expression.h>
#include <ast_operation.h>
#include <ast_generator.h>
#include <ast_template_module.h>
#include <ast_template_module_inst.h>
#include <ast_template_module_ref.h>
#include <ast_typedef.h>
#include <ast_valuebox.h>
#include <ast_valuetype.h>
#include <ast_valuetype_fwd.h>
#include <ast_eventtype.h>
#include <ast_eventtype_fwd.h>
#include <ast_component.h>
#include <ast_component_fwd.h>
#include <ast_home.h>
#include <ast_porttype.h>
#include <ast_connector.h>
#include <ast_uses.h>
#include <ast_constant.h>
#include <ast_union.h>
#include <ast_union_fwd.h>
#include <ast_structure_fwd.h>
#include <ast_extern.h>
#include <ast_enum.h>
#include <ast_root.h>
#include <ast_sequence.h>
#include <ast_string.h>
#include <ast_factory.h>
#include <ast_finder.h>
#include <ast_exception.h>
#include <ast_param_holder.h>
#include <ast_visitor_tmpl_module_inst.h>
#include <ast_visitor_tmpl_module_ref.h>
#include <ast_visitor_context.h>
#include <ast_annotation_appl.h>
#include <ast_union_branch.h>
#include <ast_enum_val.h>
#include <ast_annotation_member.h>
#include <ast_provides.h>
#include <ast_emits.h>
#include <ast_publishes.h>
#include <ast_consumes.h>
#include <ast_extended_port.h>
#include <ast_mirror_port.h>

#include <fe_declarator.h>
#include <fe_interface_header.h>
#include <fe_obv_header.h>
#include <fe_component_header.h>
#include <fe_home_header.h>
#include <fe_utils.h>

void tao_yyerror (const char *);
int tao_yylex (void);
extern "C" int tao_yywrap (void);

extern char tao_yytext[];
extern int tao_yyleng;

AST_Enum *tao_enum_constant_decl = 0;
AST_String *tao_string_decl = 0;
AST_Expression::ExprType t_param_const_type = AST_Expression::EV_none;
bool stack_based_lookup_for_primary_expr = false;

#define TAO_YYDEBUG_LEXER_TEXT (tao_yytext[tao_yyleng] = '\0', tao_yytext)
// Compile Optional Tracing Output for Parser, can be enabled with --bison-trace
#define YYDEBUG 1

#line 165 "fe/idl.tab.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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
    IDL_STRING = 280,              /* IDL_STRING  */
    IDL_WSTRING = 281,             /* IDL_WSTRING  */
    IDL_EXCEPTION = 282,           /* IDL_EXCEPTION  */
    IDL_CASE = 283,                /* IDL_CASE  */
    IDL_DEFAULT = 284,             /* IDL_DEFAULT  */
    IDL_READONLY = 285,            /* IDL_READONLY  */
    IDL_ATTRIBUTE = 286,           /* IDL_ATTRIBUTE  */
    IDL_ONEWAY = 287,              /* IDL_ONEWAY  */
    IDL_IDEMPOTENT = 288,          /* IDL_IDEMPOTENT  */
    IDL_VOID = 289,                /* IDL_VOID  */
    IDL_IN = 290,                  /* IDL_IN  */
    IDL_OUT = 291,                 /* IDL_OUT  */
    IDL_INOUT = 292,               /* IDL_INOUT  */
    IDL_RAISES = 293,              /* IDL_RAISES  */
    IDL_CONTEXT = 294,             /* IDL_CONTEXT  */
    IDL_NATIVE = 295,              /* IDL_NATIVE  */
    IDL_LOCAL = 296,               /* IDL_LOCAL  */
    IDL_ABSTRACT = 297,            /* IDL_ABSTRACT  */
    IDL_CUSTOM = 298,              /* IDL_CUSTOM  */
    IDL_FACTORY = 299,             /* IDL_FACTORY  */
    IDL_PRIVATE = 300,             /* IDL_PRIVATE  */
    IDL_PUBLIC = 301,              /* IDL_PUBLIC  */
    IDL_SUPPORTS = 302,            /* IDL_SUPPORTS  */
    IDL_TRUNCATABLE = 303,         /* IDL_TRUNCATABLE  */
    IDL_VALUETYPE = 304,           /* IDL_VALUETYPE  */
    IDL_COMPONENT = 305,           /* IDL_COMPONENT  */
    IDL_CONSUMES = 306,            /* IDL_CONSUMES  */
    IDL_EMITS = 307,               /* IDL_EMITS  */
    IDL_EVENTTYPE = 308,           /* IDL_EVENTTYPE  */
    IDL_FINDER = 309,              /* IDL_FINDER  */
    IDL_GETRAISES = 310,           /* IDL_GETRAISES  */
    IDL_HOME = 311,                /* IDL_HOME  */
    IDL_IMPORT = 312,              /* IDL_IMPORT  */
    IDL_MULTIPLE = 313,            /* IDL_MULTIPLE  */
    IDL_PRIMARYKEY = 314,          /* IDL_PRIMARYKEY  */
    IDL_PROVIDES = 315,            /* IDL_PROVIDES  */
    IDL_PUBLISHES = 316,           /* IDL_PUBLISHES  */
    IDL_SETRAISES = 317,           /* IDL_SETRAISES  */
    IDL_TYPEID = 318,              /* IDL_TYPEID  */
    IDL_TYPEPREFIX = 319,          /* IDL_TYPEPREFIX  */
    IDL_USES = 320,                /* IDL_USES  */
    IDL_MANAGES = 321,             /* IDL_MANAGES  */
    IDL_TYPENAME = 322,            /* IDL_TYPENAME  */
    IDL_PORT = 323,                /* IDL_PORT  */
    IDL_MIRRORPORT = 324,          /* IDL_MIRRORPORT  */
    IDL_PORTTYPE = 325,            /* IDL_PORTTYPE  */
    IDL_CONNECTOR = 326,           /* IDL_CONNECTOR  */
    IDL_ALIAS = 327,               /* IDL_ALIAS  */
    IDL_INTEGER_LITERAL = 328,     /* IDL_INTEGER_LITERAL  */
    IDL_UINTEGER_LITERAL = 329,    /* IDL_UINTEGER_LITERAL  */
    IDL_STRING_LITERAL = 330,      /* IDL_STRING_LITERAL  */
    IDL_CHARACTER_LITERAL = 331,   /* IDL_CHARACTER_LITERAL  */
    IDL_FLOATING_PT_LITERAL = 332, /* IDL_FLOATING_PT_LITERAL  */
    IDL_FIXED_PT_LITERAL = 333,    /* IDL_FIXED_PT_LITERAL  */
    IDL_TRUETOK = 334,             /* IDL_TRUETOK  */
    IDL_FALSETOK = 335,            /* IDL_FALSETOK  */
    IDL_SCOPE_DELIMITOR = 336,     /* IDL_SCOPE_DELIMITOR  */
    IDL_LEFT_SHIFT = 337,          /* IDL_LEFT_SHIFT  */
    IDL_RIGHT_SHIFT = 338,         /* IDL_RIGHT_SHIFT  */
    IDL_WCHAR_LITERAL = 339,       /* IDL_WCHAR_LITERAL  */
    IDL_WSTRING_LITERAL = 340,     /* IDL_WSTRING_LITERAL  */
    IDL_ANNOTATION_DECL = 341,     /* IDL_ANNOTATION_DECL  */
    IDL_ANNOTATION_SYMBOL = 342    /* IDL_ANNOTATION_SYMBOL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 163 "fe/idl.ypp"

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

#line 347 "fe/idl.tab.cpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE tao_yylval;

int tao_yyparse (void);

#endif /* !YY_TAO_YY_FE_IDL_TAB_HPP_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENTIFIER = 3,                 /* IDENTIFIER  */
  YYSYMBOL_IDL_CONST = 4,                  /* IDL_CONST  */
  YYSYMBOL_IDL_MODULE = 5,                 /* IDL_MODULE  */
  YYSYMBOL_IDL_INTERFACE = 6,              /* IDL_INTERFACE  */
  YYSYMBOL_IDL_TYPEDEF = 7,                /* IDL_TYPEDEF  */
  YYSYMBOL_IDL_LONG = 8,                   /* IDL_LONG  */
  YYSYMBOL_IDL_SHORT = 9,                  /* IDL_SHORT  */
  YYSYMBOL_IDL_UNSIGNED = 10,              /* IDL_UNSIGNED  */
  YYSYMBOL_IDL_DOUBLE = 11,                /* IDL_DOUBLE  */
  YYSYMBOL_IDL_FLOAT = 12,                 /* IDL_FLOAT  */
  YYSYMBOL_IDL_CHAR = 13,                  /* IDL_CHAR  */
  YYSYMBOL_IDL_WCHAR = 14,                 /* IDL_WCHAR  */
  YYSYMBOL_IDL_OCTET = 15,                 /* IDL_OCTET  */
  YYSYMBOL_IDL_BOOLEAN = 16,               /* IDL_BOOLEAN  */
  YYSYMBOL_IDL_FIXED = 17,                 /* IDL_FIXED  */
  YYSYMBOL_IDL_ANY = 18,                   /* IDL_ANY  */
  YYSYMBOL_IDL_OBJECT = 19,                /* IDL_OBJECT  */
  YYSYMBOL_IDL_STRUCT = 20,                /* IDL_STRUCT  */
  YYSYMBOL_IDL_UNION = 21,                 /* IDL_UNION  */
  YYSYMBOL_IDL_SWITCH = 22,                /* IDL_SWITCH  */
  YYSYMBOL_IDL_ENUM = 23,                  /* IDL_ENUM  */
  YYSYMBOL_IDL_SEQUENCE = 24,              /* IDL_SEQUENCE  */
  YYSYMBOL_IDL_STRING = 25,                /* IDL_STRING  */
  YYSYMBOL_IDL_WSTRING = 26,               /* IDL_WSTRING  */
  YYSYMBOL_IDL_EXCEPTION = 27,             /* IDL_EXCEPTION  */
  YYSYMBOL_IDL_CASE = 28,                  /* IDL_CASE  */
  YYSYMBOL_IDL_DEFAULT = 29,               /* IDL_DEFAULT  */
  YYSYMBOL_IDL_READONLY = 30,              /* IDL_READONLY  */
  YYSYMBOL_IDL_ATTRIBUTE = 31,             /* IDL_ATTRIBUTE  */
  YYSYMBOL_IDL_ONEWAY = 32,                /* IDL_ONEWAY  */
  YYSYMBOL_IDL_IDEMPOTENT = 33,            /* IDL_IDEMPOTENT  */
  YYSYMBOL_IDL_VOID = 34,                  /* IDL_VOID  */
  YYSYMBOL_IDL_IN = 35,                    /* IDL_IN  */
  YYSYMBOL_IDL_OUT = 36,                   /* IDL_OUT  */
  YYSYMBOL_IDL_INOUT = 37,                 /* IDL_INOUT  */
  YYSYMBOL_IDL_RAISES = 38,                /* IDL_RAISES  */
  YYSYMBOL_IDL_CONTEXT = 39,               /* IDL_CONTEXT  */
  YYSYMBOL_IDL_NATIVE = 40,                /* IDL_NATIVE  */
  YYSYMBOL_IDL_LOCAL = 41,                 /* IDL_LOCAL  */
  YYSYMBOL_IDL_ABSTRACT = 42,              /* IDL_ABSTRACT  */
  YYSYMBOL_IDL_CUSTOM = 43,                /* IDL_CUSTOM  */
  YYSYMBOL_IDL_FACTORY = 44,               /* IDL_FACTORY  */
  YYSYMBOL_IDL_PRIVATE = 45,               /* IDL_PRIVATE  */
  YYSYMBOL_IDL_PUBLIC = 46,                /* IDL_PUBLIC  */
  YYSYMBOL_IDL_SUPPORTS = 47,              /* IDL_SUPPORTS  */
  YYSYMBOL_IDL_TRUNCATABLE = 48,           /* IDL_TRUNCATABLE  */
  YYSYMBOL_IDL_VALUETYPE = 49,             /* IDL_VALUETYPE  */
  YYSYMBOL_IDL_COMPONENT = 50,             /* IDL_COMPONENT  */
  YYSYMBOL_IDL_CONSUMES = 51,              /* IDL_CONSUMES  */
  YYSYMBOL_IDL_EMITS = 52,                 /* IDL_EMITS  */
  YYSYMBOL_IDL_EVENTTYPE = 53,             /* IDL_EVENTTYPE  */
  YYSYMBOL_IDL_FINDER = 54,                /* IDL_FINDER  */
  YYSYMBOL_IDL_GETRAISES = 55,             /* IDL_GETRAISES  */
  YYSYMBOL_IDL_HOME = 56,                  /* IDL_HOME  */
  YYSYMBOL_IDL_IMPORT = 57,                /* IDL_IMPORT  */
  YYSYMBOL_IDL_MULTIPLE = 58,              /* IDL_MULTIPLE  */
  YYSYMBOL_IDL_PRIMARYKEY = 59,            /* IDL_PRIMARYKEY  */
  YYSYMBOL_IDL_PROVIDES = 60,              /* IDL_PROVIDES  */
  YYSYMBOL_IDL_PUBLISHES = 61,             /* IDL_PUBLISHES  */
  YYSYMBOL_IDL_SETRAISES = 62,             /* IDL_SETRAISES  */
  YYSYMBOL_IDL_TYPEID = 63,                /* IDL_TYPEID  */
  YYSYMBOL_IDL_TYPEPREFIX = 64,            /* IDL_TYPEPREFIX  */
  YYSYMBOL_IDL_USES = 65,                  /* IDL_USES  */
  YYSYMBOL_IDL_MANAGES = 66,               /* IDL_MANAGES  */
  YYSYMBOL_IDL_TYPENAME = 67,              /* IDL_TYPENAME  */
  YYSYMBOL_IDL_PORT = 68,                  /* IDL_PORT  */
  YYSYMBOL_IDL_MIRRORPORT = 69,            /* IDL_MIRRORPORT  */
  YYSYMBOL_IDL_PORTTYPE = 70,              /* IDL_PORTTYPE  */
  YYSYMBOL_IDL_CONNECTOR = 71,             /* IDL_CONNECTOR  */
  YYSYMBOL_IDL_ALIAS = 72,                 /* IDL_ALIAS  */
  YYSYMBOL_IDL_INTEGER_LITERAL = 73,       /* IDL_INTEGER_LITERAL  */
  YYSYMBOL_IDL_UINTEGER_LITERAL = 74,      /* IDL_UINTEGER_LITERAL  */
  YYSYMBOL_IDL_STRING_LITERAL = 75,        /* IDL_STRING_LITERAL  */
  YYSYMBOL_IDL_CHARACTER_LITERAL = 76,     /* IDL_CHARACTER_LITERAL  */
  YYSYMBOL_IDL_FLOATING_PT_LITERAL = 77,   /* IDL_FLOATING_PT_LITERAL  */
  YYSYMBOL_IDL_FIXED_PT_LITERAL = 78,      /* IDL_FIXED_PT_LITERAL  */
  YYSYMBOL_IDL_TRUETOK = 79,               /* IDL_TRUETOK  */
  YYSYMBOL_IDL_FALSETOK = 80,              /* IDL_FALSETOK  */
  YYSYMBOL_IDL_SCOPE_DELIMITOR = 81,       /* IDL_SCOPE_DELIMITOR  */
  YYSYMBOL_IDL_LEFT_SHIFT = 82,            /* IDL_LEFT_SHIFT  */
  YYSYMBOL_IDL_RIGHT_SHIFT = 83,           /* IDL_RIGHT_SHIFT  */
  YYSYMBOL_IDL_WCHAR_LITERAL = 84,         /* IDL_WCHAR_LITERAL  */
  YYSYMBOL_IDL_WSTRING_LITERAL = 85,       /* IDL_WSTRING_LITERAL  */
  YYSYMBOL_IDL_ANNOTATION_DECL = 86,       /* IDL_ANNOTATION_DECL  */
  YYSYMBOL_IDL_ANNOTATION_SYMBOL = 87,     /* IDL_ANNOTATION_SYMBOL  */
  YYSYMBOL_88_ = 88,                       /* ';'  */
  YYSYMBOL_89_ = 89,                       /* '{'  */
  YYSYMBOL_90_ = 90,                       /* '}'  */
  YYSYMBOL_91_ = 91,                       /* '<'  */
  YYSYMBOL_92_ = 92,                       /* '>'  */
  YYSYMBOL_93_ = 93,                       /* ':'  */
  YYSYMBOL_94_ = 94,                       /* ','  */
  YYSYMBOL_95_ = 95,                       /* '='  */
  YYSYMBOL_96_ = 96,                       /* '|'  */
  YYSYMBOL_97_ = 97,                       /* '^'  */
  YYSYMBOL_98_ = 98,                       /* '&'  */
  YYSYMBOL_99_ = 99,                       /* '+'  */
  YYSYMBOL_100_ = 100,                     /* '-'  */
  YYSYMBOL_101_ = 101,                     /* '*'  */
  YYSYMBOL_102_ = 102,                     /* '/'  */
  YYSYMBOL_103_ = 103,                     /* '%'  */
  YYSYMBOL_104_ = 104,                     /* '~'  */
  YYSYMBOL_105_ = 105,                     /* '('  */
  YYSYMBOL_106_ = 106,                     /* ')'  */
  YYSYMBOL_107_ = 107,                     /* '['  */
  YYSYMBOL_108_ = 108,                     /* ']'  */
  YYSYMBOL_YYACCEPT = 109,                 /* $accept  */
  YYSYMBOL_start = 110,                    /* start  */
  YYSYMBOL_definitions = 111,              /* definitions  */
  YYSYMBOL_at_least_one_definition = 112,  /* at_least_one_definition  */
  YYSYMBOL_definition = 113,               /* definition  */
  YYSYMBOL_fixed_definition = 114,         /* fixed_definition  */
  YYSYMBOL_115_1 = 115,                    /* $@1  */
  YYSYMBOL_116_2 = 116,                    /* $@2  */
  YYSYMBOL_117_3 = 117,                    /* $@3  */
  YYSYMBOL_118_4 = 118,                    /* $@4  */
  YYSYMBOL_119_5 = 119,                    /* $@5  */
  YYSYMBOL_120_6 = 120,                    /* $@6  */
  YYSYMBOL_121_7 = 121,                    /* $@7  */
  YYSYMBOL_122_8 = 122,                    /* $@8  */
  YYSYMBOL_123_9 = 123,                    /* $@9  */
  YYSYMBOL_124_10 = 124,                   /* $@10  */
  YYSYMBOL_125_11 = 125,                   /* $@11  */
  YYSYMBOL_126_12 = 126,                   /* $@12  */
  YYSYMBOL_127_13 = 127,                   /* $@13  */
  YYSYMBOL_128_14 = 128,                   /* $@14  */
  YYSYMBOL_129_15 = 129,                   /* $@15  */
  YYSYMBOL_module_header = 130,            /* module_header  */
  YYSYMBOL_131_16 = 131,                   /* $@16  */
  YYSYMBOL_module = 132,                   /* module  */
  YYSYMBOL_133_17 = 133,                   /* @17  */
  YYSYMBOL_134_18 = 134,                   /* $@18  */
  YYSYMBOL_135_19 = 135,                   /* $@19  */
  YYSYMBOL_template_module_header = 136,   /* template_module_header  */
  YYSYMBOL_template_module = 137,          /* template_module  */
  YYSYMBOL_138_20 = 138,                   /* $@20  */
  YYSYMBOL_139_21 = 139,                   /* $@21  */
  YYSYMBOL_140_22 = 140,                   /* $@22  */
  YYSYMBOL_141_23 = 141,                   /* $@23  */
  YYSYMBOL_142_24 = 142,                   /* $@24  */
  YYSYMBOL_at_least_one_tpl_definition = 143, /* at_least_one_tpl_definition  */
  YYSYMBOL_tpl_definitions = 144,          /* tpl_definitions  */
  YYSYMBOL_tpl_definition = 145,           /* tpl_definition  */
  YYSYMBOL_template_module_ref = 146,      /* template_module_ref  */
  YYSYMBOL_147_25 = 147,                   /* $@25  */
  YYSYMBOL_148_26 = 148,                   /* $@26  */
  YYSYMBOL_template_module_inst = 149,     /* template_module_inst  */
  YYSYMBOL_150_27 = 150,                   /* $@27  */
  YYSYMBOL_151_28 = 151,                   /* $@28  */
  YYSYMBOL_interface_def = 152,            /* interface_def  */
  YYSYMBOL_interface = 153,                /* interface  */
  YYSYMBOL_154_29 = 154,                   /* $@29  */
  YYSYMBOL_155_30 = 155,                   /* $@30  */
  YYSYMBOL_156_31 = 156,                   /* $@31  */
  YYSYMBOL_interface_decl = 157,           /* interface_decl  */
  YYSYMBOL_158_32 = 158,                   /* $@32  */
  YYSYMBOL_interface_header = 159,         /* interface_header  */
  YYSYMBOL_inheritance_spec = 160,         /* inheritance_spec  */
  YYSYMBOL_161_33 = 161,                   /* $@33  */
  YYSYMBOL_value_def = 162,                /* value_def  */
  YYSYMBOL_valuetype = 163,                /* valuetype  */
  YYSYMBOL_value_concrete_decl = 164,      /* value_concrete_decl  */
  YYSYMBOL_165_34 = 165,                   /* @34  */
  YYSYMBOL_166_35 = 166,                   /* $@35  */
  YYSYMBOL_167_36 = 167,                   /* $@36  */
  YYSYMBOL_value_abs_decl = 168,           /* value_abs_decl  */
  YYSYMBOL_169_37 = 169,                   /* $@37  */
  YYSYMBOL_170_38 = 170,                   /* $@38  */
  YYSYMBOL_171_39 = 171,                   /* $@39  */
  YYSYMBOL_value_header = 172,             /* value_header  */
  YYSYMBOL_173_40 = 173,                   /* $@40  */
  YYSYMBOL_value_decl = 174,               /* value_decl  */
  YYSYMBOL_175_41 = 175,                   /* $@41  */
  YYSYMBOL_opt_truncatable = 176,          /* opt_truncatable  */
  YYSYMBOL_supports_spec = 177,            /* supports_spec  */
  YYSYMBOL_value_forward_decl = 178,       /* value_forward_decl  */
  YYSYMBOL_value_box_decl = 179,           /* value_box_decl  */
  YYSYMBOL_value_elements = 180,           /* value_elements  */
  YYSYMBOL_value_element = 181,            /* value_element  */
  YYSYMBOL_182_42 = 182,                   /* @42  */
  YYSYMBOL_visibility = 183,               /* visibility  */
  YYSYMBOL_state_member = 184,             /* state_member  */
  YYSYMBOL_exports = 185,                  /* exports  */
  YYSYMBOL_at_least_one_export = 186,      /* at_least_one_export  */
  YYSYMBOL_export = 187,                   /* export  */
  YYSYMBOL_188_43 = 188,                   /* $@43  */
  YYSYMBOL_189_44 = 189,                   /* $@44  */
  YYSYMBOL_190_45 = 190,                   /* $@45  */
  YYSYMBOL_191_46 = 191,                   /* $@46  */
  YYSYMBOL_192_47 = 192,                   /* $@47  */
  YYSYMBOL_193_48 = 193,                   /* $@48  */
  YYSYMBOL_194_49 = 194,                   /* $@49  */
  YYSYMBOL_195_50 = 195,                   /* $@50  */
  YYSYMBOL_at_least_one_scoped_name = 196, /* at_least_one_scoped_name  */
  YYSYMBOL_scoped_names = 197,             /* scoped_names  */
  YYSYMBOL_198_51 = 198,                   /* $@51  */
  YYSYMBOL_scoped_name = 199,              /* scoped_name  */
  YYSYMBOL_200_52 = 200,                   /* $@52  */
  YYSYMBOL_201_53 = 201,                   /* $@53  */
  YYSYMBOL_id = 202,                       /* id  */
  YYSYMBOL_defining_id = 203,              /* defining_id  */
  YYSYMBOL_interface_forward = 204,        /* interface_forward  */
  YYSYMBOL_const_dcl = 205,                /* const_dcl  */
  YYSYMBOL_206_54 = 206,                   /* $@54  */
  YYSYMBOL_207_55 = 207,                   /* $@55  */
  YYSYMBOL_208_56 = 208,                   /* $@56  */
  YYSYMBOL_209_57 = 209,                   /* $@57  */
  YYSYMBOL_const_type = 210,               /* const_type  */
  YYSYMBOL_expression = 211,               /* expression  */
  YYSYMBOL_const_expr = 212,               /* const_expr  */
  YYSYMBOL_or_expr = 213,                  /* or_expr  */
  YYSYMBOL_xor_expr = 214,                 /* xor_expr  */
  YYSYMBOL_and_expr = 215,                 /* and_expr  */
  YYSYMBOL_shift_expr = 216,               /* shift_expr  */
  YYSYMBOL_add_expr = 217,                 /* add_expr  */
  YYSYMBOL_mult_expr = 218,                /* mult_expr  */
  YYSYMBOL_unary_expr = 219,               /* unary_expr  */
  YYSYMBOL_primary_expr = 220,             /* primary_expr  */
  YYSYMBOL_literal = 221,                  /* literal  */
  YYSYMBOL_positive_int_expr = 222,        /* positive_int_expr  */
  YYSYMBOL_annotation_dcl = 223,           /* annotation_dcl  */
  YYSYMBOL_224_58 = 224,                   /* $@58  */
  YYSYMBOL_annotation_body = 225,          /* annotation_body  */
  YYSYMBOL_annotation_statement = 226,     /* annotation_statement  */
  YYSYMBOL_227_59 = 227,                   /* $@59  */
  YYSYMBOL_annotation_member_type = 228,   /* annotation_member_type  */
  YYSYMBOL_annotation_member = 229,        /* annotation_member  */
  YYSYMBOL_annotation_member_default = 230, /* annotation_member_default  */
  YYSYMBOL_at_least_one_annotation = 231,  /* at_least_one_annotation  */
  YYSYMBOL_annotations_maybe = 232,        /* annotations_maybe  */
  YYSYMBOL_annotation_appl = 233,          /* annotation_appl  */
  YYSYMBOL_234_60 = 234,                   /* @60  */
  YYSYMBOL_annotation_appl_params_maybe = 235, /* annotation_appl_params_maybe  */
  YYSYMBOL_annotation_appl_params = 236,   /* annotation_appl_params  */
  YYSYMBOL_named_annotation_appl_params = 237, /* named_annotation_appl_params  */
  YYSYMBOL_more_named_annotation_appl_params = 238, /* more_named_annotation_appl_params  */
  YYSYMBOL_named_annotation_appl_param = 239, /* named_annotation_appl_param  */
  YYSYMBOL_type_dcl = 240,                 /* type_dcl  */
  YYSYMBOL_241_61 = 241,                   /* $@61  */
  YYSYMBOL_type_declarator = 242,          /* type_declarator  */
  YYSYMBOL_243_62 = 243,                   /* $@62  */
  YYSYMBOL_type_spec = 244,                /* type_spec  */
  YYSYMBOL_simple_type_spec = 245,         /* simple_type_spec  */
  YYSYMBOL_base_type_spec = 246,           /* base_type_spec  */
  YYSYMBOL_template_type_spec = 247,       /* template_type_spec  */
  YYSYMBOL_constructed_type_spec = 248,    /* constructed_type_spec  */
  YYSYMBOL_constructed_forward_type_spec = 249, /* constructed_forward_type_spec  */
  YYSYMBOL_at_least_one_declarator = 250,  /* at_least_one_declarator  */
  YYSYMBOL_declarators = 251,              /* declarators  */
  YYSYMBOL_252_63 = 252,                   /* $@63  */
  YYSYMBOL_declarator = 253,               /* declarator  */
  YYSYMBOL_at_least_one_simple_declarator = 254, /* at_least_one_simple_declarator  */
  YYSYMBOL_simple_declarators = 255,       /* simple_declarators  */
  YYSYMBOL_256_64 = 256,                   /* $@64  */
  YYSYMBOL_simple_declarator = 257,        /* simple_declarator  */
  YYSYMBOL_complex_declarator = 258,       /* complex_declarator  */
  YYSYMBOL_integer_type = 259,             /* integer_type  */
  YYSYMBOL_signed_int = 260,               /* signed_int  */
  YYSYMBOL_unsigned_int = 261,             /* unsigned_int  */
  YYSYMBOL_floating_pt_type = 262,         /* floating_pt_type  */
  YYSYMBOL_fixed_type = 263,               /* fixed_type  */
  YYSYMBOL_char_type = 264,                /* char_type  */
  YYSYMBOL_octet_type = 265,               /* octet_type  */
  YYSYMBOL_boolean_type = 266,             /* boolean_type  */
  YYSYMBOL_any_type = 267,                 /* any_type  */
  YYSYMBOL_object_type = 268,              /* object_type  */
  YYSYMBOL_struct_decl = 269,              /* struct_decl  */
  YYSYMBOL_270_65 = 270,                   /* $@65  */
  YYSYMBOL_struct_type = 271,              /* struct_type  */
  YYSYMBOL_272_66 = 272,                   /* $@66  */
  YYSYMBOL_273_67 = 273,                   /* $@67  */
  YYSYMBOL_274_68 = 274,                   /* $@68  */
  YYSYMBOL_at_least_one_member = 275,      /* at_least_one_member  */
  YYSYMBOL_members = 276,                  /* members  */
  YYSYMBOL_member = 277,                   /* member  */
  YYSYMBOL_member_i = 278,                 /* member_i  */
  YYSYMBOL_279_69 = 279,                   /* $@69  */
  YYSYMBOL_280_70 = 280,                   /* $@70  */
  YYSYMBOL_281_71 = 281,                   /* $@71  */
  YYSYMBOL_union_decl = 282,               /* union_decl  */
  YYSYMBOL_283_72 = 283,                   /* $@72  */
  YYSYMBOL_union_type = 284,               /* union_type  */
  YYSYMBOL_285_73 = 285,                   /* $@73  */
  YYSYMBOL_286_74 = 286,                   /* $@74  */
  YYSYMBOL_287_75 = 287,                   /* $@75  */
  YYSYMBOL_288_76 = 288,                   /* $@76  */
  YYSYMBOL_289_77 = 289,                   /* $@77  */
  YYSYMBOL_290_78 = 290,                   /* $@78  */
  YYSYMBOL_switch_type_spec = 291,         /* switch_type_spec  */
  YYSYMBOL_at_least_one_case_branch = 292, /* at_least_one_case_branch  */
  YYSYMBOL_case_branches = 293,            /* case_branches  */
  YYSYMBOL_case_branch = 294,              /* case_branch  */
  YYSYMBOL_295_79 = 295,                   /* $@79  */
  YYSYMBOL_296_80 = 296,                   /* $@80  */
  YYSYMBOL_297_81 = 297,                   /* $@81  */
  YYSYMBOL_at_least_one_case_label = 298,  /* at_least_one_case_label  */
  YYSYMBOL_case_labels = 299,              /* case_labels  */
  YYSYMBOL_case_label = 300,               /* case_label  */
  YYSYMBOL_301_82 = 301,                   /* $@82  */
  YYSYMBOL_302_83 = 302,                   /* $@83  */
  YYSYMBOL_303_84 = 303,                   /* $@84  */
  YYSYMBOL_element_spec = 304,             /* element_spec  */
  YYSYMBOL_305_85 = 305,                   /* $@85  */
  YYSYMBOL_struct_forward_type = 306,      /* struct_forward_type  */
  YYSYMBOL_union_forward_type = 307,       /* union_forward_type  */
  YYSYMBOL_enum_type = 308,                /* enum_type  */
  YYSYMBOL_309_86 = 309,                   /* $@86  */
  YYSYMBOL_310_87 = 310,                   /* $@87  */
  YYSYMBOL_311_88 = 311,                   /* $@88  */
  YYSYMBOL_312_89 = 312,                   /* $@89  */
  YYSYMBOL_at_least_one_enumerator = 313,  /* at_least_one_enumerator  */
  YYSYMBOL_enumerators = 314,              /* enumerators  */
  YYSYMBOL_315_90 = 315,                   /* $@90  */
  YYSYMBOL_enumerator = 316,               /* enumerator  */
  YYSYMBOL_sequence_type_spec = 317,       /* sequence_type_spec  */
  YYSYMBOL_318_91 = 318,                   /* $@91  */
  YYSYMBOL_319_92 = 319,                   /* $@92  */
  YYSYMBOL_seq_head = 320,                 /* seq_head  */
  YYSYMBOL_321_93 = 321,                   /* $@93  */
  YYSYMBOL_322_94 = 322,                   /* $@94  */
  YYSYMBOL_fixed_type_spec = 323,          /* fixed_type_spec  */
  YYSYMBOL_string_type_spec = 324,         /* string_type_spec  */
  YYSYMBOL_325_95 = 325,                   /* $@95  */
  YYSYMBOL_326_96 = 326,                   /* $@96  */
  YYSYMBOL_string_head = 327,              /* string_head  */
  YYSYMBOL_wstring_type_spec = 328,        /* wstring_type_spec  */
  YYSYMBOL_329_97 = 329,                   /* $@97  */
  YYSYMBOL_330_98 = 330,                   /* $@98  */
  YYSYMBOL_wstring_head = 331,             /* wstring_head  */
  YYSYMBOL_array_declarator = 332,         /* array_declarator  */
  YYSYMBOL_333_99 = 333,                   /* $@99  */
  YYSYMBOL_at_least_one_array_dim = 334,   /* at_least_one_array_dim  */
  YYSYMBOL_array_dims = 335,               /* array_dims  */
  YYSYMBOL_array_dim = 336,                /* array_dim  */
  YYSYMBOL_337_100 = 337,                  /* $@100  */
  YYSYMBOL_338_101 = 338,                  /* $@101  */
  YYSYMBOL_attribute = 339,                /* attribute  */
  YYSYMBOL_attribute_readonly = 340,       /* attribute_readonly  */
  YYSYMBOL_341_102 = 341,                  /* $@102  */
  YYSYMBOL_342_103 = 342,                  /* $@103  */
  YYSYMBOL_343_104 = 343,                  /* $@104  */
  YYSYMBOL_344_105 = 344,                  /* $@105  */
  YYSYMBOL_attribute_readwrite = 345,      /* attribute_readwrite  */
  YYSYMBOL_346_106 = 346,                  /* $@106  */
  YYSYMBOL_347_107 = 347,                  /* $@107  */
  YYSYMBOL_348_108 = 348,                  /* $@108  */
  YYSYMBOL_349_109 = 349,                  /* $@109  */
  YYSYMBOL_exception = 350,                /* exception  */
  YYSYMBOL_351_110 = 351,                  /* $@110  */
  YYSYMBOL_352_111 = 352,                  /* @111  */
  YYSYMBOL_353_112 = 353,                  /* $@112  */
  YYSYMBOL_354_113 = 354,                  /* $@113  */
  YYSYMBOL_operation = 355,                /* operation  */
  YYSYMBOL_356_114 = 356,                  /* $@114  */
  YYSYMBOL_357_115 = 357,                  /* $@115  */
  YYSYMBOL_358_116 = 358,                  /* $@116  */
  YYSYMBOL_359_117 = 359,                  /* $@117  */
  YYSYMBOL_opt_op_attribute = 360,         /* opt_op_attribute  */
  YYSYMBOL_op_type_spec = 361,             /* op_type_spec  */
  YYSYMBOL_init_decl = 362,                /* init_decl  */
  YYSYMBOL_363_118 = 363,                  /* $@118  */
  YYSYMBOL_364_119 = 364,                  /* @119  */
  YYSYMBOL_365_120 = 365,                  /* $@120  */
  YYSYMBOL_init_parameter_list = 366,      /* init_parameter_list  */
  YYSYMBOL_367_121 = 367,                  /* $@121  */
  YYSYMBOL_368_122 = 368,                  /* $@122  */
  YYSYMBOL_at_least_one_in_parameter = 369, /* at_least_one_in_parameter  */
  YYSYMBOL_in_parameters = 370,            /* in_parameters  */
  YYSYMBOL_371_123 = 371,                  /* $@123  */
  YYSYMBOL_in_parameter = 372,             /* in_parameter  */
  YYSYMBOL_373_124 = 373,                  /* $@124  */
  YYSYMBOL_374_125 = 374,                  /* $@125  */
  YYSYMBOL_parameter_list = 375,           /* parameter_list  */
  YYSYMBOL_376_126 = 376,                  /* $@126  */
  YYSYMBOL_377_127 = 377,                  /* $@127  */
  YYSYMBOL_at_least_one_parameter = 378,   /* at_least_one_parameter  */
  YYSYMBOL_parameters = 379,               /* parameters  */
  YYSYMBOL_380_128 = 380,                  /* $@128  */
  YYSYMBOL_parameter = 381,                /* parameter  */
  YYSYMBOL_382_129 = 382,                  /* $@129  */
  YYSYMBOL_383_130 = 383,                  /* $@130  */
  YYSYMBOL_param_type_spec = 384,          /* param_type_spec  */
  YYSYMBOL_direction = 385,                /* direction  */
  YYSYMBOL_opt_raises = 386,               /* opt_raises  */
  YYSYMBOL_387_131 = 387,                  /* $@131  */
  YYSYMBOL_388_132 = 388,                  /* $@132  */
  YYSYMBOL_opt_getraises = 389,            /* opt_getraises  */
  YYSYMBOL_390_133 = 390,                  /* $@133  */
  YYSYMBOL_391_134 = 391,                  /* $@134  */
  YYSYMBOL_opt_setraises = 392,            /* opt_setraises  */
  YYSYMBOL_393_135 = 393,                  /* $@135  */
  YYSYMBOL_394_136 = 394,                  /* $@136  */
  YYSYMBOL_opt_context = 395,              /* opt_context  */
  YYSYMBOL_396_137 = 396,                  /* $@137  */
  YYSYMBOL_397_138 = 397,                  /* $@138  */
  YYSYMBOL_at_least_one_string_literal = 398, /* at_least_one_string_literal  */
  YYSYMBOL_string_literals = 399,          /* string_literals  */
  YYSYMBOL_400_139 = 400,                  /* $@139  */
  YYSYMBOL_typeid_dcl = 401,               /* typeid_dcl  */
  YYSYMBOL_typeprefix_dcl = 402,           /* typeprefix_dcl  */
  YYSYMBOL_component = 403,                /* component  */
  YYSYMBOL_component_forward_decl = 404,   /* component_forward_decl  */
  YYSYMBOL_component_decl = 405,           /* component_decl  */
  YYSYMBOL_406_140 = 406,                  /* @140  */
  YYSYMBOL_407_141 = 407,                  /* $@141  */
  YYSYMBOL_408_142 = 408,                  /* $@142  */
  YYSYMBOL_component_header = 409,         /* component_header  */
  YYSYMBOL_410_143 = 410,                  /* $@143  */
  YYSYMBOL_411_144 = 411,                  /* $@144  */
  YYSYMBOL_component_inheritance_spec = 412, /* component_inheritance_spec  */
  YYSYMBOL_413_145 = 413,                  /* $@145  */
  YYSYMBOL_component_exports = 414,        /* component_exports  */
  YYSYMBOL_component_export = 415,         /* component_export  */
  YYSYMBOL_416_146 = 416,                  /* $@146  */
  YYSYMBOL_417_147 = 417,                  /* $@147  */
  YYSYMBOL_418_148 = 418,                  /* $@148  */
  YYSYMBOL_419_149 = 419,                  /* $@149  */
  YYSYMBOL_420_150 = 420,                  /* $@150  */
  YYSYMBOL_421_151 = 421,                  /* $@151  */
  YYSYMBOL_422_152 = 422,                  /* $@152  */
  YYSYMBOL_provides_decl = 423,            /* provides_decl  */
  YYSYMBOL_interface_type = 424,           /* interface_type  */
  YYSYMBOL_uses_decl = 425,                /* uses_decl  */
  YYSYMBOL_uses_opt_multiple = 426,        /* uses_opt_multiple  */
  YYSYMBOL_opt_multiple = 427,             /* opt_multiple  */
  YYSYMBOL_emits_decl = 428,               /* emits_decl  */
  YYSYMBOL_publishes_decl = 429,           /* publishes_decl  */
  YYSYMBOL_consumes_decl = 430,            /* consumes_decl  */
  YYSYMBOL_home_decl = 431,                /* home_decl  */
  YYSYMBOL_432_153 = 432,                  /* $@153  */
  YYSYMBOL_home_header = 433,              /* home_header  */
  YYSYMBOL_434_154 = 434,                  /* $@154  */
  YYSYMBOL_435_155 = 435,                  /* $@155  */
  YYSYMBOL_436_156 = 436,                  /* $@156  */
  YYSYMBOL_437_157 = 437,                  /* $@157  */
  YYSYMBOL_438_158 = 438,                  /* $@158  */
  YYSYMBOL_439_159 = 439,                  /* $@159  */
  YYSYMBOL_home_inheritance_spec = 440,    /* home_inheritance_spec  */
  YYSYMBOL_441_160 = 441,                  /* $@160  */
  YYSYMBOL_primary_key_spec = 442,         /* primary_key_spec  */
  YYSYMBOL_home_body = 443,                /* home_body  */
  YYSYMBOL_444_161 = 444,                  /* $@161  */
  YYSYMBOL_445_162 = 445,                  /* $@162  */
  YYSYMBOL_home_exports = 446,             /* home_exports  */
  YYSYMBOL_home_export = 447,              /* home_export  */
  YYSYMBOL_448_163 = 448,                  /* $@163  */
  YYSYMBOL_449_164 = 449,                  /* $@164  */
  YYSYMBOL_factory_decl = 450,             /* factory_decl  */
  YYSYMBOL_451_165 = 451,                  /* $@165  */
  YYSYMBOL_452_166 = 452,                  /* $@166  */
  YYSYMBOL_finder_decl = 453,              /* finder_decl  */
  YYSYMBOL_454_167 = 454,                  /* $@167  */
  YYSYMBOL_455_168 = 455,                  /* $@168  */
  YYSYMBOL_event = 456,                    /* event  */
  YYSYMBOL_event_forward_decl = 457,       /* event_forward_decl  */
  YYSYMBOL_event_concrete_forward_decl = 458, /* event_concrete_forward_decl  */
  YYSYMBOL_event_abs_forward_decl = 459,   /* event_abs_forward_decl  */
  YYSYMBOL_event_abs_decl = 460,           /* event_abs_decl  */
  YYSYMBOL_461_169 = 461,                  /* $@169  */
  YYSYMBOL_462_170 = 462,                  /* $@170  */
  YYSYMBOL_463_171 = 463,                  /* $@171  */
  YYSYMBOL_event_abs_header = 464,         /* event_abs_header  */
  YYSYMBOL_event_custom_header = 465,      /* event_custom_header  */
  YYSYMBOL_event_plain_header = 466,       /* event_plain_header  */
  YYSYMBOL_event_rest_of_header = 467,     /* event_rest_of_header  */
  YYSYMBOL_468_172 = 468,                  /* $@172  */
  YYSYMBOL_event_decl = 469,               /* event_decl  */
  YYSYMBOL_470_173 = 470,                  /* @173  */
  YYSYMBOL_471_174 = 471,                  /* $@174  */
  YYSYMBOL_472_175 = 472,                  /* $@175  */
  YYSYMBOL_event_header = 473,             /* event_header  */
  YYSYMBOL_formal_parameter_type = 474,    /* formal_parameter_type  */
  YYSYMBOL_at_least_one_formal_parameter = 475, /* at_least_one_formal_parameter  */
  YYSYMBOL_formal_parameters = 476,        /* formal_parameters  */
  YYSYMBOL_formal_parameter = 477,         /* formal_parameter  */
  YYSYMBOL_at_least_one_formal_parameter_name = 478, /* at_least_one_formal_parameter_name  */
  YYSYMBOL_formal_parameter_names = 479,   /* formal_parameter_names  */
  YYSYMBOL_formal_parameter_name = 480,    /* formal_parameter_name  */
  YYSYMBOL_porttype_decl = 481,            /* porttype_decl  */
  YYSYMBOL_482_176 = 482,                  /* $@176  */
  YYSYMBOL_483_177 = 483,                  /* @177  */
  YYSYMBOL_484_178 = 484,                  /* $@178  */
  YYSYMBOL_485_179 = 485,                  /* $@179  */
  YYSYMBOL_at_least_one_port_export = 486, /* at_least_one_port_export  */
  YYSYMBOL_port_exports = 487,             /* port_exports  */
  YYSYMBOL_port_export = 488,              /* port_export  */
  YYSYMBOL_489_180 = 489,                  /* $@180  */
  YYSYMBOL_extended_port_decl = 490,       /* extended_port_decl  */
  YYSYMBOL_at_least_one_actual_parameter = 491, /* at_least_one_actual_parameter  */
  YYSYMBOL_actual_parameters = 492,        /* actual_parameters  */
  YYSYMBOL_actual_parameter = 493,         /* actual_parameter  */
  YYSYMBOL_connector_decl = 494,           /* connector_decl  */
  YYSYMBOL_connector_header = 495,         /* connector_header  */
  YYSYMBOL_496_181 = 496,                  /* $@181  */
  YYSYMBOL_497_182 = 497,                  /* $@182  */
  YYSYMBOL_connector_body = 498,           /* connector_body  */
  YYSYMBOL_499_183 = 499,                  /* $@183  */
  YYSYMBOL_500_184 = 500,                  /* $@184  */
  YYSYMBOL_connector_exports = 501,        /* connector_exports  */
  YYSYMBOL_connector_export = 502,         /* connector_export  */
  YYSYMBOL_503_185 = 503,                  /* $@185  */
  YYSYMBOL_504_186 = 504,                  /* $@186  */
  YYSYMBOL_505_187 = 505,                  /* $@187  */
  YYSYMBOL_506_188 = 506                   /* $@188  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1637

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  109
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  398
/* YYNRULES -- Number of rules.  */
#define YYNRULES  599
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  880

#define YYMAXUTOK   342


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   103,    98,     2,
     105,   106,   101,    99,    94,   100,     2,   102,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    93,    88,
      91,    95,    92,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   107,     2,   108,    97,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    89,    96,    90,   104,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87
};

#if YYDEBUG
  /* YYRLINEYYN -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   404,   404,   407,   408,   416,   431,   435,   436,   437,
     442,   441,   450,   449,   458,   457,   466,   465,   474,   473,
     482,   481,   490,   489,   498,   497,   506,   505,   514,   513,
     522,   521,   530,   529,   538,   537,   546,   545,   554,   553,
     567,   566,   578,   617,   621,   577,   637,   645,   659,   669,
     699,   703,   644,   728,   732,   733,   737,   738,   743,   748,
     742,   834,   839,   833,   910,   911,   916,   954,   958,   915,
     975,   974,   986,  1023,  1053,  1086,  1085,  1094,  1101,  1102,
    1103,  1104,  1108,  1113,  1118,  1163,  1167,  1117,  1196,  1239,
    1243,  1194,  1262,  1260,  1300,  1299,  1311,  1315,  1322,  1327,
    1334,  1359,  1387,  1453,  1472,  1476,  1480,  1481,  1493,  1492,
    1510,  1514,  1521,  1540,  1541,  1545,  1560,  1565,  1564,  1573,
    1572,  1581,  1580,  1589,  1588,  1597,  1596,  1605,  1604,  1613,
    1612,  1621,  1620,  1633,  1645,  1643,  1668,  1675,  1685,  1684,
    1710,  1708,  1733,  1743,  1754,  1798,  1825,  1857,  1861,  1865,
    1869,  1856,  1931,  1932,  1933,  1934,  1935,  1936,  1937,  1941,
    1945,  2013,  2015,  2017,  2018,  2030,  2031,  2043,  2044,  2056,
    2057,  2066,  2078,  2079,  2088,  2100,  2101,  2110,  2119,  2131,
    2132,  2141,  2150,  2162,  2219,  2220,  2227,  2231,  2236,  2243,
    2250,  2254,  2259,  2263,  2267,  2271,  2278,  2347,  2346,  2375,
    2376,  2380,  2381,  2382,  2384,  2383,  2392,  2393,  2397,  2453,
    2457,  2464,  2477,  2487,  2495,  2494,  2582,  2586,  2593,  2602,
    2609,  2617,  2623,  2630,  2643,  2642,  2651,  2655,  2659,  2663,
    2691,  2699,  2698,  2769,  2770,  2774,  2781,  2782,  2808,  2809,
    2810,  2811,  2812,  2813,  2814,  2815,  2819,  2820,  2821,  2822,
    2826,  2827,  2828,  2832,  2833,  2837,  2849,  2847,  2872,  2879,
    2880,  2884,  2896,  2894,  2919,  2926,  2942,  2960,  2961,  2965,
    2969,  2973,  2980,  2984,  2988,  2995,  2999,  3003,  3010,  3017,
    3021,  3028,  3035,  3042,  3049,  3057,  3056,  3070,  3101,  3105,
    3069,  3122,  3125,  3126,  3130,  3148,  3152,  3147,  3210,  3209,
    3222,  3221,  3234,  3238,  3271,  3275,  3334,  3338,  3233,  3360,
    3367,  3380,  3389,  3396,  3397,  3506,  3509,  3510,  3515,  3519,
    3514,  3555,  3554,  3566,  3576,  3594,  3602,  3601,  3615,  3619,
    3614,  3635,  3634,  3684,  3709,  3733,  3737,  3768,  3772,  3732,
    3796,  3801,  3799,  3805,  3809,  3849,  3853,  3847,  3941,  4012,
    4021,  4011,  4035,  4045,  4049,  4043,  4094,  4120,  4129,  4133,
    4127,  4169,  4195,  4203,  4202,  4249,  4259,  4277,  4285,  4289,
    4284,  4349,  4350,  4355,  4359,  4363,  4367,  4354,  4426,  4430,
    4434,  4438,  4425,  4506,  4510,  4542,  4546,  4505,  4563,  4567,
    4628,  4632,  4562,  4669,  4674,  4679,  4686,  4687,  4698,  4703,
    4746,  4697,  4768,  4767,  4776,  4775,  4786,  4791,  4789,  4795,
    4800,  4804,  4799,  4843,  4842,  4851,  4850,  4861,  4866,  4864,
    4870,  4875,  4879,  4874,  4924,  4931,  4932,  4933,  5040,  5044,
    5048,  5056,  5060,  5055,  5069,  5077,  5081,  5076,  5090,  5098,
    5102,  5097,  5111,  5119,  5123,  5118,  5132,  5139,  5151,  5149,
    5172,  5179,  5209,  5248,  5249,  5253,  5284,  5325,  5329,  5283,
    5348,  5352,  5346,  5393,  5392,  5400,  5407,  5422,  5423,  5428,
    5427,  5437,  5436,  5446,  5445,  5455,  5454,  5464,  5463,  5473,
    5472,  5482,  5481,  5492,  5585,  5591,  5616,  5723,  5732,  5736,
    5743,  5818,  5890,  5966,  5965,  6015,  6019,  6023,  6027,  6031,
    6035,  6014,  6088,  6087,  6095,  6102,  6107,  6115,  6119,  6114,
    6129,  6130,  6134,  6136,  6135,  6144,  6143,  6156,  6179,  6154,
    6205,  6232,  6203,  6256,  6257,  6258,  6262,  6263,  6267,  6296,
    6328,  6372,  6376,  6326,  6393,  6402,  6420,  6431,  6430,  6468,
    6518,  6522,  6466,  6539,  6543,  6550,  6554,  6558,  6562,  6566,
    6570,  6574,  6578,  6582,  6586,  6594,  6625,  6638,  6645,  6670,
    6688,  6695,  6710,  6717,  6727,  6731,  6750,  6758,  6726,  6773,
    6788,  6792,  6793,  6797,  6798,  6800,  6799,  6810,  6877,  6925,
    6941,  6954,  6961,  7020,  7028,  7032,  7027,  7093,  7097,  7092,
    7110,  7111,  7116,  7115,  7124,  7123,  7132,  7131,  7140,  7139
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IDENTIFIER",
  "IDL_CONST", "IDL_MODULE", "IDL_INTERFACE", "IDL_TYPEDEF", "IDL_LONG",
  "IDL_SHORT", "IDL_UNSIGNED", "IDL_DOUBLE", "IDL_FLOAT", "IDL_CHAR",
  "IDL_WCHAR", "IDL_OCTET", "IDL_BOOLEAN", "IDL_FIXED", "IDL_ANY",
  "IDL_OBJECT", "IDL_STRUCT", "IDL_UNION", "IDL_SWITCH", "IDL_ENUM",
  "IDL_SEQUENCE", "IDL_STRING", "IDL_WSTRING", "IDL_EXCEPTION", "IDL_CASE",
  "IDL_DEFAULT", "IDL_READONLY", "IDL_ATTRIBUTE", "IDL_ONEWAY",
  "IDL_IDEMPOTENT", "IDL_VOID", "IDL_IN", "IDL_OUT", "IDL_INOUT",
  "IDL_RAISES", "IDL_CONTEXT", "IDL_NATIVE", "IDL_LOCAL", "IDL_ABSTRACT",
  "IDL_CUSTOM", "IDL_FACTORY", "IDL_PRIVATE", "IDL_PUBLIC", "IDL_SUPPORTS",
  "IDL_TRUNCATABLE", "IDL_VALUETYPE", "IDL_COMPONENT", "IDL_CONSUMES",
  "IDL_EMITS", "IDL_EVENTTYPE", "IDL_FINDER", "IDL_GETRAISES", "IDL_HOME",
  "IDL_IMPORT", "IDL_MULTIPLE", "IDL_PRIMARYKEY", "IDL_PROVIDES",
  "IDL_PUBLISHES", "IDL_SETRAISES", "IDL_TYPEID", "IDL_TYPEPREFIX",
  "IDL_USES", "IDL_MANAGES", "IDL_TYPENAME", "IDL_PORT", "IDL_MIRRORPORT",
  "IDL_PORTTYPE", "IDL_CONNECTOR", "IDL_ALIAS", "IDL_INTEGER_LITERAL",
  "IDL_UINTEGER_LITERAL", "IDL_STRING_LITERAL", "IDL_CHARACTER_LITERAL",
  "IDL_FLOATING_PT_LITERAL", "IDL_FIXED_PT_LITERAL", "IDL_TRUETOK",
  "IDL_FALSETOK", "IDL_SCOPE_DELIMITOR", "IDL_LEFT_SHIFT",
  "IDL_RIGHT_SHIFT", "IDL_WCHAR_LITERAL", "IDL_WSTRING_LITERAL",
  "IDL_ANNOTATION_DECL", "IDL_ANNOTATION_SYMBOL", "';'", "'{'", "'}'",
  "'<'", "'>'", "':'", "','", "'='", "'|'", "'^'", "'&'", "'+'", "'-'",
  "'*'", "'/'", "'%'", "'~'", "'('", "')'", "'['", "']'", "$accept",
  "start", "definitions", "at_least_one_definition", "definition",
  "fixed_definition", "$@1", "$@2", "$@3", "$@4", "$@5", "$@6", "$@7",
  "$@8", "$@9", "$@10", "$@11", "$@12", "$@13", "$@14", "$@15",
  "module_header", "$@16", "module", "@17", "$@18", "$@19",
  "template_module_header", "template_module", "$@20", "$@21", "$@22",
  "$@23", "$@24", "at_least_one_tpl_definition", "tpl_definitions",
  "tpl_definition", "template_module_ref", "$@25", "$@26",
  "template_module_inst", "$@27", "$@28", "interface_def", "interface",
  "$@29", "$@30", "$@31", "interface_decl", "$@32", "interface_header",
  "inheritance_spec", "$@33", "value_def", "valuetype",
  "value_concrete_decl", "@34", "$@35", "$@36", "value_abs_decl", "$@37",
  "$@38", "$@39", "value_header", "$@40", "value_decl", "$@41",
  "opt_truncatable", "supports_spec", "value_forward_decl",
  "value_box_decl", "value_elements", "value_element", "@42", "visibility",
  "state_member", "exports", "at_least_one_export", "export", "$@43",
  "$@44", "$@45", "$@46", "$@47", "$@48", "$@49", "$@50",
  "at_least_one_scoped_name", "scoped_names", "$@51", "scoped_name",
  "$@52", "$@53", "id", "defining_id", "interface_forward", "const_dcl",
  "$@54", "$@55", "$@56", "$@57", "const_type", "expression", "const_expr",
  "or_expr", "xor_expr", "and_expr", "shift_expr", "add_expr", "mult_expr",
  "unary_expr", "primary_expr", "literal", "positive_int_expr",
  "annotation_dcl", "$@58", "annotation_body", "annotation_statement",
  "$@59", "annotation_member_type", "annotation_member",
  "annotation_member_default", "at_least_one_annotation",
  "annotations_maybe", "annotation_appl", "@60",
  "annotation_appl_params_maybe", "annotation_appl_params",
  "named_annotation_appl_params", "more_named_annotation_appl_params",
  "named_annotation_appl_param", "type_dcl", "$@61", "type_declarator",
  "$@62", "type_spec", "simple_type_spec", "base_type_spec",
  "template_type_spec", "constructed_type_spec",
  "constructed_forward_type_spec", "at_least_one_declarator",
  "declarators", "$@63", "declarator", "at_least_one_simple_declarator",
  "simple_declarators", "$@64", "simple_declarator", "complex_declarator",
  "integer_type", "signed_int", "unsigned_int", "floating_pt_type",
  "fixed_type", "char_type", "octet_type", "boolean_type", "any_type",
  "object_type", "struct_decl", "$@65", "struct_type", "$@66", "$@67",
  "$@68", "at_least_one_member", "members", "member", "member_i", "$@69",
  "$@70", "$@71", "union_decl", "$@72", "union_type", "$@73", "$@74",
  "$@75", "$@76", "$@77", "$@78", "switch_type_spec",
  "at_least_one_case_branch", "case_branches", "case_branch", "$@79",
  "$@80", "$@81", "at_least_one_case_label", "case_labels", "case_label",
  "$@82", "$@83", "$@84", "element_spec", "$@85", "struct_forward_type",
  "union_forward_type", "enum_type", "$@86", "$@87", "$@88", "$@89",
  "at_least_one_enumerator", "enumerators", "$@90", "enumerator",
  "sequence_type_spec", "$@91", "$@92", "seq_head", "$@93", "$@94",
  "fixed_type_spec", "string_type_spec", "$@95", "$@96", "string_head",
  "wstring_type_spec", "$@97", "$@98", "wstring_head", "array_declarator",
  "$@99", "at_least_one_array_dim", "array_dims", "array_dim", "$@100",
  "$@101", "attribute", "attribute_readonly", "$@102", "$@103", "$@104",
  "$@105", "attribute_readwrite", "$@106", "$@107", "$@108", "$@109",
  "exception", "$@110", "@111", "$@112", "$@113", "operation", "$@114",
  "$@115", "$@116", "$@117", "opt_op_attribute", "op_type_spec",
  "init_decl", "$@118", "@119", "$@120", "init_parameter_list", "$@121",
  "$@122", "at_least_one_in_parameter", "in_parameters", "$@123",
  "in_parameter", "$@124", "$@125", "parameter_list", "$@126", "$@127",
  "at_least_one_parameter", "parameters", "$@128", "parameter", "$@129",
  "$@130", "param_type_spec", "direction", "opt_raises", "$@131", "$@132",
  "opt_getraises", "$@133", "$@134", "opt_setraises", "$@135", "$@136",
  "opt_context", "$@137", "$@138", "at_least_one_string_literal",
  "string_literals", "$@139", "typeid_dcl", "typeprefix_dcl", "component",
  "component_forward_decl", "component_decl", "@140", "$@141", "$@142",
  "component_header", "$@143", "$@144", "component_inheritance_spec",
  "$@145", "component_exports", "component_export", "$@146", "$@147",
  "$@148", "$@149", "$@150", "$@151", "$@152", "provides_decl",
  "interface_type", "uses_decl", "uses_opt_multiple", "opt_multiple",
  "emits_decl", "publishes_decl", "consumes_decl", "home_decl", "$@153",
  "home_header", "$@154", "$@155", "$@156", "$@157", "$@158", "$@159",
  "home_inheritance_spec", "$@160", "primary_key_spec", "home_body",
  "$@161", "$@162", "home_exports", "home_export", "$@163", "$@164",
  "factory_decl", "$@165", "$@166", "finder_decl", "$@167", "$@168",
  "event", "event_forward_decl", "event_concrete_forward_decl",
  "event_abs_forward_decl", "event_abs_decl", "$@169", "$@170", "$@171",
  "event_abs_header", "event_custom_header", "event_plain_header",
  "event_rest_of_header", "$@172", "event_decl", "@173", "$@174", "$@175",
  "event_header", "formal_parameter_type", "at_least_one_formal_parameter",
  "formal_parameters", "formal_parameter",
  "at_least_one_formal_parameter_name", "formal_parameter_names",
  "formal_parameter_name", "porttype_decl", "$@176", "@177", "$@178",
  "$@179", "at_least_one_port_export", "port_exports", "port_export",
  "$@180", "extended_port_decl", "at_least_one_actual_parameter",
  "actual_parameters", "actual_parameter", "connector_decl",
  "connector_header", "$@181", "$@182", "connector_body", "$@183", "$@184",
  "connector_exports", "connector_export", "$@185", "$@186", "$@187",
  "$@188", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,    59,   123,
     125,    60,    62,    58,    44,    61,   124,    94,    38,    43,
      45,    42,    47,    37,   126,    40,    41,    91,    93
};
#endif

#define YYPACT_NINF (-613)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-568)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACTSTATE-NUM -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -613,    52,  1211,  -613,  -613,  -613,  -613,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,    57,    78,    45,   157,  -613,    57,
      57,  -613,    85,    85,  -613,  -613,    57,  -613,  -613,    12,
    -613,   237,   -17,     9,  -613,  -613,   -20,  -613,  -613,  -613,
    -613,  -613,  -613,   537,  -613,  -613,  -613,  -613,  -613,  1342,
      32,  -613,  -613,    44,  -613,   119,  -613,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,   111,  -613,  -613,  -613,   111,  -613,
    -613,    75,   132,   857,    85,    57,  1556,    57,    57,    57,
      57,  -613,  -613,  -613,   -19,    57,    77,  -613,   107,    57,
    -613,   111,    57,   134,   144,    57,  -613,  -613,   -25,  -613,
       1,   256,  -613,   151,  -613,   173,   184,   331,  -613,  -613,
    -613,   199,   251,  -613,   218,   220,   229,   207,  -613,   190,
    -613,  -613,  -613,  -613,  -613,  -613,   234,  -613,  -613,  -613,
    -613,  -613,  -613,   239,  -613,  -613,  -613,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,
    -613,   119,  -613,  -613,  -613,    65,  -613,  -613,   236,  -613,
     241,   245,   246,  -613,    85,   249,   252,   250,  -613,   255,
     257,   265,   268,   270,   269,   271,   273,  -613,  -613,  -613,
     274,   275,  -613,  -613,  -613,  -613,   239,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,  -613,  -613,   239,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,  -613,   278,  -613,   280,  -613,  -613,
     235,  -613,   341,  -613,  -613,  -613,  -613,    55,  -613,  -613,
    -613,   857,  -613,  -613,  -613,  -613,   281,  -613,  -613,  -613,
    -613,   367,  -613,  -613,    50,   293,  -613,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,   363,  -613,   171,   287,   343,  -613,
    -613,  -613,  -613,  -613,  -613,   239,  -613,  -613,   292,  -613,
    -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,   343,   302,
     310,  -613,  -613,  -613,    57,    57,   311,   312,  -613,  -613,
    -613,   313,  -613,   341,   315,  -613,  -613,  -613,  -613,  -613,
     402,  -613,   318,   319,  -613,  -613,  -613,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,   204,   204,   204,   171,   239,  -613,
    -613,   316,   324,   309,   131,   135,   136,  -613,  -613,  -613,
    -613,  -613,    85,  -613,  -613,  -613,  -613,   321,  -613,    85,
    -613,   171,   171,   171,   317,  -613,  -613,  -613,  -613,  -613,
    -613,  -613,   159,  -613,    71,  -613,  -613,  -613,  -613,  -613,
    -613,  -613,  -613,    85,   343,  -613,  -613,  -613,  -613,   235,
     689,  1264,   326,   333,  -613,   331,  -613,  -613,  -613,   320,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     334,    57,  -613,   239,   986,  -613,   564,   171,  -613,  -613,
    -613,  -613,  -613,   171,  -613,   876,  -613,  -613,  -613,   601,
     902,  -613,  -613,  -613,  -613,    98,   369,    85,    85,  -613,
    -613,  -613,  -613,  -613,    98,  -613,   339,  -613,   335,  -613,
     337,  -613,  -613,  1021,   239,  -613,    85,   343,  -613,  -613,
    -613,  -613,   344,  -613,  -613,    57,  -613,  -613,   345,   346,
     431,   348,  -613,  -613,   324,   309,   131,   135,   135,   136,
     136,  -613,  -613,  -613,  -613,  -613,   347,  -613,  -613,  -613,
     349,  -613,  -613,  1497,  -613,  -613,  -613,  -613,   463,  -613,
    -613,  -613,  -613,  -613,   350,  -613,  1531,  -613,  -613,  1422,
    -613,   351,  1079,   352,   358,   360,   364,  -613,   336,  -613,
     359,  -613,  -613,  -613,   372,   373,   130,    85,    85,    85,
     461,  -613,   377,  -613,  -613,  -613,  -613,  -613,  -613,  -613,
      57,    57,  -613,   379,  -613,  -613,  -613,  1106,   817,   452,
      96,  -613,   239,   341,  -613,  -613,    88,    90,   405,   406,
     407,   341,   408,  -613,  -613,    86,  -613,    60,  -613,  -613,
     400,   409,   239,  -613,   410,   137,  1556,  -613,   469,  -613,
    -613,  -613,  -613,    50,  -613,   413,  -613,   414,  -613,   415,
     416,   417,   421,  -613,   239,  -613,  -613,  -613,  -613,  -613,
     426,   429,   517,  -613,  -613,  -613,   435,  -613,  -613,  -613,
    -613,  -613,   171,  -613,   341,  -613,   437,    57,  -613,  -613,
     519,   239,  -613,  -613,  -613,  -613,  -613,  -613,   113,   113,
     113,  -613,   440,  -613,   443,   444,   445,   447,   448,   450,
    -613,  -613,  -613,   451,   453,   474,   476,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,  -613,  -613,  -613,   171,  -613,  -613,
    -613,    57,  -613,   479,   465,   496,  -613,   522,   502,   137,
    -613,   454,   505,  -613,   511,   171,   512,  1289,  -613,    85,
    -613,  -613,  -613,  -613,  -613,  -613,   598,  -613,  -613,  -613,
    -613,  -613,   364,   359,  -613,  -613,   497,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,  -613,  -613,  -613,   501,   501,  -613,
    -613,  -613,  -613,    96,    57,  -613,   171,   495,  -613,  -613,
    -613,  -613,  -613,  -613,  -613,   523,  -613,  -613,  -613,  -613,
    -613,    85,  -613,  -613,  -613,  -613,   524,   239,  -613,   501,
    -613,   525,  -613,   572,  -613,  -613,  -613,  -613,  -613,  -613,
    -613,  -613,    85,  -613,   239,   526,  1157,  -613,   509,  -613,
    -613,   528,   513,   580,   582,   582,    57,   566,   532,   514,
    -613,   239,   538,  -613,  -613,   527,  -613,   582,  -613,  -613,
    -613,   529,  -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,
    -613,   575,   633,   533,   230,   582,  -613,   202,    96,  -613,
     544,   535,   582,   536,   581,    57,    85,  -613,  -613,   550,
    -613,  -613,  -613,  -613,  -613,   540,  -613,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,   239,  -613,   553,  -613,   554,    96,
     605,   562,   171,   563,   565,    58,  -613,   240,    57,   580,
      85,    85,   552,    57,   633,  -613,  -613,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,   708,  -613,  -613,  -613,   558,   559,
    -613,  -613,  -613,   230,    57,   555,   574,  -613,  -613,  -613,
    -613,    85,  -613,  -613,  -613,  -613,    57,   570,   571,   584,
    -613,  -613,  -613,  -613,   573,   578,  -613,  -613,   593,  -613
};

  /* YYDEFACTSTATE-NUM -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       4,     0,     0,     3,     1,    38,   147,    40,    70,   224,
     285,   300,   335,   383,     0,     0,     0,     0,    94,     0,
       0,   495,     0,     0,   564,   584,     0,     6,     7,    42,
      24,    61,     0,     0,    22,    64,    77,    66,    26,    78,
      83,    79,    84,    77,    80,    81,    65,    18,    10,     0,
       0,    12,   230,   287,   226,   334,   227,   253,   254,   228,
      20,    14,    16,    28,   454,   453,   456,    30,   493,    32,
     525,   527,   526,   524,    77,   543,   544,   523,    77,    34,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   143,   265,   229,    77,     0,    77,    88,    77,     0,
      82,    77,     0,   460,   536,     0,   142,   138,     0,   137,
       0,     0,   213,     0,    46,     0,     0,     0,   213,     8,
       9,     0,    97,    72,     0,     0,     0,   269,   271,     0,
     275,   276,   279,   280,   281,   282,   278,   283,   284,   349,
     357,   362,    92,   237,   102,   233,   235,   236,   234,   238,
     267,   268,   239,   243,   240,   242,   241,   244,   245,   287,
     250,     0,   251,   252,   246,     0,   249,   247,   356,   248,
     361,     0,     0,     5,     0,   211,     0,     0,   302,     0,
       0,     0,     0,     0,     0,     0,     0,   537,   530,   539,
       0,     0,   587,   583,    39,   278,   160,   148,   152,   156,
     157,   153,   154,   155,   158,   159,    41,    71,   225,   231,
     286,   301,   336,   384,    73,   534,    74,     0,   535,    95,
     465,   496,     0,   451,   140,   452,   565,     0,   197,    43,
      25,     0,   550,   546,   547,   552,   549,   553,   551,   548,
     545,     0,    48,   557,     0,     0,    23,    96,    75,    67,
      27,    85,   270,   277,   272,   274,     0,     0,    99,   348,
     345,   353,   358,    19,    11,   214,    13,   288,     0,    21,
      15,    17,    29,   457,    31,   507,   494,    33,    99,     0,
       0,    35,    37,   591,     0,     0,     0,     0,    89,   463,
     461,   504,   139,     0,     0,   585,   212,   200,     4,   554,
       0,   558,     0,   555,   186,   187,   188,   190,   193,   192,
     194,   195,   191,   189,     0,     0,     0,     0,   183,   582,
     161,   162,   163,   165,   167,   169,   172,   175,   179,   184,
     581,    62,     0,   114,   105,   273,   196,     0,   350,     0,
      93,     0,     0,     0,   217,   213,   303,   468,   511,   538,
     531,   540,   588,   149,   265,   232,   258,   259,   260,   266,
     337,   385,   114,     0,    99,   502,   497,   141,   566,   465,
       0,     0,     3,     0,    49,     0,   180,   181,   182,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     579,     0,    76,   136,     0,   113,     0,     0,   213,    98,
     346,   354,   359,     0,   215,     0,   289,   293,   213,   213,
       0,   114,   105,   373,   378,     0,   489,     0,     0,   596,
     371,   372,   592,   594,     0,   598,     0,   590,     0,   213,
     255,   213,   293,     0,   464,   462,     0,    99,   572,   586,
     204,   198,     0,   206,   199,     0,   201,   207,     0,     0,
       0,     0,   556,   185,   164,   166,   168,   170,   171,   173,
     174,   176,   177,   178,   213,    63,   133,   131,   393,   394,
       0,   116,   123,     0,   117,   127,   125,   129,     0,   119,
     121,   398,   111,   110,     0,   104,     0,   106,   107,     0,
     108,     0,     0,     0,     0,     0,   137,   218,     0,   219,
     222,   298,   295,   294,     0,   213,     0,     0,     0,     0,
       0,   479,     0,   467,   469,   471,   473,   475,   477,   481,
       0,     0,   512,     0,   510,   513,   515,     0,     0,     0,
       0,   485,   484,     0,   488,   487,     0,     0,     0,     0,
       0,     0,     0,   589,   150,     0,   256,     0,   338,   343,
     213,     0,   503,   498,   571,   213,     0,   202,   210,   203,
      45,   559,    50,     0,   134,     0,    69,     0,   115,     0,
       0,     0,     0,   397,   427,   424,   425,   426,   388,   396,
       0,     0,     0,    87,   112,   103,     0,   352,   351,   347,
     355,   360,     0,   216,     0,   220,     0,     0,   290,   292,
     269,   314,   309,   310,   311,   312,   304,   313,     0,     0,
       0,   466,     0,   459,     0,     0,     0,     0,     0,     0,
     517,   520,   509,     0,     0,     0,     0,   374,   379,   483,
     577,   578,   597,   593,   595,   486,   599,     0,   368,   364,
     367,     0,   344,     0,   340,     0,    91,     0,     0,     0,
     575,     0,     0,   570,     0,     0,     0,     0,   580,     0,
     132,   124,   118,   128,   126,   130,     0,   120,   122,   399,
     109,   223,     0,   222,   299,   296,     0,   492,   490,   491,
     480,   470,   472,   474,   476,   478,   482,     0,     0,   514,
     516,   533,   542,     0,     0,   151,     0,   365,   257,   339,
     341,   387,   499,   568,   569,     0,   573,   574,   205,   209,
     208,     0,    56,    42,    51,    55,     0,   135,   389,     0,
     221,     0,   305,   402,   518,   521,   375,   380,   264,   369,
     366,   213,     0,   576,    58,     0,     0,    57,     0,   400,
     297,     0,     0,     0,   434,   434,     0,   438,   261,     0,
     342,   500,     0,    52,    54,   415,   390,   434,   306,   403,
     410,     0,   409,   431,   519,   522,   376,   435,   381,   262,
     370,   506,     0,     0,     0,   434,   401,     0,     0,   405,
     406,     0,   434,     0,   442,     0,     0,   501,   563,     0,
     562,   414,   428,   429,   430,     0,   420,   421,   391,   321,
     328,   326,   307,   317,   318,   325,   411,   407,   432,   377,
     436,   439,   382,   263,   505,    59,   560,   416,   417,     0,
     446,     0,     0,     0,     0,     0,   213,   323,     0,     0,
       0,     0,     0,     0,     0,   418,   422,   443,   392,   322,
     329,   327,   308,   316,     0,   324,   412,   408,     0,     0,
     440,    60,   561,     0,     0,     0,     0,   331,   319,   433,
     437,     0,   419,   423,   444,   330,     0,     0,     0,     0,
     332,   320,   441,   450,     0,   447,   445,   448,     0,   449
};

  /* YYPGOTONTERM-NUM.  */
static const yytype_int16 yypgoto[] =
{
    -613,  -613,   378,   382,   632,  -585,  -613,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,
    -613,  -576,  -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,  -613,  -613,   -54,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,   279,  -613,
    -613,   127,  -613,  -613,  -613,   666,  -613,  -613,  -613,  -613,
    -613,  -613,  -613,   669,  -613,   286,  -613,  -613,  -246,  -613,
    -613,   276,   197,  -613,  -613,  -613,  -315,  -613,  -353,  -613,
    -613,  -613,  -613,  -613,  -613,  -613,  -613,  -324,  -613,  -613,
     -22,  -613,  -613,  -191,   -10,  -613,     6,  -613,  -613,  -613,
    -613,  -189,    53,  -217,  -613,   314,   332,   353,   -78,   -73,
     -96,   -18,  -613,  -302,  -613,  -613,  -613,  -613,  -613,  -613,
    -613,  -613,    15,   -82,   645,  -613,  -613,  -613,  -613,    64,
     146,    16,  -613,   174,  -613,   -31,   253,  -440,  -613,  -613,
    -613,   141,  -613,  -613,  -612,    -5,  -613,  -613,    -7,  -613,
     -59,  -613,  -613,   -46,   -39,   -55,   -49,   -48,   376,  -613,
     -40,  -613,   -38,  -613,  -613,  -613,  -613,   323,   398,   261,
    -613,  -613,  -613,   -37,  -613,   -32,  -613,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,  -613,   -77,  -613,  -613,  -613,  -613,
    -613,   -76,  -613,  -613,  -613,  -613,  -613,  -613,  -613,   -41,
    -613,  -613,  -613,  -613,  -613,  -613,  -613,    27,  -613,  -613,
    -613,  -613,  -613,  -613,  -613,   -70,  -613,  -613,  -613,   -69,
    -613,  -613,  -613,  -613,  -613,  -613,  -613,    62,  -613,  -613,
    -326,  -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,
    -613,    17,  -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,  -613,  -613,  -613,  -532,  -613,  -613,
    -613,  -613,  -613,   -68,  -613,  -613,  -613,  -613,  -613,  -613,
    -613,  -613,   -93,  -613,  -613,  -505,  -613,  -549,  -613,  -613,
    -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,
    -613,  -613,    18,    21,  -613,  -613,  -613,  -613,  -613,  -613,
    -613,  -613,  -613,   393,  -613,  -613,   258,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,  -330,   342,  -319,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,   685,  -613,
    -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613,   390,  -613,
    -613,   -67,  -613,  -613,  -613,  -613,  -613,  -613,  -613,   120,
    -613,   419,  -613,  -613,   209,  -613,  -613,  -613,  -613,  -613,
    -613,  -613,  -613,  -613,  -613,  -613,  -613,  -613
};

  /* YYDEFGOTONTERM-NUM.  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     3,    27,    28,   172,   176,   180,   181,
     171,   179,   121,   116,   125,   182,   184,   186,   190,   191,
      82,    29,    84,    30,   115,   298,   449,    31,    32,   117,
     302,   451,   657,   735,   714,   736,   715,   716,   752,   833,
      33,   118,   391,    34,    35,   124,   333,   470,    36,    85,
      37,   142,   332,    38,    39,    40,   126,   334,   484,    41,
     217,   362,   551,    42,   258,    43,   102,   248,   340,    44,
      45,   396,   485,   586,   486,   487,   394,   395,   471,   569,
     580,   581,   567,   571,   570,   572,   565,   392,   466,   659,
     318,   222,   293,   109,   354,    46,   472,    83,   284,   428,
     637,   197,   319,   336,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   337,    48,   297,   370,   444,   556,   445,
     446,   656,   473,    50,   296,   344,   404,   498,   499,   595,
     500,   474,    86,   208,   285,   209,   145,   146,   147,   148,
      52,   355,   430,   641,   356,   727,   748,   785,   357,   358,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
      53,    87,    54,   177,   345,   504,   406,   505,   599,   503,
     597,   721,   596,    55,    88,    56,   268,   408,   676,   741,
     777,   824,   606,   802,   825,   803,   826,   867,   821,   804,
     827,   805,   823,   822,   856,   858,   866,    57,    58,    59,
      89,   286,   431,   643,   548,   644,   731,   549,   164,   341,
     493,   165,   257,   398,   166,   167,   342,   494,   168,   169,
     343,   495,   170,   359,   429,   639,   697,   640,   696,   749,
     475,   420,   529,   693,   746,   782,   421,   530,   694,   747,
     784,   476,    90,   287,   432,   645,   477,   666,   738,   775,
     820,   478,   578,   490,   582,   719,   757,   724,   742,   743,
     761,   780,   829,   762,   778,   828,   756,   773,   774,   795,
     818,   853,   796,   819,   854,   579,   797,   764,   781,   830,
     768,   783,   831,   812,   832,   861,   838,   855,   869,   874,
     875,   878,   479,   480,    63,    64,    65,   183,   347,   512,
      66,   220,   364,   290,   363,   409,   513,   614,   615,   616,
     617,   618,   612,   619,   514,   533,   515,   424,   535,   516,
     517,   518,    67,   185,    68,   105,   291,   437,   647,   732,
     771,   366,   436,   787,   276,   348,   523,   410,   524,   623,
     624,   525,   687,   744,   526,   688,   745,    69,    70,    71,
      72,    73,   279,   411,   625,    74,    75,    76,   188,   278,
      77,   280,   412,   626,    78,   241,   242,   303,   243,   789,
     816,   790,    79,   111,   294,   438,   648,   554,   555,   653,
     705,   519,   245,   390,   330,    80,    81,   112,   369,   193,
     283,   426,   352,   427,   539,   540,   538,   542
};

  /* YYTABLEYYPACT[STATE-NUM] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     108,   110,   163,   159,    92,   160,   161,    93,    47,   103,
     104,   162,   144,   204,   205,   399,   113,    49,    51,    60,
      61,   143,   422,    62,   198,   628,   419,   320,   201,   698,
     227,   292,   349,   423,   202,   203,   244,   199,   575,   400,
     401,   402,   299,   488,   200,   163,   159,   433,   160,   161,
     223,     8,     4,   106,   162,    47,   224,   522,   295,   799,
      91,   196,   206,   642,   143,    51,    60,    61,  -144,  -145,
      62,   119,   712,   122,   122,   207,   225,   210,   211,   212,
     213,   713,   224,   511,     8,   215,   800,   801,   106,   218,
     575,   630,   219,   631,    18,   221,   527,   120,    95,   106,
     379,   106,   367,   114,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   195,   137,   138,   106,   531,   435,   174,
     568,   140,   141,   304,   305,   306,   307,   308,   309,   310,
     311,   107,  -333,   106,   312,   313,   488,   174,   600,   128,
     129,   178,   174,   132,   133,   134,   135,   174,  -315,   314,
     315,   712,   265,    12,   316,   317,   725,   259,  -363,   260,
     713,   204,   205,   123,   192,  -146,   107,   413,   414,   224,
     122,   224,   198,   174,   106,   488,   201,   107,  -363,   107,
     491,   443,   202,   203,   511,   199,   497,   739,   726,   413,
     414,   553,   200,   638,   224,  -100,   765,   415,   254,   255,
     122,   187,   416,   799,   122,   187,    18,   106,   776,   196,
      99,   107,   496,   383,   384,   252,   846,   174,   253,   415,
     194,   214,  -455,   216,   416,   651,   798,   417,   418,   650,
     800,   801,  -528,   809,   385,   386,   652,   387,   388,   389,
     228,   -47,   863,   -47,   304,   305,   306,   307,   308,   309,
     310,   311,   107,   575,   870,   312,   313,   -47,   -47,   226,
     -47,   -47,   229,   405,   -47,   792,   793,   794,   800,   801,
     314,   315,   230,   806,   353,   316,   317,   304,   305,   306,
     307,   308,   309,   310,   311,   107,   -47,   246,   312,   313,
     -47,   461,   462,   463,    94,    96,   376,   377,   378,   247,
     204,   205,    98,   101,   -47,   457,   458,   249,   250,   317,
     393,   198,   459,   460,   836,   201,   492,   393,   251,   651,
     224,   202,   203,   650,   199,   256,   506,   261,   289,   448,
     652,   200,   262,   263,   264,   231,  -212,   232,   575,   267,
     266,   434,   629,   269,   106,   270,   320,   545,   196,   547,
     635,   233,   234,   271,   235,   236,   272,   274,   237,   273,
     275,   277,   281,   282,   163,   159,  -529,   160,   161,   288,
     301,   335,   300,   162,   502,   671,   442,    47,   338,   575,
     238,   465,   563,   143,   239,   331,    49,    51,    60,    61,
     339,   350,    62,   532,   729,   536,   537,   346,   240,   351,
     360,   361,   532,   672,   368,   373,   365,   382,   576,   577,
     374,   489,   380,   375,   552,   397,   -44,   677,   678,   679,
     320,   381,   403,   405,   510,   450,   453,   534,   464,   543,
     544,   546,   557,   559,   561,   558,   560,   562,   709,   566,
     583,   564,   593,   587,   589,   163,   159,   602,   160,   161,
     590,   603,   591,   594,   162,   502,   574,   604,   605,   592,
     576,   577,   598,  -291,   143,   607,   106,   613,   405,   622,
     143,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     195,   137,   138,   627,   601,   608,   609,   610,   140,   141,
    -386,   413,   414,   632,   633,   634,   636,   573,   655,   646,
    -567,   660,   661,   662,   663,   664,   848,   849,   574,   665,
     620,   621,   507,   508,   667,   163,   159,   668,   160,   161,
     669,   415,   509,   670,   162,   674,   416,   252,   680,   417,
     418,   681,   682,   683,   143,   684,   685,   868,   686,   689,
     106,   690,   706,   489,   107,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    10,    11,   700,
      12,   139,   140,   141,   691,   467,   692,  -395,     6,   699,
     649,     9,  -395,  -395,  -395,  -395,  -395,  -395,  -395,  -395,
    -395,  -395,  -395,  -395,    10,    11,   701,    12,   702,  -395,
    -395,    13,   703,   707,   413,   414,   468,   469,  -395,   708,
     710,   718,   638,   722,    14,   840,   723,  -404,   481,   482,
     483,   733,   737,   740,   755,   760,   753,   758,   107,   759,
     763,   767,   770,   576,   577,  -101,   769,    22,    23,   772,
     122,   413,   414,  -413,   786,   779,   788,   717,   807,   791,
     808,   810,   815,   811,   837,  -395,   817,   834,   835,   547,
     839,  -213,   507,   508,   -86,   842,   841,   850,   871,   873,
     864,   415,   509,    47,   859,   860,   416,   865,   879,   417,
     418,   574,   877,    51,    60,    61,   371,   872,    62,   876,
     372,   173,   754,   100,    92,    97,   585,   728,   528,   734,
     695,  -458,   106,     6,   454,   175,   440,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   195,   137,   576,   577,
     751,   106,    12,   455,   140,   141,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,    10,    11,
     654,    12,   139,   140,   141,   456,    92,   720,   675,   728,
     673,   766,    47,   407,   844,   588,   447,   584,   843,   576,
     577,   845,    51,    60,    61,   550,   574,    62,   750,   730,
     862,   847,   439,   189,   814,   452,   541,   852,   611,   704,
     107,   425,   658,     0,     0,    92,     0,     0,   813,   441,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   107,
       0,     0,     0,     0,     0,   174,     0,   574,     0,     0,
       0,     0,     0,   163,   159,     0,   160,   161,   393,   393,
       0,     0,   162,   857,     0,     0,     0,     0,   467,     0,
    -395,     6,   143,   851,     9,  -395,  -395,  -395,  -395,  -395,
    -395,  -395,  -395,  -395,  -395,  -395,  -395,    10,    11,   393,
      12,     0,  -395,  -395,    13,     0,     0,   413,   414,   468,
     469,  -395,     0,     0,     0,     0,     0,    14,     0,     0,
     106,   481,   482,   483,     0,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   195,     0,     0,   501,     0,   106,
      22,    23,   140,   141,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,    10,    11,  -395,    12,
     139,   140,   141,   467,  -213,  -395,     6,  -541,     0,     9,
    -395,  -395,  -395,  -395,  -395,  -395,  -395,  -395,  -395,  -395,
    -395,  -395,    10,    11,     0,    12,     0,  -395,  -395,    13,
       0,     0,   413,   414,   468,   469,  -395,     0,   107,     0,
       0,     0,    14,     0,     0,     0,   520,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   521,   107,     0,     0,
       0,     0,     0,   174,     0,    22,    23,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -395,     0,     0,     0,   467,     0,  -395,
       6,     0,  -508,     9,  -395,  -395,  -395,  -395,  -395,  -395,
    -395,  -395,  -395,  -395,  -395,  -395,    10,    11,     0,    12,
       0,  -395,  -395,    13,     0,     0,   413,   414,   468,   469,
    -395,     0,   467,     0,  -395,     6,    14,     0,     9,  -395,
    -395,  -395,  -395,  -395,  -395,  -395,  -395,  -395,  -395,  -395,
    -395,    10,    11,     0,    12,     0,  -395,  -395,    13,    22,
      23,   413,   414,   468,   469,  -395,     0,     0,     0,     0,
       0,    14,     0,     0,     0,     0,     0,  -395,     0,     0,
       0,     0,     0,  -213,     0,     0,   -68,     0,     0,     0,
       0,     0,   106,     0,    22,    23,     0,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,     0,
       0,     0,  -395,   139,   140,   141,     0,   467,  -213,  -395,
       6,   -90,     0,     9,  -395,  -395,  -395,  -395,  -395,  -395,
    -395,  -395,  -395,  -395,  -395,  -395,    10,    11,     0,    12,
       0,  -395,  -395,    13,     0,     0,   413,   414,   468,   469,
    -395,     0,     0,     0,     0,     0,    14,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     0,
     107,     6,     7,     8,     9,     0,   174,     0,     0,    22,
      23,     0,     0,     0,     0,     0,     0,    10,    11,     0,
      12,     0,     0,     0,    13,     0,     0,  -395,     0,     0,
       0,     0,     0,  -213,     0,     0,  -532,    14,    15,    16,
      17,     0,     0,     0,     0,     0,    18,    19,     0,     0,
      20,    -2,     5,    21,     0,     6,     7,     8,     9,     0,
      22,    23,     0,     0,     0,     0,     0,    24,    25,   711,
       0,    10,    11,     0,    12,     0,     0,     0,    13,     0,
       0,     0,     0,    26,     0,     0,     0,   -53,     0,     0,
       0,    14,    15,    16,    17,     0,     0,     0,     0,     0,
      18,    19,     0,     0,    20,     5,     0,    21,     6,     7,
       8,     9,     0,     0,    22,    23,     0,     0,     0,     0,
       0,    24,    25,     0,    10,    11,     0,    12,     0,     0,
       5,    13,     0,     6,     7,     8,     9,    26,  -213,     0,
       0,     0,     0,     0,    14,    15,    16,    17,     0,    10,
      11,     0,    12,    18,    19,     0,    13,    20,     0,     0,
      21,     0,     0,     0,     0,     0,     0,    22,    23,    14,
      15,    16,    17,     0,    24,    25,     0,     0,    18,    19,
       0,     0,    20,     5,     0,    21,     6,     7,     8,     9,
      26,  -213,    22,    23,     0,     0,     0,     0,     0,    24,
      25,   711,    10,    11,     0,    12,     0,     0,     0,    13,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,     0,    14,    15,    16,    17,     0,     0,     0,     0,
       0,    18,    19,     0,     0,    20,     0,     0,    21,     0,
       0,     0,     0,     0,     0,    22,    23,     0,     0,     0,
       0,     0,    24,    25,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   467,     0,  -395,     6,     0,    26,     9,
    -395,  -395,  -395,  -395,  -395,  -395,  -395,  -395,  -395,  -395,
    -395,  -395,    10,    11,     0,    12,     0,  -395,  -395,    13,
       0,     0,   413,   414,   468,   469,  -395,     0,     0,     0,
       0,     0,    14,     0,     0,     0,   481,   482,   483,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,    23,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   467,     0,
    -395,     6,     0,  -395,     9,  -395,  -395,  -395,  -395,  -395,
    -395,  -395,  -395,  -395,  -395,  -395,  -395,    10,    11,     0,
      12,     0,  -395,  -395,    13,     0,     0,   413,   414,   468,
     469,  -395,   501,     0,   106,     0,     0,    14,     0,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,    10,    11,     0,    12,   139,   140,   141,     0,   106,
      22,    23,     0,     0,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,    10,    11,  -395,    12,
     139,   140,   141,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   107,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   107
};

static const yytype_int16 yycheck[] =
{
      22,    23,    43,    43,    14,    43,    43,    14,     2,    19,
      20,    43,    43,    83,    83,   339,    26,     2,     2,     2,
       2,    43,   352,     2,    83,   530,   352,   244,    83,   641,
     112,   222,   278,   352,    83,    83,   118,    83,   478,   341,
     342,   343,   231,   396,    83,    86,    86,   362,    86,    86,
      75,     6,     0,     3,    86,    49,    81,   410,     3,     1,
       3,    83,    84,     3,    86,    49,    49,    49,    88,    88,
      49,    88,   657,    93,    93,    85,    75,    87,    88,    89,
      90,   657,    81,   409,     6,    95,    28,    29,     3,    99,
     530,     3,   102,     3,    49,   105,   411,    88,    53,     3,
     317,     3,   293,    91,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,     3,    19,   364,    87,
     473,    25,    26,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    88,     3,    84,    85,   489,    87,     8,     9,
      10,    22,    87,    13,    14,    15,    16,    87,    90,    99,
     100,   736,   174,    23,   104,   105,   688,    92,    87,    94,
     736,   231,   231,    36,    89,    88,    81,    30,    31,    81,
      93,    81,   231,    87,     3,   528,   231,    81,   107,    81,
     397,   370,   231,   231,   510,   231,   403,   719,   693,    30,
      31,   437,   231,   107,    81,    88,   745,    60,     8,     9,
      93,    74,    65,     1,    93,    78,    49,     3,   757,   231,
      53,    81,   403,    82,    83,     8,   828,    87,    11,    60,
      88,    94,    88,    96,    65,   555,   775,    68,    69,   555,
      28,    29,    88,   782,    99,   100,   555,   101,   102,   103,
      89,     4,   854,     6,    73,    74,    75,    76,    77,    78,
      79,    80,    81,   693,   866,    84,    85,    20,    21,     3,
      23,    24,    89,   345,    27,    35,    36,    37,    28,    29,
      99,   100,    88,   778,   284,   104,   105,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    49,    88,    84,    85,
      53,   387,   388,   389,    15,    16,   314,   315,   316,    48,
     370,   370,    16,    17,    67,   383,   384,    89,    88,   105,
     332,   370,   385,   386,   819,   370,   398,   339,    89,   649,
      81,   370,   370,   649,   370,    91,   408,    91,    93,   370,
     649,   370,    91,    88,    88,     4,    87,     6,   778,    89,
      88,   363,   533,    88,     3,    88,   563,   429,   370,   431,
     541,    20,    21,    88,    23,    24,    88,    88,    27,    89,
      89,    88,    88,    88,   405,   405,    88,   405,   405,    89,
       3,     8,    91,   405,   405,   592,   370,   371,    91,   819,
      49,   391,   464,   405,    53,    92,   371,   371,   371,   371,
      47,    89,   371,   415,   696,   417,   418,   105,    67,    89,
      89,    89,   424,   594,    89,     3,    93,    98,   478,   478,
      92,   396,    96,    94,   436,    94,    90,   608,   609,   610,
     637,    97,   105,   505,   409,    92,   106,    58,    94,    90,
      95,    94,    88,    88,     3,   445,    90,    89,   655,    90,
      90,    94,   106,    92,    92,   486,   486,   506,   486,   486,
      92,   506,    92,    94,   486,   486,   478,   506,   506,    95,
     530,   530,    90,    90,   486,   506,     3,    90,   550,    90,
     492,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    31,   506,   507,   508,   509,    25,    26,
      90,    30,    31,    88,    88,    88,    88,    34,    29,    90,
      90,    88,    88,    88,    88,    88,   830,   831,   530,    88,
     520,   521,    51,    52,    88,   556,   556,    88,   556,   556,
       3,    60,    61,    88,   556,    88,    65,     8,    88,    68,
      69,    88,    88,    88,   556,    88,    88,   861,    88,    88,
       3,    88,    88,   528,    81,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    94,
      23,    24,    25,    26,    90,     1,    90,     3,     4,    90,
     555,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    90,    23,    66,    25,
      26,    27,    90,    88,    30,    31,    32,    33,    34,    88,
      88,     3,   107,   106,    40,   822,   105,    35,    44,    45,
      46,    88,    88,    88,   105,    35,    90,    89,    81,   106,
      38,    55,   108,   693,   693,    88,    94,    63,    64,    91,
      93,    30,    31,   106,    59,   106,     3,   659,    94,   106,
     105,   105,    92,    62,    39,    81,   106,    94,    94,   731,
      88,    87,    51,    52,    90,    90,    93,   105,    88,    75,
     105,    60,    61,   657,   106,   106,    65,    93,    75,    68,
      69,   693,    94,   657,   657,   657,   298,   106,   657,   106,
     298,    49,   736,    17,   694,    16,   489,   694,   412,   711,
     637,    90,     3,     4,   380,    50,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,   778,   778,
     732,     3,    23,   381,    25,    26,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
     556,    23,    24,    25,    26,   382,   746,   673,   597,   746,
     594,   746,   736,   345,   826,   492,   370,   486,   825,   819,
     819,   827,   736,   736,   736,   432,   778,   736,   731,   697,
     853,   829,   369,    78,   786,   375,   424,   834,   510,   649,
      81,   352,   563,    -1,    -1,   785,    -1,    -1,   785,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    87,    -1,   819,    -1,    -1,
      -1,    -1,    -1,   844,   844,    -1,   844,   844,   830,   831,
      -1,    -1,   844,   844,    -1,    -1,    -1,    -1,     1,    -1,
       3,     4,   844,   833,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,   861,
      23,    -1,    25,    26,    27,    -1,    -1,    30,    31,    32,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
       3,    44,    45,    46,    -1,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    -1,    -1,     1,    -1,     3,
      63,    64,    25,    26,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    81,    23,
      24,    25,    26,     1,    87,     3,     4,    90,    -1,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    23,    -1,    25,    26,    27,
      -1,    -1,    30,    31,    32,    33,    34,    -1,    81,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    81,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    63,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    -1,    -1,    -1,     1,    -1,     3,
       4,    -1,    90,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      -1,    25,    26,    27,    -1,    -1,    30,    31,    32,    33,
      34,    -1,     1,    -1,     3,     4,    40,    -1,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    -1,    25,    26,    27,    63,
      64,    30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,     3,    -1,    63,    64,    -1,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    81,    24,    25,    26,    -1,     1,    87,     3,
       4,    90,    -1,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      -1,    25,    26,    27,    -1,    -1,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
      81,     4,     5,     6,     7,    -1,    87,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      23,    -1,    -1,    -1,    27,    -1,    -1,    81,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    90,    40,    41,    42,
      43,    -1,    -1,    -1,    -1,    -1,    49,    50,    -1,    -1,
      53,     0,     1,    56,    -1,     4,     5,     6,     7,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,
      -1,    20,    21,    -1,    23,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    40,    41,    42,    43,    -1,    -1,    -1,    -1,    -1,
      49,    50,    -1,    -1,    53,     1,    -1,    56,     4,     5,
       6,     7,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    20,    21,    -1,    23,    -1,    -1,
       1,    27,    -1,     4,     5,     6,     7,    86,    87,    -1,
      -1,    -1,    -1,    -1,    40,    41,    42,    43,    -1,    20,
      21,    -1,    23,    49,    50,    -1,    27,    53,    -1,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    40,
      41,    42,    43,    -1,    70,    71,    -1,    -1,    49,    50,
      -1,    -1,    53,     1,    -1,    56,     4,     5,     6,     7,
      86,    87,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,
      71,    72,    20,    21,    -1,    23,    -1,    -1,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    40,    41,    42,    43,    -1,    -1,    -1,    -1,
      -1,    49,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,     4,    -1,    86,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    23,    -1,    25,    26,    27,
      -1,    -1,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    44,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,     4,    -1,    81,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      23,    -1,    25,    26,    27,    -1,    -1,    30,    31,    32,
      33,    34,     1,    -1,     3,    -1,    -1,    40,    -1,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    24,    25,    26,    -1,     3,
      63,    64,    -1,    -1,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    81,    23,
      24,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81
};

  /* YYSTOSSTATE-NUM -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   110,   111,   112,     0,     1,     4,     5,     6,     7,
      20,    21,    23,    27,    40,    41,    42,    43,    49,    50,
      53,    56,    63,    64,    70,    71,    86,   113,   114,   130,
     132,   136,   137,   149,   152,   153,   157,   159,   162,   163,
     164,   168,   172,   174,   178,   179,   204,   205,   223,   231,
     232,   240,   249,   269,   271,   282,   284,   306,   307,   308,
     350,   401,   402,   403,   404,   405,   409,   431,   433,   456,
     457,   458,   459,   460,   464,   465,   466,   469,   473,   481,
     494,   495,   129,   206,   131,   158,   241,   270,   283,   309,
     351,     3,   203,   257,   157,    53,   157,   172,   174,    53,
     164,   174,   175,   203,   203,   434,     3,    81,   199,   202,
     199,   482,   496,   203,    91,   133,   122,   138,   150,    88,
      88,   121,    93,   160,   154,   123,   165,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    24,
      25,    26,   160,   199,   244,   245,   246,   247,   248,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     271,   282,   284,   308,   317,   320,   323,   324,   327,   328,
     331,   119,   115,   113,    87,   233,   116,   272,    22,   120,
     117,   118,   124,   406,   125,   432,   126,   160,   467,   467,
     127,   128,    89,   498,    88,    17,   199,   210,   259,   262,
     263,   264,   265,   266,   324,   328,   199,   203,   242,   244,
     203,   203,   203,   203,   160,   203,   160,   169,   203,   203,
     410,   203,   200,    75,    81,    75,     3,   232,    89,    89,
      88,     4,     6,    20,    21,    23,    24,    27,    49,    53,
      67,   474,   475,   477,   232,   491,    88,    48,   176,    89,
      88,    89,     8,    11,     8,     9,    91,   321,   173,    92,
      94,    91,    91,    88,    88,   199,    88,    89,   285,    88,
      88,    88,    88,    89,    88,    89,   443,    88,   468,   461,
     470,    88,    88,   499,   207,   243,   310,   352,    89,    93,
     412,   435,   202,   201,   483,     3,   233,   224,   134,   210,
      91,     3,   139,   476,    73,    74,    75,    76,    77,    78,
      79,    80,    84,    85,    99,   100,   104,   105,   199,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     493,    92,   161,   155,   166,     8,   212,   222,    91,    47,
     177,   318,   325,   329,   234,   273,   105,   407,   444,   177,
      89,    89,   501,   203,   203,   250,   253,   257,   258,   332,
      89,    89,   170,   413,   411,    93,   440,   202,    89,   497,
     225,   111,   112,     3,    92,    94,   220,   220,   220,   212,
      96,    97,    98,    82,    83,    99,   100,   101,   102,   103,
     492,   151,   196,   199,   185,   186,   180,    94,   322,   196,
     222,   222,   222,   105,   235,   232,   275,   277,   286,   414,
     446,   462,   471,    30,    31,    60,    65,    68,    69,   339,
     340,   345,   423,   425,   426,   490,   500,   502,   208,   333,
     251,   311,   353,   185,   199,   177,   441,   436,   484,   412,
       7,    90,   205,   210,   226,   228,   229,   267,   308,   135,
      92,   140,   477,   106,   214,   215,   216,   217,   217,   218,
     218,   219,   219,   219,    94,   203,   197,     1,    32,    33,
     156,   187,   205,   231,   240,   339,   350,   355,   360,   401,
     402,    44,    45,    46,   167,   181,   183,   184,   187,   231,
     362,   212,   232,   319,   326,   330,   202,   212,   236,   237,
     239,     1,   244,   278,   274,   276,   232,    51,    52,    61,
     231,   339,   408,   415,   423,   425,   428,   429,   430,   490,
      44,    54,   187,   445,   447,   450,   453,   185,   180,   341,
     346,    19,   199,   424,    58,   427,   199,   199,   505,   503,
     504,   424,   506,    90,    95,   232,    94,   232,   313,   316,
     276,   171,   199,   177,   486,   487,   227,    88,   203,    88,
      90,     3,    89,   232,    94,   195,    90,   191,   187,   188,
     193,   192,   194,    34,   199,   246,   324,   328,   361,   384,
     189,   190,   363,    90,   278,   181,   182,    92,   245,    92,
      92,    92,    95,   106,    94,   238,   281,   279,    90,   277,
       8,   199,   259,   264,   265,   266,   291,   308,   199,   199,
     199,   415,   421,    90,   416,   417,   418,   419,   420,   422,
     203,   203,    90,   448,   449,   463,   472,    31,   384,   202,
       3,     3,    88,    88,    88,   202,    88,   209,   107,   334,
     336,   252,     3,   312,   314,   354,    90,   437,   485,   231,
     339,   423,   425,   488,   242,    29,   230,   141,   493,   198,
      88,    88,    88,    88,    88,    88,   356,    88,    88,     3,
      88,   212,   202,   239,    88,   250,   287,   202,   202,   202,
      88,    88,    88,    88,    88,    88,    88,   451,   454,    88,
      88,    90,    90,   342,   347,   211,   337,   335,   253,    90,
      94,    90,    66,    90,   488,   489,    88,    88,    88,   212,
      88,    72,   114,   130,   143,   145,   146,   199,     3,   364,
     238,   280,   106,   105,   366,   366,   384,   254,   257,   222,
     336,   315,   438,    88,   199,   142,   144,    88,   357,   366,
      88,   288,   367,   368,   452,   455,   343,   348,   255,   338,
     316,   199,   147,    90,   145,   105,   375,   365,    89,   106,
      35,   369,   372,    38,   386,   386,   254,    55,   389,    94,
     108,   439,    91,   376,   377,   358,   386,   289,   373,   106,
     370,   387,   344,   390,   349,   256,    59,   442,     3,   478,
     480,   106,    35,    36,    37,   378,   381,   385,   386,     1,
      28,    29,   292,   294,   298,   300,   384,    94,   105,   386,
     105,    62,   392,   257,   199,    92,   479,   106,   379,   382,
     359,   297,   302,   301,   290,   293,   295,   299,   374,   371,
     388,   391,   393,   148,    94,    94,   384,    39,   395,    88,
     212,    93,    90,   294,   232,   300,   253,   372,   196,   196,
     105,   203,   480,   380,   383,   396,   303,   244,   304,   106,
     106,   394,   381,   253,   105,    93,   305,   296,   196,   397,
     253,    88,   106,    75,   398,   399,   106,    94,   400,    75
};

  /* YYR1YYN -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   109,   110,   111,   111,   112,   112,   113,   113,   113,
     115,   114,   116,   114,   117,   114,   118,   114,   119,   114,
     120,   114,   121,   114,   122,   114,   123,   114,   124,   114,
     125,   114,   126,   114,   127,   114,   128,   114,   129,   114,
     131,   130,   133,   134,   135,   132,   136,   138,   139,   140,
     141,   142,   137,   143,   144,   144,   145,   145,   147,   148,
     146,   150,   151,   149,   152,   152,   154,   155,   156,   153,
     158,   157,   159,   159,   159,   161,   160,   160,   162,   162,
     162,   162,   163,   163,   165,   166,   167,   164,   169,   170,
     171,   168,   173,   172,   175,   174,   176,   176,   177,   177,
     178,   178,   179,   180,   180,   180,   181,   181,   182,   181,
     183,   183,   184,   185,   185,   186,   186,   188,   187,   189,
     187,   190,   187,   191,   187,   192,   187,   193,   187,   194,
     187,   195,   187,   196,   198,   197,   197,   199,   200,   199,
     201,   199,   202,   203,   204,   204,   204,   206,   207,   208,
     209,   205,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   211,   212,   213,   213,   214,   214,   215,   215,   216,
     216,   216,   217,   217,   217,   218,   218,   218,   218,   219,
     219,   219,   219,   220,   220,   220,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   222,   224,   223,   225,
     225,   226,   226,   226,   227,   226,   228,   228,   229,   230,
     230,   231,   232,   232,   234,   233,   235,   235,   236,   236,
     237,   238,   238,   239,   241,   240,   240,   240,   240,   240,
     240,   243,   242,   244,   244,   245,   245,   245,   246,   246,
     246,   246,   246,   246,   246,   246,   247,   247,   247,   247,
     248,   248,   248,   249,   249,   250,   252,   251,   251,   253,
     253,   254,   256,   255,   255,   257,   258,   259,   259,   260,
     260,   260,   261,   261,   261,   262,   262,   262,   263,   264,
     264,   265,   266,   267,   268,   270,   269,   272,   273,   274,
     271,   275,   276,   276,   277,   279,   280,   278,   281,   278,
     283,   282,   285,   286,   287,   288,   289,   290,   284,   291,
     291,   291,   291,   291,   291,   292,   293,   293,   295,   296,
     294,   297,   294,   298,   299,   299,   301,   300,   302,   303,
     300,   305,   304,   306,   307,   309,   310,   311,   312,   308,
     313,   315,   314,   314,   316,   318,   319,   317,   317,   321,
     322,   320,   323,   325,   326,   324,   324,   327,   329,   330,
     328,   328,   331,   333,   332,   334,   335,   335,   337,   338,
     336,   339,   339,   341,   342,   343,   344,   340,   346,   347,
     348,   349,   345,   351,   352,   353,   354,   350,   356,   357,
     358,   359,   355,   360,   360,   360,   361,   361,   363,   364,
     365,   362,   367,   366,   368,   366,   369,   371,   370,   370,
     373,   374,   372,   376,   375,   377,   375,   378,   380,   379,
     379,   382,   383,   381,   384,   384,   384,   384,   385,   385,
     385,   387,   388,   386,   386,   390,   391,   389,   389,   393,
     394,   392,   392,   396,   397,   395,   395,   398,   400,   399,
     399,   401,   402,   403,   403,   404,   406,   407,   408,   405,
     410,   411,   409,   413,   412,   412,   414,   414,   414,   416,
     415,   417,   415,   418,   415,   419,   415,   420,   415,   421,
     415,   422,   415,   423,   424,   424,   425,   426,   427,   427,
     428,   429,   430,   432,   431,   434,   435,   436,   437,   438,
     439,   433,   441,   440,   440,   442,   442,   444,   445,   443,
     446,   446,   447,   448,   447,   449,   447,   451,   452,   450,
     454,   455,   453,   456,   456,   456,   457,   457,   458,   459,
     461,   462,   463,   460,   464,   465,   466,   468,   467,   470,
     471,   472,   469,   473,   473,   474,   474,   474,   474,   474,
     474,   474,   474,   474,   474,   475,   476,   476,   477,   477,
     478,   479,   479,   480,   482,   483,   484,   485,   481,   486,
     486,   487,   487,   488,   488,   489,   488,   490,   490,   491,
     492,   492,   493,   494,   496,   497,   495,   499,   500,   498,
     501,   501,   503,   502,   504,   502,   505,   502,   506,   502
};

  /* YYR2YYN -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     0,     3,     2,     1,     2,     2,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     0,     0,     7,     2,     0,     0,     0,
       0,     0,    11,     2,     2,     0,     1,     2,     0,     0,
       8,     0,     0,     6,     1,     1,     0,     0,     0,     7,
       0,     3,     2,     3,     3,     0,     4,     0,     1,     1,
       1,     1,     2,     1,     0,     0,     0,     7,     0,     0,
       0,     8,     0,     4,     0,     3,     1,     0,     2,     0,
       2,     1,     2,     3,     2,     0,     1,     1,     0,     3,
       1,     1,     2,     1,     0,     3,     2,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     2,     0,     4,     0,     1,     0,     3,
       0,     4,     1,     1,     1,     2,     2,     0,     0,     0,
       0,     9,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     3,     1,     3,     1,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     1,
       2,     2,     2,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     6,     2,
       0,     1,     2,     2,     0,     4,     1,     1,     4,     2,
       0,     2,     2,     0,     0,     4,     3,     0,     1,     1,
       2,     3,     0,     3,     0,     3,     1,     1,     1,     2,
       1,     0,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     0,     4,     0,     1,
       1,     2,     0,     4,     0,     1,     1,     1,     1,     1,
       2,     1,     2,     3,     2,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     0,     3,     0,     0,     0,
       7,     2,     2,     0,     2,     0,     0,     5,     0,     3,
       0,     3,     0,     0,     0,     0,     0,     0,    15,     1,
       1,     1,     1,     1,     1,     2,     2,     0,     0,     0,
       6,     0,     3,     2,     2,     0,     0,     3,     0,     0,
       5,     0,     3,     1,     1,     0,     0,     0,     0,     9,
       2,     0,     4,     0,     2,     0,     0,     6,     2,     0,
       0,     6,     6,     0,     0,     6,     1,     1,     0,     0,
       6,     1,     1,     0,     4,     2,     2,     0,     0,     0,
       5,     1,     1,     0,     0,     0,     0,     9,     0,     0,
       0,     0,     9,     0,     0,     0,     0,     9,     0,     0,
       0,     0,    10,     1,     1,     0,     1,     1,     0,     0,
       0,     7,     0,     3,     0,     4,     2,     0,     4,     0,
       0,     0,     5,     0,     3,     0,     4,     2,     0,     4,
       0,     0,     0,     5,     1,     1,     1,     1,     1,     1,
       1,     0,     0,     6,     0,     0,     0,     6,     0,     0,
       0,     6,     0,     0,     0,     6,     0,     2,     0,     4,
       0,     3,     3,     1,     1,     2,     0,     0,     0,     7,
       0,     0,     6,     0,     3,     0,     3,     2,     0,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     3,     1,     1,     3,     2,     1,     0,
       3,     3,     3,     0,     3,     0,     0,     0,     0,     0,
       0,    13,     0,     3,     0,     2,     0,     0,     0,     5,
       2,     0,     1,     0,     3,     0,     3,     0,     0,     6,
       0,     0,     6,     1,     1,     1,     1,     1,     2,     3,
       0,     0,     0,     8,     3,     3,     2,     0,     3,     0,
       0,     0,     8,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     3,     0,     2,     5,
       2,     3,     0,     1,     0,     0,     0,     0,     9,     3,
       2,     1,     0,     2,     2,     0,     3,     3,     3,     3,
       4,     0,     1,     2,     0,     0,     6,     0,     0,     5,
       2,     0,     0,     3,     0,     3,     0,     3,     0,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize;

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yynerrs = 0;
  yystate = 0;
  yyerrstatus = 0;

  yystacksize = YYINITDEPTH;
  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;


  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 5:
#line 417 "fe/idl.ypp"
        {
          AST_Annotation_Appls *&annotations = (yyvsp[-1].annotations_val);
          AST_Decl *&node = (yyvsp[0].dcval);
          if (node)
            {
              node->annotation_appls (*annotations);
            }
          else
            {
              idl_global->err ()-> unsupported_warning (
                "Annotating this is not supported");
            }
          delete annotations;
        }
#line 2802 "fe/idl.tab.cpp"
    break;

  case 10:
#line 442 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AnnotationDeclSeen);
        }
#line 2810 "fe/idl.tab.cpp"
    break;

  case 11:
#line 446 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2818 "fe/idl.tab.cpp"
    break;

  case 12:
#line 450 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 2826 "fe/idl.tab.cpp"
    break;

  case 13:
#line 454 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2834 "fe/idl.tab.cpp"
    break;

  case 14:
#line 458 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 2842 "fe/idl.tab.cpp"
    break;

  case 15:
#line 462 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2850 "fe/idl.tab.cpp"
    break;

  case 16:
#line 466 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 2858 "fe/idl.tab.cpp"
    break;

  case 17:
#line 470 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2866 "fe/idl.tab.cpp"
    break;

  case 18:
#line 474 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 2874 "fe/idl.tab.cpp"
    break;

  case 19:
#line 478 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2882 "fe/idl.tab.cpp"
    break;

  case 20:
#line 482 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 2890 "fe/idl.tab.cpp"
    break;

  case 21:
#line 486 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2898 "fe/idl.tab.cpp"
    break;

  case 22:
#line 490 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceDeclSeen);
        }
#line 2906 "fe/idl.tab.cpp"
    break;

  case 23:
#line 494 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2914 "fe/idl.tab.cpp"
    break;

  case 24:
#line 498 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleDeclSeen);
        }
#line 2922 "fe/idl.tab.cpp"
    break;

  case 25:
#line 502 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2930 "fe/idl.tab.cpp"
    break;

  case 26:
#line 506 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeDeclSeen);
        }
#line 2938 "fe/idl.tab.cpp"
    break;

  case 27:
#line 510 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2946 "fe/idl.tab.cpp"
    break;

  case 28:
#line 514 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentDeclSeen);
        }
#line 2954 "fe/idl.tab.cpp"
    break;

  case 29:
#line 518 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2962 "fe/idl.tab.cpp"
    break;

  case 30:
#line 522 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeDeclSeen);
        }
#line 2970 "fe/idl.tab.cpp"
    break;

  case 31:
#line 526 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2978 "fe/idl.tab.cpp"
    break;

  case 32:
#line 530 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventDeclSeen);
        }
#line 2986 "fe/idl.tab.cpp"
    break;

  case 33:
#line 534 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2994 "fe/idl.tab.cpp"
    break;

  case 34:
#line 538 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeDeclSeen);
        }
#line 3002 "fe/idl.tab.cpp"
    break;

  case 35:
#line 542 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3010 "fe/idl.tab.cpp"
    break;

  case 36:
#line 546 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorDeclSeen);
        }
#line 3018 "fe/idl.tab.cpp"
    break;

  case 37:
#line 550 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3026 "fe/idl.tab.cpp"
    break;

  case 38:
#line 554 "fe/idl.ypp"
        {
          idl_global->err ()->syntax_error (idl_global->parse_state ());
        }
#line 3034 "fe/idl.tab.cpp"
    break;

  case 39:
#line 558 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
          (yyval.dcval) = 0;
        }
#line 3044 "fe/idl.tab.cpp"
    break;

  case 40:
#line 567 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSeen);
        }
#line 3052 "fe/idl.tab.cpp"
    break;

  case 41:
#line 571 "fe/idl.ypp"
        {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 3060 "fe/idl.tab.cpp"
    break;

  case 42:
#line 578 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleIDSeen);

          // The module_header rule is common to template module, fixed
          // module and instantiated template module. In the last
          // case, a fully scoped name is allowed, but here we
          // allow only an identifier (a scoped name of length
          // 1). If not satisfied, we output a parse error with
          // the appropriate message.
          if ((yyvsp[0].idlist)->length () != 1)
            {
              idl_global->err ()->syntax_error (
                IDL_GlobalData::PS_ModuleIDSeen);
            }

          AST_Module *m = 0;
          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          /*
           * Make a new module and add it to the enclosing scope
           */
          if (s != 0)
            {
              m = idl_global->gen ()->create_module (s, (yyvsp[0].idlist));
              (void) s->fe_add_module (m);
            }

          (yyvsp[0].idlist)->destroy ();
          delete (yyvsp[0].idlist);
          (yyvsp[0].idlist) = 0;

          /*
           * Push it on the stack
           */
          idl_global->scopes ().push (m);

          (yyval.dcval) = m;
        }
#line 3103 "fe/idl.tab.cpp"
    break;

  case 43:
#line 617 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSqSeen);
        }
#line 3111 "fe/idl.tab.cpp"
    break;

  case 44:
#line 621 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleBodySeen);
        }
#line 3119 "fe/idl.tab.cpp"
    break;

  case 45:
#line 625 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleQsSeen);
          /*
           * Finished with this module - pop it from the scope stack.
           */

          idl_global->scopes ().pop ();
          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 3133 "fe/idl.tab.cpp"
    break;

  case 46:
#line 638 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleIDSeen);
        }
#line 3141 "fe/idl.tab.cpp"
    break;

  case 47:
#line 645 "fe/idl.ypp"
        {
          // The module_header rule is common to template module, fixed
          // module and instantiated template module. In the last
          // case, a fully scoped name is allowed, but here we
          // allow only an identifier (a scoped name of length
          // 1). If not satisfied, we output a syntax error with
          // the appropriate message.
          if ((yyvsp[0].idlist)->length () != 1)
            {
              idl_global->err ()->syntax_error (
                IDL_GlobalData::PS_ModuleIDSeen);
            }
        }
#line 3159 "fe/idl.tab.cpp"
    break;

  case 48:
#line 659 "fe/idl.ypp"
        {
          if (FE_Utils::duplicate_param_id ((yyvsp[0].plval)))
            {
              idl_global->err ()->duplicate_param_id (
                (yyvsp[-2].idlist));

              return 1;
            }
        }
#line 3173 "fe/idl.tab.cpp"
    break;

  case 49:
#line 669 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleParamsSeen);

          AST_Template_Module *tm =
            idl_global->gen ()->create_template_module ((yyvsp[-4].idlist),
                                                        (yyvsp[-2].plval));

          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Module *m = s->fe_add_module (tm);

          // We've probably tried to reopen a template module,
          // going further will cause a crash.
          if (m == 0)
            {
              return 1;
            }

          /*
           * Push it on the stack
           */
          idl_global->scopes ().push (tm);

          // Contained items not part of an alias will get flag set.
          idl_global->in_tmpl_mod_no_alias (true);

          // Store these for reference as we parse the scope
          // of the template module.
          idl_global->current_params ((yyvsp[-2].plval));
        }
#line 3207 "fe/idl.tab.cpp"
    break;

  case 50:
#line 699 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleSqSeen);
        }
#line 3215 "fe/idl.tab.cpp"
    break;

  case 51:
#line 703 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleBodySeen);
        }
#line 3223 "fe/idl.tab.cpp"
    break;

  case 52:
#line 707 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleQsSeen);

          /*
           * Finished with this module - pop it from the scope stack.
           */
          idl_global->scopes ().pop ();

          // Unset the flag, the no_alias version because any scope
          // traversal triggered by an alias would have ended by now.
          idl_global->in_tmpl_mod_no_alias (false);

          // Clear the pointer so scoped name lookup will know
          // that we are no longer in a template module scope.
          idl_global->current_params (0);

          (yyval.dcval) = 0;
        }
#line 3246 "fe/idl.tab.cpp"
    break;

  case 58:
#line 743 "fe/idl.ypp"
        {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefSeen);
        }
#line 3255 "fe/idl.tab.cpp"
    break;

  case 59:
#line 748 "fe/idl.ypp"
        {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefParamsSeen);
        }
#line 3264 "fe/idl.tab.cpp"
    break;

  case 60:
#line 753 "fe/idl.ypp"
        {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefIDSeen);

          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d =
            s->lookup_by_name ((yyvsp[-6].idlist));

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((yyvsp[-6].idlist));
              return 1;
            }

          AST_Template_Module *ref =
            AST_Template_Module::narrow_from_decl (d);

          if (ref == 0)
            {
              idl_global->err ()->template_module_expected (d);
              return 1;
            }

          bool refs_match =
            ref->match_param_refs ((yyvsp[-3].slval),
                                   s);

          if (! refs_match)
            {
              // Error message is already output.
              return 1;
            }

          UTL_ScopedName sn ((yyvsp[0].idval), 0);

          AST_Template_Module_Ref *tmr =
            idl_global->gen ()->create_template_module_ref (
              &sn,
              ref,
              (yyvsp[-3].slval));

          (void) s->fe_add_template_module_ref (tmr);

          sn.destroy ();
          (yyvsp[-6].idlist)->destroy ();
          delete (yyvsp[-6].idlist);
          (yyvsp[-6].idlist) = 0;

          // Save the current flag value to be restored below.
          bool itmna_flag = idl_global->in_tmpl_mod_no_alias ();
          idl_global->in_tmpl_mod_no_alias (false);
          idl_global->in_tmpl_mod_alias (true);

          ast_visitor_context ctx;
          ctx.template_params (ref->template_params ());
          ast_visitor_tmpl_module_ref v (&ctx);

          // The implied IDL resulting from this reference is
          // created here, in the template module scope. Upon
          // instantiation of the enclosing template module, the
          // visitor copies this implied IDL to the instantiated
          // module scope. The extra copy is less than ideal, but
          // otherwise we have ugly lookup issues when the
          // referenced template module's contents are referenced
          // using the aliased scoped name.
          if (v.visit_template_module_ref (tmr) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("visit_template_module_ref")
                          ACE_TEXT (" failed\n")));

              idl_global->set_err_count (idl_global->err_count () + 1);
            }

          idl_global->in_tmpl_mod_no_alias (itmna_flag);
          idl_global->in_tmpl_mod_alias (false);
        }
#line 3346 "fe/idl.tab.cpp"
    break;

  case 61:
#line 834 "fe/idl.ypp"
        {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleSeen);
        }
#line 3355 "fe/idl.tab.cpp"
    break;

  case 62:
#line 839 "fe/idl.ypp"
        {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleArgsSeen);
        }
#line 3364 "fe/idl.tab.cpp"
    break;

  case 63:
#line 844 "fe/idl.ypp"
        {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleIDSeen);

          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName *sn = (yyvsp[-5].idlist);
          AST_Template_Module *ref = 0;
          AST_Decl *d = s->lookup_by_name (sn);

          if (d == 0)
            {
              idl_global->err ()->lookup_error (sn);
              return 1;
            }
          else
            {
              ref = AST_Template_Module::narrow_from_decl (d);

              if (ref == 0)
                {
                  idl_global->err ()->template_module_expected (d);
                  return 1;
                }
            }

          sn->destroy ();
          delete sn;
          sn = 0;
          (yyvsp[-5].idlist) = 0;

          if (! ref->match_arg_names ((yyvsp[-3].alval)))
            {
              return 1;
            }

          ACE_NEW_RETURN (sn,
                          UTL_ScopedName ((yyvsp[0].idval),
                                           0),
                          1);

          AST_Template_Module_Inst *tmi =
            idl_global->gen ()->create_template_module_inst (
              sn,
              ref,
              (yyvsp[-3].alval));

          (void) s->fe_add_template_module_inst (tmi);

          ast_visitor_context ctx;
          ctx.template_args ((yyvsp[-3].alval));
          ast_visitor_tmpl_module_inst v (&ctx);

          if (v.visit_template_module_inst (tmi) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("visit_template_module_inst")
                          ACE_TEXT (" failed\n")));

              idl_global->set_err_count (idl_global->err_count () + 1);
            }

          (yyval.dcval) = 0;
        }
#line 3432 "fe/idl.tab.cpp"
    break;

  case 66:
#line 916 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Interface *i = 0;

          FE_InterfaceHeader *header = (yyvsp[0].ihval);
          (yyvsp[0].ihval) = 0;

          /*
           * Make a new interface node and add it to its enclosing scope.
           */
          if (s && header)
            {
              i = idl_global->gen ()->create_interface (
                header->name (),
                header->inherits (),
                header->n_inherits (),
                header->inherits_flat (),
                header->n_inherits_flat (),
                header->is_local (),
                header->is_abstract ()
              );
              AST_Interface::fwd_redefinition_helper (i, s);
              /*
               * Add the interface to its definition scope.
               */
              s->fe_add_interface (i);

              // This FE_InterfaceHeader class isn't destroyed with the AST.
              header->destroy ();
              delete header;
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (i);
        }
#line 3474 "fe/idl.tab.cpp"
    break;

  case 67:
#line 954 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
#line 3482 "fe/idl.tab.cpp"
    break;

  case 68:
#line 958 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
#line 3490 "fe/idl.tab.cpp"
    break;

  case 69:
#line 962 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);

          (yyval.dcval) = dynamic_cast<AST_Decl*> (idl_global->scopes ().top_non_null ());
          /*
           * Done with this interface - pop it off the scopes stack
           */
          idl_global->scopes ().pop ();
        }
#line 3504 "fe/idl.tab.cpp"
    break;

  case 70:
#line 975 "fe/idl.ypp"
         {
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSeen);
         }
#line 3512 "fe/idl.tab.cpp"
    break;

  case 71:
#line 979 "fe/idl.ypp"
         {
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceIDSeen);
           (yyval.idval) = (yyvsp[0].idval);
         }
#line 3521 "fe/idl.tab.cpp"
    break;

  case 72:
#line 987 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);

          if ((yyvsp[0].nlval) != 0 && (yyvsp[0].nlval)->truncatable ())
            {
              idl_global->err ()->syntax_error (
                                      IDL_GlobalData::PS_InheritColonSeen
                                    );
            }

          /*
           * Create an AST representation of the information in the header
           * part of an interface - this representation contains a computed
           * list of all interfaces which this interface inherits from,
           * recursively
           */
          UTL_ScopedName *n = 0;
          ACE_NEW_RETURN (n,
                          UTL_ScopedName ((yyvsp[-1].idval), 0),
                          1);
          ACE_NEW_RETURN ((yyval.ihval),
                          FE_InterfaceHeader (n,
                                              (yyvsp[0].nlval),
                                              false,
                                              false,
                                              true),
                          1);

          if (0 != (yyvsp[0].nlval))
            {
              (yyvsp[0].nlval)->destroy ();
              delete (yyvsp[0].nlval);
              (yyvsp[0].nlval) = 0;
            }
        }
#line 3561 "fe/idl.tab.cpp"
    break;

  case 73:
#line 1024 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);

          /*
           * Create an AST representation of the information in the header
           * part of an interface - this representation contains a computed
           * list of all interfaces which this interface inherits from,
           * recursively
           */
          UTL_ScopedName *n = 0;
          ACE_NEW_RETURN (n,
                          UTL_ScopedName ((yyvsp[-1].idval), 0),
                          1);
          ACE_NEW_RETURN ((yyval.ihval),
                          FE_InterfaceHeader (n,
                                              (yyvsp[0].nlval),
                                              true,
                                              false,
                                              true),
                          1);

          if (0 != (yyvsp[0].nlval))
            {
              (yyvsp[0].nlval)->destroy ();
              delete (yyvsp[0].nlval);
              (yyvsp[0].nlval) = 0;
            }
        }
#line 3594 "fe/idl.tab.cpp"
    break;

  case 74:
#line 1054 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);

          /*
           * Create an AST representation of the information in the header
           * part of an interface - this representation contains a computed
           * list of all interfaces which this interface inherits from,
           * recursively
           */
          UTL_ScopedName *n = 0;
          ACE_NEW_RETURN (n,
                          UTL_ScopedName ((yyvsp[-1].idval), 0),
                          1);
          ACE_NEW_RETURN ((yyval.ihval),
                          FE_InterfaceHeader (n,
                                              (yyvsp[0].nlval),
                                              false,
                                              true,
                                              true),
                          1);

          if (0 != (yyvsp[0].nlval))
            {
              (yyvsp[0].nlval)->destroy ();
              delete (yyvsp[0].nlval);
              (yyvsp[0].nlval) = 0;
            }
        }
#line 3627 "fe/idl.tab.cpp"
    break;

  case 75:
#line 1086 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 3635 "fe/idl.tab.cpp"
    break;

  case 76:
#line 1090 "fe/idl.ypp"
        {
          (yyvsp[0].nlval)->truncatable ((yyvsp[-2].bval));
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3644 "fe/idl.tab.cpp"
    break;

  case 77:
#line 1095 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 3652 "fe/idl.tab.cpp"
    break;

  case 82:
#line 1109 "fe/idl.ypp"
        {
          idl_global->err ()->unsupported_error ("custom is not supported");
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 3661 "fe/idl.tab.cpp"
    break;

  case 84:
#line 1118 "fe/idl.ypp"
        {
          FE_OBVHeader *valuetype_header = (yyvsp[0].vhval);
          UTL_Scope *scope = idl_global->scopes ().top_non_null ();
          AST_ValueType *valuetype = 0;

          if (scope && valuetype_header)
            {
              valuetype = idl_global->gen ()->create_valuetype (
                valuetype_header->name (),
                valuetype_header->inherits (),
                valuetype_header->n_inherits (),
                valuetype_header->inherits_concrete (),
                valuetype_header->inherits_flat (),
                valuetype_header->n_inherits_flat (),
                valuetype_header->supports (),
                valuetype_header->n_supports (),
                valuetype_header->supports_concrete (),
                false,
                valuetype_header->truncatable (),
                false);

              AST_Interface* valuetype_as_interface =
                dynamic_cast<AST_Interface*> (valuetype);
              AST_Interface::fwd_redefinition_helper (
                valuetype_as_interface, scope);

              /*
               * Add the valuetype to its definition scope
               */
              scope->fe_add_valuetype (valuetype);

              // FE_OBVHeader is not automatically destroyed in the AST
              valuetype_header->destroy ();
              delete valuetype_header;
              valuetype_header = ((yyvsp[0].vhval) = 0);
            }

          /*
           * Push it on the scope stack
           */
          idl_global->scopes ().push (valuetype);

          (yyval.dcval) = valuetype;
        }
#line 3710 "fe/idl.tab.cpp"
    break;

  case 85:
#line 1163 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3718 "fe/idl.tab.cpp"
    break;

  case 86:
#line 1167 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3726 "fe/idl.tab.cpp"
    break;

  case 87:
#line 1171 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeQsSeen);

          AST_ValueType *vt =
            AST_ValueType::narrow_from_scope (
                idl_global->scopes ().top_non_null ()
              );

          if (vt != 0 && vt->will_have_factory ())
            {
              idl_global->valuefactory_seen_ = true;
            }

          /*
           * Done with this value type - pop it off the scopes stack
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 3751 "fe/idl.tab.cpp"
    break;

  case 88:
#line 1196 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_ValueType *v = 0;
          AST_Interface *i = 0;

          if (s != 0 && (yyvsp[0].vhval) != 0)
            {
              v =
                idl_global->gen ()->create_valuetype (
                    (yyvsp[0].vhval)->name (),
                    (yyvsp[0].vhval)->inherits (),
                    (yyvsp[0].vhval)->n_inherits (),
                    (yyvsp[0].vhval)->inherits_concrete (),
                    (yyvsp[0].vhval)->inherits_flat (),
                    (yyvsp[0].vhval)->n_inherits_flat (),
                    (yyvsp[0].vhval)->supports (),
                    (yyvsp[0].vhval)->n_supports (),
                    (yyvsp[0].vhval)->supports_concrete (),
                    true,
                    false,
                    false
                  );
              i = AST_Interface::narrow_from_decl (v);
              AST_Interface::fwd_redefinition_helper (i,
                                                      s);
              /*
               * Add the valuetype to its definition scope
               */
              v = AST_ValueType::narrow_from_decl (i);
              (void) s->fe_add_valuetype (v);

              // FE_OBVHeader is not automatically destroyed in the AST
              (yyvsp[0].vhval)->destroy ();
              delete (yyvsp[0].vhval);
              (yyvsp[0].vhval) = 0;
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (v);
        }
#line 3798 "fe/idl.tab.cpp"
    break;

  case 89:
#line 1239 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3806 "fe/idl.tab.cpp"
    break;

  case 90:
#line 1243 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3814 "fe/idl.tab.cpp"
    break;

  case 91:
#line 1247 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeQsSeen);

          /*
           * Done with this valuetype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = 0;
        }
#line 3829 "fe/idl.tab.cpp"
    break;

  case 92:
#line 1262 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 3837 "fe/idl.tab.cpp"
    break;

  case 93:
#line 1266 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);

          UTL_ScopedName *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_ScopedName ((yyvsp[-3].idval), 0),
                          1);
          ACE_NEW_RETURN ((yyval.vhval),
                          FE_OBVHeader (sn,
                                        (yyvsp[-2].nlval),
                                        (yyvsp[0].nlval),
                                        ((yyvsp[-2].nlval) != 0
                                           ? (yyvsp[-2].nlval)->truncatable ()
                                           : false)),
                          1);

          if (0 != (yyvsp[0].nlval))
            {
              (yyvsp[0].nlval)->destroy ();
              delete (yyvsp[0].nlval);
              (yyvsp[0].nlval) = 0;
            }

          if (0 != (yyvsp[-2].nlval))
            {
              (yyvsp[-2].nlval)->destroy ();
              delete (yyvsp[-2].nlval);
              (yyvsp[-2].nlval) = 0;
            }
        }
#line 3872 "fe/idl.tab.cpp"
    break;

  case 94:
#line 1300 "fe/idl.ypp"
        {
           idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSeen);
        }
#line 3880 "fe/idl.tab.cpp"
    break;

  case 95:
#line 1304 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 3889 "fe/idl.tab.cpp"
    break;

  case 96:
#line 1312 "fe/idl.ypp"
        {
          (yyval.bval) = true;
        }
#line 3897 "fe/idl.tab.cpp"
    break;

  case 97:
#line 1316 "fe/idl.ypp"
        {
          (yyval.bval) = false;
        }
#line 3905 "fe/idl.tab.cpp"
    break;

  case 98:
#line 1324 "fe/idl.ypp"
        {
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3913 "fe/idl.tab.cpp"
    break;

  case 99:
#line 1328 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 3921 "fe/idl.tab.cpp"
    break;

  case 100:
#line 1336 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
                            0);
          AST_ValueTypeFwd *f = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeForwardSeen);

          /*
           * Create a node representing a forward declaration of an
           * valuetype. Store it in the enclosing scope
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_valuetype_fwd (&n,
                                                            true);
              (void) s->fe_add_valuetype_fwd (f);
            }

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
        }
#line 3948 "fe/idl.tab.cpp"
    break;

  case 101:
#line 1360 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
                            0);
          AST_ValueTypeFwd *f = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeForwardSeen);

          /*
           * Create a node representing a forward declaration of an
           * valuetype. Store it in the enclosing scope
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_valuetype_fwd (&n,
                                                            false);
              (void) s->fe_add_valuetype_fwd (f);
            }

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;

          (yyval.dcval) = 0;
        }
#line 3977 "fe/idl.tab.cpp"
    break;

  case 102:
#line 1388 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueBoxDeclSeen);

          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[-1].idval),
                            0);

          if (s != 0 && (yyvsp[0].dcval) != 0)
            {
              /*
              * Get the type_spec associated with the valuebox
              */
              AST_Type *tp = 0;
              AST_Typedef *td
                = AST_Typedef::narrow_from_decl ((yyvsp[0].dcval));

              if (td == 0)
                {
                  tp = AST_Type::narrow_from_decl ((yyvsp[0].dcval));
                }
              else
                {
                  tp = td->primitive_base_type ();
                }

              if (tp == 0)
                {
                  // The <type_spec> given is a valid type
                  idl_global->err ()->not_a_type ((yyvsp[0].dcval));
                }
              else
                {
                  AST_Decl::NodeType nt = tp->node_type ();

                  if (nt == AST_Decl::NT_valuetype
                      || nt == AST_Decl::NT_eventtype)
                    {
                      // valuetype is not allowed as <type_spec>
                      // for boxed value
                      idl_global->err ()->error0 (
                          UTL_Error::EIDL_ILLEGAL_BOXED_TYPE
                        );
                    }
                  else
                    {
                      /*
                      * Add the valuebox to its definition scope
                      */
                      AST_ValueBox *vb =
                        idl_global->gen ()->create_valuebox (&n,
                                                             tp);
                      (void) s->fe_add_valuebox (vb);
                    }
                }
            }

          (yyvsp[-1].idval)->destroy ();
          delete (yyvsp[-1].idval);
          (yyvsp[-1].idval) = 0;

          (yyval.dcval) = 0;
        }
#line 4044 "fe/idl.tab.cpp"
    break;

  case 103:
#line 1454 "fe/idl.ypp"
        {
          AST_Annotation_Appls *&annotations = (yyvsp[-1].annotations_val);
          AST_Decls *&decls = (yyvsp[0].decls_val);
          if (decls)
            {
              for (size_t i = 0; i < decls->size (); i++)
                {
                  (*decls)[i]->annotation_appls (*annotations);
                }
            }
          else
            {
              idl_global->err ()-> unsupported_warning (
                "Annotating this is not supported");
            }
          delete annotations;
          delete decls;
        }
#line 4067 "fe/idl.tab.cpp"
    break;

  case 104:
#line 1473 "fe/idl.ypp"
        {
          delete (yyvsp[0].decls_val);
        }
#line 4075 "fe/idl.tab.cpp"
    break;

  case 107:
#line 1482 "fe/idl.ypp"
        {
          AST_Decl *&node = (yyvsp[0].dcval);
          AST_Decls *value = 0;
          if (node)
            {
              value = new AST_Decls;
              value->push_back (node);
            }
          (yyval.decls_val) = value;
        }
#line 4090 "fe/idl.tab.cpp"
    break;

  case 108:
#line 1493 "fe/idl.ypp"
        {
          AST_Decl *&node = (yyvsp[0].dcval);
          AST_Decls *value = 0;
          if (node)
            {
              value = new AST_Decls;
              value->push_back (node);
            }
          (yyval.decls_val) = value;
        }
#line 4105 "fe/idl.tab.cpp"
    break;

  case 109:
#line 1504 "fe/idl.ypp"
        {
          (yyval.decls_val) = (yyvsp[-1].decls_val);
        }
#line 4113 "fe/idl.tab.cpp"
    break;

  case 110:
#line 1511 "fe/idl.ypp"
        {
          (yyval.vival) = AST_Field::vis_PUBLIC;
        }
#line 4121 "fe/idl.tab.cpp"
    break;

  case 111:
#line 1515 "fe/idl.ypp"
        {
          (yyval.vival) = AST_Field::vis_PRIVATE;
        }
#line 4129 "fe/idl.tab.cpp"
    break;

  case 112:
#line 1522 "fe/idl.ypp"
        {
          if ((yyvsp[0].decls_val))
            {
              AST_Decls &decls = *(yyvsp[0].decls_val);
              for (size_t i = 0; i < decls.size (); i++)
                {
                  AST_Field * field = dynamic_cast<AST_Field*> (decls[i]);
                  if (field)
                    {
                      field->visibility ((yyvsp[-1].vival));
                    }
                }
            }
          (yyval.decls_val) = (yyvsp[0].decls_val);
        }
#line 4149 "fe/idl.tab.cpp"
    break;

  case 115:
#line 1546 "fe/idl.ypp"
        {
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          AST_Decl *d = (yyvsp[0].dcval);
          if (d)
            {
              d->annotation_appls (*annotations);
            }
          else
            {
              idl_global->err ()-> unsupported_warning (
                "Annotating this is not supported");
            }
          delete annotations;
        }
#line 4168 "fe/idl.tab.cpp"
    break;

  case 117:
#line 1565 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 4176 "fe/idl.tab.cpp"
    break;

  case 118:
#line 1569 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4184 "fe/idl.tab.cpp"
    break;

  case 119:
#line 1573 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 4192 "fe/idl.tab.cpp"
    break;

  case 120:
#line 1577 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4200 "fe/idl.tab.cpp"
    break;

  case 121:
#line 1581 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 4208 "fe/idl.tab.cpp"
    break;

  case 122:
#line 1585 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4216 "fe/idl.tab.cpp"
    break;

  case 123:
#line 1589 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 4224 "fe/idl.tab.cpp"
    break;

  case 124:
#line 1593 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4232 "fe/idl.tab.cpp"
    break;

  case 125:
#line 1597 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 4240 "fe/idl.tab.cpp"
    break;

  case 126:
#line 1601 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4248 "fe/idl.tab.cpp"
    break;

  case 127:
#line 1605 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 4256 "fe/idl.tab.cpp"
    break;

  case 128:
#line 1609 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4264 "fe/idl.tab.cpp"
    break;

  case 129:
#line 1613 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpDeclSeen);
        }
#line 4272 "fe/idl.tab.cpp"
    break;

  case 130:
#line 1617 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4280 "fe/idl.tab.cpp"
    break;

  case 131:
#line 1621 "fe/idl.ypp"
        {
          idl_global->err ()->syntax_error (idl_global->parse_state ());
        }
#line 4288 "fe/idl.tab.cpp"
    break;

  case 132:
#line 1625 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
          (yyval.dcval) = 0;
        }
#line 4298 "fe/idl.tab.cpp"
    break;

  case 133:
#line 1634 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.nlval),
                          UTL_NameList ((yyvsp[-1].idlist),
                                        (yyvsp[0].nlval)),
                          1);
        }
#line 4309 "fe/idl.tab.cpp"
    break;

  case 134:
#line 1645 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SNListCommaSeen);
        }
#line 4317 "fe/idl.tab.cpp"
    break;

  case 135:
#line 1649 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopedNameSeen);

          UTL_NameList *nl = 0;
          ACE_NEW_RETURN (nl,
                          UTL_NameList ((yyvsp[0].idlist),
                                        0),
                          1);

          if ((yyvsp[-3].nlval) == 0)
            {
              (yyval.nlval) = nl;
            }
          else
            {
              (yyvsp[-3].nlval)->nconc (nl);
              (yyval.nlval) = (yyvsp[-3].nlval);
            }
        }
#line 4341 "fe/idl.tab.cpp"
    break;

  case 136:
#line 1669 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 4349 "fe/idl.tab.cpp"
    break;

  case 137:
#line 1676 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          ACE_NEW_RETURN ((yyval.idlist),
                          UTL_IdList ((yyvsp[0].idval),
                                      0),
                          1);
        }
#line 4362 "fe/idl.tab.cpp"
    break;

  case 138:
#line 1685 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);
        }
#line 4370 "fe/idl.tab.cpp"
    break;

  case 139:
#line 1689 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          Identifier *id = 0;
          ACE_NEW_RETURN (id,
                          Identifier ((yyvsp[-2].strval)),
                          1);
          ACE::strdelete ((yyvsp[-2].strval));
          (yyvsp[-2].strval) = 0;
          UTL_IdList *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_IdList ((yyvsp[0].idval),
                                      0),
                          1);
          ACE_NEW_RETURN ((yyval.idlist),
                          UTL_IdList (id,
                                      sn),
                          1);
        }
#line 4394 "fe/idl.tab.cpp"
    break;

  case 140:
#line 1710 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);

          // This cleans up all the non-global "::"s in scoped names.
          // If there is a global one, it gets put into the UTL_IdList,
          // so we clean it up in the case above.
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 4408 "fe/idl.tab.cpp"
    break;

  case 141:
#line 1720 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          UTL_IdList *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_IdList ((yyvsp[0].idval),
                                      0),
                          1);
          (yyvsp[-3].idlist)->nconc (sn);
          (yyval.idlist) = (yyvsp[-3].idlist);
        }
#line 4424 "fe/idl.tab.cpp"
    break;

  case 142:
#line 1734 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.idval),
                          Identifier ((yyvsp[0].strval)),
                          1);
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 4436 "fe/idl.tab.cpp"
    break;

  case 143:
#line 1744 "fe/idl.ypp"
        {
          /* defining_id is a defining identifier
             whereas id is usually a reference to a defining identifier */
          ACE_NEW_RETURN ((yyval.idval), Identifier ((yyvsp[0].strval)), 1);
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 4448 "fe/idl.tab.cpp"
    break;

  case 144:
#line 1755 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval), 0);

          if (ACE_OS::strcmp ((yyvsp[0].idval)->get_string (),
                              "TypeCode") == 0
              && !idl_global->in_main_file ())
            {
              AST_PredefinedType *pdt =
                idl_global->gen ()->create_predefined_type (
                                        AST_PredefinedType::PT_pseudo,
                                        &n
                                      );

              s->add_to_scope (pdt);

              (yyvsp[0].idval)->destroy ();
              delete (yyvsp[0].idval);
              (yyvsp[0].idval) = 0;

              break;
            }

          AST_InterfaceFwd *f = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceForwardSeen);

          /*
           * Create a node representing a forward declaration of an
           * interface. Store it in the enclosing scope
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_interface_fwd (&n,
                                                            0,
                                                            0);
              (void) s->fe_add_interface_fwd (f);
            }

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
        }
#line 4495 "fe/idl.tab.cpp"
    break;

  case 145:
#line 1799 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
                            0);
          AST_InterfaceFwd *f = 0;
          idl_global->set_parse_state (
                          IDL_GlobalData::PS_InterfaceForwardSeen
                        );

          /*
           * Create a node representing a forward declaration of an
           * interface. Store it in the enclosing scope
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_interface_fwd (&n,
                                                            1,
                                                            0);
              (void) s->fe_add_interface_fwd (f);
            }

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
        }
#line 4525 "fe/idl.tab.cpp"
    break;

  case 146:
#line 1826 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
                            0);
          AST_InterfaceFwd *f = 0;
          idl_global->set_parse_state (
                          IDL_GlobalData::PS_InterfaceForwardSeen
                        );

          /*
           * Create a node representing a forward declaration of an
           * interface. Store it in the enclosing scope
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_interface_fwd (&n,
                                                            0,
                                                            1);
              (void) s->fe_add_interface_fwd (f);
            }

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;

          (yyval.dcval) = dynamic_cast<AST_Decl*> (f);
        }
#line 4557 "fe/idl.tab.cpp"
    break;

  case 147:
#line 1857 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstSeen);
        }
#line 4565 "fe/idl.tab.cpp"
    break;

  case 148:
#line 1861 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstTypeSeen);
        }
#line 4573 "fe/idl.tab.cpp"
    break;

  case 149:
#line 1865 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstIDSeen);
        }
#line 4581 "fe/idl.tab.cpp"
    break;

  case 150:
#line 1869 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstAssignSeen);
        }
#line 4589 "fe/idl.tab.cpp"
    break;

  case 151:
#line 1873 "fe/idl.ypp"
        {
          (yyval.dcval) = 0;
          UTL_ScopedName n ((yyvsp[-4].idval), 0);

          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Constant *c = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstExprSeen);

          /*
           * Create a node representing a constant declaration. Store
           * it in the enclosing scope.
           */
          if ((yyvsp[0].exval) != 0 && s != 0)
            {
              AST_Param_Holder *param_holder = (yyvsp[0].exval)->param_holder ();

              AST_Expression::AST_ExprValue *result =
                (yyvsp[0].exval)->check_and_coerce ((yyvsp[-6].etval), tao_enum_constant_decl);
              tao_enum_constant_decl = 0;

              // If the expression is a template parameter place
              // holder, 'result' will be 0, but it's ok.
              if (result == 0 && param_holder == 0)
                {
                  idl_global->err ()->coercion_error ((yyvsp[0].exval), (yyvsp[-6].etval));
                  (yyvsp[0].exval)->destroy ();
                  delete (yyvsp[0].exval);
                  (yyvsp[0].exval) = 0;
                }
              else
                {
                  AST_Expression::ExprType et = (yyvsp[-6].etval);

                  if (param_holder != 0
                      && et != param_holder->info ()->const_type_)
                    {
                      idl_global->err ()->mismatched_template_param (
                        param_holder->info ()->name_.c_str ());
                    }
                  else
                    {
                      c = idl_global->gen ()->create_constant ((yyvsp[-6].etval), (yyvsp[0].exval), &n);
                      (void) s->fe_add_constant (c);
                      (yyval.dcval) = c;
                    }
                }

              delete result;
              result = 0;
            }

          (yyvsp[-4].idval)->destroy ();
          delete (yyvsp[-4].idval);
          (yyvsp[-4].idval) = 0;
        }
#line 4649 "fe/idl.tab.cpp"
    break;

  case 158:
#line 1938 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_string;
        }
#line 4657 "fe/idl.tab.cpp"
    break;

  case 159:
#line 1942 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_wstring;
        }
#line 4665 "fe/idl.tab.cpp"
    break;

  case 160:
#line 1946 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_PredefinedType *c = 0;
          AST_Typedef *t = 0;
          UTL_ScopedName *sn = (yyvsp[0].idlist);

          /*
           * If the constant's type is a scoped name, it must resolve
           * to a scalar constant type
           */
          AST_Decl *d =
            s->lookup_by_name (sn);

          if (s != 0 && d != 0)
            {
              /*
               * Look through typedefs.
               */
              while (d->node_type () == AST_Decl::NT_typedef)
                {
                  t = AST_Typedef::narrow_from_decl (d);

                  if (t == 0)
                    {
                      break;
                    }

                  d = t->base_type ();
                }

              if (d->node_type () == AST_Decl::NT_pre_defined)
                {
                  c = AST_PredefinedType::narrow_from_decl (d);

                  (yyval.etval) = FE_Utils::PredefinedTypeToExprType (c->pt ());
                }
              else if (d->node_type () == AST_Decl::NT_string)
                {
                  (yyval.etval) = AST_Expression::EV_string;
                }
              else if (d->node_type () == AST_Decl::NT_wstring)
                {
                  (yyval.etval) = AST_Expression::EV_wstring;
                }
              else if (d->node_type () == AST_Decl::NT_enum)
                {
                  (yyval.etval) = AST_Expression::EV_enum;
                  tao_enum_constant_decl =
                    AST_Enum::narrow_from_decl (d);
                }
              else
                {
                  idl_global->err ()->constant_expected (sn, d);
                }
            }
          else
            {
              idl_global->err ()->lookup_error (sn);
            }

          sn->destroy ();
          delete sn;
          sn = 0;
          (yyvsp[0].idlist) = 0;
        }
#line 4735 "fe/idl.tab.cpp"
    break;

  case 164:
#line 2019 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_or,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4748 "fe/idl.tab.cpp"
    break;

  case 166:
#line 2032 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_xor,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4761 "fe/idl.tab.cpp"
    break;

  case 168:
#line 2045 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_and,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4774 "fe/idl.tab.cpp"
    break;

  case 170:
#line 2058 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_left,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4787 "fe/idl.tab.cpp"
    break;

  case 171:
#line 2067 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_right,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4800 "fe/idl.tab.cpp"
    break;

  case 173:
#line 2080 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_add,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4813 "fe/idl.tab.cpp"
    break;

  case 174:
#line 2089 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_minus,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4826 "fe/idl.tab.cpp"
    break;

  case 176:
#line 2102 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mul,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4839 "fe/idl.tab.cpp"
    break;

  case 177:
#line 2111 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_div,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4852 "fe/idl.tab.cpp"
    break;

  case 178:
#line 2120 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mod,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4865 "fe/idl.tab.cpp"
    break;

  case 180:
#line 2133 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_u_plus,
                                    (yyvsp[0].exval),
                                    0
                                  );
        }
#line 4878 "fe/idl.tab.cpp"
    break;

  case 181:
#line 2142 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                   AST_Expression::EC_u_minus,
                                   (yyvsp[0].exval),
                                   0
                                 );
        }
#line 4891 "fe/idl.tab.cpp"
    break;

  case 182:
#line 2151 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                   AST_Expression::EC_bit_neg,
                                   (yyvsp[0].exval),
                                   0
                                 );
        }
#line 4904 "fe/idl.tab.cpp"
    break;

  case 183:
#line 2163 "fe/idl.ypp"
        {
          UTL_ScopedName *name = (yyvsp[0].idlist);

          /*
           * An expression which is a scoped name is not resolved now,
           * but only when it is evaluated (such as when it is assigned
           * as a constant value).
           */
          AST_Expression *expr = 0;

          AST_Decl *node = stack_based_lookup_for_primary_expr ?
            idl_global->scopes ().lookup_by_name (name) :
            idl_global->scopes ().top_non_null ()->lookup_by_name (name);
          if (!node)
            {
              if (!idl_global->ignore_lookup_errors_)
                {
                  idl_global->err ()->lookup_error (name);
                  return 1;
                }
            }
          else if (node->node_type () == AST_Decl::NT_const)
            {
              /*
               * If the scoped name is an IDL constant, it
               * may be used in an array dim, a string
               * bound, or a sequence bound. If so, it
               * must be unsigned and > 0. We assign the
               * constant's value and type to the
               * expression created here so we can check
               * them later.
               */
              AST_Constant *c = dynamic_cast<AST_Constant *> (node);
              expr = idl_global->gen ()->create_expr (
                  c->constant_value (),
                  c->et ());
            }
          else
            {
              // An AST_Expression owns the scoped name
              // passed in this constructor, so we copy it
              // and destroy it below no matter which case
              // is followed.
              expr = idl_global->gen ()->create_expr (name->copy ());
              if (node->node_type () == AST_Decl::NT_enum_val)
                {
                  expr->enum_parent (
                    dynamic_cast<AST_EnumVal *> (node)->enum_parent ());
                }
            }

          (yyval.exval) = expr;
          name->destroy ();
          delete name;
          (yyvsp[0].idlist) = name = 0;
        }
#line 4965 "fe/idl.tab.cpp"
    break;

  case 185:
#line 2221 "fe/idl.ypp"
        {
          (yyval.exval) = (yyvsp[-1].exval);
        }
#line 4973 "fe/idl.tab.cpp"
    break;

  case 186:
#line 2228 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].ival));
        }
#line 4981 "fe/idl.tab.cpp"
    break;

  case 187:
#line 2232 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr ((yyvsp[0].uival));
        }
#line 4990 "fe/idl.tab.cpp"
    break;

  case 188:
#line 2237 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].sval));
          (yyvsp[0].sval)->destroy ();
          delete (yyvsp[0].sval);
          (yyvsp[0].sval) = 0;
        }
#line 5001 "fe/idl.tab.cpp"
    break;

  case 189:
#line 2244 "fe/idl.ypp"
        {
          char *wide_string = (yyvsp[0].wsval);
          (yyval.exval) = idl_global->gen ()->create_expr (wide_string);
          ACE_OS::free (wide_string);
          (yyvsp[0].wsval) = 0;
        }
#line 5012 "fe/idl.tab.cpp"
    break;

  case 190:
#line 2251 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].cval));
        }
#line 5020 "fe/idl.tab.cpp"
    break;

  case 191:
#line 2255 "fe/idl.ypp"
        {
          ACE_OutputCDR::from_wchar wc ((yyvsp[0].wcval));
          (yyval.exval) = idl_global->gen ()->create_expr (wc);
        }
#line 5029 "fe/idl.tab.cpp"
    break;

  case 192:
#line 2260 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].fixval));
        }
#line 5037 "fe/idl.tab.cpp"
    break;

  case 193:
#line 2264 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].dval));
        }
#line 5045 "fe/idl.tab.cpp"
    break;

  case 194:
#line 2268 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr (true);
        }
#line 5053 "fe/idl.tab.cpp"
    break;

  case 195:
#line 2272 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr (false);
        }
#line 5061 "fe/idl.tab.cpp"
    break;

  case 196:
#line 2279 "fe/idl.ypp"
        {
          int good_expression = 1;
          (yyvsp[0].exval)->evaluate (AST_Expression::EK_positive_int);
          AST_Expression::AST_ExprValue *ev = (yyvsp[0].exval)->ev ();

          /*
           * If const_expr is an enum value (AST_EnumVal inherits from
           * AST_Constant), the AST_ExprValue will probably not be set,
           * but there's no need to check anyway
           */
          if (ev != 0)
            {
              switch (ev->et)
              {
                case AST_Expression::EV_ushort:
                  if (ev->u.usval == 0)
                    {
                      good_expression = 0;
                    }

                  break;
                case AST_Expression::EV_ulong:
                  if (ev->u.ulval == 0)
                    {
                      good_expression = 0;
                    }

                  break;
                case AST_Expression::EV_ulonglong:
                  if (ev->u.ullval == 0)
                    {
                      good_expression = 0;
                    }

                  break;
                case AST_Expression::EV_octet:
                  if (ev->u.oval == 0)
                    {
                      good_expression = 0;
                    }

                  break;
                case AST_Expression::EV_bool:
                  if (ev->u.bval == 0)
                    {
                      good_expression = 0;
                    }

                  break;
                default:
                  good_expression = 0;
                  break;
              }
            }

          if (good_expression)
            {
              (yyval.exval) = (yyvsp[0].exval);
            }
          else
            {
              idl_global->err ()->syntax_error (idl_global->parse_state ());
            }
        }
#line 5130 "fe/idl.tab.cpp"
    break;

  case 197:
#line 2347 "fe/idl.ypp"
        {
          if (idl_global->idl_version_ < IDL_VERSION_4)
            {
              idl_global->err ()->idl_version_error (
                "Annotations are an IDL4 feature");
            }

          Identifier *id = (yyvsp[-1].idval);
          UTL_ScopedName name (id, 0);
          AST_Annotation_Decl::escape_name (&name);
          AST_Annotation_Decl *annotation_decl = idl_global->gen ()->
            create_annotation_decl (&name);
          idl_global->scopes ().top_non_null ()->
            fe_add_annotation_decl (annotation_decl);
          idl_global->scopes ().push (annotation_decl);
        }
#line 5151 "fe/idl.tab.cpp"
    break;

  case 198:
#line 2364 "fe/idl.ypp"
        {
          Identifier *id = (yyvsp[-4].idval);
          idl_global->scopes ().pop ();
          id->destroy ();
          delete id;

          (yyval.dcval) = 0;
        }
#line 5164 "fe/idl.tab.cpp"
    break;

  case 204:
#line 2384 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
          idl_global->in_typedef (true);
        }
#line 5173 "fe/idl.tab.cpp"
    break;

  case 208:
#line 2398 "fe/idl.ypp"
        {
          UTL_Scope *scope = idl_global->scopes ().top_non_null ();
          UTL_Scope *root = idl_global->scopes ().bottom ();

          AST_Expression::ExprType type = (yyvsp[-3].etval);
          Identifier *name = (yyvsp[-2].idval);
          // Annotation member names can't clash with C++ keywords
          FE_Utils::original_local_name (name);
          UTL_ScopedName *scoped_name = new UTL_ScopedName (name, 0);
          AST_Expression *default_value = (yyvsp[-1].exval);

          AST_Decl * type_obj;
          switch (type)
            {
            case AST_Expression::EV_string:
            case AST_Expression::EV_wstring:
              type_obj = tao_string_decl;
              break;
            case AST_Expression::EV_enum:
              type_obj = tao_enum_constant_decl;
              break;
            default:
              type_obj = root->lookup_primitive_type (type);
            }

          FE_Declarator *declarator = new FE_Declarator (
            scoped_name, FE_Declarator::FD_simple, 0);
          AST_Annotation_Member *member = idl_global->gen ()->
            create_annotation_member (
              type, declarator->compose (type_obj), declarator->name ());
          scope->fe_add_annotation_member (member);

          declarator->destroy ();
          delete declarator;

          if (default_value)
            {
              // Check if types are compatible, else report error
              AST_Expression::AST_ExprValue *result =
                default_value->check_and_coerce (type, tao_enum_constant_decl);
              tao_enum_constant_decl = 0;
              if (result)
                {
                  member->value (default_value);
                }
              else
                {
                  idl_global->err ()->coercion_error (default_value, type);
                }
              delete result;
            }
        }
#line 5230 "fe/idl.tab.cpp"
    break;

  case 209:
#line 2454 "fe/idl.ypp"
        {
          (yyval.exval) = (yyvsp[0].exval);
        }
#line 5238 "fe/idl.tab.cpp"
    break;

  case 210:
#line 2458 "fe/idl.ypp"
        {
          (yyval.exval) = 0;
        }
#line 5246 "fe/idl.tab.cpp"
    break;

  case 211:
#line 2465 "fe/idl.ypp"
        {
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          AST_Annotation_Appl *annotation = (yyvsp[0].annotation_val);
          if (annotation)
            {
              annotations->add (annotation);
            }
          (yyval.annotations_val) = annotations;
        }
#line 5260 "fe/idl.tab.cpp"
    break;

  case 212:
#line 2478 "fe/idl.ypp"
        {
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          AST_Annotation_Appl *annotation = (yyvsp[0].annotation_val);
          if (annotation)
            {
              annotations->add (annotation);
            }
          (yyval.annotations_val) = annotations;
        }
#line 5274 "fe/idl.tab.cpp"
    break;

  case 213:
#line 2488 "fe/idl.ypp"
        {
          (yyval.annotations_val) = new AST_Annotation_Appls ();
        }
#line 5282 "fe/idl.tab.cpp"
    break;

  case 214:
#line 2495 "fe/idl.ypp"
        {
          if (idl_global->idl_version_ < IDL_VERSION_4)
            {
              idl_global->err ()->idl_version_error (
                "Annotations are an IDL4 feature");
            }

          AST_Annotation_Decl *decl = 0;
          UTL_ScopedName *name = (yyvsp[0].idlist);
          AST_Annotation_Decl::escape_name (name);

          // Check for Matching Annotation Declaration
          AST_Decl *d = idl_global->scopes ().top_non_null ()->
            lookup_by_name (name);
          if (d)
            {
              decl = AST_Annotation_Decl::narrow_from_decl (d);
              if (decl)
                {
                  idl_global->scopes ().push (decl);
                }
            }
          else
            {
              Identifier &id = *name->last_component ();
              switch (idl_global->unknown_annotations_)
                {
                default:
                case IDL_GlobalData::UNKNOWN_ANNOTATIONS_WARN_ONCE:
                  if (idl_global->unknown_annotations_seen_.insert (id) == 1)
                    {
                      break;
                    }
                  idl_global->err ()->lookup_warning (name);
                  break;
                case IDL_GlobalData::UNKNOWN_ANNOTATIONS_WARN_ALL:
                  idl_global->err ()->lookup_warning (name);
                  break;
                case IDL_GlobalData::UNKNOWN_ANNOTATIONS_ERROR:
                  idl_global->err ()->lookup_error (name);
                  break;
                case IDL_GlobalData::UNKNOWN_ANNOTATIONS_IGNORE:
                  break;
                }

              /*
               * Ignore lookup errors for parameter values that might depend on
               * constants or enums from the missing annotation declaration.
               */
              idl_global->ignore_lookup_errors_ = true;
            }

          stack_based_lookup_for_primary_expr = true;

          (yyval.annotation_decl_val) = decl;
        }
#line 5343 "fe/idl.tab.cpp"
    break;

  case 215:
#line 2552 "fe/idl.ypp"
        {
          idl_global->ignore_lookup_errors_ = false;
          stack_based_lookup_for_primary_expr = false;
          AST_Annotation_Appl *appl = 0;
          UTL_ScopedName *name = (yyvsp[-2].idlist);
          AST_Annotation_Decl *decl = (yyvsp[-1].annotation_decl_val);
          AST_Annotation_Appl::Params *params = (yyvsp[0].annotation_params_val);

          if (decl)
            {
              appl = idl_global->gen ()->create_annotation_appl (name, params);
              appl->apply_from (decl);
              idl_global->scopes ().pop ();
            }
          else
            {
              AST_Annotation_Appl::delete_params (params);
            }

          if (name)
            {
              name->destroy ();
              delete name;
            }

          (yyval.annotation_val) = appl;
        }
#line 5375 "fe/idl.tab.cpp"
    break;

  case 216:
#line 2583 "fe/idl.ypp"
        {
          (yyval.annotation_params_val) = (yyvsp[-1].annotation_params_val);
        }
#line 5383 "fe/idl.tab.cpp"
    break;

  case 217:
#line 2587 "fe/idl.ypp"
        {
          (yyval.annotation_params_val) = 0;
        }
#line 5391 "fe/idl.tab.cpp"
    break;

  case 218:
#line 2594 "fe/idl.ypp"
        {
          AST_Annotation_Appl::Params *params = new AST_Annotation_Appl::Params;
          AST_Annotation_Appl::Param *param = new AST_Annotation_Appl::Param;
          param->id = 0;
          param->expr = (yyvsp[0].exval);
          params->push (param);
          (yyval.annotation_params_val) = params;
        }
#line 5404 "fe/idl.tab.cpp"
    break;

  case 219:
#line 2603 "fe/idl.ypp"
        {
          (yyval.annotation_params_val) = (yyvsp[0].annotation_params_val);
        }
#line 5412 "fe/idl.tab.cpp"
    break;

  case 220:
#line 2610 "fe/idl.ypp"
        {
          AST_Annotation_Appl::Params *params = (yyvsp[0].annotation_params_val);
          params->push ((yyvsp[-1].annotation_param_val));
          (yyval.annotation_params_val) = params;
        }
#line 5422 "fe/idl.tab.cpp"
    break;

  case 221:
#line 2618 "fe/idl.ypp"
        {
          AST_Annotation_Appl::Params *params = (yyvsp[0].annotation_params_val);
          params->push ((yyvsp[-1].annotation_param_val));
          (yyval.annotation_params_val) = params;
        }
#line 5432 "fe/idl.tab.cpp"
    break;

  case 222:
#line 2624 "fe/idl.ypp"
        {
          (yyval.annotation_params_val) = new AST_Annotation_Appl::Params;
        }
#line 5440 "fe/idl.tab.cpp"
    break;

  case 223:
#line 2631 "fe/idl.ypp"
        {
          AST_Annotation_Appl::Param *param = new AST_Annotation_Appl::Param;
          param->id = (yyvsp[-2].idval);
          // Annotation Member Names Can't Clash with C++ keywords
          FE_Utils::original_local_name (param->id);
          param->expr = (yyvsp[0].exval);
          (yyval.annotation_param_val) = param;
        }
#line 5453 "fe/idl.tab.cpp"
    break;

  case 224:
#line 2643 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
          idl_global->in_typedef (true);
        }
#line 5462 "fe/idl.tab.cpp"
    break;

  case 225:
#line 2648 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5470 "fe/idl.tab.cpp"
    break;

  case 226:
#line 2652 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5478 "fe/idl.tab.cpp"
    break;

  case 227:
#line 2656 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5486 "fe/idl.tab.cpp"
    break;

  case 228:
#line 2660 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5494 "fe/idl.tab.cpp"
    break;

  case 229:
#line 2664 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Native *node = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_NativeSeen);

          /*
           * Create a node representing a Native and add it to its
           * enclosing scope
           */
          if (s != 0)
            {
              node =
                idl_global->gen ()->create_native (
                                        (yyvsp[0].deval)->name ()
                                      );
              /*
               * Add it to its defining scope
               */
              (void) s->fe_add_native (node);
            }

          (yyvsp[0].deval)->destroy ();
          delete (yyvsp[0].deval);
          (yyvsp[0].deval) = 0;

          (yyval.dcval) = 0;
        }
#line 5526 "fe/idl.tab.cpp"
    break;

  case 230:
#line 2692 "fe/idl.ypp"
        {
          (yyval.dcval) = 0;
        }
#line 5534 "fe/idl.tab.cpp"
    break;

  case 231:
#line 2699 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeSpecSeen);
        }
#line 5542 "fe/idl.tab.cpp"
    break;

  case 232:
#line 2703 "fe/idl.ypp"
        {
          AST_Decl *type_spec = (yyvsp[-2].dcval);
          UTL_DeclList *decls = (yyvsp[0].dlval);

          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          FE_Declarator *d = 0;
          AST_Typedef *t = 0;
          unsigned long index = 0UL;
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclaratorsSeen);

          /*
           * Create a list of type renamings. Add them to the
           * enclosing scope
           */
          if (s && type_spec && decls)
            {
              for (UTL_DecllistActiveIterator l (decls); !l.is_done (); l.next ())
                {
                  d = l.item ();
                  if (d == 0)
                    {
                      continue;
                    }

                  AST_Type *tp = d->compose (type_spec);
                  if (tp == 0)
                    {
                      continue;
                    }

                  if (AST_Decl::NT_except == tp->node_type ())
                    {
                      idl_global->err ()->not_a_type (tp);
                      continue;
                    }

                  t = idl_global->gen ()->create_typedef (tp,
                                                          d->name (),
                                                          s->is_local (),
                                                          s->is_abstract ());

                  // If the base type is a sequence or array, the typedef
                  // constructor sets owns_base_type_ to true. But if
                  // there is a comma-separated list of such typedefs,
                  // the base type can be destroyed only once. In all
                  // other cases, the line below has no effect.
                  if (index++ > 0)
                    {
                      t->owns_base_type (false);
                    }

                  (void) s->fe_add_typedef (t);
                  idl_global->in_typedef (false);
                }

              // This FE_Declarator class isn't destroyed with the AST.
              decls->destroy ();
              delete decls;
              (yyvsp[0].dlval) = decls = 0;
            }

          (yyval.dcval) = t;
        }
#line 5610 "fe/idl.tab.cpp"
    break;

  case 235:
#line 2775 "fe/idl.ypp"
        {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5621 "fe/idl.tab.cpp"
    break;

  case 237:
#line 2783 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = 0;

          if (s != 0)
            {
              d =
                s->lookup_by_name ((yyvsp[0].idlist));
            }

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((yyvsp[0].idlist));
            }
          else
            {
              d->last_referenced_as ((yyvsp[0].idlist));
            }


          (yyval.dcval) = d;
        }
#line 5648 "fe/idl.tab.cpp"
    break;

  case 255:
#line 2838 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 5659 "fe/idl.tab.cpp"
    break;

  case 256:
#line 2849 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 5667 "fe/idl.tab.cpp"
    break;

  case 257:
#line 2853 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsDeclSeen);

          UTL_DeclList *dl = 0;
          ACE_NEW_RETURN (dl,
                          UTL_DeclList ((yyvsp[0].deval),
                                        0),
                          1);

          if ((yyvsp[-3].dlval) == 0)
            {
              (yyval.dlval) = dl;
            }
          else
            {
              (yyvsp[-3].dlval)->nconc (dl);
              (yyval.dlval) = (yyvsp[-3].dlval);
            }
        }
#line 5691 "fe/idl.tab.cpp"
    break;

  case 258:
#line 2873 "fe/idl.ypp"
        {
          (yyval.dlval) = 0;
        }
#line 5699 "fe/idl.tab.cpp"
    break;

  case 261:
#line 2885 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 5710 "fe/idl.tab.cpp"
    break;

  case 262:
#line 2896 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 5718 "fe/idl.tab.cpp"
    break;

  case 263:
#line 2900 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsDeclSeen);

          UTL_DeclList *dl = 0;
          ACE_NEW_RETURN (dl,
                          UTL_DeclList ((yyvsp[0].deval),
                                        0),
                          1);

          if ((yyvsp[-3].dlval) == 0)
            {
              (yyval.dlval) = dl;
            }
          else
            {
              (yyvsp[-3].dlval)->nconc (dl);
              (yyval.dlval) = (yyvsp[-3].dlval);
            }
        }
#line 5742 "fe/idl.tab.cpp"
    break;

  case 264:
#line 2920 "fe/idl.ypp"
        {
          (yyval.dlval) = 0;
        }
#line 5750 "fe/idl.tab.cpp"
    break;

  case 265:
#line 2927 "fe/idl.ypp"
        {
          UTL_ScopedName *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_ScopedName ((yyvsp[0].idval),
                                          0),
                          1);
          ACE_NEW_RETURN ((yyval.deval),
                          FE_Declarator (sn,
                                         FE_Declarator::FD_simple,
                                         0),
                          1);
        }
#line 5767 "fe/idl.tab.cpp"
    break;

  case 266:
#line 2943 "fe/idl.ypp"
        {
          UTL_ScopedName *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_ScopedName (
                              (yyvsp[0].dcval)->local_name ()->copy (),
                              0
                            ),
                          1);
          ACE_NEW_RETURN ((yyval.deval),
                          FE_Declarator (sn,
                                         FE_Declarator::FD_complex,
                                         (yyvsp[0].dcval)),
                          1);
        }
#line 5786 "fe/idl.tab.cpp"
    break;

  case 269:
#line 2966 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_long;
        }
#line 5794 "fe/idl.tab.cpp"
    break;

  case 270:
#line 2970 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_longlong;
        }
#line 5802 "fe/idl.tab.cpp"
    break;

  case 271:
#line 2974 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_short;
        }
#line 5810 "fe/idl.tab.cpp"
    break;

  case 272:
#line 2981 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ulong;
        }
#line 5818 "fe/idl.tab.cpp"
    break;

  case 273:
#line 2985 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ulonglong;
        }
#line 5826 "fe/idl.tab.cpp"
    break;

  case 274:
#line 2989 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ushort;
        }
#line 5834 "fe/idl.tab.cpp"
    break;

  case 275:
#line 2996 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_double;
        }
#line 5842 "fe/idl.tab.cpp"
    break;

  case 276:
#line 3000 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_float;
        }
#line 5850 "fe/idl.tab.cpp"
    break;

  case 277:
#line 3004 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_longdouble;
        }
#line 5858 "fe/idl.tab.cpp"
    break;

  case 278:
#line 3011 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_fixed;
        }
#line 5866 "fe/idl.tab.cpp"
    break;

  case 279:
#line 3018 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_char;
        }
#line 5874 "fe/idl.tab.cpp"
    break;

  case 280:
#line 3022 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_wchar;
        }
#line 5882 "fe/idl.tab.cpp"
    break;

  case 281:
#line 3029 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_octet;
        }
#line 5890 "fe/idl.tab.cpp"
    break;

  case 282:
#line 3036 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_bool;
        }
#line 5898 "fe/idl.tab.cpp"
    break;

  case 283:
#line 3043 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_any;
        }
#line 5906 "fe/idl.tab.cpp"
    break;

  case 284:
#line 3050 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_object;
        }
#line 5914 "fe/idl.tab.cpp"
    break;

  case 285:
#line 3057 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSeen);
        }
#line 5922 "fe/idl.tab.cpp"
    break;

  case 286:
#line 3061 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 5931 "fe/idl.tab.cpp"
    break;

  case 287:
#line 3070 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval), 0);
          AST_Structure *d = 0;

          /*
           * Create a node representing a struct declaration. Add it
           * to the enclosing scope
           */
          if (s != 0)
            {
              d =
                idl_global->gen ()->create_structure (
                                        &n,
                                        s->is_local (),
                                        s->is_abstract ()
                                      );
              AST_Structure::fwd_redefinition_helper (d, s);
              (void) s->fe_add_structure (d);
            }

          /*
           * Push the scope of the struct on the scopes stack.
           */
          idl_global->scopes ().push (d);

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
        }
#line 5966 "fe/idl.tab.cpp"
    break;

  case 288:
#line 3101 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSqSeen);
        }
#line 5974 "fe/idl.tab.cpp"
    break;

  case 289:
#line 3105 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructBodySeen);
        }
#line 5982 "fe/idl.tab.cpp"
    break;

  case 290:
#line 3109 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructQsSeen);

          /*
           * Done with this struct. Pop its scope off the scopes stack.
           */
          (yyval.dcval) = AST_Structure::narrow_from_scope (
                   idl_global->scopes ().top_non_null ()
                 );
          idl_global->scopes ().pop ();
        }
#line 5998 "fe/idl.tab.cpp"
    break;

  case 294:
#line 3131 "fe/idl.ypp"
        {
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          AST_Decls *members = (yyvsp[0].decls_val);
          if (annotations && members)
            {
              for (size_t i = 0; i < members->size (); i++)
                {
                  (*members)[i]->annotation_appls (*annotations);
                }
            }
          delete annotations;
          delete members;
        }
#line 6016 "fe/idl.tab.cpp"
    break;

  case 295:
#line 3148 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberTypeSeen);
        }
#line 6024 "fe/idl.tab.cpp"
    break;

  case 296:
#line 3152 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsSeen);
        }
#line 6032 "fe/idl.tab.cpp"
    break;

  case 297:
#line 3156 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          FE_Declarator *d = 0;
          AST_Field *f = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsCompleted);
          AST_Decls *members = 0;

          /*
           * Check for illegal recursive use of type.
           */
          if ((yyvsp[-4].dcval) != 0
              && AST_illegal_recursive_type ((yyvsp[-4].dcval)))
            {
              idl_global->err ()->error1 (UTL_Error::EIDL_RECURSIVE_TYPE,
                                          (yyvsp[-4].dcval));
            }
          /*
           * Create a node representing a struct or exception member
           * Add it to the enclosing scope.
           */
          else if (s && (yyvsp[-4].dcval) && (yyvsp[-2].dlval))
            {
              members = new AST_Decls;
              for (UTL_DecllistActiveIterator l ((yyvsp[-2].dlval));
                   !l.is_done ();
                   l.next ())
                {
                  d = l.item ();

                  if (d == 0)
                    {
                      continue;
                    }

                  AST_Type *tp = d->compose ((yyvsp[-4].dcval));

                  if (tp == 0)
                    {
                      continue;
                    }

                  f = idl_global->gen ()->create_field (tp, d->name ());
                  members->push_back (f);
                  (void) s->fe_add_field (f);
                }
            }

          (yyvsp[-2].dlval)->destroy ();
          delete (yyvsp[-2].dlval);
          (yyvsp[-2].dlval) = 0;

          (yyval.decls_val) = members;
        }
#line 6090 "fe/idl.tab.cpp"
    break;

  case 298:
#line 3210 "fe/idl.ypp"
        {
          idl_global->err ()->syntax_error (idl_global->parse_state ());
        }
#line 6098 "fe/idl.tab.cpp"
    break;

  case 299:
#line 3214 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 6107 "fe/idl.tab.cpp"
    break;

  case 300:
#line 3222 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSeen);
        }
#line 6115 "fe/idl.tab.cpp"
    break;

  case 301:
#line 3226 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 6124 "fe/idl.tab.cpp"
    break;

  case 302:
#line 3234 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchSeen);
        }
#line 6132 "fe/idl.tab.cpp"
    break;

  case 303:
#line 3238 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[-3].idval), 0);
          AST_Union *u = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchOpenParSeen);

          /*
           * Create a node representing an empty union. Add it to its enclosing
           * scope.
           */
          if (s != 0)
            {
              u = idl_global->gen ()->create_union (0,
                                                    &n,
                                                    s->is_local (),
                                                    s->is_abstract ());

              AST_Structure *st = AST_Structure::narrow_from_decl (u);
              AST_Structure::fwd_redefinition_helper (st, s);
              u = AST_Union::narrow_from_decl (st);
              (void) s->fe_add_union (u);
            }

          /*
           * Push the scope of the union on the scopes stack
           */
          idl_global->scopes ().push (u);

          /*
           * Don't delete $1 yet; we'll need it a bit later.
           */
        }
#line 6169 "fe/idl.tab.cpp"
    break;

  case 304:
#line 3271 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchTypeSeen);
        }
#line 6177 "fe/idl.tab.cpp"
    break;

  case 305:
#line 3275 "fe/idl.ypp"
        {
          /*
           * The top of the scopes must be an empty union we added after we
           * encountered 'union <id> switch ('. Now we are ready to add a
           * correct one. Temporarily remove the top so that we setup the
           * correct union in a right scope.
           */
          UTL_Scope *top = idl_global->scopes ().top_non_null ();
          idl_global->scopes ().pop ();
          AST_Annotation_Appls *disc_annotations = (yyvsp[-3].annotations_val);

          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[-8].idval), 0);
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchCloseParSeen);

          /*
           * Create a node representing a union.
           */
          if ((yyvsp[-2].dcval) != 0 && s != 0)
            {
              AST_ConcreteType *tp =
                AST_ConcreteType::narrow_from_decl ((yyvsp[-2].dcval));

              if (tp == 0)
                {
                  idl_global->err ()->not_a_type ((yyvsp[-2].dcval));
                }
              else
                {
                  /* Create a union with a correct discriminator. */
                  AST_Union *u = 0;
                  u = idl_global->gen ()->create_union (tp,
                                                        &n,
                                                        s->is_local (),
                                                        s->is_abstract ());

                  /* Narrow the enclosing scope. */
                  AST_Union *e = AST_Union::narrow_from_scope (top);

                  e->redefine (u);
                  e->disc_annotations (*disc_annotations); // Set Discriminator Annotations

                  u->destroy ();
                  delete u;
                }
            }

          /*
           * Restore the top.
           */
          idl_global->scopes ().push (top);

          (yyvsp[-8].idval)->destroy ();
          delete (yyvsp[-8].idval);
          (yyvsp[-8].idval) = 0;

          delete disc_annotations;
        }
#line 6240 "fe/idl.tab.cpp"
    break;

  case 306:
#line 3334 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSqSeen);
        }
#line 6248 "fe/idl.tab.cpp"
    break;

  case 307:
#line 3338 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionBodySeen);
        }
#line 6256 "fe/idl.tab.cpp"
    break;

  case 308:
#line 3342 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionQsSeen);

          /*
           * Done with this union. Pop its scope from the scopes stack.
           */
          (yyval.dcval) = AST_Union::narrow_from_scope (
                   idl_global->scopes ().top_non_null ()
                 );

          if ((yyval.dcval) != 0)
            {
              idl_global->scopes ().pop ();
            }
        }
#line 6276 "fe/idl.tab.cpp"
    break;

  case 309:
#line 3361 "fe/idl.ypp"
        {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 6287 "fe/idl.tab.cpp"
    break;

  case 310:
#line 3368 "fe/idl.ypp"
        {
          /* wchars are not allowed. */
          if ((yyvsp[0].etval) == AST_Expression::EV_wchar)
            {
              idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
            }

          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 6304 "fe/idl.tab.cpp"
    break;

  case 311:
#line 3381 "fe/idl.ypp"
        {
          /* octets are not allowed. */
          idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 6317 "fe/idl.tab.cpp"
    break;

  case 312:
#line 3390 "fe/idl.ypp"
        {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 6328 "fe/idl.tab.cpp"
    break;

  case 314:
#line 3398 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = 0;
          AST_PredefinedType *p = 0;
          AST_Typedef *t = 0;
          bool found = false;

          /*
           * The discriminator is a scoped name. Try to resolve to
           * one of the scalar types or to an enum. Thread through
           * typedef's to arrive at the base type at the end of the
           * chain.
           */
          d =
            s->lookup_by_name ((yyvsp[0].idlist));

          if (s != 0 && d != 0)
            {
              while (!found)
                {
                  switch (d->node_type ())
                  {
                    case AST_Decl::NT_enum:
                      (yyval.dcval) = d;
                      found = true;
                      break;
                    case AST_Decl::NT_pre_defined:
                      p = AST_PredefinedType::narrow_from_decl (d);

                      if (p != 0)
                        {
                          switch (p->pt ())
                          {
                            case AST_PredefinedType::PT_long:
                            case AST_PredefinedType::PT_ulong:
                            case AST_PredefinedType::PT_longlong:
                            case AST_PredefinedType::PT_ulonglong:
                            case AST_PredefinedType::PT_short:
                            case AST_PredefinedType::PT_ushort:
                            case AST_PredefinedType::PT_char:
                            case AST_PredefinedType::PT_boolean:
                              (yyval.dcval) = p;
                              found = true;
                              break;
                            case AST_PredefinedType::PT_wchar:
                            case AST_PredefinedType::PT_octet:
                              /* octets and wchars are not allowed */
                              idl_global->err ()->error0 (
                                  UTL_Error::EIDL_DISC_TYPE
                                );
                              (yyval.dcval) = 0;
                              found = true;
                              break;
                            default:
                              (yyval.dcval) = 0;
                              found = true;
                              break;
                          }
                        }
                      else
                        {
                          (yyval.dcval) = 0;
                          found = true;
                        }

                      break;
                    case AST_Decl::NT_typedef:
                      t = AST_Typedef::narrow_from_decl (d);

                      if (t != 0)
                        {
                          d = t->base_type ();
                        }

                      break;
                    default:
                      (yyval.dcval) = 0;
                      found = true;
                      break;
                  }
                }
            }
          else
            {
              (yyval.dcval) = 0;
            }

          if ((yyval.dcval) == 0)
            {
              idl_global->err ()->lookup_error ((yyvsp[0].idlist));

              (yyvsp[0].idlist)->destroy ();
              delete (yyvsp[0].idlist);
              (yyvsp[0].idlist) = 0;

              /* If we don't return here, we'll crash later.*/
              if (!idl_global->ignore_lookup_errors_)
                {
                  return 1;
                }
            }

          (yyvsp[0].idlist)->destroy ();
          delete (yyvsp[0].idlist);
          (yyvsp[0].idlist) = 0;
        }
#line 6439 "fe/idl.tab.cpp"
    break;

  case 318:
#line 3515 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionLabelSeen);
        }
#line 6447 "fe/idl.tab.cpp"
    break;

  case 319:
#line 3519 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemSeen);
        }
#line 6455 "fe/idl.tab.cpp"
    break;

  case 320:
#line 3523 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_UnionBranch *b = 0;
          AST_Annotation_Appls *annotations = (yyvsp[-3].annotations_val);
          AST_Field *f = (yyvsp[-2].ffval);
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemCompleted);

          /*
           * Create several nodes representing branches of a union.
           * Add them to the enclosing scope (the union scope)
           */
          if (s != 0 && (yyvsp[-5].llval) != 0 && (yyvsp[-2].ffval) != 0)
            {
              b =
                idl_global->gen ()->create_union_branch (
                                        (yyvsp[-5].llval),
                                        f->field_type (),
                                        f->name ()
                                      );
              b->annotation_appls (*annotations);
              (void) s->fe_add_union_branch (b);

              // f has passed its field type to the union branch,
              // but the rest still needs to be cleaned up.
              f->AST_Decl::destroy ();
              delete f;
              f = 0;
            }

          delete annotations;
        }
#line 6491 "fe/idl.tab.cpp"
    break;

  case 321:
#line 3555 "fe/idl.ypp"
        {
          idl_global->err ()->syntax_error (idl_global->parse_state ());
        }
#line 6499 "fe/idl.tab.cpp"
    break;

  case 322:
#line 3559 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 6508 "fe/idl.tab.cpp"
    break;

  case 323:
#line 3567 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.llval),
                          UTL_LabelList ((yyvsp[-1].ulval),
                                         (yyvsp[0].llval)),
                          1);
        }
#line 6519 "fe/idl.tab.cpp"
    break;

  case 324:
#line 3577 "fe/idl.ypp"
        {
          UTL_LabelList *ll = 0;
          ACE_NEW_RETURN (ll,
                          UTL_LabelList ((yyvsp[0].ulval),
                                         0),
                          1);

          if ((yyvsp[-1].llval) == 0)
            {
              (yyval.llval) = ll;
            }
          else
            {
              (yyvsp[-1].llval)->nconc (ll);
              (yyval.llval) = (yyvsp[-1].llval);
            }
        }
#line 6541 "fe/idl.tab.cpp"
    break;

  case 325:
#line 3595 "fe/idl.ypp"
        {
          (yyval.llval) = 0;
        }
#line 6549 "fe/idl.tab.cpp"
    break;

  case 326:
#line 3602 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DefaultSeen);
        }
#line 6557 "fe/idl.tab.cpp"
    break;

  case 327:
#line 3606 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen ()->create_union_label (
                                       AST_UnionLabel::UL_default,
                                       0
                                     );
        }
#line 6570 "fe/idl.tab.cpp"
    break;

  case 328:
#line 3615 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_CaseSeen);
        }
#line 6578 "fe/idl.tab.cpp"
    break;

  case 329:
#line 3619 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelExprSeen);
        }
#line 6586 "fe/idl.tab.cpp"
    break;

  case 330:
#line 3623 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen ()->create_union_label (
                                      AST_UnionLabel::UL_label,
                                      (yyvsp[-2].exval)
                                    );
        }
#line 6599 "fe/idl.tab.cpp"
    break;

  case 331:
#line 3635 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemTypeSeen);
        }
#line 6607 "fe/idl.tab.cpp"
    break;

  case 332:
#line 3639 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemDeclSeen);

          /*
           * Check for illegal recursive use of type
           */
          if ((yyvsp[-2].dcval) != 0
              && AST_illegal_recursive_type ((yyvsp[-2].dcval)))
            {
              idl_global->err ()->error1 (UTL_Error::EIDL_RECURSIVE_TYPE, (yyvsp[-2].dcval));

              (yyval.ffval) = 0;
            }
          /*
           * Create a field in a union branch
           */
          else if ((yyvsp[-2].dcval) == 0
                   || (yyvsp[0].deval) == 0)
            {
              (yyval.ffval) = 0;
            }
          else
            {
              AST_Type *tp = (yyvsp[0].deval)->compose ((yyvsp[-2].dcval));

              if (tp == 0)
                {
                  (yyval.ffval) = 0;
                }
              else
                {
                  (yyval.ffval) = idl_global->gen ()->create_field (
                                               tp,
                                               (yyvsp[0].deval)->name ()
                                             );
                }

              (yyvsp[0].deval)->destroy ();
              delete (yyvsp[0].deval);
              (yyvsp[0].deval) = 0;
            }
        }
#line 6654 "fe/idl.tab.cpp"
    break;

  case 333:
#line 3685 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
                            0);
          AST_StructureFwd *d = 0;

          /*
           * Create a node representing a forward declaration of a struct.
           */
          if (s != 0)
            {
              d = idl_global->gen ()->create_structure_fwd (&n);
              (void) s->fe_add_structure_fwd (d);
            }

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;

          (yyval.dcval) = d;
        }
#line 6680 "fe/idl.tab.cpp"
    break;

  case 334:
#line 3710 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
                            0);
          AST_UnionFwd *u = 0;

          /*
           * Create a node representing a forward declaration of a union.
           */
          if (s != 0)
            {
              u = idl_global->gen ()->create_union_fwd (&n);
              (void) s->fe_add_union_fwd (u);
            }

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
        }
#line 6704 "fe/idl.tab.cpp"
    break;

  case 335:
#line 3733 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSeen);
        }
#line 6712 "fe/idl.tab.cpp"
    break;

  case 336:
#line 3737 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval), 0);
          AST_Enum *e = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumIDSeen);

          /*
           * Create a node representing an enum and add it to its
           * enclosing scope.
           */
          if (s != 0)
            {
              e = idl_global->gen ()->create_enum (&n,
                                                   s->is_local (),
                                                   s->is_abstract ());
              /*
               * Add it to its defining scope
               */
              (void) s->fe_add_enum (e);
            }

          /*
           * Push the enum scope on the scopes stack.
           */
          idl_global->scopes ().push (e);

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
        }
#line 6747 "fe/idl.tab.cpp"
    break;

  case 337:
#line 3768 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSqSeen);
        }
#line 6755 "fe/idl.tab.cpp"
    break;

  case 338:
#line 3772 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumBodySeen);
        }
#line 6763 "fe/idl.tab.cpp"
    break;

  case 339:
#line 3776 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumQsSeen);

          /*
           * Done with this enum. Pop its scope from the scopes stack.
           */
          if (idl_global->scopes ().top () == 0)
            {
              (yyval.dcval) = 0;
            }
          else
            {
              (yyval.dcval) = AST_Enum::narrow_from_scope (
                       idl_global->scopes ().top_non_null ()
                     );
              idl_global->scopes ().pop ();
            }
        }
#line 6786 "fe/idl.tab.cpp"
    break;

  case 341:
#line 3801 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumCommaSeen);
        }
#line 6794 "fe/idl.tab.cpp"
    break;

  case 344:
#line 3810 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          Identifier id ((yyvsp[0].strval));
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
          UTL_ScopedName n (&id, 0);
          AST_EnumVal *e = 0;
          AST_Enum *c = 0;

          /*
           * Create a node representing one enumerator in an enum
           * Add it to the enclosing scope (the enum scope)
           */
          if (s != 0
              && s->scope_node_type () == AST_Decl::NT_enum)
            {
              c = AST_Enum::narrow_from_scope (s);

              if (c != 0)
                {
                  e = idl_global->gen ()->create_enum_val (
                                              c->next_enum_val (),
                                              &n
                                            );
                  e->enum_parent (c);
                  e->annotation_appls (*annotations);
                }

              (void) s->fe_add_enum_val (e);
            }

          delete annotations;
        }
#line 6833 "fe/idl.tab.cpp"
    break;

  case 345:
#line 3849 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceCommaSeen);
        }
#line 6841 "fe/idl.tab.cpp"
    break;

  case 346:
#line 3853 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceExprSeen);
        }
#line 6849 "fe/idl.tab.cpp"
    break;

  case 347:
#line 3857 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceQsSeen);

          AST_Sequence *seq = 0;
          Decl_Annotations_Pair *seq_head = (yyvsp[-5].decl_annotations_pair_val);
          AST_Decl *type = 0;
          AST_Annotation_Appls *type_annotations = 0;
          if (seq_head)
            {
              type = seq_head->decl;
              type_annotations = seq_head->annotations;
            }
          delete seq_head;

          /*
           * Remove sequence marker from scopes stack.
           */
          if (idl_global->scopes ().top () == 0)
            {
              idl_global->scopes ().pop ();
            }

          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          /*
           * Create a node representing a sequence
           */
          AST_Expression::AST_ExprValue *ev = 0;
          AST_Param_Holder *param_holder = 0;

          if ((yyvsp[-2].exval) != 0)
            {
              param_holder =
                (yyvsp[-2].exval)->param_holder ();

              ev = (yyvsp[-2].exval)->coerce (AST_Expression::EV_ulong);
            }

          // If the expression corresponds to a template parameter,
          // it's ok for the coercion to fail at this point. We check
          // for a type mismatch below.
          if (0 == (yyvsp[-2].exval)
              || (0 == ev && 0 == param_holder))
            {
              idl_global->err ()->coercion_error ((yyvsp[-2].exval),
                                                  AST_Expression::EV_ulong);
            }
          else if (type)
            {
              AST_Type *tp = AST_Type::narrow_from_decl (type);

              if (0 == tp)
                {
                  ; // Error will be caught in FE_Declarator.
                }
              else
                {
                  Identifier id ("sequence");
                  UTL_ScopedName sn (&id,
                                     0);

                  seq =
                    idl_global->gen ()->create_sequence (
                                            (yyvsp[-2].exval),
                                            tp,
                                            &sn,
                                            s->is_local (),
                                            s->is_abstract ()
                                          );
                  seq->base_type_annotations (*type_annotations);

                  if (!idl_global->in_typedef ()
                      && !idl_global->anon_silent ())
                    {
                      idl_global->err ()->anonymous_type_diagnostic ();
                    }
                }
            }

          delete type_annotations;
          delete ev;
          ev = 0;
          (yyval.dcval) = seq;
        }
#line 6938 "fe/idl.tab.cpp"
    break;

  case 348:
#line 3943 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceQsSeen);

          AST_Sequence *seq = 0;
          Decl_Annotations_Pair *seq_head = (yyvsp[-1].decl_annotations_pair_val);
          AST_Decl *type = 0;
          AST_Annotation_Appls *type_annotations = 0;
          if (seq_head)
            {
              type = seq_head->decl;
              type_annotations = seq_head->annotations;
            }
          delete seq_head;

          /*
           * Remove sequence marker from scopes stack.
           */
          if (idl_global->scopes ().top () == 0)
            {
             idl_global->scopes ().pop ();
            }

          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          /*
           * Create a node representing a sequence.
           */
          if (type)
            {
              AST_Type *tp = AST_Type::narrow_from_decl (type);

              if (tp == 0)
                {
                  ; // Error will be caught in FE_Declarator.
                }
              else
                {
                  Identifier id ("sequence");
                  UTL_ScopedName sn (&id, 0);
                  ACE_CDR::ULong bound = 0UL;

                  seq =
                    idl_global->gen ()->create_sequence (
                        idl_global->gen ()->create_expr (
                                                bound,
                                                AST_Expression::EV_ulong
                                              ),
                        tp,
                        &sn,
                        s->is_local (),
                        s->is_abstract ()
                      );
                  seq->base_type_annotations (*type_annotations);

                  if (!idl_global->in_typedef ()
                      && !idl_global->anon_silent ())
                    {
                      idl_global->err ()->anonymous_type_diagnostic ();
                    }
                }
            }

          delete type_annotations;
          (yyval.dcval) = seq;
        }
#line 7008 "fe/idl.tab.cpp"
    break;

  case 349:
#line 4012 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSeen);

          /*
           * Push a sequence marker on scopes stack.
           */
          idl_global->scopes ().push (0);
        }
#line 7021 "fe/idl.tab.cpp"
    break;

  case 350:
#line 4021 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSqSeen);
        }
#line 7029 "fe/idl.tab.cpp"
    break;

  case 351:
#line 4025 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceTypeSeen);
          Decl_Annotations_Pair *seq_head = new Decl_Annotations_Pair;
          seq_head->decl = (yyvsp[0].dcval);
          seq_head->annotations = (yyvsp[-1].annotations_val);
          (yyval.decl_annotations_pair_val) = seq_head;
        }
#line 7041 "fe/idl.tab.cpp"
    break;

  case 352:
#line 4036 "fe/idl.ypp"
        {
          (yyvsp[-1].exval)->evaluate (AST_Expression::EK_positive_int);
          (yyval.dcval) = idl_global->gen ()->create_fixed ((yyvsp[-3].exval), (yyvsp[-1].exval));
        }
#line 7050 "fe/idl.tab.cpp"
    break;

  case 353:
#line 4045 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 7058 "fe/idl.tab.cpp"
    break;

  case 354:
#line 4049 "fe/idl.ypp"
        {
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 7066 "fe/idl.tab.cpp"
    break;

  case 355:
#line 4053 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringQsSeen);

          /*
           * Create a node representing a string.
           */
          AST_Expression::AST_ExprValue *ev = 0;

          if ((yyvsp[-2].exval) != 0)
            {
              ev = (yyvsp[-2].exval)->coerce (AST_Expression::EV_ulong);
            }

          if (0 == (yyvsp[-2].exval) || 0 == ev)
            {
              idl_global->err ()->coercion_error ((yyvsp[-2].exval),
                                                  AST_Expression::EV_ulong);
              (yyval.dcval) = 0;
            }
          else
            {
              tao_string_decl = idl_global->gen ()->create_string ((yyvsp[-2].exval));

              /*
               * Add this AST_String to the types defined in the global scope.
               */
              idl_global->root ()->fe_add_string (
                AST_String::narrow_from_decl (
                  tao_string_decl));

              if (!idl_global->in_typedef () && !idl_global->anon_silent ())
                {
                  idl_global->err ()->anonymous_type_diagnostic ();
                }

              (yyval.dcval) = tao_string_decl;
            }

          delete ev;
          ev = 0;
        }
#line 7112 "fe/idl.tab.cpp"
    break;

  case 356:
#line 4095 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringCompleted);

          /*
           * Create a node representing a string.
           */
          ACE_CDR::ULong bound = 0UL;
          tao_string_decl =
            idl_global->gen ()->create_string (
                idl_global->gen ()->create_expr (bound,
                                                 AST_Expression::EV_ulong)
              );

          /*
           * Add this AST_String to the types defined in the global scope.
           */
          idl_global->root ()->fe_add_string (
            AST_String::narrow_from_decl (
              tao_string_decl));

          (yyval.dcval) = tao_string_decl;
        }
#line 7139 "fe/idl.tab.cpp"
    break;

  case 357:
#line 4121 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 7147 "fe/idl.tab.cpp"
    break;

  case 358:
#line 4129 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 7155 "fe/idl.tab.cpp"
    break;

  case 359:
#line 4133 "fe/idl.ypp"
        {
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 7163 "fe/idl.tab.cpp"
    break;

  case 360:
#line 4137 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringQsSeen);

          /*
           * Create a node representing a string.
           */
          if ((yyvsp[-2].exval) == 0
              || (yyvsp[-2].exval)->coerce (AST_Expression::EV_ulong) == 0)
            {
              idl_global->err ()->coercion_error ((yyvsp[-2].exval),
                                                  AST_Expression::EV_ulong);
              (yyval.dcval) = 0;
            }
          else
            {
              AST_String *string = idl_global->gen ()->create_wstring ((yyvsp[-2].exval));

              /*
               * Add this AST_String to the types defined in the global scope.
               */
              idl_global->root ()->fe_add_string (
                AST_String::narrow_from_decl (
                  string));

              if (!idl_global->in_typedef () && !idl_global->anon_silent ())
                {
                  idl_global->err ()->anonymous_type_diagnostic ();
                }

              (yyval.dcval) = string;
            }
        }
#line 7200 "fe/idl.tab.cpp"
    break;

  case 361:
#line 4170 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringCompleted);

          /*
           * Create a node representing a wstring.
           */
          ACE_CDR::ULong bound = 0UL;
          AST_String *string =
            idl_global->gen ()->create_wstring (
                idl_global->gen ()->create_expr (bound,
                                                 AST_Expression::EV_ulong)
              );

          /*
           * Add this AST_String to the types defined in the global scope.
           */
          idl_global->root ()->fe_add_string (
            AST_String::narrow_from_decl (
              string));

          (yyval.dcval) = string;
        }
#line 7227 "fe/idl.tab.cpp"
    break;

  case 362:
#line 4196 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 7235 "fe/idl.tab.cpp"
    break;

  case 363:
#line 4203 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayIDSeen);
        }
#line 7243 "fe/idl.tab.cpp"
    break;

  case 364:
#line 4207 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayCompleted);

          AST_Array *array = 0;
          AST_Annotation_Appls *base_type_annotations = (yyvsp[-1].annotations_val);
          UTL_ExprList *array_dims = (yyvsp[0].elval);

          /*
           * Create a node representing an array.
           */
          if (array_dims)
            {
              UTL_ScopedName sn ((yyvsp[-3].idval),
                                 0);
              array =
                idl_global->gen ()->create_array (
                                        &sn,
                                        array_dims->length (),
                                        array_dims,
                                        0,
                                        0
                                      );
              array->base_type_annotations (*base_type_annotations);
              sn.destroy ();

              if (!idl_global->in_typedef ()
                  && !idl_global->anon_silent ())
                {
                  idl_global->err ()->anonymous_type_diagnostic ();
                }
            }

          array_dims->destroy ();
          delete array_dims;

          delete base_type_annotations;

          (yyval.dcval) = array;
        }
#line 7287 "fe/idl.tab.cpp"
    break;

  case 365:
#line 4250 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.elval),
                          UTL_ExprList ((yyvsp[-1].exval),
                                        (yyvsp[0].elval)),
                          1);
        }
#line 7298 "fe/idl.tab.cpp"
    break;

  case 366:
#line 4260 "fe/idl.ypp"
        {
          UTL_ExprList *el = 0;
          ACE_NEW_RETURN (el,
                          UTL_ExprList ((yyvsp[0].exval),
                                        0),
                          1);

          if ((yyvsp[-1].elval) == 0)
            {
              (yyval.elval) = el;
            }
          else
            {
              (yyvsp[-1].elval)->nconc (el);
              (yyval.elval) = (yyvsp[-1].elval);
            }
        }
#line 7320 "fe/idl.tab.cpp"
    break;

  case 367:
#line 4278 "fe/idl.ypp"
        {
          (yyval.elval) = 0;
        }
#line 7328 "fe/idl.tab.cpp"
    break;

  case 368:
#line 4285 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimSqSeen);
        }
#line 7336 "fe/idl.tab.cpp"
    break;

  case 369:
#line 4289 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimExprSeen);
        }
#line 7344 "fe/idl.tab.cpp"
    break;

  case 370:
#line 4293 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimQsSeen);

          /*
           * Array dimensions are expressions which must be coerced to
           * positive integers.
           */
          AST_Expression::AST_ExprValue *ev = 0;
          AST_Param_Holder *param_holder = 0;

          if ((yyvsp[-2].exval) != 0)
            {
              param_holder =
                (yyvsp[-2].exval)->param_holder ();

              ev =
                (yyvsp[-2].exval)->coerce (AST_Expression::EV_ulong);
            }

          if (0 == (yyvsp[-2].exval)
              || (ev == 0 && param_holder == 0))
            {
              idl_global->err ()->coercion_error ((yyvsp[-2].exval),
                                                  AST_Expression::EV_ulong);
              (yyval.exval) = 0;
            }
          else
            {
              if (param_holder != 0)
                {
                  AST_Expression::ExprType et =
                    param_holder->info ()->const_type_;

                  // If the bound expression represents a
                  // template parameter, it must be a const
                  // and of type unsigned long.
                  if (et != AST_Expression::EV_ulong)
                    {
                      idl_global->err ()->mismatched_template_param (
                        param_holder->info ()->name_.c_str ());

                      delete ev;
                      ev = 0;
                      return 1;
                    }
                }

              (yyval.exval) = (yyvsp[-2].exval);
            }

          delete ev;
          ev = 0;
        }
#line 7402 "fe/idl.tab.cpp"
    break;

  case 373:
#line 4355 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrROSeen);
        }
#line 7410 "fe/idl.tab.cpp"
    break;

  case 374:
#line 4359 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 7418 "fe/idl.tab.cpp"
    break;

  case 375:
#line 4363 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 7426 "fe/idl.tab.cpp"
    break;

  case 376:
#line 4367 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 7434 "fe/idl.tab.cpp"
    break;

  case 377:
#line 4371 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Attribute *a = 0;
          FE_Declarator *d = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);

          /*
           * Create nodes representing attributes and add them to the
           * enclosing scope.
           */
          if (s != 0 && (yyvsp[-4].dcval) != 0 && (yyvsp[-2].dlval) != 0)
            {
              for (UTL_DecllistActiveIterator l ((yyvsp[-2].dlval)); !l.is_done (); l.next ())
                {
                  d = l.item ();

                  if (d == 0)
                    {
                      continue;
                    }

                  AST_Type *tp = d->compose ((yyvsp[-4].dcval));

                  if (tp == 0)
                    {
                      continue;
                    }

                  a = idl_global->gen ()->create_attribute (
                    true, tp, d->name (), s->is_local (), s->is_abstract ());

                  if ((yyvsp[0].nlval) != 0)
                    {
                      (void) a->fe_add_get_exceptions ((yyvsp[0].nlval));

                      (yyvsp[0].nlval)->destroy ();
                      delete (yyvsp[0].nlval);
                      (yyvsp[0].nlval) = 0;
                    }

                  (void) s->fe_add_attribute (a);
                }
            }

          (yyvsp[-2].dlval)->destroy ();
          delete (yyvsp[-2].dlval);
          (yyvsp[-2].dlval) = 0;

          (yyval.dcval) = a;
        }
#line 7490 "fe/idl.tab.cpp"
    break;

  case 378:
#line 4426 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 7498 "fe/idl.tab.cpp"
    break;

  case 379:
#line 4430 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 7506 "fe/idl.tab.cpp"
    break;

  case 380:
#line 4434 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 7514 "fe/idl.tab.cpp"
    break;

  case 381:
#line 4438 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseCompleted);
        }
#line 7522 "fe/idl.tab.cpp"
    break;

  case 382:
#line 4442 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Attribute *a = 0;
          FE_Declarator *d = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseCompleted);

          /*
           * Create nodes representing attributes and add them to the
           * enclosing scope.
           */
          if (s != 0 && (yyvsp[-6].dcval) != 0 && (yyvsp[-4].dlval) != 0)
            {
              for (UTL_DecllistActiveIterator l ((yyvsp[-4].dlval)); !l.is_done (); l.next ())
                {
                  d = l.item ();

                  if (d == 0)
                    {
                      continue;
                    }

                  AST_Type *tp = d->compose ((yyvsp[-6].dcval));

                  if (tp == 0)
                    {
                      continue;
                    }

                  a = idl_global->gen ()->create_attribute (
                   false, tp, d->name (), s->is_local (), s->is_abstract ());

                  if ((yyvsp[-2].nlval) != 0)
                    {
                      (void) a->fe_add_get_exceptions ((yyvsp[-2].nlval));

                      (yyvsp[-2].nlval)->destroy ();
                      delete (yyvsp[-2].nlval);
                      (yyvsp[-2].nlval) = 0;
                    }

                  if ((yyvsp[0].nlval) != 0)
                    {
                      (void) a->fe_add_set_exceptions ((yyvsp[0].nlval));

                      (yyvsp[0].nlval)->destroy ();
                      delete (yyvsp[0].nlval);
                      (yyvsp[0].nlval) = 0;
                    }

                  (void) s->fe_add_attribute (a);
                }
            }

          (yyvsp[-4].dlval)->destroy ();
          delete (yyvsp[-4].dlval);
          (yyvsp[-4].dlval) = 0;

          (yyval.dcval) = a;
        }
#line 7587 "fe/idl.tab.cpp"
    break;

  case 383:
#line 4506 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSeen);
        }
#line 7595 "fe/idl.tab.cpp"
    break;

  case 384:
#line 4510 "fe/idl.ypp"
        {
          Identifier *&id = (yyvsp[0].idval);
          UTL_Scope *scope = idl_global->scopes ().top_non_null ();
          UTL_ScopedName scoped_name (id, 0);
          AST_Exception *exception = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptIDSeen);

          /*
           * Create a node representing an exception and add it to
           * the enclosing scope.
           */
          if (scope)
            {
              exception = idl_global->gen ()->create_exception (
                &scoped_name,
                scope->is_local (),
                scope->is_abstract ());
              scope->fe_add_exception (exception);
            }

          /*
           * Push the exception scope on the scope stack.
           */
          idl_global->scopes ().push (exception);

          id->destroy ();
          delete id;
          id = 0;

          (yyval.dcval) = exception;
        }
#line 7631 "fe/idl.tab.cpp"
    break;

  case 385:
#line 4542 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSqSeen);
        }
#line 7639 "fe/idl.tab.cpp"
    break;

  case 386:
#line 4546 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptBodySeen);
        }
#line 7647 "fe/idl.tab.cpp"
    break;

  case 387:
#line 4550 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptQsSeen);
          /*
           * Done with this exception. Pop its scope from the scope stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 7661 "fe/idl.tab.cpp"
    break;

  case 388:
#line 4563 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 7669 "fe/idl.tab.cpp"
    break;

  case 389:
#line 4567 "fe/idl.ypp"
        {
          AST_Operation *op = 0;
          UTL_Scope *scope = idl_global->scopes ().top_non_null ();

          AST_Operation::Flags op_flags = (yyvsp[-3].ofval);
          AST_Decl *type_node = (yyvsp[-2].dcval);
          (yyvsp[-2].dcval) = 0;
          Identifier id ((yyvsp[0].strval));
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;

          UTL_ScopedName name (&id, 0);
          idl_global->set_parse_state (IDL_GlobalData::PS_OpIDSeen);

          /*
           * Create a node representing an operation on an interface
           * and add it to its enclosing scope.
           */
          if (scope != 0 && type_node != 0)
            {
              AST_Type *type = AST_Type::narrow_from_decl (type_node);

              if (type == 0)
                {
                  idl_global->err ()->not_a_type (type_node);
                }
              else if (type->node_type () == AST_Decl::NT_except)
                {
                  idl_global->err ()->not_a_type (type_node);
                }
              else
                {
                  AST_Decl *d = ScopeAsDecl (scope);
                  AST_Decl::NodeType nt = d->node_type ();
                  const bool local = scope->is_local ()
                    || nt == AST_Decl::NT_valuetype
                    || nt == AST_Decl::NT_eventtype;

                  op = idl_global->gen ()->create_operation (
                    type, op_flags, &name, local, scope->is_abstract ());

                  if (!local && type->is_local ())
                    {
                      idl_global->err ()->local_remote_mismatch (type, op);
                      op->destroy ();
                      delete op;
                      op = 0;
                    }
                  else
                    {
                      scope->fe_add_operation (op);
                    }
                }
            }

          /*
           * Push the operation scope onto the scopes stack.
           */
          idl_global->scopes ().push (op);
        }
#line 7734 "fe/idl.tab.cpp"
    break;

  case 390:
#line 4628 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 7742 "fe/idl.tab.cpp"
    break;

  case 391:
#line 4632 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);
        }
#line 7750 "fe/idl.tab.cpp"
    break;

  case 392:
#line 4636 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Operation *o = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpCompleted);

          /*
           * Add exceptions and context to the operation.
           */
          if (s != 0 && s->scope_node_type () == AST_Decl::NT_op)
            {
              o = AST_Operation::narrow_from_scope (s);

              if ((yyvsp[-2].nlval) != 0 && o != 0)
                {
                  (void) o->fe_add_exceptions ((yyvsp[-2].nlval));
                }

              if ((yyvsp[0].slval) != 0)
                {
                  (void) o->fe_add_context ((yyvsp[0].slval));
                }
            }

          /*
           * Done with this operation. Pop its scope from the scopes stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = o;
        }
#line 7785 "fe/idl.tab.cpp"
    break;

  case 393:
#line 4670 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_oneway;
        }
#line 7794 "fe/idl.tab.cpp"
    break;

  case 394:
#line 4675 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_idempotent;
        }
#line 7803 "fe/idl.tab.cpp"
    break;

  case 395:
#line 4680 "fe/idl.ypp"
        {
          (yyval.ofval) = AST_Operation::OP_noflags;
        }
#line 7811 "fe/idl.tab.cpp"
    break;

  case 397:
#line 4688 "fe/idl.ypp"
        {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                  AST_Expression::EV_void
                                                );
        }
#line 7822 "fe/idl.tab.cpp"
    break;

  case 398:
#line 4698 "fe/idl.ypp"
        {
          //@@ PS_FactorySeen?
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 7831 "fe/idl.tab.cpp"
    break;

  case 399:
#line 4703 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          if (s->is_abstract ())
            {
              //@@ Fire error
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("error in %C line %d:\n")
                          ACE_TEXT ("Abstract valuetype can't have a ")
                          ACE_TEXT ("factory construct.\n"),
                          idl_global->filename ()->get_string (),
                          idl_global->lineno ()));

              idl_global->set_err_count (idl_global->err_count () + 1);
            }

          Identifier id ((yyvsp[0].strval));
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;

          UTL_ScopedName n (&id,
                            0);
          AST_Factory *factory = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpIDSeen);

          /*
           * Create a node representing an factory construct
           * and add it to its enclosing scope
           */
          if (s != 0)
            {
              factory = idl_global->gen ()->create_factory (&n);
              (void) s->fe_add_factory (factory);
            }

          /*
           * Push the operation scope onto the scopes stack.
           */
          idl_global->scopes ().push (factory);

          (yyval.dcval) = factory;
        }
#line 7878 "fe/idl.tab.cpp"
    break;

  case 400:
#line 4746 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 7886 "fe/idl.tab.cpp"
    break;

  case 401:
#line 4750 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);

          if ((yyvsp[0].nlval))
            {
              UTL_Scope *s = idl_global->scopes ().top_non_null ();
              AST_Factory *f = AST_Factory::narrow_from_scope (s);
              (void) f->fe_add_exceptions ((yyvsp[0].nlval));
            }

          idl_global->scopes ().pop ();

          (yyval.dcval) = (yyvsp[-3].dcval);
        }
#line 7905 "fe/idl.tab.cpp"
    break;

  case 402:
#line 4768 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7913 "fe/idl.tab.cpp"
    break;

  case 403:
#line 4772 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7921 "fe/idl.tab.cpp"
    break;

  case 404:
#line 4776 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7929 "fe/idl.tab.cpp"
    break;

  case 405:
#line 4781 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7937 "fe/idl.tab.cpp"
    break;

  case 407:
#line 4791 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 7945 "fe/idl.tab.cpp"
    break;

  case 410:
#line 4800 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 7953 "fe/idl.tab.cpp"
    break;

  case 411:
#line 4804 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 7961 "fe/idl.tab.cpp"
    break;

  case 412:
#line 4808 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Argument *a = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDeclSeen);

          /*
           * Create a node representing an argument to an operation
           * Add it to the enclosing scope (the operation scope).
           */
          if ((yyvsp[-2].dcval) != 0
              && (yyvsp[0].deval) != 0 &&
              s != 0)
            {
              AST_Type *tp = (yyvsp[0].deval)->compose ((yyvsp[-2].dcval));

              if (tp != 0)
                {
                  a = idl_global->gen ()->create_argument (
                          AST_Argument::dir_IN,
                          tp,
                          (yyvsp[0].deval)->name ()
                        );

                  (void) s->fe_add_argument (a);
                }
            }

          (yyvsp[0].deval)->destroy ();
          delete (yyvsp[0].deval);
          (yyvsp[0].deval) = 0;
        }
#line 7997 "fe/idl.tab.cpp"
    break;

  case 413:
#line 4843 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 8005 "fe/idl.tab.cpp"
    break;

  case 414:
#line 4847 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 8013 "fe/idl.tab.cpp"
    break;

  case 415:
#line 4851 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 8021 "fe/idl.tab.cpp"
    break;

  case 416:
#line 4856 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 8029 "fe/idl.tab.cpp"
    break;

  case 418:
#line 4866 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 8037 "fe/idl.tab.cpp"
    break;

  case 421:
#line 4875 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 8045 "fe/idl.tab.cpp"
    break;

  case 422:
#line 4879 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 8053 "fe/idl.tab.cpp"
    break;

  case 423:
#line 4883 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Argument *a = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDeclSeen);

          /*
           * Create a node representing an argument to an operation
           * Add it to the enclosing scope (the operation scope).
           */
          if ((yyvsp[-2].dcval) != 0
              && (yyvsp[0].deval) != 0
              && s != 0)
            {
              AST_Type *tp = (yyvsp[0].deval)->compose ((yyvsp[-2].dcval));

              if (tp != 0)
                {
                  if (!s->is_local () && tp->is_local ())
                    {
                      idl_global->err ()->local_remote_mismatch (tp, s);
                    }
                  else
                    {
                      a =
                        idl_global->gen ()->create_argument (
                            (yyvsp[-4].dival),
                            tp,
                            (yyvsp[0].deval)->name ()
                          );
                      (void) s->fe_add_argument (a);
                    }
                }
            }

          (yyvsp[0].deval)->destroy ();
          delete (yyvsp[0].deval);
          (yyvsp[0].deval) = 0;
        }
#line 8096 "fe/idl.tab.cpp"
    break;

  case 424:
#line 4925 "fe/idl.ypp"
        {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 8107 "fe/idl.tab.cpp"
    break;

  case 427:
#line 4934 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = 0;
          UTL_ScopedName *n = (yyvsp[0].idlist);

          if (s != 0)
            {
              d = s->lookup_by_name (n, false, false);
            }

          if (d == 0)
            {
              idl_global->err ()->lookup_error (n);
              (yyvsp[0].idlist)->destroy ();
              (yyvsp[0].idlist) = 0;

              /* If we don't return here, we'll crash later.*/
              return 1;
            }
          else
            {
              d->last_referenced_as ((yyvsp[0].idlist));
              AST_Decl::NodeType nt = d->node_type ();
              AST_Type *t = AST_Type::narrow_from_decl (d);
              AST_Typedef *td = 0;
              bool can_be_undefined = false;

              if (nt == AST_Decl::NT_typedef)
                {
                  // This code block ensures that a sequence of
                  // as-yet-undefined struct or union isn't used
                  // as a return type or argument.
                  td = AST_Typedef::narrow_from_decl (d);
                  AST_Type *pbt = td->primitive_base_type ();

                  if (pbt->node_type () == AST_Decl::NT_sequence)
                    {
                      t = pbt;
                      AST_Sequence *seq_type =
                        AST_Sequence::narrow_from_decl (pbt);
                      AST_Type *elem_type =
                        seq_type->base_type ();
                      AST_Decl::NodeType elem_nt =
                        elem_type->node_type ();

                      if (elem_nt == AST_Decl::NT_typedef)
                        {
                          AST_Typedef *elem_td =
                            AST_Typedef::narrow_from_decl (elem_type);
                          elem_type = elem_td->primitive_base_type ();
                          elem_nt = elem_type->node_type ();
                        }

                      if (elem_nt == AST_Decl::NT_interface
                          || elem_nt == AST_Decl::NT_interface_fwd
                          || elem_nt == AST_Decl::NT_valuetype
                          || elem_nt == AST_Decl::NT_valuetype_fwd
                          || elem_nt == AST_Decl::NT_component
                          || elem_nt == AST_Decl::NT_component_fwd)
                        {
                          can_be_undefined = true;
                        }
                    }

                  if (! t->is_defined () && ! can_be_undefined)
                    {
                      idl_global->err ()->error1 (
                        UTL_Error::EIDL_ILLEGAL_ADD,
                        (nt == AST_Decl::NT_typedef ? td : t));

                      /* If we don't return here, we'll crash later.*/
                      return 1;
                    }
                }
              else
                {
                  // For forward declared structs and unions, we
                  // want the full definition, but we need to
                  // check that it's been fully defined.
                  AST_StructureFwd *fwd =
                    AST_StructureFwd::narrow_from_decl (d);

                  if (fwd != 0)
                    {
                      t = fwd->full_definition ();

                      if (! t->is_defined ())
                        {
                          idl_global->err ()->error1 (
                            UTL_Error::EIDL_ILLEGAL_ADD,
                            t);

                          /* If we don't return here, we'll crash later.*/
                          return 1;
                        }

                      d = t;
                    }
                }
            }

          (yyval.dcval) = d;
        }
#line 8215 "fe/idl.tab.cpp"
    break;

  case 428:
#line 5041 "fe/idl.ypp"
        {
          (yyval.dival) = AST_Argument::dir_IN;
        }
#line 8223 "fe/idl.tab.cpp"
    break;

  case 429:
#line 5045 "fe/idl.ypp"
        {
          (yyval.dival) = AST_Argument::dir_OUT;
        }
#line 8231 "fe/idl.tab.cpp"
    break;

  case 430:
#line 5049 "fe/idl.ypp"
        {
          (yyval.dival) = AST_Argument::dir_INOUT;
        }
#line 8239 "fe/idl.tab.cpp"
    break;

  case 431:
#line 5056 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSeen);
        }
#line 8247 "fe/idl.tab.cpp"
    break;

  case 432:
#line 5060 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSqSeen);
        }
#line 8255 "fe/idl.tab.cpp"
    break;

  case 433:
#line 5065 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 8264 "fe/idl.tab.cpp"
    break;

  case 434:
#line 5070 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 8272 "fe/idl.tab.cpp"
    break;

  case 435:
#line 5077 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSeen);
        }
#line 8280 "fe/idl.tab.cpp"
    break;

  case 436:
#line 5081 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSqSeen);
        }
#line 8288 "fe/idl.tab.cpp"
    break;

  case 437:
#line 5086 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 8297 "fe/idl.tab.cpp"
    break;

  case 438:
#line 5091 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 8305 "fe/idl.tab.cpp"
    break;

  case 439:
#line 5098 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSeen);
        }
#line 8313 "fe/idl.tab.cpp"
    break;

  case 440:
#line 5102 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSqSeen);
        }
#line 8321 "fe/idl.tab.cpp"
    break;

  case 441:
#line 5107 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 8330 "fe/idl.tab.cpp"
    break;

  case 442:
#line 5112 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 8338 "fe/idl.tab.cpp"
    break;

  case 443:
#line 5119 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSeen);
        }
#line 8346 "fe/idl.tab.cpp"
    break;

  case 444:
#line 5123 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSqSeen);
        }
#line 8354 "fe/idl.tab.cpp"
    break;

  case 445:
#line 5128 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextQsSeen);
          (yyval.slval) = (yyvsp[-1].slval);
        }
#line 8363 "fe/idl.tab.cpp"
    break;

  case 446:
#line 5133 "fe/idl.ypp"
        {
          (yyval.slval) = 0;
        }
#line 8371 "fe/idl.tab.cpp"
    break;

  case 447:
#line 5140 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.slval),
                          UTL_StrList ((yyvsp[-1].sval),
                                       (yyvsp[0].slval)),
                          1);
        }
#line 8382 "fe/idl.tab.cpp"
    break;

  case 448:
#line 5151 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextCommaSeen);
        }
#line 8390 "fe/idl.tab.cpp"
    break;

  case 449:
#line 5155 "fe/idl.ypp"
        {
          UTL_StrList *sl = 0;
          ACE_NEW_RETURN (sl,
                          UTL_StrList ((yyvsp[0].sval),
                                       0),
                          1);

          if ((yyvsp[-3].slval) == 0)
            {
              (yyval.slval) = sl;
            }
          else
            {
              (yyvsp[-3].slval)->nconc (sl);
              (yyval.slval) = (yyvsp[-3].slval);
            }
        }
#line 8412 "fe/idl.tab.cpp"
    break;

  case 450:
#line 5173 "fe/idl.ypp"
        {
          (yyval.slval) = 0;
        }
#line 8420 "fe/idl.tab.cpp"
    break;

  case 451:
#line 5180 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d =
            s->lookup_by_name ((yyvsp[-1].idlist));

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((yyvsp[-1].idlist));
            }
          else
            {
              d->set_id_with_typeid (
                     (yyvsp[0].sval)->get_string ()
                   );
            }

          (yyvsp[-1].idlist)->destroy ();
          delete (yyvsp[-1].idlist);
          (yyvsp[-1].idlist) = 0;

          (yyvsp[0].sval)->destroy ();
          delete (yyvsp[0].sval);
          (yyvsp[0].sval) = 0;

          (yyval.dcval) = 0;
        }
#line 8451 "fe/idl.tab.cpp"
    break;

  case 452:
#line 5210 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = ScopeAsDecl (s);

          // If we are in a module, we want to avoid a lookup unless the
          // typeprefix is to be applied to some other scope, since we
          // might get a previous opening of the module, and the prefix
          // of this opening would never get checked or set.
          if (d->name ()->compare ((yyvsp[-1].idlist)) != 0)
            {
              d =
                s->lookup_by_name ((yyvsp[-1].idlist));
            }

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((yyvsp[-1].idlist));
            }
          else
            {
              d->set_prefix_with_typeprefix (
                     (yyvsp[0].sval)->get_string ()
                   );
            }

          (yyvsp[-1].idlist)->destroy ();
          delete (yyvsp[-1].idlist);
          (yyvsp[-1].idlist) = 0;

          (yyvsp[0].sval)->destroy ();
          delete (yyvsp[0].sval);
          (yyvsp[0].sval) = 0;

          (yyval.dcval) = 0;
        }
#line 8491 "fe/idl.tab.cpp"
    break;

  case 455:
#line 5255 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
                            0);
          AST_ComponentFwd *f = 0;
          idl_global->set_parse_state (
                          IDL_GlobalData::PS_ComponentForwardSeen
                        );

          /*
           * Create a node representing a forward declaration of a
           * component. Store it in the enclosing scope.
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_component_fwd (&n);
              (void) s->fe_add_component_fwd (f);
            }

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;

          (yyval.dcval) = 0;
        }
#line 8521 "fe/idl.tab.cpp"
    break;

  case 456:
#line 5284 "fe/idl.ypp"
        {
          FE_ComponentHeader *&component_header = (yyvsp[0].chval);
          UTL_Scope *scope = idl_global->scopes ().top_non_null ();
          AST_Component *component = 0;

          /*
           * Make a new component node and add it to the enclosing scope.
           */
          if (scope && component_header)
            {
              component = idl_global->gen ()->create_component (
                component_header->name (),
                component_header->base_component (),
                component_header->supports (),
                component_header->n_supports (),
                component_header->supports_flat (),
                component_header->n_supports_flat ());
              AST_Interface *component_as_interface =
                dynamic_cast<AST_Interface*> (component);
              AST_Interface::fwd_redefinition_helper (
                component_as_interface, scope);

              /*
               * Add the component to its definition scope.
               */
              scope->fe_add_component (component);

              // This FE_ComponentHeader class isn't destroyed with the AST.
              component_header->destroy ();
              delete component_header;
              component_header = 0;
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (component);

          (yyval.dcval) = component;
        }
#line 8566 "fe/idl.tab.cpp"
    break;

  case 457:
#line 5325 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentSqSeen);
        }
#line 8574 "fe/idl.tab.cpp"
    break;

  case 458:
#line 5329 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentBodySeen);
        }
#line 8582 "fe/idl.tab.cpp"
    break;

  case 459:
#line 5333 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentQsSeen);

          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 8597 "fe/idl.tab.cpp"
    break;

  case 460:
#line 5348 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentIDSeen);
        }
#line 8605 "fe/idl.tab.cpp"
    break;

  case 461:
#line 5352 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 8613 "fe/idl.tab.cpp"
    break;

  case 462:
#line 5356 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);

          /*
           * Create an AST representation of the information in the header
           * part of a component.
           */
          UTL_ScopedName *n = 0;
          ACE_NEW_RETURN (n,
                          UTL_ScopedName ((yyvsp[-4].idval),
                                          0),
                          1);
          ACE_NEW_RETURN ((yyval.chval),
                          FE_ComponentHeader (n,
                                              (yyvsp[-2].idlist),
                                              (yyvsp[0].nlval),
                                              false),
                          1);

          if (0 != (yyvsp[0].nlval))
            {
              (yyvsp[0].nlval)->destroy ();
              delete (yyvsp[0].nlval);
              (yyvsp[0].nlval) = 0;
            }

          if (0 != (yyvsp[-2].idlist))
            {
              (yyvsp[-2].idlist)->destroy ();
              delete (yyvsp[-2].idlist);
              (yyvsp[-2].idlist) = 0;
            }
        }
#line 8651 "fe/idl.tab.cpp"
    break;

  case 463:
#line 5393 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 8659 "fe/idl.tab.cpp"
    break;

  case 464:
#line 5397 "fe/idl.ypp"
        {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8667 "fe/idl.tab.cpp"
    break;

  case 465:
#line 5401 "fe/idl.ypp"
        {
          (yyval.idlist) = 0;
        }
#line 8675 "fe/idl.tab.cpp"
    break;

  case 466:
#line 5408 "fe/idl.ypp"
        {
          AST_Annotation_Appls *&annotations = (yyvsp[-1].annotations_val);
          AST_Decl *&node = (yyvsp[0].dcval);
          if (node)
            {
              node->annotation_appls (*annotations);
            }
          else
            {
              idl_global->err ()-> unsupported_warning (
                "Annotating this is not supported");
            }
          delete annotations;
        }
#line 8694 "fe/idl.tab.cpp"
    break;

  case 469:
#line 5428 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 8702 "fe/idl.tab.cpp"
    break;

  case 470:
#line 5432 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8711 "fe/idl.tab.cpp"
    break;

  case 471:
#line 5437 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 8719 "fe/idl.tab.cpp"
    break;

  case 472:
#line 5441 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8728 "fe/idl.tab.cpp"
    break;

  case 473:
#line 5446 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EmitsDeclSeen);
        }
#line 8736 "fe/idl.tab.cpp"
    break;

  case 474:
#line 5450 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8745 "fe/idl.tab.cpp"
    break;

  case 475:
#line 5455 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PublishesDeclSeen);
        }
#line 8753 "fe/idl.tab.cpp"
    break;

  case 476:
#line 5459 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8762 "fe/idl.tab.cpp"
    break;

  case 477:
#line 5464 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConsumesDeclSeen);
        }
#line 8770 "fe/idl.tab.cpp"
    break;

  case 478:
#line 5468 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8779 "fe/idl.tab.cpp"
    break;

  case 479:
#line 5473 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 8787 "fe/idl.tab.cpp"
    break;

  case 480:
#line 5477 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8796 "fe/idl.tab.cpp"
    break;

  case 481:
#line 5482 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 8804 "fe/idl.tab.cpp"
    break;

  case 482:
#line 5486 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8813 "fe/idl.tab.cpp"
    break;

  case 483:
#line 5493 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;
          AST_Decl::NodeType nt = AST_Decl::NT_type;
          AST_Param_Holder *ph = 0;

          AST_Decl *d =
            s->lookup_by_name ((yyvsp[-1].idlist), true, false);

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((yyvsp[-1].idlist));
              so_far_so_good = false;
            }
          else
            {
              int compare = 0;
              nt = d->node_type ();

              switch (nt)
                {
                  case AST_Decl::NT_interface:
                    break;
                  case AST_Decl::NT_param_holder:
                    ph = AST_Param_Holder::narrow_from_decl (d);
                    nt = ph->info ()->type_;

                    if (nt != AST_Decl::NT_type
                       && nt != AST_Decl::NT_interface)
                      {
                        idl_global->err ()->mismatched_template_param (
                          ph->info ()->name_.c_str ());

                        so_far_so_good = false;
                      }

                    break;
                  case AST_Decl::NT_pre_defined:
                    // Nothing else but CORBA::Object can have
                    // this identifier.
                    compare =
                      ACE_OS::strcmp (d->local_name ()->get_string (),
                                      "Object");

                    // Simple provides port must use IDL interface
                    // or CORBA::Object.
                    if (compare != 0)
                      {
                        idl_global->err ()->interface_expected (d);
                        so_far_so_good = false;
                      }

                    break;
                  default:
                    idl_global->err ()->interface_expected (d);
                    so_far_so_good = false;
                    break;
                }
            }

          AST_Provides *provides = 0;

          if (so_far_so_good)
            {
              AST_Type *port_interface_type =
                AST_Type::narrow_from_decl (d);

              // Strip off _cxx_, if any, for port name.
              FE_Utils::original_local_name ((yyvsp[0].idval));

              UTL_ScopedName sn ((yyvsp[0].idval),
                                 0);

              provides = idl_global->gen ()->create_provides (
                &sn, port_interface_type);

              (void) s->fe_add_provides (provides);
            }

          (yyvsp[-1].idlist)->destroy ();
          delete (yyvsp[-1].idlist);
          (yyvsp[-1].idlist) = 0;

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;

          (yyval.dcval) = dynamic_cast<AST_Decl *> (provides);
        }
#line 8907 "fe/idl.tab.cpp"
    break;

  case 484:
#line 5586 "fe/idl.ypp"
        {
          // Lookups and checking are done where the 'interface_type'
          // token is used, in 'provides_decl' and 'uses_decl'.
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8917 "fe/idl.tab.cpp"
    break;

  case 485:
#line 5592 "fe/idl.ypp"
        {
          Identifier *corba_id = 0;

          ACE_NEW_RETURN (corba_id,
                          Identifier ("Object"),
                          1);

          UTL_IdList *conc_name = 0;
          ACE_NEW_RETURN (conc_name,
                          UTL_IdList (corba_id,
                                      0),
                          1);

          ACE_NEW_RETURN (corba_id,
                          Identifier ("CORBA"),
                          1);

          ACE_NEW_RETURN ((yyval.idlist),
                          UTL_IdList (corba_id,
                                      conc_name),
                          1);
        }
#line 8944 "fe/idl.tab.cpp"
    break;

  case 486:
#line 5617 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;
          AST_Decl::NodeType nt = AST_Decl::NT_type;
          AST_Param_Holder *ph = 0;

          AST_Decl *d =
            s->lookup_by_name ((yyvsp[-1].idlist), true, false);

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((yyvsp[-1].idlist));
              so_far_so_good = false;
            }
          else
            {
              int compare = 0;
              nt = d->node_type ();

              switch (nt)
                {
                  case AST_Decl::NT_interface:
                    break;
                  case AST_Decl::NT_param_holder:
                    ph = AST_Param_Holder::narrow_from_decl (d);
                    nt = ph->info ()->type_;

                    if (nt != AST_Decl::NT_type
                       && nt != AST_Decl::NT_interface)
                      {
                        idl_global->err ()->mismatched_template_param (
                          ph->info ()->name_.c_str ());

                        so_far_so_good = false;
                      }

                    break;
                  case AST_Decl::NT_pre_defined:
                    // Nothing else but CORBA::Object can have
                    // this identifier.
                    compare =
                      ACE_OS::strcmp (d->local_name ()->get_string (),
                                      "Object");

                    // Simple provides port must use IDL interface
                    // or CORBA::Object.
                    if (compare != 0)
                      {
                        idl_global->err ()->interface_expected (d);
                        so_far_so_good = false;
                      }

                    break;
                  default:
                    idl_global->err ()->interface_expected (d);
                    so_far_so_good = false;
                    break;
                }
            }

          AST_Uses *uses = 0;

          if (so_far_so_good)
            {
              AST_Type *port_interface_type =
                AST_Type::narrow_from_decl (d);

              // Strip off _cxx_, if any, for port name.
              FE_Utils::original_local_name ((yyvsp[0].idval));

              UTL_ScopedName sn ((yyvsp[0].idval),
                                 0);

              uses = idl_global->gen ()->create_uses (
                &sn, port_interface_type, (yyvsp[-2].bval));
              s->fe_add_uses (uses);

              AST_Component *c =
                AST_Component::narrow_from_scope (s);

              if (c != 0
                  && uses->is_multiple ()
                  && !idl_global->using_ifr_backend ()
                  && !idl_global->ignore_idl3 ()
                  && nt != AST_Decl::NT_param_holder)
                {
                  // These datatypes must be created in the
                  // front end so they can be looked up
                  // when compiling the generated executor IDL.
                  FE_Utils::create_uses_multiple_stuff (c, uses);
                }
            }

          (yyvsp[-1].idlist)->destroy ();
          delete (yyvsp[-1].idlist);
          (yyvsp[-1].idlist) = 0;

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;

          (yyval.dcval) = uses;
        }
#line 9052 "fe/idl.tab.cpp"
    break;

  case 487:
#line 5724 "fe/idl.ypp"
        {
          // We use this extra rule here to use in both uses_decl and
          // extended_uses_decl, so the LALR(1) parser can avoid conflicts.
          (yyval.bval) = (yyvsp[0].bval);
        }
#line 9062 "fe/idl.tab.cpp"
    break;

  case 488:
#line 5733 "fe/idl.ypp"
        {
          (yyval.bval) = true;
        }
#line 9070 "fe/idl.tab.cpp"
    break;

  case 489:
#line 5737 "fe/idl.ypp"
        {
          (yyval.bval) = false;
        }
#line 9078 "fe/idl.tab.cpp"
    break;

  case 490:
#line 5744 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;
          AST_Decl::NodeType nt = AST_Decl::NT_type;
          AST_Param_Holder *ph = 0;

          AST_Decl *d =
            s->lookup_by_name ((yyvsp[-1].idlist), true, false);

          if (0 == d)
            {
              idl_global->err ()->lookup_error ((yyvsp[-1].idlist));
              so_far_so_good = false;
            }
          else
            {
              nt = d->node_type ();

              switch (nt)
                {
                  case AST_Decl::NT_eventtype:
                    break;
                  case AST_Decl::NT_param_holder:
                    ph = AST_Param_Holder::narrow_from_decl (d);
                    nt = ph->info ()->type_;

                    if (nt != AST_Decl::NT_type
                       && nt != AST_Decl::NT_eventtype)
                      {
                        idl_global->err ()->mismatched_template_param (
                          ph->info ()->name_.c_str ());

                        so_far_so_good = false;
                      }

                    break;
                  default:
                    idl_global->err ()->eventtype_expected (d);
                    so_far_so_good = false;
                    break;
                }
            }

          AST_Emits *e = 0;

          if (so_far_so_good)
            {
              AST_Type *event_type =
                AST_Type::narrow_from_decl (d);

              // Strip off _cxx_, if any, for port name.
              FE_Utils::original_local_name ((yyvsp[0].idval));

              UTL_ScopedName sn ((yyvsp[0].idval),
                                 0);

              e = idl_global->gen ()->create_emits (&sn, event_type);

              (void) s->fe_add_emits (e);
            }

          (yyvsp[-1].idlist)->destroy ();
          delete (yyvsp[-1].idlist);
          (yyvsp[-1].idlist) = 0;

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;

          (yyval.dcval) = e;
        }
#line 9154 "fe/idl.tab.cpp"
    break;

  case 491:
#line 5819 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;
          AST_Decl::NodeType nt = AST_Decl::NT_type;
          AST_Param_Holder *ph = 0;

          AST_Decl *d =
            s->lookup_by_name ((yyvsp[-1].idlist), true, false);

          if (0 == d)
            {
              idl_global->err ()->lookup_error ((yyvsp[-1].idlist));
              so_far_so_good = false;
            }
          else
            {
              nt = d->node_type ();

              switch (nt)
                {
                  case AST_Decl::NT_eventtype:
                    break;
                  case AST_Decl::NT_param_holder:
                    ph = AST_Param_Holder::narrow_from_decl (d);
                    nt = ph->info ()->type_;

                    if (nt != AST_Decl::NT_type
                       && nt != AST_Decl::NT_eventtype)
                      {
                        idl_global->err ()->mismatched_template_param (
                          ph->info ()->name_.c_str ());

                        so_far_so_good = false;
                      }

                    break;
                  default:
                    idl_global->err ()->eventtype_expected (d);
                    so_far_so_good = false;
                    break;
                }
            }

          AST_Publishes *p = 0;

          if (so_far_so_good)
            {
              AST_Type *event_type =
                AST_Type::narrow_from_decl (d);

              // Strip off _cxx_, if any, for port name.
              FE_Utils::original_local_name ((yyvsp[0].idval));

              UTL_ScopedName sn ((yyvsp[0].idval), 0);
              p = idl_global->gen ()->create_publishes (&sn, event_type);
              s->fe_add_publishes (p);
            }

          (yyvsp[-1].idlist)->destroy ();
          delete (yyvsp[-1].idlist);
          (yyvsp[-1].idlist) = 0;

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;

          (yyval.dcval) = p;
        }
#line 9227 "fe/idl.tab.cpp"
    break;

  case 492:
#line 5891 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;
          AST_Decl::NodeType nt = AST_Decl::NT_type;
          AST_Param_Holder *ph = 0;

          AST_Decl *d =
            s->lookup_by_name ((yyvsp[-1].idlist), true, false);

          if (0 == d)
            {
              idl_global->err ()->lookup_error ((yyvsp[-1].idlist));
              so_far_so_good = false;
            }
          else
            {
              nt = d->node_type ();

              switch (nt)
                {
                  case AST_Decl::NT_eventtype:
                    break;
                  case AST_Decl::NT_param_holder:
                    ph = AST_Param_Holder::narrow_from_decl (d);
                    nt = ph->info ()->type_;

                    if (nt != AST_Decl::NT_type
                       && nt != AST_Decl::NT_eventtype)
                      {
                        idl_global->err ()->mismatched_template_param (
                          ph->info ()->name_.c_str ());

                        so_far_so_good = false;
                      }

                    break;
                  default:
                    idl_global->err ()->eventtype_expected (d);
                    so_far_so_good = false;
                    break;
                }
            }

          AST_Consumes *c = 0;

          if (so_far_so_good)
            {
              AST_Type *event_type =
                AST_Type::narrow_from_decl (d);

              // Strip off _cxx_, if any, for port name.
              FE_Utils::original_local_name ((yyvsp[0].idval));

              UTL_ScopedName sn ((yyvsp[0].idval),
                                 0);

              c = idl_global->gen ()->create_consumes (&sn, event_type);

              (void) s->fe_add_consumes (c);
            }

          (yyvsp[-1].idlist)->destroy ();
          delete (yyvsp[-1].idlist);
          (yyvsp[-1].idlist) = 0;

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;

          (yyval.dcval) = c;
       }
#line 9303 "fe/idl.tab.cpp"
    break;

  case 493:
#line 5966 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Home *h = 0;

          /*
           * Make a new home node and add it to the enclosing scope.
           */
          if (s != 0 && (yyvsp[0].hhval) != 0)
            {
              h =
                idl_global->gen ()->create_home (
                                        (yyvsp[0].hhval)->name (),
                                        (yyvsp[0].hhval)->base_home (),
                                        (yyvsp[0].hhval)->managed_component (),
                                        (yyvsp[0].hhval)->primary_key (),
                                        (yyvsp[0].hhval)->supports (),
                                        (yyvsp[0].hhval)->n_supports (),
                                        (yyvsp[0].hhval)->supports_flat (),
                                        (yyvsp[0].hhval)->n_supports_flat ()
                                      );
              /*
               * Add the home to its definition scope.
               */
              (void) s->fe_add_home (h);

              // This FE_HomeHeader class isn't destroyed with the AST.
              (yyvsp[0].hhval)->destroy ();
              delete (yyvsp[0].hhval);
              (yyvsp[0].hhval) = 0;
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (h);
        }
#line 9344 "fe/idl.tab.cpp"
    break;

  case 494:
#line 6003 "fe/idl.ypp"
        {
          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = 0;
        }
#line 9357 "fe/idl.tab.cpp"
    break;

  case 495:
#line 6015 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSeen);
        }
#line 9365 "fe/idl.tab.cpp"
    break;

  case 496:
#line 6019 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeIDSeen);
        }
#line 9373 "fe/idl.tab.cpp"
    break;

  case 497:
#line 6023 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 9381 "fe/idl.tab.cpp"
    break;

  case 498:
#line 6027 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);
        }
#line 9389 "fe/idl.tab.cpp"
    break;

  case 499:
#line 6031 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesSeen);
        }
#line 9397 "fe/idl.tab.cpp"
    break;

  case 500:
#line 6035 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesIDSeen);
        }
#line 9405 "fe/idl.tab.cpp"
    break;

  case 501:
#line 6039 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PrimaryKeySpecSeen);

          /*
           * Create an AST representation of the information in the header
           * part of a component home.
           */
          UTL_ScopedName *n = 0;
          ACE_NEW_RETURN (n,
                          UTL_ScopedName ((yyvsp[-10].idval), 0),
                          1);

          ACE_NEW_RETURN ((yyval.hhval),
                          FE_HomeHeader (n,
                                         (yyvsp[-8].idlist),
                                         (yyvsp[-6].nlval),
                                         (yyvsp[-2].idlist),
                                         (yyvsp[0].idlist)),
                          1);

          (yyvsp[-2].idlist)->destroy ();
          delete (yyvsp[-2].idlist);
          (yyvsp[-2].idlist) = 0;

          if (0 != (yyvsp[-8].idlist))
            {
              (yyvsp[-8].idlist)->destroy ();
              delete (yyvsp[-8].idlist);
              (yyvsp[-8].idlist) = 0;
            }

          if (0 != (yyvsp[0].idlist))
            {
              (yyvsp[0].idlist)->destroy ();
              delete (yyvsp[0].idlist);
              (yyvsp[0].idlist) = 0;
            }

          if (0 != (yyvsp[-6].nlval))
            {
              (yyvsp[-6].nlval)->destroy ();
              delete (yyvsp[-6].nlval);
              (yyvsp[-6].nlval) = 0;
            }
        }
#line 9455 "fe/idl.tab.cpp"
    break;

  case 502:
#line 6088 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 9463 "fe/idl.tab.cpp"
    break;

  case 503:
#line 6092 "fe/idl.ypp"
        {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 9471 "fe/idl.tab.cpp"
    break;

  case 504:
#line 6096 "fe/idl.ypp"
        {
          (yyval.idlist) = 0;
        }
#line 9479 "fe/idl.tab.cpp"
    break;

  case 505:
#line 6104 "fe/idl.ypp"
        {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 9487 "fe/idl.tab.cpp"
    break;

  case 506:
#line 6108 "fe/idl.ypp"
        {
          (yyval.idlist) = 0;
        }
#line 9495 "fe/idl.tab.cpp"
    break;

  case 507:
#line 6115 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSqSeen);
        }
#line 9503 "fe/idl.tab.cpp"
    break;

  case 508:
#line 6119 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeBodySeen);
        }
#line 9511 "fe/idl.tab.cpp"
    break;

  case 509:
#line 6123 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeQsSeen);
        }
#line 9519 "fe/idl.tab.cpp"
    break;

  case 513:
#line 6136 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_FactoryDeclSeen);
        }
#line 9527 "fe/idl.tab.cpp"
    break;

  case 514:
#line 6140 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 9535 "fe/idl.tab.cpp"
    break;

  case 515:
#line 6144 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_FinderDeclSeen);
        }
#line 9543 "fe/idl.tab.cpp"
    break;

  case 516:
#line 6148 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 9551 "fe/idl.tab.cpp"
    break;

  case 517:
#line 6156 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
                            0);
          idl_global->set_parse_state (IDL_GlobalData::PS_OpIDSeen);

          /*
           * Create a node representing a factory operation
           * and add it to the enclosing scope.
           */
          AST_Factory *f = idl_global->gen ()->create_factory (&n);
          (void) s->fe_add_factory (f);

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;

          /*
           * Push the factory scope onto the scopes stack.
           */
          idl_global->scopes ().push (f);
        }
#line 9578 "fe/idl.tab.cpp"
    break;

  case 518:
#line 6179 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 9586 "fe/idl.tab.cpp"
    break;

  case 519:
#line 6183 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);

          /*
           * Add exceptions and context to the factory.
           */
          if ((yyvsp[0].nlval) != 0)
            {
              (void) s->fe_add_exceptions ((yyvsp[0].nlval));
            }

          /*
           * Done with this factory. Pop its scope from the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 9608 "fe/idl.tab.cpp"
    break;

  case 520:
#line 6205 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
                            0);

          idl_global->set_parse_state (IDL_GlobalData::PS_OpIDSeen);

          /*
           * Create a node representing a home finder
           * and add it to the enclosing scope.
           */
          AST_Finder *f =
            idl_global->gen ()->create_finder (&n);

          (void) s->fe_add_finder (f);


          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;

          /*
           * Push the operation scope onto the scopes stack.
           */
          idl_global->scopes ().push (f);
        }
#line 9639 "fe/idl.tab.cpp"
    break;

  case 521:
#line 6232 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 9647 "fe/idl.tab.cpp"
    break;

  case 522:
#line 6236 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);

          /*
           * Add exceptions and context to the finder.
           */
          if ((yyvsp[0].nlval) != 0)
            {
              (void) s->fe_add_exceptions ((yyvsp[0].nlval));
            }

          /*
           * Done with this operation. Pop its scope from the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 9669 "fe/idl.tab.cpp"
    break;

  case 528:
#line 6269 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
                            0);
          AST_EventTypeFwd *f = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeForwardSeen);

          /*
           * Create a node representing a forward declaration of an
           * eventtype. Store it in the enclosing scope
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_eventtype_fwd (&n,
                                                            false);
              (void) s->fe_add_valuetype_fwd (f);
            }

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;

          (yyval.dcval) = 0;
        }
#line 9698 "fe/idl.tab.cpp"
    break;

  case 529:
#line 6299 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
                            0);
          AST_EventTypeFwd *f = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeForwardSeen);

          /*
           * Create a node representing a forward declaration of an
           * eventtype. Store it in the enclosing scope
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_eventtype_fwd (&n,
                                                            true);
              (void) s->fe_add_valuetype_fwd (f);
            }

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;

          (yyval.dcval) = 0;
        }
#line 9727 "fe/idl.tab.cpp"
    break;

  case 530:
#line 6328 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_EventType *e = 0;
          AST_Interface *i = 0;

          if (s != 0 && (yyvsp[-1].idval) != 0)
            {
              UTL_ScopedName sn ((yyvsp[-1].idval),
                                 0);
              e =
                idl_global->gen ()->create_eventtype (
                    &sn,
                    (yyvsp[0].vhval)->inherits (),
                    (yyvsp[0].vhval)->n_inherits (),
                    (yyvsp[0].vhval)->inherits_concrete (),
                    (yyvsp[0].vhval)->inherits_flat (),
                    (yyvsp[0].vhval)->n_inherits_flat (),
                    (yyvsp[0].vhval)->supports (),
                    (yyvsp[0].vhval)->n_supports (),
                    (yyvsp[0].vhval)->supports_concrete (),
                    true,
                    false,
                    false
                  );
              i = AST_Interface::narrow_from_decl (e);
              AST_Interface::fwd_redefinition_helper (i,
                                                      s);
              /*
               * Add the eventetype to its definition scope
               */
              e = AST_EventType::narrow_from_decl (i);
              (void) s->fe_add_eventtype (e);
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (e);

          (yyvsp[-1].idval)->destroy ();
          delete (yyvsp[-1].idval);
          (yyvsp[-1].idval) = 0;
        }
#line 9775 "fe/idl.tab.cpp"
    break;

  case 531:
#line 6372 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 9783 "fe/idl.tab.cpp"
    break;

  case 532:
#line 6376 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 9791 "fe/idl.tab.cpp"
    break;

  case 533:
#line 6380 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = 0;
        }
#line 9806 "fe/idl.tab.cpp"
    break;

  case 534:
#line 6396 "fe/idl.ypp"
        {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9814 "fe/idl.tab.cpp"
    break;

  case 535:
#line 6405 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("error in %C line %d\n"),
                      idl_global->filename ()->get_string (),
                      idl_global->lineno ()));
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Sorry, I (TAO_IDL) can't handle")
                      ACE_TEXT (" custom yet\n")));
          (yyval.idval) = 0;
        }
#line 9831 "fe/idl.tab.cpp"
    break;

  case 536:
#line 6422 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9841 "fe/idl.tab.cpp"
    break;

  case 537:
#line 6431 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 9849 "fe/idl.tab.cpp"
    break;

  case 538:
#line 6435 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);

          ACE_NEW_RETURN ((yyval.vhval),
                          FE_OBVHeader (
                            0,
                            (yyvsp[-2].nlval),
                            (yyvsp[0].nlval),
                            (yyvsp[-2].nlval)
                              ? (yyvsp[-2].nlval)->truncatable ()
                              : false,
                            true),
                          1);

          if (0 != (yyvsp[0].nlval))
            {
              (yyvsp[0].nlval)->destroy ();
              delete (yyvsp[0].nlval);
              (yyvsp[0].nlval) = 0;
            }

          if (0 != (yyvsp[-2].nlval))
            {
              (yyvsp[-2].nlval)->destroy ();
              delete (yyvsp[-2].nlval);
              (yyvsp[-2].nlval) = 0;
            }
        }
#line 9882 "fe/idl.tab.cpp"
    break;

  case 539:
#line 6468 "fe/idl.ypp"
        {
          UTL_Scope *scope = idl_global->scopes ().top_non_null ();
          Identifier *&event_id = (yyvsp[-1].idval);
          FE_OBVHeader *&event_header = (yyvsp[0].vhval);
          AST_EventType *eventtype = 0;

          if (scope && event_header)
            {
              // We create the scoped name here instead of with the
              // FE_OBVHeader because there is a token for it only here.
              UTL_ScopedName sn (event_id, 0);
              eventtype = idl_global->gen ()->create_eventtype (
                &sn,
                event_header->inherits (),
                event_header->n_inherits (),
                event_header->inherits_concrete (),
                event_header->inherits_flat (),
                event_header->n_inherits_flat (),
                event_header->supports (),
                event_header->n_supports (),
                event_header->supports_concrete (),
                false,
                event_header->truncatable (),
                false);
              AST_Interface *eventtype_as_interface =
                dynamic_cast<AST_Interface *> (eventtype);
              AST_Interface::fwd_redefinition_helper (
                eventtype_as_interface, scope);

              /*
               * Add the eventetype to its definition scope
               */
              scope->fe_add_eventtype (eventtype);

              // FE_OBVHeader is not automatically destroyed in the AST
              event_header->destroy ();
              delete event_header;
              event_header = 0;

              sn.destroy ();
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (eventtype);

          (yyval.dcval) = eventtype;
        }
#line 9936 "fe/idl.tab.cpp"
    break;

  case 540:
#line 6518 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 9944 "fe/idl.tab.cpp"
    break;

  case 541:
#line 6522 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 9952 "fe/idl.tab.cpp"
    break;

  case 542:
#line 6526 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 9967 "fe/idl.tab.cpp"
    break;

  case 543:
#line 6540 "fe/idl.ypp"
        {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9975 "fe/idl.tab.cpp"
    break;

  case 544:
#line 6544 "fe/idl.ypp"
        {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9983 "fe/idl.tab.cpp"
    break;

  case 545:
#line 6551 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_type;
        }
#line 9991 "fe/idl.tab.cpp"
    break;

  case 546:
#line 6555 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_struct;
        }
#line 9999 "fe/idl.tab.cpp"
    break;

  case 547:
#line 6559 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_union;
        }
#line 10007 "fe/idl.tab.cpp"
    break;

  case 548:
#line 6563 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_eventtype;
        }
#line 10015 "fe/idl.tab.cpp"
    break;

  case 549:
#line 6567 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_sequence;
        }
#line 10023 "fe/idl.tab.cpp"
    break;

  case 550:
#line 6571 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_interface;
        }
#line 10031 "fe/idl.tab.cpp"
    break;

  case 551:
#line 6575 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_valuetype;
        }
#line 10039 "fe/idl.tab.cpp"
    break;

  case 552:
#line 6579 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_enum;
        }
#line 10047 "fe/idl.tab.cpp"
    break;

  case 553:
#line 6583 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_except;
        }
#line 10055 "fe/idl.tab.cpp"
    break;

  case 554:
#line 6587 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_const;
          t_param_const_type = (yyvsp[0].etval);
        }
#line 10064 "fe/idl.tab.cpp"
    break;

  case 555:
#line 6595 "fe/idl.ypp"
        {
          if ((yyvsp[0].plval) == 0)
            {
              ACE_NEW_RETURN ((yyvsp[0].plval), FE_Utils::T_PARAMLIST_INFO, 1);
            }

          (yyvsp[0].plval)->enqueue_head (*(yyvsp[-1].pival));
          delete (yyvsp[-1].pival);
          (yyvsp[-1].pival) = 0;

          // The param added above is always the last one parsed,
          // so we check for matches between sequence<T> & T here.
          ACE_CString bad_id =
            FE_Utils::check_for_seq_of_param (
              (yyvsp[0].plval));

          if (!bad_id.empty ())
            {
              delete (yyvsp[0].plval);
              (yyvsp[0].plval) = 0;

              idl_global->err ()->mismatch_seq_of_param (bad_id.c_str ());
              return 1;
            }

          (yyval.plval) = (yyvsp[0].plval);
        }
#line 10096 "fe/idl.tab.cpp"
    break;

  case 556:
#line 6626 "fe/idl.ypp"
        {
          if ((yyvsp[-2].plval) == 0)
            {
              ACE_NEW_RETURN ((yyvsp[-2].plval), FE_Utils::T_PARAMLIST_INFO, 1);
            }

          (yyvsp[-2].plval)->enqueue_tail (*(yyvsp[0].pival));
          (yyval.plval) = (yyvsp[-2].plval);

          delete (yyvsp[0].pival);
          (yyvsp[0].pival) = 0;
        }
#line 10113 "fe/idl.tab.cpp"
    break;

  case 557:
#line 6639 "fe/idl.ypp"
        {
          (yyval.plval) = 0;
        }
#line 10121 "fe/idl.tab.cpp"
    break;

  case 558:
#line 6646 "fe/idl.ypp"
        {

          ACE_NEW_RETURN ((yyval.pival),
                          FE_Utils::T_Param_Info,
                          1);

          AST_Decl::NodeType nt = (yyvsp[-1].ntval);

          (yyval.pival)->type_ = nt;
          (yyval.pival)->name_ = (yyvsp[0].strval);
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;

          if (nt == AST_Decl::NT_const)
            {
              (yyval.pival)->const_type_ = t_param_const_type;
              (yyval.pival)->enum_const_type_decl_ =
                tao_enum_constant_decl;

              // Reset these values.
              t_param_const_type = AST_Expression::EV_none;
              tao_enum_constant_decl = 0;
            }
        }
#line 10150 "fe/idl.tab.cpp"
    break;

  case 559:
#line 6671 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.pival),
                          FE_Utils::T_Param_Info,
                          1);

          (yyval.pival)->type_ = AST_Decl::NT_sequence;
          (yyval.pival)->seq_param_ref_  = (yyvsp[-2].strval);
          (yyval.pival)->name_ += (yyvsp[0].strval);

          ACE::strdelete ((yyvsp[-2].strval));
          (yyvsp[-2].strval) = 0;
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 10169 "fe/idl.tab.cpp"
    break;

  case 560:
#line 6689 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.slval), UTL_StrList ((yyvsp[-1].sval), (yyvsp[0].slval)), 1);
        }
#line 10177 "fe/idl.tab.cpp"
    break;

  case 561:
#line 6696 "fe/idl.ypp"
        {
          UTL_StrList *sl = 0;
          ACE_NEW_RETURN (sl, UTL_StrList ((yyvsp[0].sval), 0), 1);

          if ((yyvsp[-2].slval) == 0)
            {
              (yyval.slval) = sl;
            }
          else
            {
              (yyvsp[-2].slval)->nconc (sl);
              (yyval.slval) = (yyvsp[-2].slval);
            }
        }
#line 10196 "fe/idl.tab.cpp"
    break;

  case 562:
#line 6711 "fe/idl.ypp"
        {
          (yyval.slval) = 0;
        }
#line 10204 "fe/idl.tab.cpp"
    break;

  case 563:
#line 6718 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.sval),
                          UTL_String ((yyvsp[0].strval), true),
                          1);
        }
#line 10214 "fe/idl.tab.cpp"
    break;

  case 564:
#line 6727 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSeen);
        }
#line 10222 "fe/idl.tab.cpp"
    break;

  case 565:
#line 6731 "fe/idl.ypp"
        {
          char *&id_value = (yyvsp[0].strval);
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeIDSeen);
          UTL_Scope *scope = idl_global->scopes ().top_non_null ();

          Identifier id (id_value);
          ACE::strdelete (id_value);
          id_value = 0;

          UTL_ScopedName scoped_name (&id, 0);
          AST_PortType *porttype = idl_global->gen ()->create_porttype (
            &scoped_name);
          scope->fe_add_porttype (porttype);
          (yyval.dcval) = porttype;

          // Push it on the scopes stack.
          idl_global->scopes ().push (porttype);
        }
#line 10245 "fe/idl.tab.cpp"
    break;

  case 566:
#line 6750 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSqSeen);
        }
#line 10253 "fe/idl.tab.cpp"
    break;

  case 567:
#line 6758 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeBodySeen);
        }
#line 10261 "fe/idl.tab.cpp"
    break;

  case 568:
#line 6762 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeQsSeen);

          // Done with this port type - pop it off the scopes stack.
          idl_global->scopes ().pop ();

          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 10274 "fe/idl.tab.cpp"
    break;

  case 569:
#line 6774 "fe/idl.ypp"
        {
          AST_Annotation_Appls *&annotations = (yyvsp[-1].annotations_val);
          AST_Decl *&node = (yyvsp[0].dcval);
          if (node)
            {
              node->annotation_appls (*annotations);
            }
          else
            {
              idl_global->err ()-> unsupported_warning (
                "Annotating this is not supported");
            }
          delete annotations;
        }
#line 10293 "fe/idl.tab.cpp"
    break;

  case 575:
#line 6800 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 10301 "fe/idl.tab.cpp"
    break;

  case 576:
#line 6804 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 10309 "fe/idl.tab.cpp"
    break;

  case 577:
#line 6811 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d =
            s->lookup_by_name ((yyvsp[-1].idlist));
          AST_PortType *pt = 0;
          bool so_far_so_good = true;

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((yyvsp[-1].idlist));
              so_far_so_good = false;
            }
          else
            {
              pt = AST_PortType::narrow_from_decl (d);

              if (pt == 0)
                {
                  idl_global->err ()->error1 (UTL_Error::EIDL_PORTTYPE_EXPECTED,
                                              d);
                  so_far_so_good = false;
                }
            }

          AST_Extended_Port *ep = 0;

          if (so_far_so_good)
            {
              Identifier id ((yyvsp[0].strval));
              ACE::strdelete ((yyvsp[0].strval));
              (yyvsp[0].strval) = 0;

              UTL_ScopedName sn (&id, 0);
              ep = idl_global->gen ()->create_extended_port (&sn, pt);
              s->fe_add_extended_port (ep);

              // Create (in the AST) the struct(s) and sequence(s)
              // needed for multiplex uses ports, if any.
              for (UTL_ScopeActiveIterator i (pt, UTL_Scope::IK_decls);
                   !i.is_done ();
                   i.next ())
                {
                  d = i.item ();

                  AST_Uses *u = AST_Uses::narrow_from_decl (d);

                  if (u != 0 && u->is_multiple ())
                    {
                      AST_Component *c =
                        AST_Component::narrow_from_scope (s);

                      FE_Utils::create_uses_multiple_stuff (
                        c,
                        u,
                        id.get_string ());
                    }
                }
            }

          (yyvsp[-1].idlist)->destroy ();
          delete (yyvsp[-1].idlist);
          (yyvsp[-1].idlist) = 0;

          (yyval.dcval) = ep;
        }
#line 10380 "fe/idl.tab.cpp"
    break;

  case 578:
#line 6878 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_MirrorPortDeclSeen);
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d =
            s->lookup_by_name ((yyvsp[-1].idlist));
          AST_PortType *pt = 0;
          bool so_far_so_good = true;

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((yyvsp[-1].idlist));
              so_far_so_good = false;
            }
           else
             {
               pt = AST_PortType::narrow_from_decl (d);

               if (pt == 0)
                 {
                   idl_global->err ()->error1 (UTL_Error::EIDL_PORTTYPE_EXPECTED,
                                               d);
                   so_far_so_good = false;
                 }
             }

          AST_Mirror_Port *mp = 0;

          if (so_far_so_good)
            {
              Identifier id ((yyvsp[0].strval));
              ACE::strdelete ((yyvsp[0].strval));
              (yyvsp[0].strval) = 0;

              UTL_ScopedName sn (&id, 0);
              mp = idl_global->gen ()->create_mirror_port (&sn, pt);
              s->fe_add_mirror_port (mp);
            }

          (yyvsp[-1].idlist)->destroy ();
          delete (yyvsp[-1].idlist);
          (yyvsp[-1].idlist) = 0;

          (yyval.dcval) = mp;
        }
#line 10429 "fe/idl.tab.cpp"
    break;

  case 579:
#line 6926 "fe/idl.ypp"
        {
          if ((yyvsp[0].alval) == 0)
            {
              ACE_NEW_RETURN ((yyvsp[0].alval),
                              FE_Utils::T_ARGLIST,
                              1);
            }

          delete (yyvsp[-2].annotations_val);
          (yyvsp[0].alval)->enqueue_head ((yyvsp[-1].dcval));
          (yyval.alval) = (yyvsp[0].alval);
        }
#line 10446 "fe/idl.tab.cpp"
    break;

  case 580:
#line 6942 "fe/idl.ypp"
        {
          if ((yyvsp[-3].alval) == 0)
            {
              ACE_NEW_RETURN ((yyvsp[-3].alval),
                              FE_Utils::T_ARGLIST,
                              1);
            }

          delete (yyvsp[-1].annotations_val);
          (yyvsp[-3].alval)->enqueue_tail ((yyvsp[0].dcval));
          (yyval.alval) = (yyvsp[-3].alval);
        }
#line 10463 "fe/idl.tab.cpp"
    break;

  case 581:
#line 6955 "fe/idl.ypp"
        {
          (yyval.alval) = 0;
        }
#line 10471 "fe/idl.tab.cpp"
    break;

  case 582:
#line 6962 "fe/idl.ypp"
        {
          // To avoid grammar conflicts with this LALR(1) parser,
          // we take advantage of the fact that an expression can
          // be a scoped name. At that lower level, we create an
          // expression containing the scoped name, and at a
          // higher lever, deduce that it's not supposed to be
          // a constant and look up the type to add to the template
          // arg list.
          AST_Expression *ex = (yyvsp[0].exval);
          UTL_ScopedName *sn = ex->n ();
          AST_Decl *d = 0;
          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          if (sn != 0)
            {
              d = s->lookup_by_name (sn);

              if (d == 0)
                {
                  idl_global->err ()->lookup_error (sn);
                  return 1;
                }
              else
                {
                  AST_Decl::NodeType nt = d->node_type ();

                  if (nt == AST_Decl::NT_enum_val)
                    {
                      (yyvsp[0].exval)->evaluate (
                        AST_Expression::EK_const);

                      (yyval.dcval) =
                        idl_global->gen ()->create_constant (
                          (yyvsp[0].exval)->ev ()->et,
                          (yyvsp[0].exval),
                          sn);
                    }
                  else
                    {
                      (yyval.dcval) = d;
                    }
                }
            }
          else
            {
              (yyvsp[0].exval)->evaluate (
                AST_Expression::EK_const);

              (yyval.dcval) =
                idl_global->gen ()->create_constant (
                  (yyvsp[0].exval)->ev ()->et,
                  (yyvsp[0].exval),
                  0);
            }
        }
#line 10531 "fe/idl.tab.cpp"
    break;

  case 583:
#line 7021 "fe/idl.ypp"
        {
          (yyval.dcval) = 0;
        }
#line 10539 "fe/idl.tab.cpp"
    break;

  case 584:
#line 7028 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSeen);
        }
#line 10547 "fe/idl.tab.cpp"
    break;

  case 585:
#line 7032 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorIDSeen);
        }
#line 10555 "fe/idl.tab.cpp"
    break;

  case 586:
#line 7036 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Connector *parent = 0;
          bool so_far_so_good = true;

          Identifier id ((yyvsp[-2].strval));
          ACE::strdelete ((yyvsp[-2].strval));
          (yyvsp[-2].strval) = 0;

          UTL_ScopedName sn (&id, 0);

          if ((yyvsp[0].idlist) != 0)
            {
              AST_Decl *d =
                s->lookup_by_name ((yyvsp[0].idlist));

              if (d == 0)
                {
                  idl_global->err ()->lookup_error ((yyvsp[0].idlist));
                  so_far_so_good = false;
                }

              parent =
                AST_Connector::narrow_from_decl (d);

              if (parent == 0)
                {
                  idl_global->err ()->error1 (
                    UTL_Error::EIDL_CONNECTOR_EXPECTED,
                    d);

                  so_far_so_good = false;
                }

              (yyvsp[0].idlist)->destroy ();
              delete (yyvsp[0].idlist);
              (yyvsp[0].idlist) = 0;
            }

          if (so_far_so_good)
            {
              AST_Connector *c =
                idl_global->gen ()->create_connector (&sn,
                                                      parent);

              (void) s->fe_add_connector (c);

              // Push it on the scopes stack.
              idl_global->scopes ().push (c);
           }

          delete (yyvsp[-3].annotations_val);
        }
#line 10613 "fe/idl.tab.cpp"
    break;

  case 587:
#line 7093 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSqSeen);
        }
#line 10621 "fe/idl.tab.cpp"
    break;

  case 588:
#line 7097 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorBodySeen);
        }
#line 10629 "fe/idl.tab.cpp"
    break;

  case 589:
#line 7101 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorQsSeen);

          // Done with this connector - pop it off the scope stack.
          idl_global->scopes ().pop ();
        }
#line 10640 "fe/idl.tab.cpp"
    break;

  case 592:
#line 7116 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 10648 "fe/idl.tab.cpp"
    break;

  case 593:
#line 7120 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10656 "fe/idl.tab.cpp"
    break;

  case 594:
#line 7124 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 10664 "fe/idl.tab.cpp"
    break;

  case 595:
#line 7128 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10672 "fe/idl.tab.cpp"
    break;

  case 596:
#line 7132 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 10680 "fe/idl.tab.cpp"
    break;

  case 597:
#line 7136 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10688 "fe/idl.tab.cpp"
    break;

  case 598:
#line 7140 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 10696 "fe/idl.tab.cpp"
    break;

  case 599:
#line 7144 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10704 "fe/idl.tab.cpp"
    break;


#line 10708 "fe/idl.tab.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 7149 "fe/idl.ypp"

/* programs */

/*
 * ???
 */
int
tao_yywrap (void)
{
  return 1;
}

/*
 * Report an error situation discovered in a production
 */
void
tao_yyerror (const char *msg)
{
  ACE_ERROR ((LM_ERROR, "%C\n", msg));
}
