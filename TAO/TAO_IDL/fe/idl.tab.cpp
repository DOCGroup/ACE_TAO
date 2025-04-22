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
#define YYLAST   2110

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  121
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  403
/* YYNRULES -- Number of rules.  */
#define YYNRULES  616
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  903

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
    3838,  3798,  3862,  3867,  3865,  3871,  3875,  3913,  3977,  3987,
    3976,  4012,  4016,  4010,  4100,  4167,  4176,  4166,  4190,  4200,
    4204,  4198,  4246,  4272,  4281,  4285,  4279,  4327,  4353,  4361,
    4360,  4403,  4413,  4431,  4439,  4443,  4438,  4503,  4504,  4509,
    4513,  4517,  4521,  4508,  4580,  4584,  4588,  4592,  4579,  4660,
    4664,  4696,  4700,  4659,  4717,  4721,  4782,  4786,  4716,  4823,
    4828,  4833,  4840,  4841,  4852,  4857,  4900,  4851,  4922,  4921,
    4930,  4929,  4940,  4945,  4943,  4949,  4954,  4958,  4953,  4997,
    4996,  5005,  5004,  5015,  5020,  5018,  5024,  5029,  5033,  5028,
    5078,  5085,  5086,  5087,  5242,  5246,  5250,  5258,  5262,  5257,
    5271,  5279,  5283,  5278,  5292,  5300,  5304,  5299,  5313,  5321,
    5325,  5320,  5334,  5341,  5353,  5351,  5374,  5381,  5411,  5450,
    5451,  5455,  5486,  5528,  5532,  5485,  5551,  5555,  5549,  5596,
    5595,  5603,  5610,  5625,  5626,  5631,  5630,  5640,  5639,  5649,
    5648,  5658,  5657,  5667,  5666,  5676,  5675,  5685,  5684,  5695,
    5788,  5794,  5819,  5926,  5935,  5939,  5946,  6021,  6093,  6169,
    6168,  6218,  6222,  6226,  6230,  6234,  6238,  6217,  6291,  6290,
    6298,  6305,  6310,  6318,  6322,  6317,  6332,  6333,  6337,  6339,
    6338,  6347,  6346,  6359,  6382,  6357,  6408,  6435,  6406,  6459,
    6460,  6461,  6465,  6466,  6470,  6499,  6531,  6575,  6579,  6529,
    6596,  6605,  6623,  6634,  6633,  6671,  6722,  6726,  6669,  6743,
    6747,  6754,  6758,  6762,  6766,  6770,  6774,  6778,  6782,  6786,
    6790,  6794,  6802,  6833,  6846,  6853,  6878,  6896,  6903,  6918,
    6925,  6935,  6939,  6958,  6966,  6934,  6981,  6996,  7000,  7001,
    7005,  7006,  7008,  7007,  7018,  7085,  7133,  7149,  7162,  7169,
    7228,  7236,  7240,  7235,  7301,  7305,  7300,  7318,  7319,  7324,
    7323,  7332,  7331,  7340,  7339,  7348,  7347
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

#define YYPACT_NINF (-655)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-585)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -655,    96,  1478,  -655,  -655,  -655,  -655,  -655,  -655,  -655,
    -655,  -655,  -655,  -655,    99,   113,    68,    89,  -655,    99,
      99,  -655,    47,    47,  -655,  -655,    99,  -655,  -655,    21,
    -655,  1531,    31,    50,  -655,  -655,     3,  -655,  -655,  -655,
    -655,  -655,  -655,   548,  -655,  -655,  -655,  -655,  -655,  1671,
      53,  -655,  -655,    75,  -655,   100,  -655,  -655,  -655,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,
    -655,  -655,  -655,  -655,    73,  -655,  -655,  -655,    73,  -655,
    -655,    93,   119,  2021,    47,    99,  1426,    99,    99,    99,
      99,  -655,  -655,  -655,     6,    99,    25,  -655,    27,    99,
    -655,    73,    99,   126,   129,    99,  -655,  -655,    12,  -655,
      29,   229,  -655,   137,  -655,   140,   144,  1539,  -655,  -655,
    -655,   153,   199,  -655,   156,   158,   170,   101,  -655,   147,
    -655,  -655,  -655,  -655,  -655,  -655,   169,  -655,  -655,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,
    -655,  -655,   187,  -655,  -655,  -655,  -655,  -655,  -655,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,
     100,  -655,  -655,  -655,   175,  -655,    81,  -655,  -655,   177,
    -655,   180,   184,   185,  -655,    47,   192,   194,   188,  -655,
     197,   198,   200,   201,   202,   210,   213,   211,  -655,  -655,
    -655,   218,   221,  -655,  -655,  -655,  -655,   187,  -655,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,   187,  -655,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,   222,  -655,   224,  -655,
    -655,   186,  -655,   287,  -655,  -655,  -655,  -655,    40,  -655,
    -655,  -655,  2021,  -655,  -655,  -655,  -655,   196,  -655,  -655,
    -655,  -655,  -655,   289,  -655,  -655,   138,   219,  -655,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,   294,  -655,    91,   223,
     225,   284,  -655,  -655,  -655,  -655,  -655,  -655,  -655,   187,
    -655,  -655,   230,  -655,  -655,  -655,  -655,  -655,  -655,  -655,
    -655,  -655,   284,   237,   238,  -655,  -655,  -655,    99,    99,
     248,   249,  -655,  -655,  -655,   246,  -655,   287,   251,  -655,
    -655,  -655,  -655,  -655,   350,  -655,   253,   252,  -655,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,   189,   189,
     189,    91,   187,  -655,  -655,   247,   250,   255,    71,    70,
      39,  -655,  -655,  -655,  -655,  -655,    47,  -655,  -655,  -655,
    -655,   256,  -655,  -655,    47,  -655,    91,    91,    91,   254,
     259,  -655,  -655,  -655,  -655,  -655,  -655,   277,  -655,   -14,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,    47,   284,
    -655,  -655,  -655,  -655,   186,  1293,  1570,   265,   266,  -655,
    1539,  -655,  -655,  -655,   257,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,   263,    99,  -655,   187,  1128,
    -655,   840,    91,  -655,   506,  -655,  -655,  -655,  -655,    91,
    -655,  -655,   726,  -655,  -655,  -655,  -655,   191,  1032,  -655,
    -655,  -655,  -655,    54,   314,    47,    47,  -655,  -655,  -655,
    -655,  -655,    54,  -655,   271,  -655,   267,  -655,   273,  -655,
    -655,  1163,   187,  -655,    47,   284,  -655,  -655,  -655,  -655,
     280,  -655,  -655,    99,  -655,  -655,   283,   275,   383,   286,
    -655,  -655,   250,   255,    71,    70,    70,    39,    39,  -655,
    -655,  -655,  -655,  -655,   282,  -655,  -655,  -655,   291,  -655,
    -655,  1901,  -655,  -655,  -655,  -655,  1934,  -655,  -655,  -655,
    -655,  -655,   292,  -655,  1388,  -655,  -655,  1812,  -655,   285,
     506,  -655,   297,   301,   303,   288,  -655,   298,  -655,   306,
    -655,  -655,  -655,   296,  1723,    47,    47,    47,   244,  -655,
     313,  -655,  -655,  -655,  -655,  -655,  -655,  -655,    99,    99,
    -655,   315,  -655,  -655,  -655,  1259,   936,   387,  1988,  -655,
     187,   287,  -655,  -655,    48,    56,   319,   321,   324,   287,
     325,  -655,  -655,    -9,  -655,    44,  -655,  -655,   327,   328,
     187,  -655,   329,    69,  1426,  -655,   398,  -655,  -655,  -655,
    -655,   138,  -655,   333,  -655,   334,  -655,   339,   342,   343,
     344,  -655,   187,  -655,  -655,  -655,  -655,  -655,   345,   346,
     444,  -655,  -655,  -655,   348,  -655,  -655,   351,  -655,  -655,
    -655,    91,  -655,   287,  -655,   349,    99,   352,  -655,   442,
     187,  -655,  -655,  -655,  -655,  -655,  -655,    57,    57,    57,
    -655,   356,  -655,   360,   361,   362,   365,   368,   369,  -655,
    -655,  -655,   370,   371,   374,   375,  -655,  -655,  -655,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,    91,  -655,  -655,  -655,
      99,  -655,   377,   378,   379,  -655,   386,   384,    69,  -655,
     389,   390,  -655,   391,    91,   392,  1646,  -655,    47,  -655,
    -655,  -655,  -655,  -655,  -655,   484,  -655,  -655,  -655,  -655,
    -655,  -655,   288,   306,  -655,  -655,  -655,   376,  -655,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,   380,   380,
    -655,  -655,  -655,  -655,  1988,    99,  -655,    91,   381,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,   393,  -655,  -655,  -655,
    -655,  -655,    47,  -655,  -655,  -655,  -655,   395,   187,  -655,
     380,   506,  -655,   396,  -655,   463,  -655,  -655,  -655,  -655,
    -655,  -655,  -655,  -655,    47,  -655,   187,   397,   618,  -655,
     394,  -655,  -655,  -655,   400,   388,   475,   474,   474,    99,
     458,   421,   415,  -655,   187,   436,  -655,  -655,   422,  -655,
     474,  -655,  -655,  -655,   423,  -655,  -655,  -655,  -655,  -655,
    -655,  -655,  -655,  -655,   485,   540,   427,   123,   474,  -655,
      62,  1988,  -655,   440,   430,   474,   431,   487,    99,    47,
    -655,  -655,   446,  -655,  -655,  -655,  -655,  -655,   435,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,   187,  -655,   448,
    -655,   449,  1988,   536,   476,    91,   472,   477,    55,  -655,
     172,    99,   475,    47,    47,   461,    99,   540,  -655,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,  1407,  -655,  -655,
    -655,   462,   464,  -655,  -655,  -655,   123,    99,   466,   479,
    -655,  -655,  -655,  -655,    47,  -655,  -655,  -655,  -655,    99,
     481,   467,   511,  -655,  -655,  -655,  -655,   478,   492,  -655,
    -655,   524,  -655
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       4,     0,     0,     3,     1,    38,   147,    40,    70,   225,
     295,   312,   347,   399,     0,     0,     0,     0,    94,     0,
       0,   511,     0,     0,   581,   601,     0,     6,     7,    42,
      24,    61,     0,     0,    22,    64,    77,    66,    26,    78,
      83,    79,    84,    77,    80,    81,    65,    18,    10,     0,
       0,    12,   231,   297,   227,   346,   228,   255,   256,   229,
      20,    14,    16,    28,   470,   469,   472,    30,   509,    32,
     541,   543,   542,   540,    77,   559,   560,   539,    77,    34,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   143,   267,   230,    77,     0,    77,    88,    77,     0,
      82,    77,     0,   476,   552,     0,   142,   138,     0,   137,
       0,     0,   213,     0,    46,     0,     0,     0,   213,     8,
       9,     0,    97,    72,     0,     0,     0,   271,   273,     0,
     285,   286,   289,   290,   291,   292,   288,   293,   294,   365,
     373,   378,   274,   281,   275,   282,   276,   283,   277,   284,
     358,    92,   238,   102,   234,   236,   237,   235,   239,   269,
     270,   240,   244,   241,   243,   242,   245,   246,   297,   252,
       0,   253,   254,   251,     0,   247,     0,   250,   248,   372,
     249,   377,     0,     0,     5,     0,   211,     0,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,   553,   546,
     555,     0,     0,   604,   600,    39,   288,   160,   148,   152,
     156,   157,   153,   154,   155,   158,   159,    41,    71,   226,
     232,   296,   313,   348,   400,    73,   550,    74,     0,   551,
      95,   481,   512,     0,   467,   140,   468,   582,     0,   197,
      43,    25,     0,   567,   562,   563,   569,   565,   570,   568,
     564,   561,   566,     0,    48,   574,     0,     0,    23,    96,
      75,    67,    27,    85,   272,   287,   278,   280,     0,     0,
       0,    99,   357,   364,   361,   369,   374,    19,    11,   214,
      13,   298,     0,    21,    15,    17,    29,   473,    31,   523,
     510,    33,    99,     0,     0,    35,    37,   608,     0,     0,
       0,     0,    89,   479,   477,   520,   139,     0,     0,   602,
     212,   200,     4,   571,     0,   575,     0,   572,   186,   187,
     188,   190,   193,   192,   194,   195,   191,   189,     0,     0,
       0,     0,   183,   599,   161,   162,   163,   165,   167,   169,
     172,   175,   179,   184,   598,    62,     0,   114,   105,   279,
     196,     0,   366,   213,     0,    93,     0,     0,     0,   217,
     213,   315,   484,   527,   554,   547,   556,   605,   149,   267,
     233,   260,   261,   262,   268,   349,   401,   114,     0,    99,
     518,   513,   141,   583,   481,     0,     0,     3,     0,    49,
       0,   180,   181,   182,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   596,     0,    76,   136,     0,
     113,     0,     0,   213,     0,    98,   362,   370,   375,   220,
     215,   300,     0,   299,   301,   305,   213,   213,     0,   114,
     105,   389,   394,     0,   505,     0,     0,   613,   387,   388,
     609,   611,     0,   615,     0,   607,     0,   213,   257,   213,
     305,     0,   480,   478,     0,    99,   589,   603,   204,   198,
       0,   206,   199,     0,   201,   207,     0,     0,     0,     0,
     573,   185,   164,   166,   168,   170,   171,   173,   174,   176,
     177,   178,   213,    63,   133,   131,   409,   410,     0,   116,
     123,     0,   117,   127,   125,   129,     0,   119,   121,   414,
     111,   110,     0,   104,     0,   106,   107,     0,   108,     0,
       0,   359,     0,     0,     0,   137,   218,     0,   219,   223,
     310,   307,   306,   213,     0,     0,     0,     0,     0,   495,
       0,   483,   485,   487,   489,   491,   493,   497,     0,     0,
     528,     0,   526,   529,   531,     0,     0,     0,     0,   501,
     500,     0,   504,   503,     0,     0,     0,     0,     0,     0,
       0,   606,   150,     0,   258,     0,   350,   355,   213,     0,
     519,   514,   588,   213,     0,   202,   210,   203,    45,   576,
      50,     0,   134,     0,    69,     0,   115,     0,     0,     0,
       0,   413,   443,   440,   441,   442,   404,   412,     0,     0,
       0,    87,   112,   103,     0,   368,   367,     0,   363,   371,
     376,     0,   216,     0,   221,     0,     0,     0,   304,   271,
     326,   321,   322,   323,   324,   316,   325,     0,     0,     0,
     482,     0,   475,     0,     0,     0,     0,     0,     0,   533,
     536,   525,     0,     0,     0,     0,   390,   395,   499,   594,
     595,   614,   610,   612,   502,   616,     0,   384,   380,   383,
       0,   356,     0,   352,     0,    91,     0,     0,     0,   592,
       0,     0,   587,     0,     0,     0,     0,   597,     0,   132,
     124,   118,   128,   126,   130,     0,   120,   122,   415,   109,
     213,   224,     0,   223,   311,   308,   303,     0,   508,   506,
     507,   496,   486,   488,   490,   492,   494,   498,     0,     0,
     530,   532,   549,   558,     0,     0,   151,     0,   381,   259,
     351,   353,   403,   515,   585,   586,     0,   590,   591,   205,
     209,   208,     0,    56,    42,    51,    55,     0,   135,   405,
       0,     0,   222,     0,   317,   418,   534,   537,   391,   396,
     266,   385,   382,   213,     0,   593,    58,     0,     0,    57,
       0,   416,   360,   309,     0,     0,     0,   450,   450,     0,
     454,   263,     0,   354,   516,     0,    52,    54,   431,   406,
     450,   318,   419,   426,     0,   425,   447,   535,   538,   392,
     451,   397,   264,   386,   522,     0,     0,     0,   450,   417,
       0,     0,   421,   422,     0,   450,     0,   458,     0,     0,
     517,   580,     0,   579,   430,   444,   445,   446,     0,   436,
     437,   407,   333,   340,   338,   319,   329,   330,   337,   427,
     423,   448,   393,   452,   455,   398,   265,   521,    59,   577,
     432,   433,     0,   462,     0,     0,     0,     0,     0,   213,
     335,     0,     0,     0,     0,     0,     0,     0,   434,   438,
     459,   408,   334,   341,   339,   320,   328,     0,   336,   428,
     424,     0,     0,   456,    60,   578,     0,     0,     0,     0,
     343,   331,   449,   453,     0,   435,   439,   460,   342,     0,
       0,     0,     0,   344,   332,   457,   466,     0,   463,   461,
     464,     0,   465
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -655,  -655,   290,   299,   551,  -618,  -655,  -655,  -655,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,
    -655,  -611,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,  -155,  -655,  -655,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,   190,  -655,
    -655,    19,  -655,  -655,  -655,   587,  -655,  -655,  -655,  -655,
    -655,  -655,  -655,   590,  -655,   193,  -655,  -655,  -251,  -655,
    -655,   178,   102,  -655,  -655,  -655,  -309,  -655,  -358,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,  -346,  -655,  -655,
     -22,  -655,  -655,  -191,   -10,  -655,    17,  -655,  -655,  -655,
    -655,  -190,   -46,  -233,  -655,   220,   217,   231,  -162,  -161,
    -206,  -105,  -655,  -320,  -655,  -655,  -655,  -655,  -655,  -655,
    -655,  -655,    24,   -83,   564,  -655,  -655,  -655,  -655,   -77,
       4,    18,  -655,    46,  -655,   -31,  -387,  -466,  -655,  -655,
    -655,     2,  -655,  -655,  -616,  -148,  -655,  -655,    -7,  -655,
     -69,  -655,  -655,   -52,   -51,   -66,   -65,   -49,   241,  -655,
     -40,  -655,   -38,  -655,  -655,  -655,  -655,  -655,   204,   295,
     136,  -655,  -655,  -655,   -37,  -655,   -32,  -655,  -655,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,  -221,  -655,  -655,  -655,
    -655,  -655,  -208,  -655,  -655,  -655,  -655,  -655,  -655,  -655,
     -41,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -110,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,
     -70,  -655,  -655,  -655,   -68,  -655,  -655,  -655,  -655,  -655,
    -655,  -655,   -67,  -655,  -655,  -339,  -655,  -655,  -655,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,    20,  -655,  -655,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,
    -655,  -655,  -633,  -655,  -655,  -655,  -655,  -655,  -203,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,  -226,  -655,  -655,
    -515,  -655,  -654,  -655,  -655,  -655,  -655,  -655,  -655,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,    22,    23,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,   268,  -655,
    -655,   134,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -328,
     215,  -327,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,
    -655,  -655,  -655,   585,  -655,  -655,  -655,  -655,  -655,  -655,
    -655,  -655,  -655,   274,  -655,  -655,  -192,  -655,  -655,  -655,
    -655,  -655,  -655,  -655,    -2,  -655,   302,  -655,  -655,    92,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,
    -655,  -655,  -655
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,     3,    27,    28,   183,   187,   191,   192,
     182,   190,   121,   116,   125,   193,   195,   197,   201,   202,
      82,    29,    84,    30,   115,   312,   467,    31,    32,   117,
     316,   469,   676,   757,   735,   758,   736,   737,   775,   856,
      33,   118,   406,    34,    35,   124,   347,   488,    36,    85,
      37,   151,   346,    38,    39,    40,   126,   348,   502,    41,
     228,   377,   569,    42,   271,    43,   102,   260,   355,    44,
      45,   411,   503,   604,   504,   505,   409,   410,   489,   587,
     598,   599,   585,   589,   588,   590,   583,   407,   484,   678,
     332,   233,   307,   109,   369,    46,   490,    83,   298,   446,
     656,   208,   333,   350,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   351,    48,   311,   385,   462,   574,   463,
     464,   675,   491,    50,   310,   359,   420,   517,   518,   614,
     519,   492,    86,   219,   299,   220,   154,   155,   156,   157,
      52,   370,   448,   660,   371,   749,   771,   808,   372,   373,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
      53,    87,    54,   188,   360,   423,   424,   617,   523,   618,
     522,   616,   743,   615,    55,    88,    56,   282,   426,   697,
     764,   800,   847,   625,   825,   848,   826,   849,   890,   844,
     827,   850,   828,   846,   845,   879,   881,   889,    57,    58,
      59,    89,   300,   449,   662,   566,   663,   753,   567,   173,
     174,   270,   607,   175,   356,   512,   176,   269,   413,   177,
     178,   357,   513,   179,   180,   358,   514,   181,   374,   447,
     658,   718,   659,   717,   772,   493,   438,   547,   714,   769,
     805,   439,   548,   715,   770,   807,   494,    90,   301,   450,
     664,   495,   685,   760,   798,   843,   496,   596,   508,   600,
     740,   780,   746,   765,   766,   784,   803,   852,   785,   801,
     851,   779,   796,   797,   818,   841,   876,   819,   842,   877,
     597,   820,   787,   804,   853,   791,   806,   854,   835,   855,
     884,   861,   878,   892,   897,   898,   901,   497,   498,    63,
      64,    65,   194,   362,   530,    66,   231,   379,   304,   378,
     427,   531,   633,   634,   635,   636,   637,   631,   638,   532,
     551,   533,   442,   553,   534,   535,   536,    67,   196,    68,
     105,   305,   455,   666,   754,   794,   381,   454,   810,   290,
     363,   541,   428,   542,   642,   643,   543,   708,   767,   544,
     709,   768,    69,    70,    71,    72,    73,   293,   429,   644,
      74,    75,    76,   199,   292,    77,   294,   430,   645,    78,
     253,   254,   317,   255,   812,   839,   813,    79,   111,   308,
     456,   667,   572,   573,   672,   726,   537,   257,   405,   344,
      80,    81,   112,   384,   204,   297,   444,   367,   445,   557,
     558,   556,   560
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     108,   110,   172,   168,    92,   169,   170,    93,   415,   103,
     104,   171,   153,   215,   209,   216,   113,   212,   213,    47,
      51,   152,    60,   334,    61,    62,    49,   511,   437,   238,
     593,   210,   211,   647,   214,   256,   416,   417,   418,   440,
     441,   364,   306,   309,   719,   172,   168,   661,   169,   170,
     106,   649,   313,   506,   171,   123,   822,   106,   733,   650,
     106,   207,   217,   822,   152,   734,    47,    51,   451,    60,
     540,    61,    62,   549,     8,   218,   747,   221,   222,   223,
     224,  -379,   593,   823,   824,   226,   185,   234,   529,   229,
     823,   824,   230,   198,   106,   232,     4,   198,   394,   431,
     432,   235,    91,  -144,   236,  -379,  -145,   761,   122,   264,
     657,   122,   265,   225,   788,   227,   382,    18,   235,     8,
     545,    95,   189,   606,   114,  -146,   799,  -100,   453,   433,
     122,   119,   122,   586,   434,   185,   107,   235,    18,   185,
     733,   106,    99,   107,   821,   235,   235,   734,   185,   506,
     120,   832,   402,   403,   404,   266,   267,  -327,   815,   816,
     817,   398,   399,   279,   318,   319,   320,   321,   322,   323,
     324,   325,   215,   209,   216,  -345,   212,   213,   122,   509,
     107,   400,   401,   326,   327,   273,   516,   274,   506,   529,
     210,   211,   106,   214,   203,   461,   479,   480,   481,   748,
     823,   824,   328,   329,   571,    94,    96,   330,   331,    98,
     101,   318,   319,   320,   321,   322,   323,   324,   325,   205,
     207,   431,   432,   391,   392,   393,  -471,   107,   515,  -544,
     326,   327,   237,   185,   669,   869,   475,   476,   239,   477,
     478,   240,   525,   526,   241,   670,   671,   259,   593,   328,
     329,   433,   527,   258,   330,   331,   434,   261,   262,   435,
     436,   886,   318,   319,   320,   321,   322,   323,   324,   325,
     414,   263,   268,   893,   431,   432,   235,   422,   107,   272,
     275,   326,   327,   276,   277,   278,   829,  -212,   368,   281,
     106,   303,   315,  -474,   280,   525,   526,   283,   284,   314,
     285,   286,   349,   287,   433,   527,   331,   431,   432,   434,
     288,   291,   435,   436,   289,   215,   209,   216,   295,   212,
     213,   296,  -545,   345,   408,   302,   352,   859,   353,   669,
     510,   354,   408,   210,   211,   593,   214,   433,   365,   366,
     670,   671,   434,   524,   466,   435,   436,   361,   334,   375,
     376,   380,   383,   388,   762,   395,   452,   389,   390,   396,
     648,   421,   412,   207,   563,   397,   565,   -44,   654,   482,
     468,   419,   552,   561,   562,   471,   593,   578,   691,   564,
     575,   172,   168,   577,   169,   170,   579,   580,   582,   605,
     171,   521,   152,   584,   601,   611,   483,   751,  -302,   581,
     152,   608,   460,    47,    51,   609,    60,   610,    61,    62,
      49,   550,   613,   554,   555,   632,   612,   641,   646,   651,
     550,   652,   692,   334,   653,   655,   594,   674,   595,  -402,
     665,  -584,   570,   679,   680,   507,   698,   699,   700,   681,
     422,   730,   682,   683,   684,   686,   687,   688,   689,   694,
     264,   528,   723,   576,   696,   621,   701,   690,   622,   623,
     702,   703,   704,   172,   168,   705,   169,   170,   706,   707,
     710,   711,   171,   521,   592,   624,   712,   713,   594,   720,
     595,   722,   152,   626,   721,   422,   724,   739,   152,   727,
     728,   729,   731,   755,   744,   759,   763,   745,  -420,   776,
     657,   781,   620,   627,   628,   629,   782,   871,   872,   106,
     783,   778,   786,   790,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   592,   792,   639,   640,
     139,   140,   141,   172,   168,   793,   169,   170,   891,   795,
    -429,   802,   171,   811,   809,   814,   830,   831,   833,   834,
     838,   106,   152,   840,   857,   858,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,    10,    11,
     507,    12,   139,   140,   141,   860,   862,   864,   873,   865,
     882,   894,   883,   887,   888,   895,   896,   142,   143,   144,
     145,   146,   147,   148,   149,   107,   899,   668,   900,   902,
     184,   185,   386,   777,   100,   150,    97,   741,   546,   603,
     716,   387,   863,   473,   186,   472,   742,   693,   695,     5,
     673,   789,     6,     7,     8,     9,   465,   866,   474,   142,
     143,   144,   145,   146,   147,   148,   149,   107,    10,    11,
     602,    12,   868,   773,   594,    13,   595,   150,  -101,   870,
     885,   752,   457,   122,   568,   425,   738,   559,    14,    15,
      16,    17,   630,   200,   470,   875,   725,    18,    19,   443,
     565,    20,     0,   677,    21,     0,     0,     0,     0,     0,
       0,    22,    23,     0,     0,     0,     0,     0,    24,    25,
     732,     0,   592,    47,    51,     0,    60,     0,    61,    62,
       0,     0,     0,     0,     0,    92,     0,     0,   750,     0,
     756,     0,    26,     0,     0,     0,     0,     0,     0,   152,
     -53,     0,     0,     0,     0,     0,     0,   520,     0,   106,
       0,   594,   774,   595,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,    10,    11,     0,    12,
     139,   140,   141,     0,     0,     0,     0,     0,     0,    92,
       0,     0,   750,     0,     0,     0,   867,     0,     0,     0,
       0,     0,   594,     0,   595,    47,    51,     0,    60,   592,
      61,    62,     0,     0,     0,     0,     0,   837,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,   836,     0,     0,     0,     0,     0,   142,   143,   144,
     145,   146,   147,   148,   149,   107,     0,     0,     0,     0,
     592,   185,     0,     0,     0,   150,   172,   168,     0,   169,
     170,   408,   408,     0,     0,   171,   880,     0,     0,     0,
       0,   485,     0,  -411,     6,   152,   874,     9,  -411,  -411,
    -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,
      10,    11,   408,    12,     0,  -411,  -411,    13,     0,     0,
     431,   432,   486,   487,  -411,     0,     0,     0,     0,     0,
      14,     0,     0,     0,   499,   500,   501,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,    23,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,
       0,     0,     0,     0,     0,  -213,     0,   485,     0,  -411,
       6,     0,   -86,     9,  -411,  -411,  -411,  -411,  -411,  -411,
    -411,  -411,  -411,  -411,  -411,  -411,    10,    11,     0,    12,
       0,  -411,  -411,    13,     0,     0,   431,   432,   486,   487,
    -411,     0,     0,     0,     0,     0,    14,     0,     0,     0,
     499,   500,   501,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
      23,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -411,  -411,  -411,
    -411,  -411,  -411,  -411,  -411,  -411,     0,     0,     0,     0,
       0,  -213,     0,   485,     0,  -411,     6,     0,  -557,     9,
    -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,
    -411,  -411,    10,    11,     0,    12,     0,  -411,  -411,    13,
       0,     0,   431,   432,   486,   487,  -411,     0,     0,     0,
       0,     0,    14,     0,     0,     0,   538,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   539,     0,     0,     0,
       0,     0,     0,     0,     0,    22,    23,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -411,  -411,  -411,  -411,  -411,  -411,  -411,
    -411,  -411,     0,     0,     0,     0,     0,     0,     0,   485,
       0,  -411,     6,     0,  -524,     9,  -411,  -411,  -411,  -411,
    -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,    10,    11,
       0,    12,     0,  -411,  -411,    13,     0,     0,   431,   432,
     486,   487,  -411,     0,   485,     0,  -411,     6,    14,     0,
       9,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,
    -411,  -411,  -411,    10,    11,     0,    12,     0,  -411,  -411,
      13,    22,    23,   431,   432,   486,   487,  -411,     0,     0,
       0,     0,     0,    14,     0,     0,     0,     0,     0,  -411,
    -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,     0,     0,
       0,     0,     0,  -213,     0,     0,    22,    23,     0,     0,
     -68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -411,  -411,  -411,  -411,  -411,  -411,
    -411,  -411,  -411,     0,     0,     0,     0,     0,  -213,     0,
     485,     0,  -411,     6,     0,   -90,     9,  -411,  -411,  -411,
    -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,    10,
      11,     0,    12,     0,  -411,  -411,    13,     0,     0,   431,
     432,   486,   487,  -411,     0,     0,   106,     6,     0,    14,
     458,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     206,   137,     0,     0,     0,     0,    12,     0,   140,   141,
       0,     0,    22,    23,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,     0,
       0,     0,     0,     0,  -213,     0,     0,     0,     0,     0,
       0,  -548,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   142,   143,   144,   145,   146,   147,
     148,   149,   107,     0,     0,     0,     0,     0,     0,   520,
       0,   106,     0,     0,     0,   459,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,    10,    11,
     106,    12,   139,   140,   141,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    10,    11,   106,
      12,   139,   140,   141,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,    10,    11,     0,    12,
     139,   140,   141,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   142,
     143,   144,   145,   146,   147,   148,   149,   107,    -2,     5,
       0,     0,     6,     7,     8,     9,     0,   150,   142,   143,
     144,   145,   146,   147,   148,   149,   107,     0,    10,    11,
       0,    12,   185,     0,     0,    13,   150,   142,   143,   144,
     145,   146,   147,   148,   149,   107,     0,     0,    14,    15,
      16,    17,     0,     0,     0,   150,     0,    18,    19,     0,
       0,    20,     0,     0,    21,   -47,     0,   -47,     0,     0,
       0,    22,    23,   242,     0,   243,     0,     0,    24,    25,
       0,   -47,   -47,     0,   -47,   -47,     0,     0,   -47,   244,
     245,     0,   246,   247,     0,     0,   248,     0,     0,     0,
       0,     5,    26,  -213,     6,     7,     8,     9,     0,     0,
     -47,     0,     0,     0,   -47,     0,     0,     0,   249,     0,
      10,    11,   250,    12,     0,     0,     0,    13,   -47,     0,
       0,     0,     0,     0,     0,     0,   251,     0,     0,     0,
      14,    15,    16,    17,     0,     0,     0,     0,     0,    18,
      19,     0,     0,    20,     0,     0,    21,     0,     0,     0,
     -47,     0,     0,    22,    23,     0,     0,     0,   252,     0,
      24,    25,     0,     0,     0,     0,     0,     5,     0,     0,
       6,     7,     8,     9,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    26,  -213,    10,    11,     0,    12,
       0,     0,     5,    13,     0,     6,     7,     8,     9,     0,
       0,     0,     0,     0,     0,     0,    14,    15,    16,    17,
       0,    10,    11,     0,    12,    18,    19,     0,    13,    20,
       0,     0,    21,     0,     0,     0,     0,     0,     0,    22,
      23,    14,    15,    16,    17,     0,    24,    25,   732,     0,
      18,    19,     0,     0,    20,     0,   106,    21,     0,     0,
       0,   619,   128,   129,    22,    23,   132,   133,   134,   135,
      26,    24,    25,     0,     0,     0,    12,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   142,   143,   144,   145,   146,   147,
     148,   149,   107,   485,     0,  -411,     6,     0,   185,     9,
    -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,
    -411,  -411,    10,    11,     0,    12,     0,  -411,  -411,    13,
       0,     0,   431,   432,   486,   487,  -411,     0,     0,     0,
       0,     0,    14,     0,     0,     0,   499,   500,   501,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,    23,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -411,  -411,  -411,  -411,  -411,  -411,  -411,
    -411,  -411,   485,     0,  -411,     6,     0,     0,     9,  -411,
    -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,
    -411,    10,    11,     0,    12,     0,  -411,  -411,    13,     0,
       0,   431,   432,   486,   487,  -411,     0,   106,     0,     0,
       0,    14,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   206,   137,   138,     0,     0,     0,     0,     0,   140,
     141,     0,     0,     0,    22,    23,     0,     0,   591,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,
    -411,   106,     0,     0,     0,     0,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   206,   137,   138,     0,     0,
       0,     0,     0,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   107,   106,     0,     0,     0,     0,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   206,     0,
       0,     0,     0,     0,     0,     0,   140,   141,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   142,
     143,   144,   145,   146,   147,   148,   149,   107,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   142,   143,   144,   145,   146,   147,   148,   149,
     107
};

static const yytype_int16 yycheck[] =
{
      22,    23,    43,    43,    14,    43,    43,    14,   354,    19,
      20,    43,    43,    83,    83,    83,    26,    83,    83,     2,
       2,    43,     2,   256,     2,     2,     2,   414,   367,   112,
     496,    83,    83,   548,    83,   118,   356,   357,   358,   367,
     367,   292,   233,     3,   660,    86,    86,     3,    86,    86,
       3,     3,   242,   411,    86,    36,     1,     3,   676,     3,
       3,    83,    84,     1,    86,   676,    49,    49,   377,    49,
     428,    49,    49,    19,     6,    85,   709,    87,    88,    89,
      90,    95,   548,    28,    29,    95,    95,    75,   427,    99,
      28,    29,   102,    74,     3,   105,     0,    78,   331,    30,
      31,    89,     3,   100,    75,   119,   100,   740,   105,     8,
     119,   105,    11,    94,   768,    96,   307,    49,    89,     6,
     429,    53,    22,   510,   103,   100,   780,   100,   379,    60,
     105,   100,   105,   491,    65,    95,    89,    89,    49,    95,
     758,     3,    53,    89,   798,    89,    89,   758,    95,   507,
     100,   805,   113,   114,   115,     8,     9,   102,    35,    36,
      37,    90,    91,   185,    73,    74,    75,    76,    77,    78,
      79,    80,   242,   242,   242,   100,   242,   242,   105,   412,
      89,   111,   112,    92,    93,   104,   419,   106,   546,   528,
     242,   242,     3,   242,   101,   385,   402,   403,   404,   714,
      28,    29,   111,   112,   455,    15,    16,   116,   117,    16,
      17,    73,    74,    75,    76,    77,    78,    79,    80,   100,
     242,    30,    31,   328,   329,   330,   100,    89,   419,   100,
      92,    93,     3,    95,   573,   851,   398,   399,   101,   400,
     401,   101,    51,    52,   100,   573,   573,    48,   714,   111,
     112,    60,    61,   100,   116,   117,    65,   101,   100,    68,
      69,   877,    73,    74,    75,    76,    77,    78,    79,    80,
     353,   101,   103,   889,    30,    31,    89,   360,    89,   104,
     103,    92,    93,   103,   100,   100,   801,    95,   298,   101,
       3,   105,     3,   102,   100,    51,    52,   100,   100,   103,
     100,   100,     8,   101,    60,    61,   117,    30,    31,    65,
     100,   100,    68,    69,   101,   385,   385,   385,   100,   385,
     385,   100,   100,   104,   346,   101,   103,   842,   103,   668,
     413,    47,   354,   385,   385,   801,   385,    60,   101,   101,
     668,   668,    65,   426,   385,    68,    69,   117,   581,   101,
     101,   105,   101,     3,   741,   108,   378,   104,   106,   109,
     551,   102,   106,   385,   447,   110,   449,   102,   559,   106,
     104,   117,    58,   102,   107,   118,   842,   102,   611,   106,
     100,   422,   422,   100,   422,   422,     3,   101,   106,   104,
     422,   422,   414,   102,   102,   107,   406,   717,   102,   482,
     422,   104,   385,   386,   386,   104,   386,   104,   386,   386,
     386,   433,   106,   435,   436,   102,   118,   102,    31,   100,
     442,   100,   613,   656,   100,   100,   496,    29,   496,   102,
     102,   102,   454,   100,   100,   411,   627,   628,   629,   100,
     523,   674,   100,   100,   100,   100,   100,     3,   100,   100,
       8,   427,    66,   463,   102,   524,   100,   106,   524,   524,
     100,   100,   100,   504,   504,   100,   504,   504,   100,   100,
     100,   100,   504,   504,   496,   524,   102,   102,   548,   102,
     548,   102,   504,   524,   106,   568,   102,     3,   510,   100,
     100,   100,   100,   100,   118,   100,   100,   117,    35,   102,
     119,   101,   524,   525,   526,   527,   118,   853,   854,     3,
      35,   117,    38,    55,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,   548,   106,   538,   539,
      24,    25,    26,   574,   574,   120,   574,   574,   884,   103,
     118,   118,   574,     3,    59,   118,   106,   117,   117,    62,
     104,     3,   574,   118,   106,   106,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
     546,    23,    24,    25,    26,    39,   100,   105,   117,   102,
     118,   100,   118,   117,   105,   118,    75,    81,    82,    83,
      84,    85,    86,    87,    88,    89,   118,   573,   106,    75,
      49,    95,   312,   758,    17,    99,    16,   690,   430,   507,
     656,   312,   845,   396,    50,   395,   693,   613,   616,     1,
     574,   769,     4,     5,     6,     7,   385,   848,   397,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    20,    21,
     504,    23,   850,   753,   714,    27,   714,    99,   100,   852,
     876,   718,   384,   105,   450,   360,   678,   442,    40,    41,
      42,    43,   528,    78,   390,   857,   668,    49,    50,   367,
     753,    53,    -1,   581,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,    71,
      72,    -1,   714,   676,   676,    -1,   676,    -1,   676,   676,
      -1,    -1,    -1,    -1,    -1,   715,    -1,    -1,   715,    -1,
     732,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,   741,
     102,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,
      -1,   801,   754,   801,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,   769,
      -1,    -1,   769,    -1,    -1,    -1,   849,    -1,    -1,    -1,
      -1,    -1,   842,    -1,   842,   758,   758,    -1,   758,   801,
     758,   758,    -1,    -1,    -1,    -1,    -1,   809,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   808,    -1,
      -1,   808,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    -1,    -1,    -1,
     842,    95,    -1,    -1,    -1,    99,   867,   867,    -1,   867,
     867,   853,   854,    -1,    -1,   867,   867,    -1,    -1,    -1,
      -1,     1,    -1,     3,     4,   867,   856,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   884,    23,    -1,    25,    26,    27,    -1,    -1,
      30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    44,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    95,    -1,     1,    -1,     3,
       4,    -1,   102,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      -1,    25,    26,    27,    -1,    -1,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      44,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    95,    -1,     1,    -1,     3,     4,    -1,   102,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    23,    -1,    25,    26,    27,
      -1,    -1,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,     3,     4,    -1,   102,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    23,    -1,    25,    26,    27,    -1,    -1,    30,    31,
      32,    33,    34,    -1,     1,    -1,     3,     4,    40,    -1,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    23,    -1,    25,    26,
      27,    63,    64,    30,    31,    32,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    -1,    63,    64,    -1,    -1,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    95,    -1,
       1,    -1,     3,     4,    -1,   102,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    -1,    25,    26,    27,    -1,    -1,    30,
      31,    32,    33,    34,    -1,    -1,     3,     4,    -1,    40,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    23,    -1,    25,    26,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,     3,    -1,    -1,    -1,   102,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
       3,    23,    24,    25,    26,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,     3,
      23,    24,    25,    26,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    89,     0,     1,
      -1,    -1,     4,     5,     6,     7,    -1,    99,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    20,    21,
      -1,    23,    95,    -1,    -1,    27,    99,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    -1,    40,    41,
      42,    43,    -1,    -1,    -1,    99,    -1,    49,    50,    -1,
      -1,    53,    -1,    -1,    56,     4,    -1,     6,    -1,    -1,
      -1,    63,    64,     4,    -1,     6,    -1,    -1,    70,    71,
      -1,    20,    21,    -1,    23,    24,    -1,    -1,    27,    20,
      21,    -1,    23,    24,    -1,    -1,    27,    -1,    -1,    -1,
      -1,     1,    94,    95,     4,     5,     6,     7,    -1,    -1,
      49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    49,    -1,
      20,    21,    53,    23,    -1,    -1,    -1,    27,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,
      40,    41,    42,    43,    -1,    -1,    -1,    -1,    -1,    49,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    -1,
      99,    -1,    -1,    63,    64,    -1,    -1,    -1,    99,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,     1,    -1,    -1,
       4,     5,     6,     7,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    20,    21,    -1,    23,
      -1,    -1,     1,    27,    -1,     4,     5,     6,     7,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    40,    41,    42,    43,
      -1,    20,    21,    -1,    23,    49,    50,    -1,    27,    53,
      -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    40,    41,    42,    43,    -1,    70,    71,    72,    -1,
      49,    50,    -1,    -1,    53,    -1,     3,    56,    -1,    -1,
      -1,     8,     9,    10,    63,    64,    13,    14,    15,    16,
      94,    70,    71,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    89,     1,    -1,     3,     4,    -1,    95,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    23,    -1,    25,    26,    27,
      -1,    -1,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    44,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    84,    85,    86,    87,
      88,    89,     1,    -1,     3,     4,    -1,    -1,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    -1,    25,    26,    27,    -1,
      -1,    30,    31,    32,    33,    34,    -1,     3,    -1,    -1,
      -1,    40,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    -1,    -1,    -1,    63,    64,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    84,    85,    86,    87,    88,
      89,     3,    -1,    -1,    -1,    -1,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    81,    82,    83,    84,    85,
      86,    87,    88,    89,     3,    -1,    -1,    -1,    -1,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    84,    85,    86,    87,    88,
      89
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
     332,   185,   104,   104,   106,   103,   103,   100,   100,   211,
     100,   101,   298,   100,   100,   100,   100,   101,   100,   101,
     460,   100,   485,   478,   487,   100,   100,   516,   219,   255,
     323,   369,   101,   105,   429,   452,   214,   213,   500,     3,
     245,   236,   146,   222,   103,     3,   151,   493,    73,    74,
      75,    76,    77,    78,    79,    80,    92,    93,   111,   112,
     116,   117,   211,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   510,   104,   173,   167,   178,     8,
     224,   234,   103,   103,    47,   189,   335,   342,   346,   246,
     285,   117,   424,   461,   189,   101,   101,   518,   215,   215,
     262,   265,   269,   270,   349,   101,   101,   182,   430,   428,
     105,   457,   214,   101,   514,   237,   123,   124,     3,   104,
     106,   232,   232,   232,   224,   108,   109,   110,    90,    91,
     111,   112,   113,   114,   115,   509,   163,   208,   211,   197,
     198,   192,   106,   339,   244,   208,   234,   234,   234,   117,
     247,   102,   244,   286,   287,   290,   299,   431,   463,   479,
     488,    30,    31,    60,    65,    68,    69,   356,   357,   362,
     440,   442,   443,   507,   517,   519,   220,   350,   263,   324,
     370,   197,   211,   189,   458,   453,   501,   429,     7,   102,
     217,   222,   238,   240,   241,   279,   321,   147,   104,   152,
     494,   118,   226,   227,   228,   229,   229,   230,   230,   231,
     231,   231,   106,   215,   209,     1,    32,    33,   168,   199,
     217,   243,   252,   356,   367,   372,   377,   418,   419,    44,
      45,    46,   179,   193,   195,   196,   199,   243,   379,   224,
     244,   257,   336,   343,   347,   214,   224,   248,   249,   251,
       1,   256,   291,   289,   244,    51,    52,    61,   243,   356,
     425,   432,   440,   442,   445,   446,   447,   507,    44,    54,
     199,   462,   464,   467,   470,   197,   192,   358,   363,    19,
     211,   441,    58,   444,   211,   211,   522,   520,   521,   441,
     523,   102,   107,   244,   106,   244,   326,   329,   289,   183,
     211,   189,   503,   504,   239,   100,   215,   100,   102,     3,
     101,   244,   106,   207,   102,   203,   199,   200,   205,   204,
     206,    34,   211,   258,   341,   345,   378,   401,   201,   202,
     380,   102,   291,   193,   194,   104,   257,   333,   104,   104,
     104,   107,   118,   106,   250,   294,   292,   288,   290,     8,
     211,   271,   276,   277,   278,   304,   321,   211,   211,   211,
     432,   438,   102,   433,   434,   435,   436,   437,   439,   215,
     215,   102,   465,   466,   480,   489,    31,   401,   214,     3,
       3,   100,   100,   100,   214,   100,   221,   119,   351,   353,
     264,     3,   325,   327,   371,   102,   454,   502,   243,   356,
     440,   442,   505,   254,    29,   242,   153,   510,   210,   100,
     100,   100,   100,   100,   100,   373,   100,   100,     3,   100,
     106,   224,   214,   251,   100,   262,   102,   300,   214,   214,
     214,   100,   100,   100,   100,   100,   100,   100,   468,   471,
     100,   100,   102,   102,   359,   364,   223,   354,   352,   265,
     102,   106,   102,    66,   102,   505,   506,   100,   100,   100,
     224,   100,    72,   126,   142,   155,   157,   158,   211,     3,
     381,   244,   250,   293,   118,   117,   383,   383,   401,   266,
     269,   234,   353,   328,   455,   100,   211,   154,   156,   100,
     374,   383,   257,   100,   301,   384,   385,   469,   472,   360,
     365,   267,   355,   329,   211,   159,   102,   157,   117,   392,
     382,   101,   118,    35,   386,   389,    38,   403,   403,   266,
      55,   406,   106,   120,   456,   103,   393,   394,   375,   403,
     302,   390,   118,   387,   404,   361,   407,   366,   268,    59,
     459,     3,   495,   497,   118,    35,    36,    37,   395,   398,
     402,   403,     1,    28,    29,   305,   307,   311,   313,   401,
     106,   117,   403,   117,    62,   409,   269,   211,   104,   496,
     118,   396,   399,   376,   310,   315,   314,   303,   306,   308,
     312,   391,   388,   405,   408,   410,   160,   106,   106,   401,
      39,   412,   100,   224,   105,   102,   307,   244,   313,   265,
     389,   208,   208,   117,   215,   497,   397,   400,   413,   316,
     256,   317,   118,   118,   411,   398,   265,   117,   105,   318,
     309,   208,   414,   265,   100,   118,    75,   415,   416,   118,
     106,   417,    75
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
     325,   321,   326,   328,   327,   327,   329,   330,   332,   333,
     331,   335,   336,   334,   334,   338,   339,   337,   340,   342,
     343,   341,   341,   344,   346,   347,   345,   345,   348,   350,
     349,   351,   352,   352,   354,   355,   353,   356,   356,   358,
     359,   360,   361,   357,   363,   364,   365,   366,   362,   368,
     369,   370,   371,   367,   373,   374,   375,   376,   372,   377,
     377,   377,   378,   378,   380,   381,   382,   379,   384,   383,
     385,   383,   386,   388,   387,   387,   390,   391,   389,   393,
     392,   394,   392,   395,   397,   396,   396,   399,   400,   398,
     401,   401,   401,   401,   402,   402,   402,   404,   405,   403,
     403,   407,   408,   406,   406,   410,   411,   409,   409,   413,
     414,   412,   412,   415,   417,   416,   416,   418,   419,   420,
     420,   421,   423,   424,   425,   422,   427,   428,   426,   430,
     429,   429,   431,   431,   431,   433,   432,   434,   432,   435,
     432,   436,   432,   437,   432,   438,   432,   439,   432,   440,
     441,   441,   442,   443,   444,   444,   445,   446,   447,   449,
     448,   451,   452,   453,   454,   455,   456,   450,   458,   457,
     457,   459,   459,   461,   462,   460,   463,   463,   464,   465,
     464,   466,   464,   468,   469,   467,   471,   472,   470,   473,
     473,   473,   474,   474,   475,   476,   478,   479,   480,   477,
     481,   482,   483,   485,   484,   487,   488,   489,   486,   490,
     490,   491,   491,   491,   491,   491,   491,   491,   491,   491,
     491,   491,   492,   493,   493,   494,   494,   495,   496,   496,
     497,   499,   500,   501,   502,   498,   503,   503,   504,   504,
     505,   505,   506,   505,   507,   507,   508,   509,   509,   510,
     511,   513,   514,   512,   516,   517,   515,   518,   518,   520,
     519,   521,   519,   522,   519,   523,   519
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
       0,     9,     2,     0,     4,     0,     2,     2,     0,     0,
       9,     0,     0,     6,     2,     0,     0,     6,     6,     0,
       0,     6,     1,     1,     0,     0,     6,     1,     1,     0,
       4,     2,     2,     0,     0,     0,     5,     1,     1,     0,
       0,     0,     0,     9,     0,     0,     0,     0,     9,     0,
       0,     0,     0,     9,     0,     0,     0,     0,    10,     1,
       1,     0,     1,     1,     0,     0,     0,     7,     0,     3,
       0,     4,     2,     0,     4,     0,     0,     0,     5,     0,
       3,     0,     4,     2,     0,     4,     0,     0,     0,     5,
       1,     1,     1,     1,     1,     1,     1,     0,     0,     6,
       0,     0,     0,     6,     0,     0,     0,     6,     0,     0,
       0,     6,     0,     2,     0,     4,     0,     3,     3,     1,
       1,     2,     0,     0,     0,     7,     0,     0,     6,     0,
       3,     0,     3,     2,     0,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     3,
       1,     1,     3,     2,     1,     0,     3,     3,     3,     0,
       3,     0,     0,     0,     0,     0,     0,    13,     0,     3,
       0,     2,     0,     0,     0,     5,     2,     0,     1,     0,
       3,     0,     3,     0,     0,     6,     0,     0,     6,     1,
       1,     1,     1,     1,     2,     3,     0,     0,     0,     8,
       3,     3,     2,     0,     3,     0,     0,     0,     8,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     3,     0,     2,     5,     2,     3,     0,
       1,     0,     0,     0,     0,     9,     3,     2,     1,     0,
       2,     2,     0,     3,     3,     3,     3,     4,     0,     1,
       2,     0,     0,     6,     0,     0,     5,     2,     0,     0,
       3,     0,     3,     0,     3,     0,     3
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
#line 2755 "fe/idl.tab.cpp"
    break;

  case 10: /* $@1: %empty  */
#line 468 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AnnotationDeclSeen);
        }
#line 2763 "fe/idl.tab.cpp"
    break;

  case 11: /* fixed_definition: annotation_dcl $@1 ';'  */
#line 472 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2771 "fe/idl.tab.cpp"
    break;

  case 12: /* $@2: %empty  */
#line 476 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 2779 "fe/idl.tab.cpp"
    break;

  case 13: /* fixed_definition: type_dcl $@2 ';'  */
#line 480 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2787 "fe/idl.tab.cpp"
    break;

  case 14: /* $@3: %empty  */
#line 484 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 2795 "fe/idl.tab.cpp"
    break;

  case 15: /* fixed_definition: typeid_dcl $@3 ';'  */
#line 488 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2803 "fe/idl.tab.cpp"
    break;

  case 16: /* $@4: %empty  */
#line 492 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 2811 "fe/idl.tab.cpp"
    break;

  case 17: /* fixed_definition: typeprefix_dcl $@4 ';'  */
#line 496 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2819 "fe/idl.tab.cpp"
    break;

  case 18: /* $@5: %empty  */
#line 500 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 2827 "fe/idl.tab.cpp"
    break;

  case 19: /* fixed_definition: const_dcl $@5 ';'  */
#line 504 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2835 "fe/idl.tab.cpp"
    break;

  case 20: /* $@6: %empty  */
#line 508 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 2843 "fe/idl.tab.cpp"
    break;

  case 21: /* fixed_definition: exception $@6 ';'  */
#line 512 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2851 "fe/idl.tab.cpp"
    break;

  case 22: /* $@7: %empty  */
#line 516 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceDeclSeen);
        }
#line 2859 "fe/idl.tab.cpp"
    break;

  case 23: /* fixed_definition: interface_def $@7 ';'  */
#line 520 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2867 "fe/idl.tab.cpp"
    break;

  case 24: /* $@8: %empty  */
#line 524 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleDeclSeen);
        }
#line 2875 "fe/idl.tab.cpp"
    break;

  case 25: /* fixed_definition: module $@8 ';'  */
#line 528 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2883 "fe/idl.tab.cpp"
    break;

  case 26: /* $@9: %empty  */
#line 532 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeDeclSeen);
        }
#line 2891 "fe/idl.tab.cpp"
    break;

  case 27: /* fixed_definition: value_def $@9 ';'  */
#line 536 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2899 "fe/idl.tab.cpp"
    break;

  case 28: /* $@10: %empty  */
#line 540 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentDeclSeen);
        }
#line 2907 "fe/idl.tab.cpp"
    break;

  case 29: /* fixed_definition: component $@10 ';'  */
#line 544 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2915 "fe/idl.tab.cpp"
    break;

  case 30: /* $@11: %empty  */
#line 548 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeDeclSeen);
        }
#line 2923 "fe/idl.tab.cpp"
    break;

  case 31: /* fixed_definition: home_decl $@11 ';'  */
#line 552 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2931 "fe/idl.tab.cpp"
    break;

  case 32: /* $@12: %empty  */
#line 556 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventDeclSeen);
        }
#line 2939 "fe/idl.tab.cpp"
    break;

  case 33: /* fixed_definition: event $@12 ';'  */
#line 560 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2947 "fe/idl.tab.cpp"
    break;

  case 34: /* $@13: %empty  */
#line 564 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeDeclSeen);
        }
#line 2955 "fe/idl.tab.cpp"
    break;

  case 35: /* fixed_definition: porttype_decl $@13 ';'  */
#line 568 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2963 "fe/idl.tab.cpp"
    break;

  case 36: /* $@14: %empty  */
#line 572 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorDeclSeen);
        }
#line 2971 "fe/idl.tab.cpp"
    break;

  case 37: /* fixed_definition: connector_decl $@14 ';'  */
#line 576 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2979 "fe/idl.tab.cpp"
    break;

  case 38: /* $@15: %empty  */
#line 580 "fe/idl.ypp"
        {
          idl_global->err ()->syntax_error (idl_global->parse_state ());
        }
#line 2987 "fe/idl.tab.cpp"
    break;

  case 39: /* fixed_definition: error $@15 ';'  */
#line 584 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
          (yyval.dcval) = 0;
        }
#line 2997 "fe/idl.tab.cpp"
    break;

  case 40: /* $@16: %empty  */
#line 593 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSeen);
        }
#line 3005 "fe/idl.tab.cpp"
    break;

  case 41: /* module_header: IDL_MODULE $@16 scoped_name  */
#line 597 "fe/idl.ypp"
        {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 3013 "fe/idl.tab.cpp"
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
#line 3056 "fe/idl.tab.cpp"
    break;

  case 43: /* $@18: %empty  */
#line 643 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSqSeen);
        }
#line 3064 "fe/idl.tab.cpp"
    break;

  case 44: /* $@19: %empty  */
#line 647 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleBodySeen);
        }
#line 3072 "fe/idl.tab.cpp"
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
#line 3086 "fe/idl.tab.cpp"
    break;

  case 46: /* template_module_header: module_header '<'  */
#line 664 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleIDSeen);
        }
#line 3094 "fe/idl.tab.cpp"
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
#line 3112 "fe/idl.tab.cpp"
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
#line 3126 "fe/idl.tab.cpp"
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
#line 3160 "fe/idl.tab.cpp"
    break;

  case 50: /* $@23: %empty  */
#line 725 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleSqSeen);
        }
#line 3168 "fe/idl.tab.cpp"
    break;

  case 51: /* $@24: %empty  */
#line 729 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleBodySeen);
        }
#line 3176 "fe/idl.tab.cpp"
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
#line 3199 "fe/idl.tab.cpp"
    break;

  case 58: /* $@25: %empty  */
#line 769 "fe/idl.ypp"
        {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefSeen);
        }
#line 3208 "fe/idl.tab.cpp"
    break;

  case 59: /* $@26: %empty  */
#line 774 "fe/idl.ypp"
        {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefParamsSeen);
        }
#line 3217 "fe/idl.tab.cpp"
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
#line 3299 "fe/idl.tab.cpp"
    break;

  case 61: /* $@27: %empty  */
#line 860 "fe/idl.ypp"
        {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleSeen);
        }
#line 3308 "fe/idl.tab.cpp"
    break;

  case 62: /* $@28: %empty  */
#line 865 "fe/idl.ypp"
        {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleArgsSeen);
        }
#line 3317 "fe/idl.tab.cpp"
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
#line 3385 "fe/idl.tab.cpp"
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
#line 3427 "fe/idl.tab.cpp"
    break;

  case 67: /* $@30: %empty  */
#line 980 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
#line 3435 "fe/idl.tab.cpp"
    break;

  case 68: /* $@31: %empty  */
#line 984 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
#line 3443 "fe/idl.tab.cpp"
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
#line 3457 "fe/idl.tab.cpp"
    break;

  case 70: /* $@32: %empty  */
#line 1001 "fe/idl.ypp"
         {
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSeen);
         }
#line 3465 "fe/idl.tab.cpp"
    break;

  case 71: /* interface_decl: IDL_INTERFACE $@32 defining_id  */
#line 1005 "fe/idl.ypp"
         {
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceIDSeen);
           (yyval.idval) = (yyvsp[0].idval);
         }
#line 3474 "fe/idl.tab.cpp"
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
#line 3514 "fe/idl.tab.cpp"
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
#line 3547 "fe/idl.tab.cpp"
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
#line 3580 "fe/idl.tab.cpp"
    break;

  case 75: /* $@33: %empty  */
#line 1112 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 3588 "fe/idl.tab.cpp"
    break;

  case 76: /* inheritance_spec: ':' opt_truncatable $@33 at_least_one_scoped_name  */
#line 1116 "fe/idl.ypp"
        {
          (yyvsp[0].nlval)->truncatable ((yyvsp[-2].bval));
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3597 "fe/idl.tab.cpp"
    break;

  case 77: /* inheritance_spec: %empty  */
#line 1121 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 3605 "fe/idl.tab.cpp"
    break;

  case 82: /* valuetype: IDL_CUSTOM value_concrete_decl  */
#line 1135 "fe/idl.ypp"
        {
          idl_global->err ()->unsupported_error ("custom is not supported");
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 3614 "fe/idl.tab.cpp"
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
#line 3665 "fe/idl.tab.cpp"
    break;

  case 85: /* $@35: %empty  */
#line 1191 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3673 "fe/idl.tab.cpp"
    break;

  case 86: /* $@36: %empty  */
#line 1195 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3681 "fe/idl.tab.cpp"
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
#line 3706 "fe/idl.tab.cpp"
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
#line 3753 "fe/idl.tab.cpp"
    break;

  case 89: /* $@38: %empty  */
#line 1267 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3761 "fe/idl.tab.cpp"
    break;

  case 90: /* $@39: %empty  */
#line 1271 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3769 "fe/idl.tab.cpp"
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
#line 3784 "fe/idl.tab.cpp"
    break;

  case 92: /* $@40: %empty  */
#line 1290 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 3792 "fe/idl.tab.cpp"
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
#line 3827 "fe/idl.tab.cpp"
    break;

  case 94: /* $@41: %empty  */
#line 1328 "fe/idl.ypp"
        {
           idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSeen);
        }
#line 3835 "fe/idl.tab.cpp"
    break;

  case 95: /* value_decl: IDL_VALUETYPE $@41 defining_id  */
#line 1332 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 3844 "fe/idl.tab.cpp"
    break;

  case 96: /* opt_truncatable: IDL_TRUNCATABLE  */
#line 1340 "fe/idl.ypp"
        {
          (yyval.bval) = true;
        }
#line 3852 "fe/idl.tab.cpp"
    break;

  case 97: /* opt_truncatable: %empty  */
#line 1344 "fe/idl.ypp"
        {
          (yyval.bval) = false;
        }
#line 3860 "fe/idl.tab.cpp"
    break;

  case 98: /* supports_spec: IDL_SUPPORTS at_least_one_scoped_name  */
#line 1352 "fe/idl.ypp"
        {
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3868 "fe/idl.tab.cpp"
    break;

  case 99: /* supports_spec: %empty  */
#line 1356 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 3876 "fe/idl.tab.cpp"
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
#line 3903 "fe/idl.tab.cpp"
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
#line 3932 "fe/idl.tab.cpp"
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
#line 3999 "fe/idl.tab.cpp"
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
#line 4022 "fe/idl.tab.cpp"
    break;

  case 104: /* value_elements: value_elements value_element  */
#line 1501 "fe/idl.ypp"
        {
          delete (yyvsp[0].decls_val);
        }
#line 4030 "fe/idl.tab.cpp"
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
#line 4045 "fe/idl.tab.cpp"
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
#line 4060 "fe/idl.tab.cpp"
    break;

  case 109: /* value_element: init_decl @42 ';'  */
#line 1532 "fe/idl.ypp"
        {
          (yyval.decls_val) = (yyvsp[-1].decls_val);
        }
#line 4068 "fe/idl.tab.cpp"
    break;

  case 110: /* visibility: IDL_PUBLIC  */
#line 1539 "fe/idl.ypp"
        {
          (yyval.vival) = AST_Field::vis_PUBLIC;
        }
#line 4076 "fe/idl.tab.cpp"
    break;

  case 111: /* visibility: IDL_PRIVATE  */
#line 1543 "fe/idl.ypp"
        {
          (yyval.vival) = AST_Field::vis_PRIVATE;
        }
#line 4084 "fe/idl.tab.cpp"
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
#line 4106 "fe/idl.tab.cpp"
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
#line 4125 "fe/idl.tab.cpp"
    break;

  case 117: /* $@43: %empty  */
#line 1595 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 4133 "fe/idl.tab.cpp"
    break;

  case 118: /* export: type_dcl $@43 ';'  */
#line 1599 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4141 "fe/idl.tab.cpp"
    break;

  case 119: /* $@44: %empty  */
#line 1603 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 4149 "fe/idl.tab.cpp"
    break;

  case 120: /* export: typeid_dcl $@44 ';'  */
#line 1607 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4157 "fe/idl.tab.cpp"
    break;

  case 121: /* $@45: %empty  */
#line 1611 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 4165 "fe/idl.tab.cpp"
    break;

  case 122: /* export: typeprefix_dcl $@45 ';'  */
#line 1615 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4173 "fe/idl.tab.cpp"
    break;

  case 123: /* $@46: %empty  */
#line 1619 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 4181 "fe/idl.tab.cpp"
    break;

  case 124: /* export: const_dcl $@46 ';'  */
#line 1623 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4189 "fe/idl.tab.cpp"
    break;

  case 125: /* $@47: %empty  */
#line 1627 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 4197 "fe/idl.tab.cpp"
    break;

  case 126: /* export: exception $@47 ';'  */
#line 1631 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4205 "fe/idl.tab.cpp"
    break;

  case 127: /* $@48: %empty  */
#line 1635 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 4213 "fe/idl.tab.cpp"
    break;

  case 128: /* export: attribute $@48 ';'  */
#line 1639 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4221 "fe/idl.tab.cpp"
    break;

  case 129: /* $@49: %empty  */
#line 1643 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpDeclSeen);
        }
#line 4229 "fe/idl.tab.cpp"
    break;

  case 130: /* export: operation $@49 ';'  */
#line 1647 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4237 "fe/idl.tab.cpp"
    break;

  case 131: /* $@50: %empty  */
#line 1651 "fe/idl.ypp"
        {
          idl_global->err ()->syntax_error (idl_global->parse_state ());
        }
#line 4245 "fe/idl.tab.cpp"
    break;

  case 132: /* export: error $@50 ';'  */
#line 1655 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
          (yyval.dcval) = 0;
        }
#line 4255 "fe/idl.tab.cpp"
    break;

  case 133: /* at_least_one_scoped_name: scoped_name scoped_names  */
#line 1664 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.nlval),
                          UTL_NameList ((yyvsp[-1].idlist),
                                        (yyvsp[0].nlval)),
                          1);
        }
#line 4266 "fe/idl.tab.cpp"
    break;

  case 134: /* $@51: %empty  */
#line 1675 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SNListCommaSeen);
        }
#line 4274 "fe/idl.tab.cpp"
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
#line 4298 "fe/idl.tab.cpp"
    break;

  case 136: /* scoped_names: %empty  */
#line 1699 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 4306 "fe/idl.tab.cpp"
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
#line 4319 "fe/idl.tab.cpp"
    break;

  case 138: /* $@52: %empty  */
#line 1715 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);
        }
#line 4327 "fe/idl.tab.cpp"
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
#line 4351 "fe/idl.tab.cpp"
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
#line 4365 "fe/idl.tab.cpp"
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
#line 4381 "fe/idl.tab.cpp"
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
#line 4393 "fe/idl.tab.cpp"
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
#line 4405 "fe/idl.tab.cpp"
    break;

  case 144: /* interface_forward: interface_decl  */
#line 1785 "fe/idl.ypp"
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
#line 4452 "fe/idl.tab.cpp"
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
#line 4482 "fe/idl.tab.cpp"
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
#line 4514 "fe/idl.tab.cpp"
    break;

  case 147: /* $@54: %empty  */
#line 1887 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstSeen);
        }
#line 4522 "fe/idl.tab.cpp"
    break;

  case 148: /* $@55: %empty  */
#line 1891 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstTypeSeen);
        }
#line 4530 "fe/idl.tab.cpp"
    break;

  case 149: /* $@56: %empty  */
#line 1895 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstIDSeen);
        }
#line 4538 "fe/idl.tab.cpp"
    break;

  case 150: /* $@57: %empty  */
#line 1899 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstAssignSeen);
        }
#line 4546 "fe/idl.tab.cpp"
    break;

  case 151: /* const_dcl: IDL_CONST $@54 const_type $@55 defining_id $@56 '=' $@57 expression  */
#line 1903 "fe/idl.ypp"
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
#line 4606 "fe/idl.tab.cpp"
    break;

  case 158: /* const_type: string_type_spec  */
#line 1968 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_string;
        }
#line 4614 "fe/idl.tab.cpp"
    break;

  case 159: /* const_type: wstring_type_spec  */
#line 1972 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_wstring;
        }
#line 4622 "fe/idl.tab.cpp"
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
#line 4692 "fe/idl.tab.cpp"
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
#line 4705 "fe/idl.tab.cpp"
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
#line 4718 "fe/idl.tab.cpp"
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
#line 4731 "fe/idl.tab.cpp"
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
#line 4744 "fe/idl.tab.cpp"
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
#line 4757 "fe/idl.tab.cpp"
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
#line 4770 "fe/idl.tab.cpp"
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
#line 4783 "fe/idl.tab.cpp"
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
#line 4796 "fe/idl.tab.cpp"
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
#line 4809 "fe/idl.tab.cpp"
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
#line 4822 "fe/idl.tab.cpp"
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
#line 4835 "fe/idl.tab.cpp"
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
#line 4848 "fe/idl.tab.cpp"
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
#line 4861 "fe/idl.tab.cpp"
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
#line 4922 "fe/idl.tab.cpp"
    break;

  case 185: /* primary_expr: '(' const_expr ')'  */
#line 2251 "fe/idl.ypp"
        {
          (yyval.exval) = (yyvsp[-1].exval);
        }
#line 4930 "fe/idl.tab.cpp"
    break;

  case 186: /* literal: IDL_INTEGER_LITERAL  */
#line 2258 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].ival));
        }
#line 4938 "fe/idl.tab.cpp"
    break;

  case 187: /* literal: IDL_UINTEGER_LITERAL  */
#line 2262 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr ((yyvsp[0].uival));
        }
#line 4947 "fe/idl.tab.cpp"
    break;

  case 188: /* literal: IDL_STRING_LITERAL  */
#line 2267 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].sval));
          (yyvsp[0].sval)->destroy ();
          delete (yyvsp[0].sval);
          (yyvsp[0].sval) = 0;
        }
#line 4958 "fe/idl.tab.cpp"
    break;

  case 189: /* literal: IDL_WSTRING_LITERAL  */
#line 2274 "fe/idl.ypp"
        {
          char *wide_string = (yyvsp[0].wsval);
          (yyval.exval) = idl_global->gen ()->create_expr (wide_string);
          ACE_OS::free (wide_string);
          (yyvsp[0].wsval) = 0;
        }
#line 4969 "fe/idl.tab.cpp"
    break;

  case 190: /* literal: IDL_CHARACTER_LITERAL  */
#line 2281 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].cval));
        }
#line 4977 "fe/idl.tab.cpp"
    break;

  case 191: /* literal: IDL_WCHAR_LITERAL  */
#line 2285 "fe/idl.ypp"
        {
          ACE_OutputCDR::from_wchar wc ((yyvsp[0].wcval));
          (yyval.exval) = idl_global->gen ()->create_expr (wc);
        }
#line 4986 "fe/idl.tab.cpp"
    break;

  case 192: /* literal: IDL_FIXED_PT_LITERAL  */
#line 2290 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].fixval));
        }
#line 4994 "fe/idl.tab.cpp"
    break;

  case 193: /* literal: IDL_FLOATING_PT_LITERAL  */
#line 2294 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].dval));
        }
#line 5002 "fe/idl.tab.cpp"
    break;

  case 194: /* literal: IDL_TRUETOK  */
#line 2298 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr (true);
        }
#line 5010 "fe/idl.tab.cpp"
    break;

  case 195: /* literal: IDL_FALSETOK  */
#line 2302 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr (false);
        }
#line 5018 "fe/idl.tab.cpp"
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
#line 5087 "fe/idl.tab.cpp"
    break;

  case 197: /* $@58: %empty  */
#line 2377 "fe/idl.ypp"
        {
          if (idl_global->idl_version_ < IDL_VERSION_4)
            idl_global->err ()->idl_version_error (
              "Annotations are not allowed in IDL versions before 4");

          Identifier *id = (yyvsp[-1].idval);
          UTL_ScopedName name (id, 0);
          AST_Annotation_Decl::escape_name (&name);
          AST_Annotation_Decl *annotation_decl = idl_global->gen ()->
            create_annotation_decl (&name);
          idl_global->scopes ().top_non_null ()->
            fe_add_annotation_decl (annotation_decl);
          idl_global->scopes ().push (annotation_decl);
        }
#line 5106 "fe/idl.tab.cpp"
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
#line 5119 "fe/idl.tab.cpp"
    break;

  case 204: /* $@59: %empty  */
#line 2412 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
          idl_global->in_typedef (true);
        }
#line 5128 "fe/idl.tab.cpp"
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
#line 5185 "fe/idl.tab.cpp"
    break;

  case 209: /* annotation_member_default: IDL_DEFAULT const_expr  */
#line 2482 "fe/idl.ypp"
        {
          (yyval.exval) = (yyvsp[0].exval);
        }
#line 5193 "fe/idl.tab.cpp"
    break;

  case 210: /* annotation_member_default: %empty  */
#line 2486 "fe/idl.ypp"
        {
          (yyval.exval) = 0;
        }
#line 5201 "fe/idl.tab.cpp"
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
#line 5215 "fe/idl.tab.cpp"
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
#line 5229 "fe/idl.tab.cpp"
    break;

  case 213: /* annotations_maybe: %empty  */
#line 2516 "fe/idl.ypp"
        {
          (yyval.annotations_val) = new AST_Annotation_Appls ();
        }
#line 5237 "fe/idl.tab.cpp"
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
#line 5296 "fe/idl.tab.cpp"
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
#line 5328 "fe/idl.tab.cpp"
    break;

  case 216: /* annotation_appl_params_maybe: '(' annotation_appl_params ')'  */
#line 2609 "fe/idl.ypp"
        {
          (yyval.annotation_params_val) = (yyvsp[-1].annotation_params_val);
        }
#line 5336 "fe/idl.tab.cpp"
    break;

  case 217: /* annotation_appl_params_maybe: %empty  */
#line 2613 "fe/idl.ypp"
        {
          (yyval.annotation_params_val) = 0;
        }
#line 5344 "fe/idl.tab.cpp"
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
#line 5357 "fe/idl.tab.cpp"
    break;

  case 219: /* annotation_appl_params: named_annotation_appl_params  */
#line 2629 "fe/idl.ypp"
        {
          (yyval.annotation_params_val) = (yyvsp[0].annotation_params_val);
        }
#line 5365 "fe/idl.tab.cpp"
    break;

  case 220: /* annotation_appl_params: %empty  */
#line 2633 "fe/idl.ypp"
        {
          (yyval.annotation_params_val) = 0;
        }
#line 5373 "fe/idl.tab.cpp"
    break;

  case 221: /* named_annotation_appl_params: named_annotation_appl_param more_named_annotation_appl_params  */
#line 2640 "fe/idl.ypp"
        {
          AST_Annotation_Appl::Params *params = (yyvsp[0].annotation_params_val);
          params->push ((yyvsp[-1].annotation_param_val));
          (yyval.annotation_params_val) = params;
        }
#line 5383 "fe/idl.tab.cpp"
    break;

  case 222: /* more_named_annotation_appl_params: ',' named_annotation_appl_param more_named_annotation_appl_params  */
#line 2648 "fe/idl.ypp"
        {
          AST_Annotation_Appl::Params *params = (yyvsp[0].annotation_params_val);
          params->push ((yyvsp[-1].annotation_param_val));
          (yyval.annotation_params_val) = params;
        }
#line 5393 "fe/idl.tab.cpp"
    break;

  case 223: /* more_named_annotation_appl_params: %empty  */
#line 2654 "fe/idl.ypp"
        {
          (yyval.annotation_params_val) = new AST_Annotation_Appl::Params;
        }
#line 5401 "fe/idl.tab.cpp"
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
#line 5414 "fe/idl.tab.cpp"
    break;

  case 225: /* $@61: %empty  */
#line 2673 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
          idl_global->in_typedef (true);
        }
#line 5423 "fe/idl.tab.cpp"
    break;

  case 226: /* type_dcl: IDL_TYPEDEF $@61 type_declarator  */
#line 2678 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5431 "fe/idl.tab.cpp"
    break;

  case 227: /* type_dcl: struct_type  */
#line 2682 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5439 "fe/idl.tab.cpp"
    break;

  case 228: /* type_dcl: union_type  */
#line 2686 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5447 "fe/idl.tab.cpp"
    break;

  case 229: /* type_dcl: enum_type  */
#line 2690 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5455 "fe/idl.tab.cpp"
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
#line 5487 "fe/idl.tab.cpp"
    break;

  case 231: /* type_dcl: constructed_forward_type_spec  */
#line 2722 "fe/idl.ypp"
        {
          (yyval.dcval) = 0;
        }
#line 5495 "fe/idl.tab.cpp"
    break;

  case 232: /* $@62: %empty  */
#line 2729 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeSpecSeen);
        }
#line 5503 "fe/idl.tab.cpp"
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
#line 5571 "fe/idl.tab.cpp"
    break;

  case 236: /* simple_type_spec: base_type_spec  */
#line 2805 "fe/idl.ypp"
        {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5582 "fe/idl.tab.cpp"
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
#line 5609 "fe/idl.tab.cpp"
    break;

  case 257: /* at_least_one_declarator: declarator declarators  */
#line 2869 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 5620 "fe/idl.tab.cpp"
    break;

  case 258: /* $@63: %empty  */
#line 2880 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 5628 "fe/idl.tab.cpp"
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
#line 5652 "fe/idl.tab.cpp"
    break;

  case 260: /* declarators: %empty  */
#line 2904 "fe/idl.ypp"
        {
          (yyval.dlval) = 0;
        }
#line 5660 "fe/idl.tab.cpp"
    break;

  case 263: /* at_least_one_simple_declarator: simple_declarator simple_declarators  */
#line 2916 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 5671 "fe/idl.tab.cpp"
    break;

  case 264: /* $@64: %empty  */
#line 2927 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 5679 "fe/idl.tab.cpp"
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
#line 5703 "fe/idl.tab.cpp"
    break;

  case 266: /* simple_declarators: %empty  */
#line 2951 "fe/idl.ypp"
        {
          (yyval.dlval) = 0;
        }
#line 5711 "fe/idl.tab.cpp"
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
#line 5728 "fe/idl.tab.cpp"
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
#line 5747 "fe/idl.tab.cpp"
    break;

  case 271: /* signed_int: IDL_LONG  */
#line 2997 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_long;
        }
#line 5755 "fe/idl.tab.cpp"
    break;

  case 272: /* signed_int: IDL_LONG IDL_LONG  */
#line 3001 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_longlong;
        }
#line 5763 "fe/idl.tab.cpp"
    break;

  case 273: /* signed_int: IDL_SHORT  */
#line 3005 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_short;
        }
#line 5771 "fe/idl.tab.cpp"
    break;

  case 274: /* signed_int: IDL_INT8  */
#line 3009 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_int8;
        }
#line 5779 "fe/idl.tab.cpp"
    break;

  case 275: /* signed_int: IDL_INT16  */
#line 3013 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_short;
        }
#line 5787 "fe/idl.tab.cpp"
    break;

  case 276: /* signed_int: IDL_INT32  */
#line 3017 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_long;
        }
#line 5795 "fe/idl.tab.cpp"
    break;

  case 277: /* signed_int: IDL_INT64  */
#line 3021 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_longlong;
        }
#line 5803 "fe/idl.tab.cpp"
    break;

  case 278: /* unsigned_int: IDL_UNSIGNED IDL_LONG  */
#line 3028 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ulong;
        }
#line 5811 "fe/idl.tab.cpp"
    break;

  case 279: /* unsigned_int: IDL_UNSIGNED IDL_LONG IDL_LONG  */
#line 3032 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ulonglong;
        }
#line 5819 "fe/idl.tab.cpp"
    break;

  case 280: /* unsigned_int: IDL_UNSIGNED IDL_SHORT  */
#line 3036 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ushort;
        }
#line 5827 "fe/idl.tab.cpp"
    break;

  case 281: /* unsigned_int: IDL_UINT8  */
#line 3040 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_uint8;
        }
#line 5835 "fe/idl.tab.cpp"
    break;

  case 282: /* unsigned_int: IDL_UINT16  */
#line 3044 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ushort;
        }
#line 5843 "fe/idl.tab.cpp"
    break;

  case 283: /* unsigned_int: IDL_UINT32  */
#line 3048 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ulong;
        }
#line 5851 "fe/idl.tab.cpp"
    break;

  case 284: /* unsigned_int: IDL_UINT64  */
#line 3052 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ulonglong;
        }
#line 5859 "fe/idl.tab.cpp"
    break;

  case 285: /* floating_pt_type: IDL_DOUBLE  */
#line 3059 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_double;
        }
#line 5867 "fe/idl.tab.cpp"
    break;

  case 286: /* floating_pt_type: IDL_FLOAT  */
#line 3063 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_float;
        }
#line 5875 "fe/idl.tab.cpp"
    break;

  case 287: /* floating_pt_type: IDL_LONG IDL_DOUBLE  */
#line 3067 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_longdouble;
        }
#line 5883 "fe/idl.tab.cpp"
    break;

  case 288: /* fixed_type: IDL_FIXED  */
#line 3074 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_fixed;
        }
#line 5891 "fe/idl.tab.cpp"
    break;

  case 289: /* char_type: IDL_CHAR  */
#line 3081 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_char;
        }
#line 5899 "fe/idl.tab.cpp"
    break;

  case 290: /* char_type: IDL_WCHAR  */
#line 3085 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_wchar;
        }
#line 5907 "fe/idl.tab.cpp"
    break;

  case 291: /* octet_type: IDL_OCTET  */
#line 3092 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_octet;
        }
#line 5915 "fe/idl.tab.cpp"
    break;

  case 292: /* boolean_type: IDL_BOOLEAN  */
#line 3099 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_bool;
        }
#line 5923 "fe/idl.tab.cpp"
    break;

  case 293: /* any_type: IDL_ANY  */
#line 3106 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_any;
        }
#line 5931 "fe/idl.tab.cpp"
    break;

  case 294: /* object_type: IDL_OBJECT  */
#line 3113 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_object;
        }
#line 5939 "fe/idl.tab.cpp"
    break;

  case 295: /* $@65: %empty  */
#line 3120 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSeen);
        }
#line 5947 "fe/idl.tab.cpp"
    break;

  case 296: /* struct_decl: IDL_STRUCT $@65 defining_id  */
#line 3124 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 5956 "fe/idl.tab.cpp"
    break;

  case 297: /* $@66: %empty  */
#line 3133 "fe/idl.ypp"
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
#line 5991 "fe/idl.tab.cpp"
    break;

  case 298: /* $@67: %empty  */
#line 3164 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSqSeen);
        }
#line 5999 "fe/idl.tab.cpp"
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
#line 6013 "fe/idl.tab.cpp"
    break;

  case 300: /* struct_body: '}'  */
#line 3181 "fe/idl.ypp"
        {
          if (idl_global->idl_version_ < IDL_VERSION_4)
            idl_global->err ()->idl_version_error (
              "Empty structs are not allowed in IDL versions before 4");

          idl_global->set_parse_state (IDL_GlobalData::PS_StructQsSeen);
        }
#line 6025 "fe/idl.tab.cpp"
    break;

  case 302: /* $@68: %empty  */
#line 3193 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructBodySeen);
        }
#line 6033 "fe/idl.tab.cpp"
    break;

  case 303: /* struct_body_with_members: member members $@68 '}'  */
#line 3197 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructQsSeen);
        }
#line 6041 "fe/idl.tab.cpp"
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
#line 6059 "fe/idl.tab.cpp"
    break;

  case 307: /* $@69: %empty  */
#line 3226 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberTypeSeen);
        }
#line 6067 "fe/idl.tab.cpp"
    break;

  case 308: /* $@70: %empty  */
#line 3230 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsSeen);
        }
#line 6075 "fe/idl.tab.cpp"
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
#line 6133 "fe/idl.tab.cpp"
    break;

  case 310: /* $@71: %empty  */
#line 3288 "fe/idl.ypp"
        {
          idl_global->err ()->syntax_error (idl_global->parse_state ());
        }
#line 6141 "fe/idl.tab.cpp"
    break;

  case 311: /* member_i: error $@71 ';'  */
#line 3292 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 6150 "fe/idl.tab.cpp"
    break;

  case 312: /* $@72: %empty  */
#line 3300 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSeen);
        }
#line 6158 "fe/idl.tab.cpp"
    break;

  case 313: /* union_decl: IDL_UNION $@72 defining_id  */
#line 3304 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 6167 "fe/idl.tab.cpp"
    break;

  case 314: /* $@73: %empty  */
#line 3312 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchSeen);
        }
#line 6175 "fe/idl.tab.cpp"
    break;

  case 315: /* $@74: %empty  */
#line 3316 "fe/idl.ypp"
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
#line 6212 "fe/idl.tab.cpp"
    break;

  case 316: /* $@75: %empty  */
#line 3349 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchTypeSeen);
        }
#line 6220 "fe/idl.tab.cpp"
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
          UTL_ScopedName n ((yyvsp[-8].idval), 0);
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
#line 6283 "fe/idl.tab.cpp"
    break;

  case 318: /* $@77: %empty  */
#line 3412 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSqSeen);
        }
#line 6291 "fe/idl.tab.cpp"
    break;

  case 319: /* $@78: %empty  */
#line 3416 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionBodySeen);
        }
#line 6299 "fe/idl.tab.cpp"
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
#line 6319 "fe/idl.tab.cpp"
    break;

  case 321: /* switch_type_spec: integer_type  */
#line 3439 "fe/idl.ypp"
        {
          (yyval.dcval) = idl_global->scopes ().bottom ()->lookup_primitive_type ((yyvsp[0].etval));
        }
#line 6327 "fe/idl.tab.cpp"
    break;

  case 322: /* switch_type_spec: char_type  */
#line 3443 "fe/idl.ypp"
        {
          if ((yyvsp[0].etval) == AST_Expression::EV_wchar && idl_global->idl_version_ < IDL_VERSION_4)
            idl_global->err ()->idl_version_error (
              "Using wchar as a union discriminator isn't allowed in IDL versions before 4");

          (yyval.dcval) = idl_global->scopes ().bottom ()->lookup_primitive_type ((yyvsp[0].etval));
        }
#line 6339 "fe/idl.tab.cpp"
    break;

  case 323: /* switch_type_spec: octet_type  */
#line 3451 "fe/idl.ypp"
        {
          if (idl_global->idl_version_ < IDL_VERSION_4)
            idl_global->err ()->idl_version_error (
              "Using octet as a union discriminator isn't allowed in IDL versions before 4");

          (yyval.dcval) = idl_global->scopes ().bottom ()->lookup_primitive_type ((yyvsp[0].etval));
        }
#line 6351 "fe/idl.tab.cpp"
    break;

  case 324: /* switch_type_spec: boolean_type  */
#line 3459 "fe/idl.ypp"
        {
          (yyval.dcval) = idl_global->scopes ().bottom ()->lookup_primitive_type ((yyvsp[0].etval));
        }
#line 6359 "fe/idl.tab.cpp"
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
#line 6470 "fe/idl.tab.cpp"
    break;

  case 330: /* $@79: %empty  */
#line 3581 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionLabelSeen);
        }
#line 6478 "fe/idl.tab.cpp"
    break;

  case 331: /* $@80: %empty  */
#line 3585 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemSeen);
        }
#line 6486 "fe/idl.tab.cpp"
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
#line 6522 "fe/idl.tab.cpp"
    break;

  case 333: /* $@81: %empty  */
#line 3621 "fe/idl.ypp"
        {
          idl_global->err ()->syntax_error (idl_global->parse_state ());
        }
#line 6530 "fe/idl.tab.cpp"
    break;

  case 334: /* case_branch: error $@81 ';'  */
#line 3625 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 6539 "fe/idl.tab.cpp"
    break;

  case 335: /* at_least_one_case_label: case_label case_labels  */
#line 3633 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.llval),
                          UTL_LabelList ((yyvsp[-1].ulval),
                                         (yyvsp[0].llval)),
                          1);
        }
#line 6550 "fe/idl.tab.cpp"
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
#line 6572 "fe/idl.tab.cpp"
    break;

  case 337: /* case_labels: %empty  */
#line 3661 "fe/idl.ypp"
        {
          (yyval.llval) = 0;
        }
#line 6580 "fe/idl.tab.cpp"
    break;

  case 338: /* $@82: %empty  */
#line 3668 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DefaultSeen);
        }
#line 6588 "fe/idl.tab.cpp"
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
#line 6601 "fe/idl.tab.cpp"
    break;

  case 340: /* $@83: %empty  */
#line 3681 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_CaseSeen);
        }
#line 6609 "fe/idl.tab.cpp"
    break;

  case 341: /* $@84: %empty  */
#line 3685 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelExprSeen);
        }
#line 6617 "fe/idl.tab.cpp"
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
#line 6630 "fe/idl.tab.cpp"
    break;

  case 343: /* $@85: %empty  */
#line 3701 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemTypeSeen);
        }
#line 6638 "fe/idl.tab.cpp"
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
#line 6685 "fe/idl.tab.cpp"
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
#line 6711 "fe/idl.tab.cpp"
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
#line 6735 "fe/idl.tab.cpp"
    break;

  case 347: /* $@86: %empty  */
#line 3799 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSeen);
        }
#line 6743 "fe/idl.tab.cpp"
    break;

  case 348: /* $@87: %empty  */
#line 3803 "fe/idl.ypp"
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
#line 6778 "fe/idl.tab.cpp"
    break;

  case 349: /* $@88: %empty  */
#line 3834 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSqSeen);
        }
#line 6786 "fe/idl.tab.cpp"
    break;

  case 350: /* $@89: %empty  */
#line 3838 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumBodySeen);
        }
#line 6794 "fe/idl.tab.cpp"
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
#line 6817 "fe/idl.tab.cpp"
    break;

  case 353: /* $@90: %empty  */
#line 3867 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumCommaSeen);
        }
#line 6825 "fe/idl.tab.cpp"
    break;

  case 356: /* enumerator: annotations_maybe IDENTIFIER  */
#line 3876 "fe/idl.ypp"
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
#line 6864 "fe/idl.tab.cpp"
    break;

  case 357: /* map_type_spec: map_head '>'  */
#line 3915 "fe/idl.ypp"
        {
          AST_Map *map = 0;
          Decl_Annotations_Pair_Pair* type_pair = (yyvsp[-1].decl_annotations_pair_val_pair);
          Decl_Annotations_Pair *key_type = type_pair->first;
          Decl_Annotations_Pair *val_type = type_pair->second;

          /*
           * Remove map marker from scopes stack.
           */
          if (idl_global->scopes ().top () == 0)
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

              if (ktp == 0 || vtp == 0)
                {
                  ; // Error will be caught in FE_Declarator.
                }
              else
                {
                  Identifier id ("map");
                  UTL_ScopedName sn (&id, 0);
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
          delete val_type->annotations;
          delete type_pair;
          (yyval.dcval) = map;
        }
#line 6927 "fe/idl.tab.cpp"
    break;

  case 358: /* $@91: %empty  */
#line 3977 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_MapSeen);

          /*
           * Push a map marker on scopes stack.
           */
          idl_global->scopes ().push (0);
        }
#line 6940 "fe/idl.tab.cpp"
    break;

  case 359: /* $@92: %empty  */
#line 3987 "fe/idl.ypp"
        {
          idl_global->set_parse_state(IDL_GlobalData::PS_MapKeyTypeSeen);
        }
#line 6948 "fe/idl.tab.cpp"
    break;

  case 360: /* map_head: IDL_MAP $@91 '<' annotations_maybe simple_type_spec $@92 ',' annotations_maybe simple_type_spec  */
#line 3992 "fe/idl.ypp"
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
#line 6968 "fe/idl.tab.cpp"
    break;

  case 361: /* $@93: %empty  */
#line 4012 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceCommaSeen);
        }
#line 6976 "fe/idl.tab.cpp"
    break;

  case 362: /* $@94: %empty  */
#line 4016 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceExprSeen);
        }
#line 6984 "fe/idl.tab.cpp"
    break;

  case 363: /* sequence_type_spec: seq_head ',' $@93 positive_int_expr $@94 '>'  */
#line 4020 "fe/idl.ypp"
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
#line 7069 "fe/idl.tab.cpp"
    break;

  case 364: /* sequence_type_spec: seq_head '>'  */
#line 4102 "fe/idl.ypp"
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

                  idl_global->err ()->anonymous_type_diagnostic ();
                }
            }

          delete type_annotations;
          (yyval.dcval) = seq;
        }
#line 7135 "fe/idl.tab.cpp"
    break;

  case 365: /* $@95: %empty  */
#line 4167 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSeen);

          /*
           * Push a sequence marker on scopes stack.
           */
          idl_global->scopes ().push (0);
        }
#line 7148 "fe/idl.tab.cpp"
    break;

  case 366: /* $@96: %empty  */
#line 4176 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSqSeen);
        }
#line 7156 "fe/idl.tab.cpp"
    break;

  case 367: /* seq_head: IDL_SEQUENCE $@95 '<' $@96 annotations_maybe simple_type_spec  */
#line 4180 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceTypeSeen);
          Decl_Annotations_Pair *seq_head = new Decl_Annotations_Pair;
          seq_head->decl = (yyvsp[0].dcval);
          seq_head->annotations = (yyvsp[-1].annotations_val);
          (yyval.decl_annotations_pair_val) = seq_head;
        }
#line 7168 "fe/idl.tab.cpp"
    break;

  case 368: /* fixed_type_spec: IDL_FIXED '<' positive_int_expr ',' const_expr '>'  */
#line 4191 "fe/idl.ypp"
        {
          (yyvsp[-1].exval)->evaluate (AST_Expression::EK_positive_int);
          (yyval.dcval) = idl_global->gen ()->create_fixed ((yyvsp[-3].exval), (yyvsp[-1].exval));
        }
#line 7177 "fe/idl.tab.cpp"
    break;

  case 369: /* $@97: %empty  */
#line 4200 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 7185 "fe/idl.tab.cpp"
    break;

  case 370: /* $@98: %empty  */
#line 4204 "fe/idl.ypp"
        {
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 7193 "fe/idl.tab.cpp"
    break;

  case 371: /* string_type_spec: string_head '<' $@97 positive_int_expr $@98 '>'  */
#line 4208 "fe/idl.ypp"
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
#line 7236 "fe/idl.tab.cpp"
    break;

  case 372: /* string_type_spec: string_head  */
#line 4247 "fe/idl.ypp"
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
#line 7263 "fe/idl.tab.cpp"
    break;

  case 373: /* string_head: IDL_STRING  */
#line 4273 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 7271 "fe/idl.tab.cpp"
    break;

  case 374: /* $@99: %empty  */
#line 4281 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 7279 "fe/idl.tab.cpp"
    break;

  case 375: /* $@100: %empty  */
#line 4285 "fe/idl.ypp"
        {
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 7287 "fe/idl.tab.cpp"
    break;

  case 376: /* wstring_type_spec: wstring_head '<' $@99 positive_int_expr $@100 '>'  */
#line 4289 "fe/idl.ypp"
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
#line 7330 "fe/idl.tab.cpp"
    break;

  case 377: /* wstring_type_spec: wstring_head  */
#line 4328 "fe/idl.ypp"
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
#line 7357 "fe/idl.tab.cpp"
    break;

  case 378: /* wstring_head: IDL_WSTRING  */
#line 4354 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 7365 "fe/idl.tab.cpp"
    break;

  case 379: /* $@101: %empty  */
#line 4361 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayIDSeen);
        }
#line 7373 "fe/idl.tab.cpp"
    break;

  case 380: /* array_declarator: defining_id $@101 annotations_maybe at_least_one_array_dim  */
#line 4365 "fe/idl.ypp"
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
#line 7413 "fe/idl.tab.cpp"
    break;

  case 381: /* at_least_one_array_dim: array_dim array_dims  */
#line 4404 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.elval),
                          UTL_ExprList ((yyvsp[-1].exval),
                                        (yyvsp[0].elval)),
                          1);
        }
#line 7424 "fe/idl.tab.cpp"
    break;

  case 382: /* array_dims: array_dims array_dim  */
#line 4414 "fe/idl.ypp"
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
#line 7446 "fe/idl.tab.cpp"
    break;

  case 383: /* array_dims: %empty  */
#line 4432 "fe/idl.ypp"
        {
          (yyval.elval) = 0;
        }
#line 7454 "fe/idl.tab.cpp"
    break;

  case 384: /* $@102: %empty  */
#line 4439 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimSqSeen);
        }
#line 7462 "fe/idl.tab.cpp"
    break;

  case 385: /* $@103: %empty  */
#line 4443 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimExprSeen);
        }
#line 7470 "fe/idl.tab.cpp"
    break;

  case 386: /* array_dim: '[' $@102 positive_int_expr $@103 ']'  */
#line 4447 "fe/idl.ypp"
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
#line 7528 "fe/idl.tab.cpp"
    break;

  case 389: /* $@104: %empty  */
#line 4509 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrROSeen);
        }
#line 7536 "fe/idl.tab.cpp"
    break;

  case 390: /* $@105: %empty  */
#line 4513 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 7544 "fe/idl.tab.cpp"
    break;

  case 391: /* $@106: %empty  */
#line 4517 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 7552 "fe/idl.tab.cpp"
    break;

  case 392: /* $@107: %empty  */
#line 4521 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 7560 "fe/idl.tab.cpp"
    break;

  case 393: /* attribute_readonly: IDL_READONLY $@104 IDL_ATTRIBUTE $@105 param_type_spec $@106 at_least_one_simple_declarator $@107 opt_raises  */
#line 4525 "fe/idl.ypp"
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
#line 7616 "fe/idl.tab.cpp"
    break;

  case 394: /* $@108: %empty  */
#line 4580 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 7624 "fe/idl.tab.cpp"
    break;

  case 395: /* $@109: %empty  */
#line 4584 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 7632 "fe/idl.tab.cpp"
    break;

  case 396: /* $@110: %empty  */
#line 4588 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 7640 "fe/idl.tab.cpp"
    break;

  case 397: /* $@111: %empty  */
#line 4592 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseCompleted);
        }
#line 7648 "fe/idl.tab.cpp"
    break;

  case 398: /* attribute_readwrite: IDL_ATTRIBUTE $@108 param_type_spec $@109 at_least_one_simple_declarator $@110 opt_getraises $@111 opt_setraises  */
#line 4596 "fe/idl.ypp"
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
#line 7713 "fe/idl.tab.cpp"
    break;

  case 399: /* $@112: %empty  */
#line 4660 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSeen);
        }
#line 7721 "fe/idl.tab.cpp"
    break;

  case 400: /* @113: %empty  */
#line 4664 "fe/idl.ypp"
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
#line 7757 "fe/idl.tab.cpp"
    break;

  case 401: /* $@114: %empty  */
#line 4696 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSqSeen);
        }
#line 7765 "fe/idl.tab.cpp"
    break;

  case 402: /* $@115: %empty  */
#line 4700 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptBodySeen);
        }
#line 7773 "fe/idl.tab.cpp"
    break;

  case 403: /* exception: IDL_EXCEPTION $@112 defining_id @113 '{' $@114 members $@115 '}'  */
#line 4704 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptQsSeen);
          /*
           * Done with this exception. Pop its scope from the scope stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 7787 "fe/idl.tab.cpp"
    break;

  case 404: /* $@116: %empty  */
#line 4717 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 7795 "fe/idl.tab.cpp"
    break;

  case 405: /* $@117: %empty  */
#line 4721 "fe/idl.ypp"
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
#line 7860 "fe/idl.tab.cpp"
    break;

  case 406: /* $@118: %empty  */
#line 4782 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 7868 "fe/idl.tab.cpp"
    break;

  case 407: /* $@119: %empty  */
#line 4786 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);
        }
#line 7876 "fe/idl.tab.cpp"
    break;

  case 408: /* operation: opt_op_attribute op_type_spec $@116 IDENTIFIER $@117 parameter_list $@118 opt_raises $@119 opt_context  */
#line 4790 "fe/idl.ypp"
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
#line 7911 "fe/idl.tab.cpp"
    break;

  case 409: /* opt_op_attribute: IDL_ONEWAY  */
#line 4824 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_oneway;
        }
#line 7920 "fe/idl.tab.cpp"
    break;

  case 410: /* opt_op_attribute: IDL_IDEMPOTENT  */
#line 4829 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_idempotent;
        }
#line 7929 "fe/idl.tab.cpp"
    break;

  case 411: /* opt_op_attribute: %empty  */
#line 4834 "fe/idl.ypp"
        {
          (yyval.ofval) = AST_Operation::OP_noflags;
        }
#line 7937 "fe/idl.tab.cpp"
    break;

  case 413: /* op_type_spec: IDL_VOID  */
#line 4842 "fe/idl.ypp"
        {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                  AST_Expression::EV_void
                                                );
        }
#line 7948 "fe/idl.tab.cpp"
    break;

  case 414: /* $@120: %empty  */
#line 4852 "fe/idl.ypp"
        {
          //@@ PS_FactorySeen?
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 7957 "fe/idl.tab.cpp"
    break;

  case 415: /* @121: %empty  */
#line 4857 "fe/idl.ypp"
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
#line 8004 "fe/idl.tab.cpp"
    break;

  case 416: /* $@122: %empty  */
#line 4900 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 8012 "fe/idl.tab.cpp"
    break;

  case 417: /* init_decl: IDL_FACTORY $@120 IDENTIFIER @121 init_parameter_list $@122 opt_raises  */
#line 4904 "fe/idl.ypp"
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
#line 8031 "fe/idl.tab.cpp"
    break;

  case 418: /* $@123: %empty  */
#line 4922 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 8039 "fe/idl.tab.cpp"
    break;

  case 419: /* init_parameter_list: '(' $@123 ')'  */
#line 4926 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 8047 "fe/idl.tab.cpp"
    break;

  case 420: /* $@124: %empty  */
#line 4930 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 8055 "fe/idl.tab.cpp"
    break;

  case 421: /* init_parameter_list: '(' $@124 at_least_one_in_parameter ')'  */
#line 4935 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 8063 "fe/idl.tab.cpp"
    break;

  case 423: /* $@125: %empty  */
#line 4945 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 8071 "fe/idl.tab.cpp"
    break;

  case 426: /* $@126: %empty  */
#line 4954 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 8079 "fe/idl.tab.cpp"
    break;

  case 427: /* $@127: %empty  */
#line 4958 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 8087 "fe/idl.tab.cpp"
    break;

  case 428: /* in_parameter: IDL_IN $@126 param_type_spec $@127 declarator  */
#line 4962 "fe/idl.ypp"
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
#line 8123 "fe/idl.tab.cpp"
    break;

  case 429: /* $@128: %empty  */
#line 4997 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 8131 "fe/idl.tab.cpp"
    break;

  case 430: /* parameter_list: '(' $@128 ')'  */
#line 5001 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 8139 "fe/idl.tab.cpp"
    break;

  case 431: /* $@129: %empty  */
#line 5005 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 8147 "fe/idl.tab.cpp"
    break;

  case 432: /* parameter_list: '(' $@129 at_least_one_parameter ')'  */
#line 5010 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 8155 "fe/idl.tab.cpp"
    break;

  case 434: /* $@130: %empty  */
#line 5020 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 8163 "fe/idl.tab.cpp"
    break;

  case 437: /* $@131: %empty  */
#line 5029 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 8171 "fe/idl.tab.cpp"
    break;

  case 438: /* $@132: %empty  */
#line 5033 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 8179 "fe/idl.tab.cpp"
    break;

  case 439: /* parameter: direction $@131 param_type_spec $@132 declarator  */
#line 5037 "fe/idl.ypp"
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
#line 8222 "fe/idl.tab.cpp"
    break;

  case 440: /* param_type_spec: base_type_spec  */
#line 5079 "fe/idl.ypp"
        {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 8233 "fe/idl.tab.cpp"
    break;

  case 443: /* param_type_spec: scoped_name  */
#line 5088 "fe/idl.ypp"
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
#line 8389 "fe/idl.tab.cpp"
    break;

  case 444: /* direction: IDL_IN  */
#line 5243 "fe/idl.ypp"
        {
          (yyval.dival) = AST_Argument::dir_IN;
        }
#line 8397 "fe/idl.tab.cpp"
    break;

  case 445: /* direction: IDL_OUT  */
#line 5247 "fe/idl.ypp"
        {
          (yyval.dival) = AST_Argument::dir_OUT;
        }
#line 8405 "fe/idl.tab.cpp"
    break;

  case 446: /* direction: IDL_INOUT  */
#line 5251 "fe/idl.ypp"
        {
          (yyval.dival) = AST_Argument::dir_INOUT;
        }
#line 8413 "fe/idl.tab.cpp"
    break;

  case 447: /* $@133: %empty  */
#line 5258 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSeen);
        }
#line 8421 "fe/idl.tab.cpp"
    break;

  case 448: /* $@134: %empty  */
#line 5262 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSqSeen);
        }
#line 8429 "fe/idl.tab.cpp"
    break;

  case 449: /* opt_raises: IDL_RAISES $@133 '(' $@134 at_least_one_scoped_name ')'  */
#line 5267 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 8438 "fe/idl.tab.cpp"
    break;

  case 450: /* opt_raises: %empty  */
#line 5272 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 8446 "fe/idl.tab.cpp"
    break;

  case 451: /* $@135: %empty  */
#line 5279 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSeen);
        }
#line 8454 "fe/idl.tab.cpp"
    break;

  case 452: /* $@136: %empty  */
#line 5283 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSqSeen);
        }
#line 8462 "fe/idl.tab.cpp"
    break;

  case 453: /* opt_getraises: IDL_GETRAISES $@135 '(' $@136 at_least_one_scoped_name ')'  */
#line 5288 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 8471 "fe/idl.tab.cpp"
    break;

  case 454: /* opt_getraises: %empty  */
#line 5293 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 8479 "fe/idl.tab.cpp"
    break;

  case 455: /* $@137: %empty  */
#line 5300 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSeen);
        }
#line 8487 "fe/idl.tab.cpp"
    break;

  case 456: /* $@138: %empty  */
#line 5304 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSqSeen);
        }
#line 8495 "fe/idl.tab.cpp"
    break;

  case 457: /* opt_setraises: IDL_SETRAISES $@137 '(' $@138 at_least_one_scoped_name ')'  */
#line 5309 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 8504 "fe/idl.tab.cpp"
    break;

  case 458: /* opt_setraises: %empty  */
#line 5314 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 8512 "fe/idl.tab.cpp"
    break;

  case 459: /* $@139: %empty  */
#line 5321 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSeen);
        }
#line 8520 "fe/idl.tab.cpp"
    break;

  case 460: /* $@140: %empty  */
#line 5325 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSqSeen);
        }
#line 8528 "fe/idl.tab.cpp"
    break;

  case 461: /* opt_context: IDL_CONTEXT $@139 '(' $@140 at_least_one_string_literal ')'  */
#line 5330 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextQsSeen);
          (yyval.slval) = (yyvsp[-1].slval);
        }
#line 8537 "fe/idl.tab.cpp"
    break;

  case 462: /* opt_context: %empty  */
#line 5335 "fe/idl.ypp"
        {
          (yyval.slval) = 0;
        }
#line 8545 "fe/idl.tab.cpp"
    break;

  case 463: /* at_least_one_string_literal: IDL_STRING_LITERAL string_literals  */
#line 5342 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.slval),
                          UTL_StrList ((yyvsp[-1].sval),
                                       (yyvsp[0].slval)),
                          1);
        }
#line 8556 "fe/idl.tab.cpp"
    break;

  case 464: /* $@141: %empty  */
#line 5353 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextCommaSeen);
        }
#line 8564 "fe/idl.tab.cpp"
    break;

  case 465: /* string_literals: string_literals ',' $@141 IDL_STRING_LITERAL  */
#line 5357 "fe/idl.ypp"
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
#line 8586 "fe/idl.tab.cpp"
    break;

  case 466: /* string_literals: %empty  */
#line 5375 "fe/idl.ypp"
        {
          (yyval.slval) = 0;
        }
#line 8594 "fe/idl.tab.cpp"
    break;

  case 467: /* typeid_dcl: IDL_TYPEID scoped_name IDL_STRING_LITERAL  */
#line 5382 "fe/idl.ypp"
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
#line 8625 "fe/idl.tab.cpp"
    break;

  case 468: /* typeprefix_dcl: IDL_TYPEPREFIX scoped_name IDL_STRING_LITERAL  */
#line 5412 "fe/idl.ypp"
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
#line 8665 "fe/idl.tab.cpp"
    break;

  case 471: /* component_forward_decl: IDL_COMPONENT defining_id  */
#line 5457 "fe/idl.ypp"
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
#line 8695 "fe/idl.tab.cpp"
    break;

  case 472: /* @142: %empty  */
#line 5486 "fe/idl.ypp"
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
#line 8741 "fe/idl.tab.cpp"
    break;

  case 473: /* $@143: %empty  */
#line 5528 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentSqSeen);
        }
#line 8749 "fe/idl.tab.cpp"
    break;

  case 474: /* $@144: %empty  */
#line 5532 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentBodySeen);
        }
#line 8757 "fe/idl.tab.cpp"
    break;

  case 475: /* component_decl: component_header @142 '{' $@143 component_exports $@144 '}'  */
#line 5536 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentQsSeen);

          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 8772 "fe/idl.tab.cpp"
    break;

  case 476: /* $@145: %empty  */
#line 5551 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentIDSeen);
        }
#line 8780 "fe/idl.tab.cpp"
    break;

  case 477: /* $@146: %empty  */
#line 5555 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 8788 "fe/idl.tab.cpp"
    break;

  case 478: /* component_header: IDL_COMPONENT defining_id $@145 component_inheritance_spec $@146 supports_spec  */
#line 5559 "fe/idl.ypp"
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
#line 8826 "fe/idl.tab.cpp"
    break;

  case 479: /* $@147: %empty  */
#line 5596 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 8834 "fe/idl.tab.cpp"
    break;

  case 480: /* component_inheritance_spec: ':' $@147 scoped_name  */
#line 5600 "fe/idl.ypp"
        {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8842 "fe/idl.tab.cpp"
    break;

  case 481: /* component_inheritance_spec: %empty  */
#line 5604 "fe/idl.ypp"
        {
          (yyval.idlist) = 0;
        }
#line 8850 "fe/idl.tab.cpp"
    break;

  case 482: /* component_exports: component_exports at_least_one_annotation component_export  */
#line 5611 "fe/idl.ypp"
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
#line 8869 "fe/idl.tab.cpp"
    break;

  case 485: /* $@148: %empty  */
#line 5631 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 8877 "fe/idl.tab.cpp"
    break;

  case 486: /* component_export: provides_decl $@148 ';'  */
#line 5635 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8886 "fe/idl.tab.cpp"
    break;

  case 487: /* $@149: %empty  */
#line 5640 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 8894 "fe/idl.tab.cpp"
    break;

  case 488: /* component_export: uses_decl $@149 ';'  */
#line 5644 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8903 "fe/idl.tab.cpp"
    break;

  case 489: /* $@150: %empty  */
#line 5649 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EmitsDeclSeen);
        }
#line 8911 "fe/idl.tab.cpp"
    break;

  case 490: /* component_export: emits_decl $@150 ';'  */
#line 5653 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8920 "fe/idl.tab.cpp"
    break;

  case 491: /* $@151: %empty  */
#line 5658 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PublishesDeclSeen);
        }
#line 8928 "fe/idl.tab.cpp"
    break;

  case 492: /* component_export: publishes_decl $@151 ';'  */
#line 5662 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8937 "fe/idl.tab.cpp"
    break;

  case 493: /* $@152: %empty  */
#line 5667 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConsumesDeclSeen);
        }
#line 8945 "fe/idl.tab.cpp"
    break;

  case 494: /* component_export: consumes_decl $@152 ';'  */
#line 5671 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8954 "fe/idl.tab.cpp"
    break;

  case 495: /* $@153: %empty  */
#line 5676 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 8962 "fe/idl.tab.cpp"
    break;

  case 496: /* component_export: attribute $@153 ';'  */
#line 5680 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8971 "fe/idl.tab.cpp"
    break;

  case 497: /* $@154: %empty  */
#line 5685 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 8979 "fe/idl.tab.cpp"
    break;

  case 498: /* component_export: extended_port_decl $@154 ';'  */
#line 5689 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8988 "fe/idl.tab.cpp"
    break;

  case 499: /* provides_decl: IDL_PROVIDES interface_type id  */
#line 5696 "fe/idl.ypp"
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
#line 9082 "fe/idl.tab.cpp"
    break;

  case 500: /* interface_type: scoped_name  */
#line 5789 "fe/idl.ypp"
        {
          // Lookups and checking are done where the 'interface_type'
          // token is used, in 'provides_decl' and 'uses_decl'.
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 9092 "fe/idl.tab.cpp"
    break;

  case 501: /* interface_type: IDL_OBJECT  */
#line 5795 "fe/idl.ypp"
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
#line 9119 "fe/idl.tab.cpp"
    break;

  case 502: /* uses_decl: uses_opt_multiple interface_type id  */
#line 5820 "fe/idl.ypp"
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
#line 9227 "fe/idl.tab.cpp"
    break;

  case 503: /* uses_opt_multiple: IDL_USES opt_multiple  */
#line 5927 "fe/idl.ypp"
        {
          // We use this extra rule here to use in both uses_decl and
          // extended_uses_decl, so the LALR(1) parser can avoid conflicts.
          (yyval.bval) = (yyvsp[0].bval);
        }
#line 9237 "fe/idl.tab.cpp"
    break;

  case 504: /* opt_multiple: IDL_MULTIPLE  */
#line 5936 "fe/idl.ypp"
        {
          (yyval.bval) = true;
        }
#line 9245 "fe/idl.tab.cpp"
    break;

  case 505: /* opt_multiple: %empty  */
#line 5940 "fe/idl.ypp"
        {
          (yyval.bval) = false;
        }
#line 9253 "fe/idl.tab.cpp"
    break;

  case 506: /* emits_decl: IDL_EMITS scoped_name id  */
#line 5947 "fe/idl.ypp"
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
#line 9329 "fe/idl.tab.cpp"
    break;

  case 507: /* publishes_decl: IDL_PUBLISHES scoped_name id  */
#line 6022 "fe/idl.ypp"
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
#line 9402 "fe/idl.tab.cpp"
    break;

  case 508: /* consumes_decl: IDL_CONSUMES scoped_name id  */
#line 6094 "fe/idl.ypp"
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
#line 9478 "fe/idl.tab.cpp"
    break;

  case 509: /* $@155: %empty  */
#line 6169 "fe/idl.ypp"
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
#line 9519 "fe/idl.tab.cpp"
    break;

  case 510: /* home_decl: home_header $@155 home_body  */
#line 6206 "fe/idl.ypp"
        {
          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = 0;
        }
#line 9532 "fe/idl.tab.cpp"
    break;

  case 511: /* $@156: %empty  */
#line 6218 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSeen);
        }
#line 9540 "fe/idl.tab.cpp"
    break;

  case 512: /* $@157: %empty  */
#line 6222 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeIDSeen);
        }
#line 9548 "fe/idl.tab.cpp"
    break;

  case 513: /* $@158: %empty  */
#line 6226 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 9556 "fe/idl.tab.cpp"
    break;

  case 514: /* $@159: %empty  */
#line 6230 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);
        }
#line 9564 "fe/idl.tab.cpp"
    break;

  case 515: /* $@160: %empty  */
#line 6234 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesSeen);
        }
#line 9572 "fe/idl.tab.cpp"
    break;

  case 516: /* $@161: %empty  */
#line 6238 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesIDSeen);
        }
#line 9580 "fe/idl.tab.cpp"
    break;

  case 517: /* home_header: IDL_HOME $@156 defining_id $@157 home_inheritance_spec $@158 supports_spec $@159 IDL_MANAGES $@160 scoped_name $@161 primary_key_spec  */
#line 6242 "fe/idl.ypp"
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
#line 9630 "fe/idl.tab.cpp"
    break;

  case 518: /* $@162: %empty  */
#line 6291 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 9638 "fe/idl.tab.cpp"
    break;

  case 519: /* home_inheritance_spec: ':' $@162 scoped_name  */
#line 6295 "fe/idl.ypp"
        {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 9646 "fe/idl.tab.cpp"
    break;

  case 520: /* home_inheritance_spec: %empty  */
#line 6299 "fe/idl.ypp"
        {
          (yyval.idlist) = 0;
        }
#line 9654 "fe/idl.tab.cpp"
    break;

  case 521: /* primary_key_spec: IDL_PRIMARYKEY scoped_name  */
#line 6307 "fe/idl.ypp"
        {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 9662 "fe/idl.tab.cpp"
    break;

  case 522: /* primary_key_spec: %empty  */
#line 6311 "fe/idl.ypp"
        {
          (yyval.idlist) = 0;
        }
#line 9670 "fe/idl.tab.cpp"
    break;

  case 523: /* $@163: %empty  */
#line 6318 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSqSeen);
        }
#line 9678 "fe/idl.tab.cpp"
    break;

  case 524: /* $@164: %empty  */
#line 6322 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeBodySeen);
        }
#line 9686 "fe/idl.tab.cpp"
    break;

  case 525: /* home_body: '{' $@163 home_exports $@164 '}'  */
#line 6326 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeQsSeen);
        }
#line 9694 "fe/idl.tab.cpp"
    break;

  case 529: /* $@165: %empty  */
#line 6339 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_FactoryDeclSeen);
        }
#line 9702 "fe/idl.tab.cpp"
    break;

  case 530: /* home_export: factory_decl $@165 ';'  */
#line 6343 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 9710 "fe/idl.tab.cpp"
    break;

  case 531: /* $@166: %empty  */
#line 6347 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_FinderDeclSeen);
        }
#line 9718 "fe/idl.tab.cpp"
    break;

  case 532: /* home_export: finder_decl $@166 ';'  */
#line 6351 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 9726 "fe/idl.tab.cpp"
    break;

  case 533: /* $@167: %empty  */
#line 6359 "fe/idl.ypp"
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
#line 9753 "fe/idl.tab.cpp"
    break;

  case 534: /* $@168: %empty  */
#line 6382 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 9761 "fe/idl.tab.cpp"
    break;

  case 535: /* factory_decl: IDL_FACTORY defining_id $@167 init_parameter_list $@168 opt_raises  */
#line 6386 "fe/idl.ypp"
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
#line 9783 "fe/idl.tab.cpp"
    break;

  case 536: /* $@169: %empty  */
#line 6408 "fe/idl.ypp"
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
#line 9814 "fe/idl.tab.cpp"
    break;

  case 537: /* $@170: %empty  */
#line 6435 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 9822 "fe/idl.tab.cpp"
    break;

  case 538: /* finder_decl: IDL_FINDER defining_id $@169 init_parameter_list $@170 opt_raises  */
#line 6439 "fe/idl.ypp"
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
#line 9844 "fe/idl.tab.cpp"
    break;

  case 544: /* event_concrete_forward_decl: IDL_EVENTTYPE defining_id  */
#line 6472 "fe/idl.ypp"
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
#line 9873 "fe/idl.tab.cpp"
    break;

  case 545: /* event_abs_forward_decl: IDL_ABSTRACT IDL_EVENTTYPE defining_id  */
#line 6502 "fe/idl.ypp"
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
#line 9902 "fe/idl.tab.cpp"
    break;

  case 546: /* $@171: %empty  */
#line 6531 "fe/idl.ypp"
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
#line 9950 "fe/idl.tab.cpp"
    break;

  case 547: /* $@172: %empty  */
#line 6575 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 9958 "fe/idl.tab.cpp"
    break;

  case 548: /* $@173: %empty  */
#line 6579 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 9966 "fe/idl.tab.cpp"
    break;

  case 549: /* event_abs_decl: event_abs_header event_rest_of_header $@171 '{' $@172 exports $@173 '}'  */
#line 6583 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = 0;
        }
#line 9981 "fe/idl.tab.cpp"
    break;

  case 550: /* event_abs_header: IDL_ABSTRACT IDL_EVENTTYPE defining_id  */
#line 6599 "fe/idl.ypp"
        {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9989 "fe/idl.tab.cpp"
    break;

  case 551: /* event_custom_header: IDL_CUSTOM IDL_EVENTTYPE defining_id  */
#line 6608 "fe/idl.ypp"
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
#line 10006 "fe/idl.tab.cpp"
    break;

  case 552: /* event_plain_header: IDL_EVENTTYPE defining_id  */
#line 6625 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          (yyval.idval) = (yyvsp[0].idval);
        }
#line 10016 "fe/idl.tab.cpp"
    break;

  case 553: /* $@174: %empty  */
#line 6634 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 10024 "fe/idl.tab.cpp"
    break;

  case 554: /* event_rest_of_header: inheritance_spec $@174 supports_spec  */
#line 6638 "fe/idl.ypp"
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
#line 10057 "fe/idl.tab.cpp"
    break;

  case 555: /* @175: %empty  */
#line 6671 "fe/idl.ypp"
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
#line 10112 "fe/idl.tab.cpp"
    break;

  case 556: /* $@176: %empty  */
#line 6722 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 10120 "fe/idl.tab.cpp"
    break;

  case 557: /* $@177: %empty  */
#line 6726 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 10128 "fe/idl.tab.cpp"
    break;

  case 558: /* event_decl: event_header event_rest_of_header @175 '{' $@176 value_elements $@177 '}'  */
#line 6730 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 10143 "fe/idl.tab.cpp"
    break;

  case 559: /* event_header: event_custom_header  */
#line 6744 "fe/idl.ypp"
        {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 10151 "fe/idl.tab.cpp"
    break;

  case 560: /* event_header: event_plain_header  */
#line 6748 "fe/idl.ypp"
        {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 10159 "fe/idl.tab.cpp"
    break;

  case 561: /* formal_parameter_type: IDL_TYPENAME  */
#line 6755 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_type;
        }
#line 10167 "fe/idl.tab.cpp"
    break;

  case 562: /* formal_parameter_type: IDL_STRUCT  */
#line 6759 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_struct;
        }
#line 10175 "fe/idl.tab.cpp"
    break;

  case 563: /* formal_parameter_type: IDL_UNION  */
#line 6763 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_union;
        }
#line 10183 "fe/idl.tab.cpp"
    break;

  case 564: /* formal_parameter_type: IDL_EVENTTYPE  */
#line 6767 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_eventtype;
        }
#line 10191 "fe/idl.tab.cpp"
    break;

  case 565: /* formal_parameter_type: IDL_SEQUENCE  */
#line 6771 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_sequence;
        }
#line 10199 "fe/idl.tab.cpp"
    break;

  case 566: /* formal_parameter_type: IDL_MAP  */
#line 6775 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_map;
        }
#line 10207 "fe/idl.tab.cpp"
    break;

  case 567: /* formal_parameter_type: IDL_INTERFACE  */
#line 6779 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_interface;
        }
#line 10215 "fe/idl.tab.cpp"
    break;

  case 568: /* formal_parameter_type: IDL_VALUETYPE  */
#line 6783 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_valuetype;
        }
#line 10223 "fe/idl.tab.cpp"
    break;

  case 569: /* formal_parameter_type: IDL_ENUM  */
#line 6787 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_enum;
        }
#line 10231 "fe/idl.tab.cpp"
    break;

  case 570: /* formal_parameter_type: IDL_EXCEPTION  */
#line 6791 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_except;
        }
#line 10239 "fe/idl.tab.cpp"
    break;

  case 571: /* formal_parameter_type: IDL_CONST const_type  */
#line 6795 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_const;
          t_param_const_type = (yyvsp[0].etval);
        }
#line 10248 "fe/idl.tab.cpp"
    break;

  case 572: /* at_least_one_formal_parameter: formal_parameter formal_parameters  */
#line 6803 "fe/idl.ypp"
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
#line 10280 "fe/idl.tab.cpp"
    break;

  case 573: /* formal_parameters: formal_parameters ',' formal_parameter  */
#line 6834 "fe/idl.ypp"
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
#line 10297 "fe/idl.tab.cpp"
    break;

  case 574: /* formal_parameters: %empty  */
#line 6847 "fe/idl.ypp"
        {
          (yyval.plval) = 0;
        }
#line 10305 "fe/idl.tab.cpp"
    break;

  case 575: /* formal_parameter: formal_parameter_type IDENTIFIER  */
#line 6854 "fe/idl.ypp"
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
#line 10334 "fe/idl.tab.cpp"
    break;

  case 576: /* formal_parameter: IDL_SEQUENCE '<' IDENTIFIER '>' IDENTIFIER  */
#line 6879 "fe/idl.ypp"
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
#line 10353 "fe/idl.tab.cpp"
    break;

  case 577: /* at_least_one_formal_parameter_name: formal_parameter_name formal_parameter_names  */
#line 6897 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.slval), UTL_StrList ((yyvsp[-1].sval), (yyvsp[0].slval)), 1);
        }
#line 10361 "fe/idl.tab.cpp"
    break;

  case 578: /* formal_parameter_names: formal_parameter_names ',' formal_parameter_name  */
#line 6904 "fe/idl.ypp"
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
#line 10380 "fe/idl.tab.cpp"
    break;

  case 579: /* formal_parameter_names: %empty  */
#line 6919 "fe/idl.ypp"
        {
          (yyval.slval) = 0;
        }
#line 10388 "fe/idl.tab.cpp"
    break;

  case 580: /* formal_parameter_name: IDENTIFIER  */
#line 6926 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.sval),
                          UTL_String ((yyvsp[0].strval), true),
                          1);
        }
#line 10398 "fe/idl.tab.cpp"
    break;

  case 581: /* $@178: %empty  */
#line 6935 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSeen);
        }
#line 10406 "fe/idl.tab.cpp"
    break;

  case 582: /* @179: %empty  */
#line 6939 "fe/idl.ypp"
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
#line 10429 "fe/idl.tab.cpp"
    break;

  case 583: /* $@180: %empty  */
#line 6958 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSqSeen);
        }
#line 10437 "fe/idl.tab.cpp"
    break;

  case 584: /* $@181: %empty  */
#line 6966 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeBodySeen);
        }
#line 10445 "fe/idl.tab.cpp"
    break;

  case 585: /* porttype_decl: IDL_PORTTYPE $@178 IDENTIFIER @179 '{' $@180 at_least_one_port_export $@181 '}'  */
#line 6970 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeQsSeen);

          // Done with this port type - pop it off the scopes stack.
          idl_global->scopes ().pop ();

          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 10458 "fe/idl.tab.cpp"
    break;

  case 586: /* at_least_one_port_export: port_exports at_least_one_annotation port_export  */
#line 6982 "fe/idl.ypp"
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
#line 10477 "fe/idl.tab.cpp"
    break;

  case 592: /* $@182: %empty  */
#line 7008 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 10485 "fe/idl.tab.cpp"
    break;

  case 593: /* port_export: attribute $@182 ';'  */
#line 7012 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 10493 "fe/idl.tab.cpp"
    break;

  case 594: /* extended_port_decl: IDL_PORT scoped_name IDENTIFIER  */
#line 7019 "fe/idl.ypp"
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
#line 10564 "fe/idl.tab.cpp"
    break;

  case 595: /* extended_port_decl: IDL_MIRRORPORT scoped_name IDENTIFIER  */
#line 7086 "fe/idl.ypp"
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

              UTL_ScopedName sn (&id, 0);
              mp = idl_global->gen ()->create_mirror_port (&sn, pt);
              s->fe_add_mirror_port (mp);
            }

          (yyvsp[-1].idlist)->destroy ();
          delete (yyvsp[-1].idlist);
          (yyvsp[-1].idlist) = 0;

          (yyval.dcval) = mp;
        }
#line 10613 "fe/idl.tab.cpp"
    break;

  case 596: /* at_least_one_actual_parameter: annotations_maybe actual_parameter actual_parameters  */
#line 7134 "fe/idl.ypp"
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
#line 10630 "fe/idl.tab.cpp"
    break;

  case 597: /* actual_parameters: actual_parameters ',' annotations_maybe actual_parameter  */
#line 7150 "fe/idl.ypp"
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
#line 10647 "fe/idl.tab.cpp"
    break;

  case 598: /* actual_parameters: %empty  */
#line 7163 "fe/idl.ypp"
        {
          (yyval.alval) = 0;
        }
#line 10655 "fe/idl.tab.cpp"
    break;

  case 599: /* actual_parameter: expression  */
#line 7170 "fe/idl.ypp"
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
#line 10715 "fe/idl.tab.cpp"
    break;

  case 600: /* connector_decl: connector_header connector_body  */
#line 7229 "fe/idl.ypp"
        {
          (yyval.dcval) = 0;
        }
#line 10723 "fe/idl.tab.cpp"
    break;

  case 601: /* $@183: %empty  */
#line 7236 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSeen);
        }
#line 10731 "fe/idl.tab.cpp"
    break;

  case 602: /* $@184: %empty  */
#line 7240 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorIDSeen);
        }
#line 10739 "fe/idl.tab.cpp"
    break;

  case 603: /* connector_header: IDL_CONNECTOR $@183 annotations_maybe IDENTIFIER $@184 component_inheritance_spec  */
#line 7244 "fe/idl.ypp"
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
#line 10797 "fe/idl.tab.cpp"
    break;

  case 604: /* $@185: %empty  */
#line 7301 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSqSeen);
        }
#line 10805 "fe/idl.tab.cpp"
    break;

  case 605: /* $@186: %empty  */
#line 7305 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorBodySeen);
        }
#line 10813 "fe/idl.tab.cpp"
    break;

  case 606: /* connector_body: '{' $@185 connector_exports $@186 '}'  */
#line 7309 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorQsSeen);

          // Done with this connector - pop it off the scope stack.
          idl_global->scopes ().pop ();
        }
#line 10824 "fe/idl.tab.cpp"
    break;

  case 609: /* $@187: %empty  */
#line 7324 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 10832 "fe/idl.tab.cpp"
    break;

  case 610: /* connector_export: provides_decl $@187 ';'  */
#line 7328 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10840 "fe/idl.tab.cpp"
    break;

  case 611: /* $@188: %empty  */
#line 7332 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 10848 "fe/idl.tab.cpp"
    break;

  case 612: /* connector_export: uses_decl $@188 ';'  */
#line 7336 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10856 "fe/idl.tab.cpp"
    break;

  case 613: /* $@189: %empty  */
#line 7340 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 10864 "fe/idl.tab.cpp"
    break;

  case 614: /* connector_export: attribute $@189 ';'  */
#line 7344 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10872 "fe/idl.tab.cpp"
    break;

  case 615: /* $@190: %empty  */
#line 7348 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 10880 "fe/idl.tab.cpp"
    break;

  case 616: /* connector_export: extended_port_decl $@190 ';'  */
#line 7352 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10888 "fe/idl.tab.cpp"
    break;


#line 10892 "fe/idl.tab.cpp"

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

#line 7357 "fe/idl.ypp"

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
