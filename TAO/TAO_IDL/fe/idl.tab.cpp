/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

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
#include <ast_map.h>

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

// Avoid a Visual C++ warning due to unreachable code generated by bison
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4702)
#endif


#line 173 "fe/idl.tab.cpp"

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

#include "idl.tab.hpp"
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
  YYSYMBOL_IDL_INT8 = 81,                  /* IDL_INT8  */
  YYSYMBOL_IDL_UINT8 = 82,                 /* IDL_UINT8  */
  YYSYMBOL_IDL_INT16 = 83,                 /* IDL_INT16  */
  YYSYMBOL_IDL_UINT16 = 84,                /* IDL_UINT16  */
  YYSYMBOL_IDL_INT32 = 85,                 /* IDL_INT32  */
  YYSYMBOL_IDL_UINT32 = 86,                /* IDL_UINT32  */
  YYSYMBOL_IDL_INT64 = 87,                 /* IDL_INT64  */
  YYSYMBOL_IDL_UINT64 = 88,                /* IDL_UINT64  */
  YYSYMBOL_IDL_SCOPE_DELIMITOR = 89,       /* IDL_SCOPE_DELIMITOR  */
  YYSYMBOL_IDL_LEFT_SHIFT = 90,            /* IDL_LEFT_SHIFT  */
  YYSYMBOL_IDL_RIGHT_SHIFT = 91,           /* IDL_RIGHT_SHIFT  */
  YYSYMBOL_IDL_WCHAR_LITERAL = 92,         /* IDL_WCHAR_LITERAL  */
  YYSYMBOL_IDL_WSTRING_LITERAL = 93,       /* IDL_WSTRING_LITERAL  */
  YYSYMBOL_IDL_ANNOTATION_DECL = 94,       /* IDL_ANNOTATION_DECL  */
  YYSYMBOL_IDL_ANNOTATION_SYMBOL = 95,     /* IDL_ANNOTATION_SYMBOL  */
  YYSYMBOL_IDL_BITFIELD = 96,              /* IDL_BITFIELD  */
  YYSYMBOL_IDL_BITMASK = 97,               /* IDL_BITMASK  */
  YYSYMBOL_IDL_BITSET = 98,                /* IDL_BITSET  */
  YYSYMBOL_IDL_MAP = 99,                   /* IDL_MAP  */
  YYSYMBOL_100_ = 100,                     /* ';'  */
  YYSYMBOL_101_ = 101,                     /* '{'  */
  YYSYMBOL_102_ = 102,                     /* '}'  */
  YYSYMBOL_103_ = 103,                     /* '<'  */
  YYSYMBOL_104_ = 104,                     /* '>'  */
  YYSYMBOL_105_ = 105,                     /* ':'  */
  YYSYMBOL_106_ = 106,                     /* ','  */
  YYSYMBOL_107_ = 107,                     /* '='  */
  YYSYMBOL_108_ = 108,                     /* '|'  */
  YYSYMBOL_109_ = 109,                     /* '^'  */
  YYSYMBOL_110_ = 110,                     /* '&'  */
  YYSYMBOL_111_ = 111,                     /* '+'  */
  YYSYMBOL_112_ = 112,                     /* '-'  */
  YYSYMBOL_113_ = 113,                     /* '*'  */
  YYSYMBOL_114_ = 114,                     /* '/'  */
  YYSYMBOL_115_ = 115,                     /* '%'  */
  YYSYMBOL_116_ = 116,                     /* '~'  */
  YYSYMBOL_117_ = 117,                     /* '('  */
  YYSYMBOL_118_ = 118,                     /* ')'  */
  YYSYMBOL_119_ = 119,                     /* '['  */
  YYSYMBOL_120_ = 120,                     /* ']'  */
  YYSYMBOL_YYACCEPT = 121,                 /* $accept  */
  YYSYMBOL_start = 122,                    /* start  */
  YYSYMBOL_definitions = 123,              /* definitions  */
  YYSYMBOL_at_least_one_definition = 124,  /* at_least_one_definition  */
  YYSYMBOL_definition = 125,               /* definition  */
  YYSYMBOL_fixed_definition = 126,         /* fixed_definition  */
  YYSYMBOL_127_1 = 127,                    /* $@1  */
  YYSYMBOL_128_2 = 128,                    /* $@2  */
  YYSYMBOL_129_3 = 129,                    /* $@3  */
  YYSYMBOL_130_4 = 130,                    /* $@4  */
  YYSYMBOL_131_5 = 131,                    /* $@5  */
  YYSYMBOL_132_6 = 132,                    /* $@6  */
  YYSYMBOL_133_7 = 133,                    /* $@7  */
  YYSYMBOL_134_8 = 134,                    /* $@8  */
  YYSYMBOL_135_9 = 135,                    /* $@9  */
  YYSYMBOL_136_10 = 136,                   /* $@10  */
  YYSYMBOL_137_11 = 137,                   /* $@11  */
  YYSYMBOL_138_12 = 138,                   /* $@12  */
  YYSYMBOL_139_13 = 139,                   /* $@13  */
  YYSYMBOL_140_14 = 140,                   /* $@14  */
  YYSYMBOL_141_15 = 141,                   /* $@15  */
  YYSYMBOL_module_header = 142,            /* module_header  */
  YYSYMBOL_143_16 = 143,                   /* $@16  */
  YYSYMBOL_module = 144,                   /* module  */
  YYSYMBOL_145_17 = 145,                   /* @17  */
  YYSYMBOL_146_18 = 146,                   /* $@18  */
  YYSYMBOL_147_19 = 147,                   /* $@19  */
  YYSYMBOL_template_module_header = 148,   /* template_module_header  */
  YYSYMBOL_template_module = 149,          /* template_module  */
  YYSYMBOL_150_20 = 150,                   /* $@20  */
  YYSYMBOL_151_21 = 151,                   /* $@21  */
  YYSYMBOL_152_22 = 152,                   /* $@22  */
  YYSYMBOL_153_23 = 153,                   /* $@23  */
  YYSYMBOL_154_24 = 154,                   /* $@24  */
  YYSYMBOL_at_least_one_tpl_definition = 155, /* at_least_one_tpl_definition  */
  YYSYMBOL_tpl_definitions = 156,          /* tpl_definitions  */
  YYSYMBOL_tpl_definition = 157,           /* tpl_definition  */
  YYSYMBOL_template_module_ref = 158,      /* template_module_ref  */
  YYSYMBOL_159_25 = 159,                   /* $@25  */
  YYSYMBOL_160_26 = 160,                   /* $@26  */
  YYSYMBOL_template_module_inst = 161,     /* template_module_inst  */
  YYSYMBOL_162_27 = 162,                   /* $@27  */
  YYSYMBOL_163_28 = 163,                   /* $@28  */
  YYSYMBOL_interface_def = 164,            /* interface_def  */
  YYSYMBOL_interface = 165,                /* interface  */
  YYSYMBOL_166_29 = 166,                   /* $@29  */
  YYSYMBOL_167_30 = 167,                   /* $@30  */
  YYSYMBOL_168_31 = 168,                   /* $@31  */
  YYSYMBOL_interface_decl = 169,           /* interface_decl  */
  YYSYMBOL_170_32 = 170,                   /* $@32  */
  YYSYMBOL_interface_header = 171,         /* interface_header  */
  YYSYMBOL_inheritance_spec = 172,         /* inheritance_spec  */
  YYSYMBOL_173_33 = 173,                   /* $@33  */
  YYSYMBOL_value_def = 174,                /* value_def  */
  YYSYMBOL_valuetype = 175,                /* valuetype  */
  YYSYMBOL_value_concrete_decl = 176,      /* value_concrete_decl  */
  YYSYMBOL_177_34 = 177,                   /* @34  */
  YYSYMBOL_178_35 = 178,                   /* $@35  */
  YYSYMBOL_179_36 = 179,                   /* $@36  */
  YYSYMBOL_value_abs_decl = 180,           /* value_abs_decl  */
  YYSYMBOL_181_37 = 181,                   /* $@37  */
  YYSYMBOL_182_38 = 182,                   /* $@38  */
  YYSYMBOL_183_39 = 183,                   /* $@39  */
  YYSYMBOL_value_header = 184,             /* value_header  */
  YYSYMBOL_185_40 = 185,                   /* $@40  */
  YYSYMBOL_value_decl = 186,               /* value_decl  */
  YYSYMBOL_187_41 = 187,                   /* $@41  */
  YYSYMBOL_opt_truncatable = 188,          /* opt_truncatable  */
  YYSYMBOL_supports_spec = 189,            /* supports_spec  */
  YYSYMBOL_value_forward_decl = 190,       /* value_forward_decl  */
  YYSYMBOL_value_box_decl = 191,           /* value_box_decl  */
  YYSYMBOL_value_elements = 192,           /* value_elements  */
  YYSYMBOL_value_element = 193,            /* value_element  */
  YYSYMBOL_194_42 = 194,                   /* @42  */
  YYSYMBOL_visibility = 195,               /* visibility  */
  YYSYMBOL_state_member = 196,             /* state_member  */
  YYSYMBOL_exports = 197,                  /* exports  */
  YYSYMBOL_at_least_one_export = 198,      /* at_least_one_export  */
  YYSYMBOL_export = 199,                   /* export  */
  YYSYMBOL_200_43 = 200,                   /* $@43  */
  YYSYMBOL_201_44 = 201,                   /* $@44  */
  YYSYMBOL_202_45 = 202,                   /* $@45  */
  YYSYMBOL_203_46 = 203,                   /* $@46  */
  YYSYMBOL_204_47 = 204,                   /* $@47  */
  YYSYMBOL_205_48 = 205,                   /* $@48  */
  YYSYMBOL_206_49 = 206,                   /* $@49  */
  YYSYMBOL_207_50 = 207,                   /* $@50  */
  YYSYMBOL_at_least_one_scoped_name = 208, /* at_least_one_scoped_name  */
  YYSYMBOL_scoped_names = 209,             /* scoped_names  */
  YYSYMBOL_210_51 = 210,                   /* $@51  */
  YYSYMBOL_scoped_name = 211,              /* scoped_name  */
  YYSYMBOL_212_52 = 212,                   /* $@52  */
  YYSYMBOL_213_53 = 213,                   /* $@53  */
  YYSYMBOL_id = 214,                       /* id  */
  YYSYMBOL_defining_id = 215,              /* defining_id  */
  YYSYMBOL_interface_forward = 216,        /* interface_forward  */
  YYSYMBOL_const_dcl = 217,                /* const_dcl  */
  YYSYMBOL_218_54 = 218,                   /* $@54  */
  YYSYMBOL_219_55 = 219,                   /* $@55  */
  YYSYMBOL_220_56 = 220,                   /* $@56  */
  YYSYMBOL_221_57 = 221,                   /* $@57  */
  YYSYMBOL_const_type = 222,               /* const_type  */
  YYSYMBOL_expression = 223,               /* expression  */
  YYSYMBOL_const_expr = 224,               /* const_expr  */
  YYSYMBOL_or_expr = 225,                  /* or_expr  */
  YYSYMBOL_xor_expr = 226,                 /* xor_expr  */
  YYSYMBOL_and_expr = 227,                 /* and_expr  */
  YYSYMBOL_shift_expr = 228,               /* shift_expr  */
  YYSYMBOL_add_expr = 229,                 /* add_expr  */
  YYSYMBOL_mult_expr = 230,                /* mult_expr  */
  YYSYMBOL_unary_expr = 231,               /* unary_expr  */
  YYSYMBOL_primary_expr = 232,             /* primary_expr  */
  YYSYMBOL_literal = 233,                  /* literal  */
  YYSYMBOL_positive_int_expr = 234,        /* positive_int_expr  */
  YYSYMBOL_annotation_dcl = 235,           /* annotation_dcl  */
  YYSYMBOL_236_58 = 236,                   /* $@58  */
  YYSYMBOL_annotation_body = 237,          /* annotation_body  */
  YYSYMBOL_annotation_statement = 238,     /* annotation_statement  */
  YYSYMBOL_239_59 = 239,                   /* $@59  */
  YYSYMBOL_annotation_member_type = 240,   /* annotation_member_type  */
  YYSYMBOL_annotation_member = 241,        /* annotation_member  */
  YYSYMBOL_annotation_member_default = 242, /* annotation_member_default  */
  YYSYMBOL_at_least_one_annotation = 243,  /* at_least_one_annotation  */
  YYSYMBOL_annotations_maybe = 244,        /* annotations_maybe  */
  YYSYMBOL_annotation_appl = 245,          /* annotation_appl  */
  YYSYMBOL_246_60 = 246,                   /* @60  */
  YYSYMBOL_annotation_appl_params_maybe = 247, /* annotation_appl_params_maybe  */
  YYSYMBOL_annotation_appl_params = 248,   /* annotation_appl_params  */
  YYSYMBOL_named_annotation_appl_params = 249, /* named_annotation_appl_params  */
  YYSYMBOL_more_named_annotation_appl_params = 250, /* more_named_annotation_appl_params  */
  YYSYMBOL_named_annotation_appl_param = 251, /* named_annotation_appl_param  */
  YYSYMBOL_type_dcl = 252,                 /* type_dcl  */
  YYSYMBOL_253_61 = 253,                   /* $@61  */
  YYSYMBOL_type_declarator = 254,          /* type_declarator  */
  YYSYMBOL_255_62 = 255,                   /* $@62  */
  YYSYMBOL_type_spec = 256,                /* type_spec  */
  YYSYMBOL_simple_type_spec = 257,         /* simple_type_spec  */
  YYSYMBOL_base_type_spec = 258,           /* base_type_spec  */
  YYSYMBOL_template_type_spec = 259,       /* template_type_spec  */
  YYSYMBOL_constructed_type_spec = 260,    /* constructed_type_spec  */
  YYSYMBOL_constructed_forward_type_spec = 261, /* constructed_forward_type_spec  */
  YYSYMBOL_at_least_one_declarator = 262,  /* at_least_one_declarator  */
  YYSYMBOL_declarators = 263,              /* declarators  */
  YYSYMBOL_264_63 = 264,                   /* $@63  */
  YYSYMBOL_declarator = 265,               /* declarator  */
  YYSYMBOL_at_least_one_simple_declarator = 266, /* at_least_one_simple_declarator  */
  YYSYMBOL_simple_declarators = 267,       /* simple_declarators  */
  YYSYMBOL_268_64 = 268,                   /* $@64  */
  YYSYMBOL_simple_declarator = 269,        /* simple_declarator  */
  YYSYMBOL_complex_declarator = 270,       /* complex_declarator  */
  YYSYMBOL_integer_type = 271,             /* integer_type  */
  YYSYMBOL_signed_int = 272,               /* signed_int  */
  YYSYMBOL_unsigned_int = 273,             /* unsigned_int  */
  YYSYMBOL_floating_pt_type = 274,         /* floating_pt_type  */
  YYSYMBOL_fixed_type = 275,               /* fixed_type  */
  YYSYMBOL_char_type = 276,                /* char_type  */
  YYSYMBOL_octet_type = 277,               /* octet_type  */
  YYSYMBOL_boolean_type = 278,             /* boolean_type  */
  YYSYMBOL_any_type = 279,                 /* any_type  */
  YYSYMBOL_object_type = 280,              /* object_type  */
  YYSYMBOL_struct_decl = 281,              /* struct_decl  */
  YYSYMBOL_282_65 = 282,                   /* $@65  */
  YYSYMBOL_struct_type = 283,              /* struct_type  */
  YYSYMBOL_284_66 = 284,                   /* $@66  */
  YYSYMBOL_285_67 = 285,                   /* $@67  */
  YYSYMBOL_struct_body = 286,              /* struct_body  */
  YYSYMBOL_struct_body_with_members = 287, /* struct_body_with_members  */
  YYSYMBOL_288_68 = 288,                   /* $@68  */
  YYSYMBOL_members = 289,                  /* members  */
  YYSYMBOL_member = 290,                   /* member  */
  YYSYMBOL_member_i = 291,                 /* member_i  */
  YYSYMBOL_292_69 = 292,                   /* $@69  */
  YYSYMBOL_293_70 = 293,                   /* $@70  */
  YYSYMBOL_294_71 = 294,                   /* $@71  */
  YYSYMBOL_union_decl = 295,               /* union_decl  */
  YYSYMBOL_296_72 = 296,                   /* $@72  */
  YYSYMBOL_union_type = 297,               /* union_type  */
  YYSYMBOL_298_73 = 298,                   /* $@73  */
  YYSYMBOL_299_74 = 299,                   /* $@74  */
  YYSYMBOL_300_75 = 300,                   /* $@75  */
  YYSYMBOL_301_76 = 301,                   /* $@76  */
  YYSYMBOL_302_77 = 302,                   /* $@77  */
  YYSYMBOL_303_78 = 303,                   /* $@78  */
  YYSYMBOL_switch_type_spec = 304,         /* switch_type_spec  */
  YYSYMBOL_at_least_one_case_branch = 305, /* at_least_one_case_branch  */
  YYSYMBOL_case_branches = 306,            /* case_branches  */
  YYSYMBOL_case_branch = 307,              /* case_branch  */
  YYSYMBOL_308_79 = 308,                   /* $@79  */
  YYSYMBOL_309_80 = 309,                   /* $@80  */
  YYSYMBOL_310_81 = 310,                   /* $@81  */
  YYSYMBOL_at_least_one_case_label = 311,  /* at_least_one_case_label  */
  YYSYMBOL_case_labels = 312,              /* case_labels  */
  YYSYMBOL_case_label = 313,               /* case_label  */
  YYSYMBOL_314_82 = 314,                   /* $@82  */
  YYSYMBOL_315_83 = 315,                   /* $@83  */
  YYSYMBOL_316_84 = 316,                   /* $@84  */
  YYSYMBOL_element_spec = 317,             /* element_spec  */
  YYSYMBOL_318_85 = 318,                   /* $@85  */
  YYSYMBOL_struct_forward_type = 319,      /* struct_forward_type  */
  YYSYMBOL_union_forward_type = 320,       /* union_forward_type  */
  YYSYMBOL_enum_type = 321,                /* enum_type  */
  YYSYMBOL_322_86 = 322,                   /* $@86  */
  YYSYMBOL_323_87 = 323,                   /* $@87  */
  YYSYMBOL_324_88 = 324,                   /* $@88  */
  YYSYMBOL_325_89 = 325,                   /* $@89  */
  YYSYMBOL_at_least_one_enumerator = 326,  /* at_least_one_enumerator  */
  YYSYMBOL_enumerators = 327,              /* enumerators  */
  YYSYMBOL_328_90 = 328,                   /* $@90  */
  YYSYMBOL_enumerator = 329,               /* enumerator  */
  YYSYMBOL_map_type_spec = 330,            /* map_type_spec  */
  YYSYMBOL_map_head = 331,                 /* map_head  */
  YYSYMBOL_332_91 = 332,                   /* $@91  */
  YYSYMBOL_333_92 = 333,                   /* $@92  */
  YYSYMBOL_sequence_type_spec = 334,       /* sequence_type_spec  */
  YYSYMBOL_335_93 = 335,                   /* $@93  */
  YYSYMBOL_336_94 = 336,                   /* $@94  */
  YYSYMBOL_seq_head = 337,                 /* seq_head  */
  YYSYMBOL_338_95 = 338,                   /* $@95  */
  YYSYMBOL_339_96 = 339,                   /* $@96  */
  YYSYMBOL_fixed_type_spec = 340,          /* fixed_type_spec  */
  YYSYMBOL_string_type_spec = 341,         /* string_type_spec  */
  YYSYMBOL_342_97 = 342,                   /* $@97  */
  YYSYMBOL_343_98 = 343,                   /* $@98  */
  YYSYMBOL_string_head = 344,              /* string_head  */
  YYSYMBOL_wstring_type_spec = 345,        /* wstring_type_spec  */
  YYSYMBOL_346_99 = 346,                   /* $@99  */
  YYSYMBOL_347_100 = 347,                  /* $@100  */
  YYSYMBOL_wstring_head = 348,             /* wstring_head  */
  YYSYMBOL_array_declarator = 349,         /* array_declarator  */
  YYSYMBOL_350_101 = 350,                  /* $@101  */
  YYSYMBOL_at_least_one_array_dim = 351,   /* at_least_one_array_dim  */
  YYSYMBOL_array_dims = 352,               /* array_dims  */
  YYSYMBOL_array_dim = 353,                /* array_dim  */
  YYSYMBOL_354_102 = 354,                  /* $@102  */
  YYSYMBOL_355_103 = 355,                  /* $@103  */
  YYSYMBOL_attribute = 356,                /* attribute  */
  YYSYMBOL_attribute_readonly = 357,       /* attribute_readonly  */
  YYSYMBOL_358_104 = 358,                  /* $@104  */
  YYSYMBOL_359_105 = 359,                  /* $@105  */
  YYSYMBOL_360_106 = 360,                  /* $@106  */
  YYSYMBOL_361_107 = 361,                  /* $@107  */
  YYSYMBOL_attribute_readwrite = 362,      /* attribute_readwrite  */
  YYSYMBOL_363_108 = 363,                  /* $@108  */
  YYSYMBOL_364_109 = 364,                  /* $@109  */
  YYSYMBOL_365_110 = 365,                  /* $@110  */
  YYSYMBOL_366_111 = 366,                  /* $@111  */
  YYSYMBOL_exception = 367,                /* exception  */
  YYSYMBOL_368_112 = 368,                  /* $@112  */
  YYSYMBOL_369_113 = 369,                  /* @113  */
  YYSYMBOL_370_114 = 370,                  /* $@114  */
  YYSYMBOL_371_115 = 371,                  /* $@115  */
  YYSYMBOL_operation = 372,                /* operation  */
  YYSYMBOL_373_116 = 373,                  /* $@116  */
  YYSYMBOL_374_117 = 374,                  /* $@117  */
  YYSYMBOL_375_118 = 375,                  /* $@118  */
  YYSYMBOL_376_119 = 376,                  /* $@119  */
  YYSYMBOL_opt_op_attribute = 377,         /* opt_op_attribute  */
  YYSYMBOL_op_type_spec = 378,             /* op_type_spec  */
  YYSYMBOL_init_decl = 379,                /* init_decl  */
  YYSYMBOL_380_120 = 380,                  /* $@120  */
  YYSYMBOL_381_121 = 381,                  /* @121  */
  YYSYMBOL_382_122 = 382,                  /* $@122  */
  YYSYMBOL_init_parameter_list = 383,      /* init_parameter_list  */
  YYSYMBOL_384_123 = 384,                  /* $@123  */
  YYSYMBOL_385_124 = 385,                  /* $@124  */
  YYSYMBOL_at_least_one_in_parameter = 386, /* at_least_one_in_parameter  */
  YYSYMBOL_in_parameters = 387,            /* in_parameters  */
  YYSYMBOL_388_125 = 388,                  /* $@125  */
  YYSYMBOL_in_parameter = 389,             /* in_parameter  */
  YYSYMBOL_390_126 = 390,                  /* $@126  */
  YYSYMBOL_391_127 = 391,                  /* $@127  */
  YYSYMBOL_parameter_list = 392,           /* parameter_list  */
  YYSYMBOL_393_128 = 393,                  /* $@128  */
  YYSYMBOL_394_129 = 394,                  /* $@129  */
  YYSYMBOL_at_least_one_parameter = 395,   /* at_least_one_parameter  */
  YYSYMBOL_parameters = 396,               /* parameters  */
  YYSYMBOL_397_130 = 397,                  /* $@130  */
  YYSYMBOL_parameter = 398,                /* parameter  */
  YYSYMBOL_399_131 = 399,                  /* $@131  */
  YYSYMBOL_400_132 = 400,                  /* $@132  */
  YYSYMBOL_param_type_spec = 401,          /* param_type_spec  */
  YYSYMBOL_direction = 402,                /* direction  */
  YYSYMBOL_opt_raises = 403,               /* opt_raises  */
  YYSYMBOL_404_133 = 404,                  /* $@133  */
  YYSYMBOL_405_134 = 405,                  /* $@134  */
  YYSYMBOL_opt_getraises = 406,            /* opt_getraises  */
  YYSYMBOL_407_135 = 407,                  /* $@135  */
  YYSYMBOL_408_136 = 408,                  /* $@136  */
  YYSYMBOL_opt_setraises = 409,            /* opt_setraises  */
  YYSYMBOL_410_137 = 410,                  /* $@137  */
  YYSYMBOL_411_138 = 411,                  /* $@138  */
  YYSYMBOL_opt_context = 412,              /* opt_context  */
  YYSYMBOL_413_139 = 413,                  /* $@139  */
  YYSYMBOL_414_140 = 414,                  /* $@140  */
  YYSYMBOL_at_least_one_string_literal = 415, /* at_least_one_string_literal  */
  YYSYMBOL_string_literals = 416,          /* string_literals  */
  YYSYMBOL_417_141 = 417,                  /* $@141  */
  YYSYMBOL_typeid_dcl = 418,               /* typeid_dcl  */
  YYSYMBOL_typeprefix_dcl = 419,           /* typeprefix_dcl  */
  YYSYMBOL_component = 420,                /* component  */
  YYSYMBOL_component_forward_decl = 421,   /* component_forward_decl  */
  YYSYMBOL_component_decl = 422,           /* component_decl  */
  YYSYMBOL_423_142 = 423,                  /* @142  */
  YYSYMBOL_424_143 = 424,                  /* $@143  */
  YYSYMBOL_425_144 = 425,                  /* $@144  */
  YYSYMBOL_component_header = 426,         /* component_header  */
  YYSYMBOL_427_145 = 427,                  /* $@145  */
  YYSYMBOL_428_146 = 428,                  /* $@146  */
  YYSYMBOL_component_inheritance_spec = 429, /* component_inheritance_spec  */
  YYSYMBOL_430_147 = 430,                  /* $@147  */
  YYSYMBOL_component_exports = 431,        /* component_exports  */
  YYSYMBOL_component_export = 432,         /* component_export  */
  YYSYMBOL_433_148 = 433,                  /* $@148  */
  YYSYMBOL_434_149 = 434,                  /* $@149  */
  YYSYMBOL_435_150 = 435,                  /* $@150  */
  YYSYMBOL_436_151 = 436,                  /* $@151  */
  YYSYMBOL_437_152 = 437,                  /* $@152  */
  YYSYMBOL_438_153 = 438,                  /* $@153  */
  YYSYMBOL_439_154 = 439,                  /* $@154  */
  YYSYMBOL_provides_decl = 440,            /* provides_decl  */
  YYSYMBOL_interface_type = 441,           /* interface_type  */
  YYSYMBOL_uses_decl = 442,                /* uses_decl  */
  YYSYMBOL_uses_opt_multiple = 443,        /* uses_opt_multiple  */
  YYSYMBOL_opt_multiple = 444,             /* opt_multiple  */
  YYSYMBOL_emits_decl = 445,               /* emits_decl  */
  YYSYMBOL_publishes_decl = 446,           /* publishes_decl  */
  YYSYMBOL_consumes_decl = 447,            /* consumes_decl  */
  YYSYMBOL_home_decl = 448,                /* home_decl  */
  YYSYMBOL_449_155 = 449,                  /* $@155  */
  YYSYMBOL_home_header = 450,              /* home_header  */
  YYSYMBOL_451_156 = 451,                  /* $@156  */
  YYSYMBOL_452_157 = 452,                  /* $@157  */
  YYSYMBOL_453_158 = 453,                  /* $@158  */
  YYSYMBOL_454_159 = 454,                  /* $@159  */
  YYSYMBOL_455_160 = 455,                  /* $@160  */
  YYSYMBOL_456_161 = 456,                  /* $@161  */
  YYSYMBOL_home_inheritance_spec = 457,    /* home_inheritance_spec  */
  YYSYMBOL_458_162 = 458,                  /* $@162  */
  YYSYMBOL_primary_key_spec = 459,         /* primary_key_spec  */
  YYSYMBOL_home_body = 460,                /* home_body  */
  YYSYMBOL_461_163 = 461,                  /* $@163  */
  YYSYMBOL_462_164 = 462,                  /* $@164  */
  YYSYMBOL_home_exports = 463,             /* home_exports  */
  YYSYMBOL_home_export = 464,              /* home_export  */
  YYSYMBOL_465_165 = 465,                  /* $@165  */
  YYSYMBOL_466_166 = 466,                  /* $@166  */
  YYSYMBOL_factory_decl = 467,             /* factory_decl  */
  YYSYMBOL_468_167 = 468,                  /* $@167  */
  YYSYMBOL_469_168 = 469,                  /* $@168  */
  YYSYMBOL_finder_decl = 470,              /* finder_decl  */
  YYSYMBOL_471_169 = 471,                  /* $@169  */
  YYSYMBOL_472_170 = 472,                  /* $@170  */
  YYSYMBOL_event = 473,                    /* event  */
  YYSYMBOL_event_forward_decl = 474,       /* event_forward_decl  */
  YYSYMBOL_event_concrete_forward_decl = 475, /* event_concrete_forward_decl  */
  YYSYMBOL_event_abs_forward_decl = 476,   /* event_abs_forward_decl  */
  YYSYMBOL_event_abs_decl = 477,           /* event_abs_decl  */
  YYSYMBOL_478_171 = 478,                  /* $@171  */
  YYSYMBOL_479_172 = 479,                  /* $@172  */
  YYSYMBOL_480_173 = 480,                  /* $@173  */
  YYSYMBOL_event_abs_header = 481,         /* event_abs_header  */
  YYSYMBOL_event_custom_header = 482,      /* event_custom_header  */
  YYSYMBOL_event_plain_header = 483,       /* event_plain_header  */
  YYSYMBOL_event_rest_of_header = 484,     /* event_rest_of_header  */
  YYSYMBOL_485_174 = 485,                  /* $@174  */
  YYSYMBOL_event_decl = 486,               /* event_decl  */
  YYSYMBOL_487_175 = 487,                  /* @175  */
  YYSYMBOL_488_176 = 488,                  /* $@176  */
  YYSYMBOL_489_177 = 489,                  /* $@177  */
  YYSYMBOL_event_header = 490,             /* event_header  */
  YYSYMBOL_formal_parameter_type = 491,    /* formal_parameter_type  */
  YYSYMBOL_at_least_one_formal_parameter = 492, /* at_least_one_formal_parameter  */
  YYSYMBOL_formal_parameters = 493,        /* formal_parameters  */
  YYSYMBOL_formal_parameter = 494,         /* formal_parameter  */
  YYSYMBOL_at_least_one_formal_parameter_name = 495, /* at_least_one_formal_parameter_name  */
  YYSYMBOL_formal_parameter_names = 496,   /* formal_parameter_names  */
  YYSYMBOL_formal_parameter_name = 497,    /* formal_parameter_name  */
  YYSYMBOL_porttype_decl = 498,            /* porttype_decl  */
  YYSYMBOL_499_178 = 499,                  /* $@178  */
  YYSYMBOL_500_179 = 500,                  /* @179  */
  YYSYMBOL_501_180 = 501,                  /* $@180  */
  YYSYMBOL_502_181 = 502,                  /* $@181  */
  YYSYMBOL_at_least_one_port_export = 503, /* at_least_one_port_export  */
  YYSYMBOL_port_exports = 504,             /* port_exports  */
  YYSYMBOL_port_export = 505,              /* port_export  */
  YYSYMBOL_506_182 = 506,                  /* $@182  */
  YYSYMBOL_extended_port_decl = 507,       /* extended_port_decl  */
  YYSYMBOL_at_least_one_actual_parameter = 508, /* at_least_one_actual_parameter  */
  YYSYMBOL_actual_parameters = 509,        /* actual_parameters  */
  YYSYMBOL_actual_parameter = 510,         /* actual_parameter  */
  YYSYMBOL_connector_decl = 511,           /* connector_decl  */
  YYSYMBOL_connector_header = 512,         /* connector_header  */
  YYSYMBOL_513_183 = 513,                  /* $@183  */
  YYSYMBOL_514_184 = 514,                  /* $@184  */
  YYSYMBOL_connector_body = 515,           /* connector_body  */
  YYSYMBOL_516_185 = 516,                  /* $@185  */
  YYSYMBOL_517_186 = 517,                  /* $@186  */
  YYSYMBOL_connector_exports = 518,        /* connector_exports  */
  YYSYMBOL_connector_export = 519,         /* connector_export  */
  YYSYMBOL_520_187 = 520,                  /* $@187  */
  YYSYMBOL_521_188 = 521,                  /* $@188  */
  YYSYMBOL_522_189 = 522,                  /* $@189  */
  YYSYMBOL_523_190 = 523                   /* $@190  */
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

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
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
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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
#define YYLAST   2046

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  121
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  403
/* YYNRULES -- Number of rules.  */
#define YYNRULES  617
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  906

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   354


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
       2,     2,     2,     2,     2,     2,     2,   115,   110,     2,
     117,   118,   113,   111,   106,   112,     2,   114,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   105,   100,
     103,   107,   104,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   119,     2,   120,   109,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   101,   108,   102,   116,     2,     2,     2,
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
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   430,   430,   433,   434,   442,   457,   461,   462,   463,
     468,   467,   476,   475,   484,   483,   492,   491,   500,   499,
     508,   507,   516,   515,   524,   523,   532,   531,   540,   539,
     548,   547,   556,   555,   564,   563,   572,   571,   580,   579,
     593,   592,   604,   643,   647,   603,   663,   671,   685,   695,
     725,   729,   670,   754,   758,   759,   763,   764,   769,   774,
     768,   860,   865,   859,   936,   937,   942,   980,   984,   941,
    1001,  1000,  1012,  1049,  1079,  1112,  1111,  1120,  1127,  1128,
    1129,  1130,  1134,  1139,  1144,  1191,  1195,  1143,  1224,  1267,
    1271,  1222,  1290,  1288,  1328,  1327,  1339,  1343,  1350,  1355,
    1362,  1387,  1415,  1481,  1500,  1504,  1508,  1509,  1521,  1520,
    1538,  1542,  1549,  1570,  1571,  1575,  1590,  1595,  1594,  1603,
    1602,  1611,  1610,  1619,  1618,  1627,  1626,  1635,  1634,  1643,
    1642,  1651,  1650,  1663,  1675,  1673,  1698,  1705,  1715,  1714,
    1740,  1738,  1763,  1773,  1784,  1828,  1855,  1887,  1891,  1895,
    1899,  1886,  1961,  1962,  1963,  1964,  1965,  1966,  1967,  1971,
    1975,  2043,  2045,  2047,  2048,  2060,  2061,  2073,  2074,  2086,
    2087,  2096,  2108,  2109,  2118,  2130,  2131,  2140,  2149,  2161,
    2162,  2171,  2180,  2192,  2249,  2250,  2257,  2261,  2266,  2273,
    2280,  2284,  2289,  2293,  2297,  2301,  2308,  2377,  2376,  2403,
    2404,  2408,  2409,  2410,  2412,  2411,  2420,  2421,  2425,  2481,
    2485,  2492,  2505,  2515,  2523,  2522,  2608,  2612,  2619,  2628,
    2632,  2639,  2647,  2653,  2660,  2673,  2672,  2681,  2685,  2689,
    2693,  2721,  2729,  2728,  2799,  2800,  2804,  2811,  2812,  2838,
    2839,  2840,  2841,  2842,  2843,  2844,  2845,  2849,  2850,  2851,
    2852,  2853,  2857,  2858,  2859,  2863,  2864,  2868,  2880,  2878,
    2903,  2910,  2911,  2915,  2927,  2925,  2950,  2957,  2973,  2991,
    2992,  2996,  3000,  3004,  3008,  3012,  3016,  3020,  3027,  3031,
    3035,  3039,  3043,  3047,  3051,  3058,  3062,  3066,  3073,  3080,
    3084,  3091,  3098,  3105,  3112,  3120,  3119,  3133,  3164,  3132,
    3180,  3188,  3193,  3192,  3203,  3204,  3208,  3226,  3230,  3225,
    3288,  3287,  3300,  3299,  3312,  3316,  3349,  3353,  3412,  3416,
    3311,  3438,  3442,  3450,  3458,  3462,  3463,  3572,  3575,  3576,
    3581,  3585,  3580,  3621,  3620,  3632,  3642,  3660,  3668,  3667,
    3681,  3685,  3680,  3701,  3700,  3750,  3775,  3799,  3803,  3834,
    3838,  3798,  3862,  3867,  3865,  3871,  3875,  3913,  3984,  4046,
    4056,  4045,  4081,  4085,  4079,  4169,  4236,  4245,  4235,  4259,
    4269,  4273,  4267,  4315,  4341,  4350,  4354,  4348,  4396,  4422,
    4430,  4429,  4472,  4482,  4500,  4508,  4512,  4507,  4572,  4573,
    4578,  4582,  4586,  4590,  4577,  4649,  4653,  4657,  4661,  4648,
    4729,  4733,  4765,  4769,  4728,  4786,  4790,  4851,  4855,  4785,
    4892,  4897,  4902,  4909,  4910,  4921,  4926,  4969,  4920,  4991,
    4990,  4999,  4998,  5009,  5014,  5012,  5018,  5023,  5027,  5022,
    5066,  5065,  5074,  5073,  5084,  5089,  5087,  5093,  5098,  5102,
    5097,  5147,  5154,  5155,  5156,  5311,  5315,  5319,  5327,  5331,
    5326,  5340,  5348,  5352,  5347,  5361,  5369,  5373,  5368,  5382,
    5390,  5394,  5389,  5403,  5410,  5422,  5420,  5443,  5450,  5480,
    5519,  5520,  5524,  5555,  5597,  5601,  5554,  5620,  5624,  5618,
    5665,  5664,  5672,  5679,  5694,  5695,  5700,  5699,  5709,  5708,
    5718,  5717,  5727,  5726,  5736,  5735,  5745,  5744,  5754,  5753,
    5764,  5857,  5863,  5888,  5995,  6004,  6008,  6015,  6090,  6162,
    6238,  6237,  6287,  6291,  6295,  6299,  6303,  6307,  6286,  6360,
    6359,  6367,  6374,  6379,  6387,  6391,  6386,  6401,  6402,  6406,
    6408,  6407,  6416,  6415,  6428,  6451,  6426,  6477,  6504,  6475,
    6528,  6529,  6530,  6534,  6535,  6539,  6568,  6600,  6644,  6648,
    6598,  6665,  6674,  6692,  6703,  6702,  6740,  6791,  6795,  6738,
    6812,  6816,  6823,  6827,  6831,  6835,  6839,  6843,  6847,  6851,
    6855,  6859,  6863,  6871,  6902,  6915,  6922,  6947,  6965,  6972,
    6987,  6994,  7004,  7008,  7027,  7035,  7003,  7050,  7065,  7069,
    7070,  7074,  7075,  7077,  7076,  7087,  7154,  7202,  7218,  7231,
    7238,  7297,  7305,  7309,  7304,  7370,  7374,  7369,  7387,  7388,
    7393,  7392,  7401,  7400,  7409,  7408,  7417,  7416
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
  "IDL_FALSETOK", "IDL_INT8", "IDL_UINT8", "IDL_INT16", "IDL_UINT16",
  "IDL_INT32", "IDL_UINT32", "IDL_INT64", "IDL_UINT64",
  "IDL_SCOPE_DELIMITOR", "IDL_LEFT_SHIFT", "IDL_RIGHT_SHIFT",
  "IDL_WCHAR_LITERAL", "IDL_WSTRING_LITERAL", "IDL_ANNOTATION_DECL",
  "IDL_ANNOTATION_SYMBOL", "IDL_BITFIELD", "IDL_BITMASK", "IDL_BITSET",
  "IDL_MAP", "';'", "'{'", "'}'", "'<'", "'>'", "':'", "','", "'='", "'|'",
  "'^'", "'&'", "'+'", "'-'", "'*'", "'/'", "'%'", "'~'", "'('", "')'",
  "'['", "']'", "$accept", "start", "definitions",
  "at_least_one_definition", "definition", "fixed_definition", "$@1",
  "$@2", "$@3", "$@4", "$@5", "$@6", "$@7", "$@8", "$@9", "$@10", "$@11",
  "$@12", "$@13", "$@14", "$@15", "module_header", "$@16", "module", "@17",
  "$@18", "$@19", "template_module_header", "template_module", "$@20",
  "$@21", "$@22", "$@23", "$@24", "at_least_one_tpl_definition",
  "tpl_definitions", "tpl_definition", "template_module_ref", "$@25",
  "$@26", "template_module_inst", "$@27", "$@28", "interface_def",
  "interface", "$@29", "$@30", "$@31", "interface_decl", "$@32",
  "interface_header", "inheritance_spec", "$@33", "value_def", "valuetype",
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
  "struct_body", "struct_body_with_members", "$@68", "members", "member",
  "member_i", "$@69", "$@70", "$@71", "union_decl", "$@72", "union_type",
  "$@73", "$@74", "$@75", "$@76", "$@77", "$@78", "switch_type_spec",
  "at_least_one_case_branch", "case_branches", "case_branch", "$@79",
  "$@80", "$@81", "at_least_one_case_label", "case_labels", "case_label",
  "$@82", "$@83", "$@84", "element_spec", "$@85", "struct_forward_type",
  "union_forward_type", "enum_type", "$@86", "$@87", "$@88", "$@89",
  "at_least_one_enumerator", "enumerators", "$@90", "enumerator",
  "map_type_spec", "map_head", "$@91", "$@92", "sequence_type_spec",
  "$@93", "$@94", "seq_head", "$@95", "$@96", "fixed_type_spec",
  "string_type_spec", "$@97", "$@98", "string_head", "wstring_type_spec",
  "$@99", "$@100", "wstring_head", "array_declarator", "$@101",
  "at_least_one_array_dim", "array_dims", "array_dim", "$@102", "$@103",
  "attribute", "attribute_readonly", "$@104", "$@105", "$@106", "$@107",
  "attribute_readwrite", "$@108", "$@109", "$@110", "$@111", "exception",
  "$@112", "@113", "$@114", "$@115", "operation", "$@116", "$@117",
  "$@118", "$@119", "opt_op_attribute", "op_type_spec", "init_decl",
  "$@120", "@121", "$@122", "init_parameter_list", "$@123", "$@124",
  "at_least_one_in_parameter", "in_parameters", "$@125", "in_parameter",
  "$@126", "$@127", "parameter_list", "$@128", "$@129",
  "at_least_one_parameter", "parameters", "$@130", "parameter", "$@131",
  "$@132", "param_type_spec", "direction", "opt_raises", "$@133", "$@134",
  "opt_getraises", "$@135", "$@136", "opt_setraises", "$@137", "$@138",
  "opt_context", "$@139", "$@140", "at_least_one_string_literal",
  "string_literals", "$@141", "typeid_dcl", "typeprefix_dcl", "component",
  "component_forward_decl", "component_decl", "@142", "$@143", "$@144",
  "component_header", "$@145", "$@146", "component_inheritance_spec",
  "$@147", "component_exports", "component_export", "$@148", "$@149",
  "$@150", "$@151", "$@152", "$@153", "$@154", "provides_decl",
  "interface_type", "uses_decl", "uses_opt_multiple", "opt_multiple",
  "emits_decl", "publishes_decl", "consumes_decl", "home_decl", "$@155",
  "home_header", "$@156", "$@157", "$@158", "$@159", "$@160", "$@161",
  "home_inheritance_spec", "$@162", "primary_key_spec", "home_body",
  "$@163", "$@164", "home_exports", "home_export", "$@165", "$@166",
  "factory_decl", "$@167", "$@168", "finder_decl", "$@169", "$@170",
  "event", "event_forward_decl", "event_concrete_forward_decl",
  "event_abs_forward_decl", "event_abs_decl", "$@171", "$@172", "$@173",
  "event_abs_header", "event_custom_header", "event_plain_header",
  "event_rest_of_header", "$@174", "event_decl", "@175", "$@176", "$@177",
  "event_header", "formal_parameter_type", "at_least_one_formal_parameter",
  "formal_parameters", "formal_parameter",
  "at_least_one_formal_parameter_name", "formal_parameter_names",
  "formal_parameter_name", "porttype_decl", "$@178", "@179", "$@180",
  "$@181", "at_least_one_port_export", "port_exports", "port_export",
  "$@182", "extended_port_decl", "at_least_one_actual_parameter",
  "actual_parameters", "actual_parameter", "connector_decl",
  "connector_header", "$@183", "$@184", "connector_body", "$@185", "$@186",
  "connector_exports", "connector_export", "$@187", "$@188", "$@189",
  "$@190", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-721)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-586)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -721,    57,   232,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,  -721,  -721,    67,    87,   176,    78,  -721,    67,
      67,  -721,    50,    50,  -721,  -721,    67,  -721,  -721,   -17,
    -721,   195,     8,    30,  -721,  -721,     6,  -721,  -721,  -721,
    -721,  -721,  -721,   548,  -721,  -721,  -721,  -721,  -721,  1676,
      58,  -721,  -721,    91,  -721,   157,  -721,  -721,  -721,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,  -721,  -721,   112,  -721,  -721,  -721,   112,  -721,
    -721,    82,   106,  1957,    50,    67,  1520,    67,    67,    67,
      67,  -721,  -721,  -721,    80,    67,   142,  -721,   160,    67,
    -721,   112,    67,   124,   128,    67,  -721,  -721,    75,  -721,
     101,   228,  -721,   165,  -721,   170,   158,   571,  -721,  -721,
    -721,   161,   216,  -721,   178,   213,   219,   113,  -721,    90,
    -721,  -721,  -721,  -721,  -721,  -721,   220,  -721,  -721,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,   233,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
     157,  -721,  -721,  -721,    66,  -721,   163,  -721,  -721,   221,
    -721,   225,   230,   235,  -721,    50,   237,   240,   243,  -721,
     247,   250,   251,   259,   261,   260,   263,   267,  -721,  -721,
    -721,   269,   271,  -721,  -721,  -721,  -721,   233,  -721,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,   233,  -721,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,   272,  -721,   274,  -721,
    -721,   258,  -721,   367,  -721,  -721,  -721,  -721,    39,  -721,
    -721,  -721,  1957,  -721,  -721,  -721,  -721,   275,  -721,  -721,
    -721,  -721,  -721,   371,  -721,  -721,    40,   276,  -721,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,   368,  -721,   134,   278,
     279,   332,  -721,   134,  -721,  -721,  -721,  -721,  -721,  -721,
     233,  -721,  -721,   266,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,  -721,   332,   285,   288,  -721,  -721,  -721,    67,
      67,   289,   290,  -721,  -721,  -721,   287,  -721,   367,   294,
    -721,  -721,  -721,  -721,  -721,   394,  -721,   296,   301,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,    69,
      69,    69,   134,   233,  -721,  -721,   303,   293,   302,    97,
     123,   186,  -721,  -721,  -721,  -721,  -721,    50,  -721,  -721,
    -721,  -721,   307,  -721,  -721,    50,  -721,   317,   134,   134,
     134,   305,   323,  -721,  -721,  -721,  -721,  -721,  -721,   135,
    -721,   -19,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
      50,   332,  -721,  -721,  -721,  -721,   258,  1382,  1590,   324,
     328,  -721,   571,  -721,  -721,  -721,   309,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   327,    67,  -721,
     233,  1131,  -721,   843,   134,  -721,  1539,  -721,  -721,  -721,
    -721,  -721,   134,  -721,  -721,   668,  -721,  -721,  -721,  -721,
     246,  1035,  -721,  -721,  -721,  -721,    65,   379,    50,    50,
    -721,  -721,  -721,  -721,  -721,    65,  -721,   336,  -721,   337,
    -721,   333,  -721,  -721,  1166,   233,  -721,    50,   332,  -721,
    -721,  -721,  -721,   343,  -721,  -721,    67,  -721,  -721,   345,
     344,   444,   347,  -721,  -721,   293,   302,    97,   123,   123,
     186,   186,  -721,  -721,  -721,  -721,  -721,   346,  -721,  -721,
    -721,   351,  -721,  -721,  1837,  -721,  -721,  -721,  -721,  1870,
    -721,  -721,  -721,  -721,  -721,   353,  -721,  1408,  -721,  -721,
    1748,  -721,   355,  1539,  -721,   356,   357,   358,   361,  -721,
     354,  -721,   365,  -721,  -721,  -721,   362,   501,    50,    50,
      50,   389,  -721,   381,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,    67,    67,  -721,   382,  -721,  -721,  -721,  1262,   939,
     432,  1924,  -721,   233,   367,  -721,  -721,    52,    70,   373,
     388,   390,   367,   392,  -721,  -721,    83,  -721,    41,  -721,
    -721,   387,   391,   233,  -721,   393,    95,  1520,  -721,   465,
    -721,  -721,  -721,  -721,    40,  -721,   396,  -721,   397,  -721,
     398,   399,   400,   401,  -721,   233,  -721,  -721,  -721,  -721,
    -721,   402,   403,   509,  -721,  -721,  -721,   413,  -721,  -721,
     414,  -721,  -721,  -721,   134,  -721,   367,  -721,   421,    67,
     420,  -721,   515,   233,  -721,  -721,  -721,  -721,  -721,  -721,
      79,    79,    79,  -721,   425,  -721,   426,   427,   428,   430,
     433,   434,  -721,  -721,  -721,   435,   438,   439,   440,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,   134,
    -721,  -721,  -721,    67,  -721,   441,   442,   445,  -721,   478,
     448,    95,  -721,   446,   452,  -721,   453,   134,   454,  1651,
    -721,    50,  -721,  -721,  -721,  -721,  -721,  -721,   573,  -721,
    -721,  -721,  -721,  -721,  -721,   361,   365,  -721,  -721,  -721,
     460,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,   462,   462,  -721,  -721,  -721,  -721,  1924,    67,  -721,
     134,   461,  -721,  -721,  -721,  -721,  -721,  -721,  -721,   481,
    -721,  -721,  -721,  -721,  -721,    50,  -721,  -721,  -721,  -721,
     493,   233,  -721,   462,  1539,  -721,   499,  -721,   565,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,    50,  -721,   233,
     500,  1312,  -721,   484,  -721,  -721,  -721,   502,   486,   570,
     568,   568,    67,   553,   504,   491,  -721,   233,   510,  -721,
    -721,   494,  -721,   568,  -721,  -721,  -721,   496,  -721,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,   556,   613,   503,
     273,   568,  -721,   262,  1924,  -721,   511,   505,   568,   506,
     557,    67,    50,  -721,  -721,   514,  -721,  -721,  -721,  -721,
    -721,   507,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
     233,  -721,   520,  -721,   521,  1924,   589,   539,   134,   535,
     541,    73,  -721,   212,    67,   570,    50,    50,   524,    67,
     613,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
    1427,  -721,  -721,  -721,   526,   527,  -721,  -721,  -721,   273,
      67,   532,   545,  -721,  -721,  -721,  -721,    50,  -721,  -721,
    -721,  -721,    67,   551,   534,   579,  -721,  -721,  -721,  -721,
     537,   550,  -721,  -721,   582,  -721
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       4,     0,     0,     3,     1,    38,   147,    40,    70,   225,
     295,   312,   347,   400,     0,     0,     0,     0,    94,     0,
       0,   512,     0,     0,   582,   602,     0,     6,     7,    42,
      24,    61,     0,     0,    22,    64,    77,    66,    26,    78,
      83,    79,    84,    77,    80,    81,    65,    18,    10,     0,
       0,    12,   231,   297,   227,   346,   228,   255,   256,   229,
      20,    14,    16,    28,   471,   470,   473,    30,   510,    32,
     542,   544,   543,   541,    77,   560,   561,   540,    77,    34,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   143,   267,   230,    77,     0,    77,    88,    77,     0,
      82,    77,     0,   477,   553,     0,   142,   138,     0,   137,
       0,     0,   213,     0,    46,     0,     0,     0,   213,     8,
       9,     0,    97,    72,     0,     0,     0,   271,   273,     0,
     285,   286,   289,   290,   291,   292,   288,   293,   294,   366,
     374,   379,   274,   281,   275,   282,   276,   283,   277,   284,
     359,    92,   238,   102,   234,   236,   237,   235,   239,   269,
     270,   240,   244,   241,   243,   242,   245,   246,   297,   252,
       0,   253,   254,   251,     0,   247,     0,   250,   248,   373,
     249,   378,     0,     0,     5,     0,   211,     0,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,   554,   547,
     556,     0,     0,   605,   601,    39,   288,   160,   148,   152,
     156,   157,   153,   154,   155,   158,   159,    41,    71,   226,
     232,   296,   313,   348,   401,    73,   551,    74,     0,   552,
      95,   482,   513,     0,   468,   140,   469,   583,     0,   197,
      43,    25,     0,   568,   563,   564,   570,   566,   571,   569,
     565,   562,   567,     0,    48,   575,     0,     0,    23,    96,
      75,    67,    27,    85,   272,   287,   278,   280,     0,     0,
       0,    99,   358,     0,   365,   362,   370,   375,    19,    11,
     214,    13,   298,     0,    21,    15,    17,    29,   474,    31,
     524,   511,    33,    99,     0,     0,    35,    37,   609,     0,
       0,     0,     0,    89,   480,   478,   521,   139,     0,     0,
     603,   212,   200,     4,   572,     0,   576,     0,   573,   186,
     187,   188,   190,   193,   192,   194,   195,   191,   189,     0,
       0,     0,     0,   183,   600,   161,   162,   163,   165,   167,
     169,   172,   175,   179,   184,   599,    62,     0,   114,   105,
     279,   196,     0,   367,   213,     0,    93,     0,     0,     0,
       0,   217,   213,   315,   485,   528,   555,   548,   557,   606,
     149,   267,   233,   260,   261,   262,   268,   349,   402,   114,
       0,    99,   519,   514,   141,   584,   482,     0,     0,     3,
       0,    49,     0,   180,   181,   182,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   597,     0,    76,
     136,     0,   113,     0,     0,   213,     0,    98,   357,   363,
     371,   376,   220,   215,   300,     0,   299,   301,   305,   213,
     213,     0,   114,   105,   390,   395,     0,   506,     0,     0,
     614,   388,   389,   610,   612,     0,   616,     0,   608,     0,
     213,   257,   213,   305,     0,   481,   479,     0,    99,   590,
     604,   204,   198,     0,   206,   199,     0,   201,   207,     0,
       0,     0,     0,   574,   185,   164,   166,   168,   170,   171,
     173,   174,   176,   177,   178,   213,    63,   133,   131,   410,
     411,     0,   116,   123,     0,   117,   127,   125,   129,     0,
     119,   121,   415,   111,   110,     0,   104,     0,   106,   107,
       0,   108,     0,     0,   360,     0,     0,     0,   137,   218,
       0,   219,   223,   310,   307,   306,   213,     0,     0,     0,
       0,     0,   496,     0,   484,   486,   488,   490,   492,   494,
     498,     0,     0,   529,     0,   527,   530,   532,     0,     0,
       0,     0,   502,   501,     0,   505,   504,     0,     0,     0,
       0,     0,     0,     0,   607,   150,     0,   258,     0,   350,
     355,   213,     0,   520,   515,   589,   213,     0,   202,   210,
     203,    45,   577,    50,     0,   134,     0,    69,     0,   115,
       0,     0,     0,     0,   414,   444,   441,   442,   443,   405,
     413,     0,     0,     0,    87,   112,   103,     0,   369,   368,
       0,   364,   372,   377,     0,   216,     0,   221,     0,     0,
       0,   304,   271,   326,   321,   322,   323,   324,   316,   325,
       0,     0,     0,   483,     0,   476,     0,     0,     0,     0,
       0,     0,   534,   537,   526,     0,     0,     0,     0,   391,
     396,   500,   595,   596,   615,   611,   613,   503,   617,     0,
     385,   381,   384,     0,   356,     0,   352,     0,    91,     0,
       0,     0,   593,     0,     0,   588,     0,     0,     0,     0,
     598,     0,   132,   124,   118,   128,   126,   130,     0,   120,
     122,   416,   109,   213,   224,     0,   223,   311,   308,   303,
       0,   509,   507,   508,   497,   487,   489,   491,   493,   495,
     499,     0,     0,   531,   533,   550,   559,     0,     0,   151,
       0,   382,   259,   351,   353,   404,   516,   586,   587,     0,
     591,   592,   205,   209,   208,     0,    56,    42,    51,    55,
       0,   135,   406,     0,     0,   222,     0,   317,   419,   535,
     538,   392,   397,   266,   386,   383,   213,     0,   594,    58,
       0,     0,    57,     0,   417,   361,   309,     0,     0,     0,
     451,   451,     0,   455,   263,     0,   354,   517,     0,    52,
      54,   432,   407,   451,   318,   420,   427,     0,   426,   448,
     536,   539,   393,   452,   398,   264,   387,   523,     0,     0,
       0,   451,   418,     0,     0,   422,   423,     0,   451,     0,
     459,     0,     0,   518,   581,     0,   580,   431,   445,   446,
     447,     0,   437,   438,   408,   333,   340,   338,   319,   329,
     330,   337,   428,   424,   449,   394,   453,   456,   399,   265,
     522,    59,   578,   433,   434,     0,   463,     0,     0,     0,
       0,     0,   213,   335,     0,     0,     0,     0,     0,     0,
       0,   435,   439,   460,   409,   334,   341,   339,   320,   328,
       0,   336,   429,   425,     0,     0,   457,    60,   579,     0,
       0,     0,     0,   343,   331,   450,   454,     0,   436,   440,
     461,   342,     0,     0,     0,     0,   344,   332,   458,   467,
       0,   464,   462,   465,     0,   466
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -721,  -721,   348,   349,   609,  -621,  -721,  -721,  -721,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,  -592,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -101,  -721,  -721,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,   268,  -721,
    -721,    16,  -721,  -721,  -721,   646,  -721,  -721,  -721,  -721,
    -721,  -721,  -721,   648,  -721,   270,  -721,  -721,  -253,  -721,
    -721,   234,   155,  -721,  -721,  -721,  -323,  -721,  -372,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,  -338,  -721,  -721,
     -22,  -721,  -721,  -201,   -10,  -721,    17,  -721,  -721,  -721,
    -721,  -213,     7,  -241,  -721,   277,   300,   304,  -108,   -98,
     -87,    26,  -721,  -255,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,    21,   -84,   618,  -721,  -721,  -721,  -721,   -23,
      59,    11,  -721,   125,  -721,   -29,  -390,  -468,  -721,  -721,
    -721,    81,  -721,  -721,  -624,   -68,  -721,  -721,    -7,  -721,
     -53,  -721,  -721,   -58,   -50,   -48,   -46,   -45,   318,  -721,
     -40,  -721,   -38,  -721,  -721,  -721,  -721,  -721,   253,   350,
     200,  -721,  -721,  -721,   -37,  -721,   -32,  -721,  -721,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -142,  -721,  -721,  -721,
    -721,  -721,  -143,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
     -41,  -721,  -721,  -721,  -721,  -721,  -721,  -721,   -42,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
     -75,  -721,  -721,  -721,   -71,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,    -6,  -721,  -721,  -333,  -721,  -721,  -721,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,    18,  -721,  -721,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,  -647,  -721,  -721,  -721,  -721,  -721,  -139,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,  -162,  -721,  -721,
    -524,  -721,  -720,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,    20,    22,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,   334,  -721,
    -721,   187,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -322,
     280,  -319,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,  -721,   641,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,  -721,   329,  -721,  -721,  -137,  -721,  -721,  -721,
    -721,  -721,  -721,  -721,    53,  -721,   359,  -721,  -721,   143,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,  -721
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,     3,    27,    28,   183,   187,   191,   192,
     182,   190,   121,   116,   125,   193,   195,   197,   201,   202,
      82,    29,    84,    30,   115,   313,   470,    31,    32,   117,
     317,   472,   679,   760,   738,   761,   739,   740,   778,   859,
      33,   118,   408,    34,    35,   124,   348,   491,    36,    85,
      37,   151,   347,    38,    39,    40,   126,   349,   505,    41,
     228,   379,   572,    42,   271,    43,   102,   260,   356,    44,
      45,   413,   506,   607,   507,   508,   411,   412,   492,   590,
     601,   602,   588,   592,   591,   593,   586,   409,   487,   681,
     333,   233,   308,   109,   371,    46,   493,    83,   299,   449,
     659,   208,   334,   351,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   352,    48,   312,   387,   465,   577,   466,
     467,   678,   494,    50,   311,   361,   423,   520,   521,   617,
     522,   495,    86,   219,   300,   220,   154,   155,   156,   157,
      52,   372,   451,   663,   373,   752,   774,   811,   374,   375,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
      53,    87,    54,   188,   362,   426,   427,   620,   526,   621,
     525,   619,   746,   618,    55,    88,    56,   283,   429,   700,
     767,   803,   850,   628,   828,   851,   829,   852,   893,   847,
     830,   853,   831,   849,   848,   882,   884,   892,    57,    58,
      59,    89,   301,   452,   665,   569,   666,   756,   570,   173,
     174,   270,   610,   175,   358,   515,   176,   269,   415,   177,
     178,   359,   516,   179,   180,   360,   517,   181,   376,   450,
     661,   721,   662,   720,   775,   496,   441,   550,   717,   772,
     808,   442,   551,   718,   773,   810,   497,    90,   302,   453,
     667,   498,   688,   763,   801,   846,   499,   599,   511,   603,
     743,   783,   749,   768,   769,   787,   806,   855,   788,   804,
     854,   782,   799,   800,   821,   844,   879,   822,   845,   880,
     600,   823,   790,   807,   856,   794,   809,   857,   838,   858,
     887,   864,   881,   895,   900,   901,   904,   500,   501,    63,
      64,    65,   194,   364,   533,    66,   231,   381,   305,   380,
     430,   534,   636,   637,   638,   639,   640,   634,   641,   535,
     554,   536,   445,   556,   537,   538,   539,    67,   196,    68,
     105,   306,   458,   669,   757,   797,   383,   457,   813,   291,
     365,   544,   431,   545,   645,   646,   546,   711,   770,   547,
     712,   771,    69,    70,    71,    72,    73,   294,   432,   647,
      74,    75,    76,   199,   293,    77,   295,   433,   648,    78,
     253,   254,   318,   255,   815,   842,   816,    79,   111,   309,
     459,   670,   575,   576,   675,   729,   540,   257,   407,   345,
      80,    81,   112,   386,   204,   298,   447,   369,   448,   560,
     561,   559,   563
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     108,   110,   172,   168,    92,   169,   170,    93,   215,   103,
     104,   171,   216,    51,   153,   335,   113,   417,   357,    47,
      60,   152,    61,    49,    62,   210,   514,   650,   238,   314,
     209,   596,   307,   211,   256,   212,   440,   213,   214,   722,
     366,   509,   310,   106,   664,   172,   168,   443,   169,   170,
     444,   791,   123,   106,   171,   652,   454,     4,   736,   543,
      51,   207,   217,   802,   152,   750,    47,    60,   106,    61,
      91,    62,   106,   653,   825,   218,  -380,   221,   222,   223,
     224,   824,   106,   596,   552,   226,   114,   737,   835,   229,
     198,   396,   230,     8,   198,   232,   764,   532,   266,   267,
    -380,   826,   827,   419,   420,   421,  -144,   384,   119,   548,
     225,   122,   227,   319,   320,   321,   322,   323,   324,   325,
     326,   264,   589,   609,   265,   434,   435,    18,   456,   107,
     120,    99,   327,   328,   185,   185,   185,   106,   509,   107,
     736,   235,   319,   320,   321,   322,   323,   324,   325,   326,
     234,   329,   330,   185,   107,   436,   331,   332,   107,   235,
     437,   327,   328,   280,   235,   434,   435,   215,   235,   737,
     272,   216,   273,   512,   464,  -327,   236,   509,   185,   189,
    -145,   519,     8,   203,   210,   122,   332,   400,   401,   209,
     235,  -345,   211,   751,   212,   436,   213,   214,   532,   -47,
     437,   -47,   660,   438,   439,   574,   205,   319,   320,   321,
     322,   323,   324,   325,   326,   -47,   -47,   122,   -47,   -47,
     207,   518,   -47,   107,  -472,    18,   327,   328,  -545,    95,
     872,   237,    -2,     5,   402,   403,     6,     7,     8,     9,
     826,   827,  -146,   672,   -47,   329,   330,   122,   -47,   596,
     331,   332,    10,    11,   673,    12,   889,   674,   241,    13,
    -100,   258,   -47,   825,   259,   122,   239,   274,   896,   275,
     416,   240,    14,    15,    16,    17,   434,   435,   425,   261,
     832,    18,    19,    94,    96,    20,    98,   101,    21,   370,
     826,   827,   478,   479,   -47,    22,    23,   528,   529,   404,
     405,   406,    24,    25,   480,   481,   436,   530,   818,   819,
     820,   437,   215,   262,   438,   439,   216,   482,   483,   484,
     263,   862,   235,   268,   276,   410,    26,  -213,   277,   210,
     278,   513,  -212,   410,   209,   279,   596,   211,   672,   212,
     281,   213,   214,   335,   282,   527,   469,   284,  -475,   673,
     285,   286,   674,   651,   765,   393,   394,   395,   455,   287,
     289,   657,   288,   304,   290,   207,   566,   292,   568,   296,
     106,   297,  -546,   694,   316,   303,   350,   596,   315,   355,
     346,   353,   354,   363,   172,   168,   367,   169,   170,   368,
     377,   378,   382,   171,   152,   385,   524,   390,   486,    51,
     391,   584,   398,   152,   463,    47,    60,   392,    61,    49,
      62,   397,   399,   414,   553,   695,   557,   558,   335,   434,
     435,   418,   422,   553,   597,   424,   -44,   474,   598,   701,
     702,   703,   471,   485,   510,   573,   733,   555,   564,   567,
     528,   529,   425,   578,   565,   580,   581,   582,   583,   436,
     530,   531,   585,   587,   437,   604,   579,   438,   439,   608,
     611,   612,   613,   649,  -302,   754,   172,   168,   614,   169,
     170,   616,   615,   654,   624,   171,   597,   595,   524,   625,
     598,   626,   627,   635,   644,   152,   629,   425,   655,  -403,
     656,   152,   658,   668,   677,  -585,   682,   683,   684,   685,
     686,   687,   689,   690,   106,   623,   630,   631,   632,   622,
     128,   129,   691,   692,   132,   133,   134,   135,   874,   875,
     693,   697,   699,   264,    12,   704,   705,   706,   707,   595,
     708,   642,   643,   709,   710,   713,   172,   168,   714,   169,
     170,   715,   716,   723,   726,   171,   730,   725,   724,   894,
     727,   106,   731,   732,   734,   152,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,    10,    11,
     510,    12,   139,   140,   141,   242,   742,   243,   747,   748,
     660,   758,   142,   143,   144,   145,   146,   147,   148,   149,
     107,   244,   245,   762,   246,   247,   185,   671,   248,   766,
    -421,   781,   779,   784,   785,   786,   789,   866,   793,   744,
     795,   796,  -430,   798,   805,   812,   814,   833,   841,   837,
     249,   817,   834,   836,   250,   843,   860,   861,   863,   142,
     143,   144,   145,   146,   147,   148,   149,   107,   251,   865,
     867,   876,   597,   868,   885,   886,   598,   150,  -101,   890,
     891,   897,   898,   122,   899,   902,   903,   905,   184,   741,
     780,   388,   389,   100,    97,   606,   719,   549,   186,   523,
     252,   106,   568,   745,   475,   696,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,    10,    11,
      51,    12,   139,   140,   141,   595,    47,    60,   476,    61,
     698,    62,   676,   477,   792,   468,   571,   605,    92,   869,
     871,   753,   428,   759,   776,   755,   873,   888,   633,   200,
     460,   473,   152,   878,   728,   562,     0,   680,   446,   597,
       0,     0,     0,   598,     0,   777,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   142,
     143,   144,   145,   146,   147,   148,   149,   107,     0,     0,
       0,     0,    92,   185,     0,   753,     0,   150,   870,     0,
     597,     0,    51,     0,   598,     0,     0,     0,    47,    60,
       0,    61,   595,    62,     0,     0,     0,     0,     0,     0,
     840,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,   839,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   595,     0,     0,     0,     0,     0,   172,
     168,     0,   169,   170,   410,   410,     0,     0,   171,     0,
       0,   883,     0,     0,   488,     0,  -412,     6,   152,   877,
       9,  -412,  -412,  -412,  -412,  -412,  -412,  -412,  -412,  -412,
    -412,  -412,  -412,    10,    11,   410,    12,     0,  -412,  -412,
      13,     0,     0,   434,   435,   489,   490,  -412,     0,     0,
       0,     0,     0,    14,     0,     0,     0,   502,   503,   504,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    22,    23,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -412,  -412,  -412,  -412,  -412,  -412,
    -412,  -412,  -412,     0,     0,     0,     0,     0,  -213,     0,
     488,     0,  -412,     6,     0,   -86,     9,  -412,  -412,  -412,
    -412,  -412,  -412,  -412,  -412,  -412,  -412,  -412,  -412,    10,
      11,     0,    12,     0,  -412,  -412,    13,     0,     0,   434,
     435,   489,   490,  -412,     0,     0,     0,     0,     0,    14,
       0,     0,     0,   502,   503,   504,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,    23,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -412,  -412,  -412,  -412,  -412,  -412,  -412,  -412,  -412,     0,
       0,     0,     0,     0,  -213,     0,   488,     0,  -412,     6,
       0,  -558,     9,  -412,  -412,  -412,  -412,  -412,  -412,  -412,
    -412,  -412,  -412,  -412,  -412,    10,    11,     0,    12,     0,
    -412,  -412,    13,     0,     0,   434,   435,   489,   490,  -412,
       0,     0,     0,     0,     0,    14,     0,     0,     0,   541,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   542,
       0,     0,     0,     0,     0,     0,     0,     0,    22,    23,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -412,  -412,  -412,  -412,
    -412,  -412,  -412,  -412,  -412,     0,     0,     0,     0,     0,
       0,     0,   488,     0,  -412,     6,     0,  -525,     9,  -412,
    -412,  -412,  -412,  -412,  -412,  -412,  -412,  -412,  -412,  -412,
    -412,    10,    11,     0,    12,     0,  -412,  -412,    13,     0,
       0,   434,   435,   489,   490,  -412,     0,   488,     0,  -412,
       6,    14,     0,     9,  -412,  -412,  -412,  -412,  -412,  -412,
    -412,  -412,  -412,  -412,  -412,  -412,    10,    11,     0,    12,
       0,  -412,  -412,    13,    22,    23,   434,   435,   489,   490,
    -412,     0,     0,     0,     0,     0,    14,     0,     0,     0,
       0,     0,  -412,  -412,  -412,  -412,  -412,  -412,  -412,  -412,
    -412,     0,     0,     0,     0,     0,  -213,     0,     0,    22,
      23,     0,     0,   -68,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -412,  -412,  -412,
    -412,  -412,  -412,  -412,  -412,  -412,     0,     0,     0,     0,
       0,  -213,     0,   488,     0,  -412,     6,     0,   -90,     9,
    -412,  -412,  -412,  -412,  -412,  -412,  -412,  -412,  -412,  -412,
    -412,  -412,    10,    11,     0,    12,     0,  -412,  -412,    13,
       0,     0,   434,   435,   489,   490,  -412,     0,     0,     0,
       0,     0,    14,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     5,     0,     0,     6,     7,     8,     9,
       0,     0,     0,     0,     0,    22,    23,     0,     0,     0,
       0,     0,    10,    11,     0,    12,     0,     0,     0,    13,
       0,     0,     0,  -412,  -412,  -412,  -412,  -412,  -412,  -412,
    -412,  -412,    14,    15,    16,    17,     0,  -213,     0,     0,
       0,    18,    19,     0,  -549,    20,     0,     0,    21,     0,
       0,     0,     0,     0,     0,    22,    23,     0,     0,     0,
       0,     0,    24,    25,   735,   106,     6,     0,     0,   461,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   206,
     137,     0,     0,     0,     0,    12,    26,   140,   141,   523,
       0,   106,     0,     0,   -53,     0,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,    10,    11,
     106,    12,   139,   140,   141,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    10,    11,     0,
      12,   139,   140,   141,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   142,   143,   144,   145,   146,   147,   148,
     149,   107,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   462,     0,     0,     0,     0,   142,
     143,   144,   145,   146,   147,   148,   149,   107,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   150,   142,   143,
     144,   145,   146,   147,   148,   149,   107,     0,     0,     0,
       0,     0,   185,   106,     0,     0,   150,     0,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
      10,    11,   106,    12,   139,   140,   141,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,     0,
       0,     0,     0,   139,   140,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     5,     0,     0,     6,     7,     8,     9,     0,     0,
       0,   142,   143,   144,   145,   146,   147,   148,   149,   107,
      10,    11,     0,    12,     0,     0,     0,    13,     0,   150,
     142,   143,   144,   145,   146,   147,   148,   149,   107,     0,
      14,    15,    16,    17,   185,     0,     0,     0,   150,    18,
      19,     0,     0,    20,     0,     0,    21,     0,     0,     0,
       0,     0,     5,    22,    23,     6,     7,     8,     9,     0,
      24,    25,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    10,    11,     0,    12,     0,     0,     5,    13,     0,
       6,     7,     8,     9,    26,  -213,     0,     0,     0,     0,
       0,    14,    15,    16,    17,     0,    10,    11,     0,    12,
      18,    19,     0,    13,    20,     0,     0,    21,     0,     0,
       0,     0,     0,     0,    22,    23,    14,    15,    16,    17,
       0,    24,    25,   735,     0,    18,    19,     0,     0,    20,
       0,     0,    21,     0,     0,     0,     0,     0,     0,    22,
      23,     0,     0,     0,     0,    26,    24,    25,     0,   488,
       0,  -412,     6,     0,     0,     9,  -412,  -412,  -412,  -412,
    -412,  -412,  -412,  -412,  -412,  -412,  -412,  -412,    10,    11,
      26,    12,     0,  -412,  -412,    13,     0,     0,   434,   435,
     489,   490,  -412,     0,     0,     0,     0,     0,    14,     0,
       0,     0,   502,   503,   504,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,    23,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -412,
    -412,  -412,  -412,  -412,  -412,  -412,  -412,  -412,   488,     0,
    -412,     6,     0,     0,     9,  -412,  -412,  -412,  -412,  -412,
    -412,  -412,  -412,  -412,  -412,  -412,  -412,    10,    11,     0,
      12,     0,  -412,  -412,    13,     0,     0,   434,   435,   489,
     490,  -412,     0,   106,     0,     0,     0,    14,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   206,   137,   138,
       0,     0,     0,     0,     0,   140,   141,     0,     0,     0,
      22,    23,     0,     0,   594,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -412,  -412,
    -412,  -412,  -412,  -412,  -412,  -412,  -412,   106,     0,     0,
       0,     0,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   206,   137,   138,     0,     0,     0,     0,     0,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   107,
     106,     0,     0,     0,     0,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   206,     0,     0,     0,     0,     0,
       0,     0,   140,   141,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   142,   143,   144,   145,   146,
     147,   148,   149,   107,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   142,   143,
     144,   145,   146,   147,   148,   149,   107
};

static const yytype_int16 yycheck[] =
{
      22,    23,    43,    43,    14,    43,    43,    14,    83,    19,
      20,    43,    83,     2,    43,   256,    26,   355,   273,     2,
       2,    43,     2,     2,     2,    83,   416,   551,   112,   242,
      83,   499,   233,    83,   118,    83,   369,    83,    83,   663,
     293,   413,     3,     3,     3,    86,    86,   369,    86,    86,
     369,   771,    36,     3,    86,     3,   379,     0,   679,   431,
      49,    83,    84,   783,    86,   712,    49,    49,     3,    49,
       3,    49,     3,     3,     1,    85,    95,    87,    88,    89,
      90,   801,     3,   551,    19,    95,   103,   679,   808,    99,
      74,   332,   102,     6,    78,   105,   743,   430,     8,     9,
     119,    28,    29,   358,   359,   360,   100,   308,   100,   432,
      94,   105,    96,    73,    74,    75,    76,    77,    78,    79,
      80,     8,   494,   513,    11,    30,    31,    49,   381,    89,
     100,    53,    92,    93,    95,    95,    95,     3,   510,    89,
     761,    89,    73,    74,    75,    76,    77,    78,    79,    80,
      75,   111,   112,    95,    89,    60,   116,   117,    89,    89,
      65,    92,    93,   185,    89,    30,    31,   242,    89,   761,
     104,   242,   106,   414,   387,   102,    75,   549,    95,    22,
     100,   422,     6,   101,   242,   105,   117,    90,    91,   242,
      89,   100,   242,   717,   242,    60,   242,   242,   531,     4,
      65,     6,   119,    68,    69,   458,   100,    73,    74,    75,
      76,    77,    78,    79,    80,    20,    21,   105,    23,    24,
     242,   422,    27,    89,   100,    49,    92,    93,   100,    53,
     854,     3,     0,     1,   111,   112,     4,     5,     6,     7,
      28,    29,   100,   576,    49,   111,   112,   105,    53,   717,
     116,   117,    20,    21,   576,    23,   880,   576,   100,    27,
     100,   100,    67,     1,    48,   105,   101,   104,   892,   106,
     354,   101,    40,    41,    42,    43,    30,    31,   362,   101,
     804,    49,    50,    15,    16,    53,    16,    17,    56,   299,
      28,    29,   400,   401,    99,    63,    64,    51,    52,   113,
     114,   115,    70,    71,   402,   403,    60,    61,    35,    36,
      37,    65,   387,   100,    68,    69,   387,   404,   405,   406,
     101,   845,    89,   103,   103,   347,    94,    95,   103,   387,
     100,   415,    95,   355,   387,   100,   804,   387,   671,   387,
     100,   387,   387,   584,   101,   429,   387,   100,   102,   671,
     100,   100,   671,   554,   744,   329,   330,   331,   380,   100,
     100,   562,   101,   105,   101,   387,   450,   100,   452,   100,
       3,   100,   100,   614,     3,   101,     8,   845,   103,    47,
     104,   103,   103,   117,   425,   425,   101,   425,   425,   101,
     101,   101,   105,   425,   416,   101,   425,     3,   408,   388,
     104,   485,   109,   425,   387,   388,   388,   106,   388,   388,
     388,   108,   110,   106,   436,   616,   438,   439,   659,    30,
      31,   104,   117,   445,   499,   102,   102,   118,   499,   630,
     631,   632,   104,   106,   413,   457,   677,    58,   102,   106,
      51,    52,   526,   100,   107,   100,   102,     3,   101,    60,
      61,   430,   106,   102,    65,   102,   466,    68,    69,   104,
     104,   104,   104,    31,   102,   720,   507,   507,   107,   507,
     507,   106,   118,   100,   527,   507,   551,   499,   507,   527,
     551,   527,   527,   102,   102,   507,   527,   571,   100,   102,
     100,   513,   100,   102,    29,   102,   100,   100,   100,   100,
     100,   100,   100,   100,     3,   527,   528,   529,   530,     8,
       9,    10,     3,   100,    13,    14,    15,    16,   856,   857,
     106,   100,   102,     8,    23,   100,   100,   100,   100,   551,
     100,   541,   542,   100,   100,   100,   577,   577,   100,   577,
     577,   102,   102,   102,    66,   577,   100,   102,   106,   887,
     102,     3,   100,   100,   100,   577,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
     549,    23,    24,    25,    26,     4,     3,     6,   118,   117,
     119,   100,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    20,    21,   100,    23,    24,    95,   576,    27,   100,
      35,   117,   102,   101,   118,    35,    38,   848,    55,   693,
     106,   120,   118,   103,   118,    59,     3,   106,   104,    62,
      49,   118,   117,   117,    53,   118,   106,   106,    39,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    67,   100,
     105,   117,   717,   102,   118,   118,   717,    99,   100,   117,
     105,   100,   118,   105,    75,   118,   106,    75,    49,   681,
     761,   313,   313,    17,    16,   510,   659,   433,    50,     1,
      99,     3,   756,   696,   397,   616,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
     679,    23,    24,    25,    26,   717,   679,   679,   398,   679,
     619,   679,   577,   399,   772,   387,   453,   507,   718,   851,
     853,   718,   362,   735,   756,   721,   855,   879,   531,    78,
     386,   392,   744,   860,   671,   445,    -1,   584,   369,   804,
      -1,    -1,    -1,   804,    -1,   757,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    -1,
      -1,    -1,   772,    95,    -1,   772,    -1,    99,   852,    -1,
     845,    -1,   761,    -1,   845,    -1,    -1,    -1,   761,   761,
      -1,   761,   804,   761,    -1,    -1,    -1,    -1,    -1,    -1,
     812,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   811,    -1,    -1,   811,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   845,    -1,    -1,    -1,    -1,    -1,   870,
     870,    -1,   870,   870,   856,   857,    -1,    -1,   870,    -1,
      -1,   870,    -1,    -1,     1,    -1,     3,     4,   870,   859,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,   887,    23,    -1,    25,    26,
      27,    -1,    -1,    30,    31,    32,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    44,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    95,    -1,
       1,    -1,     3,     4,    -1,   102,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    -1,    25,    26,    27,    -1,    -1,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    44,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    95,    -1,     1,    -1,     3,     4,
      -1,   102,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    23,    -1,
      25,    26,    27,    -1,    -1,    30,    31,    32,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,     3,     4,    -1,   102,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    -1,    25,    26,    27,    -1,
      -1,    30,    31,    32,    33,    34,    -1,     1,    -1,     3,
       4,    40,    -1,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      -1,    25,    26,    27,    63,    64,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    63,
      64,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    95,    -1,     1,    -1,     3,     4,    -1,   102,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    23,    -1,    25,    26,    27,
      -1,    -1,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,    -1,     4,     5,     6,     7,
      -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    23,    -1,    -1,    -1,    27,
      -1,    -1,    -1,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    40,    41,    42,    43,    -1,    95,    -1,    -1,
      -1,    49,    50,    -1,   102,    53,    -1,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,     3,     4,    -1,    -1,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    23,    94,    25,    26,     1,
      -1,     3,    -1,    -1,   102,    -1,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
       3,    23,    24,    25,    26,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      23,    24,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    95,     3,    -1,    -1,    99,    -1,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,     3,    23,    24,    25,    26,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    24,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     1,    -1,    -1,     4,     5,     6,     7,    -1,    -1,
      -1,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      20,    21,    -1,    23,    -1,    -1,    -1,    27,    -1,    99,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      40,    41,    42,    43,    95,    -1,    -1,    -1,    99,    49,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    -1,
      -1,    -1,     1,    63,    64,     4,     5,     6,     7,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    23,    -1,    -1,     1,    27,    -1,
       4,     5,     6,     7,    94,    95,    -1,    -1,    -1,    -1,
      -1,    40,    41,    42,    43,    -1,    20,    21,    -1,    23,
      49,    50,    -1,    27,    53,    -1,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    40,    41,    42,    43,
      -1,    70,    71,    72,    -1,    49,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    94,    70,    71,    -1,     1,
      -1,     3,     4,    -1,    -1,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      94,    23,    -1,    25,    26,    27,    -1,    -1,    30,    31,
      32,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    44,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    89,     1,    -1,
       3,     4,    -1,    -1,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      23,    -1,    25,    26,    27,    -1,    -1,    30,    31,    32,
      33,    34,    -1,     3,    -1,    -1,    -1,    40,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,
      63,    64,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    84,    85,    86,    87,    88,    89,     3,    -1,    -1,
      -1,    -1,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    81,    82,    83,    84,    85,    86,    87,    88,    89,
       3,    -1,    -1,    -1,    -1,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    84,    85,    86,    87,    88,    89
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   122,   123,   124,     0,     1,     4,     5,     6,     7,
      20,    21,    23,    27,    40,    41,    42,    43,    49,    50,
      53,    56,    63,    64,    70,    71,    94,   125,   126,   142,
     144,   148,   149,   161,   164,   165,   169,   171,   174,   175,
     176,   180,   184,   186,   190,   191,   216,   217,   235,   243,
     244,   252,   261,   281,   283,   295,   297,   319,   320,   321,
     367,   418,   419,   420,   421,   422,   426,   448,   450,   473,
     474,   475,   476,   477,   481,   482,   483,   486,   490,   498,
     511,   512,   141,   218,   143,   170,   253,   282,   296,   322,
     368,     3,   215,   269,   169,    53,   169,   184,   186,    53,
     176,   186,   187,   215,   215,   451,     3,    89,   211,   214,
     211,   499,   513,   215,   103,   145,   134,   150,   162,   100,
     100,   133,   105,   172,   166,   135,   177,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    24,
      25,    26,    81,    82,    83,    84,    85,    86,    87,    88,
      99,   172,   211,   256,   257,   258,   259,   260,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   283,
     295,   297,   321,   330,   331,   334,   337,   340,   341,   344,
     345,   348,   131,   127,   125,    95,   245,   128,   284,    22,
     132,   129,   130,   136,   423,   137,   449,   138,   172,   484,
     484,   139,   140,   101,   515,   100,    17,   211,   222,   271,
     274,   275,   276,   277,   278,   341,   345,   211,   215,   254,
     256,   215,   215,   215,   215,   172,   215,   172,   181,   215,
     215,   427,   215,   212,    75,    89,    75,     3,   244,   101,
     101,   100,     4,     6,    20,    21,    23,    24,    27,    49,
      53,    67,    99,   491,   492,   494,   244,   508,   100,    48,
     188,   101,   100,   101,     8,    11,     8,     9,   103,   338,
     332,   185,   104,   106,   104,   106,   103,   103,   100,   100,
     211,   100,   101,   298,   100,   100,   100,   100,   101,   100,
     101,   460,   100,   485,   478,   487,   100,   100,   516,   219,
     255,   323,   369,   101,   105,   429,   452,   214,   213,   500,
       3,   245,   236,   146,   222,   103,     3,   151,   493,    73,
      74,    75,    76,    77,    78,    79,    80,    92,    93,   111,
     112,   116,   117,   211,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   510,   104,   173,   167,   178,
       8,   224,   234,   103,   103,    47,   189,   234,   335,   342,
     346,   246,   285,   117,   424,   461,   189,   101,   101,   518,
     215,   215,   262,   265,   269,   270,   349,   101,   101,   182,
     430,   428,   105,   457,   214,   101,   514,   237,   123,   124,
       3,   104,   106,   232,   232,   232,   224,   108,   109,   110,
      90,    91,   111,   112,   113,   114,   115,   509,   163,   208,
     211,   197,   198,   192,   106,   339,   244,   208,   104,   234,
     234,   234,   117,   247,   102,   244,   286,   287,   290,   299,
     431,   463,   479,   488,    30,    31,    60,    65,    68,    69,
     356,   357,   362,   440,   442,   443,   507,   517,   519,   220,
     350,   263,   324,   370,   197,   211,   189,   458,   453,   501,
     429,     7,   102,   217,   222,   238,   240,   241,   279,   321,
     147,   104,   152,   494,   118,   226,   227,   228,   229,   229,
     230,   230,   231,   231,   231,   106,   215,   209,     1,    32,
      33,   168,   199,   217,   243,   252,   356,   367,   372,   377,
     418,   419,    44,    45,    46,   179,   193,   195,   196,   199,
     243,   379,   224,   244,   257,   336,   343,   347,   214,   224,
     248,   249,   251,     1,   256,   291,   289,   244,    51,    52,
      61,   243,   356,   425,   432,   440,   442,   445,   446,   447,
     507,    44,    54,   199,   462,   464,   467,   470,   197,   192,
     358,   363,    19,   211,   441,    58,   444,   211,   211,   522,
     520,   521,   441,   523,   102,   107,   244,   106,   244,   326,
     329,   289,   183,   211,   189,   503,   504,   239,   100,   215,
     100,   102,     3,   101,   244,   106,   207,   102,   203,   199,
     200,   205,   204,   206,    34,   211,   258,   341,   345,   378,
     401,   201,   202,   380,   102,   291,   193,   194,   104,   257,
     333,   104,   104,   104,   107,   118,   106,   250,   294,   292,
     288,   290,     8,   211,   271,   276,   277,   278,   304,   321,
     211,   211,   211,   432,   438,   102,   433,   434,   435,   436,
     437,   439,   215,   215,   102,   465,   466,   480,   489,    31,
     401,   214,     3,     3,   100,   100,   100,   214,   100,   221,
     119,   351,   353,   264,     3,   325,   327,   371,   102,   454,
     502,   243,   356,   440,   442,   505,   254,    29,   242,   153,
     510,   210,   100,   100,   100,   100,   100,   100,   373,   100,
     100,     3,   100,   106,   224,   214,   251,   100,   262,   102,
     300,   214,   214,   214,   100,   100,   100,   100,   100,   100,
     100,   468,   471,   100,   100,   102,   102,   359,   364,   223,
     354,   352,   265,   102,   106,   102,    66,   102,   505,   506,
     100,   100,   100,   224,   100,    72,   126,   142,   155,   157,
     158,   211,     3,   381,   244,   250,   293,   118,   117,   383,
     383,   401,   266,   269,   234,   353,   328,   455,   100,   211,
     154,   156,   100,   374,   383,   257,   100,   301,   384,   385,
     469,   472,   360,   365,   267,   355,   329,   211,   159,   102,
     157,   117,   392,   382,   101,   118,    35,   386,   389,    38,
     403,   403,   266,    55,   406,   106,   120,   456,   103,   393,
     394,   375,   403,   302,   390,   118,   387,   404,   361,   407,
     366,   268,    59,   459,     3,   495,   497,   118,    35,    36,
      37,   395,   398,   402,   403,     1,    28,    29,   305,   307,
     311,   313,   401,   106,   117,   403,   117,    62,   409,   269,
     211,   104,   496,   118,   396,   399,   376,   310,   315,   314,
     303,   306,   308,   312,   391,   388,   405,   408,   410,   160,
     106,   106,   401,    39,   412,   100,   224,   105,   102,   307,
     244,   313,   265,   389,   208,   208,   117,   215,   497,   397,
     400,   413,   316,   256,   317,   118,   118,   411,   398,   265,
     117,   105,   318,   309,   208,   414,   265,   100,   118,    75,
     415,   416,   118,   106,   417,    75
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   121,   122,   123,   123,   124,   124,   125,   125,   125,
     127,   126,   128,   126,   129,   126,   130,   126,   131,   126,
     132,   126,   133,   126,   134,   126,   135,   126,   136,   126,
     137,   126,   138,   126,   139,   126,   140,   126,   141,   126,
     143,   142,   145,   146,   147,   144,   148,   150,   151,   152,
     153,   154,   149,   155,   156,   156,   157,   157,   159,   160,
     158,   162,   163,   161,   164,   164,   166,   167,   168,   165,
     170,   169,   171,   171,   171,   173,   172,   172,   174,   174,
     174,   174,   175,   175,   177,   178,   179,   176,   181,   182,
     183,   180,   185,   184,   187,   186,   188,   188,   189,   189,
     190,   190,   191,   192,   192,   192,   193,   193,   194,   193,
     195,   195,   196,   197,   197,   198,   198,   200,   199,   201,
     199,   202,   199,   203,   199,   204,   199,   205,   199,   206,
     199,   207,   199,   208,   210,   209,   209,   211,   212,   211,
     213,   211,   214,   215,   216,   216,   216,   218,   219,   220,
     221,   217,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   223,   224,   225,   225,   226,   226,   227,   227,   228,
     228,   228,   229,   229,   229,   230,   230,   230,   230,   231,
     231,   231,   231,   232,   232,   232,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   234,   236,   235,   237,
     237,   238,   238,   238,   239,   238,   240,   240,   241,   242,
     242,   243,   244,   244,   246,   245,   247,   247,   248,   248,
     248,   249,   250,   250,   251,   253,   252,   252,   252,   252,
     252,   252,   255,   254,   256,   256,   257,   257,   257,   258,
     258,   258,   258,   258,   258,   258,   258,   259,   259,   259,
     259,   259,   260,   260,   260,   261,   261,   262,   264,   263,
     263,   265,   265,   266,   268,   267,   267,   269,   270,   271,
     271,   272,   272,   272,   272,   272,   272,   272,   273,   273,
     273,   273,   273,   273,   273,   274,   274,   274,   275,   276,
     276,   277,   278,   279,   280,   282,   281,   284,   285,   283,
     286,   286,   288,   287,   289,   289,   290,   292,   293,   291,
     294,   291,   296,   295,   298,   299,   300,   301,   302,   303,
     297,   304,   304,   304,   304,   304,   304,   305,   306,   306,
     308,   309,   307,   310,   307,   311,   312,   312,   314,   313,
     315,   316,   313,   318,   317,   319,   320,   322,   323,   324,
     325,   321,   326,   328,   327,   327,   329,   330,   330,   332,
     333,   331,   335,   336,   334,   334,   338,   339,   337,   340,
     342,   343,   341,   341,   344,   346,   347,   345,   345,   348,
     350,   349,   351,   352,   352,   354,   355,   353,   356,   356,
     358,   359,   360,   361,   357,   363,   364,   365,   366,   362,
     368,   369,   370,   371,   367,   373,   374,   375,   376,   372,
     377,   377,   377,   378,   378,   380,   381,   382,   379,   384,
     383,   385,   383,   386,   388,   387,   387,   390,   391,   389,
     393,   392,   394,   392,   395,   397,   396,   396,   399,   400,
     398,   401,   401,   401,   401,   402,   402,   402,   404,   405,
     403,   403,   407,   408,   406,   406,   410,   411,   409,   409,
     413,   414,   412,   412,   415,   417,   416,   416,   418,   419,
     420,   420,   421,   423,   424,   425,   422,   427,   428,   426,
     430,   429,   429,   431,   431,   431,   433,   432,   434,   432,
     435,   432,   436,   432,   437,   432,   438,   432,   439,   432,
     440,   441,   441,   442,   443,   444,   444,   445,   446,   447,
     449,   448,   451,   452,   453,   454,   455,   456,   450,   458,
     457,   457,   459,   459,   461,   462,   460,   463,   463,   464,
     465,   464,   466,   464,   468,   469,   467,   471,   472,   470,
     473,   473,   473,   474,   474,   475,   476,   478,   479,   480,
     477,   481,   482,   483,   485,   484,   487,   488,   489,   486,
     490,   490,   491,   491,   491,   491,   491,   491,   491,   491,
     491,   491,   491,   492,   493,   493,   494,   494,   495,   496,
     496,   497,   499,   500,   501,   502,   498,   503,   503,   504,
     504,   505,   505,   506,   505,   507,   507,   508,   509,   509,
     510,   511,   513,   514,   512,   516,   517,   515,   518,   518,
     520,   519,   521,   519,   522,   519,   523,   519
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
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
       0,     2,     3,     0,     3,     0,     3,     1,     1,     1,
       2,     1,     0,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     0,     4,
       0,     1,     1,     2,     0,     4,     0,     1,     1,     1,
       1,     1,     2,     1,     1,     1,     1,     1,     2,     3,
       2,     1,     1,     1,     1,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     0,     3,     0,     0,     5,
       1,     1,     0,     4,     2,     0,     2,     0,     0,     5,
       0,     3,     0,     3,     0,     0,     0,     0,     0,     0,
      15,     1,     1,     1,     1,     1,     1,     2,     2,     0,
       0,     0,     6,     0,     3,     2,     2,     0,     0,     3,
       0,     0,     5,     0,     3,     1,     1,     0,     0,     0,
       0,     9,     2,     0,     4,     0,     2,     4,     2,     0,
       0,     9,     0,     0,     6,     2,     0,     0,     6,     6,
       0,     0,     6,     1,     1,     0,     0,     6,     1,     1,
       0,     4,     2,     2,     0,     0,     0,     5,     1,     1,
       0,     0,     0,     0,     9,     0,     0,     0,     0,     9,
       0,     0,     0,     0,     9,     0,     0,     0,     0,    10,
       1,     1,     0,     1,     1,     0,     0,     0,     7,     0,
       3,     0,     4,     2,     0,     4,     0,     0,     0,     5,
       0,     3,     0,     4,     2,     0,     4,     0,     0,     0,
       5,     1,     1,     1,     1,     1,     1,     1,     0,     0,
       6,     0,     0,     0,     6,     0,     0,     0,     6,     0,
       0,     0,     6,     0,     2,     0,     4,     0,     3,     3,
       1,     1,     2,     0,     0,     0,     7,     0,     0,     6,
       0,     3,     0,     3,     2,     0,     0,     3,     0,     3,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       3,     1,     1,     3,     2,     1,     0,     3,     3,     3,
       0,     3,     0,     0,     0,     0,     0,     0,    13,     0,
       3,     0,     2,     0,     0,     0,     5,     2,     0,     1,
       0,     3,     0,     3,     0,     0,     6,     0,     0,     6,
       1,     1,     1,     1,     1,     2,     3,     0,     0,     0,
       8,     3,     3,     2,     0,     3,     0,     0,     0,     8,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     3,     0,     2,     5,     2,     3,
       0,     1,     0,     0,     0,     0,     9,     3,     2,     1,
       0,     2,     2,     0,     3,     3,     3,     3,     4,     0,
       1,     2,     0,     0,     6,     0,     0,     5,     2,     0,
       0,     3,     0,     3,     0,     3,     0,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
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
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
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
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

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
    YYNOMEM;
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
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
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
  case 5: /* at_least_one_definition: definitions at_least_one_annotation definition  */
#line 443 "fe/idl.ypp"
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
#line 2741 "fe/idl.tab.cpp"
    break;

  case 10: /* $@1: %empty  */
#line 468 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AnnotationDeclSeen);
        }
#line 2749 "fe/idl.tab.cpp"
    break;

  case 11: /* fixed_definition: annotation_dcl $@1 ';'  */
#line 472 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2757 "fe/idl.tab.cpp"
    break;

  case 12: /* $@2: %empty  */
#line 476 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 2765 "fe/idl.tab.cpp"
    break;

  case 13: /* fixed_definition: type_dcl $@2 ';'  */
#line 480 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2773 "fe/idl.tab.cpp"
    break;

  case 14: /* $@3: %empty  */
#line 484 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 2781 "fe/idl.tab.cpp"
    break;

  case 15: /* fixed_definition: typeid_dcl $@3 ';'  */
#line 488 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2789 "fe/idl.tab.cpp"
    break;

  case 16: /* $@4: %empty  */
#line 492 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 2797 "fe/idl.tab.cpp"
    break;

  case 17: /* fixed_definition: typeprefix_dcl $@4 ';'  */
#line 496 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2805 "fe/idl.tab.cpp"
    break;

  case 18: /* $@5: %empty  */
#line 500 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 2813 "fe/idl.tab.cpp"
    break;

  case 19: /* fixed_definition: const_dcl $@5 ';'  */
#line 504 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2821 "fe/idl.tab.cpp"
    break;

  case 20: /* $@6: %empty  */
#line 508 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 2829 "fe/idl.tab.cpp"
    break;

  case 21: /* fixed_definition: exception $@6 ';'  */
#line 512 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2837 "fe/idl.tab.cpp"
    break;

  case 22: /* $@7: %empty  */
#line 516 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceDeclSeen);
        }
#line 2845 "fe/idl.tab.cpp"
    break;

  case 23: /* fixed_definition: interface_def $@7 ';'  */
#line 520 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2853 "fe/idl.tab.cpp"
    break;

  case 24: /* $@8: %empty  */
#line 524 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleDeclSeen);
        }
#line 2861 "fe/idl.tab.cpp"
    break;

  case 25: /* fixed_definition: module $@8 ';'  */
#line 528 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2869 "fe/idl.tab.cpp"
    break;

  case 26: /* $@9: %empty  */
#line 532 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeDeclSeen);
        }
#line 2877 "fe/idl.tab.cpp"
    break;

  case 27: /* fixed_definition: value_def $@9 ';'  */
#line 536 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2885 "fe/idl.tab.cpp"
    break;

  case 28: /* $@10: %empty  */
#line 540 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentDeclSeen);
        }
#line 2893 "fe/idl.tab.cpp"
    break;

  case 29: /* fixed_definition: component $@10 ';'  */
#line 544 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2901 "fe/idl.tab.cpp"
    break;

  case 30: /* $@11: %empty  */
#line 548 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeDeclSeen);
        }
#line 2909 "fe/idl.tab.cpp"
    break;

  case 31: /* fixed_definition: home_decl $@11 ';'  */
#line 552 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2917 "fe/idl.tab.cpp"
    break;

  case 32: /* $@12: %empty  */
#line 556 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventDeclSeen);
        }
#line 2925 "fe/idl.tab.cpp"
    break;

  case 33: /* fixed_definition: event $@12 ';'  */
#line 560 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2933 "fe/idl.tab.cpp"
    break;

  case 34: /* $@13: %empty  */
#line 564 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeDeclSeen);
        }
#line 2941 "fe/idl.tab.cpp"
    break;

  case 35: /* fixed_definition: porttype_decl $@13 ';'  */
#line 568 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2949 "fe/idl.tab.cpp"
    break;

  case 36: /* $@14: %empty  */
#line 572 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorDeclSeen);
        }
#line 2957 "fe/idl.tab.cpp"
    break;

  case 37: /* fixed_definition: connector_decl $@14 ';'  */
#line 576 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2965 "fe/idl.tab.cpp"
    break;

  case 38: /* $@15: %empty  */
#line 580 "fe/idl.ypp"
        {
          idl_global->err ()->syntax_error (idl_global->parse_state ());
        }
#line 2973 "fe/idl.tab.cpp"
    break;

  case 39: /* fixed_definition: error $@15 ';'  */
#line 584 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
          (yyval.dcval) = 0;
        }
#line 2983 "fe/idl.tab.cpp"
    break;

  case 40: /* $@16: %empty  */
#line 593 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSeen);
        }
#line 2991 "fe/idl.tab.cpp"
    break;

  case 41: /* module_header: IDL_MODULE $@16 scoped_name  */
#line 597 "fe/idl.ypp"
        {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 2999 "fe/idl.tab.cpp"
    break;

  case 42: /* @17: %empty  */
#line 604 "fe/idl.ypp"
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
#line 3042 "fe/idl.tab.cpp"
    break;

  case 43: /* $@18: %empty  */
#line 643 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSqSeen);
        }
#line 3050 "fe/idl.tab.cpp"
    break;

  case 44: /* $@19: %empty  */
#line 647 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleBodySeen);
        }
#line 3058 "fe/idl.tab.cpp"
    break;

  case 45: /* module: module_header @17 '{' $@18 at_least_one_definition $@19 '}'  */
#line 651 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleQsSeen);
          /*
           * Finished with this module - pop it from the scope stack.
           */

          idl_global->scopes ().pop ();
          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 3072 "fe/idl.tab.cpp"
    break;

  case 46: /* template_module_header: module_header '<'  */
#line 664 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleIDSeen);
        }
#line 3080 "fe/idl.tab.cpp"
    break;

  case 47: /* $@20: %empty  */
#line 671 "fe/idl.ypp"
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
#line 3098 "fe/idl.tab.cpp"
    break;

  case 48: /* $@21: %empty  */
#line 685 "fe/idl.ypp"
        {
          if (FE_Utils::duplicate_param_id ((yyvsp[0].plval)))
            {
              idl_global->err ()->duplicate_param_id (
                (yyvsp[-2].idlist));

              return 1;
            }
        }
#line 3112 "fe/idl.tab.cpp"
    break;

  case 49: /* $@22: %empty  */
#line 695 "fe/idl.ypp"
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
#line 3146 "fe/idl.tab.cpp"
    break;

  case 50: /* $@23: %empty  */
#line 725 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleSqSeen);
        }
#line 3154 "fe/idl.tab.cpp"
    break;

  case 51: /* $@24: %empty  */
#line 729 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleBodySeen);
        }
#line 3162 "fe/idl.tab.cpp"
    break;

  case 52: /* template_module: template_module_header $@20 at_least_one_formal_parameter $@21 '>' $@22 '{' $@23 at_least_one_tpl_definition $@24 '}'  */
#line 733 "fe/idl.ypp"
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
#line 3185 "fe/idl.tab.cpp"
    break;

  case 58: /* $@25: %empty  */
#line 769 "fe/idl.ypp"
        {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefSeen);
        }
#line 3194 "fe/idl.tab.cpp"
    break;

  case 59: /* $@26: %empty  */
#line 774 "fe/idl.ypp"
        {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefParamsSeen);
        }
#line 3203 "fe/idl.tab.cpp"
    break;

  case 60: /* template_module_ref: IDL_ALIAS scoped_name $@25 '<' at_least_one_formal_parameter_name '>' $@26 defining_id  */
#line 779 "fe/idl.ypp"
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
            dynamic_cast<AST_Template_Module*> (d);

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

          UTL_ScopedName sn ((yyvsp[0].idval));

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
#line 3285 "fe/idl.tab.cpp"
    break;

  case 61: /* $@27: %empty  */
#line 860 "fe/idl.ypp"
        {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleSeen);
        }
#line 3294 "fe/idl.tab.cpp"
    break;

  case 62: /* $@28: %empty  */
#line 865 "fe/idl.ypp"
        {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleArgsSeen);
        }
#line 3303 "fe/idl.tab.cpp"
    break;

  case 63: /* template_module_inst: template_module_header $@27 at_least_one_actual_parameter '>' $@28 defining_id  */
#line 870 "fe/idl.ypp"
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
              ref = dynamic_cast<AST_Template_Module*> (d);

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
#line 3371 "fe/idl.tab.cpp"
    break;

  case 66: /* $@29: %empty  */
#line 942 "fe/idl.ypp"
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
#line 3413 "fe/idl.tab.cpp"
    break;

  case 67: /* $@30: %empty  */
#line 980 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
#line 3421 "fe/idl.tab.cpp"
    break;

  case 68: /* $@31: %empty  */
#line 984 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
#line 3429 "fe/idl.tab.cpp"
    break;

  case 69: /* interface: interface_header $@29 '{' $@30 exports $@31 '}'  */
#line 988 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);

          (yyval.dcval) = dynamic_cast<AST_Decl *> (idl_global->scopes ().top_non_null ());
          /*
           * Done with this interface - pop it off the scopes stack
           */
          idl_global->scopes ().pop ();
        }
#line 3443 "fe/idl.tab.cpp"
    break;

  case 70: /* $@32: %empty  */
#line 1001 "fe/idl.ypp"
         {
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSeen);
         }
#line 3451 "fe/idl.tab.cpp"
    break;

  case 71: /* interface_decl: IDL_INTERFACE $@32 defining_id  */
#line 1005 "fe/idl.ypp"
         {
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceIDSeen);
           (yyval.idval) = (yyvsp[0].idval);
         }
#line 3460 "fe/idl.tab.cpp"
    break;

  case 72: /* interface_header: interface_decl inheritance_spec  */
#line 1013 "fe/idl.ypp"
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
#line 3500 "fe/idl.tab.cpp"
    break;

  case 73: /* interface_header: IDL_LOCAL interface_decl inheritance_spec  */
#line 1050 "fe/idl.ypp"
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
#line 3533 "fe/idl.tab.cpp"
    break;

  case 74: /* interface_header: IDL_ABSTRACT interface_decl inheritance_spec  */
#line 1080 "fe/idl.ypp"
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
#line 3566 "fe/idl.tab.cpp"
    break;

  case 75: /* $@33: %empty  */
#line 1112 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 3574 "fe/idl.tab.cpp"
    break;

  case 76: /* inheritance_spec: ':' opt_truncatable $@33 at_least_one_scoped_name  */
#line 1116 "fe/idl.ypp"
        {
          (yyvsp[0].nlval)->truncatable ((yyvsp[-2].bval));
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3583 "fe/idl.tab.cpp"
    break;

  case 77: /* inheritance_spec: %empty  */
#line 1121 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 3591 "fe/idl.tab.cpp"
    break;

  case 82: /* valuetype: IDL_CUSTOM value_concrete_decl  */
#line 1135 "fe/idl.ypp"
        {
          idl_global->err ()->unsupported_error ("custom is not supported");
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 3600 "fe/idl.tab.cpp"
    break;

  case 84: /* @34: %empty  */
#line 1144 "fe/idl.ypp"
        {
          FE_OBVHeader *&valuetype_header = (yyvsp[0].vhval);
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

              AST_Interface *valuetype_as_interface =
                dynamic_cast<AST_Interface *> (valuetype);
              AST_Interface::fwd_redefinition_helper (
                valuetype_as_interface, scope);
              valuetype = dynamic_cast<AST_ValueType *> (
                valuetype_as_interface);

              /*
               * Add the valuetype to its definition scope
               */
              scope->fe_add_valuetype (valuetype);

              // FE_OBVHeader is not automatically destroyed in the AST
              valuetype_header->destroy ();
              delete valuetype_header;
              valuetype_header = 0;
            }

          /*
           * Push it on the scope stack
           */
          idl_global->scopes ().push (valuetype);

          (yyval.dcval) = valuetype;
        }
#line 3651 "fe/idl.tab.cpp"
    break;

  case 85: /* $@35: %empty  */
#line 1191 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3659 "fe/idl.tab.cpp"
    break;

  case 86: /* $@36: %empty  */
#line 1195 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3667 "fe/idl.tab.cpp"
    break;

  case 87: /* value_concrete_decl: value_header @34 '{' $@35 value_elements $@36 '}'  */
#line 1199 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeQsSeen);

          AST_ValueType *vt =
            dynamic_cast<AST_ValueType*> (
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
#line 3692 "fe/idl.tab.cpp"
    break;

  case 88: /* $@37: %empty  */
#line 1224 "fe/idl.ypp"
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
              i = dynamic_cast<AST_Interface*> (v);
              AST_Interface::fwd_redefinition_helper (i,
                                                      s);
              /*
               * Add the valuetype to its definition scope
               */
              v = dynamic_cast<AST_ValueType*> (i);
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
#line 3739 "fe/idl.tab.cpp"
    break;

  case 89: /* $@38: %empty  */
#line 1267 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3747 "fe/idl.tab.cpp"
    break;

  case 90: /* $@39: %empty  */
#line 1271 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3755 "fe/idl.tab.cpp"
    break;

  case 91: /* value_abs_decl: IDL_ABSTRACT value_header $@37 '{' $@38 exports $@39 '}'  */
#line 1275 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeQsSeen);

          /*
           * Done with this valuetype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = 0;
        }
#line 3770 "fe/idl.tab.cpp"
    break;

  case 92: /* $@40: %empty  */
#line 1290 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 3778 "fe/idl.tab.cpp"
    break;

  case 93: /* value_header: value_decl inheritance_spec $@40 supports_spec  */
#line 1294 "fe/idl.ypp"
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
#line 3813 "fe/idl.tab.cpp"
    break;

  case 94: /* $@41: %empty  */
#line 1328 "fe/idl.ypp"
        {
           idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSeen);
        }
#line 3821 "fe/idl.tab.cpp"
    break;

  case 95: /* value_decl: IDL_VALUETYPE $@41 defining_id  */
#line 1332 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 3830 "fe/idl.tab.cpp"
    break;

  case 96: /* opt_truncatable: IDL_TRUNCATABLE  */
#line 1340 "fe/idl.ypp"
        {
          (yyval.bval) = true;
        }
#line 3838 "fe/idl.tab.cpp"
    break;

  case 97: /* opt_truncatable: %empty  */
#line 1344 "fe/idl.ypp"
        {
          (yyval.bval) = false;
        }
#line 3846 "fe/idl.tab.cpp"
    break;

  case 98: /* supports_spec: IDL_SUPPORTS at_least_one_scoped_name  */
#line 1352 "fe/idl.ypp"
        {
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3854 "fe/idl.tab.cpp"
    break;

  case 99: /* supports_spec: %empty  */
#line 1356 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 3862 "fe/idl.tab.cpp"
    break;

  case 100: /* value_forward_decl: IDL_ABSTRACT value_decl  */
#line 1364 "fe/idl.ypp"
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
#line 3889 "fe/idl.tab.cpp"
    break;

  case 101: /* value_forward_decl: value_decl  */
#line 1388 "fe/idl.ypp"
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
#line 3918 "fe/idl.tab.cpp"
    break;

  case 102: /* value_box_decl: value_decl type_spec  */
#line 1416 "fe/idl.ypp"
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
                = dynamic_cast<AST_Typedef*> ((yyvsp[0].dcval));

              if (td == 0)
                {
                  tp = dynamic_cast<AST_Type*> ((yyvsp[0].dcval));
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
#line 3985 "fe/idl.tab.cpp"
    break;

  case 103: /* value_elements: value_elements at_least_one_annotation value_element  */
#line 1482 "fe/idl.ypp"
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
#line 4008 "fe/idl.tab.cpp"
    break;

  case 104: /* value_elements: value_elements value_element  */
#line 1501 "fe/idl.ypp"
        {
          delete (yyvsp[0].decls_val);
        }
#line 4016 "fe/idl.tab.cpp"
    break;

  case 107: /* value_element: export  */
#line 1510 "fe/idl.ypp"
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
#line 4031 "fe/idl.tab.cpp"
    break;

  case 108: /* @42: %empty  */
#line 1521 "fe/idl.ypp"
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
#line 4046 "fe/idl.tab.cpp"
    break;

  case 109: /* value_element: init_decl @42 ';'  */
#line 1532 "fe/idl.ypp"
        {
          (yyval.decls_val) = (yyvsp[-1].decls_val);
        }
#line 4054 "fe/idl.tab.cpp"
    break;

  case 110: /* visibility: IDL_PUBLIC  */
#line 1539 "fe/idl.ypp"
        {
          (yyval.vival) = AST_Field::vis_PUBLIC;
        }
#line 4062 "fe/idl.tab.cpp"
    break;

  case 111: /* visibility: IDL_PRIVATE  */
#line 1543 "fe/idl.ypp"
        {
          (yyval.vival) = AST_Field::vis_PRIVATE;
        }
#line 4070 "fe/idl.tab.cpp"
    break;

  case 112: /* state_member: visibility member_i  */
#line 1550 "fe/idl.ypp"
        {
          AST_Field::Visibility &visibility = (yyvsp[-1].vival);
          AST_Decls *&decls_ptr = (yyvsp[0].decls_val);
          if (decls_ptr)
            {
              AST_Decls &decls = *decls_ptr;
              for (size_t i = 0; i < decls.size (); i++)
                {
                  AST_Field *field = dynamic_cast<AST_Field *> (decls[i]);
                  if (field)
                    {
                      field->visibility (visibility);
                    }
                }
            }
          (yyval.decls_val) = decls_ptr;
        }
#line 4092 "fe/idl.tab.cpp"
    break;

  case 115: /* at_least_one_export: exports at_least_one_annotation export  */
#line 1576 "fe/idl.ypp"
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
#line 4111 "fe/idl.tab.cpp"
    break;

  case 117: /* $@43: %empty  */
#line 1595 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 4119 "fe/idl.tab.cpp"
    break;

  case 118: /* export: type_dcl $@43 ';'  */
#line 1599 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4127 "fe/idl.tab.cpp"
    break;

  case 119: /* $@44: %empty  */
#line 1603 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 4135 "fe/idl.tab.cpp"
    break;

  case 120: /* export: typeid_dcl $@44 ';'  */
#line 1607 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4143 "fe/idl.tab.cpp"
    break;

  case 121: /* $@45: %empty  */
#line 1611 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 4151 "fe/idl.tab.cpp"
    break;

  case 122: /* export: typeprefix_dcl $@45 ';'  */
#line 1615 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4159 "fe/idl.tab.cpp"
    break;

  case 123: /* $@46: %empty  */
#line 1619 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 4167 "fe/idl.tab.cpp"
    break;

  case 124: /* export: const_dcl $@46 ';'  */
#line 1623 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4175 "fe/idl.tab.cpp"
    break;

  case 125: /* $@47: %empty  */
#line 1627 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 4183 "fe/idl.tab.cpp"
    break;

  case 126: /* export: exception $@47 ';'  */
#line 1631 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4191 "fe/idl.tab.cpp"
    break;

  case 127: /* $@48: %empty  */
#line 1635 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 4199 "fe/idl.tab.cpp"
    break;

  case 128: /* export: attribute $@48 ';'  */
#line 1639 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4207 "fe/idl.tab.cpp"
    break;

  case 129: /* $@49: %empty  */
#line 1643 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpDeclSeen);
        }
#line 4215 "fe/idl.tab.cpp"
    break;

  case 130: /* export: operation $@49 ';'  */
#line 1647 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4223 "fe/idl.tab.cpp"
    break;

  case 131: /* $@50: %empty  */
#line 1651 "fe/idl.ypp"
        {
          idl_global->err ()->syntax_error (idl_global->parse_state ());
        }
#line 4231 "fe/idl.tab.cpp"
    break;

  case 132: /* export: error $@50 ';'  */
#line 1655 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
          (yyval.dcval) = 0;
        }
#line 4241 "fe/idl.tab.cpp"
    break;

  case 133: /* at_least_one_scoped_name: scoped_name scoped_names  */
#line 1664 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.nlval),
                          UTL_NameList ((yyvsp[-1].idlist),
                                        (yyvsp[0].nlval)),
                          1);
        }
#line 4252 "fe/idl.tab.cpp"
    break;

  case 134: /* $@51: %empty  */
#line 1675 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SNListCommaSeen);
        }
#line 4260 "fe/idl.tab.cpp"
    break;

  case 135: /* scoped_names: scoped_names ',' $@51 scoped_name  */
#line 1679 "fe/idl.ypp"
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
#line 4284 "fe/idl.tab.cpp"
    break;

  case 136: /* scoped_names: %empty  */
#line 1699 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 4292 "fe/idl.tab.cpp"
    break;

  case 137: /* scoped_name: id  */
#line 1706 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          ACE_NEW_RETURN ((yyval.idlist),
                          UTL_IdList ((yyvsp[0].idval),
                                      0),
                          1);
        }
#line 4305 "fe/idl.tab.cpp"
    break;

  case 138: /* $@52: %empty  */
#line 1715 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);
        }
#line 4313 "fe/idl.tab.cpp"
    break;

  case 139: /* scoped_name: IDL_SCOPE_DELIMITOR $@52 id  */
#line 1719 "fe/idl.ypp"
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
#line 4337 "fe/idl.tab.cpp"
    break;

  case 140: /* $@53: %empty  */
#line 1740 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);

          // This cleans up all the non-global "::"s in scoped names.
          // If there is a global one, it gets put into the UTL_IdList,
          // so we clean it up in the case above.
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 4351 "fe/idl.tab.cpp"
    break;

  case 141: /* scoped_name: scoped_name IDL_SCOPE_DELIMITOR $@53 id  */
#line 1750 "fe/idl.ypp"
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
#line 4367 "fe/idl.tab.cpp"
    break;

  case 142: /* id: IDENTIFIER  */
#line 1764 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.idval),
                          Identifier ((yyvsp[0].strval)),
                          1);
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 4379 "fe/idl.tab.cpp"
    break;

  case 143: /* defining_id: IDENTIFIER  */
#line 1774 "fe/idl.ypp"
        {
          /* defining_id is a defining identifier
             whereas id is usually a reference to a defining identifier */
          ACE_NEW_RETURN ((yyval.idval), Identifier ((yyvsp[0].strval)), 1);
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 4391 "fe/idl.tab.cpp"
    break;

  case 144: /* interface_forward: interface_decl  */
#line 1785 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval));

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
#line 4438 "fe/idl.tab.cpp"
    break;

  case 145: /* interface_forward: IDL_LOCAL interface_decl  */
#line 1829 "fe/idl.ypp"
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
#line 4468 "fe/idl.tab.cpp"
    break;

  case 146: /* interface_forward: IDL_ABSTRACT interface_decl  */
#line 1856 "fe/idl.ypp"
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

          (yyval.dcval) = dynamic_cast<AST_Decl *> (f);
        }
#line 4500 "fe/idl.tab.cpp"
    break;

  case 147: /* $@54: %empty  */
#line 1887 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstSeen);
        }
#line 4508 "fe/idl.tab.cpp"
    break;

  case 148: /* $@55: %empty  */
#line 1891 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstTypeSeen);
        }
#line 4516 "fe/idl.tab.cpp"
    break;

  case 149: /* $@56: %empty  */
#line 1895 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstIDSeen);
        }
#line 4524 "fe/idl.tab.cpp"
    break;

  case 150: /* $@57: %empty  */
#line 1899 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstAssignSeen);
        }
#line 4532 "fe/idl.tab.cpp"
    break;

  case 151: /* const_dcl: IDL_CONST $@54 const_type $@55 defining_id $@56 '=' $@57 expression  */
#line 1903 "fe/idl.ypp"
        {
          (yyval.dcval) = 0;
          UTL_ScopedName n ((yyvsp[-4].idval));

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
#line 4592 "fe/idl.tab.cpp"
    break;

  case 158: /* const_type: string_type_spec  */
#line 1968 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_string;
        }
#line 4600 "fe/idl.tab.cpp"
    break;

  case 159: /* const_type: wstring_type_spec  */
#line 1972 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_wstring;
        }
#line 4608 "fe/idl.tab.cpp"
    break;

  case 160: /* const_type: scoped_name  */
#line 1976 "fe/idl.ypp"
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
                  t = dynamic_cast<AST_Typedef*> (d);

                  if (t == 0)
                    {
                      break;
                    }

                  d = t->base_type ();
                }

              if (d->node_type () == AST_Decl::NT_pre_defined)
                {
                  c = dynamic_cast<AST_PredefinedType*> (d);

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
                    dynamic_cast<AST_Enum*> (d);
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
#line 4678 "fe/idl.tab.cpp"
    break;

  case 164: /* or_expr: or_expr '|' xor_expr  */
#line 2049 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_or,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4691 "fe/idl.tab.cpp"
    break;

  case 166: /* xor_expr: xor_expr '^' and_expr  */
#line 2062 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_xor,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4704 "fe/idl.tab.cpp"
    break;

  case 168: /* and_expr: and_expr '&' shift_expr  */
#line 2075 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_and,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4717 "fe/idl.tab.cpp"
    break;

  case 170: /* shift_expr: shift_expr IDL_LEFT_SHIFT add_expr  */
#line 2088 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_left,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4730 "fe/idl.tab.cpp"
    break;

  case 171: /* shift_expr: shift_expr IDL_RIGHT_SHIFT add_expr  */
#line 2097 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_right,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4743 "fe/idl.tab.cpp"
    break;

  case 173: /* add_expr: add_expr '+' mult_expr  */
#line 2110 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_add,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4756 "fe/idl.tab.cpp"
    break;

  case 174: /* add_expr: add_expr '-' mult_expr  */
#line 2119 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_minus,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4769 "fe/idl.tab.cpp"
    break;

  case 176: /* mult_expr: mult_expr '*' unary_expr  */
#line 2132 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mul,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4782 "fe/idl.tab.cpp"
    break;

  case 177: /* mult_expr: mult_expr '/' unary_expr  */
#line 2141 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_div,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4795 "fe/idl.tab.cpp"
    break;

  case 178: /* mult_expr: mult_expr '%' unary_expr  */
#line 2150 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mod,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4808 "fe/idl.tab.cpp"
    break;

  case 180: /* unary_expr: '+' primary_expr  */
#line 2163 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_u_plus,
                                    (yyvsp[0].exval),
                                    0
                                  );
        }
#line 4821 "fe/idl.tab.cpp"
    break;

  case 181: /* unary_expr: '-' primary_expr  */
#line 2172 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                   AST_Expression::EC_u_minus,
                                   (yyvsp[0].exval),
                                   0
                                 );
        }
#line 4834 "fe/idl.tab.cpp"
    break;

  case 182: /* unary_expr: '~' primary_expr  */
#line 2181 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                   AST_Expression::EC_bit_neg,
                                   (yyvsp[0].exval),
                                   0
                                 );
        }
#line 4847 "fe/idl.tab.cpp"
    break;

  case 183: /* primary_expr: scoped_name  */
#line 2193 "fe/idl.ypp"
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
#line 4908 "fe/idl.tab.cpp"
    break;

  case 185: /* primary_expr: '(' const_expr ')'  */
#line 2251 "fe/idl.ypp"
        {
          (yyval.exval) = (yyvsp[-1].exval);
        }
#line 4916 "fe/idl.tab.cpp"
    break;

  case 186: /* literal: IDL_INTEGER_LITERAL  */
#line 2258 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].ival));
        }
#line 4924 "fe/idl.tab.cpp"
    break;

  case 187: /* literal: IDL_UINTEGER_LITERAL  */
#line 2262 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr ((yyvsp[0].uival));
        }
#line 4933 "fe/idl.tab.cpp"
    break;

  case 188: /* literal: IDL_STRING_LITERAL  */
#line 2267 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].sval));
          (yyvsp[0].sval)->destroy ();
          delete (yyvsp[0].sval);
          (yyvsp[0].sval) = 0;
        }
#line 4944 "fe/idl.tab.cpp"
    break;

  case 189: /* literal: IDL_WSTRING_LITERAL  */
#line 2274 "fe/idl.ypp"
        {
          char *wide_string = (yyvsp[0].wsval);
          (yyval.exval) = idl_global->gen ()->create_expr (wide_string);
          ACE_OS::free (wide_string);
          (yyvsp[0].wsval) = 0;
        }
#line 4955 "fe/idl.tab.cpp"
    break;

  case 190: /* literal: IDL_CHARACTER_LITERAL  */
#line 2281 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].cval));
        }
#line 4963 "fe/idl.tab.cpp"
    break;

  case 191: /* literal: IDL_WCHAR_LITERAL  */
#line 2285 "fe/idl.ypp"
        {
          ACE_OutputCDR::from_wchar wc ((yyvsp[0].wcval));
          (yyval.exval) = idl_global->gen ()->create_expr (wc);
        }
#line 4972 "fe/idl.tab.cpp"
    break;

  case 192: /* literal: IDL_FIXED_PT_LITERAL  */
#line 2290 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].fixval));
        }
#line 4980 "fe/idl.tab.cpp"
    break;

  case 193: /* literal: IDL_FLOATING_PT_LITERAL  */
#line 2294 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].dval));
        }
#line 4988 "fe/idl.tab.cpp"
    break;

  case 194: /* literal: IDL_TRUETOK  */
#line 2298 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr (true);
        }
#line 4996 "fe/idl.tab.cpp"
    break;

  case 195: /* literal: IDL_FALSETOK  */
#line 2302 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr (false);
        }
#line 5004 "fe/idl.tab.cpp"
    break;

  case 196: /* positive_int_expr: const_expr  */
#line 2309 "fe/idl.ypp"
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
#line 5073 "fe/idl.tab.cpp"
    break;

  case 197: /* $@58: %empty  */
#line 2377 "fe/idl.ypp"
        {
          if (idl_global->idl_version_ < IDL_VERSION_4)
            idl_global->err ()->idl_version_error (
              "Annotations are not allowed in IDL versions before 4");

          Identifier *id = (yyvsp[-1].idval);
          UTL_ScopedName name (id);
          AST_Annotation_Decl::escape_name (&name);
          AST_Annotation_Decl *annotation_decl = idl_global->gen ()->
            create_annotation_decl (&name);
          idl_global->scopes ().top_non_null ()->
            fe_add_annotation_decl (annotation_decl);
          idl_global->scopes ().push (annotation_decl);
        }
#line 5092 "fe/idl.tab.cpp"
    break;

  case 198: /* annotation_dcl: IDL_ANNOTATION_DECL defining_id '{' $@58 annotation_body '}'  */
#line 2392 "fe/idl.ypp"
        {
          Identifier *id = (yyvsp[-4].idval);
          idl_global->scopes ().pop ();
          id->destroy ();
          delete id;

          (yyval.dcval) = 0;
        }
#line 5105 "fe/idl.tab.cpp"
    break;

  case 204: /* $@59: %empty  */
#line 2412 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
          idl_global->in_typedef (true);
        }
#line 5114 "fe/idl.tab.cpp"
    break;

  case 208: /* annotation_member: annotation_member_type defining_id annotation_member_default ';'  */
#line 2426 "fe/idl.ypp"
        {
          UTL_Scope *scope = idl_global->scopes ().top_non_null ();
          UTL_Scope *root = idl_global->scopes ().bottom ();

          AST_Expression::ExprType type = (yyvsp[-3].etval);
          Identifier *name = (yyvsp[-2].idval);
          // Annotation member names can't clash with C++ keywords
          FE_Utils::original_local_name (name);
          UTL_ScopedName *scoped_name = new UTL_ScopedName (name);
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
#line 5171 "fe/idl.tab.cpp"
    break;

  case 209: /* annotation_member_default: IDL_DEFAULT const_expr  */
#line 2482 "fe/idl.ypp"
        {
          (yyval.exval) = (yyvsp[0].exval);
        }
#line 5179 "fe/idl.tab.cpp"
    break;

  case 210: /* annotation_member_default: %empty  */
#line 2486 "fe/idl.ypp"
        {
          (yyval.exval) = 0;
        }
#line 5187 "fe/idl.tab.cpp"
    break;

  case 211: /* at_least_one_annotation: annotations_maybe annotation_appl  */
#line 2493 "fe/idl.ypp"
        {
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          AST_Annotation_Appl *annotation = (yyvsp[0].annotation_val);
          if (annotation)
            {
              annotations->add (annotation);
            }
          (yyval.annotations_val) = annotations;
        }
#line 5201 "fe/idl.tab.cpp"
    break;

  case 212: /* annotations_maybe: annotations_maybe annotation_appl  */
#line 2506 "fe/idl.ypp"
        {
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          AST_Annotation_Appl *annotation = (yyvsp[0].annotation_val);
          if (annotation)
            {
              annotations->add (annotation);
            }
          (yyval.annotations_val) = annotations;
        }
#line 5215 "fe/idl.tab.cpp"
    break;

  case 213: /* annotations_maybe: %empty  */
#line 2516 "fe/idl.ypp"
        {
          (yyval.annotations_val) = new AST_Annotation_Appls ();
        }
#line 5223 "fe/idl.tab.cpp"
    break;

  case 214: /* @60: %empty  */
#line 2523 "fe/idl.ypp"
        {
          if (idl_global->idl_version_ < IDL_VERSION_4)
            idl_global->err ()->idl_version_error (
              "Annotations are not allowed in IDL versions before 4");

          AST_Annotation_Decl *decl = 0;
          UTL_ScopedName *name = (yyvsp[0].idlist);
          AST_Annotation_Decl::escape_name (name);

          // Check for Matching Annotation Declaration
          AST_Decl *d = idl_global->scopes ().top_non_null ()->
            lookup_by_name (name);
          if (d)
            {
              decl = dynamic_cast<AST_Annotation_Decl*> (d);
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
#line 5282 "fe/idl.tab.cpp"
    break;

  case 215: /* annotation_appl: IDL_ANNOTATION_SYMBOL scoped_name @60 annotation_appl_params_maybe  */
#line 2578 "fe/idl.ypp"
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
#line 5314 "fe/idl.tab.cpp"
    break;

  case 216: /* annotation_appl_params_maybe: '(' annotation_appl_params ')'  */
#line 2609 "fe/idl.ypp"
        {
          (yyval.annotation_params_val) = (yyvsp[-1].annotation_params_val);
        }
#line 5322 "fe/idl.tab.cpp"
    break;

  case 217: /* annotation_appl_params_maybe: %empty  */
#line 2613 "fe/idl.ypp"
        {
          (yyval.annotation_params_val) = 0;
        }
#line 5330 "fe/idl.tab.cpp"
    break;

  case 218: /* annotation_appl_params: const_expr  */
#line 2620 "fe/idl.ypp"
        {
          AST_Annotation_Appl::Params *params = new AST_Annotation_Appl::Params;
          AST_Annotation_Appl::Param *param = new AST_Annotation_Appl::Param;
          param->id = 0;
          param->expr = (yyvsp[0].exval);
          params->push (param);
          (yyval.annotation_params_val) = params;
        }
#line 5343 "fe/idl.tab.cpp"
    break;

  case 219: /* annotation_appl_params: named_annotation_appl_params  */
#line 2629 "fe/idl.ypp"
        {
          (yyval.annotation_params_val) = (yyvsp[0].annotation_params_val);
        }
#line 5351 "fe/idl.tab.cpp"
    break;

  case 220: /* annotation_appl_params: %empty  */
#line 2633 "fe/idl.ypp"
        {
          (yyval.annotation_params_val) = 0;
        }
#line 5359 "fe/idl.tab.cpp"
    break;

  case 221: /* named_annotation_appl_params: named_annotation_appl_param more_named_annotation_appl_params  */
#line 2640 "fe/idl.ypp"
        {
          AST_Annotation_Appl::Params *params = (yyvsp[0].annotation_params_val);
          params->push ((yyvsp[-1].annotation_param_val));
          (yyval.annotation_params_val) = params;
        }
#line 5369 "fe/idl.tab.cpp"
    break;

  case 222: /* more_named_annotation_appl_params: ',' named_annotation_appl_param more_named_annotation_appl_params  */
#line 2648 "fe/idl.ypp"
        {
          AST_Annotation_Appl::Params *params = (yyvsp[0].annotation_params_val);
          params->push ((yyvsp[-1].annotation_param_val));
          (yyval.annotation_params_val) = params;
        }
#line 5379 "fe/idl.tab.cpp"
    break;

  case 223: /* more_named_annotation_appl_params: %empty  */
#line 2654 "fe/idl.ypp"
        {
          (yyval.annotation_params_val) = new AST_Annotation_Appl::Params;
        }
#line 5387 "fe/idl.tab.cpp"
    break;

  case 224: /* named_annotation_appl_param: id '=' const_expr  */
#line 2661 "fe/idl.ypp"
        {
          AST_Annotation_Appl::Param *param = new AST_Annotation_Appl::Param;
          param->id = (yyvsp[-2].idval);
          // Annotation Member Names Can't Clash with C++ keywords
          FE_Utils::original_local_name (param->id);
          param->expr = (yyvsp[0].exval);
          (yyval.annotation_param_val) = param;
        }
#line 5400 "fe/idl.tab.cpp"
    break;

  case 225: /* $@61: %empty  */
#line 2673 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
          idl_global->in_typedef (true);
        }
#line 5409 "fe/idl.tab.cpp"
    break;

  case 226: /* type_dcl: IDL_TYPEDEF $@61 type_declarator  */
#line 2678 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5417 "fe/idl.tab.cpp"
    break;

  case 227: /* type_dcl: struct_type  */
#line 2682 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5425 "fe/idl.tab.cpp"
    break;

  case 228: /* type_dcl: union_type  */
#line 2686 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5433 "fe/idl.tab.cpp"
    break;

  case 229: /* type_dcl: enum_type  */
#line 2690 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5441 "fe/idl.tab.cpp"
    break;

  case 230: /* type_dcl: IDL_NATIVE simple_declarator  */
#line 2694 "fe/idl.ypp"
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
#line 5473 "fe/idl.tab.cpp"
    break;

  case 231: /* type_dcl: constructed_forward_type_spec  */
#line 2722 "fe/idl.ypp"
        {
          (yyval.dcval) = 0;
        }
#line 5481 "fe/idl.tab.cpp"
    break;

  case 232: /* $@62: %empty  */
#line 2729 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeSpecSeen);
        }
#line 5489 "fe/idl.tab.cpp"
    break;

  case 233: /* type_declarator: type_spec $@62 at_least_one_declarator  */
#line 2733 "fe/idl.ypp"
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
#line 5557 "fe/idl.tab.cpp"
    break;

  case 236: /* simple_type_spec: base_type_spec  */
#line 2805 "fe/idl.ypp"
        {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5568 "fe/idl.tab.cpp"
    break;

  case 238: /* simple_type_spec: scoped_name  */
#line 2813 "fe/idl.ypp"
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
#line 5595 "fe/idl.tab.cpp"
    break;

  case 257: /* at_least_one_declarator: declarator declarators  */
#line 2869 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 5606 "fe/idl.tab.cpp"
    break;

  case 258: /* $@63: %empty  */
#line 2880 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 5614 "fe/idl.tab.cpp"
    break;

  case 259: /* declarators: declarators ',' $@63 declarator  */
#line 2884 "fe/idl.ypp"
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
#line 5638 "fe/idl.tab.cpp"
    break;

  case 260: /* declarators: %empty  */
#line 2904 "fe/idl.ypp"
        {
          (yyval.dlval) = 0;
        }
#line 5646 "fe/idl.tab.cpp"
    break;

  case 263: /* at_least_one_simple_declarator: simple_declarator simple_declarators  */
#line 2916 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 5657 "fe/idl.tab.cpp"
    break;

  case 264: /* $@64: %empty  */
#line 2927 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 5665 "fe/idl.tab.cpp"
    break;

  case 265: /* simple_declarators: simple_declarators ',' $@64 simple_declarator  */
#line 2931 "fe/idl.ypp"
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
#line 5689 "fe/idl.tab.cpp"
    break;

  case 266: /* simple_declarators: %empty  */
#line 2951 "fe/idl.ypp"
        {
          (yyval.dlval) = 0;
        }
#line 5697 "fe/idl.tab.cpp"
    break;

  case 267: /* simple_declarator: defining_id  */
#line 2958 "fe/idl.ypp"
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
#line 5714 "fe/idl.tab.cpp"
    break;

  case 268: /* complex_declarator: array_declarator  */
#line 2974 "fe/idl.ypp"
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
#line 5733 "fe/idl.tab.cpp"
    break;

  case 271: /* signed_int: IDL_LONG  */
#line 2997 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_long;
        }
#line 5741 "fe/idl.tab.cpp"
    break;

  case 272: /* signed_int: IDL_LONG IDL_LONG  */
#line 3001 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_longlong;
        }
#line 5749 "fe/idl.tab.cpp"
    break;

  case 273: /* signed_int: IDL_SHORT  */
#line 3005 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_short;
        }
#line 5757 "fe/idl.tab.cpp"
    break;

  case 274: /* signed_int: IDL_INT8  */
#line 3009 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_int8;
        }
#line 5765 "fe/idl.tab.cpp"
    break;

  case 275: /* signed_int: IDL_INT16  */
#line 3013 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_short;
        }
#line 5773 "fe/idl.tab.cpp"
    break;

  case 276: /* signed_int: IDL_INT32  */
#line 3017 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_long;
        }
#line 5781 "fe/idl.tab.cpp"
    break;

  case 277: /* signed_int: IDL_INT64  */
#line 3021 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_longlong;
        }
#line 5789 "fe/idl.tab.cpp"
    break;

  case 278: /* unsigned_int: IDL_UNSIGNED IDL_LONG  */
#line 3028 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ulong;
        }
#line 5797 "fe/idl.tab.cpp"
    break;

  case 279: /* unsigned_int: IDL_UNSIGNED IDL_LONG IDL_LONG  */
#line 3032 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ulonglong;
        }
#line 5805 "fe/idl.tab.cpp"
    break;

  case 280: /* unsigned_int: IDL_UNSIGNED IDL_SHORT  */
#line 3036 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ushort;
        }
#line 5813 "fe/idl.tab.cpp"
    break;

  case 281: /* unsigned_int: IDL_UINT8  */
#line 3040 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_uint8;
        }
#line 5821 "fe/idl.tab.cpp"
    break;

  case 282: /* unsigned_int: IDL_UINT16  */
#line 3044 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ushort;
        }
#line 5829 "fe/idl.tab.cpp"
    break;

  case 283: /* unsigned_int: IDL_UINT32  */
#line 3048 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ulong;
        }
#line 5837 "fe/idl.tab.cpp"
    break;

  case 284: /* unsigned_int: IDL_UINT64  */
#line 3052 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ulonglong;
        }
#line 5845 "fe/idl.tab.cpp"
    break;

  case 285: /* floating_pt_type: IDL_DOUBLE  */
#line 3059 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_double;
        }
#line 5853 "fe/idl.tab.cpp"
    break;

  case 286: /* floating_pt_type: IDL_FLOAT  */
#line 3063 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_float;
        }
#line 5861 "fe/idl.tab.cpp"
    break;

  case 287: /* floating_pt_type: IDL_LONG IDL_DOUBLE  */
#line 3067 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_longdouble;
        }
#line 5869 "fe/idl.tab.cpp"
    break;

  case 288: /* fixed_type: IDL_FIXED  */
#line 3074 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_fixed;
        }
#line 5877 "fe/idl.tab.cpp"
    break;

  case 289: /* char_type: IDL_CHAR  */
#line 3081 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_char;
        }
#line 5885 "fe/idl.tab.cpp"
    break;

  case 290: /* char_type: IDL_WCHAR  */
#line 3085 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_wchar;
        }
#line 5893 "fe/idl.tab.cpp"
    break;

  case 291: /* octet_type: IDL_OCTET  */
#line 3092 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_octet;
        }
#line 5901 "fe/idl.tab.cpp"
    break;

  case 292: /* boolean_type: IDL_BOOLEAN  */
#line 3099 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_bool;
        }
#line 5909 "fe/idl.tab.cpp"
    break;

  case 293: /* any_type: IDL_ANY  */
#line 3106 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_any;
        }
#line 5917 "fe/idl.tab.cpp"
    break;

  case 294: /* object_type: IDL_OBJECT  */
#line 3113 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_object;
        }
#line 5925 "fe/idl.tab.cpp"
    break;

  case 295: /* $@65: %empty  */
#line 3120 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSeen);
        }
#line 5933 "fe/idl.tab.cpp"
    break;

  case 296: /* struct_decl: IDL_STRUCT $@65 defining_id  */
#line 3124 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 5942 "fe/idl.tab.cpp"
    break;

  case 297: /* $@66: %empty  */
#line 3133 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval));
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
#line 5977 "fe/idl.tab.cpp"
    break;

  case 298: /* $@67: %empty  */
#line 3164 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSqSeen);
        }
#line 5985 "fe/idl.tab.cpp"
    break;

  case 299: /* struct_type: struct_decl $@66 '{' $@67 struct_body  */
#line 3168 "fe/idl.ypp"
        {
          /*
           * Done with this struct. Pop its scope off the scopes stack.
           */
          (yyval.dcval) = dynamic_cast<AST_Structure*> (
                   idl_global->scopes ().top_non_null ()
                 );
          idl_global->scopes ().pop ();
        }
#line 5999 "fe/idl.tab.cpp"
    break;

  case 300: /* struct_body: '}'  */
#line 3181 "fe/idl.ypp"
        {
          if (idl_global->idl_version_ < IDL_VERSION_4)
            idl_global->err ()->idl_version_error (
              "Empty structs are not allowed in IDL versions before 4");

          idl_global->set_parse_state (IDL_GlobalData::PS_StructQsSeen);
        }
#line 6011 "fe/idl.tab.cpp"
    break;

  case 302: /* $@68: %empty  */
#line 3193 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructBodySeen);
        }
#line 6019 "fe/idl.tab.cpp"
    break;

  case 303: /* struct_body_with_members: member members $@68 '}'  */
#line 3197 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructQsSeen);
        }
#line 6027 "fe/idl.tab.cpp"
    break;

  case 306: /* member: annotations_maybe member_i  */
#line 3209 "fe/idl.ypp"
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
#line 6045 "fe/idl.tab.cpp"
    break;

  case 307: /* $@69: %empty  */
#line 3226 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberTypeSeen);
        }
#line 6053 "fe/idl.tab.cpp"
    break;

  case 308: /* $@70: %empty  */
#line 3230 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsSeen);
        }
#line 6061 "fe/idl.tab.cpp"
    break;

  case 309: /* member_i: type_spec $@69 at_least_one_declarator $@70 ';'  */
#line 3234 "fe/idl.ypp"
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
#line 6119 "fe/idl.tab.cpp"
    break;

  case 310: /* $@71: %empty  */
#line 3288 "fe/idl.ypp"
        {
          idl_global->err ()->syntax_error (idl_global->parse_state ());
        }
#line 6127 "fe/idl.tab.cpp"
    break;

  case 311: /* member_i: error $@71 ';'  */
#line 3292 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 6136 "fe/idl.tab.cpp"
    break;

  case 312: /* $@72: %empty  */
#line 3300 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSeen);
        }
#line 6144 "fe/idl.tab.cpp"
    break;

  case 313: /* union_decl: IDL_UNION $@72 defining_id  */
#line 3304 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 6153 "fe/idl.tab.cpp"
    break;

  case 314: /* $@73: %empty  */
#line 3312 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchSeen);
        }
#line 6161 "fe/idl.tab.cpp"
    break;

  case 315: /* $@74: %empty  */
#line 3316 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[-3].idval));
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

              AST_Structure *st = dynamic_cast<AST_Structure*> (u);
              AST_Structure::fwd_redefinition_helper (st, s);
              u = dynamic_cast<AST_Union*> (st);
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
#line 6198 "fe/idl.tab.cpp"
    break;

  case 316: /* $@75: %empty  */
#line 3349 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchTypeSeen);
        }
#line 6206 "fe/idl.tab.cpp"
    break;

  case 317: /* $@76: %empty  */
#line 3353 "fe/idl.ypp"
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
          UTL_ScopedName n ((yyvsp[-8].idval));
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchCloseParSeen);

          /*
           * Create a node representing a union.
           */
          if ((yyvsp[-2].dcval) != 0 && s != 0)
            {
              AST_ConcreteType *tp =
                dynamic_cast<AST_ConcreteType*> ((yyvsp[-2].dcval));

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
                  AST_Union *e = dynamic_cast<AST_Union*> (top);

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
#line 6269 "fe/idl.tab.cpp"
    break;

  case 318: /* $@77: %empty  */
#line 3412 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSqSeen);
        }
#line 6277 "fe/idl.tab.cpp"
    break;

  case 319: /* $@78: %empty  */
#line 3416 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionBodySeen);
        }
#line 6285 "fe/idl.tab.cpp"
    break;

  case 320: /* union_type: union_decl IDL_SWITCH $@73 '(' $@74 annotations_maybe switch_type_spec $@75 ')' $@76 '{' $@77 at_least_one_case_branch $@78 '}'  */
#line 3420 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionQsSeen);

          /*
           * Done with this union. Pop its scope from the scopes stack.
           */
          (yyval.dcval) = dynamic_cast<AST_Union*> (
                   idl_global->scopes ().top_non_null ()
                 );

          if ((yyval.dcval) != 0)
            {
              idl_global->scopes ().pop ();
            }
        }
#line 6305 "fe/idl.tab.cpp"
    break;

  case 321: /* switch_type_spec: integer_type  */
#line 3439 "fe/idl.ypp"
        {
          (yyval.dcval) = idl_global->scopes ().bottom ()->lookup_primitive_type ((yyvsp[0].etval));
        }
#line 6313 "fe/idl.tab.cpp"
    break;

  case 322: /* switch_type_spec: char_type  */
#line 3443 "fe/idl.ypp"
        {
          if ((yyvsp[0].etval) == AST_Expression::EV_wchar && idl_global->idl_version_ < IDL_VERSION_4)
            idl_global->err ()->idl_version_error (
              "Using wchar as a union discriminator isn't allowed in IDL versions before 4");

          (yyval.dcval) = idl_global->scopes ().bottom ()->lookup_primitive_type ((yyvsp[0].etval));
        }
#line 6325 "fe/idl.tab.cpp"
    break;

  case 323: /* switch_type_spec: octet_type  */
#line 3451 "fe/idl.ypp"
        {
          if (idl_global->idl_version_ < IDL_VERSION_4)
            idl_global->err ()->idl_version_error (
              "Using octet as a union discriminator isn't allowed in IDL versions before 4");

          (yyval.dcval) = idl_global->scopes ().bottom ()->lookup_primitive_type ((yyvsp[0].etval));
        }
#line 6337 "fe/idl.tab.cpp"
    break;

  case 324: /* switch_type_spec: boolean_type  */
#line 3459 "fe/idl.ypp"
        {
          (yyval.dcval) = idl_global->scopes ().bottom ()->lookup_primitive_type ((yyvsp[0].etval));
        }
#line 6345 "fe/idl.tab.cpp"
    break;

  case 326: /* switch_type_spec: scoped_name  */
#line 3464 "fe/idl.ypp"
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
          d = s->lookup_by_name ((yyvsp[0].idlist));

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
                      p = dynamic_cast<AST_PredefinedType*> (d);

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
                            case AST_PredefinedType::PT_int8:
                            case AST_PredefinedType::PT_uint8:
                            case AST_PredefinedType::PT_char:
                            case AST_PredefinedType::PT_boolean:
                              (yyval.dcval) = p;
                              found = true;
                              break;
                            case AST_PredefinedType::PT_wchar:
                            case AST_PredefinedType::PT_octet:
                              if (idl_global->idl_version_ < IDL_VERSION_4)
                                idl_global->err ()->idl_version_error (
                                  "Using octet or wchar as a union discriminator isn't allowed in IDL versions before 4");
                              (yyval.dcval) = p;
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
                      t = dynamic_cast<AST_Typedef*> (d);

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
#line 6456 "fe/idl.tab.cpp"
    break;

  case 330: /* $@79: %empty  */
#line 3581 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionLabelSeen);
        }
#line 6464 "fe/idl.tab.cpp"
    break;

  case 331: /* $@80: %empty  */
#line 3585 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemSeen);
        }
#line 6472 "fe/idl.tab.cpp"
    break;

  case 332: /* case_branch: at_least_one_case_label $@79 annotations_maybe element_spec $@80 ';'  */
#line 3589 "fe/idl.ypp"
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
#line 6508 "fe/idl.tab.cpp"
    break;

  case 333: /* $@81: %empty  */
#line 3621 "fe/idl.ypp"
        {
          idl_global->err ()->syntax_error (idl_global->parse_state ());
        }
#line 6516 "fe/idl.tab.cpp"
    break;

  case 334: /* case_branch: error $@81 ';'  */
#line 3625 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 6525 "fe/idl.tab.cpp"
    break;

  case 335: /* at_least_one_case_label: case_label case_labels  */
#line 3633 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.llval),
                          UTL_LabelList ((yyvsp[-1].ulval),
                                         (yyvsp[0].llval)),
                          1);
        }
#line 6536 "fe/idl.tab.cpp"
    break;

  case 336: /* case_labels: case_labels case_label  */
#line 3643 "fe/idl.ypp"
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
#line 6558 "fe/idl.tab.cpp"
    break;

  case 337: /* case_labels: %empty  */
#line 3661 "fe/idl.ypp"
        {
          (yyval.llval) = 0;
        }
#line 6566 "fe/idl.tab.cpp"
    break;

  case 338: /* $@82: %empty  */
#line 3668 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DefaultSeen);
        }
#line 6574 "fe/idl.tab.cpp"
    break;

  case 339: /* case_label: IDL_DEFAULT $@82 ':'  */
#line 3672 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen ()->create_union_label (
                                       AST_UnionLabel::UL_default,
                                       0
                                     );
        }
#line 6587 "fe/idl.tab.cpp"
    break;

  case 340: /* $@83: %empty  */
#line 3681 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_CaseSeen);
        }
#line 6595 "fe/idl.tab.cpp"
    break;

  case 341: /* $@84: %empty  */
#line 3685 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelExprSeen);
        }
#line 6603 "fe/idl.tab.cpp"
    break;

  case 342: /* case_label: IDL_CASE $@83 const_expr $@84 ':'  */
#line 3689 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen ()->create_union_label (
                                      AST_UnionLabel::UL_label,
                                      (yyvsp[-2].exval)
                                    );
        }
#line 6616 "fe/idl.tab.cpp"
    break;

  case 343: /* $@85: %empty  */
#line 3701 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemTypeSeen);
        }
#line 6624 "fe/idl.tab.cpp"
    break;

  case 344: /* element_spec: type_spec $@85 declarator  */
#line 3705 "fe/idl.ypp"
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
#line 6671 "fe/idl.tab.cpp"
    break;

  case 345: /* struct_forward_type: struct_decl  */
#line 3751 "fe/idl.ypp"
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
#line 6697 "fe/idl.tab.cpp"
    break;

  case 346: /* union_forward_type: union_decl  */
#line 3776 "fe/idl.ypp"
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
#line 6721 "fe/idl.tab.cpp"
    break;

  case 347: /* $@86: %empty  */
#line 3799 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSeen);
        }
#line 6729 "fe/idl.tab.cpp"
    break;

  case 348: /* $@87: %empty  */
#line 3803 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval));
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
#line 6764 "fe/idl.tab.cpp"
    break;

  case 349: /* $@88: %empty  */
#line 3834 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSqSeen);
        }
#line 6772 "fe/idl.tab.cpp"
    break;

  case 350: /* $@89: %empty  */
#line 3838 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumBodySeen);
        }
#line 6780 "fe/idl.tab.cpp"
    break;

  case 351: /* enum_type: IDL_ENUM $@86 defining_id $@87 '{' $@88 at_least_one_enumerator $@89 '}'  */
#line 3842 "fe/idl.ypp"
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
              (yyval.dcval) = dynamic_cast<AST_Enum*> (
                       idl_global->scopes ().top_non_null ()
                     );
              idl_global->scopes ().pop ();
            }
        }
#line 6803 "fe/idl.tab.cpp"
    break;

  case 353: /* $@90: %empty  */
#line 3867 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumCommaSeen);
        }
#line 6811 "fe/idl.tab.cpp"
    break;

  case 356: /* enumerator: annotations_maybe IDENTIFIER  */
#line 3876 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          Identifier id ((yyvsp[0].strval));
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
          UTL_ScopedName n (&id);
          AST_EnumVal *e = 0;
          AST_Enum *c = 0;

          /*
           * Create a node representing one enumerator in an enum
           * Add it to the enclosing scope (the enum scope)
           */
          if (s != 0
              && s->scope_node_type () == AST_Decl::NT_enum)
            {
              c = dynamic_cast<AST_Enum*> (s);

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
#line 6850 "fe/idl.tab.cpp"
    break;

  case 357: /* map_type_spec: map_head ',' positive_int_expr '>'  */
#line 3914 "fe/idl.ypp"
        {
          AST_Map *map = nullptr;
          Decl_Annotations_Pair_Pair *type_pair = (yyvsp[-3].decl_annotations_pair_val_pair);
          Decl_Annotations_Pair *key_type = type_pair->first;
          Decl_Annotations_Pair *val_type = type_pair->second;
          delete type_pair;
          type_pair = nullptr;
          AST_Expression *max_size = (yyvsp[-1].exval);

          /*
           * Remove map marker from scopes stack.
           */
          if (!idl_global->scopes ().top ())
            {
              idl_global->scopes ().pop ();
            }

          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          AST_Expression::AST_ExprValue *ev = nullptr;
          AST_Param_Holder *param_holder = nullptr;
          if (max_size)
            {
              param_holder = max_size->param_holder ();
              ev = max_size->coerce (AST_Expression::EV_ulong);
            }

          // If the expression corresponds to a template parameter,
          // it's ok for the coercion to fail at this point. We check
          // for a type mismatch below.
          if (!max_size || (!ev && !param_holder))
            {
              idl_global->err ()->coercion_error (max_size, AST_Expression::EV_ulong);
            }
          else if (key_type && val_type)
            {
              /*
               * Create a node representing a map.
               */
              AST_Type *ktp = dynamic_cast<AST_Type*> (key_type->decl);
              AST_Type *vtp = dynamic_cast<AST_Type*> (val_type->decl);

              if (ktp && vtp)
                {
                  Identifier id ("map");
                  UTL_ScopedName sn (&id);

                  map =
                    idl_global->gen ()->create_map (
                        max_size,
                        ktp,
                        vtp,
                        &sn,
                        s->is_local (),
                        s->is_abstract ()
                      );
                  map->key_type_annotations (*key_type->annotations);
                  map->value_type_annotations (*val_type->annotations);

                  idl_global->err ()->anonymous_type_diagnostic ();
                }
            }

          delete key_type->annotations;
          delete key_type;
          delete val_type->annotations;
          delete val_type;
          delete ev;
          (yyval.dcval) = map;
        }
#line 6925 "fe/idl.tab.cpp"
    break;

  case 358: /* map_type_spec: map_head '>'  */
#line 3985 "fe/idl.ypp"
        {
          AST_Map *map = nullptr;
          Decl_Annotations_Pair_Pair *type_pair = (yyvsp[-1].decl_annotations_pair_val_pair);
          Decl_Annotations_Pair *key_type = type_pair->first;
          Decl_Annotations_Pair *val_type = type_pair->second;
          delete type_pair;
          type_pair = nullptr;

          /*
           * Remove map marker from scopes stack.
           */
          if (!idl_global->scopes ().top ())
            {
              idl_global->scopes ().pop ();
            }

          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          /*
           * Create a node representing a map.
           */
          if (key_type && val_type)
            {
              AST_Type *ktp = dynamic_cast<AST_Type*> (key_type->decl);
              AST_Type *vtp = dynamic_cast<AST_Type*> (val_type->decl);

              if (key_type && val_type)
                {
                  Identifier id ("map");
                  UTL_ScopedName sn (&id);
                  ACE_CDR::ULong bound = 0UL;

                  map =
                    idl_global->gen ()->create_map (
                        idl_global->gen ()->create_expr (
                                                bound,
                                                AST_Expression::EV_ulong
                                              ),
                        ktp,
                        vtp,
                        &sn,
                        s->is_local (),
                        s->is_abstract ()
                      );
                  map->key_type_annotations (*key_type->annotations);
                  map->value_type_annotations (*val_type->annotations);

                  idl_global->err ()->anonymous_type_diagnostic ();
                }
            }

          delete key_type->annotations;
          delete key_type;
          delete val_type->annotations;
          delete val_type;
          (yyval.dcval) = map;
        }
#line 6987 "fe/idl.tab.cpp"
    break;

  case 359: /* $@91: %empty  */
#line 4046 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_MapSeen);

          /*
           * Push a map marker on scopes stack.
           */
          idl_global->scopes ().push (0);
        }
#line 7000 "fe/idl.tab.cpp"
    break;

  case 360: /* $@92: %empty  */
#line 4056 "fe/idl.ypp"
        {
          idl_global->set_parse_state(IDL_GlobalData::PS_MapKeyTypeSeen);
        }
#line 7008 "fe/idl.tab.cpp"
    break;

  case 361: /* map_head: IDL_MAP $@91 '<' annotations_maybe simple_type_spec $@92 ',' annotations_maybe simple_type_spec  */
#line 4061 "fe/idl.ypp"
        {
          idl_global->set_parse_state(IDL_GlobalData::PS_MapValueTypeSeen);
          Decl_Annotations_Pair *key = new Decl_Annotations_Pair;
          key->decl = (yyvsp[-4].dcval);
          key->annotations = (yyvsp[-5].annotations_val);

          Decl_Annotations_Pair *value = new Decl_Annotations_Pair;
          value->decl = (yyvsp[0].dcval);
          value->annotations = (yyvsp[-1].annotations_val);

          Decl_Annotations_Pair_Pair* pairs = new Decl_Annotations_Pair_Pair;
          pairs->first = key;
          pairs->second = value;
          (yyval.decl_annotations_pair_val_pair) = pairs;
        }
#line 7028 "fe/idl.tab.cpp"
    break;

  case 362: /* $@93: %empty  */
#line 4081 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceCommaSeen);
        }
#line 7036 "fe/idl.tab.cpp"
    break;

  case 363: /* $@94: %empty  */
#line 4085 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceExprSeen);
        }
#line 7044 "fe/idl.tab.cpp"
    break;

  case 364: /* sequence_type_spec: seq_head ',' $@93 positive_int_expr $@94 '>'  */
#line 4089 "fe/idl.ypp"
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
              AST_Type *tp = dynamic_cast<AST_Type*> (type);

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

                  idl_global->err ()->anonymous_type_diagnostic ();
                }
            }

          delete type_annotations;
          delete ev;
          ev = 0;
          (yyval.dcval) = seq;
        }
#line 7129 "fe/idl.tab.cpp"
    break;

  case 365: /* sequence_type_spec: seq_head '>'  */
#line 4171 "fe/idl.ypp"
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
              AST_Type *tp = dynamic_cast<AST_Type*> (type);

              if (tp == 0)
                {
                  ; // Error will be caught in FE_Declarator.
                }
              else
                {
                  Identifier id ("sequence");
                  UTL_ScopedName sn (&id);
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

                  idl_global->err ()->anonymous_type_diagnostic ();
                }
            }

          delete type_annotations;
          (yyval.dcval) = seq;
        }
#line 7195 "fe/idl.tab.cpp"
    break;

  case 366: /* $@95: %empty  */
#line 4236 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSeen);

          /*
           * Push a sequence marker on scopes stack.
           */
          idl_global->scopes ().push (0);
        }
#line 7208 "fe/idl.tab.cpp"
    break;

  case 367: /* $@96: %empty  */
#line 4245 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSqSeen);
        }
#line 7216 "fe/idl.tab.cpp"
    break;

  case 368: /* seq_head: IDL_SEQUENCE $@95 '<' $@96 annotations_maybe simple_type_spec  */
#line 4249 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceTypeSeen);
          Decl_Annotations_Pair *seq_head = new Decl_Annotations_Pair;
          seq_head->decl = (yyvsp[0].dcval);
          seq_head->annotations = (yyvsp[-1].annotations_val);
          (yyval.decl_annotations_pair_val) = seq_head;
        }
#line 7228 "fe/idl.tab.cpp"
    break;

  case 369: /* fixed_type_spec: IDL_FIXED '<' positive_int_expr ',' const_expr '>'  */
#line 4260 "fe/idl.ypp"
        {
          (yyvsp[-1].exval)->evaluate (AST_Expression::EK_positive_int);
          (yyval.dcval) = idl_global->gen ()->create_fixed ((yyvsp[-3].exval), (yyvsp[-1].exval));
        }
#line 7237 "fe/idl.tab.cpp"
    break;

  case 370: /* $@97: %empty  */
#line 4269 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 7245 "fe/idl.tab.cpp"
    break;

  case 371: /* $@98: %empty  */
#line 4273 "fe/idl.ypp"
        {
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 7253 "fe/idl.tab.cpp"
    break;

  case 372: /* string_type_spec: string_head '<' $@97 positive_int_expr $@98 '>'  */
#line 4277 "fe/idl.ypp"
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
                dynamic_cast<AST_String*> (
                  tao_string_decl));

              idl_global->err ()->anonymous_type_diagnostic ();

              (yyval.dcval) = tao_string_decl;
            }

          delete ev;
          ev = 0;
        }
#line 7296 "fe/idl.tab.cpp"
    break;

  case 373: /* string_type_spec: string_head  */
#line 4316 "fe/idl.ypp"
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
            dynamic_cast<AST_String*> (
              tao_string_decl));

          (yyval.dcval) = tao_string_decl;
        }
#line 7323 "fe/idl.tab.cpp"
    break;

  case 374: /* string_head: IDL_STRING  */
#line 4342 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 7331 "fe/idl.tab.cpp"
    break;

  case 375: /* $@99: %empty  */
#line 4350 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 7339 "fe/idl.tab.cpp"
    break;

  case 376: /* $@100: %empty  */
#line 4354 "fe/idl.ypp"
        {
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 7347 "fe/idl.tab.cpp"
    break;

  case 377: /* wstring_type_spec: wstring_head '<' $@99 positive_int_expr $@100 '>'  */
#line 4358 "fe/idl.ypp"
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
              AST_String *string = idl_global->gen ()->create_wstring ((yyvsp[-2].exval));

              /*
               * Add this AST_String to the types defined in the global scope.
               */
              idl_global->root ()->fe_add_string (
                dynamic_cast<AST_String*> (
                  string));

              idl_global->err ()->anonymous_type_diagnostic ();

              (yyval.dcval) = string;
            }

          delete ev;
          ev = 0;
        }
#line 7390 "fe/idl.tab.cpp"
    break;

  case 378: /* wstring_type_spec: wstring_head  */
#line 4397 "fe/idl.ypp"
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
            dynamic_cast<AST_String*> (
              string));

          (yyval.dcval) = string;
        }
#line 7417 "fe/idl.tab.cpp"
    break;

  case 379: /* wstring_head: IDL_WSTRING  */
#line 4423 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 7425 "fe/idl.tab.cpp"
    break;

  case 380: /* $@101: %empty  */
#line 4430 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayIDSeen);
        }
#line 7433 "fe/idl.tab.cpp"
    break;

  case 381: /* array_declarator: defining_id $@101 annotations_maybe at_least_one_array_dim  */
#line 4434 "fe/idl.ypp"
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

              idl_global->err ()->anonymous_type_diagnostic ();
            }

          array_dims->destroy ();
          delete array_dims;

          delete base_type_annotations;

          (yyval.dcval) = array;
        }
#line 7473 "fe/idl.tab.cpp"
    break;

  case 382: /* at_least_one_array_dim: array_dim array_dims  */
#line 4473 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.elval),
                          UTL_ExprList ((yyvsp[-1].exval),
                                        (yyvsp[0].elval)),
                          1);
        }
#line 7484 "fe/idl.tab.cpp"
    break;

  case 383: /* array_dims: array_dims array_dim  */
#line 4483 "fe/idl.ypp"
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
#line 7506 "fe/idl.tab.cpp"
    break;

  case 384: /* array_dims: %empty  */
#line 4501 "fe/idl.ypp"
        {
          (yyval.elval) = 0;
        }
#line 7514 "fe/idl.tab.cpp"
    break;

  case 385: /* $@102: %empty  */
#line 4508 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimSqSeen);
        }
#line 7522 "fe/idl.tab.cpp"
    break;

  case 386: /* $@103: %empty  */
#line 4512 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimExprSeen);
        }
#line 7530 "fe/idl.tab.cpp"
    break;

  case 387: /* array_dim: '[' $@102 positive_int_expr $@103 ']'  */
#line 4516 "fe/idl.ypp"
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
#line 7588 "fe/idl.tab.cpp"
    break;

  case 390: /* $@104: %empty  */
#line 4578 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrROSeen);
        }
#line 7596 "fe/idl.tab.cpp"
    break;

  case 391: /* $@105: %empty  */
#line 4582 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 7604 "fe/idl.tab.cpp"
    break;

  case 392: /* $@106: %empty  */
#line 4586 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 7612 "fe/idl.tab.cpp"
    break;

  case 393: /* $@107: %empty  */
#line 4590 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 7620 "fe/idl.tab.cpp"
    break;

  case 394: /* attribute_readonly: IDL_READONLY $@104 IDL_ATTRIBUTE $@105 param_type_spec $@106 at_least_one_simple_declarator $@107 opt_raises  */
#line 4594 "fe/idl.ypp"
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
#line 7676 "fe/idl.tab.cpp"
    break;

  case 395: /* $@108: %empty  */
#line 4649 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 7684 "fe/idl.tab.cpp"
    break;

  case 396: /* $@109: %empty  */
#line 4653 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 7692 "fe/idl.tab.cpp"
    break;

  case 397: /* $@110: %empty  */
#line 4657 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 7700 "fe/idl.tab.cpp"
    break;

  case 398: /* $@111: %empty  */
#line 4661 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseCompleted);
        }
#line 7708 "fe/idl.tab.cpp"
    break;

  case 399: /* attribute_readwrite: IDL_ATTRIBUTE $@108 param_type_spec $@109 at_least_one_simple_declarator $@110 opt_getraises $@111 opt_setraises  */
#line 4665 "fe/idl.ypp"
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
#line 7773 "fe/idl.tab.cpp"
    break;

  case 400: /* $@112: %empty  */
#line 4729 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSeen);
        }
#line 7781 "fe/idl.tab.cpp"
    break;

  case 401: /* @113: %empty  */
#line 4733 "fe/idl.ypp"
        {
          Identifier *&id = (yyvsp[0].idval);
          UTL_Scope *scope = idl_global->scopes ().top_non_null ();
          UTL_ScopedName scoped_name (id);
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
#line 7817 "fe/idl.tab.cpp"
    break;

  case 402: /* $@114: %empty  */
#line 4765 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSqSeen);
        }
#line 7825 "fe/idl.tab.cpp"
    break;

  case 403: /* $@115: %empty  */
#line 4769 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptBodySeen);
        }
#line 7833 "fe/idl.tab.cpp"
    break;

  case 404: /* exception: IDL_EXCEPTION $@112 defining_id @113 '{' $@114 members $@115 '}'  */
#line 4773 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptQsSeen);
          /*
           * Done with this exception. Pop its scope from the scope stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 7847 "fe/idl.tab.cpp"
    break;

  case 405: /* $@116: %empty  */
#line 4786 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 7855 "fe/idl.tab.cpp"
    break;

  case 406: /* $@117: %empty  */
#line 4790 "fe/idl.ypp"
        {
          AST_Operation *op = 0;
          UTL_Scope *scope = idl_global->scopes ().top_non_null ();

          AST_Operation::Flags op_flags = (yyvsp[-3].ofval);
          AST_Decl *type_node = (yyvsp[-2].dcval);
          (yyvsp[-2].dcval) = 0;
          Identifier id ((yyvsp[0].strval));
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;

          UTL_ScopedName name (&id);
          idl_global->set_parse_state (IDL_GlobalData::PS_OpIDSeen);

          /*
           * Create a node representing an operation on an interface
           * and add it to its enclosing scope.
           */
          if (scope != 0 && type_node != 0)
            {
              AST_Type *type = dynamic_cast<AST_Type*> (type_node);

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
#line 7920 "fe/idl.tab.cpp"
    break;

  case 407: /* $@118: %empty  */
#line 4851 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 7928 "fe/idl.tab.cpp"
    break;

  case 408: /* $@119: %empty  */
#line 4855 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);
        }
#line 7936 "fe/idl.tab.cpp"
    break;

  case 409: /* operation: opt_op_attribute op_type_spec $@116 IDENTIFIER $@117 parameter_list $@118 opt_raises $@119 opt_context  */
#line 4859 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Operation *o = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpCompleted);

          /*
           * Add exceptions and context to the operation.
           */
          if (s != 0 && s->scope_node_type () == AST_Decl::NT_op)
            {
              o = dynamic_cast<AST_Operation*> (s);

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
#line 7971 "fe/idl.tab.cpp"
    break;

  case 410: /* opt_op_attribute: IDL_ONEWAY  */
#line 4893 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_oneway;
        }
#line 7980 "fe/idl.tab.cpp"
    break;

  case 411: /* opt_op_attribute: IDL_IDEMPOTENT  */
#line 4898 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_idempotent;
        }
#line 7989 "fe/idl.tab.cpp"
    break;

  case 412: /* opt_op_attribute: %empty  */
#line 4903 "fe/idl.ypp"
        {
          (yyval.ofval) = AST_Operation::OP_noflags;
        }
#line 7997 "fe/idl.tab.cpp"
    break;

  case 414: /* op_type_spec: IDL_VOID  */
#line 4911 "fe/idl.ypp"
        {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                  AST_Expression::EV_void
                                                );
        }
#line 8008 "fe/idl.tab.cpp"
    break;

  case 415: /* $@120: %empty  */
#line 4921 "fe/idl.ypp"
        {
          //@@ PS_FactorySeen?
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 8017 "fe/idl.tab.cpp"
    break;

  case 416: /* @121: %empty  */
#line 4926 "fe/idl.ypp"
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
#line 8064 "fe/idl.tab.cpp"
    break;

  case 417: /* $@122: %empty  */
#line 4969 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 8072 "fe/idl.tab.cpp"
    break;

  case 418: /* init_decl: IDL_FACTORY $@120 IDENTIFIER @121 init_parameter_list $@122 opt_raises  */
#line 4973 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);

          if ((yyvsp[0].nlval))
            {
              UTL_Scope *s = idl_global->scopes ().top_non_null ();
              AST_Factory *f = dynamic_cast<AST_Factory*> (s);
              (void) f->fe_add_exceptions ((yyvsp[0].nlval));
            }

          idl_global->scopes ().pop ();

          (yyval.dcval) = (yyvsp[-3].dcval);
        }
#line 8091 "fe/idl.tab.cpp"
    break;

  case 419: /* $@123: %empty  */
#line 4991 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 8099 "fe/idl.tab.cpp"
    break;

  case 420: /* init_parameter_list: '(' $@123 ')'  */
#line 4995 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 8107 "fe/idl.tab.cpp"
    break;

  case 421: /* $@124: %empty  */
#line 4999 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 8115 "fe/idl.tab.cpp"
    break;

  case 422: /* init_parameter_list: '(' $@124 at_least_one_in_parameter ')'  */
#line 5004 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 8123 "fe/idl.tab.cpp"
    break;

  case 424: /* $@125: %empty  */
#line 5014 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 8131 "fe/idl.tab.cpp"
    break;

  case 427: /* $@126: %empty  */
#line 5023 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 8139 "fe/idl.tab.cpp"
    break;

  case 428: /* $@127: %empty  */
#line 5027 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 8147 "fe/idl.tab.cpp"
    break;

  case 429: /* in_parameter: IDL_IN $@126 param_type_spec $@127 declarator  */
#line 5031 "fe/idl.ypp"
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
#line 8183 "fe/idl.tab.cpp"
    break;

  case 430: /* $@128: %empty  */
#line 5066 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 8191 "fe/idl.tab.cpp"
    break;

  case 431: /* parameter_list: '(' $@128 ')'  */
#line 5070 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 8199 "fe/idl.tab.cpp"
    break;

  case 432: /* $@129: %empty  */
#line 5074 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 8207 "fe/idl.tab.cpp"
    break;

  case 433: /* parameter_list: '(' $@129 at_least_one_parameter ')'  */
#line 5079 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 8215 "fe/idl.tab.cpp"
    break;

  case 435: /* $@130: %empty  */
#line 5089 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 8223 "fe/idl.tab.cpp"
    break;

  case 438: /* $@131: %empty  */
#line 5098 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 8231 "fe/idl.tab.cpp"
    break;

  case 439: /* $@132: %empty  */
#line 5102 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 8239 "fe/idl.tab.cpp"
    break;

  case 440: /* parameter: direction $@131 param_type_spec $@132 declarator  */
#line 5106 "fe/idl.ypp"
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
#line 8282 "fe/idl.tab.cpp"
    break;

  case 441: /* param_type_spec: base_type_spec  */
#line 5148 "fe/idl.ypp"
        {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 8293 "fe/idl.tab.cpp"
    break;

  case 444: /* param_type_spec: scoped_name  */
#line 5157 "fe/idl.ypp"
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
              AST_Type *t = dynamic_cast<AST_Type*> (d);
              AST_Typedef *td = 0;
              bool can_be_undefined = false;

              if (nt == AST_Decl::NT_typedef)
                {
                  // This code block ensures that a sequence of
                  // as-yet-undefined struct or union isn't used
                  // as a return type or argument.
                  td = dynamic_cast<AST_Typedef*> (d);
                  AST_Type *pbt = td->primitive_base_type ();

                  if (pbt->node_type () == AST_Decl::NT_sequence)
                    {
                      t = pbt;
                      AST_Sequence *seq_type =
                        dynamic_cast<AST_Sequence*> (pbt);
                      AST_Type *elem_type =
                        seq_type->base_type ();
                      AST_Decl::NodeType elem_nt =
                        elem_type->node_type ();

                      if (elem_nt == AST_Decl::NT_typedef)
                        {
                          AST_Typedef *elem_td =
                            dynamic_cast<AST_Typedef*> (elem_type);
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

                  if (pbt->node_type() == AST_Decl::NT_map)
                    {
                      t = pbt;
                      AST_Map *map_type =
                        dynamic_cast<AST_Map*> (pbt);
                      AST_Type *key_type =
                        map_type->key_type ();
                      AST_Type *val_type =
                        map_type->value_type ();

                      AST_Decl::NodeType key_nt =
                        key_type->node_type ();
                      AST_Decl::NodeType val_nt =
                        val_type->node_type ();

                      if (key_nt == AST_Decl::NT_typedef)
                        {
                          AST_Typedef *key_td =
                            dynamic_cast<AST_Typedef*> (key_type);
                          key_type = key_td->primitive_base_type ();
                          key_nt = key_type->node_type ();
                        }

                      if (val_nt == AST_Decl::NT_typedef)
                        {
                          AST_Typedef *val_td =
                            dynamic_cast<AST_Typedef*> (val_type);
                          val_type = val_td->primitive_base_type ();
                          val_nt = val_type->node_type ();
                        }

                      if (key_nt == AST_Decl::NT_interface
                          || key_nt == AST_Decl::NT_interface_fwd
                          || key_nt == AST_Decl::NT_valuetype
                          || key_nt == AST_Decl::NT_valuetype_fwd
                          || key_nt == AST_Decl::NT_component
                          || key_nt == AST_Decl::NT_component_fwd
                          || val_nt == AST_Decl::NT_interface
                          || val_nt == AST_Decl::NT_interface_fwd
                          || val_nt == AST_Decl::NT_valuetype
                          || val_nt == AST_Decl::NT_valuetype_fwd
                          || val_nt == AST_Decl::NT_component
                          || val_nt == AST_Decl::NT_component_fwd)
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
                    dynamic_cast<AST_StructureFwd*> (d);

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
#line 8449 "fe/idl.tab.cpp"
    break;

  case 445: /* direction: IDL_IN  */
#line 5312 "fe/idl.ypp"
        {
          (yyval.dival) = AST_Argument::dir_IN;
        }
#line 8457 "fe/idl.tab.cpp"
    break;

  case 446: /* direction: IDL_OUT  */
#line 5316 "fe/idl.ypp"
        {
          (yyval.dival) = AST_Argument::dir_OUT;
        }
#line 8465 "fe/idl.tab.cpp"
    break;

  case 447: /* direction: IDL_INOUT  */
#line 5320 "fe/idl.ypp"
        {
          (yyval.dival) = AST_Argument::dir_INOUT;
        }
#line 8473 "fe/idl.tab.cpp"
    break;

  case 448: /* $@133: %empty  */
#line 5327 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSeen);
        }
#line 8481 "fe/idl.tab.cpp"
    break;

  case 449: /* $@134: %empty  */
#line 5331 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSqSeen);
        }
#line 8489 "fe/idl.tab.cpp"
    break;

  case 450: /* opt_raises: IDL_RAISES $@133 '(' $@134 at_least_one_scoped_name ')'  */
#line 5336 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 8498 "fe/idl.tab.cpp"
    break;

  case 451: /* opt_raises: %empty  */
#line 5341 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 8506 "fe/idl.tab.cpp"
    break;

  case 452: /* $@135: %empty  */
#line 5348 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSeen);
        }
#line 8514 "fe/idl.tab.cpp"
    break;

  case 453: /* $@136: %empty  */
#line 5352 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSqSeen);
        }
#line 8522 "fe/idl.tab.cpp"
    break;

  case 454: /* opt_getraises: IDL_GETRAISES $@135 '(' $@136 at_least_one_scoped_name ')'  */
#line 5357 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 8531 "fe/idl.tab.cpp"
    break;

  case 455: /* opt_getraises: %empty  */
#line 5362 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 8539 "fe/idl.tab.cpp"
    break;

  case 456: /* $@137: %empty  */
#line 5369 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSeen);
        }
#line 8547 "fe/idl.tab.cpp"
    break;

  case 457: /* $@138: %empty  */
#line 5373 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSqSeen);
        }
#line 8555 "fe/idl.tab.cpp"
    break;

  case 458: /* opt_setraises: IDL_SETRAISES $@137 '(' $@138 at_least_one_scoped_name ')'  */
#line 5378 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 8564 "fe/idl.tab.cpp"
    break;

  case 459: /* opt_setraises: %empty  */
#line 5383 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 8572 "fe/idl.tab.cpp"
    break;

  case 460: /* $@139: %empty  */
#line 5390 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSeen);
        }
#line 8580 "fe/idl.tab.cpp"
    break;

  case 461: /* $@140: %empty  */
#line 5394 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSqSeen);
        }
#line 8588 "fe/idl.tab.cpp"
    break;

  case 462: /* opt_context: IDL_CONTEXT $@139 '(' $@140 at_least_one_string_literal ')'  */
#line 5399 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextQsSeen);
          (yyval.slval) = (yyvsp[-1].slval);
        }
#line 8597 "fe/idl.tab.cpp"
    break;

  case 463: /* opt_context: %empty  */
#line 5404 "fe/idl.ypp"
        {
          (yyval.slval) = 0;
        }
#line 8605 "fe/idl.tab.cpp"
    break;

  case 464: /* at_least_one_string_literal: IDL_STRING_LITERAL string_literals  */
#line 5411 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.slval),
                          UTL_StrList ((yyvsp[-1].sval),
                                       (yyvsp[0].slval)),
                          1);
        }
#line 8616 "fe/idl.tab.cpp"
    break;

  case 465: /* $@141: %empty  */
#line 5422 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextCommaSeen);
        }
#line 8624 "fe/idl.tab.cpp"
    break;

  case 466: /* string_literals: string_literals ',' $@141 IDL_STRING_LITERAL  */
#line 5426 "fe/idl.ypp"
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
#line 8646 "fe/idl.tab.cpp"
    break;

  case 467: /* string_literals: %empty  */
#line 5444 "fe/idl.ypp"
        {
          (yyval.slval) = 0;
        }
#line 8654 "fe/idl.tab.cpp"
    break;

  case 468: /* typeid_dcl: IDL_TYPEID scoped_name IDL_STRING_LITERAL  */
#line 5451 "fe/idl.ypp"
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
#line 8685 "fe/idl.tab.cpp"
    break;

  case 469: /* typeprefix_dcl: IDL_TYPEPREFIX scoped_name IDL_STRING_LITERAL  */
#line 5481 "fe/idl.ypp"
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
#line 8725 "fe/idl.tab.cpp"
    break;

  case 472: /* component_forward_decl: IDL_COMPONENT defining_id  */
#line 5526 "fe/idl.ypp"
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
#line 8755 "fe/idl.tab.cpp"
    break;

  case 473: /* @142: %empty  */
#line 5555 "fe/idl.ypp"
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
                dynamic_cast<AST_Interface *> (component);
              AST_Interface::fwd_redefinition_helper (
                component_as_interface, scope);
              component = dynamic_cast<AST_Component *> (component_as_interface);

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
#line 8801 "fe/idl.tab.cpp"
    break;

  case 474: /* $@143: %empty  */
#line 5597 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentSqSeen);
        }
#line 8809 "fe/idl.tab.cpp"
    break;

  case 475: /* $@144: %empty  */
#line 5601 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentBodySeen);
        }
#line 8817 "fe/idl.tab.cpp"
    break;

  case 476: /* component_decl: component_header @142 '{' $@143 component_exports $@144 '}'  */
#line 5605 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentQsSeen);

          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 8832 "fe/idl.tab.cpp"
    break;

  case 477: /* $@145: %empty  */
#line 5620 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentIDSeen);
        }
#line 8840 "fe/idl.tab.cpp"
    break;

  case 478: /* $@146: %empty  */
#line 5624 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 8848 "fe/idl.tab.cpp"
    break;

  case 479: /* component_header: IDL_COMPONENT defining_id $@145 component_inheritance_spec $@146 supports_spec  */
#line 5628 "fe/idl.ypp"
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
#line 8886 "fe/idl.tab.cpp"
    break;

  case 480: /* $@147: %empty  */
#line 5665 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 8894 "fe/idl.tab.cpp"
    break;

  case 481: /* component_inheritance_spec: ':' $@147 scoped_name  */
#line 5669 "fe/idl.ypp"
        {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8902 "fe/idl.tab.cpp"
    break;

  case 482: /* component_inheritance_spec: %empty  */
#line 5673 "fe/idl.ypp"
        {
          (yyval.idlist) = 0;
        }
#line 8910 "fe/idl.tab.cpp"
    break;

  case 483: /* component_exports: component_exports at_least_one_annotation component_export  */
#line 5680 "fe/idl.ypp"
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
#line 8929 "fe/idl.tab.cpp"
    break;

  case 486: /* $@148: %empty  */
#line 5700 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 8937 "fe/idl.tab.cpp"
    break;

  case 487: /* component_export: provides_decl $@148 ';'  */
#line 5704 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8946 "fe/idl.tab.cpp"
    break;

  case 488: /* $@149: %empty  */
#line 5709 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 8954 "fe/idl.tab.cpp"
    break;

  case 489: /* component_export: uses_decl $@149 ';'  */
#line 5713 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8963 "fe/idl.tab.cpp"
    break;

  case 490: /* $@150: %empty  */
#line 5718 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EmitsDeclSeen);
        }
#line 8971 "fe/idl.tab.cpp"
    break;

  case 491: /* component_export: emits_decl $@150 ';'  */
#line 5722 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8980 "fe/idl.tab.cpp"
    break;

  case 492: /* $@151: %empty  */
#line 5727 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PublishesDeclSeen);
        }
#line 8988 "fe/idl.tab.cpp"
    break;

  case 493: /* component_export: publishes_decl $@151 ';'  */
#line 5731 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8997 "fe/idl.tab.cpp"
    break;

  case 494: /* $@152: %empty  */
#line 5736 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConsumesDeclSeen);
        }
#line 9005 "fe/idl.tab.cpp"
    break;

  case 495: /* component_export: consumes_decl $@152 ';'  */
#line 5740 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 9014 "fe/idl.tab.cpp"
    break;

  case 496: /* $@153: %empty  */
#line 5745 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 9022 "fe/idl.tab.cpp"
    break;

  case 497: /* component_export: attribute $@153 ';'  */
#line 5749 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 9031 "fe/idl.tab.cpp"
    break;

  case 498: /* $@154: %empty  */
#line 5754 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 9039 "fe/idl.tab.cpp"
    break;

  case 499: /* component_export: extended_port_decl $@154 ';'  */
#line 5758 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 9048 "fe/idl.tab.cpp"
    break;

  case 500: /* provides_decl: IDL_PROVIDES interface_type id  */
#line 5765 "fe/idl.ypp"
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
                    ph = dynamic_cast<AST_Param_Holder*> (d);
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
                dynamic_cast<AST_Type*> (d);

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
#line 9142 "fe/idl.tab.cpp"
    break;

  case 501: /* interface_type: scoped_name  */
#line 5858 "fe/idl.ypp"
        {
          // Lookups and checking are done where the 'interface_type'
          // token is used, in 'provides_decl' and 'uses_decl'.
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 9152 "fe/idl.tab.cpp"
    break;

  case 502: /* interface_type: IDL_OBJECT  */
#line 5864 "fe/idl.ypp"
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
#line 9179 "fe/idl.tab.cpp"
    break;

  case 503: /* uses_decl: uses_opt_multiple interface_type id  */
#line 5889 "fe/idl.ypp"
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
                    ph = dynamic_cast<AST_Param_Holder*> (d);
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
                dynamic_cast<AST_Type*> (d);

              // Strip off _cxx_, if any, for port name.
              FE_Utils::original_local_name ((yyvsp[0].idval));

              UTL_ScopedName sn ((yyvsp[0].idval),
                                 0);

              uses = idl_global->gen ()->create_uses (
                &sn, port_interface_type, (yyvsp[-2].bval));
              s->fe_add_uses (uses);

              AST_Component *c =
                dynamic_cast<AST_Component*> (s);

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
#line 9287 "fe/idl.tab.cpp"
    break;

  case 504: /* uses_opt_multiple: IDL_USES opt_multiple  */
#line 5996 "fe/idl.ypp"
        {
          // We use this extra rule here to use in both uses_decl and
          // extended_uses_decl, so the LALR(1) parser can avoid conflicts.
          (yyval.bval) = (yyvsp[0].bval);
        }
#line 9297 "fe/idl.tab.cpp"
    break;

  case 505: /* opt_multiple: IDL_MULTIPLE  */
#line 6005 "fe/idl.ypp"
        {
          (yyval.bval) = true;
        }
#line 9305 "fe/idl.tab.cpp"
    break;

  case 506: /* opt_multiple: %empty  */
#line 6009 "fe/idl.ypp"
        {
          (yyval.bval) = false;
        }
#line 9313 "fe/idl.tab.cpp"
    break;

  case 507: /* emits_decl: IDL_EMITS scoped_name id  */
#line 6016 "fe/idl.ypp"
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
                    ph = dynamic_cast<AST_Param_Holder*> (d);
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
                dynamic_cast<AST_Type*> (d);

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
#line 9389 "fe/idl.tab.cpp"
    break;

  case 508: /* publishes_decl: IDL_PUBLISHES scoped_name id  */
#line 6091 "fe/idl.ypp"
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
                    ph = dynamic_cast<AST_Param_Holder*> (d);
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
                dynamic_cast<AST_Type*> (d);

              // Strip off _cxx_, if any, for port name.
              FE_Utils::original_local_name ((yyvsp[0].idval));

              UTL_ScopedName sn ((yyvsp[0].idval));
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
#line 9462 "fe/idl.tab.cpp"
    break;

  case 509: /* consumes_decl: IDL_CONSUMES scoped_name id  */
#line 6163 "fe/idl.ypp"
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
                    ph = dynamic_cast<AST_Param_Holder*> (d);
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
                dynamic_cast<AST_Type*> (d);

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
#line 9538 "fe/idl.tab.cpp"
    break;

  case 510: /* $@155: %empty  */
#line 6238 "fe/idl.ypp"
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
#line 9579 "fe/idl.tab.cpp"
    break;

  case 511: /* home_decl: home_header $@155 home_body  */
#line 6275 "fe/idl.ypp"
        {
          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = 0;
        }
#line 9592 "fe/idl.tab.cpp"
    break;

  case 512: /* $@156: %empty  */
#line 6287 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSeen);
        }
#line 9600 "fe/idl.tab.cpp"
    break;

  case 513: /* $@157: %empty  */
#line 6291 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeIDSeen);
        }
#line 9608 "fe/idl.tab.cpp"
    break;

  case 514: /* $@158: %empty  */
#line 6295 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 9616 "fe/idl.tab.cpp"
    break;

  case 515: /* $@159: %empty  */
#line 6299 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);
        }
#line 9624 "fe/idl.tab.cpp"
    break;

  case 516: /* $@160: %empty  */
#line 6303 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesSeen);
        }
#line 9632 "fe/idl.tab.cpp"
    break;

  case 517: /* $@161: %empty  */
#line 6307 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesIDSeen);
        }
#line 9640 "fe/idl.tab.cpp"
    break;

  case 518: /* home_header: IDL_HOME $@156 defining_id $@157 home_inheritance_spec $@158 supports_spec $@159 IDL_MANAGES $@160 scoped_name $@161 primary_key_spec  */
#line 6311 "fe/idl.ypp"
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
#line 9690 "fe/idl.tab.cpp"
    break;

  case 519: /* $@162: %empty  */
#line 6360 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 9698 "fe/idl.tab.cpp"
    break;

  case 520: /* home_inheritance_spec: ':' $@162 scoped_name  */
#line 6364 "fe/idl.ypp"
        {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 9706 "fe/idl.tab.cpp"
    break;

  case 521: /* home_inheritance_spec: %empty  */
#line 6368 "fe/idl.ypp"
        {
          (yyval.idlist) = 0;
        }
#line 9714 "fe/idl.tab.cpp"
    break;

  case 522: /* primary_key_spec: IDL_PRIMARYKEY scoped_name  */
#line 6376 "fe/idl.ypp"
        {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 9722 "fe/idl.tab.cpp"
    break;

  case 523: /* primary_key_spec: %empty  */
#line 6380 "fe/idl.ypp"
        {
          (yyval.idlist) = 0;
        }
#line 9730 "fe/idl.tab.cpp"
    break;

  case 524: /* $@163: %empty  */
#line 6387 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSqSeen);
        }
#line 9738 "fe/idl.tab.cpp"
    break;

  case 525: /* $@164: %empty  */
#line 6391 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeBodySeen);
        }
#line 9746 "fe/idl.tab.cpp"
    break;

  case 526: /* home_body: '{' $@163 home_exports $@164 '}'  */
#line 6395 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeQsSeen);
        }
#line 9754 "fe/idl.tab.cpp"
    break;

  case 530: /* $@165: %empty  */
#line 6408 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_FactoryDeclSeen);
        }
#line 9762 "fe/idl.tab.cpp"
    break;

  case 531: /* home_export: factory_decl $@165 ';'  */
#line 6412 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 9770 "fe/idl.tab.cpp"
    break;

  case 532: /* $@166: %empty  */
#line 6416 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_FinderDeclSeen);
        }
#line 9778 "fe/idl.tab.cpp"
    break;

  case 533: /* home_export: finder_decl $@166 ';'  */
#line 6420 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 9786 "fe/idl.tab.cpp"
    break;

  case 534: /* $@167: %empty  */
#line 6428 "fe/idl.ypp"
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
#line 9813 "fe/idl.tab.cpp"
    break;

  case 535: /* $@168: %empty  */
#line 6451 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 9821 "fe/idl.tab.cpp"
    break;

  case 536: /* factory_decl: IDL_FACTORY defining_id $@167 init_parameter_list $@168 opt_raises  */
#line 6455 "fe/idl.ypp"
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
#line 9843 "fe/idl.tab.cpp"
    break;

  case 537: /* $@169: %empty  */
#line 6477 "fe/idl.ypp"
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
#line 9874 "fe/idl.tab.cpp"
    break;

  case 538: /* $@170: %empty  */
#line 6504 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 9882 "fe/idl.tab.cpp"
    break;

  case 539: /* finder_decl: IDL_FINDER defining_id $@169 init_parameter_list $@170 opt_raises  */
#line 6508 "fe/idl.ypp"
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
#line 9904 "fe/idl.tab.cpp"
    break;

  case 545: /* event_concrete_forward_decl: IDL_EVENTTYPE defining_id  */
#line 6541 "fe/idl.ypp"
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
#line 9933 "fe/idl.tab.cpp"
    break;

  case 546: /* event_abs_forward_decl: IDL_ABSTRACT IDL_EVENTTYPE defining_id  */
#line 6571 "fe/idl.ypp"
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
#line 9962 "fe/idl.tab.cpp"
    break;

  case 547: /* $@171: %empty  */
#line 6600 "fe/idl.ypp"
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
              i = dynamic_cast<AST_Interface*> (e);
              AST_Interface::fwd_redefinition_helper (i,
                                                      s);
              /*
               * Add the eventetype to its definition scope
               */
              e = dynamic_cast<AST_EventType*> (i);
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
#line 10010 "fe/idl.tab.cpp"
    break;

  case 548: /* $@172: %empty  */
#line 6644 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 10018 "fe/idl.tab.cpp"
    break;

  case 549: /* $@173: %empty  */
#line 6648 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 10026 "fe/idl.tab.cpp"
    break;

  case 550: /* event_abs_decl: event_abs_header event_rest_of_header $@171 '{' $@172 exports $@173 '}'  */
#line 6652 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = 0;
        }
#line 10041 "fe/idl.tab.cpp"
    break;

  case 551: /* event_abs_header: IDL_ABSTRACT IDL_EVENTTYPE defining_id  */
#line 6668 "fe/idl.ypp"
        {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 10049 "fe/idl.tab.cpp"
    break;

  case 552: /* event_custom_header: IDL_CUSTOM IDL_EVENTTYPE defining_id  */
#line 6677 "fe/idl.ypp"
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
#line 10066 "fe/idl.tab.cpp"
    break;

  case 553: /* event_plain_header: IDL_EVENTTYPE defining_id  */
#line 6694 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          (yyval.idval) = (yyvsp[0].idval);
        }
#line 10076 "fe/idl.tab.cpp"
    break;

  case 554: /* $@174: %empty  */
#line 6703 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 10084 "fe/idl.tab.cpp"
    break;

  case 555: /* event_rest_of_header: inheritance_spec $@174 supports_spec  */
#line 6707 "fe/idl.ypp"
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
#line 10117 "fe/idl.tab.cpp"
    break;

  case 556: /* @175: %empty  */
#line 6740 "fe/idl.ypp"
        {
          UTL_Scope *scope = idl_global->scopes ().top_non_null ();
          Identifier *&event_id = (yyvsp[-1].idval);
          FE_OBVHeader *&event_header = (yyvsp[0].vhval);
          AST_EventType *eventtype = 0;

          if (scope && event_header)
            {
              // We create the scoped name here instead of with the
              // FE_OBVHeader because there is a token for it only here.
              UTL_ScopedName sn (event_id);
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
              eventtype = dynamic_cast<AST_EventType *> (eventtype_as_interface);

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
#line 10172 "fe/idl.tab.cpp"
    break;

  case 557: /* $@176: %empty  */
#line 6791 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 10180 "fe/idl.tab.cpp"
    break;

  case 558: /* $@177: %empty  */
#line 6795 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 10188 "fe/idl.tab.cpp"
    break;

  case 559: /* event_decl: event_header event_rest_of_header @175 '{' $@176 value_elements $@177 '}'  */
#line 6799 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 10203 "fe/idl.tab.cpp"
    break;

  case 560: /* event_header: event_custom_header  */
#line 6813 "fe/idl.ypp"
        {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 10211 "fe/idl.tab.cpp"
    break;

  case 561: /* event_header: event_plain_header  */
#line 6817 "fe/idl.ypp"
        {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 10219 "fe/idl.tab.cpp"
    break;

  case 562: /* formal_parameter_type: IDL_TYPENAME  */
#line 6824 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_type;
        }
#line 10227 "fe/idl.tab.cpp"
    break;

  case 563: /* formal_parameter_type: IDL_STRUCT  */
#line 6828 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_struct;
        }
#line 10235 "fe/idl.tab.cpp"
    break;

  case 564: /* formal_parameter_type: IDL_UNION  */
#line 6832 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_union;
        }
#line 10243 "fe/idl.tab.cpp"
    break;

  case 565: /* formal_parameter_type: IDL_EVENTTYPE  */
#line 6836 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_eventtype;
        }
#line 10251 "fe/idl.tab.cpp"
    break;

  case 566: /* formal_parameter_type: IDL_SEQUENCE  */
#line 6840 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_sequence;
        }
#line 10259 "fe/idl.tab.cpp"
    break;

  case 567: /* formal_parameter_type: IDL_MAP  */
#line 6844 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_map;
        }
#line 10267 "fe/idl.tab.cpp"
    break;

  case 568: /* formal_parameter_type: IDL_INTERFACE  */
#line 6848 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_interface;
        }
#line 10275 "fe/idl.tab.cpp"
    break;

  case 569: /* formal_parameter_type: IDL_VALUETYPE  */
#line 6852 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_valuetype;
        }
#line 10283 "fe/idl.tab.cpp"
    break;

  case 570: /* formal_parameter_type: IDL_ENUM  */
#line 6856 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_enum;
        }
#line 10291 "fe/idl.tab.cpp"
    break;

  case 571: /* formal_parameter_type: IDL_EXCEPTION  */
#line 6860 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_except;
        }
#line 10299 "fe/idl.tab.cpp"
    break;

  case 572: /* formal_parameter_type: IDL_CONST const_type  */
#line 6864 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_const;
          t_param_const_type = (yyvsp[0].etval);
        }
#line 10308 "fe/idl.tab.cpp"
    break;

  case 573: /* at_least_one_formal_parameter: formal_parameter formal_parameters  */
#line 6872 "fe/idl.ypp"
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
#line 10340 "fe/idl.tab.cpp"
    break;

  case 574: /* formal_parameters: formal_parameters ',' formal_parameter  */
#line 6903 "fe/idl.ypp"
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
#line 10357 "fe/idl.tab.cpp"
    break;

  case 575: /* formal_parameters: %empty  */
#line 6916 "fe/idl.ypp"
        {
          (yyval.plval) = 0;
        }
#line 10365 "fe/idl.tab.cpp"
    break;

  case 576: /* formal_parameter: formal_parameter_type IDENTIFIER  */
#line 6923 "fe/idl.ypp"
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
#line 10394 "fe/idl.tab.cpp"
    break;

  case 577: /* formal_parameter: IDL_SEQUENCE '<' IDENTIFIER '>' IDENTIFIER  */
#line 6948 "fe/idl.ypp"
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
#line 10413 "fe/idl.tab.cpp"
    break;

  case 578: /* at_least_one_formal_parameter_name: formal_parameter_name formal_parameter_names  */
#line 6966 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.slval), UTL_StrList ((yyvsp[-1].sval), (yyvsp[0].slval)), 1);
        }
#line 10421 "fe/idl.tab.cpp"
    break;

  case 579: /* formal_parameter_names: formal_parameter_names ',' formal_parameter_name  */
#line 6973 "fe/idl.ypp"
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
#line 10440 "fe/idl.tab.cpp"
    break;

  case 580: /* formal_parameter_names: %empty  */
#line 6988 "fe/idl.ypp"
        {
          (yyval.slval) = 0;
        }
#line 10448 "fe/idl.tab.cpp"
    break;

  case 581: /* formal_parameter_name: IDENTIFIER  */
#line 6995 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.sval),
                          UTL_String ((yyvsp[0].strval), true),
                          1);
        }
#line 10458 "fe/idl.tab.cpp"
    break;

  case 582: /* $@178: %empty  */
#line 7004 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSeen);
        }
#line 10466 "fe/idl.tab.cpp"
    break;

  case 583: /* @179: %empty  */
#line 7008 "fe/idl.ypp"
        {
          char *&id_value = (yyvsp[0].strval);
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeIDSeen);
          UTL_Scope *scope = idl_global->scopes ().top_non_null ();

          Identifier id (id_value);
          ACE::strdelete (id_value);
          id_value = 0;

          UTL_ScopedName scoped_name (&id);
          AST_PortType *porttype = idl_global->gen ()->create_porttype (
            &scoped_name);
          scope->fe_add_porttype (porttype);
          (yyval.dcval) = porttype;

          // Push it on the scopes stack.
          idl_global->scopes ().push (porttype);
        }
#line 10489 "fe/idl.tab.cpp"
    break;

  case 584: /* $@180: %empty  */
#line 7027 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSqSeen);
        }
#line 10497 "fe/idl.tab.cpp"
    break;

  case 585: /* $@181: %empty  */
#line 7035 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeBodySeen);
        }
#line 10505 "fe/idl.tab.cpp"
    break;

  case 586: /* porttype_decl: IDL_PORTTYPE $@178 IDENTIFIER @179 '{' $@180 at_least_one_port_export $@181 '}'  */
#line 7039 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeQsSeen);

          // Done with this port type - pop it off the scopes stack.
          idl_global->scopes ().pop ();

          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 10518 "fe/idl.tab.cpp"
    break;

  case 587: /* at_least_one_port_export: port_exports at_least_one_annotation port_export  */
#line 7051 "fe/idl.ypp"
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
#line 10537 "fe/idl.tab.cpp"
    break;

  case 593: /* $@182: %empty  */
#line 7077 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 10545 "fe/idl.tab.cpp"
    break;

  case 594: /* port_export: attribute $@182 ';'  */
#line 7081 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 10553 "fe/idl.tab.cpp"
    break;

  case 595: /* extended_port_decl: IDL_PORT scoped_name IDENTIFIER  */
#line 7088 "fe/idl.ypp"
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
              pt = dynamic_cast<AST_PortType*> (d);

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

              UTL_ScopedName sn (&id);
              ep = idl_global->gen ()->create_extended_port (&sn, pt);
              s->fe_add_extended_port (ep);

              // Create (in the AST) the struct(s) and sequence(s)
              // needed for multiplex uses ports, if any.
              for (UTL_ScopeActiveIterator i (pt, UTL_Scope::IK_decls);
                   !i.is_done ();
                   i.next ())
                {
                  d = i.item ();

                  AST_Uses *u = dynamic_cast<AST_Uses*> (d);

                  if (u != 0 && u->is_multiple ())
                    {
                      AST_Component *c =
                        dynamic_cast<AST_Component*> (s);

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
#line 10624 "fe/idl.tab.cpp"
    break;

  case 596: /* extended_port_decl: IDL_MIRRORPORT scoped_name IDENTIFIER  */
#line 7155 "fe/idl.ypp"
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
               pt = dynamic_cast<AST_PortType*> (d);

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

              UTL_ScopedName sn (&id);
              mp = idl_global->gen ()->create_mirror_port (&sn, pt);
              s->fe_add_mirror_port (mp);
            }

          (yyvsp[-1].idlist)->destroy ();
          delete (yyvsp[-1].idlist);
          (yyvsp[-1].idlist) = 0;

          (yyval.dcval) = mp;
        }
#line 10673 "fe/idl.tab.cpp"
    break;

  case 597: /* at_least_one_actual_parameter: annotations_maybe actual_parameter actual_parameters  */
#line 7203 "fe/idl.ypp"
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
#line 10690 "fe/idl.tab.cpp"
    break;

  case 598: /* actual_parameters: actual_parameters ',' annotations_maybe actual_parameter  */
#line 7219 "fe/idl.ypp"
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
#line 10707 "fe/idl.tab.cpp"
    break;

  case 599: /* actual_parameters: %empty  */
#line 7232 "fe/idl.ypp"
        {
          (yyval.alval) = 0;
        }
#line 10715 "fe/idl.tab.cpp"
    break;

  case 600: /* actual_parameter: expression  */
#line 7239 "fe/idl.ypp"
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
#line 10775 "fe/idl.tab.cpp"
    break;

  case 601: /* connector_decl: connector_header connector_body  */
#line 7298 "fe/idl.ypp"
        {
          (yyval.dcval) = 0;
        }
#line 10783 "fe/idl.tab.cpp"
    break;

  case 602: /* $@183: %empty  */
#line 7305 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSeen);
        }
#line 10791 "fe/idl.tab.cpp"
    break;

  case 603: /* $@184: %empty  */
#line 7309 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorIDSeen);
        }
#line 10799 "fe/idl.tab.cpp"
    break;

  case 604: /* connector_header: IDL_CONNECTOR $@183 annotations_maybe IDENTIFIER $@184 component_inheritance_spec  */
#line 7313 "fe/idl.ypp"
        {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Connector *parent = 0;
          bool so_far_so_good = true;

          Identifier id ((yyvsp[-2].strval));
          ACE::strdelete ((yyvsp[-2].strval));
          (yyvsp[-2].strval) = 0;

          UTL_ScopedName sn (&id);

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
                dynamic_cast<AST_Connector*> (d);

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
#line 10857 "fe/idl.tab.cpp"
    break;

  case 605: /* $@185: %empty  */
#line 7370 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSqSeen);
        }
#line 10865 "fe/idl.tab.cpp"
    break;

  case 606: /* $@186: %empty  */
#line 7374 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorBodySeen);
        }
#line 10873 "fe/idl.tab.cpp"
    break;

  case 607: /* connector_body: '{' $@185 connector_exports $@186 '}'  */
#line 7378 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorQsSeen);

          // Done with this connector - pop it off the scope stack.
          idl_global->scopes ().pop ();
        }
#line 10884 "fe/idl.tab.cpp"
    break;

  case 610: /* $@187: %empty  */
#line 7393 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 10892 "fe/idl.tab.cpp"
    break;

  case 611: /* connector_export: provides_decl $@187 ';'  */
#line 7397 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10900 "fe/idl.tab.cpp"
    break;

  case 612: /* $@188: %empty  */
#line 7401 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 10908 "fe/idl.tab.cpp"
    break;

  case 613: /* connector_export: uses_decl $@188 ';'  */
#line 7405 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10916 "fe/idl.tab.cpp"
    break;

  case 614: /* $@189: %empty  */
#line 7409 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 10924 "fe/idl.tab.cpp"
    break;

  case 615: /* connector_export: attribute $@189 ';'  */
#line 7413 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10932 "fe/idl.tab.cpp"
    break;

  case 616: /* $@190: %empty  */
#line 7417 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 10940 "fe/idl.tab.cpp"
    break;

  case 617: /* connector_export: extended_port_decl $@190 ';'  */
#line 7421 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10948 "fe/idl.tab.cpp"
    break;


#line 10952 "fe/idl.tab.cpp"

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
  ++yynerrs;

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
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
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

#line 7426 "fe/idl.ypp"

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

#ifdef _MSC_VER
#pragma warning(pop)
#endif
