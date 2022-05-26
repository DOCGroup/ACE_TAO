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

#line 166 "fe/idl.tab.cpp"

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
  YYSYMBOL_IDL_MAP = 25,                   /* IDL_MAP  */
  YYSYMBOL_IDL_STRING = 26,                /* IDL_STRING  */
  YYSYMBOL_IDL_WSTRING = 27,               /* IDL_WSTRING  */
  YYSYMBOL_IDL_EXCEPTION = 28,             /* IDL_EXCEPTION  */
  YYSYMBOL_IDL_CASE = 29,                  /* IDL_CASE  */
  YYSYMBOL_IDL_DEFAULT = 30,               /* IDL_DEFAULT  */
  YYSYMBOL_IDL_READONLY = 31,              /* IDL_READONLY  */
  YYSYMBOL_IDL_ATTRIBUTE = 32,             /* IDL_ATTRIBUTE  */
  YYSYMBOL_IDL_ONEWAY = 33,                /* IDL_ONEWAY  */
  YYSYMBOL_IDL_IDEMPOTENT = 34,            /* IDL_IDEMPOTENT  */
  YYSYMBOL_IDL_VOID = 35,                  /* IDL_VOID  */
  YYSYMBOL_IDL_IN = 36,                    /* IDL_IN  */
  YYSYMBOL_IDL_OUT = 37,                   /* IDL_OUT  */
  YYSYMBOL_IDL_INOUT = 38,                 /* IDL_INOUT  */
  YYSYMBOL_IDL_RAISES = 39,                /* IDL_RAISES  */
  YYSYMBOL_IDL_CONTEXT = 40,               /* IDL_CONTEXT  */
  YYSYMBOL_IDL_NATIVE = 41,                /* IDL_NATIVE  */
  YYSYMBOL_IDL_LOCAL = 42,                 /* IDL_LOCAL  */
  YYSYMBOL_IDL_ABSTRACT = 43,              /* IDL_ABSTRACT  */
  YYSYMBOL_IDL_CUSTOM = 44,                /* IDL_CUSTOM  */
  YYSYMBOL_IDL_FACTORY = 45,               /* IDL_FACTORY  */
  YYSYMBOL_IDL_PRIVATE = 46,               /* IDL_PRIVATE  */
  YYSYMBOL_IDL_PUBLIC = 47,                /* IDL_PUBLIC  */
  YYSYMBOL_IDL_SUPPORTS = 48,              /* IDL_SUPPORTS  */
  YYSYMBOL_IDL_TRUNCATABLE = 49,           /* IDL_TRUNCATABLE  */
  YYSYMBOL_IDL_VALUETYPE = 50,             /* IDL_VALUETYPE  */
  YYSYMBOL_IDL_COMPONENT = 51,             /* IDL_COMPONENT  */
  YYSYMBOL_IDL_CONSUMES = 52,              /* IDL_CONSUMES  */
  YYSYMBOL_IDL_EMITS = 53,                 /* IDL_EMITS  */
  YYSYMBOL_IDL_EVENTTYPE = 54,             /* IDL_EVENTTYPE  */
  YYSYMBOL_IDL_FINDER = 55,                /* IDL_FINDER  */
  YYSYMBOL_IDL_GETRAISES = 56,             /* IDL_GETRAISES  */
  YYSYMBOL_IDL_HOME = 57,                  /* IDL_HOME  */
  YYSYMBOL_IDL_IMPORT = 58,                /* IDL_IMPORT  */
  YYSYMBOL_IDL_MULTIPLE = 59,              /* IDL_MULTIPLE  */
  YYSYMBOL_IDL_PRIMARYKEY = 60,            /* IDL_PRIMARYKEY  */
  YYSYMBOL_IDL_PROVIDES = 61,              /* IDL_PROVIDES  */
  YYSYMBOL_IDL_PUBLISHES = 62,             /* IDL_PUBLISHES  */
  YYSYMBOL_IDL_SETRAISES = 63,             /* IDL_SETRAISES  */
  YYSYMBOL_IDL_TYPEID = 64,                /* IDL_TYPEID  */
  YYSYMBOL_IDL_TYPEPREFIX = 65,            /* IDL_TYPEPREFIX  */
  YYSYMBOL_IDL_USES = 66,                  /* IDL_USES  */
  YYSYMBOL_IDL_MANAGES = 67,               /* IDL_MANAGES  */
  YYSYMBOL_IDL_TYPENAME = 68,              /* IDL_TYPENAME  */
  YYSYMBOL_IDL_PORT = 69,                  /* IDL_PORT  */
  YYSYMBOL_IDL_MIRRORPORT = 70,            /* IDL_MIRRORPORT  */
  YYSYMBOL_IDL_PORTTYPE = 71,              /* IDL_PORTTYPE  */
  YYSYMBOL_IDL_CONNECTOR = 72,             /* IDL_CONNECTOR  */
  YYSYMBOL_IDL_ALIAS = 73,                 /* IDL_ALIAS  */
  YYSYMBOL_IDL_INTEGER_LITERAL = 74,       /* IDL_INTEGER_LITERAL  */
  YYSYMBOL_IDL_UINTEGER_LITERAL = 75,      /* IDL_UINTEGER_LITERAL  */
  YYSYMBOL_IDL_STRING_LITERAL = 76,        /* IDL_STRING_LITERAL  */
  YYSYMBOL_IDL_CHARACTER_LITERAL = 77,     /* IDL_CHARACTER_LITERAL  */
  YYSYMBOL_IDL_FLOATING_PT_LITERAL = 78,   /* IDL_FLOATING_PT_LITERAL  */
  YYSYMBOL_IDL_FIXED_PT_LITERAL = 79,      /* IDL_FIXED_PT_LITERAL  */
  YYSYMBOL_IDL_TRUETOK = 80,               /* IDL_TRUETOK  */
  YYSYMBOL_IDL_FALSETOK = 81,              /* IDL_FALSETOK  */
  YYSYMBOL_IDL_INT8 = 82,                  /* IDL_INT8  */
  YYSYMBOL_IDL_UINT8 = 83,                 /* IDL_UINT8  */
  YYSYMBOL_IDL_INT16 = 84,                 /* IDL_INT16  */
  YYSYMBOL_IDL_UINT16 = 85,                /* IDL_UINT16  */
  YYSYMBOL_IDL_INT32 = 86,                 /* IDL_INT32  */
  YYSYMBOL_IDL_UINT32 = 87,                /* IDL_UINT32  */
  YYSYMBOL_IDL_INT64 = 88,                 /* IDL_INT64  */
  YYSYMBOL_IDL_UINT64 = 89,                /* IDL_UINT64  */
  YYSYMBOL_IDL_SCOPE_DELIMITOR = 90,       /* IDL_SCOPE_DELIMITOR  */
  YYSYMBOL_IDL_LEFT_SHIFT = 91,            /* IDL_LEFT_SHIFT  */
  YYSYMBOL_IDL_RIGHT_SHIFT = 92,           /* IDL_RIGHT_SHIFT  */
  YYSYMBOL_IDL_WCHAR_LITERAL = 93,         /* IDL_WCHAR_LITERAL  */
  YYSYMBOL_IDL_WSTRING_LITERAL = 94,       /* IDL_WSTRING_LITERAL  */
  YYSYMBOL_IDL_ANNOTATION_DECL = 95,       /* IDL_ANNOTATION_DECL  */
  YYSYMBOL_IDL_ANNOTATION_SYMBOL = 96,     /* IDL_ANNOTATION_SYMBOL  */
  YYSYMBOL_97_ = 97,                       /* ';'  */
  YYSYMBOL_98_ = 98,                       /* '{'  */
  YYSYMBOL_99_ = 99,                       /* '}'  */
  YYSYMBOL_100_ = 100,                     /* '<'  */
  YYSYMBOL_101_ = 101,                     /* '>'  */
  YYSYMBOL_102_ = 102,                     /* ':'  */
  YYSYMBOL_103_ = 103,                     /* ','  */
  YYSYMBOL_104_ = 104,                     /* '='  */
  YYSYMBOL_105_ = 105,                     /* '|'  */
  YYSYMBOL_106_ = 106,                     /* '^'  */
  YYSYMBOL_107_ = 107,                     /* '&'  */
  YYSYMBOL_108_ = 108,                     /* '+'  */
  YYSYMBOL_109_ = 109,                     /* '-'  */
  YYSYMBOL_110_ = 110,                     /* '*'  */
  YYSYMBOL_111_ = 111,                     /* '/'  */
  YYSYMBOL_112_ = 112,                     /* '%'  */
  YYSYMBOL_113_ = 113,                     /* '~'  */
  YYSYMBOL_114_ = 114,                     /* '('  */
  YYSYMBOL_115_ = 115,                     /* ')'  */
  YYSYMBOL_116_ = 116,                     /* '['  */
  YYSYMBOL_117_ = 117,                     /* ']'  */
  YYSYMBOL_YYACCEPT = 118,                 /* $accept  */
  YYSYMBOL_start = 119,                    /* start  */
  YYSYMBOL_definitions = 120,              /* definitions  */
  YYSYMBOL_at_least_one_definition = 121,  /* at_least_one_definition  */
  YYSYMBOL_definition = 122,               /* definition  */
  YYSYMBOL_fixed_definition = 123,         /* fixed_definition  */
  YYSYMBOL_124_1 = 124,                    /* $@1  */
  YYSYMBOL_125_2 = 125,                    /* $@2  */
  YYSYMBOL_126_3 = 126,                    /* $@3  */
  YYSYMBOL_127_4 = 127,                    /* $@4  */
  YYSYMBOL_128_5 = 128,                    /* $@5  */
  YYSYMBOL_129_6 = 129,                    /* $@6  */
  YYSYMBOL_130_7 = 130,                    /* $@7  */
  YYSYMBOL_131_8 = 131,                    /* $@8  */
  YYSYMBOL_132_9 = 132,                    /* $@9  */
  YYSYMBOL_133_10 = 133,                   /* $@10  */
  YYSYMBOL_134_11 = 134,                   /* $@11  */
  YYSYMBOL_135_12 = 135,                   /* $@12  */
  YYSYMBOL_136_13 = 136,                   /* $@13  */
  YYSYMBOL_137_14 = 137,                   /* $@14  */
  YYSYMBOL_138_15 = 138,                   /* $@15  */
  YYSYMBOL_module_header = 139,            /* module_header  */
  YYSYMBOL_140_16 = 140,                   /* $@16  */
  YYSYMBOL_module = 141,                   /* module  */
  YYSYMBOL_142_17 = 142,                   /* @17  */
  YYSYMBOL_143_18 = 143,                   /* $@18  */
  YYSYMBOL_144_19 = 144,                   /* $@19  */
  YYSYMBOL_template_module_header = 145,   /* template_module_header  */
  YYSYMBOL_template_module = 146,          /* template_module  */
  YYSYMBOL_147_20 = 147,                   /* $@20  */
  YYSYMBOL_148_21 = 148,                   /* $@21  */
  YYSYMBOL_149_22 = 149,                   /* $@22  */
  YYSYMBOL_150_23 = 150,                   /* $@23  */
  YYSYMBOL_151_24 = 151,                   /* $@24  */
  YYSYMBOL_at_least_one_tpl_definition = 152, /* at_least_one_tpl_definition  */
  YYSYMBOL_tpl_definitions = 153,          /* tpl_definitions  */
  YYSYMBOL_tpl_definition = 154,           /* tpl_definition  */
  YYSYMBOL_template_module_ref = 155,      /* template_module_ref  */
  YYSYMBOL_156_25 = 156,                   /* $@25  */
  YYSYMBOL_157_26 = 157,                   /* $@26  */
  YYSYMBOL_template_module_inst = 158,     /* template_module_inst  */
  YYSYMBOL_159_27 = 159,                   /* $@27  */
  YYSYMBOL_160_28 = 160,                   /* $@28  */
  YYSYMBOL_interface_def = 161,            /* interface_def  */
  YYSYMBOL_interface = 162,                /* interface  */
  YYSYMBOL_163_29 = 163,                   /* $@29  */
  YYSYMBOL_164_30 = 164,                   /* $@30  */
  YYSYMBOL_165_31 = 165,                   /* $@31  */
  YYSYMBOL_interface_decl = 166,           /* interface_decl  */
  YYSYMBOL_167_32 = 167,                   /* $@32  */
  YYSYMBOL_interface_header = 168,         /* interface_header  */
  YYSYMBOL_inheritance_spec = 169,         /* inheritance_spec  */
  YYSYMBOL_170_33 = 170,                   /* $@33  */
  YYSYMBOL_value_def = 171,                /* value_def  */
  YYSYMBOL_valuetype = 172,                /* valuetype  */
  YYSYMBOL_value_concrete_decl = 173,      /* value_concrete_decl  */
  YYSYMBOL_174_34 = 174,                   /* @34  */
  YYSYMBOL_175_35 = 175,                   /* $@35  */
  YYSYMBOL_176_36 = 176,                   /* $@36  */
  YYSYMBOL_value_abs_decl = 177,           /* value_abs_decl  */
  YYSYMBOL_178_37 = 178,                   /* $@37  */
  YYSYMBOL_179_38 = 179,                   /* $@38  */
  YYSYMBOL_180_39 = 180,                   /* $@39  */
  YYSYMBOL_value_header = 181,             /* value_header  */
  YYSYMBOL_182_40 = 182,                   /* $@40  */
  YYSYMBOL_value_decl = 183,               /* value_decl  */
  YYSYMBOL_184_41 = 184,                   /* $@41  */
  YYSYMBOL_opt_truncatable = 185,          /* opt_truncatable  */
  YYSYMBOL_supports_spec = 186,            /* supports_spec  */
  YYSYMBOL_value_forward_decl = 187,       /* value_forward_decl  */
  YYSYMBOL_value_box_decl = 188,           /* value_box_decl  */
  YYSYMBOL_value_elements = 189,           /* value_elements  */
  YYSYMBOL_value_element = 190,            /* value_element  */
  YYSYMBOL_191_42 = 191,                   /* @42  */
  YYSYMBOL_visibility = 192,               /* visibility  */
  YYSYMBOL_state_member = 193,             /* state_member  */
  YYSYMBOL_exports = 194,                  /* exports  */
  YYSYMBOL_at_least_one_export = 195,      /* at_least_one_export  */
  YYSYMBOL_export = 196,                   /* export  */
  YYSYMBOL_197_43 = 197,                   /* $@43  */
  YYSYMBOL_198_44 = 198,                   /* $@44  */
  YYSYMBOL_199_45 = 199,                   /* $@45  */
  YYSYMBOL_200_46 = 200,                   /* $@46  */
  YYSYMBOL_201_47 = 201,                   /* $@47  */
  YYSYMBOL_202_48 = 202,                   /* $@48  */
  YYSYMBOL_203_49 = 203,                   /* $@49  */
  YYSYMBOL_204_50 = 204,                   /* $@50  */
  YYSYMBOL_at_least_one_scoped_name = 205, /* at_least_one_scoped_name  */
  YYSYMBOL_scoped_names = 206,             /* scoped_names  */
  YYSYMBOL_207_51 = 207,                   /* $@51  */
  YYSYMBOL_scoped_name = 208,              /* scoped_name  */
  YYSYMBOL_209_52 = 209,                   /* $@52  */
  YYSYMBOL_210_53 = 210,                   /* $@53  */
  YYSYMBOL_id = 211,                       /* id  */
  YYSYMBOL_defining_id = 212,              /* defining_id  */
  YYSYMBOL_interface_forward = 213,        /* interface_forward  */
  YYSYMBOL_const_dcl = 214,                /* const_dcl  */
  YYSYMBOL_215_54 = 215,                   /* $@54  */
  YYSYMBOL_216_55 = 216,                   /* $@55  */
  YYSYMBOL_217_56 = 217,                   /* $@56  */
  YYSYMBOL_218_57 = 218,                   /* $@57  */
  YYSYMBOL_const_type = 219,               /* const_type  */
  YYSYMBOL_expression = 220,               /* expression  */
  YYSYMBOL_const_expr = 221,               /* const_expr  */
  YYSYMBOL_or_expr = 222,                  /* or_expr  */
  YYSYMBOL_xor_expr = 223,                 /* xor_expr  */
  YYSYMBOL_and_expr = 224,                 /* and_expr  */
  YYSYMBOL_shift_expr = 225,               /* shift_expr  */
  YYSYMBOL_add_expr = 226,                 /* add_expr  */
  YYSYMBOL_mult_expr = 227,                /* mult_expr  */
  YYSYMBOL_unary_expr = 228,               /* unary_expr  */
  YYSYMBOL_primary_expr = 229,             /* primary_expr  */
  YYSYMBOL_literal = 230,                  /* literal  */
  YYSYMBOL_positive_int_expr = 231,        /* positive_int_expr  */
  YYSYMBOL_annotation_dcl = 232,           /* annotation_dcl  */
  YYSYMBOL_233_58 = 233,                   /* $@58  */
  YYSYMBOL_annotation_body = 234,          /* annotation_body  */
  YYSYMBOL_annotation_statement = 235,     /* annotation_statement  */
  YYSYMBOL_236_59 = 236,                   /* $@59  */
  YYSYMBOL_annotation_member_type = 237,   /* annotation_member_type  */
  YYSYMBOL_annotation_member = 238,        /* annotation_member  */
  YYSYMBOL_annotation_member_default = 239, /* annotation_member_default  */
  YYSYMBOL_at_least_one_annotation = 240,  /* at_least_one_annotation  */
  YYSYMBOL_annotations_maybe = 241,        /* annotations_maybe  */
  YYSYMBOL_annotation_appl = 242,          /* annotation_appl  */
  YYSYMBOL_243_60 = 243,                   /* @60  */
  YYSYMBOL_annotation_appl_params_maybe = 244, /* annotation_appl_params_maybe  */
  YYSYMBOL_annotation_appl_params = 245,   /* annotation_appl_params  */
  YYSYMBOL_named_annotation_appl_params = 246, /* named_annotation_appl_params  */
  YYSYMBOL_more_named_annotation_appl_params = 247, /* more_named_annotation_appl_params  */
  YYSYMBOL_named_annotation_appl_param = 248, /* named_annotation_appl_param  */
  YYSYMBOL_type_dcl = 249,                 /* type_dcl  */
  YYSYMBOL_250_61 = 250,                   /* $@61  */
  YYSYMBOL_type_declarator = 251,          /* type_declarator  */
  YYSYMBOL_252_62 = 252,                   /* $@62  */
  YYSYMBOL_type_spec = 253,                /* type_spec  */
  YYSYMBOL_simple_type_spec = 254,         /* simple_type_spec  */
  YYSYMBOL_base_type_spec = 255,           /* base_type_spec  */
  YYSYMBOL_template_type_spec = 256,       /* template_type_spec  */
  YYSYMBOL_constructed_type_spec = 257,    /* constructed_type_spec  */
  YYSYMBOL_constructed_forward_type_spec = 258, /* constructed_forward_type_spec  */
  YYSYMBOL_at_least_one_declarator = 259,  /* at_least_one_declarator  */
  YYSYMBOL_declarators = 260,              /* declarators  */
  YYSYMBOL_261_63 = 261,                   /* $@63  */
  YYSYMBOL_declarator = 262,               /* declarator  */
  YYSYMBOL_at_least_one_simple_declarator = 263, /* at_least_one_simple_declarator  */
  YYSYMBOL_simple_declarators = 264,       /* simple_declarators  */
  YYSYMBOL_265_64 = 265,                   /* $@64  */
  YYSYMBOL_simple_declarator = 266,        /* simple_declarator  */
  YYSYMBOL_complex_declarator = 267,       /* complex_declarator  */
  YYSYMBOL_integer_type = 268,             /* integer_type  */
  YYSYMBOL_signed_int = 269,               /* signed_int  */
  YYSYMBOL_unsigned_int = 270,             /* unsigned_int  */
  YYSYMBOL_floating_pt_type = 271,         /* floating_pt_type  */
  YYSYMBOL_fixed_type = 272,               /* fixed_type  */
  YYSYMBOL_char_type = 273,                /* char_type  */
  YYSYMBOL_octet_type = 274,               /* octet_type  */
  YYSYMBOL_boolean_type = 275,             /* boolean_type  */
  YYSYMBOL_any_type = 276,                 /* any_type  */
  YYSYMBOL_object_type = 277,              /* object_type  */
  YYSYMBOL_struct_decl = 278,              /* struct_decl  */
  YYSYMBOL_279_65 = 279,                   /* $@65  */
  YYSYMBOL_struct_type = 280,              /* struct_type  */
  YYSYMBOL_281_66 = 281,                   /* $@66  */
  YYSYMBOL_282_67 = 282,                   /* $@67  */
  YYSYMBOL_283_68 = 283,                   /* $@68  */
  YYSYMBOL_at_least_one_member = 284,      /* at_least_one_member  */
  YYSYMBOL_members = 285,                  /* members  */
  YYSYMBOL_member = 286,                   /* member  */
  YYSYMBOL_member_i = 287,                 /* member_i  */
  YYSYMBOL_288_69 = 288,                   /* $@69  */
  YYSYMBOL_289_70 = 289,                   /* $@70  */
  YYSYMBOL_290_71 = 290,                   /* $@71  */
  YYSYMBOL_union_decl = 291,               /* union_decl  */
  YYSYMBOL_292_72 = 292,                   /* $@72  */
  YYSYMBOL_union_type = 293,               /* union_type  */
  YYSYMBOL_294_73 = 294,                   /* $@73  */
  YYSYMBOL_295_74 = 295,                   /* $@74  */
  YYSYMBOL_296_75 = 296,                   /* $@75  */
  YYSYMBOL_297_76 = 297,                   /* $@76  */
  YYSYMBOL_298_77 = 298,                   /* $@77  */
  YYSYMBOL_299_78 = 299,                   /* $@78  */
  YYSYMBOL_switch_type_spec = 300,         /* switch_type_spec  */
  YYSYMBOL_at_least_one_case_branch = 301, /* at_least_one_case_branch  */
  YYSYMBOL_case_branches = 302,            /* case_branches  */
  YYSYMBOL_case_branch = 303,              /* case_branch  */
  YYSYMBOL_304_79 = 304,                   /* $@79  */
  YYSYMBOL_305_80 = 305,                   /* $@80  */
  YYSYMBOL_306_81 = 306,                   /* $@81  */
  YYSYMBOL_at_least_one_case_label = 307,  /* at_least_one_case_label  */
  YYSYMBOL_case_labels = 308,              /* case_labels  */
  YYSYMBOL_case_label = 309,               /* case_label  */
  YYSYMBOL_310_82 = 310,                   /* $@82  */
  YYSYMBOL_311_83 = 311,                   /* $@83  */
  YYSYMBOL_312_84 = 312,                   /* $@84  */
  YYSYMBOL_element_spec = 313,             /* element_spec  */
  YYSYMBOL_314_85 = 314,                   /* $@85  */
  YYSYMBOL_struct_forward_type = 315,      /* struct_forward_type  */
  YYSYMBOL_union_forward_type = 316,       /* union_forward_type  */
  YYSYMBOL_enum_type = 317,                /* enum_type  */
  YYSYMBOL_318_86 = 318,                   /* $@86  */
  YYSYMBOL_319_87 = 319,                   /* $@87  */
  YYSYMBOL_320_88 = 320,                   /* $@88  */
  YYSYMBOL_321_89 = 321,                   /* $@89  */
  YYSYMBOL_at_least_one_enumerator = 322,  /* at_least_one_enumerator  */
  YYSYMBOL_enumerators = 323,              /* enumerators  */
  YYSYMBOL_324_90 = 324,                   /* $@90  */
  YYSYMBOL_enumerator = 325,               /* enumerator  */
  YYSYMBOL_map_type_spec = 326,            /* map_type_spec  */
  YYSYMBOL_327_91 = 327,                   /* $@91  */
  YYSYMBOL_328_92 = 328,                   /* $@92  */
  YYSYMBOL_329_93 = 329,                   /* $@93  */
  YYSYMBOL_map_type = 330,                 /* map_type  */
  YYSYMBOL_sequence_type_spec = 331,       /* sequence_type_spec  */
  YYSYMBOL_332_94 = 332,                   /* $@94  */
  YYSYMBOL_333_95 = 333,                   /* $@95  */
  YYSYMBOL_seq_head = 334,                 /* seq_head  */
  YYSYMBOL_335_96 = 335,                   /* $@96  */
  YYSYMBOL_336_97 = 336,                   /* $@97  */
  YYSYMBOL_fixed_type_spec = 337,          /* fixed_type_spec  */
  YYSYMBOL_string_type_spec = 338,         /* string_type_spec  */
  YYSYMBOL_339_98 = 339,                   /* $@98  */
  YYSYMBOL_340_99 = 340,                   /* $@99  */
  YYSYMBOL_string_head = 341,              /* string_head  */
  YYSYMBOL_wstring_type_spec = 342,        /* wstring_type_spec  */
  YYSYMBOL_343_100 = 343,                  /* $@100  */
  YYSYMBOL_344_101 = 344,                  /* $@101  */
  YYSYMBOL_wstring_head = 345,             /* wstring_head  */
  YYSYMBOL_array_declarator = 346,         /* array_declarator  */
  YYSYMBOL_347_102 = 347,                  /* $@102  */
  YYSYMBOL_at_least_one_array_dim = 348,   /* at_least_one_array_dim  */
  YYSYMBOL_array_dims = 349,               /* array_dims  */
  YYSYMBOL_array_dim = 350,                /* array_dim  */
  YYSYMBOL_351_103 = 351,                  /* $@103  */
  YYSYMBOL_352_104 = 352,                  /* $@104  */
  YYSYMBOL_attribute = 353,                /* attribute  */
  YYSYMBOL_attribute_readonly = 354,       /* attribute_readonly  */
  YYSYMBOL_355_105 = 355,                  /* $@105  */
  YYSYMBOL_356_106 = 356,                  /* $@106  */
  YYSYMBOL_357_107 = 357,                  /* $@107  */
  YYSYMBOL_358_108 = 358,                  /* $@108  */
  YYSYMBOL_attribute_readwrite = 359,      /* attribute_readwrite  */
  YYSYMBOL_360_109 = 360,                  /* $@109  */
  YYSYMBOL_361_110 = 361,                  /* $@110  */
  YYSYMBOL_362_111 = 362,                  /* $@111  */
  YYSYMBOL_363_112 = 363,                  /* $@112  */
  YYSYMBOL_exception = 364,                /* exception  */
  YYSYMBOL_365_113 = 365,                  /* $@113  */
  YYSYMBOL_366_114 = 366,                  /* @114  */
  YYSYMBOL_367_115 = 367,                  /* $@115  */
  YYSYMBOL_368_116 = 368,                  /* $@116  */
  YYSYMBOL_operation = 369,                /* operation  */
  YYSYMBOL_370_117 = 370,                  /* $@117  */
  YYSYMBOL_371_118 = 371,                  /* $@118  */
  YYSYMBOL_372_119 = 372,                  /* $@119  */
  YYSYMBOL_373_120 = 373,                  /* $@120  */
  YYSYMBOL_opt_op_attribute = 374,         /* opt_op_attribute  */
  YYSYMBOL_op_type_spec = 375,             /* op_type_spec  */
  YYSYMBOL_init_decl = 376,                /* init_decl  */
  YYSYMBOL_377_121 = 377,                  /* $@121  */
  YYSYMBOL_378_122 = 378,                  /* @122  */
  YYSYMBOL_379_123 = 379,                  /* $@123  */
  YYSYMBOL_init_parameter_list = 380,      /* init_parameter_list  */
  YYSYMBOL_381_124 = 381,                  /* $@124  */
  YYSYMBOL_382_125 = 382,                  /* $@125  */
  YYSYMBOL_at_least_one_in_parameter = 383, /* at_least_one_in_parameter  */
  YYSYMBOL_in_parameters = 384,            /* in_parameters  */
  YYSYMBOL_385_126 = 385,                  /* $@126  */
  YYSYMBOL_in_parameter = 386,             /* in_parameter  */
  YYSYMBOL_387_127 = 387,                  /* $@127  */
  YYSYMBOL_388_128 = 388,                  /* $@128  */
  YYSYMBOL_parameter_list = 389,           /* parameter_list  */
  YYSYMBOL_390_129 = 390,                  /* $@129  */
  YYSYMBOL_391_130 = 391,                  /* $@130  */
  YYSYMBOL_at_least_one_parameter = 392,   /* at_least_one_parameter  */
  YYSYMBOL_parameters = 393,               /* parameters  */
  YYSYMBOL_394_131 = 394,                  /* $@131  */
  YYSYMBOL_parameter = 395,                /* parameter  */
  YYSYMBOL_396_132 = 396,                  /* $@132  */
  YYSYMBOL_397_133 = 397,                  /* $@133  */
  YYSYMBOL_param_type_spec = 398,          /* param_type_spec  */
  YYSYMBOL_direction = 399,                /* direction  */
  YYSYMBOL_opt_raises = 400,               /* opt_raises  */
  YYSYMBOL_401_134 = 401,                  /* $@134  */
  YYSYMBOL_402_135 = 402,                  /* $@135  */
  YYSYMBOL_opt_getraises = 403,            /* opt_getraises  */
  YYSYMBOL_404_136 = 404,                  /* $@136  */
  YYSYMBOL_405_137 = 405,                  /* $@137  */
  YYSYMBOL_opt_setraises = 406,            /* opt_setraises  */
  YYSYMBOL_407_138 = 407,                  /* $@138  */
  YYSYMBOL_408_139 = 408,                  /* $@139  */
  YYSYMBOL_opt_context = 409,              /* opt_context  */
  YYSYMBOL_410_140 = 410,                  /* $@140  */
  YYSYMBOL_411_141 = 411,                  /* $@141  */
  YYSYMBOL_at_least_one_string_literal = 412, /* at_least_one_string_literal  */
  YYSYMBOL_string_literals = 413,          /* string_literals  */
  YYSYMBOL_414_142 = 414,                  /* $@142  */
  YYSYMBOL_typeid_dcl = 415,               /* typeid_dcl  */
  YYSYMBOL_typeprefix_dcl = 416,           /* typeprefix_dcl  */
  YYSYMBOL_component = 417,                /* component  */
  YYSYMBOL_component_forward_decl = 418,   /* component_forward_decl  */
  YYSYMBOL_component_decl = 419,           /* component_decl  */
  YYSYMBOL_420_143 = 420,                  /* @143  */
  YYSYMBOL_421_144 = 421,                  /* $@144  */
  YYSYMBOL_422_145 = 422,                  /* $@145  */
  YYSYMBOL_component_header = 423,         /* component_header  */
  YYSYMBOL_424_146 = 424,                  /* $@146  */
  YYSYMBOL_425_147 = 425,                  /* $@147  */
  YYSYMBOL_component_inheritance_spec = 426, /* component_inheritance_spec  */
  YYSYMBOL_427_148 = 427,                  /* $@148  */
  YYSYMBOL_component_exports = 428,        /* component_exports  */
  YYSYMBOL_component_export = 429,         /* component_export  */
  YYSYMBOL_430_149 = 430,                  /* $@149  */
  YYSYMBOL_431_150 = 431,                  /* $@150  */
  YYSYMBOL_432_151 = 432,                  /* $@151  */
  YYSYMBOL_433_152 = 433,                  /* $@152  */
  YYSYMBOL_434_153 = 434,                  /* $@153  */
  YYSYMBOL_435_154 = 435,                  /* $@154  */
  YYSYMBOL_436_155 = 436,                  /* $@155  */
  YYSYMBOL_provides_decl = 437,            /* provides_decl  */
  YYSYMBOL_interface_type = 438,           /* interface_type  */
  YYSYMBOL_uses_decl = 439,                /* uses_decl  */
  YYSYMBOL_uses_opt_multiple = 440,        /* uses_opt_multiple  */
  YYSYMBOL_opt_multiple = 441,             /* opt_multiple  */
  YYSYMBOL_emits_decl = 442,               /* emits_decl  */
  YYSYMBOL_publishes_decl = 443,           /* publishes_decl  */
  YYSYMBOL_consumes_decl = 444,            /* consumes_decl  */
  YYSYMBOL_home_decl = 445,                /* home_decl  */
  YYSYMBOL_446_156 = 446,                  /* $@156  */
  YYSYMBOL_home_header = 447,              /* home_header  */
  YYSYMBOL_448_157 = 448,                  /* $@157  */
  YYSYMBOL_449_158 = 449,                  /* $@158  */
  YYSYMBOL_450_159 = 450,                  /* $@159  */
  YYSYMBOL_451_160 = 451,                  /* $@160  */
  YYSYMBOL_452_161 = 452,                  /* $@161  */
  YYSYMBOL_453_162 = 453,                  /* $@162  */
  YYSYMBOL_home_inheritance_spec = 454,    /* home_inheritance_spec  */
  YYSYMBOL_455_163 = 455,                  /* $@163  */
  YYSYMBOL_primary_key_spec = 456,         /* primary_key_spec  */
  YYSYMBOL_home_body = 457,                /* home_body  */
  YYSYMBOL_458_164 = 458,                  /* $@164  */
  YYSYMBOL_459_165 = 459,                  /* $@165  */
  YYSYMBOL_home_exports = 460,             /* home_exports  */
  YYSYMBOL_home_export = 461,              /* home_export  */
  YYSYMBOL_462_166 = 462,                  /* $@166  */
  YYSYMBOL_463_167 = 463,                  /* $@167  */
  YYSYMBOL_factory_decl = 464,             /* factory_decl  */
  YYSYMBOL_465_168 = 465,                  /* $@168  */
  YYSYMBOL_466_169 = 466,                  /* $@169  */
  YYSYMBOL_finder_decl = 467,              /* finder_decl  */
  YYSYMBOL_468_170 = 468,                  /* $@170  */
  YYSYMBOL_469_171 = 469,                  /* $@171  */
  YYSYMBOL_event = 470,                    /* event  */
  YYSYMBOL_event_forward_decl = 471,       /* event_forward_decl  */
  YYSYMBOL_event_concrete_forward_decl = 472, /* event_concrete_forward_decl  */
  YYSYMBOL_event_abs_forward_decl = 473,   /* event_abs_forward_decl  */
  YYSYMBOL_event_abs_decl = 474,           /* event_abs_decl  */
  YYSYMBOL_475_172 = 475,                  /* $@172  */
  YYSYMBOL_476_173 = 476,                  /* $@173  */
  YYSYMBOL_477_174 = 477,                  /* $@174  */
  YYSYMBOL_event_abs_header = 478,         /* event_abs_header  */
  YYSYMBOL_event_custom_header = 479,      /* event_custom_header  */
  YYSYMBOL_event_plain_header = 480,       /* event_plain_header  */
  YYSYMBOL_event_rest_of_header = 481,     /* event_rest_of_header  */
  YYSYMBOL_482_175 = 482,                  /* $@175  */
  YYSYMBOL_event_decl = 483,               /* event_decl  */
  YYSYMBOL_484_176 = 484,                  /* @176  */
  YYSYMBOL_485_177 = 485,                  /* $@177  */
  YYSYMBOL_486_178 = 486,                  /* $@178  */
  YYSYMBOL_event_header = 487,             /* event_header  */
  YYSYMBOL_formal_parameter_type = 488,    /* formal_parameter_type  */
  YYSYMBOL_at_least_one_formal_parameter = 489, /* at_least_one_formal_parameter  */
  YYSYMBOL_formal_parameters = 490,        /* formal_parameters  */
  YYSYMBOL_formal_parameter = 491,         /* formal_parameter  */
  YYSYMBOL_at_least_one_formal_parameter_name = 492, /* at_least_one_formal_parameter_name  */
  YYSYMBOL_formal_parameter_names = 493,   /* formal_parameter_names  */
  YYSYMBOL_formal_parameter_name = 494,    /* formal_parameter_name  */
  YYSYMBOL_porttype_decl = 495,            /* porttype_decl  */
  YYSYMBOL_496_179 = 496,                  /* $@179  */
  YYSYMBOL_497_180 = 497,                  /* @180  */
  YYSYMBOL_498_181 = 498,                  /* $@181  */
  YYSYMBOL_499_182 = 499,                  /* $@182  */
  YYSYMBOL_at_least_one_port_export = 500, /* at_least_one_port_export  */
  YYSYMBOL_port_exports = 501,             /* port_exports  */
  YYSYMBOL_port_export = 502,              /* port_export  */
  YYSYMBOL_503_183 = 503,                  /* $@183  */
  YYSYMBOL_extended_port_decl = 504,       /* extended_port_decl  */
  YYSYMBOL_at_least_one_actual_parameter = 505, /* at_least_one_actual_parameter  */
  YYSYMBOL_actual_parameters = 506,        /* actual_parameters  */
  YYSYMBOL_actual_parameter = 507,         /* actual_parameter  */
  YYSYMBOL_connector_decl = 508,           /* connector_decl  */
  YYSYMBOL_connector_header = 509,         /* connector_header  */
  YYSYMBOL_510_184 = 510,                  /* $@184  */
  YYSYMBOL_511_185 = 511,                  /* $@185  */
  YYSYMBOL_connector_body = 512,           /* connector_body  */
  YYSYMBOL_513_186 = 513,                  /* $@186  */
  YYSYMBOL_514_187 = 514,                  /* $@187  */
  YYSYMBOL_connector_exports = 515,        /* connector_exports  */
  YYSYMBOL_connector_export = 516,         /* connector_export  */
  YYSYMBOL_517_188 = 517,                  /* $@188  */
  YYSYMBOL_518_189 = 518,                  /* $@189  */
  YYSYMBOL_519_190 = 519,                  /* $@190  */
  YYSYMBOL_520_191 = 520                   /* $@191  */
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
#define YYLAST   2105

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  118
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  403
/* YYNRULES -- Number of rules.  */
#define YYNRULES  614
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  907

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   351


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
       2,     2,     2,     2,     2,     2,     2,   112,   107,     2,
     114,   115,   110,   108,   103,   109,     2,   111,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   102,    97,
     100,   104,   101,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   116,     2,   117,   106,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    98,   105,    99,   113,     2,     2,     2,
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
      95,    96
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   416,   416,   419,   420,   428,   443,   447,   448,   449,
     454,   453,   462,   461,   470,   469,   478,   477,   486,   485,
     494,   493,   502,   501,   510,   509,   518,   517,   526,   525,
     534,   533,   542,   541,   550,   549,   558,   557,   566,   565,
     579,   578,   590,   629,   633,   589,   649,   657,   671,   681,
     711,   715,   656,   740,   744,   745,   749,   750,   755,   760,
     754,   846,   851,   845,   922,   923,   928,   966,   970,   927,
     987,   986,   998,  1035,  1065,  1098,  1097,  1106,  1113,  1114,
    1115,  1116,  1120,  1125,  1130,  1177,  1181,  1129,  1210,  1253,
    1257,  1208,  1276,  1274,  1314,  1313,  1325,  1329,  1336,  1341,
    1348,  1373,  1401,  1467,  1486,  1490,  1494,  1495,  1507,  1506,
    1524,  1528,  1535,  1556,  1557,  1561,  1576,  1581,  1580,  1589,
    1588,  1597,  1596,  1605,  1604,  1613,  1612,  1621,  1620,  1629,
    1628,  1637,  1636,  1649,  1661,  1659,  1684,  1691,  1701,  1700,
    1726,  1724,  1749,  1759,  1770,  1814,  1841,  1873,  1877,  1881,
    1885,  1872,  1947,  1948,  1949,  1950,  1951,  1952,  1953,  1957,
    1961,  2029,  2031,  2033,  2034,  2046,  2047,  2059,  2060,  2072,
    2073,  2082,  2094,  2095,  2104,  2116,  2117,  2126,  2135,  2147,
    2148,  2157,  2166,  2178,  2235,  2236,  2243,  2247,  2252,  2259,
    2266,  2270,  2275,  2279,  2283,  2287,  2294,  2363,  2362,  2391,
    2392,  2396,  2397,  2398,  2400,  2399,  2408,  2409,  2413,  2469,
    2473,  2480,  2493,  2503,  2511,  2510,  2598,  2602,  2609,  2618,
    2625,  2633,  2639,  2646,  2659,  2658,  2667,  2671,  2675,  2679,
    2707,  2715,  2714,  2785,  2786,  2790,  2797,  2798,  2824,  2825,
    2826,  2827,  2828,  2829,  2830,  2831,  2835,  2836,  2837,  2838,
    2839,  2843,  2844,  2845,  2849,  2850,  2854,  2866,  2864,  2889,
    2896,  2897,  2901,  2913,  2911,  2936,  2943,  2959,  2977,  2978,
    2982,  2986,  2990,  2994,  2998,  3002,  3006,  3013,  3017,  3021,
    3025,  3029,  3033,  3037,  3044,  3048,  3052,  3059,  3066,  3070,
    3077,  3084,  3091,  3098,  3106,  3105,  3119,  3150,  3154,  3118,
    3171,  3174,  3175,  3179,  3197,  3201,  3196,  3259,  3258,  3271,
    3270,  3283,  3287,  3320,  3324,  3383,  3387,  3282,  3409,  3416,
    3429,  3438,  3445,  3446,  3555,  3558,  3559,  3564,  3568,  3563,
    3604,  3603,  3615,  3625,  3643,  3651,  3650,  3664,  3668,  3663,
    3684,  3683,  3733,  3758,  3782,  3786,  3817,  3821,  3781,  3845,
    3850,  3848,  3854,  3858,  3897,  3902,  3907,  3896,  3967,  4028,
    4041,  4045,  4039,  4129,  4196,  4205,  4195,  4219,  4229,  4233,
    4227,  4275,  4301,  4310,  4314,  4308,  4356,  4382,  4390,  4389,
    4432,  4442,  4460,  4468,  4472,  4467,  4532,  4533,  4538,  4542,
    4546,  4550,  4537,  4609,  4613,  4617,  4621,  4608,  4689,  4693,
    4725,  4729,  4688,  4746,  4750,  4811,  4815,  4745,  4852,  4857,
    4862,  4869,  4870,  4881,  4886,  4929,  4880,  4951,  4950,  4959,
    4958,  4969,  4974,  4972,  4978,  4983,  4987,  4982,  5026,  5025,
    5034,  5033,  5044,  5049,  5047,  5053,  5058,  5062,  5057,  5107,
    5114,  5115,  5116,  5223,  5227,  5231,  5239,  5243,  5238,  5252,
    5260,  5264,  5259,  5273,  5281,  5285,  5280,  5294,  5302,  5306,
    5301,  5315,  5322,  5334,  5332,  5355,  5362,  5392,  5431,  5432,
    5436,  5467,  5509,  5513,  5466,  5532,  5536,  5530,  5577,  5576,
    5584,  5591,  5606,  5607,  5612,  5611,  5621,  5620,  5630,  5629,
    5639,  5638,  5648,  5647,  5657,  5656,  5666,  5665,  5676,  5769,
    5775,  5800,  5907,  5916,  5920,  5927,  6002,  6074,  6150,  6149,
    6199,  6203,  6207,  6211,  6215,  6219,  6198,  6272,  6271,  6279,
    6286,  6291,  6299,  6303,  6298,  6313,  6314,  6318,  6320,  6319,
    6328,  6327,  6340,  6363,  6338,  6389,  6416,  6387,  6440,  6441,
    6442,  6446,  6447,  6451,  6480,  6512,  6556,  6560,  6510,  6577,
    6586,  6604,  6615,  6614,  6652,  6703,  6707,  6650,  6724,  6728,
    6735,  6739,  6743,  6747,  6751,  6755,  6759,  6763,  6767,  6771,
    6779,  6810,  6823,  6830,  6855,  6873,  6880,  6895,  6902,  6912,
    6916,  6935,  6943,  6911,  6958,  6973,  6977,  6978,  6982,  6983,
    6985,  6984,  6995,  7062,  7110,  7126,  7139,  7146,  7205,  7213,
    7217,  7212,  7278,  7282,  7277,  7295,  7296,  7301,  7300,  7309,
    7308,  7317,  7316,  7325,  7324
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
  "IDL_SEQUENCE", "IDL_MAP", "IDL_STRING", "IDL_WSTRING", "IDL_EXCEPTION",
  "IDL_CASE", "IDL_DEFAULT", "IDL_READONLY", "IDL_ATTRIBUTE", "IDL_ONEWAY",
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
  "IDL_ANNOTATION_SYMBOL", "';'", "'{'", "'}'", "'<'", "'>'", "':'", "','",
  "'='", "'|'", "'^'", "'&'", "'+'", "'-'", "'*'", "'/'", "'%'", "'~'",
  "'('", "')'", "'['", "']'", "$accept", "start", "definitions",
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
  "$@68", "at_least_one_member", "members", "member", "member_i", "$@69",
  "$@70", "$@71", "union_decl", "$@72", "union_type", "$@73", "$@74",
  "$@75", "$@76", "$@77", "$@78", "switch_type_spec",
  "at_least_one_case_branch", "case_branches", "case_branch", "$@79",
  "$@80", "$@81", "at_least_one_case_label", "case_labels", "case_label",
  "$@82", "$@83", "$@84", "element_spec", "$@85", "struct_forward_type",
  "union_forward_type", "enum_type", "$@86", "$@87", "$@88", "$@89",
  "at_least_one_enumerator", "enumerators", "$@90", "enumerator",
  "map_type_spec", "$@91", "$@92", "$@93", "map_type",
  "sequence_type_spec", "$@94", "$@95", "seq_head", "$@96", "$@97",
  "fixed_type_spec", "string_type_spec", "$@98", "$@99", "string_head",
  "wstring_type_spec", "$@100", "$@101", "wstring_head",
  "array_declarator", "$@102", "at_least_one_array_dim", "array_dims",
  "array_dim", "$@103", "$@104", "attribute", "attribute_readonly",
  "$@105", "$@106", "$@107", "$@108", "attribute_readwrite", "$@109",
  "$@110", "$@111", "$@112", "exception", "$@113", "@114", "$@115",
  "$@116", "operation", "$@117", "$@118", "$@119", "$@120",
  "opt_op_attribute", "op_type_spec", "init_decl", "$@121", "@122",
  "$@123", "init_parameter_list", "$@124", "$@125",
  "at_least_one_in_parameter", "in_parameters", "$@126", "in_parameter",
  "$@127", "$@128", "parameter_list", "$@129", "$@130",
  "at_least_one_parameter", "parameters", "$@131", "parameter", "$@132",
  "$@133", "param_type_spec", "direction", "opt_raises", "$@134", "$@135",
  "opt_getraises", "$@136", "$@137", "opt_setraises", "$@138", "$@139",
  "opt_context", "$@140", "$@141", "at_least_one_string_literal",
  "string_literals", "$@142", "typeid_dcl", "typeprefix_dcl", "component",
  "component_forward_decl", "component_decl", "@143", "$@144", "$@145",
  "component_header", "$@146", "$@147", "component_inheritance_spec",
  "$@148", "component_exports", "component_export", "$@149", "$@150",
  "$@151", "$@152", "$@153", "$@154", "$@155", "provides_decl",
  "interface_type", "uses_decl", "uses_opt_multiple", "opt_multiple",
  "emits_decl", "publishes_decl", "consumes_decl", "home_decl", "$@156",
  "home_header", "$@157", "$@158", "$@159", "$@160", "$@161", "$@162",
  "home_inheritance_spec", "$@163", "primary_key_spec", "home_body",
  "$@164", "$@165", "home_exports", "home_export", "$@166", "$@167",
  "factory_decl", "$@168", "$@169", "finder_decl", "$@170", "$@171",
  "event", "event_forward_decl", "event_concrete_forward_decl",
  "event_abs_forward_decl", "event_abs_decl", "$@172", "$@173", "$@174",
  "event_abs_header", "event_custom_header", "event_plain_header",
  "event_rest_of_header", "$@175", "event_decl", "@176", "$@177", "$@178",
  "event_header", "formal_parameter_type", "at_least_one_formal_parameter",
  "formal_parameters", "formal_parameter",
  "at_least_one_formal_parameter_name", "formal_parameter_names",
  "formal_parameter_name", "porttype_decl", "$@179", "@180", "$@181",
  "$@182", "at_least_one_port_export", "port_exports", "port_export",
  "$@183", "extended_port_decl", "at_least_one_actual_parameter",
  "actual_parameters", "actual_parameter", "connector_decl",
  "connector_header", "$@184", "$@185", "connector_body", "$@186", "$@187",
  "connector_exports", "connector_export", "$@188", "$@189", "$@190",
  "$@191", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-687)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-583)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -687,   100,  1332,  -687,  -687,  -687,  -687,  -687,  -687,  -687,
    -687,  -687,  -687,  -687,    57,   118,    64,    76,  -687,    57,
      57,  -687,    52,    52,  -687,  -687,    57,  -687,  -687,    33,
    -687,   201,    84,    92,  -687,  -687,    -1,  -687,  -687,  -687,
    -687,  -687,  -687,   663,  -687,  -687,  -687,  -687,  -687,  1534,
      55,  -687,  -687,    96,  -687,   133,  -687,  -687,  -687,  -687,
    -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,
    -687,  -687,  -687,  -687,   121,  -687,  -687,  -687,   121,  -687,
    -687,   122,   150,  2015,    52,    57,  1892,    57,    57,    57,
      57,  -687,  -687,  -687,     0,    57,     7,  -687,    90,    57,
    -687,   121,    57,   152,   155,    57,  -687,  -687,    37,  -687,
      87,   253,  -687,   172,  -687,   186,   185,  1648,  -687,  -687,
    -687,   188,   238,  -687,   190,   192,   193,    97,  -687,    98,
    -687,  -687,  -687,  -687,  -687,  -687,   194,  -687,  -687,  -687,
     197,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,
    -687,  -687,   200,  -687,  -687,  -687,  -687,  -687,  -687,  -687,
    -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,
     133,  -687,  -687,  -687,  -687,    69,  -687,  -687,   198,  -687,
     199,   196,   205,  -687,    52,   207,   208,   206,  -687,   209,
     210,   211,   212,   213,   215,   216,   219,  -687,  -687,  -687,
     221,   223,  -687,  -687,  -687,  -687,   200,  -687,  -687,  -687,
    -687,  -687,  -687,  -687,  -687,  -687,   200,  -687,  -687,  -687,
    -687,  -687,  -687,  -687,  -687,   224,  -687,   227,  -687,  -687,
     229,  -687,   310,  -687,  -687,  -687,  -687,    49,  -687,  -687,
    -687,  2015,  -687,  -687,  -687,  -687,   234,  -687,  -687,  -687,
    -687,   320,  -687,  -687,    60,   240,  -687,  -687,  -687,  -687,
    -687,  -687,  -687,  -687,   337,  -687,   187,   246,  -687,   248,
     301,  -687,  -687,  -687,  -687,  -687,  -687,   200,  -687,  -687,
     236,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,
     301,   254,   255,  -687,  -687,  -687,    57,    57,   256,   259,
    -687,  -687,  -687,   249,  -687,   310,   261,  -687,  -687,  -687,
    -687,  -687,   352,  -687,   260,   263,  -687,  -687,  -687,  -687,
    -687,  -687,  -687,  -687,  -687,  -687,   419,   419,   419,   187,
     200,  -687,  -687,   262,   258,   264,   111,   109,    99,  -687,
    -687,  -687,  -687,  -687,    52,  -687,  -687,  -687,  -687,   265,
    -687,  1624,   266,  -687,    52,  -687,   187,   187,   187,   267,
    -687,  -687,  -687,  -687,  -687,  -687,  -687,   169,  -687,   -13,
    -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,    52,   301,
    -687,  -687,  -687,  -687,   229,   711,  1447,   271,   273,  -687,
    1648,  -687,  -687,  -687,   247,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   269,    57,  -687,   200,  1032,
    -687,   844,   187,  -687,  -687,  -687,  -687,  -687,  -687,  -687,
    -687,   187,  -687,  1397,  -687,  -687,  -687,    59,   566,  -687,
    -687,  -687,  -687,    54,   316,    52,    52,  -687,  -687,  -687,
    -687,  -687,    54,  -687,   277,  -687,   275,  -687,   281,  -687,
    -687,  1126,   200,  -687,    52,   301,  -687,  -687,  -687,  -687,
     283,  -687,  -687,    57,  -687,  -687,   290,   289,   386,   292,
    -687,  -687,   258,   264,   111,   109,   109,    99,    99,  -687,
    -687,  -687,  -687,  -687,   294,  -687,  -687,  -687,   296,  -687,
    -687,  1804,  -687,  -687,  -687,  -687,  1927,  -687,  -687,  -687,
    -687,  -687,   299,  -687,  1839,  -687,  -687,  1714,  -687,   304,
    1624,   297,   307,   306,   311,   314,   312,  -687,   302,  -687,
     315,  -687,  -687,  -687,   326,   328,   996,    52,    52,    52,
     184,  -687,   330,  -687,  -687,  -687,  -687,  -687,  -687,  -687,
      57,    57,  -687,   331,  -687,  -687,  -687,  1220,   938,   387,
    1980,  -687,   200,   310,  -687,  -687,    53,    62,   334,   336,
     338,   310,   340,  -687,  -687,     3,  -687,    50,  -687,  -687,
     335,   339,   200,  -687,   346,   117,  1892,  -687,   409,  -687,
    -687,  -687,  -687,    60,  -687,   349,  -687,   350,  -687,   351,
     353,   354,   355,  -687,   200,  -687,  -687,  -687,  -687,  -687,
     357,   358,   446,  -687,  -687,  -687,   359,  -687,  -687,   187,
    -687,  -687,  -687,  -687,   187,  -687,   310,  -687,   360,    57,
    -687,  -687,   450,   200,  -687,  -687,  -687,  -687,  -687,  -687,
      71,    71,    71,  -687,   362,  -687,   364,   365,   368,   371,
     373,   380,  -687,  -687,  -687,   381,   382,   384,   385,  -687,
    -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,   187,
    -687,  -687,  -687,    57,  -687,   388,   383,   390,  -687,   423,
     392,   117,  -687,   395,   404,  -687,   405,   187,   406,  1509,
    -687,    52,  -687,  -687,  -687,  -687,  -687,  -687,   505,  -687,
    -687,  -687,  -687,   410,  -687,  -687,   312,   315,  -687,  -687,
     399,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,
    -687,   396,   396,  -687,  -687,  -687,  -687,  1980,    57,  -687,
     187,   400,  -687,  -687,  -687,  -687,  -687,  -687,  -687,   418,
    -687,  -687,  -687,  -687,  -687,    52,  -687,  -687,  -687,  -687,
     422,   200,  -687,   396,  -687,   420,  -687,   425,  -687,   484,
    -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,    52,  -687,
     200,   424,  1270,  -687,   411,  -687,  -687,  -687,   426,   412,
     490,   493,   493,    57,   473,   431,   428,  -687,   200,   437,
    -687,  -687,   427,  -687,   493,  -687,  -687,  -687,   432,  -687,
    -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,   480,   538,
     434,   176,   493,  -687,   166,  1980,  -687,   440,   436,   493,
     438,   483,    57,    52,  -687,  -687,   452,  -687,  -687,  -687,
    -687,  -687,   441,  -687,  -687,  -687,  -687,  -687,  -687,  -687,
    -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,
    -687,   200,  -687,   448,  -687,   454,  1980,   515,   461,   187,
     457,   463,    58,  -687,   202,    57,   490,    52,    52,   449,
      57,   538,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,
    -687,  1599,  -687,  -687,  -687,   445,   451,  -687,  -687,  -687,
     176,    57,   458,   462,  -687,  -687,  -687,  -687,    52,  -687,
    -687,  -687,  -687,    57,   468,   453,   495,  -687,  -687,  -687,
    -687,   475,   488,  -687,  -687,   519,  -687
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       4,     0,     0,     3,     1,    38,   147,    40,    70,   224,
     294,   309,   344,   398,     0,     0,     0,     0,    94,     0,
       0,   510,     0,     0,   579,   599,     0,     6,     7,    42,
      24,    61,     0,     0,    22,    64,    77,    66,    26,    78,
      83,    79,    84,    77,    80,    81,    65,    18,    10,     0,
       0,    12,   230,   296,   226,   343,   227,   254,   255,   228,
      20,    14,    16,    28,   469,   468,   471,    30,   508,    32,
     540,   542,   541,   539,    77,   558,   559,   538,    77,    34,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   143,   266,   229,    77,     0,    77,    88,    77,     0,
      82,    77,     0,   475,   551,     0,   142,   138,     0,   137,
       0,     0,   213,     0,    46,     0,     0,     0,   213,     8,
       9,     0,    97,    72,     0,     0,     0,   270,   272,     0,
     284,   285,   288,   289,   290,   291,   287,   292,   293,   364,
       0,   372,   377,   273,   280,   274,   281,   275,   282,   276,
     283,    92,   237,   102,   233,   235,   236,   234,   238,   268,
     269,   239,   243,   240,   242,   241,   244,   245,   296,   251,
       0,   252,   253,   250,   246,     0,   249,   247,   371,   248,
     376,     0,     0,     5,     0,   211,     0,     0,   311,     0,
       0,     0,     0,     0,     0,     0,     0,   552,   545,   554,
       0,     0,   602,   598,    39,   287,   160,   148,   152,   156,
     157,   153,   154,   155,   158,   159,    41,    71,   225,   231,
     295,   310,   345,   399,    73,   549,    74,     0,   550,    95,
     480,   511,     0,   466,   140,   467,   580,     0,   197,    43,
      25,     0,   565,   561,   562,   567,   564,   568,   566,   563,
     560,     0,    48,   572,     0,     0,    23,    96,    75,    67,
      27,    85,   271,   286,   277,   279,     0,     0,   213,     0,
      99,   363,   360,   368,   373,    19,    11,   214,    13,   297,
       0,    21,    15,    17,    29,   472,    31,   522,   509,    33,
      99,     0,     0,    35,    37,   606,     0,     0,     0,     0,
      89,   478,   476,   519,   139,     0,     0,   600,   212,   200,
       4,   569,     0,   573,     0,   570,   186,   187,   188,   190,
     193,   192,   194,   195,   191,   189,     0,     0,     0,     0,
     183,   597,   161,   162,   163,   165,   167,   169,   172,   175,
     179,   184,   596,    62,     0,   114,   105,   278,   196,     0,
     365,     0,     0,   355,     0,    93,     0,     0,     0,   217,
     213,   312,   483,   526,   553,   546,   555,   603,   149,   266,
     232,   259,   260,   261,   267,   346,   400,   114,     0,    99,
     517,   512,   141,   581,   480,     0,     0,     3,     0,    49,
       0,   180,   181,   182,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   594,     0,    76,   136,     0,
     113,     0,     0,   213,   359,   213,   213,    98,   361,   369,
     374,     0,   215,     0,   298,   302,   213,   213,     0,   114,
     105,   388,   393,     0,   504,     0,     0,   611,   386,   387,
     607,   609,     0,   613,     0,   605,     0,   213,   256,   213,
     302,     0,   479,   477,     0,    99,   587,   601,   204,   198,
       0,   206,   199,     0,   201,   207,     0,     0,     0,     0,
     571,   185,   164,   166,   168,   170,   171,   173,   174,   176,
     177,   178,   213,    63,   133,   131,   408,   409,     0,   116,
     123,     0,   117,   127,   125,   129,     0,   119,   121,   413,
     111,   110,     0,   104,     0,   106,   107,     0,   108,     0,
       0,     0,     0,     0,     0,     0,   137,   218,     0,   219,
     222,   307,   304,   303,     0,   213,     0,     0,     0,     0,
       0,   494,     0,   482,   484,   486,   488,   490,   492,   496,
       0,     0,   527,     0,   525,   528,   530,     0,     0,     0,
       0,   500,   499,     0,   503,   502,     0,     0,     0,     0,
       0,     0,     0,   604,   150,     0,   257,     0,   347,   352,
     213,     0,   518,   513,   586,   213,     0,   202,   210,   203,
      45,   574,    50,     0,   134,     0,    69,     0,   115,     0,
       0,     0,     0,   412,   442,   439,   440,   441,   403,   411,
       0,     0,     0,    87,   112,   103,     0,   367,   366,     0,
     356,   362,   370,   375,     0,   216,     0,   220,     0,     0,
     299,   301,   270,   323,   318,   319,   320,   321,   313,   322,
       0,     0,     0,   481,     0,   474,     0,     0,     0,     0,
       0,     0,   532,   535,   524,     0,     0,     0,     0,   389,
     394,   498,   592,   593,   612,   608,   610,   501,   614,     0,
     383,   379,   382,     0,   353,     0,   349,     0,    91,     0,
       0,     0,   590,     0,     0,   585,     0,     0,     0,     0,
     595,     0,   132,   124,   118,   128,   126,   130,     0,   120,
     122,   414,   109,     0,   213,   223,     0,   222,   308,   305,
       0,   507,   505,   506,   495,   485,   487,   489,   491,   493,
     497,     0,     0,   529,   531,   548,   557,     0,     0,   151,
       0,   380,   258,   348,   350,   402,   514,   583,   584,     0,
     588,   589,   205,   209,   208,     0,    56,    42,    51,    55,
       0,   135,   404,     0,   358,     0,   221,     0,   314,   417,
     533,   536,   390,   395,   265,   384,   381,   213,     0,   591,
      58,     0,     0,    57,     0,   415,   357,   306,     0,     0,
       0,   449,   449,     0,   453,   262,     0,   351,   515,     0,
      52,    54,   430,   405,   449,   315,   418,   425,     0,   424,
     446,   534,   537,   391,   450,   396,   263,   385,   521,     0,
       0,     0,   449,   416,     0,     0,   420,   421,     0,   449,
       0,   457,     0,     0,   516,   578,     0,   577,   429,   443,
     444,   445,     0,   435,   436,   406,   330,   337,   335,   316,
     326,   327,   334,   426,   422,   447,   392,   451,   454,   397,
     264,   520,    59,   575,   431,   432,     0,   461,     0,     0,
       0,     0,     0,   213,   332,     0,     0,     0,     0,     0,
       0,     0,   433,   437,   458,   407,   331,   338,   336,   317,
     325,     0,   333,   427,   423,     0,     0,   455,    60,   576,
       0,     0,     0,     0,   340,   328,   448,   452,     0,   434,
     438,   459,   339,     0,     0,     0,     0,   341,   329,   456,
     465,     0,   462,   460,   463,     0,   464
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -687,  -687,   286,   293,   553,  -603,  -687,  -687,  -687,  -687,
    -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,
    -687,  -597,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,
    -687,  -687,  -687,  -687,  -687,  -687,  -158,  -687,  -687,  -687,
    -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,   225,  -687,
    -687,    86,  -687,  -687,  -687,   589,  -687,  -687,  -687,  -687,
    -687,  -687,  -687,   592,  -687,   257,  -687,  -687,  -247,  -687,
    -687,   179,   103,  -687,  -687,  -687,  -319,  -687,  -360,  -687,
    -687,  -687,  -687,  -687,  -687,  -687,  -687,  -340,  -687,  -687,
     -22,  -687,  -687,  -188,   -10,  -687,    17,  -687,  -687,  -687,
    -687,  -191,   -47,  -236,  -687,   220,   218,   222,  -123,  -122,
    -176,   -69,  -687,  -326,  -687,  -687,  -687,  -687,  -687,  -687,
    -687,  -687,    13,   -89,   567,  -687,  -687,  -687,  -687,   -81,
       2,    18,  -687,    44,  -687,   -31,  -304,  -469,  -687,  -687,
    -687,     4,  -687,  -687,  -621,  -151,  -687,  -687,    -7,  -687,
     -66,  -687,  -687,   -43,   -42,   -57,   -55,   -50,   239,  -687,
     -40,  -687,   -38,  -687,  -687,  -687,  -687,   175,   268,   123,
    -687,  -687,  -687,   -37,  -687,   -32,  -687,  -687,  -687,  -687,
    -687,  -687,  -687,  -687,  -687,  -226,  -687,  -687,  -687,  -687,
    -687,  -225,  -687,  -687,  -687,  -687,  -687,  -687,  -687,   -41,
    -687,  -687,  -687,  -687,  -687,  -687,  -687,  -125,  -687,  -687,
    -687,  -687,  -377,  -687,  -687,  -687,  -687,  -687,  -687,  -687,
     -75,  -687,  -687,  -687,   -70,  -687,  -687,  -687,  -687,  -687,
    -687,  -687,   -88,  -687,  -687,  -333,  -687,  -687,  -687,  -687,
    -687,  -687,  -687,  -687,  -687,  -687,    20,  -687,  -687,  -687,
    -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,
    -687,  -687,  -628,  -687,  -687,  -687,  -687,  -687,  -222,  -687,
    -687,  -687,  -687,  -687,  -687,  -687,  -687,  -245,  -687,  -687,
    -513,  -687,  -686,  -687,  -687,  -687,  -687,  -687,  -687,  -687,
    -687,  -687,  -687,  -687,  -687,  -687,  -687,    22,    23,  -687,
    -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,   252,  -687,
    -687,   107,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -332,
     203,  -331,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,
    -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,
    -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,
    -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,
    -687,  -687,  -687,   560,  -687,  -687,  -687,  -687,  -687,  -687,
    -687,  -687,  -687,   250,  -687,  -687,  -220,  -687,  -687,  -687,
    -687,  -687,  -687,  -687,   -28,  -687,   272,  -687,  -687,    61,
    -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,  -687,
    -687,  -687,  -687
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,     3,    27,    28,   182,   186,   190,   191,
     181,   189,   121,   116,   125,   192,   194,   196,   200,   201,
      82,    29,    84,    30,   115,   310,   467,    31,    32,   117,
     314,   469,   679,   761,   738,   762,   739,   740,   779,   860,
      33,   118,   406,    34,    35,   124,   345,   488,    36,    85,
      37,   151,   344,    38,    39,    40,   126,   346,   502,    41,
     227,   377,   571,    42,   270,    43,   102,   258,   355,    44,
      45,   411,   503,   606,   504,   505,   409,   410,   489,   589,
     600,   601,   587,   591,   590,   592,   585,   407,   484,   681,
     330,   232,   305,   109,   369,    46,   490,    83,   296,   446,
     659,   207,   331,   348,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   349,    48,   309,   385,   462,   576,   463,
     464,   678,   491,    50,   308,   359,   422,   518,   519,   617,
     520,   492,    86,   218,   297,   219,   154,   155,   156,   157,
      52,   370,   448,   663,   371,   753,   775,   812,   372,   373,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
      53,    87,    54,   187,   360,   524,   424,   525,   621,   523,
     619,   747,   618,    55,    88,    56,   280,   426,   700,   768,
     804,   851,   628,   829,   852,   830,   853,   894,   848,   831,
     854,   832,   850,   849,   883,   885,   893,    57,    58,    59,
      89,   298,   449,   665,   568,   666,   757,   569,   173,   269,
     416,   694,   352,   174,   356,   513,   175,   267,   413,   176,
     177,   357,   514,   178,   179,   358,   515,   180,   374,   447,
     661,   721,   662,   720,   776,   493,   438,   549,   717,   773,
     809,   439,   550,   718,   774,   811,   494,    90,   299,   450,
     667,   495,   688,   764,   802,   847,   496,   598,   508,   602,
     743,   784,   750,   769,   770,   788,   807,   856,   789,   805,
     855,   783,   800,   801,   822,   845,   880,   823,   846,   881,
     599,   824,   791,   808,   857,   795,   810,   858,   839,   859,
     888,   865,   882,   896,   901,   902,   905,   497,   498,    63,
      64,    65,   193,   362,   532,    66,   230,   379,   302,   378,
     427,   533,   636,   637,   638,   639,   640,   634,   641,   534,
     553,   535,   442,   555,   536,   537,   538,    67,   195,    68,
     105,   303,   455,   669,   758,   798,   381,   454,   814,   288,
     363,   543,   428,   544,   645,   646,   545,   711,   771,   546,
     712,   772,    69,    70,    71,    72,    73,   291,   429,   647,
      74,    75,    76,   198,   290,    77,   292,   430,   648,    78,
     251,   252,   315,   253,   816,   843,   817,    79,   111,   306,
     456,   670,   574,   575,   675,   729,   539,   255,   405,   342,
      80,    81,   112,   384,   203,   295,   444,   367,   445,   559,
     560,   558,   562
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     108,   110,   172,   168,    92,   169,   170,    93,   214,   103,
     104,   171,   153,   215,   417,    49,   113,   208,   332,    47,
      51,   152,    60,   237,    61,    62,   211,   595,   212,   254,
     418,   419,   420,   213,   437,   440,   441,   650,   511,   512,
     209,   210,   722,   364,   304,   172,   168,   414,   169,   170,
     311,   506,   307,   664,   171,   106,   652,   106,   451,   826,
      91,   206,   216,   106,   152,   653,    47,    51,   542,    60,
       8,    61,    62,   551,   106,   217,   736,   220,   221,   222,
     223,   595,   737,  -378,   751,   225,   792,   827,   828,   228,
     431,   432,   229,   394,   531,   231,  -144,  -145,   803,   184,
       4,   122,   122,  -378,  -146,   262,   264,   265,   263,   122,
     547,   527,   528,   233,    18,   765,   825,   382,    95,   660,
     433,   529,   123,   836,     8,   434,    18,   234,   435,   436,
      99,   588,   453,   114,   316,   317,   318,   319,   320,   321,
     322,   323,   107,   234,   107,   184,   184,   506,   431,   432,
     107,   184,   234,   324,   325,   188,   184,  -324,  -473,   736,
     197,   234,   277,   235,   197,   737,   214,   826,   326,   327,
     271,   215,   272,   328,   329,   208,   509,   234,   433,   351,
     224,   119,   226,   434,   211,   517,   212,  -100,   506,   120,
     106,   213,   122,  -342,   461,   827,   828,   531,   209,   210,
     431,   432,   398,   399,   752,   -47,   608,   -47,   573,   402,
     403,   404,   819,   820,   821,   431,   432,   400,   401,   206,
     202,   -47,   -47,   122,   -47,   -47,   479,   480,   481,   -47,
     433,   827,   828,   516,   873,   434,   527,   528,   435,   436,
      94,    96,   672,   673,   674,   433,   529,   204,   595,  -470,
     434,   -47,  -543,   435,   436,   -47,   236,   391,   392,   393,
     890,   316,   317,   318,   319,   320,   321,   322,   323,   -47,
     238,   423,   897,    98,   101,   475,   476,   107,   477,   478,
     324,   325,   240,   693,   239,   256,   368,   257,   259,   260,
     234,   261,   833,   275,   266,   326,   327,   268,   273,   274,
     328,   329,   276,  -212,   279,   278,   281,   282,   283,   284,
     214,   285,   286,   106,   287,   215,   289,   745,   293,   208,
     294,  -544,   408,   313,   510,   300,   351,   351,   211,   152,
     212,   301,   408,   863,   312,   213,   595,   526,   672,   673,
     674,   343,   209,   210,   466,   347,   350,   332,   353,   354,
     361,   380,   365,   366,   375,   388,   452,   376,   565,   383,
     567,   389,   471,   206,   396,   651,   390,   395,   412,   415,
     -44,   397,   482,   657,   468,   554,   563,   595,   695,   564,
     577,   421,   172,   168,   566,   169,   170,   579,   580,   581,
     582,   171,   522,   583,   755,   586,   483,   584,   603,    49,
     609,   152,   460,    47,    51,   607,    60,   611,    61,    62,
     610,   552,   612,   556,   557,   613,   614,   615,   616,   649,
     552,   596,   106,   332,   507,   620,   597,  -300,   696,   635,
     644,   654,   572,   655,  -401,   656,   423,   658,   668,   677,
     530,   733,   701,   702,   703,  -582,   682,   683,   684,   691,
     685,   686,   687,   578,   689,   690,   692,   698,   262,   704,
     624,   705,   706,   172,   168,   707,   169,   170,   708,   625,
     709,   626,   171,   522,   594,   596,   627,   710,   713,   714,
     597,   423,   152,   715,   716,   629,   724,   723,   152,   725,
     726,   727,   730,   316,   317,   318,   319,   320,   321,   322,
     323,   731,   732,   734,   623,   630,   631,   632,   742,   107,
     749,   744,   324,   325,   748,   759,   660,   875,   876,   763,
    -419,   766,   767,   780,   785,   782,   787,   786,   594,   794,
     642,   643,   790,   329,   796,   172,   168,   799,   169,   170,
     813,   815,  -428,   834,   171,   797,   838,   806,   895,   818,
     835,   861,   837,   842,   152,   864,   844,   862,   866,   868,
     886,   507,   869,   877,   892,   898,   887,   485,   899,  -410,
       6,   900,   891,     9,  -410,  -410,  -410,  -410,  -410,  -410,
    -410,  -410,  -410,  -410,  -410,  -410,    10,    11,   671,    12,
     903,   904,  -410,  -410,    13,   906,   386,   431,   432,   486,
     487,  -410,   183,   387,   781,   351,   100,    14,    97,   548,
     605,   540,   719,   867,   473,   472,   746,   185,   697,   474,
     676,   541,   793,   699,   465,   570,   870,   604,   425,   872,
      22,    23,   777,   756,   874,   889,   457,   633,   199,   443,
     470,   879,   596,   728,   680,   561,     0,   597,  -410,  -410,
    -410,  -410,  -410,  -410,  -410,  -410,  -410,     0,     0,   741,
       0,     0,     0,     0,     0,  -523,   106,     0,   567,     0,
       0,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    10,    11,     0,    12,   139,   140,   141,
     142,     0,     0,     0,     0,   594,    47,    51,     0,    60,
       0,    61,    62,     0,     0,     0,     0,     0,    92,     0,
       0,   754,     0,   760,   106,     6,     0,     0,   458,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   205,   137,
     596,     0,     0,     0,    12,   597,   778,   141,   142,     0,
       0,     0,     0,     0,     0,   143,   144,   145,   146,   147,
     148,   149,   150,   107,     0,     0,     0,     0,     0,     0,
    -101,     0,     0,    92,   871,   122,   754,     0,     0,     0,
       0,   596,     0,     0,     0,     0,   597,     0,     0,    47,
      51,     0,    60,   594,    61,    62,     0,     0,     0,     0,
       0,   841,     0,   143,   144,   145,   146,   147,   148,   149,
     150,   107,    92,     0,     0,   840,     0,     0,     0,     0,
     459,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   594,     0,     0,     0,     0,     0,
     172,   168,     0,   169,   170,   408,   408,     0,     0,   171,
     884,     0,     0,     0,     0,   485,     0,  -410,     6,   152,
     878,     9,  -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,
    -410,  -410,  -410,  -410,    10,    11,   408,    12,     0,     0,
    -410,  -410,    13,     0,     0,   431,   432,   486,   487,  -410,
       0,     0,     0,     0,     0,    14,     0,     0,     0,   499,
     500,   501,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,    23,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -410,  -410,  -410,  -410,
    -410,  -410,  -410,  -410,  -410,     0,     0,     0,     0,   485,
    -213,  -410,     6,   -86,     0,     9,  -410,  -410,  -410,  -410,
    -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,    10,    11,
       0,    12,     0,     0,  -410,  -410,    13,     0,     0,   431,
     432,   486,   487,  -410,     0,     0,     0,     0,     0,    14,
       0,     0,     0,   499,   500,   501,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   106,
       0,     0,    22,    23,   622,   128,   129,     0,     0,   132,
     133,   134,   135,     0,     0,     0,     0,     0,     0,    12,
    -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,     0,
       0,     0,     0,   485,  -213,  -410,     6,  -556,     0,     9,
    -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,
    -410,  -410,    10,    11,     0,    12,     0,     0,  -410,  -410,
      13,     0,     0,   431,   432,   486,   487,  -410,     0,     0,
       0,     0,     0,    14,     0,     0,     0,     0,   143,   144,
     145,   146,   147,   148,   149,   150,   107,     0,     0,     0,
       0,     0,   184,     0,     0,     0,    22,    23,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -410,  -410,  -410,  -410,  -410,  -410,
    -410,  -410,  -410,     0,     0,     0,     0,   485,  -213,  -410,
       6,   -68,     0,     9,  -410,  -410,  -410,  -410,  -410,  -410,
    -410,  -410,  -410,  -410,  -410,  -410,    10,    11,     0,    12,
       0,     0,  -410,  -410,    13,     0,     0,   431,   432,   486,
     487,  -410,     0,     0,     0,     0,     0,    14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,    23,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -410,  -410,
    -410,  -410,  -410,  -410,  -410,  -410,  -410,     0,     0,     0,
       0,   485,  -213,  -410,     6,   -90,     0,     9,  -410,  -410,
    -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,
      10,    11,     0,    12,     0,     0,  -410,  -410,    13,     0,
       0,   431,   432,   486,   487,  -410,     0,     0,     0,     0,
       0,    14,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     5,     0,     0,     6,     7,     8,     9,     0,     0,
       0,     0,     0,     0,    22,    23,     0,     0,     0,     0,
      10,    11,     0,    12,     0,     0,     0,     0,    13,     0,
       0,     0,  -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,
    -410,    14,    15,    16,    17,     0,  -213,     0,     0,  -547,
      18,    19,     0,     0,    20,     0,     0,    21,     0,     0,
       0,     0,    -2,     5,    22,    23,     6,     7,     8,     9,
       0,    24,    25,   735,     0,     0,     0,     0,     0,     0,
       0,     0,    10,    11,     0,    12,     0,     0,     0,     0,
      13,     0,     0,     0,     0,    26,     0,     0,     0,   -53,
       0,     0,     0,    14,    15,    16,    17,     0,     0,     0,
       0,     0,    18,    19,     0,     0,    20,     0,     0,    21,
       0,     0,     0,     0,     0,     0,    22,    23,   521,     0,
     106,     0,     0,    24,    25,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    10,    11,     0,
      12,   139,   140,   141,   142,     0,     0,    26,  -213,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     0,
       0,     6,     7,     8,     9,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    10,    11,     0,
      12,     0,     0,     0,     0,    13,     0,     0,     0,   143,
     144,   145,   146,   147,   148,   149,   150,   107,    14,    15,
      16,    17,     0,   184,     0,     0,     0,    18,    19,     0,
       0,    20,     0,     0,    21,     0,     0,     0,     0,     0,
       5,    22,    23,     6,     7,     8,     9,     0,    24,    25,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    10,
      11,     0,    12,     0,     0,     5,     0,    13,     6,     7,
       8,     9,    26,  -213,     0,     0,     0,     0,     0,     0,
      14,    15,    16,    17,    10,    11,     0,    12,     0,    18,
      19,     0,    13,    20,     0,     0,    21,     0,     0,     0,
       0,     0,     0,    22,    23,    14,    15,    16,    17,     0,
      24,    25,   735,     0,    18,    19,     0,     0,    20,     0,
       0,    21,     0,     0,     0,     0,     0,     0,    22,    23,
       0,     0,   106,     0,    26,    24,    25,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,    10,
      11,     0,    12,   139,   140,   141,   142,   106,     0,    26,
       0,     0,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,     0,     0,     0,     0,   139,   140,
     141,   142,   241,     0,   242,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   243,   244,
       0,   245,   246,     0,     0,     0,   247,     0,     0,     0,
       0,   143,   144,   145,   146,   147,   148,   149,   150,   107,
       0,     0,     0,     0,     0,   184,     0,     0,   248,     0,
       0,     0,   249,     0,     0,     0,   143,   144,   145,   146,
     147,   148,   149,   150,   107,   485,   250,  -410,     6,     0,
     184,     9,  -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,
    -410,  -410,  -410,  -410,    10,    11,     0,    12,     0,     0,
    -410,  -410,    13,     0,     0,   431,   432,   486,   487,  -410,
       0,     0,     0,     0,     0,    14,     0,     0,     0,   499,
     500,   501,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,    23,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -410,  -410,  -410,  -410,
    -410,  -410,  -410,  -410,  -410,   485,     0,  -410,     6,     0,
       0,     9,  -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,
    -410,  -410,  -410,  -410,    10,    11,     0,    12,     0,     0,
    -410,  -410,    13,     0,     0,   431,   432,   486,   487,  -410,
     521,     0,   106,     0,     0,    14,     0,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,    10,
      11,     0,    12,   139,   140,   141,   142,     0,    22,    23,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -410,  -410,  -410,  -410,
    -410,  -410,  -410,  -410,  -410,   106,     0,     0,     0,     0,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,    10,    11,     0,    12,   139,   140,   141,   142,
       0,   143,   144,   145,   146,   147,   148,   149,   150,   107,
     106,     0,     0,     0,     0,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   205,   137,   138,     0,     0,     0,
       0,     0,     0,   141,   142,     0,     0,     0,     0,     0,
       0,     0,   593,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   143,   144,   145,   146,   147,   148,
     149,   150,   107,   106,     0,     0,     0,     0,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   205,   137,   138,
       0,     0,     0,     0,     0,     0,   141,   142,     0,   143,
     144,   145,   146,   147,   148,   149,   150,   107,   106,     0,
       0,     0,     0,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   205,     0,     0,     0,     0,     0,     0,     0,
       0,   141,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   143,   144,   145,   146,   147,   148,   149,   150,
     107,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   143,   144,   145,
     146,   147,   148,   149,   150,   107
};

static const yytype_int16 yycheck[] =
{
      22,    23,    43,    43,    14,    43,    43,    14,    83,    19,
      20,    43,    43,    83,   354,     2,    26,    83,   254,     2,
       2,    43,     2,   112,     2,     2,    83,   496,    83,   118,
     356,   357,   358,    83,   367,   367,   367,   550,   415,   416,
      83,    83,   663,   290,   232,    86,    86,   351,    86,    86,
     241,   411,     3,     3,    86,     3,     3,     3,   377,     1,
       3,    83,    84,     3,    86,     3,    49,    49,   428,    49,
       6,    49,    49,    19,     3,    85,   679,    87,    88,    89,
      90,   550,   679,    96,   712,    95,   772,    29,    30,    99,
      31,    32,   102,   329,   427,   105,    97,    97,   784,    96,
       0,   102,   102,   116,    97,     8,     8,     9,    11,   102,
     429,    52,    53,    76,    50,   743,   802,   305,    54,   116,
      61,    62,    36,   809,     6,    66,    50,    90,    69,    70,
      54,   491,   379,   100,    74,    75,    76,    77,    78,    79,
      80,    81,    90,    90,    90,    96,    96,   507,    31,    32,
      90,    96,    90,    93,    94,    22,    96,    99,    99,   762,
      74,    90,   184,    76,    78,   762,   241,     1,   108,   109,
     101,   241,   103,   113,   114,   241,   412,    90,    61,   268,
      94,    97,    96,    66,   241,   421,   241,    97,   548,    97,
       3,   241,   102,    97,   385,    29,    30,   530,   241,   241,
      31,    32,    91,    92,   717,     4,   510,     6,   455,   110,
     111,   112,    36,    37,    38,    31,    32,   108,   109,   241,
      98,    20,    21,   102,    23,    24,   402,   403,   404,    28,
      61,    29,    30,   421,   855,    66,    52,    53,    69,    70,
      15,    16,   575,   575,   575,    61,    62,    97,   717,    97,
      66,    50,    97,    69,    70,    54,     3,   326,   327,   328,
     881,    74,    75,    76,    77,    78,    79,    80,    81,    68,
      98,   360,   893,    16,    17,   398,   399,    90,   400,   401,
      93,    94,    97,   609,    98,    97,   296,    49,    98,    97,
      90,    98,   805,    97,   100,   108,   109,   100,   100,   100,
     113,   114,    97,    96,    98,    97,    97,    97,    97,    97,
     385,    98,    97,     3,    98,   385,    97,   694,    97,   385,
      97,    97,   344,     3,   413,    98,   415,   416,   385,   351,
     385,   102,   354,   846,   100,   385,   805,   426,   671,   671,
     671,   101,   385,   385,   385,     8,   100,   583,   100,    48,
     114,   102,    98,    98,    98,     3,   378,    98,   447,    98,
     449,   101,   115,   385,   106,   553,   103,   105,   103,   103,
      99,   107,   103,   561,   101,    59,    99,   846,   614,   104,
      97,   114,   423,   423,   103,   423,   423,    97,    99,     3,
      98,   423,   423,   482,   720,    99,   406,   103,    99,   386,
     103,   423,   385,   386,   386,   101,   386,   101,   386,   386,
     103,   433,   101,   435,   436,   101,   104,   115,   103,    32,
     442,   496,     3,   659,   411,    99,   496,    99,   616,    99,
      99,    97,   454,    97,    99,    97,   525,    97,    99,    30,
     427,   677,   630,   631,   632,    99,    97,    97,    97,     3,
      97,    97,    97,   463,    97,    97,    97,    97,     8,    97,
     526,    97,    97,   504,   504,    97,   504,   504,    97,   526,
      97,   526,   504,   504,   496,   550,   526,    97,    97,    97,
     550,   570,   504,    99,    99,   526,   103,    99,   510,    99,
      67,    99,    97,    74,    75,    76,    77,    78,    79,    80,
      81,    97,    97,    97,   526,   527,   528,   529,     3,    90,
     114,   101,    93,    94,   115,    97,   116,   857,   858,    97,
      36,   101,    97,    99,    98,   114,    36,   115,   550,    56,
     540,   541,    39,   114,   103,   576,   576,   100,   576,   576,
      60,     3,   115,   103,   576,   117,    63,   115,   888,   115,
     114,   103,   114,   101,   576,    40,   115,   103,    97,   102,
     115,   548,    99,   114,   102,    97,   115,     1,   115,     3,
       4,    76,   114,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,   575,    23,
     115,   103,    26,    27,    28,    76,   310,    31,    32,    33,
      34,    35,    49,   310,   762,   694,    17,    41,    16,   430,
     507,    45,   659,   849,   396,   395,   697,    50,   616,   397,
     576,    55,   773,   619,   385,   450,   852,   504,   360,   854,
      64,    65,   757,   721,   856,   880,   384,   530,    78,   367,
     390,   861,   717,   671,   583,   442,    -1,   717,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    -1,   681,
      -1,    -1,    -1,    -1,    -1,    99,     3,    -1,   757,    -1,
      -1,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    23,    24,    25,    26,
      27,    -1,    -1,    -1,    -1,   717,   679,   679,    -1,   679,
      -1,   679,   679,    -1,    -1,    -1,    -1,    -1,   718,    -1,
      -1,   718,    -1,   735,     3,     4,    -1,    -1,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
     805,    -1,    -1,    -1,    23,   805,   758,    26,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    -1,    -1,   773,   853,   102,   773,    -1,    -1,    -1,
      -1,   846,    -1,    -1,    -1,    -1,   846,    -1,    -1,   762,
     762,    -1,   762,   805,   762,   762,    -1,    -1,    -1,    -1,
      -1,   813,    -1,    82,    83,    84,    85,    86,    87,    88,
      89,    90,   812,    -1,    -1,   812,    -1,    -1,    -1,    -1,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   846,    -1,    -1,    -1,    -1,    -1,
     871,   871,    -1,   871,   871,   857,   858,    -1,    -1,   871,
     871,    -1,    -1,    -1,    -1,     1,    -1,     3,     4,   871,
     860,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,   888,    23,    -1,    -1,
      26,    27,    28,    -1,    -1,    31,    32,    33,    34,    35,
      -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    45,
      46,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    -1,    -1,    -1,     1,
      96,     3,     4,    99,    -1,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    23,    -1,    -1,    26,    27,    28,    -1,    -1,    31,
      32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,    41,
      -1,    -1,    -1,    45,    46,    47,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      -1,    -1,    64,    65,     8,     9,    10,    -1,    -1,    13,
      14,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      -1,    -1,    -1,     1,    96,     3,     4,    99,    -1,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    23,    -1,    -1,    26,    27,
      28,    -1,    -1,    31,    32,    33,    34,    35,    -1,    -1,
      -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    -1,    -1,
      -1,    -1,    96,    -1,    -1,    -1,    64,    65,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    -1,    -1,    -1,     1,    96,     3,
       4,    99,    -1,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      -1,    -1,    26,    27,    28,    -1,    -1,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    -1,    -1,
      -1,     1,    96,     3,     4,    99,    -1,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    23,    -1,    -1,    26,    27,    28,    -1,
      -1,    31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,
      -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     1,    -1,    -1,     4,     5,     6,     7,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,
      20,    21,    -1,    23,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    41,    42,    43,    44,    -1,    96,    -1,    -1,    99,
      50,    51,    -1,    -1,    54,    -1,    -1,    57,    -1,    -1,
      -1,    -1,     0,     1,    64,    65,     4,     5,     6,     7,
      -1,    71,    72,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    23,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    99,
      -1,    -1,    -1,    41,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    50,    51,    -1,    -1,    54,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    65,     1,    -1,
       3,    -1,    -1,    71,    72,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      23,    24,    25,    26,    27,    -1,    -1,    95,    96,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
      -1,     4,     5,     6,     7,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      23,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    41,    42,
      43,    44,    -1,    96,    -1,    -1,    -1,    50,    51,    -1,
      -1,    54,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,
       1,    64,    65,     4,     5,     6,     7,    -1,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    23,    -1,    -1,     1,    -1,    28,     4,     5,
       6,     7,    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      41,    42,    43,    44,    20,    21,    -1,    23,    -1,    50,
      51,    -1,    28,    54,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    65,    41,    42,    43,    44,    -1,
      71,    72,    73,    -1,    50,    51,    -1,    -1,    54,    -1,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,
      -1,    -1,     3,    -1,    95,    71,    72,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    25,    26,    27,     3,    -1,    95,
      -1,    -1,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    24,    25,
      26,    27,     4,    -1,     6,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    23,    24,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    96,    -1,    -1,    50,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    82,    83,    84,    85,
      86,    87,    88,    89,    90,     1,    68,     3,     4,    -1,
      96,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    23,    -1,    -1,
      26,    27,    28,    -1,    -1,    31,    32,    33,    34,    35,
      -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    45,
      46,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,    85,
      86,    87,    88,    89,    90,     1,    -1,     3,     4,    -1,
      -1,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    23,    -1,    -1,
      26,    27,    28,    -1,    -1,    31,    32,    33,    34,    35,
       1,    -1,     3,    -1,    -1,    41,    -1,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    25,    26,    27,    -1,    64,    65,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,    85,
      86,    87,    88,    89,    90,     3,    -1,    -1,    -1,    -1,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    23,    24,    25,    26,    27,
      -1,    82,    83,    84,    85,    86,    87,    88,    89,    90,
       3,    -1,    -1,    -1,    -1,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    83,    84,    85,    86,    87,
      88,    89,    90,     3,    -1,    -1,    -1,    -1,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    -1,    82,
      83,    84,    85,    86,    87,    88,    89,    90,     3,    -1,
      -1,    -1,    -1,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,
      85,    86,    87,    88,    89,    90
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   119,   120,   121,     0,     1,     4,     5,     6,     7,
      20,    21,    23,    28,    41,    42,    43,    44,    50,    51,
      54,    57,    64,    65,    71,    72,    95,   122,   123,   139,
     141,   145,   146,   158,   161,   162,   166,   168,   171,   172,
     173,   177,   181,   183,   187,   188,   213,   214,   232,   240,
     241,   249,   258,   278,   280,   291,   293,   315,   316,   317,
     364,   415,   416,   417,   418,   419,   423,   445,   447,   470,
     471,   472,   473,   474,   478,   479,   480,   483,   487,   495,
     508,   509,   138,   215,   140,   167,   250,   279,   292,   318,
     365,     3,   212,   266,   166,    54,   166,   181,   183,    54,
     173,   183,   184,   212,   212,   448,     3,    90,   208,   211,
     208,   496,   510,   212,   100,   142,   131,   147,   159,    97,
      97,   130,   102,   169,   163,   132,   174,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    24,
      25,    26,    27,    82,    83,    84,    85,    86,    87,    88,
      89,   169,   208,   253,   254,   255,   256,   257,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   280,
     291,   293,   317,   326,   331,   334,   337,   338,   341,   342,
     345,   128,   124,   122,    96,   242,   125,   281,    22,   129,
     126,   127,   133,   420,   134,   446,   135,   169,   481,   481,
     136,   137,    98,   512,    97,    17,   208,   219,   268,   271,
     272,   273,   274,   275,   338,   342,   208,   212,   251,   253,
     212,   212,   212,   212,   169,   212,   169,   178,   212,   212,
     424,   212,   209,    76,    90,    76,     3,   241,    98,    98,
      97,     4,     6,    20,    21,    23,    24,    28,    50,    54,
      68,   488,   489,   491,   241,   505,    97,    49,   185,    98,
      97,    98,     8,    11,     8,     9,   100,   335,   100,   327,
     182,   101,   103,   100,   100,    97,    97,   208,    97,    98,
     294,    97,    97,    97,    97,    98,    97,    98,   457,    97,
     482,   475,   484,    97,    97,   513,   216,   252,   319,   366,
      98,   102,   426,   449,   211,   210,   497,     3,   242,   233,
     143,   219,   100,     3,   148,   490,    74,    75,    76,    77,
      78,    79,    80,    81,    93,    94,   108,   109,   113,   114,
     208,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   507,   101,   170,   164,   175,     8,   221,   231,
     100,   241,   330,   100,    48,   186,   332,   339,   343,   243,
     282,   114,   421,   458,   186,    98,    98,   515,   212,   212,
     259,   262,   266,   267,   346,    98,    98,   179,   427,   425,
     102,   454,   211,    98,   511,   234,   120,   121,     3,   101,
     103,   229,   229,   229,   221,   105,   106,   107,    91,    92,
     108,   109,   110,   111,   112,   506,   160,   205,   208,   194,
     195,   189,   103,   336,   254,   103,   328,   205,   231,   231,
     231,   114,   244,   241,   284,   286,   295,   428,   460,   476,
     485,    31,    32,    61,    66,    69,    70,   353,   354,   359,
     437,   439,   440,   504,   514,   516,   217,   347,   260,   320,
     367,   194,   208,   186,   455,   450,   498,   426,     7,    99,
     214,   219,   235,   237,   238,   276,   317,   144,   101,   149,
     491,   115,   223,   224,   225,   226,   226,   227,   227,   228,
     228,   228,   103,   212,   206,     1,    33,    34,   165,   196,
     214,   240,   249,   353,   364,   369,   374,   415,   416,    45,
      46,    47,   176,   190,   192,   193,   196,   240,   376,   221,
     241,   330,   330,   333,   340,   344,   211,   221,   245,   246,
     248,     1,   253,   287,   283,   285,   241,    52,    53,    62,
     240,   353,   422,   429,   437,   439,   442,   443,   444,   504,
      45,    55,   196,   459,   461,   464,   467,   194,   189,   355,
     360,    19,   208,   438,    59,   441,   208,   208,   519,   517,
     518,   438,   520,    99,   104,   241,   103,   241,   322,   325,
     285,   180,   208,   186,   500,   501,   236,    97,   212,    97,
      99,     3,    98,   241,   103,   204,    99,   200,   196,   197,
     202,   201,   203,    35,   208,   255,   338,   342,   375,   398,
     198,   199,   377,    99,   287,   190,   191,   101,   254,   103,
     103,   101,   101,   101,   104,   115,   103,   247,   290,   288,
      99,   286,     8,   208,   268,   273,   274,   275,   300,   317,
     208,   208,   208,   429,   435,    99,   430,   431,   432,   433,
     434,   436,   212,   212,    99,   462,   463,   477,   486,    32,
     398,   211,     3,     3,    97,    97,    97,   211,    97,   218,
     116,   348,   350,   261,     3,   321,   323,   368,    99,   451,
     499,   240,   353,   437,   439,   502,   251,    30,   239,   150,
     507,   207,    97,    97,    97,    97,    97,    97,   370,    97,
      97,     3,    97,   231,   329,   221,   211,   248,    97,   259,
     296,   211,   211,   211,    97,    97,    97,    97,    97,    97,
      97,   465,   468,    97,    97,    99,    99,   356,   361,   220,
     351,   349,   262,    99,   103,    99,    67,    99,   502,   503,
      97,    97,    97,   221,    97,    73,   123,   139,   152,   154,
     155,   208,     3,   378,   101,   330,   247,   289,   115,   114,
     380,   380,   398,   263,   266,   231,   350,   324,   452,    97,
     208,   151,   153,    97,   371,   380,   101,    97,   297,   381,
     382,   466,   469,   357,   362,   264,   352,   325,   208,   156,
      99,   154,   114,   389,   379,    98,   115,    36,   383,   386,
      39,   400,   400,   263,    56,   403,   103,   117,   453,   100,
     390,   391,   372,   400,   298,   387,   115,   384,   401,   358,
     404,   363,   265,    60,   456,     3,   492,   494,   115,    36,
      37,    38,   392,   395,   399,   400,     1,    29,    30,   301,
     303,   307,   309,   398,   103,   114,   400,   114,    63,   406,
     266,   208,   101,   493,   115,   393,   396,   373,   306,   311,
     310,   299,   302,   304,   308,   388,   385,   402,   405,   407,
     157,   103,   103,   398,    40,   409,    97,   221,   102,    99,
     303,   241,   309,   262,   386,   205,   205,   114,   212,   494,
     394,   397,   410,   312,   253,   313,   115,   115,   408,   395,
     262,   114,   102,   314,   305,   205,   411,   262,    97,   115,
      76,   412,   413,   115,   103,   414,    76
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   118,   119,   120,   120,   121,   121,   122,   122,   122,
     124,   123,   125,   123,   126,   123,   127,   123,   128,   123,
     129,   123,   130,   123,   131,   123,   132,   123,   133,   123,
     134,   123,   135,   123,   136,   123,   137,   123,   138,   123,
     140,   139,   142,   143,   144,   141,   145,   147,   148,   149,
     150,   151,   146,   152,   153,   153,   154,   154,   156,   157,
     155,   159,   160,   158,   161,   161,   163,   164,   165,   162,
     167,   166,   168,   168,   168,   170,   169,   169,   171,   171,
     171,   171,   172,   172,   174,   175,   176,   173,   178,   179,
     180,   177,   182,   181,   184,   183,   185,   185,   186,   186,
     187,   187,   188,   189,   189,   189,   190,   190,   191,   190,
     192,   192,   193,   194,   194,   195,   195,   197,   196,   198,
     196,   199,   196,   200,   196,   201,   196,   202,   196,   203,
     196,   204,   196,   205,   207,   206,   206,   208,   209,   208,
     210,   208,   211,   212,   213,   213,   213,   215,   216,   217,
     218,   214,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   220,   221,   222,   222,   223,   223,   224,   224,   225,
     225,   225,   226,   226,   226,   227,   227,   227,   227,   228,
     228,   228,   228,   229,   229,   229,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   231,   233,   232,   234,
     234,   235,   235,   235,   236,   235,   237,   237,   238,   239,
     239,   240,   241,   241,   243,   242,   244,   244,   245,   245,
     246,   247,   247,   248,   250,   249,   249,   249,   249,   249,
     249,   252,   251,   253,   253,   254,   254,   254,   255,   255,
     255,   255,   255,   255,   255,   255,   256,   256,   256,   256,
     256,   257,   257,   257,   258,   258,   259,   261,   260,   260,
     262,   262,   263,   265,   264,   264,   266,   267,   268,   268,
     269,   269,   269,   269,   269,   269,   269,   270,   270,   270,
     270,   270,   270,   270,   271,   271,   271,   272,   273,   273,
     274,   275,   276,   277,   279,   278,   281,   282,   283,   280,
     284,   285,   285,   286,   288,   289,   287,   290,   287,   292,
     291,   294,   295,   296,   297,   298,   299,   293,   300,   300,
     300,   300,   300,   300,   301,   302,   302,   304,   305,   303,
     306,   303,   307,   308,   308,   310,   309,   311,   312,   309,
     314,   313,   315,   316,   318,   319,   320,   321,   317,   322,
     324,   323,   323,   325,   327,   328,   329,   326,   326,   330,
     332,   333,   331,   331,   335,   336,   334,   337,   339,   340,
     338,   338,   341,   343,   344,   342,   342,   345,   347,   346,
     348,   349,   349,   351,   352,   350,   353,   353,   355,   356,
     357,   358,   354,   360,   361,   362,   363,   359,   365,   366,
     367,   368,   364,   370,   371,   372,   373,   369,   374,   374,
     374,   375,   375,   377,   378,   379,   376,   381,   380,   382,
     380,   383,   385,   384,   384,   387,   388,   386,   390,   389,
     391,   389,   392,   394,   393,   393,   396,   397,   395,   398,
     398,   398,   398,   399,   399,   399,   401,   402,   400,   400,
     404,   405,   403,   403,   407,   408,   406,   406,   410,   411,
     409,   409,   412,   414,   413,   413,   415,   416,   417,   417,
     418,   420,   421,   422,   419,   424,   425,   423,   427,   426,
     426,   428,   428,   428,   430,   429,   431,   429,   432,   429,
     433,   429,   434,   429,   435,   429,   436,   429,   437,   438,
     438,   439,   440,   441,   441,   442,   443,   444,   446,   445,
     448,   449,   450,   451,   452,   453,   447,   455,   454,   454,
     456,   456,   458,   459,   457,   460,   460,   461,   462,   461,
     463,   461,   465,   466,   464,   468,   469,   467,   470,   470,
     470,   471,   471,   472,   473,   475,   476,   477,   474,   478,
     479,   480,   482,   481,   484,   485,   486,   483,   487,   487,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     489,   490,   490,   491,   491,   492,   493,   493,   494,   496,
     497,   498,   499,   495,   500,   500,   501,   501,   502,   502,
     503,   502,   504,   504,   505,   506,   506,   507,   508,   510,
     511,   509,   513,   514,   512,   515,   515,   517,   516,   518,
     516,   519,   516,   520,   516
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
       2,     3,     0,     3,     0,     3,     1,     1,     1,     2,
       1,     0,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     0,     4,     0,
       1,     1,     2,     0,     4,     0,     1,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     2,     3,     2,
       1,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     0,     3,     0,     0,     0,     7,
       2,     2,     0,     2,     0,     0,     5,     0,     3,     0,
       3,     0,     0,     0,     0,     0,     0,    15,     1,     1,
       1,     1,     1,     1,     2,     2,     0,     0,     0,     6,
       0,     3,     2,     2,     0,     0,     3,     0,     0,     5,
       0,     3,     1,     1,     0,     0,     0,     0,     9,     2,
       0,     4,     0,     2,     0,     0,     0,     9,     8,     2,
       0,     0,     6,     2,     0,     0,     6,     6,     0,     0,
       6,     1,     1,     0,     0,     6,     1,     1,     0,     4,
       2,     2,     0,     0,     0,     5,     1,     1,     0,     0,
       0,     0,     9,     0,     0,     0,     0,     9,     0,     0,
       0,     0,     9,     0,     0,     0,     0,    10,     1,     1,
       0,     1,     1,     0,     0,     0,     7,     0,     3,     0,
       4,     2,     0,     4,     0,     0,     0,     5,     0,     3,
       0,     4,     2,     0,     4,     0,     0,     0,     5,     1,
       1,     1,     1,     1,     1,     1,     0,     0,     6,     0,
       0,     0,     6,     0,     0,     0,     6,     0,     0,     0,
       6,     0,     2,     0,     4,     0,     3,     3,     1,     1,
       2,     0,     0,     0,     7,     0,     0,     6,     0,     3,
       0,     3,     2,     0,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     3,     0,     3,     0,     3,     3,     1,
       1,     3,     2,     1,     0,     3,     3,     3,     0,     3,
       0,     0,     0,     0,     0,     0,    13,     0,     3,     0,
       2,     0,     0,     0,     5,     2,     0,     1,     0,     3,
       0,     3,     0,     0,     6,     0,     0,     6,     1,     1,
       1,     1,     1,     2,     3,     0,     0,     0,     8,     3,
       3,     2,     0,     3,     0,     0,     0,     8,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     3,     0,     2,     5,     2,     3,     0,     1,     0,
       0,     0,     0,     9,     3,     2,     1,     0,     2,     2,
       0,     3,     3,     3,     3,     4,     0,     1,     2,     0,
       0,     6,     0,     0,     5,     2,     0,     0,     3,     0,
       3,     0,     3,     0,     3
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
#line 429 "fe/idl.ypp"
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
#line 2743 "fe/idl.tab.cpp"
    break;

  case 10: /* $@1: %empty  */
#line 454 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AnnotationDeclSeen);
        }
#line 2751 "fe/idl.tab.cpp"
    break;

  case 11: /* fixed_definition: annotation_dcl $@1 ';'  */
#line 458 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2759 "fe/idl.tab.cpp"
    break;

  case 12: /* $@2: %empty  */
#line 462 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 2767 "fe/idl.tab.cpp"
    break;

  case 13: /* fixed_definition: type_dcl $@2 ';'  */
#line 466 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2775 "fe/idl.tab.cpp"
    break;

  case 14: /* $@3: %empty  */
#line 470 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 2783 "fe/idl.tab.cpp"
    break;

  case 15: /* fixed_definition: typeid_dcl $@3 ';'  */
#line 474 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2791 "fe/idl.tab.cpp"
    break;

  case 16: /* $@4: %empty  */
#line 478 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 2799 "fe/idl.tab.cpp"
    break;

  case 17: /* fixed_definition: typeprefix_dcl $@4 ';'  */
#line 482 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2807 "fe/idl.tab.cpp"
    break;

  case 18: /* $@5: %empty  */
#line 486 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 2815 "fe/idl.tab.cpp"
    break;

  case 19: /* fixed_definition: const_dcl $@5 ';'  */
#line 490 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2823 "fe/idl.tab.cpp"
    break;

  case 20: /* $@6: %empty  */
#line 494 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 2831 "fe/idl.tab.cpp"
    break;

  case 21: /* fixed_definition: exception $@6 ';'  */
#line 498 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2839 "fe/idl.tab.cpp"
    break;

  case 22: /* $@7: %empty  */
#line 502 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceDeclSeen);
        }
#line 2847 "fe/idl.tab.cpp"
    break;

  case 23: /* fixed_definition: interface_def $@7 ';'  */
#line 506 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2855 "fe/idl.tab.cpp"
    break;

  case 24: /* $@8: %empty  */
#line 510 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleDeclSeen);
        }
#line 2863 "fe/idl.tab.cpp"
    break;

  case 25: /* fixed_definition: module $@8 ';'  */
#line 514 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2871 "fe/idl.tab.cpp"
    break;

  case 26: /* $@9: %empty  */
#line 518 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeDeclSeen);
        }
#line 2879 "fe/idl.tab.cpp"
    break;

  case 27: /* fixed_definition: value_def $@9 ';'  */
#line 522 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2887 "fe/idl.tab.cpp"
    break;

  case 28: /* $@10: %empty  */
#line 526 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentDeclSeen);
        }
#line 2895 "fe/idl.tab.cpp"
    break;

  case 29: /* fixed_definition: component $@10 ';'  */
#line 530 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2903 "fe/idl.tab.cpp"
    break;

  case 30: /* $@11: %empty  */
#line 534 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeDeclSeen);
        }
#line 2911 "fe/idl.tab.cpp"
    break;

  case 31: /* fixed_definition: home_decl $@11 ';'  */
#line 538 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2919 "fe/idl.tab.cpp"
    break;

  case 32: /* $@12: %empty  */
#line 542 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventDeclSeen);
        }
#line 2927 "fe/idl.tab.cpp"
    break;

  case 33: /* fixed_definition: event $@12 ';'  */
#line 546 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2935 "fe/idl.tab.cpp"
    break;

  case 34: /* $@13: %empty  */
#line 550 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeDeclSeen);
        }
#line 2943 "fe/idl.tab.cpp"
    break;

  case 35: /* fixed_definition: porttype_decl $@13 ';'  */
#line 554 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2951 "fe/idl.tab.cpp"
    break;

  case 36: /* $@14: %empty  */
#line 558 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorDeclSeen);
        }
#line 2959 "fe/idl.tab.cpp"
    break;

  case 37: /* fixed_definition: connector_decl $@14 ';'  */
#line 562 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2967 "fe/idl.tab.cpp"
    break;

  case 38: /* $@15: %empty  */
#line 566 "fe/idl.ypp"
        {
          idl_global->err ()->syntax_error (idl_global->parse_state ());
        }
#line 2975 "fe/idl.tab.cpp"
    break;

  case 39: /* fixed_definition: error $@15 ';'  */
#line 570 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
          (yyval.dcval) = 0;
        }
#line 2985 "fe/idl.tab.cpp"
    break;

  case 40: /* $@16: %empty  */
#line 579 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSeen);
        }
#line 2993 "fe/idl.tab.cpp"
    break;

  case 41: /* module_header: IDL_MODULE $@16 scoped_name  */
#line 583 "fe/idl.ypp"
        {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 3001 "fe/idl.tab.cpp"
    break;

  case 42: /* @17: %empty  */
#line 590 "fe/idl.ypp"
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
#line 3044 "fe/idl.tab.cpp"
    break;

  case 43: /* $@18: %empty  */
#line 629 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSqSeen);
        }
#line 3052 "fe/idl.tab.cpp"
    break;

  case 44: /* $@19: %empty  */
#line 633 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleBodySeen);
        }
#line 3060 "fe/idl.tab.cpp"
    break;

  case 45: /* module: module_header @17 '{' $@18 at_least_one_definition $@19 '}'  */
#line 637 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleQsSeen);
          /*
           * Finished with this module - pop it from the scope stack.
           */

          idl_global->scopes ().pop ();
          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 3074 "fe/idl.tab.cpp"
    break;

  case 46: /* template_module_header: module_header '<'  */
#line 650 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleIDSeen);
        }
#line 3082 "fe/idl.tab.cpp"
    break;

  case 47: /* $@20: %empty  */
#line 657 "fe/idl.ypp"
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
#line 3100 "fe/idl.tab.cpp"
    break;

  case 48: /* $@21: %empty  */
#line 671 "fe/idl.ypp"
        {
          if (FE_Utils::duplicate_param_id ((yyvsp[0].plval)))
            {
              idl_global->err ()->duplicate_param_id (
                (yyvsp[-2].idlist));

              return 1;
            }
        }
#line 3114 "fe/idl.tab.cpp"
    break;

  case 49: /* $@22: %empty  */
#line 681 "fe/idl.ypp"
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
#line 3148 "fe/idl.tab.cpp"
    break;

  case 50: /* $@23: %empty  */
#line 711 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleSqSeen);
        }
#line 3156 "fe/idl.tab.cpp"
    break;

  case 51: /* $@24: %empty  */
#line 715 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleBodySeen);
        }
#line 3164 "fe/idl.tab.cpp"
    break;

  case 52: /* template_module: template_module_header $@20 at_least_one_formal_parameter $@21 '>' $@22 '{' $@23 at_least_one_tpl_definition $@24 '}'  */
#line 719 "fe/idl.ypp"
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
#line 3187 "fe/idl.tab.cpp"
    break;

  case 58: /* $@25: %empty  */
#line 755 "fe/idl.ypp"
        {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefSeen);
        }
#line 3196 "fe/idl.tab.cpp"
    break;

  case 59: /* $@26: %empty  */
#line 760 "fe/idl.ypp"
        {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefParamsSeen);
        }
#line 3205 "fe/idl.tab.cpp"
    break;

  case 60: /* template_module_ref: IDL_ALIAS scoped_name $@25 '<' at_least_one_formal_parameter_name '>' $@26 defining_id  */
#line 765 "fe/idl.ypp"
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
#line 3287 "fe/idl.tab.cpp"
    break;

  case 61: /* $@27: %empty  */
#line 846 "fe/idl.ypp"
        {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleSeen);
        }
#line 3296 "fe/idl.tab.cpp"
    break;

  case 62: /* $@28: %empty  */
#line 851 "fe/idl.ypp"
        {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleArgsSeen);
        }
#line 3305 "fe/idl.tab.cpp"
    break;

  case 63: /* template_module_inst: template_module_header $@27 at_least_one_actual_parameter '>' $@28 defining_id  */
#line 856 "fe/idl.ypp"
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
#line 3373 "fe/idl.tab.cpp"
    break;

  case 66: /* $@29: %empty  */
#line 928 "fe/idl.ypp"
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
#line 3415 "fe/idl.tab.cpp"
    break;

  case 67: /* $@30: %empty  */
#line 966 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
#line 3423 "fe/idl.tab.cpp"
    break;

  case 68: /* $@31: %empty  */
#line 970 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
#line 3431 "fe/idl.tab.cpp"
    break;

  case 69: /* interface: interface_header $@29 '{' $@30 exports $@31 '}'  */
#line 974 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);

          (yyval.dcval) = dynamic_cast<AST_Decl *> (idl_global->scopes ().top_non_null ());
          /*
           * Done with this interface - pop it off the scopes stack
           */
          idl_global->scopes ().pop ();
        }
#line 3445 "fe/idl.tab.cpp"
    break;

  case 70: /* $@32: %empty  */
#line 987 "fe/idl.ypp"
         {
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSeen);
         }
#line 3453 "fe/idl.tab.cpp"
    break;

  case 71: /* interface_decl: IDL_INTERFACE $@32 defining_id  */
#line 991 "fe/idl.ypp"
         {
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceIDSeen);
           (yyval.idval) = (yyvsp[0].idval);
         }
#line 3462 "fe/idl.tab.cpp"
    break;

  case 72: /* interface_header: interface_decl inheritance_spec  */
#line 999 "fe/idl.ypp"
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
#line 3502 "fe/idl.tab.cpp"
    break;

  case 73: /* interface_header: IDL_LOCAL interface_decl inheritance_spec  */
#line 1036 "fe/idl.ypp"
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
#line 3535 "fe/idl.tab.cpp"
    break;

  case 74: /* interface_header: IDL_ABSTRACT interface_decl inheritance_spec  */
#line 1066 "fe/idl.ypp"
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
#line 3568 "fe/idl.tab.cpp"
    break;

  case 75: /* $@33: %empty  */
#line 1098 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 3576 "fe/idl.tab.cpp"
    break;

  case 76: /* inheritance_spec: ':' opt_truncatable $@33 at_least_one_scoped_name  */
#line 1102 "fe/idl.ypp"
        {
          (yyvsp[0].nlval)->truncatable ((yyvsp[-2].bval));
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3585 "fe/idl.tab.cpp"
    break;

  case 77: /* inheritance_spec: %empty  */
#line 1107 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 3593 "fe/idl.tab.cpp"
    break;

  case 82: /* valuetype: IDL_CUSTOM value_concrete_decl  */
#line 1121 "fe/idl.ypp"
        {
          idl_global->err ()->unsupported_error ("custom is not supported");
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 3602 "fe/idl.tab.cpp"
    break;

  case 84: /* @34: %empty  */
#line 1130 "fe/idl.ypp"
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
#line 3653 "fe/idl.tab.cpp"
    break;

  case 85: /* $@35: %empty  */
#line 1177 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3661 "fe/idl.tab.cpp"
    break;

  case 86: /* $@36: %empty  */
#line 1181 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3669 "fe/idl.tab.cpp"
    break;

  case 87: /* value_concrete_decl: value_header @34 '{' $@35 value_elements $@36 '}'  */
#line 1185 "fe/idl.ypp"
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
#line 3694 "fe/idl.tab.cpp"
    break;

  case 88: /* $@37: %empty  */
#line 1210 "fe/idl.ypp"
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
#line 3741 "fe/idl.tab.cpp"
    break;

  case 89: /* $@38: %empty  */
#line 1253 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3749 "fe/idl.tab.cpp"
    break;

  case 90: /* $@39: %empty  */
#line 1257 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3757 "fe/idl.tab.cpp"
    break;

  case 91: /* value_abs_decl: IDL_ABSTRACT value_header $@37 '{' $@38 exports $@39 '}'  */
#line 1261 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeQsSeen);

          /*
           * Done with this valuetype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = 0;
        }
#line 3772 "fe/idl.tab.cpp"
    break;

  case 92: /* $@40: %empty  */
#line 1276 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 3780 "fe/idl.tab.cpp"
    break;

  case 93: /* value_header: value_decl inheritance_spec $@40 supports_spec  */
#line 1280 "fe/idl.ypp"
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
#line 3815 "fe/idl.tab.cpp"
    break;

  case 94: /* $@41: %empty  */
#line 1314 "fe/idl.ypp"
        {
           idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSeen);
        }
#line 3823 "fe/idl.tab.cpp"
    break;

  case 95: /* value_decl: IDL_VALUETYPE $@41 defining_id  */
#line 1318 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 3832 "fe/idl.tab.cpp"
    break;

  case 96: /* opt_truncatable: IDL_TRUNCATABLE  */
#line 1326 "fe/idl.ypp"
        {
          (yyval.bval) = true;
        }
#line 3840 "fe/idl.tab.cpp"
    break;

  case 97: /* opt_truncatable: %empty  */
#line 1330 "fe/idl.ypp"
        {
          (yyval.bval) = false;
        }
#line 3848 "fe/idl.tab.cpp"
    break;

  case 98: /* supports_spec: IDL_SUPPORTS at_least_one_scoped_name  */
#line 1338 "fe/idl.ypp"
        {
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3856 "fe/idl.tab.cpp"
    break;

  case 99: /* supports_spec: %empty  */
#line 1342 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 3864 "fe/idl.tab.cpp"
    break;

  case 100: /* value_forward_decl: IDL_ABSTRACT value_decl  */
#line 1350 "fe/idl.ypp"
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
#line 3891 "fe/idl.tab.cpp"
    break;

  case 101: /* value_forward_decl: value_decl  */
#line 1374 "fe/idl.ypp"
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
#line 3920 "fe/idl.tab.cpp"
    break;

  case 102: /* value_box_decl: value_decl type_spec  */
#line 1402 "fe/idl.ypp"
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
#line 3987 "fe/idl.tab.cpp"
    break;

  case 103: /* value_elements: value_elements at_least_one_annotation value_element  */
#line 1468 "fe/idl.ypp"
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
#line 4010 "fe/idl.tab.cpp"
    break;

  case 104: /* value_elements: value_elements value_element  */
#line 1487 "fe/idl.ypp"
        {
          delete (yyvsp[0].decls_val);
        }
#line 4018 "fe/idl.tab.cpp"
    break;

  case 107: /* value_element: export  */
#line 1496 "fe/idl.ypp"
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
#line 4033 "fe/idl.tab.cpp"
    break;

  case 108: /* @42: %empty  */
#line 1507 "fe/idl.ypp"
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
#line 4048 "fe/idl.tab.cpp"
    break;

  case 109: /* value_element: init_decl @42 ';'  */
#line 1518 "fe/idl.ypp"
        {
          (yyval.decls_val) = (yyvsp[-1].decls_val);
        }
#line 4056 "fe/idl.tab.cpp"
    break;

  case 110: /* visibility: IDL_PUBLIC  */
#line 1525 "fe/idl.ypp"
        {
          (yyval.vival) = AST_Field::vis_PUBLIC;
        }
#line 4064 "fe/idl.tab.cpp"
    break;

  case 111: /* visibility: IDL_PRIVATE  */
#line 1529 "fe/idl.ypp"
        {
          (yyval.vival) = AST_Field::vis_PRIVATE;
        }
#line 4072 "fe/idl.tab.cpp"
    break;

  case 112: /* state_member: visibility member_i  */
#line 1536 "fe/idl.ypp"
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
#line 4094 "fe/idl.tab.cpp"
    break;

  case 115: /* at_least_one_export: exports at_least_one_annotation export  */
#line 1562 "fe/idl.ypp"
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
#line 4113 "fe/idl.tab.cpp"
    break;

  case 117: /* $@43: %empty  */
#line 1581 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 4121 "fe/idl.tab.cpp"
    break;

  case 118: /* export: type_dcl $@43 ';'  */
#line 1585 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4129 "fe/idl.tab.cpp"
    break;

  case 119: /* $@44: %empty  */
#line 1589 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 4137 "fe/idl.tab.cpp"
    break;

  case 120: /* export: typeid_dcl $@44 ';'  */
#line 1593 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4145 "fe/idl.tab.cpp"
    break;

  case 121: /* $@45: %empty  */
#line 1597 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 4153 "fe/idl.tab.cpp"
    break;

  case 122: /* export: typeprefix_dcl $@45 ';'  */
#line 1601 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4161 "fe/idl.tab.cpp"
    break;

  case 123: /* $@46: %empty  */
#line 1605 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 4169 "fe/idl.tab.cpp"
    break;

  case 124: /* export: const_dcl $@46 ';'  */
#line 1609 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4177 "fe/idl.tab.cpp"
    break;

  case 125: /* $@47: %empty  */
#line 1613 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 4185 "fe/idl.tab.cpp"
    break;

  case 126: /* export: exception $@47 ';'  */
#line 1617 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4193 "fe/idl.tab.cpp"
    break;

  case 127: /* $@48: %empty  */
#line 1621 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 4201 "fe/idl.tab.cpp"
    break;

  case 128: /* export: attribute $@48 ';'  */
#line 1625 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4209 "fe/idl.tab.cpp"
    break;

  case 129: /* $@49: %empty  */
#line 1629 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpDeclSeen);
        }
#line 4217 "fe/idl.tab.cpp"
    break;

  case 130: /* export: operation $@49 ';'  */
#line 1633 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4225 "fe/idl.tab.cpp"
    break;

  case 131: /* $@50: %empty  */
#line 1637 "fe/idl.ypp"
        {
          idl_global->err ()->syntax_error (idl_global->parse_state ());
        }
#line 4233 "fe/idl.tab.cpp"
    break;

  case 132: /* export: error $@50 ';'  */
#line 1641 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
          (yyval.dcval) = 0;
        }
#line 4243 "fe/idl.tab.cpp"
    break;

  case 133: /* at_least_one_scoped_name: scoped_name scoped_names  */
#line 1650 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.nlval),
                          UTL_NameList ((yyvsp[-1].idlist),
                                        (yyvsp[0].nlval)),
                          1);
        }
#line 4254 "fe/idl.tab.cpp"
    break;

  case 134: /* $@51: %empty  */
#line 1661 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SNListCommaSeen);
        }
#line 4262 "fe/idl.tab.cpp"
    break;

  case 135: /* scoped_names: scoped_names ',' $@51 scoped_name  */
#line 1665 "fe/idl.ypp"
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
#line 4286 "fe/idl.tab.cpp"
    break;

  case 136: /* scoped_names: %empty  */
#line 1685 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 4294 "fe/idl.tab.cpp"
    break;

  case 137: /* scoped_name: id  */
#line 1692 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          ACE_NEW_RETURN ((yyval.idlist),
                          UTL_IdList ((yyvsp[0].idval),
                                      0),
                          1);
        }
#line 4307 "fe/idl.tab.cpp"
    break;

  case 138: /* $@52: %empty  */
#line 1701 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);
        }
#line 4315 "fe/idl.tab.cpp"
    break;

  case 139: /* scoped_name: IDL_SCOPE_DELIMITOR $@52 id  */
#line 1705 "fe/idl.ypp"
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
#line 4339 "fe/idl.tab.cpp"
    break;

  case 140: /* $@53: %empty  */
#line 1726 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);

          // This cleans up all the non-global "::"s in scoped names.
          // If there is a global one, it gets put into the UTL_IdList,
          // so we clean it up in the case above.
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 4353 "fe/idl.tab.cpp"
    break;

  case 141: /* scoped_name: scoped_name IDL_SCOPE_DELIMITOR $@53 id  */
#line 1736 "fe/idl.ypp"
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
#line 4369 "fe/idl.tab.cpp"
    break;

  case 142: /* id: IDENTIFIER  */
#line 1750 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.idval),
                          Identifier ((yyvsp[0].strval)),
                          1);
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 4381 "fe/idl.tab.cpp"
    break;

  case 143: /* defining_id: IDENTIFIER  */
#line 1760 "fe/idl.ypp"
        {
          /* defining_id is a defining identifier
             whereas id is usually a reference to a defining identifier */
          ACE_NEW_RETURN ((yyval.idval), Identifier ((yyvsp[0].strval)), 1);
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 4393 "fe/idl.tab.cpp"
    break;

  case 144: /* interface_forward: interface_decl  */
#line 1771 "fe/idl.ypp"
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
#line 4440 "fe/idl.tab.cpp"
    break;

  case 145: /* interface_forward: IDL_LOCAL interface_decl  */
#line 1815 "fe/idl.ypp"
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
#line 4470 "fe/idl.tab.cpp"
    break;

  case 146: /* interface_forward: IDL_ABSTRACT interface_decl  */
#line 1842 "fe/idl.ypp"
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
#line 4502 "fe/idl.tab.cpp"
    break;

  case 147: /* $@54: %empty  */
#line 1873 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstSeen);
        }
#line 4510 "fe/idl.tab.cpp"
    break;

  case 148: /* $@55: %empty  */
#line 1877 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstTypeSeen);
        }
#line 4518 "fe/idl.tab.cpp"
    break;

  case 149: /* $@56: %empty  */
#line 1881 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstIDSeen);
        }
#line 4526 "fe/idl.tab.cpp"
    break;

  case 150: /* $@57: %empty  */
#line 1885 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstAssignSeen);
        }
#line 4534 "fe/idl.tab.cpp"
    break;

  case 151: /* const_dcl: IDL_CONST $@54 const_type $@55 defining_id $@56 '=' $@57 expression  */
#line 1889 "fe/idl.ypp"
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
#line 4594 "fe/idl.tab.cpp"
    break;

  case 158: /* const_type: string_type_spec  */
#line 1954 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_string;
        }
#line 4602 "fe/idl.tab.cpp"
    break;

  case 159: /* const_type: wstring_type_spec  */
#line 1958 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_wstring;
        }
#line 4610 "fe/idl.tab.cpp"
    break;

  case 160: /* const_type: scoped_name  */
#line 1962 "fe/idl.ypp"
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
#line 4680 "fe/idl.tab.cpp"
    break;

  case 164: /* or_expr: or_expr '|' xor_expr  */
#line 2035 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_or,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4693 "fe/idl.tab.cpp"
    break;

  case 166: /* xor_expr: xor_expr '^' and_expr  */
#line 2048 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_xor,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4706 "fe/idl.tab.cpp"
    break;

  case 168: /* and_expr: and_expr '&' shift_expr  */
#line 2061 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_and,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4719 "fe/idl.tab.cpp"
    break;

  case 170: /* shift_expr: shift_expr IDL_LEFT_SHIFT add_expr  */
#line 2074 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_left,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4732 "fe/idl.tab.cpp"
    break;

  case 171: /* shift_expr: shift_expr IDL_RIGHT_SHIFT add_expr  */
#line 2083 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_right,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4745 "fe/idl.tab.cpp"
    break;

  case 173: /* add_expr: add_expr '+' mult_expr  */
#line 2096 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_add,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4758 "fe/idl.tab.cpp"
    break;

  case 174: /* add_expr: add_expr '-' mult_expr  */
#line 2105 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_minus,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4771 "fe/idl.tab.cpp"
    break;

  case 176: /* mult_expr: mult_expr '*' unary_expr  */
#line 2118 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mul,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4784 "fe/idl.tab.cpp"
    break;

  case 177: /* mult_expr: mult_expr '/' unary_expr  */
#line 2127 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_div,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4797 "fe/idl.tab.cpp"
    break;

  case 178: /* mult_expr: mult_expr '%' unary_expr  */
#line 2136 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mod,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4810 "fe/idl.tab.cpp"
    break;

  case 180: /* unary_expr: '+' primary_expr  */
#line 2149 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_u_plus,
                                    (yyvsp[0].exval),
                                    0
                                  );
        }
#line 4823 "fe/idl.tab.cpp"
    break;

  case 181: /* unary_expr: '-' primary_expr  */
#line 2158 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                   AST_Expression::EC_u_minus,
                                   (yyvsp[0].exval),
                                   0
                                 );
        }
#line 4836 "fe/idl.tab.cpp"
    break;

  case 182: /* unary_expr: '~' primary_expr  */
#line 2167 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                   AST_Expression::EC_bit_neg,
                                   (yyvsp[0].exval),
                                   0
                                 );
        }
#line 4849 "fe/idl.tab.cpp"
    break;

  case 183: /* primary_expr: scoped_name  */
#line 2179 "fe/idl.ypp"
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
#line 4910 "fe/idl.tab.cpp"
    break;

  case 185: /* primary_expr: '(' const_expr ')'  */
#line 2237 "fe/idl.ypp"
        {
          (yyval.exval) = (yyvsp[-1].exval);
        }
#line 4918 "fe/idl.tab.cpp"
    break;

  case 186: /* literal: IDL_INTEGER_LITERAL  */
#line 2244 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].ival));
        }
#line 4926 "fe/idl.tab.cpp"
    break;

  case 187: /* literal: IDL_UINTEGER_LITERAL  */
#line 2248 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr ((yyvsp[0].uival));
        }
#line 4935 "fe/idl.tab.cpp"
    break;

  case 188: /* literal: IDL_STRING_LITERAL  */
#line 2253 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].sval));
          (yyvsp[0].sval)->destroy ();
          delete (yyvsp[0].sval);
          (yyvsp[0].sval) = 0;
        }
#line 4946 "fe/idl.tab.cpp"
    break;

  case 189: /* literal: IDL_WSTRING_LITERAL  */
#line 2260 "fe/idl.ypp"
        {
          char *wide_string = (yyvsp[0].wsval);
          (yyval.exval) = idl_global->gen ()->create_expr (wide_string);
          ACE_OS::free (wide_string);
          (yyvsp[0].wsval) = 0;
        }
#line 4957 "fe/idl.tab.cpp"
    break;

  case 190: /* literal: IDL_CHARACTER_LITERAL  */
#line 2267 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].cval));
        }
#line 4965 "fe/idl.tab.cpp"
    break;

  case 191: /* literal: IDL_WCHAR_LITERAL  */
#line 2271 "fe/idl.ypp"
        {
          ACE_OutputCDR::from_wchar wc ((yyvsp[0].wcval));
          (yyval.exval) = idl_global->gen ()->create_expr (wc);
        }
#line 4974 "fe/idl.tab.cpp"
    break;

  case 192: /* literal: IDL_FIXED_PT_LITERAL  */
#line 2276 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].fixval));
        }
#line 4982 "fe/idl.tab.cpp"
    break;

  case 193: /* literal: IDL_FLOATING_PT_LITERAL  */
#line 2280 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].dval));
        }
#line 4990 "fe/idl.tab.cpp"
    break;

  case 194: /* literal: IDL_TRUETOK  */
#line 2284 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr (true);
        }
#line 4998 "fe/idl.tab.cpp"
    break;

  case 195: /* literal: IDL_FALSETOK  */
#line 2288 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr (false);
        }
#line 5006 "fe/idl.tab.cpp"
    break;

  case 196: /* positive_int_expr: const_expr  */
#line 2295 "fe/idl.ypp"
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
#line 5075 "fe/idl.tab.cpp"
    break;

  case 197: /* $@58: %empty  */
#line 2363 "fe/idl.ypp"
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
#line 5096 "fe/idl.tab.cpp"
    break;

  case 198: /* annotation_dcl: IDL_ANNOTATION_DECL defining_id '{' $@58 annotation_body '}'  */
#line 2380 "fe/idl.ypp"
        {
          Identifier *id = (yyvsp[-4].idval);
          idl_global->scopes ().pop ();
          id->destroy ();
          delete id;

          (yyval.dcval) = 0;
        }
#line 5109 "fe/idl.tab.cpp"
    break;

  case 204: /* $@59: %empty  */
#line 2400 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
          idl_global->in_typedef (true);
        }
#line 5118 "fe/idl.tab.cpp"
    break;

  case 208: /* annotation_member: annotation_member_type defining_id annotation_member_default ';'  */
#line 2414 "fe/idl.ypp"
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
#line 5175 "fe/idl.tab.cpp"
    break;

  case 209: /* annotation_member_default: IDL_DEFAULT const_expr  */
#line 2470 "fe/idl.ypp"
        {
          (yyval.exval) = (yyvsp[0].exval);
        }
#line 5183 "fe/idl.tab.cpp"
    break;

  case 210: /* annotation_member_default: %empty  */
#line 2474 "fe/idl.ypp"
        {
          (yyval.exval) = 0;
        }
#line 5191 "fe/idl.tab.cpp"
    break;

  case 211: /* at_least_one_annotation: annotations_maybe annotation_appl  */
#line 2481 "fe/idl.ypp"
        {
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          AST_Annotation_Appl *annotation = (yyvsp[0].annotation_val);
          if (annotation)
            {
              annotations->add (annotation);
            }
          (yyval.annotations_val) = annotations;
        }
#line 5205 "fe/idl.tab.cpp"
    break;

  case 212: /* annotations_maybe: annotations_maybe annotation_appl  */
#line 2494 "fe/idl.ypp"
        {
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          AST_Annotation_Appl *annotation = (yyvsp[0].annotation_val);
          if (annotation)
            {
              annotations->add (annotation);
            }
          (yyval.annotations_val) = annotations;
        }
#line 5219 "fe/idl.tab.cpp"
    break;

  case 213: /* annotations_maybe: %empty  */
#line 2504 "fe/idl.ypp"
        {
          (yyval.annotations_val) = new AST_Annotation_Appls ();
        }
#line 5227 "fe/idl.tab.cpp"
    break;

  case 214: /* @60: %empty  */
#line 2511 "fe/idl.ypp"
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
#line 5288 "fe/idl.tab.cpp"
    break;

  case 215: /* annotation_appl: IDL_ANNOTATION_SYMBOL scoped_name @60 annotation_appl_params_maybe  */
#line 2568 "fe/idl.ypp"
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
#line 5320 "fe/idl.tab.cpp"
    break;

  case 216: /* annotation_appl_params_maybe: '(' annotation_appl_params ')'  */
#line 2599 "fe/idl.ypp"
        {
          (yyval.annotation_params_val) = (yyvsp[-1].annotation_params_val);
        }
#line 5328 "fe/idl.tab.cpp"
    break;

  case 217: /* annotation_appl_params_maybe: %empty  */
#line 2603 "fe/idl.ypp"
        {
          (yyval.annotation_params_val) = 0;
        }
#line 5336 "fe/idl.tab.cpp"
    break;

  case 218: /* annotation_appl_params: const_expr  */
#line 2610 "fe/idl.ypp"
        {
          AST_Annotation_Appl::Params *params = new AST_Annotation_Appl::Params;
          AST_Annotation_Appl::Param *param = new AST_Annotation_Appl::Param;
          param->id = 0;
          param->expr = (yyvsp[0].exval);
          params->push (param);
          (yyval.annotation_params_val) = params;
        }
#line 5349 "fe/idl.tab.cpp"
    break;

  case 219: /* annotation_appl_params: named_annotation_appl_params  */
#line 2619 "fe/idl.ypp"
        {
          (yyval.annotation_params_val) = (yyvsp[0].annotation_params_val);
        }
#line 5357 "fe/idl.tab.cpp"
    break;

  case 220: /* named_annotation_appl_params: named_annotation_appl_param more_named_annotation_appl_params  */
#line 2626 "fe/idl.ypp"
        {
          AST_Annotation_Appl::Params *params = (yyvsp[0].annotation_params_val);
          params->push ((yyvsp[-1].annotation_param_val));
          (yyval.annotation_params_val) = params;
        }
#line 5367 "fe/idl.tab.cpp"
    break;

  case 221: /* more_named_annotation_appl_params: ',' named_annotation_appl_param more_named_annotation_appl_params  */
#line 2634 "fe/idl.ypp"
        {
          AST_Annotation_Appl::Params *params = (yyvsp[0].annotation_params_val);
          params->push ((yyvsp[-1].annotation_param_val));
          (yyval.annotation_params_val) = params;
        }
#line 5377 "fe/idl.tab.cpp"
    break;

  case 222: /* more_named_annotation_appl_params: %empty  */
#line 2640 "fe/idl.ypp"
        {
          (yyval.annotation_params_val) = new AST_Annotation_Appl::Params;
        }
#line 5385 "fe/idl.tab.cpp"
    break;

  case 223: /* named_annotation_appl_param: id '=' const_expr  */
#line 2647 "fe/idl.ypp"
        {
          AST_Annotation_Appl::Param *param = new AST_Annotation_Appl::Param;
          param->id = (yyvsp[-2].idval);
          // Annotation Member Names Can't Clash with C++ keywords
          FE_Utils::original_local_name (param->id);
          param->expr = (yyvsp[0].exval);
          (yyval.annotation_param_val) = param;
        }
#line 5398 "fe/idl.tab.cpp"
    break;

  case 224: /* $@61: %empty  */
#line 2659 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
          idl_global->in_typedef (true);
        }
#line 5407 "fe/idl.tab.cpp"
    break;

  case 225: /* type_dcl: IDL_TYPEDEF $@61 type_declarator  */
#line 2664 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5415 "fe/idl.tab.cpp"
    break;

  case 226: /* type_dcl: struct_type  */
#line 2668 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5423 "fe/idl.tab.cpp"
    break;

  case 227: /* type_dcl: union_type  */
#line 2672 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5431 "fe/idl.tab.cpp"
    break;

  case 228: /* type_dcl: enum_type  */
#line 2676 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5439 "fe/idl.tab.cpp"
    break;

  case 229: /* type_dcl: IDL_NATIVE simple_declarator  */
#line 2680 "fe/idl.ypp"
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
#line 5471 "fe/idl.tab.cpp"
    break;

  case 230: /* type_dcl: constructed_forward_type_spec  */
#line 2708 "fe/idl.ypp"
        {
          (yyval.dcval) = 0;
        }
#line 5479 "fe/idl.tab.cpp"
    break;

  case 231: /* $@62: %empty  */
#line 2715 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeSpecSeen);
        }
#line 5487 "fe/idl.tab.cpp"
    break;

  case 232: /* type_declarator: type_spec $@62 at_least_one_declarator  */
#line 2719 "fe/idl.ypp"
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
#line 5555 "fe/idl.tab.cpp"
    break;

  case 235: /* simple_type_spec: base_type_spec  */
#line 2791 "fe/idl.ypp"
        {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5566 "fe/idl.tab.cpp"
    break;

  case 237: /* simple_type_spec: scoped_name  */
#line 2799 "fe/idl.ypp"
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
#line 5593 "fe/idl.tab.cpp"
    break;

  case 256: /* at_least_one_declarator: declarator declarators  */
#line 2855 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 5604 "fe/idl.tab.cpp"
    break;

  case 257: /* $@63: %empty  */
#line 2866 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 5612 "fe/idl.tab.cpp"
    break;

  case 258: /* declarators: declarators ',' $@63 declarator  */
#line 2870 "fe/idl.ypp"
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
#line 5636 "fe/idl.tab.cpp"
    break;

  case 259: /* declarators: %empty  */
#line 2890 "fe/idl.ypp"
        {
          (yyval.dlval) = 0;
        }
#line 5644 "fe/idl.tab.cpp"
    break;

  case 262: /* at_least_one_simple_declarator: simple_declarator simple_declarators  */
#line 2902 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 5655 "fe/idl.tab.cpp"
    break;

  case 263: /* $@64: %empty  */
#line 2913 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 5663 "fe/idl.tab.cpp"
    break;

  case 264: /* simple_declarators: simple_declarators ',' $@64 simple_declarator  */
#line 2917 "fe/idl.ypp"
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
#line 5687 "fe/idl.tab.cpp"
    break;

  case 265: /* simple_declarators: %empty  */
#line 2937 "fe/idl.ypp"
        {
          (yyval.dlval) = 0;
        }
#line 5695 "fe/idl.tab.cpp"
    break;

  case 266: /* simple_declarator: defining_id  */
#line 2944 "fe/idl.ypp"
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
#line 5712 "fe/idl.tab.cpp"
    break;

  case 267: /* complex_declarator: array_declarator  */
#line 2960 "fe/idl.ypp"
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
#line 5731 "fe/idl.tab.cpp"
    break;

  case 270: /* signed_int: IDL_LONG  */
#line 2983 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_long;
        }
#line 5739 "fe/idl.tab.cpp"
    break;

  case 271: /* signed_int: IDL_LONG IDL_LONG  */
#line 2987 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_longlong;
        }
#line 5747 "fe/idl.tab.cpp"
    break;

  case 272: /* signed_int: IDL_SHORT  */
#line 2991 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_short;
        }
#line 5755 "fe/idl.tab.cpp"
    break;

  case 273: /* signed_int: IDL_INT8  */
#line 2995 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_int8;
        }
#line 5763 "fe/idl.tab.cpp"
    break;

  case 274: /* signed_int: IDL_INT16  */
#line 2999 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_short;
        }
#line 5771 "fe/idl.tab.cpp"
    break;

  case 275: /* signed_int: IDL_INT32  */
#line 3003 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_long;
        }
#line 5779 "fe/idl.tab.cpp"
    break;

  case 276: /* signed_int: IDL_INT64  */
#line 3007 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_longlong;
        }
#line 5787 "fe/idl.tab.cpp"
    break;

  case 277: /* unsigned_int: IDL_UNSIGNED IDL_LONG  */
#line 3014 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ulong;
        }
#line 5795 "fe/idl.tab.cpp"
    break;

  case 278: /* unsigned_int: IDL_UNSIGNED IDL_LONG IDL_LONG  */
#line 3018 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ulonglong;
        }
#line 5803 "fe/idl.tab.cpp"
    break;

  case 279: /* unsigned_int: IDL_UNSIGNED IDL_SHORT  */
#line 3022 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ushort;
        }
#line 5811 "fe/idl.tab.cpp"
    break;

  case 280: /* unsigned_int: IDL_UINT8  */
#line 3026 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_uint8;
        }
#line 5819 "fe/idl.tab.cpp"
    break;

  case 281: /* unsigned_int: IDL_UINT16  */
#line 3030 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ushort;
        }
#line 5827 "fe/idl.tab.cpp"
    break;

  case 282: /* unsigned_int: IDL_UINT32  */
#line 3034 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ulong;
        }
#line 5835 "fe/idl.tab.cpp"
    break;

  case 283: /* unsigned_int: IDL_UINT64  */
#line 3038 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ulonglong;
        }
#line 5843 "fe/idl.tab.cpp"
    break;

  case 284: /* floating_pt_type: IDL_DOUBLE  */
#line 3045 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_double;
        }
#line 5851 "fe/idl.tab.cpp"
    break;

  case 285: /* floating_pt_type: IDL_FLOAT  */
#line 3049 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_float;
        }
#line 5859 "fe/idl.tab.cpp"
    break;

  case 286: /* floating_pt_type: IDL_LONG IDL_DOUBLE  */
#line 3053 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_longdouble;
        }
#line 5867 "fe/idl.tab.cpp"
    break;

  case 287: /* fixed_type: IDL_FIXED  */
#line 3060 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_fixed;
        }
#line 5875 "fe/idl.tab.cpp"
    break;

  case 288: /* char_type: IDL_CHAR  */
#line 3067 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_char;
        }
#line 5883 "fe/idl.tab.cpp"
    break;

  case 289: /* char_type: IDL_WCHAR  */
#line 3071 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_wchar;
        }
#line 5891 "fe/idl.tab.cpp"
    break;

  case 290: /* octet_type: IDL_OCTET  */
#line 3078 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_octet;
        }
#line 5899 "fe/idl.tab.cpp"
    break;

  case 291: /* boolean_type: IDL_BOOLEAN  */
#line 3085 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_bool;
        }
#line 5907 "fe/idl.tab.cpp"
    break;

  case 292: /* any_type: IDL_ANY  */
#line 3092 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_any;
        }
#line 5915 "fe/idl.tab.cpp"
    break;

  case 293: /* object_type: IDL_OBJECT  */
#line 3099 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_object;
        }
#line 5923 "fe/idl.tab.cpp"
    break;

  case 294: /* $@65: %empty  */
#line 3106 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSeen);
        }
#line 5931 "fe/idl.tab.cpp"
    break;

  case 295: /* struct_decl: IDL_STRUCT $@65 defining_id  */
#line 3110 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 5940 "fe/idl.tab.cpp"
    break;

  case 296: /* $@66: %empty  */
#line 3119 "fe/idl.ypp"
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
#line 5975 "fe/idl.tab.cpp"
    break;

  case 297: /* $@67: %empty  */
#line 3150 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSqSeen);
        }
#line 5983 "fe/idl.tab.cpp"
    break;

  case 298: /* $@68: %empty  */
#line 3154 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructBodySeen);
        }
#line 5991 "fe/idl.tab.cpp"
    break;

  case 299: /* struct_type: struct_decl $@66 '{' $@67 at_least_one_member $@68 '}'  */
#line 3158 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructQsSeen);

          /*
           * Done with this struct. Pop its scope off the scopes stack.
           */
          (yyval.dcval) = dynamic_cast<AST_Structure*> (
                   idl_global->scopes ().top_non_null ()
                 );
          idl_global->scopes ().pop ();
        }
#line 6007 "fe/idl.tab.cpp"
    break;

  case 303: /* member: annotations_maybe member_i  */
#line 3180 "fe/idl.ypp"
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
#line 6025 "fe/idl.tab.cpp"
    break;

  case 304: /* $@69: %empty  */
#line 3197 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberTypeSeen);
        }
#line 6033 "fe/idl.tab.cpp"
    break;

  case 305: /* $@70: %empty  */
#line 3201 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsSeen);
        }
#line 6041 "fe/idl.tab.cpp"
    break;

  case 306: /* member_i: type_spec $@69 at_least_one_declarator $@70 ';'  */
#line 3205 "fe/idl.ypp"
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
#line 6099 "fe/idl.tab.cpp"
    break;

  case 307: /* $@71: %empty  */
#line 3259 "fe/idl.ypp"
        {
          idl_global->err ()->syntax_error (idl_global->parse_state ());
        }
#line 6107 "fe/idl.tab.cpp"
    break;

  case 308: /* member_i: error $@71 ';'  */
#line 3263 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 6116 "fe/idl.tab.cpp"
    break;

  case 309: /* $@72: %empty  */
#line 3271 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSeen);
        }
#line 6124 "fe/idl.tab.cpp"
    break;

  case 310: /* union_decl: IDL_UNION $@72 defining_id  */
#line 3275 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 6133 "fe/idl.tab.cpp"
    break;

  case 311: /* $@73: %empty  */
#line 3283 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchSeen);
        }
#line 6141 "fe/idl.tab.cpp"
    break;

  case 312: /* $@74: %empty  */
#line 3287 "fe/idl.ypp"
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
#line 6178 "fe/idl.tab.cpp"
    break;

  case 313: /* $@75: %empty  */
#line 3320 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchTypeSeen);
        }
#line 6186 "fe/idl.tab.cpp"
    break;

  case 314: /* $@76: %empty  */
#line 3324 "fe/idl.ypp"
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
#line 6249 "fe/idl.tab.cpp"
    break;

  case 315: /* $@77: %empty  */
#line 3383 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSqSeen);
        }
#line 6257 "fe/idl.tab.cpp"
    break;

  case 316: /* $@78: %empty  */
#line 3387 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionBodySeen);
        }
#line 6265 "fe/idl.tab.cpp"
    break;

  case 317: /* union_type: union_decl IDL_SWITCH $@73 '(' $@74 annotations_maybe switch_type_spec $@75 ')' $@76 '{' $@77 at_least_one_case_branch $@78 '}'  */
#line 3391 "fe/idl.ypp"
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
#line 6285 "fe/idl.tab.cpp"
    break;

  case 318: /* switch_type_spec: integer_type  */
#line 3410 "fe/idl.ypp"
        {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 6296 "fe/idl.tab.cpp"
    break;

  case 319: /* switch_type_spec: char_type  */
#line 3417 "fe/idl.ypp"
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
#line 6313 "fe/idl.tab.cpp"
    break;

  case 320: /* switch_type_spec: octet_type  */
#line 3430 "fe/idl.ypp"
        {
          /* octets are not allowed. */
          idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 6326 "fe/idl.tab.cpp"
    break;

  case 321: /* switch_type_spec: boolean_type  */
#line 3439 "fe/idl.ypp"
        {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 6337 "fe/idl.tab.cpp"
    break;

  case 323: /* switch_type_spec: scoped_name  */
#line 3447 "fe/idl.ypp"
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
#line 6448 "fe/idl.tab.cpp"
    break;

  case 327: /* $@79: %empty  */
#line 3564 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionLabelSeen);
        }
#line 6456 "fe/idl.tab.cpp"
    break;

  case 328: /* $@80: %empty  */
#line 3568 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemSeen);
        }
#line 6464 "fe/idl.tab.cpp"
    break;

  case 329: /* case_branch: at_least_one_case_label $@79 annotations_maybe element_spec $@80 ';'  */
#line 3572 "fe/idl.ypp"
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
#line 6500 "fe/idl.tab.cpp"
    break;

  case 330: /* $@81: %empty  */
#line 3604 "fe/idl.ypp"
        {
          idl_global->err ()->syntax_error (idl_global->parse_state ());
        }
#line 6508 "fe/idl.tab.cpp"
    break;

  case 331: /* case_branch: error $@81 ';'  */
#line 3608 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 6517 "fe/idl.tab.cpp"
    break;

  case 332: /* at_least_one_case_label: case_label case_labels  */
#line 3616 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.llval),
                          UTL_LabelList ((yyvsp[-1].ulval),
                                         (yyvsp[0].llval)),
                          1);
        }
#line 6528 "fe/idl.tab.cpp"
    break;

  case 333: /* case_labels: case_labels case_label  */
#line 3626 "fe/idl.ypp"
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
#line 6550 "fe/idl.tab.cpp"
    break;

  case 334: /* case_labels: %empty  */
#line 3644 "fe/idl.ypp"
        {
          (yyval.llval) = 0;
        }
#line 6558 "fe/idl.tab.cpp"
    break;

  case 335: /* $@82: %empty  */
#line 3651 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DefaultSeen);
        }
#line 6566 "fe/idl.tab.cpp"
    break;

  case 336: /* case_label: IDL_DEFAULT $@82 ':'  */
#line 3655 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen ()->create_union_label (
                                       AST_UnionLabel::UL_default,
                                       0
                                     );
        }
#line 6579 "fe/idl.tab.cpp"
    break;

  case 337: /* $@83: %empty  */
#line 3664 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_CaseSeen);
        }
#line 6587 "fe/idl.tab.cpp"
    break;

  case 338: /* $@84: %empty  */
#line 3668 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelExprSeen);
        }
#line 6595 "fe/idl.tab.cpp"
    break;

  case 339: /* case_label: IDL_CASE $@83 const_expr $@84 ':'  */
#line 3672 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen ()->create_union_label (
                                      AST_UnionLabel::UL_label,
                                      (yyvsp[-2].exval)
                                    );
        }
#line 6608 "fe/idl.tab.cpp"
    break;

  case 340: /* $@85: %empty  */
#line 3684 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemTypeSeen);
        }
#line 6616 "fe/idl.tab.cpp"
    break;

  case 341: /* element_spec: type_spec $@85 declarator  */
#line 3688 "fe/idl.ypp"
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
#line 6663 "fe/idl.tab.cpp"
    break;

  case 342: /* struct_forward_type: struct_decl  */
#line 3734 "fe/idl.ypp"
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
#line 6689 "fe/idl.tab.cpp"
    break;

  case 343: /* union_forward_type: union_decl  */
#line 3759 "fe/idl.ypp"
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
#line 6713 "fe/idl.tab.cpp"
    break;

  case 344: /* $@86: %empty  */
#line 3782 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSeen);
        }
#line 6721 "fe/idl.tab.cpp"
    break;

  case 345: /* $@87: %empty  */
#line 3786 "fe/idl.ypp"
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
#line 6756 "fe/idl.tab.cpp"
    break;

  case 346: /* $@88: %empty  */
#line 3817 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSqSeen);
        }
#line 6764 "fe/idl.tab.cpp"
    break;

  case 347: /* $@89: %empty  */
#line 3821 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumBodySeen);
        }
#line 6772 "fe/idl.tab.cpp"
    break;

  case 348: /* enum_type: IDL_ENUM $@86 defining_id $@87 '{' $@88 at_least_one_enumerator $@89 '}'  */
#line 3825 "fe/idl.ypp"
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
#line 6795 "fe/idl.tab.cpp"
    break;

  case 350: /* $@90: %empty  */
#line 3850 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumCommaSeen);
        }
#line 6803 "fe/idl.tab.cpp"
    break;

  case 353: /* enumerator: annotations_maybe IDENTIFIER  */
#line 3859 "fe/idl.ypp"
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
#line 6842 "fe/idl.tab.cpp"
    break;

  case 354: /* $@91: %empty  */
#line 3897 "fe/idl.ypp"
        {
          idl_global->set_parse_state(IDL_GlobalData::PS_MapSeen);
          idl_global->scopes().push(0);
        }
#line 6851 "fe/idl.tab.cpp"
    break;

  case 355: /* $@92: %empty  */
#line 3902 "fe/idl.ypp"
        {
          idl_global->set_parse_state(IDL_GlobalData::PS_MapSqSeen);
        }
#line 6859 "fe/idl.tab.cpp"
    break;

  case 356: /* $@93: %empty  */
#line 3907 "fe/idl.ypp"
        {
          idl_global->set_parse_state(IDL_GlobalData::PS_MapCommaSeen);
        }
#line 6867 "fe/idl.tab.cpp"
    break;

  case 357: /* map_type_spec: IDL_MAP $@91 '<' $@92 map_type ',' $@93 map_type '>'  */
#line 3912 "fe/idl.ypp"
        {
          idl_global->set_parse_state(IDL_GlobalData::PS_MapQsSeen);

          AST_Map *map = 0;
          Decl_Annotations_Pair *key_type = (yyvsp[-4].decl_annotations_pair_val);
          Decl_Annotations_Pair *val_type = (yyvsp[-1].decl_annotations_pair_val);
          
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
                  // map->base_type_annotations (*type_annotations);

                  idl_global->err ()->anonymous_type_diagnostic ();
                }
            }

          (yyval.dcval) = map;
        }
#line 6927 "fe/idl.tab.cpp"
    break;

  case 358: /* map_type_spec: IDL_MAP '<' map_type ',' map_type ',' positive_int_expr '>'  */
#line 3975 "fe/idl.ypp"
        {
          idl_global->set_parse_state(IDL_GlobalData::PS_MapQsSeen);

          AST_Map *map = 0;
          Decl_Annotations_Pair *key_type = (yyvsp[-5].decl_annotations_pair_val);
          Decl_Annotations_Pair *val_type = (yyvsp[-3].decl_annotations_pair_val);

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

                  map =
                    idl_global->gen ()->create_map (
                        (yyvsp[-1].exval),
                        ktp,
                        vtp,
                        &sn,
                        s->is_local (),
                        s->is_abstract ()
                      );
                  // map->base_type_annotations (*type_annotations);

                  idl_global->err ()->anonymous_type_diagnostic ();
                }
            }
          (yyval.dcval) = map;
        }
#line 6982 "fe/idl.tab.cpp"
    break;

  case 359: /* map_type: annotations_maybe simple_type_spec  */
#line 4029 "fe/idl.ypp"
        {
          idl_global->set_parse_state(IDL_GlobalData::PS_MapTypeSeen);
          Decl_Annotations_Pair* pair = new Decl_Annotations_Pair;
          pair->decl = (yyvsp[0].dcval);
          pair->annotations = (yyvsp[-1].annotations_val);
          (yyval.decl_annotations_pair_val) = pair;
        }
#line 6994 "fe/idl.tab.cpp"
    break;

  case 360: /* $@94: %empty  */
#line 4041 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceCommaSeen);
        }
#line 7002 "fe/idl.tab.cpp"
    break;

  case 361: /* $@95: %empty  */
#line 4045 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceExprSeen);
        }
#line 7010 "fe/idl.tab.cpp"
    break;

  case 362: /* sequence_type_spec: seq_head ',' $@94 positive_int_expr $@95 '>'  */
#line 4049 "fe/idl.ypp"
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
#line 7095 "fe/idl.tab.cpp"
    break;

  case 363: /* sequence_type_spec: seq_head '>'  */
#line 4131 "fe/idl.ypp"
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
#line 7161 "fe/idl.tab.cpp"
    break;

  case 364: /* $@96: %empty  */
#line 4196 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSeen);

          /*
           * Push a sequence marker on scopes stack.
           */
          idl_global->scopes ().push (0);
        }
#line 7174 "fe/idl.tab.cpp"
    break;

  case 365: /* $@97: %empty  */
#line 4205 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSqSeen);
        }
#line 7182 "fe/idl.tab.cpp"
    break;

  case 366: /* seq_head: IDL_SEQUENCE $@96 '<' $@97 annotations_maybe simple_type_spec  */
#line 4209 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceTypeSeen);
          Decl_Annotations_Pair *seq_head = new Decl_Annotations_Pair;
          seq_head->decl = (yyvsp[0].dcval);
          seq_head->annotations = (yyvsp[-1].annotations_val);
          (yyval.decl_annotations_pair_val) = seq_head;
        }
#line 7194 "fe/idl.tab.cpp"
    break;

  case 367: /* fixed_type_spec: IDL_FIXED '<' positive_int_expr ',' const_expr '>'  */
#line 4220 "fe/idl.ypp"
        {
          (yyvsp[-1].exval)->evaluate (AST_Expression::EK_positive_int);
          (yyval.dcval) = idl_global->gen ()->create_fixed ((yyvsp[-3].exval), (yyvsp[-1].exval));
        }
#line 7203 "fe/idl.tab.cpp"
    break;

  case 368: /* $@98: %empty  */
#line 4229 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 7211 "fe/idl.tab.cpp"
    break;

  case 369: /* $@99: %empty  */
#line 4233 "fe/idl.ypp"
        {
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 7219 "fe/idl.tab.cpp"
    break;

  case 370: /* string_type_spec: string_head '<' $@98 positive_int_expr $@99 '>'  */
#line 4237 "fe/idl.ypp"
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
#line 7262 "fe/idl.tab.cpp"
    break;

  case 371: /* string_type_spec: string_head  */
#line 4276 "fe/idl.ypp"
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
#line 7289 "fe/idl.tab.cpp"
    break;

  case 372: /* string_head: IDL_STRING  */
#line 4302 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 7297 "fe/idl.tab.cpp"
    break;

  case 373: /* $@100: %empty  */
#line 4310 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 7305 "fe/idl.tab.cpp"
    break;

  case 374: /* $@101: %empty  */
#line 4314 "fe/idl.ypp"
        {
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 7313 "fe/idl.tab.cpp"
    break;

  case 375: /* wstring_type_spec: wstring_head '<' $@100 positive_int_expr $@101 '>'  */
#line 4318 "fe/idl.ypp"
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
#line 7356 "fe/idl.tab.cpp"
    break;

  case 376: /* wstring_type_spec: wstring_head  */
#line 4357 "fe/idl.ypp"
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
#line 7383 "fe/idl.tab.cpp"
    break;

  case 377: /* wstring_head: IDL_WSTRING  */
#line 4383 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 7391 "fe/idl.tab.cpp"
    break;

  case 378: /* $@102: %empty  */
#line 4390 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayIDSeen);
        }
#line 7399 "fe/idl.tab.cpp"
    break;

  case 379: /* array_declarator: defining_id $@102 annotations_maybe at_least_one_array_dim  */
#line 4394 "fe/idl.ypp"
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
#line 7439 "fe/idl.tab.cpp"
    break;

  case 380: /* at_least_one_array_dim: array_dim array_dims  */
#line 4433 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.elval),
                          UTL_ExprList ((yyvsp[-1].exval),
                                        (yyvsp[0].elval)),
                          1);
        }
#line 7450 "fe/idl.tab.cpp"
    break;

  case 381: /* array_dims: array_dims array_dim  */
#line 4443 "fe/idl.ypp"
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
#line 7472 "fe/idl.tab.cpp"
    break;

  case 382: /* array_dims: %empty  */
#line 4461 "fe/idl.ypp"
        {
          (yyval.elval) = 0;
        }
#line 7480 "fe/idl.tab.cpp"
    break;

  case 383: /* $@103: %empty  */
#line 4468 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimSqSeen);
        }
#line 7488 "fe/idl.tab.cpp"
    break;

  case 384: /* $@104: %empty  */
#line 4472 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimExprSeen);
        }
#line 7496 "fe/idl.tab.cpp"
    break;

  case 385: /* array_dim: '[' $@103 positive_int_expr $@104 ']'  */
#line 4476 "fe/idl.ypp"
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
#line 7554 "fe/idl.tab.cpp"
    break;

  case 388: /* $@105: %empty  */
#line 4538 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrROSeen);
        }
#line 7562 "fe/idl.tab.cpp"
    break;

  case 389: /* $@106: %empty  */
#line 4542 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 7570 "fe/idl.tab.cpp"
    break;

  case 390: /* $@107: %empty  */
#line 4546 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 7578 "fe/idl.tab.cpp"
    break;

  case 391: /* $@108: %empty  */
#line 4550 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 7586 "fe/idl.tab.cpp"
    break;

  case 392: /* attribute_readonly: IDL_READONLY $@105 IDL_ATTRIBUTE $@106 param_type_spec $@107 at_least_one_simple_declarator $@108 opt_raises  */
#line 4554 "fe/idl.ypp"
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
#line 7642 "fe/idl.tab.cpp"
    break;

  case 393: /* $@109: %empty  */
#line 4609 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 7650 "fe/idl.tab.cpp"
    break;

  case 394: /* $@110: %empty  */
#line 4613 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 7658 "fe/idl.tab.cpp"
    break;

  case 395: /* $@111: %empty  */
#line 4617 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 7666 "fe/idl.tab.cpp"
    break;

  case 396: /* $@112: %empty  */
#line 4621 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseCompleted);
        }
#line 7674 "fe/idl.tab.cpp"
    break;

  case 397: /* attribute_readwrite: IDL_ATTRIBUTE $@109 param_type_spec $@110 at_least_one_simple_declarator $@111 opt_getraises $@112 opt_setraises  */
#line 4625 "fe/idl.ypp"
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
#line 7739 "fe/idl.tab.cpp"
    break;

  case 398: /* $@113: %empty  */
#line 4689 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSeen);
        }
#line 7747 "fe/idl.tab.cpp"
    break;

  case 399: /* @114: %empty  */
#line 4693 "fe/idl.ypp"
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
#line 7783 "fe/idl.tab.cpp"
    break;

  case 400: /* $@115: %empty  */
#line 4725 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSqSeen);
        }
#line 7791 "fe/idl.tab.cpp"
    break;

  case 401: /* $@116: %empty  */
#line 4729 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptBodySeen);
        }
#line 7799 "fe/idl.tab.cpp"
    break;

  case 402: /* exception: IDL_EXCEPTION $@113 defining_id @114 '{' $@115 members $@116 '}'  */
#line 4733 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptQsSeen);
          /*
           * Done with this exception. Pop its scope from the scope stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 7813 "fe/idl.tab.cpp"
    break;

  case 403: /* $@117: %empty  */
#line 4746 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 7821 "fe/idl.tab.cpp"
    break;

  case 404: /* $@118: %empty  */
#line 4750 "fe/idl.ypp"
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
#line 7886 "fe/idl.tab.cpp"
    break;

  case 405: /* $@119: %empty  */
#line 4811 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 7894 "fe/idl.tab.cpp"
    break;

  case 406: /* $@120: %empty  */
#line 4815 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);
        }
#line 7902 "fe/idl.tab.cpp"
    break;

  case 407: /* operation: opt_op_attribute op_type_spec $@117 IDENTIFIER $@118 parameter_list $@119 opt_raises $@120 opt_context  */
#line 4819 "fe/idl.ypp"
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
#line 7937 "fe/idl.tab.cpp"
    break;

  case 408: /* opt_op_attribute: IDL_ONEWAY  */
#line 4853 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_oneway;
        }
#line 7946 "fe/idl.tab.cpp"
    break;

  case 409: /* opt_op_attribute: IDL_IDEMPOTENT  */
#line 4858 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_idempotent;
        }
#line 7955 "fe/idl.tab.cpp"
    break;

  case 410: /* opt_op_attribute: %empty  */
#line 4863 "fe/idl.ypp"
        {
          (yyval.ofval) = AST_Operation::OP_noflags;
        }
#line 7963 "fe/idl.tab.cpp"
    break;

  case 412: /* op_type_spec: IDL_VOID  */
#line 4871 "fe/idl.ypp"
        {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                  AST_Expression::EV_void
                                                );
        }
#line 7974 "fe/idl.tab.cpp"
    break;

  case 413: /* $@121: %empty  */
#line 4881 "fe/idl.ypp"
        {
          //@@ PS_FactorySeen?
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 7983 "fe/idl.tab.cpp"
    break;

  case 414: /* @122: %empty  */
#line 4886 "fe/idl.ypp"
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
#line 8030 "fe/idl.tab.cpp"
    break;

  case 415: /* $@123: %empty  */
#line 4929 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 8038 "fe/idl.tab.cpp"
    break;

  case 416: /* init_decl: IDL_FACTORY $@121 IDENTIFIER @122 init_parameter_list $@123 opt_raises  */
#line 4933 "fe/idl.ypp"
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
#line 8057 "fe/idl.tab.cpp"
    break;

  case 417: /* $@124: %empty  */
#line 4951 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 8065 "fe/idl.tab.cpp"
    break;

  case 418: /* init_parameter_list: '(' $@124 ')'  */
#line 4955 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 8073 "fe/idl.tab.cpp"
    break;

  case 419: /* $@125: %empty  */
#line 4959 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 8081 "fe/idl.tab.cpp"
    break;

  case 420: /* init_parameter_list: '(' $@125 at_least_one_in_parameter ')'  */
#line 4964 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 8089 "fe/idl.tab.cpp"
    break;

  case 422: /* $@126: %empty  */
#line 4974 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 8097 "fe/idl.tab.cpp"
    break;

  case 425: /* $@127: %empty  */
#line 4983 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 8105 "fe/idl.tab.cpp"
    break;

  case 426: /* $@128: %empty  */
#line 4987 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 8113 "fe/idl.tab.cpp"
    break;

  case 427: /* in_parameter: IDL_IN $@127 param_type_spec $@128 declarator  */
#line 4991 "fe/idl.ypp"
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
#line 8149 "fe/idl.tab.cpp"
    break;

  case 428: /* $@129: %empty  */
#line 5026 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 8157 "fe/idl.tab.cpp"
    break;

  case 429: /* parameter_list: '(' $@129 ')'  */
#line 5030 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 8165 "fe/idl.tab.cpp"
    break;

  case 430: /* $@130: %empty  */
#line 5034 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 8173 "fe/idl.tab.cpp"
    break;

  case 431: /* parameter_list: '(' $@130 at_least_one_parameter ')'  */
#line 5039 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 8181 "fe/idl.tab.cpp"
    break;

  case 433: /* $@131: %empty  */
#line 5049 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 8189 "fe/idl.tab.cpp"
    break;

  case 436: /* $@132: %empty  */
#line 5058 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 8197 "fe/idl.tab.cpp"
    break;

  case 437: /* $@133: %empty  */
#line 5062 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 8205 "fe/idl.tab.cpp"
    break;

  case 438: /* parameter: direction $@132 param_type_spec $@133 declarator  */
#line 5066 "fe/idl.ypp"
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
#line 8248 "fe/idl.tab.cpp"
    break;

  case 439: /* param_type_spec: base_type_spec  */
#line 5108 "fe/idl.ypp"
        {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 8259 "fe/idl.tab.cpp"
    break;

  case 442: /* param_type_spec: scoped_name  */
#line 5117 "fe/idl.ypp"
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
#line 8367 "fe/idl.tab.cpp"
    break;

  case 443: /* direction: IDL_IN  */
#line 5224 "fe/idl.ypp"
        {
          (yyval.dival) = AST_Argument::dir_IN;
        }
#line 8375 "fe/idl.tab.cpp"
    break;

  case 444: /* direction: IDL_OUT  */
#line 5228 "fe/idl.ypp"
        {
          (yyval.dival) = AST_Argument::dir_OUT;
        }
#line 8383 "fe/idl.tab.cpp"
    break;

  case 445: /* direction: IDL_INOUT  */
#line 5232 "fe/idl.ypp"
        {
          (yyval.dival) = AST_Argument::dir_INOUT;
        }
#line 8391 "fe/idl.tab.cpp"
    break;

  case 446: /* $@134: %empty  */
#line 5239 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSeen);
        }
#line 8399 "fe/idl.tab.cpp"
    break;

  case 447: /* $@135: %empty  */
#line 5243 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSqSeen);
        }
#line 8407 "fe/idl.tab.cpp"
    break;

  case 448: /* opt_raises: IDL_RAISES $@134 '(' $@135 at_least_one_scoped_name ')'  */
#line 5248 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 8416 "fe/idl.tab.cpp"
    break;

  case 449: /* opt_raises: %empty  */
#line 5253 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 8424 "fe/idl.tab.cpp"
    break;

  case 450: /* $@136: %empty  */
#line 5260 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSeen);
        }
#line 8432 "fe/idl.tab.cpp"
    break;

  case 451: /* $@137: %empty  */
#line 5264 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSqSeen);
        }
#line 8440 "fe/idl.tab.cpp"
    break;

  case 452: /* opt_getraises: IDL_GETRAISES $@136 '(' $@137 at_least_one_scoped_name ')'  */
#line 5269 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 8449 "fe/idl.tab.cpp"
    break;

  case 453: /* opt_getraises: %empty  */
#line 5274 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 8457 "fe/idl.tab.cpp"
    break;

  case 454: /* $@138: %empty  */
#line 5281 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSeen);
        }
#line 8465 "fe/idl.tab.cpp"
    break;

  case 455: /* $@139: %empty  */
#line 5285 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSqSeen);
        }
#line 8473 "fe/idl.tab.cpp"
    break;

  case 456: /* opt_setraises: IDL_SETRAISES $@138 '(' $@139 at_least_one_scoped_name ')'  */
#line 5290 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 8482 "fe/idl.tab.cpp"
    break;

  case 457: /* opt_setraises: %empty  */
#line 5295 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 8490 "fe/idl.tab.cpp"
    break;

  case 458: /* $@140: %empty  */
#line 5302 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSeen);
        }
#line 8498 "fe/idl.tab.cpp"
    break;

  case 459: /* $@141: %empty  */
#line 5306 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSqSeen);
        }
#line 8506 "fe/idl.tab.cpp"
    break;

  case 460: /* opt_context: IDL_CONTEXT $@140 '(' $@141 at_least_one_string_literal ')'  */
#line 5311 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextQsSeen);
          (yyval.slval) = (yyvsp[-1].slval);
        }
#line 8515 "fe/idl.tab.cpp"
    break;

  case 461: /* opt_context: %empty  */
#line 5316 "fe/idl.ypp"
        {
          (yyval.slval) = 0;
        }
#line 8523 "fe/idl.tab.cpp"
    break;

  case 462: /* at_least_one_string_literal: IDL_STRING_LITERAL string_literals  */
#line 5323 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.slval),
                          UTL_StrList ((yyvsp[-1].sval),
                                       (yyvsp[0].slval)),
                          1);
        }
#line 8534 "fe/idl.tab.cpp"
    break;

  case 463: /* $@142: %empty  */
#line 5334 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextCommaSeen);
        }
#line 8542 "fe/idl.tab.cpp"
    break;

  case 464: /* string_literals: string_literals ',' $@142 IDL_STRING_LITERAL  */
#line 5338 "fe/idl.ypp"
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
#line 8564 "fe/idl.tab.cpp"
    break;

  case 465: /* string_literals: %empty  */
#line 5356 "fe/idl.ypp"
        {
          (yyval.slval) = 0;
        }
#line 8572 "fe/idl.tab.cpp"
    break;

  case 466: /* typeid_dcl: IDL_TYPEID scoped_name IDL_STRING_LITERAL  */
#line 5363 "fe/idl.ypp"
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
#line 8603 "fe/idl.tab.cpp"
    break;

  case 467: /* typeprefix_dcl: IDL_TYPEPREFIX scoped_name IDL_STRING_LITERAL  */
#line 5393 "fe/idl.ypp"
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
#line 8643 "fe/idl.tab.cpp"
    break;

  case 470: /* component_forward_decl: IDL_COMPONENT defining_id  */
#line 5438 "fe/idl.ypp"
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
#line 8673 "fe/idl.tab.cpp"
    break;

  case 471: /* @143: %empty  */
#line 5467 "fe/idl.ypp"
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
#line 8719 "fe/idl.tab.cpp"
    break;

  case 472: /* $@144: %empty  */
#line 5509 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentSqSeen);
        }
#line 8727 "fe/idl.tab.cpp"
    break;

  case 473: /* $@145: %empty  */
#line 5513 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentBodySeen);
        }
#line 8735 "fe/idl.tab.cpp"
    break;

  case 474: /* component_decl: component_header @143 '{' $@144 component_exports $@145 '}'  */
#line 5517 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentQsSeen);

          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 8750 "fe/idl.tab.cpp"
    break;

  case 475: /* $@146: %empty  */
#line 5532 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentIDSeen);
        }
#line 8758 "fe/idl.tab.cpp"
    break;

  case 476: /* $@147: %empty  */
#line 5536 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 8766 "fe/idl.tab.cpp"
    break;

  case 477: /* component_header: IDL_COMPONENT defining_id $@146 component_inheritance_spec $@147 supports_spec  */
#line 5540 "fe/idl.ypp"
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
#line 8804 "fe/idl.tab.cpp"
    break;

  case 478: /* $@148: %empty  */
#line 5577 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 8812 "fe/idl.tab.cpp"
    break;

  case 479: /* component_inheritance_spec: ':' $@148 scoped_name  */
#line 5581 "fe/idl.ypp"
        {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8820 "fe/idl.tab.cpp"
    break;

  case 480: /* component_inheritance_spec: %empty  */
#line 5585 "fe/idl.ypp"
        {
          (yyval.idlist) = 0;
        }
#line 8828 "fe/idl.tab.cpp"
    break;

  case 481: /* component_exports: component_exports at_least_one_annotation component_export  */
#line 5592 "fe/idl.ypp"
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
#line 8847 "fe/idl.tab.cpp"
    break;

  case 484: /* $@149: %empty  */
#line 5612 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 8855 "fe/idl.tab.cpp"
    break;

  case 485: /* component_export: provides_decl $@149 ';'  */
#line 5616 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8864 "fe/idl.tab.cpp"
    break;

  case 486: /* $@150: %empty  */
#line 5621 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 8872 "fe/idl.tab.cpp"
    break;

  case 487: /* component_export: uses_decl $@150 ';'  */
#line 5625 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8881 "fe/idl.tab.cpp"
    break;

  case 488: /* $@151: %empty  */
#line 5630 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EmitsDeclSeen);
        }
#line 8889 "fe/idl.tab.cpp"
    break;

  case 489: /* component_export: emits_decl $@151 ';'  */
#line 5634 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8898 "fe/idl.tab.cpp"
    break;

  case 490: /* $@152: %empty  */
#line 5639 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PublishesDeclSeen);
        }
#line 8906 "fe/idl.tab.cpp"
    break;

  case 491: /* component_export: publishes_decl $@152 ';'  */
#line 5643 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8915 "fe/idl.tab.cpp"
    break;

  case 492: /* $@153: %empty  */
#line 5648 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConsumesDeclSeen);
        }
#line 8923 "fe/idl.tab.cpp"
    break;

  case 493: /* component_export: consumes_decl $@153 ';'  */
#line 5652 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8932 "fe/idl.tab.cpp"
    break;

  case 494: /* $@154: %empty  */
#line 5657 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 8940 "fe/idl.tab.cpp"
    break;

  case 495: /* component_export: attribute $@154 ';'  */
#line 5661 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8949 "fe/idl.tab.cpp"
    break;

  case 496: /* $@155: %empty  */
#line 5666 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 8957 "fe/idl.tab.cpp"
    break;

  case 497: /* component_export: extended_port_decl $@155 ';'  */
#line 5670 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8966 "fe/idl.tab.cpp"
    break;

  case 498: /* provides_decl: IDL_PROVIDES interface_type id  */
#line 5677 "fe/idl.ypp"
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
#line 9060 "fe/idl.tab.cpp"
    break;

  case 499: /* interface_type: scoped_name  */
#line 5770 "fe/idl.ypp"
        {
          // Lookups and checking are done where the 'interface_type'
          // token is used, in 'provides_decl' and 'uses_decl'.
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 9070 "fe/idl.tab.cpp"
    break;

  case 500: /* interface_type: IDL_OBJECT  */
#line 5776 "fe/idl.ypp"
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
#line 9097 "fe/idl.tab.cpp"
    break;

  case 501: /* uses_decl: uses_opt_multiple interface_type id  */
#line 5801 "fe/idl.ypp"
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
#line 9205 "fe/idl.tab.cpp"
    break;

  case 502: /* uses_opt_multiple: IDL_USES opt_multiple  */
#line 5908 "fe/idl.ypp"
        {
          // We use this extra rule here to use in both uses_decl and
          // extended_uses_decl, so the LALR(1) parser can avoid conflicts.
          (yyval.bval) = (yyvsp[0].bval);
        }
#line 9215 "fe/idl.tab.cpp"
    break;

  case 503: /* opt_multiple: IDL_MULTIPLE  */
#line 5917 "fe/idl.ypp"
        {
          (yyval.bval) = true;
        }
#line 9223 "fe/idl.tab.cpp"
    break;

  case 504: /* opt_multiple: %empty  */
#line 5921 "fe/idl.ypp"
        {
          (yyval.bval) = false;
        }
#line 9231 "fe/idl.tab.cpp"
    break;

  case 505: /* emits_decl: IDL_EMITS scoped_name id  */
#line 5928 "fe/idl.ypp"
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
#line 9307 "fe/idl.tab.cpp"
    break;

  case 506: /* publishes_decl: IDL_PUBLISHES scoped_name id  */
#line 6003 "fe/idl.ypp"
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
#line 9380 "fe/idl.tab.cpp"
    break;

  case 507: /* consumes_decl: IDL_CONSUMES scoped_name id  */
#line 6075 "fe/idl.ypp"
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
#line 9456 "fe/idl.tab.cpp"
    break;

  case 508: /* $@156: %empty  */
#line 6150 "fe/idl.ypp"
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
#line 9497 "fe/idl.tab.cpp"
    break;

  case 509: /* home_decl: home_header $@156 home_body  */
#line 6187 "fe/idl.ypp"
        {
          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = 0;
        }
#line 9510 "fe/idl.tab.cpp"
    break;

  case 510: /* $@157: %empty  */
#line 6199 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSeen);
        }
#line 9518 "fe/idl.tab.cpp"
    break;

  case 511: /* $@158: %empty  */
#line 6203 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeIDSeen);
        }
#line 9526 "fe/idl.tab.cpp"
    break;

  case 512: /* $@159: %empty  */
#line 6207 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 9534 "fe/idl.tab.cpp"
    break;

  case 513: /* $@160: %empty  */
#line 6211 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);
        }
#line 9542 "fe/idl.tab.cpp"
    break;

  case 514: /* $@161: %empty  */
#line 6215 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesSeen);
        }
#line 9550 "fe/idl.tab.cpp"
    break;

  case 515: /* $@162: %empty  */
#line 6219 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesIDSeen);
        }
#line 9558 "fe/idl.tab.cpp"
    break;

  case 516: /* home_header: IDL_HOME $@157 defining_id $@158 home_inheritance_spec $@159 supports_spec $@160 IDL_MANAGES $@161 scoped_name $@162 primary_key_spec  */
#line 6223 "fe/idl.ypp"
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
#line 9608 "fe/idl.tab.cpp"
    break;

  case 517: /* $@163: %empty  */
#line 6272 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 9616 "fe/idl.tab.cpp"
    break;

  case 518: /* home_inheritance_spec: ':' $@163 scoped_name  */
#line 6276 "fe/idl.ypp"
        {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 9624 "fe/idl.tab.cpp"
    break;

  case 519: /* home_inheritance_spec: %empty  */
#line 6280 "fe/idl.ypp"
        {
          (yyval.idlist) = 0;
        }
#line 9632 "fe/idl.tab.cpp"
    break;

  case 520: /* primary_key_spec: IDL_PRIMARYKEY scoped_name  */
#line 6288 "fe/idl.ypp"
        {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 9640 "fe/idl.tab.cpp"
    break;

  case 521: /* primary_key_spec: %empty  */
#line 6292 "fe/idl.ypp"
        {
          (yyval.idlist) = 0;
        }
#line 9648 "fe/idl.tab.cpp"
    break;

  case 522: /* $@164: %empty  */
#line 6299 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSqSeen);
        }
#line 9656 "fe/idl.tab.cpp"
    break;

  case 523: /* $@165: %empty  */
#line 6303 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeBodySeen);
        }
#line 9664 "fe/idl.tab.cpp"
    break;

  case 524: /* home_body: '{' $@164 home_exports $@165 '}'  */
#line 6307 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeQsSeen);
        }
#line 9672 "fe/idl.tab.cpp"
    break;

  case 528: /* $@166: %empty  */
#line 6320 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_FactoryDeclSeen);
        }
#line 9680 "fe/idl.tab.cpp"
    break;

  case 529: /* home_export: factory_decl $@166 ';'  */
#line 6324 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 9688 "fe/idl.tab.cpp"
    break;

  case 530: /* $@167: %empty  */
#line 6328 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_FinderDeclSeen);
        }
#line 9696 "fe/idl.tab.cpp"
    break;

  case 531: /* home_export: finder_decl $@167 ';'  */
#line 6332 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 9704 "fe/idl.tab.cpp"
    break;

  case 532: /* $@168: %empty  */
#line 6340 "fe/idl.ypp"
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
#line 9731 "fe/idl.tab.cpp"
    break;

  case 533: /* $@169: %empty  */
#line 6363 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 9739 "fe/idl.tab.cpp"
    break;

  case 534: /* factory_decl: IDL_FACTORY defining_id $@168 init_parameter_list $@169 opt_raises  */
#line 6367 "fe/idl.ypp"
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
#line 9761 "fe/idl.tab.cpp"
    break;

  case 535: /* $@170: %empty  */
#line 6389 "fe/idl.ypp"
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
#line 9792 "fe/idl.tab.cpp"
    break;

  case 536: /* $@171: %empty  */
#line 6416 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 9800 "fe/idl.tab.cpp"
    break;

  case 537: /* finder_decl: IDL_FINDER defining_id $@170 init_parameter_list $@171 opt_raises  */
#line 6420 "fe/idl.ypp"
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
#line 9822 "fe/idl.tab.cpp"
    break;

  case 543: /* event_concrete_forward_decl: IDL_EVENTTYPE defining_id  */
#line 6453 "fe/idl.ypp"
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
#line 9851 "fe/idl.tab.cpp"
    break;

  case 544: /* event_abs_forward_decl: IDL_ABSTRACT IDL_EVENTTYPE defining_id  */
#line 6483 "fe/idl.ypp"
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
#line 9880 "fe/idl.tab.cpp"
    break;

  case 545: /* $@172: %empty  */
#line 6512 "fe/idl.ypp"
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
#line 9928 "fe/idl.tab.cpp"
    break;

  case 546: /* $@173: %empty  */
#line 6556 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 9936 "fe/idl.tab.cpp"
    break;

  case 547: /* $@174: %empty  */
#line 6560 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 9944 "fe/idl.tab.cpp"
    break;

  case 548: /* event_abs_decl: event_abs_header event_rest_of_header $@172 '{' $@173 exports $@174 '}'  */
#line 6564 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = 0;
        }
#line 9959 "fe/idl.tab.cpp"
    break;

  case 549: /* event_abs_header: IDL_ABSTRACT IDL_EVENTTYPE defining_id  */
#line 6580 "fe/idl.ypp"
        {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9967 "fe/idl.tab.cpp"
    break;

  case 550: /* event_custom_header: IDL_CUSTOM IDL_EVENTTYPE defining_id  */
#line 6589 "fe/idl.ypp"
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
#line 9984 "fe/idl.tab.cpp"
    break;

  case 551: /* event_plain_header: IDL_EVENTTYPE defining_id  */
#line 6606 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9994 "fe/idl.tab.cpp"
    break;

  case 552: /* $@175: %empty  */
#line 6615 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 10002 "fe/idl.tab.cpp"
    break;

  case 553: /* event_rest_of_header: inheritance_spec $@175 supports_spec  */
#line 6619 "fe/idl.ypp"
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
#line 10035 "fe/idl.tab.cpp"
    break;

  case 554: /* @176: %empty  */
#line 6652 "fe/idl.ypp"
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
#line 10090 "fe/idl.tab.cpp"
    break;

  case 555: /* $@177: %empty  */
#line 6703 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 10098 "fe/idl.tab.cpp"
    break;

  case 556: /* $@178: %empty  */
#line 6707 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 10106 "fe/idl.tab.cpp"
    break;

  case 557: /* event_decl: event_header event_rest_of_header @176 '{' $@177 value_elements $@178 '}'  */
#line 6711 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 10121 "fe/idl.tab.cpp"
    break;

  case 558: /* event_header: event_custom_header  */
#line 6725 "fe/idl.ypp"
        {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 10129 "fe/idl.tab.cpp"
    break;

  case 559: /* event_header: event_plain_header  */
#line 6729 "fe/idl.ypp"
        {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 10137 "fe/idl.tab.cpp"
    break;

  case 560: /* formal_parameter_type: IDL_TYPENAME  */
#line 6736 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_type;
        }
#line 10145 "fe/idl.tab.cpp"
    break;

  case 561: /* formal_parameter_type: IDL_STRUCT  */
#line 6740 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_struct;
        }
#line 10153 "fe/idl.tab.cpp"
    break;

  case 562: /* formal_parameter_type: IDL_UNION  */
#line 6744 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_union;
        }
#line 10161 "fe/idl.tab.cpp"
    break;

  case 563: /* formal_parameter_type: IDL_EVENTTYPE  */
#line 6748 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_eventtype;
        }
#line 10169 "fe/idl.tab.cpp"
    break;

  case 564: /* formal_parameter_type: IDL_SEQUENCE  */
#line 6752 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_sequence;
        }
#line 10177 "fe/idl.tab.cpp"
    break;

  case 565: /* formal_parameter_type: IDL_INTERFACE  */
#line 6756 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_interface;
        }
#line 10185 "fe/idl.tab.cpp"
    break;

  case 566: /* formal_parameter_type: IDL_VALUETYPE  */
#line 6760 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_valuetype;
        }
#line 10193 "fe/idl.tab.cpp"
    break;

  case 567: /* formal_parameter_type: IDL_ENUM  */
#line 6764 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_enum;
        }
#line 10201 "fe/idl.tab.cpp"
    break;

  case 568: /* formal_parameter_type: IDL_EXCEPTION  */
#line 6768 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_except;
        }
#line 10209 "fe/idl.tab.cpp"
    break;

  case 569: /* formal_parameter_type: IDL_CONST const_type  */
#line 6772 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_const;
          t_param_const_type = (yyvsp[0].etval);
        }
#line 10218 "fe/idl.tab.cpp"
    break;

  case 570: /* at_least_one_formal_parameter: formal_parameter formal_parameters  */
#line 6780 "fe/idl.ypp"
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
#line 10250 "fe/idl.tab.cpp"
    break;

  case 571: /* formal_parameters: formal_parameters ',' formal_parameter  */
#line 6811 "fe/idl.ypp"
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
#line 10267 "fe/idl.tab.cpp"
    break;

  case 572: /* formal_parameters: %empty  */
#line 6824 "fe/idl.ypp"
        {
          (yyval.plval) = 0;
        }
#line 10275 "fe/idl.tab.cpp"
    break;

  case 573: /* formal_parameter: formal_parameter_type IDENTIFIER  */
#line 6831 "fe/idl.ypp"
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
#line 10304 "fe/idl.tab.cpp"
    break;

  case 574: /* formal_parameter: IDL_SEQUENCE '<' IDENTIFIER '>' IDENTIFIER  */
#line 6856 "fe/idl.ypp"
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
#line 10323 "fe/idl.tab.cpp"
    break;

  case 575: /* at_least_one_formal_parameter_name: formal_parameter_name formal_parameter_names  */
#line 6874 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.slval), UTL_StrList ((yyvsp[-1].sval), (yyvsp[0].slval)), 1);
        }
#line 10331 "fe/idl.tab.cpp"
    break;

  case 576: /* formal_parameter_names: formal_parameter_names ',' formal_parameter_name  */
#line 6881 "fe/idl.ypp"
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
#line 10350 "fe/idl.tab.cpp"
    break;

  case 577: /* formal_parameter_names: %empty  */
#line 6896 "fe/idl.ypp"
        {
          (yyval.slval) = 0;
        }
#line 10358 "fe/idl.tab.cpp"
    break;

  case 578: /* formal_parameter_name: IDENTIFIER  */
#line 6903 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.sval),
                          UTL_String ((yyvsp[0].strval), true),
                          1);
        }
#line 10368 "fe/idl.tab.cpp"
    break;

  case 579: /* $@179: %empty  */
#line 6912 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSeen);
        }
#line 10376 "fe/idl.tab.cpp"
    break;

  case 580: /* @180: %empty  */
#line 6916 "fe/idl.ypp"
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
#line 10399 "fe/idl.tab.cpp"
    break;

  case 581: /* $@181: %empty  */
#line 6935 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSqSeen);
        }
#line 10407 "fe/idl.tab.cpp"
    break;

  case 582: /* $@182: %empty  */
#line 6943 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeBodySeen);
        }
#line 10415 "fe/idl.tab.cpp"
    break;

  case 583: /* porttype_decl: IDL_PORTTYPE $@179 IDENTIFIER @180 '{' $@181 at_least_one_port_export $@182 '}'  */
#line 6947 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeQsSeen);

          // Done with this port type - pop it off the scopes stack.
          idl_global->scopes ().pop ();

          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 10428 "fe/idl.tab.cpp"
    break;

  case 584: /* at_least_one_port_export: port_exports at_least_one_annotation port_export  */
#line 6959 "fe/idl.ypp"
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
#line 10447 "fe/idl.tab.cpp"
    break;

  case 590: /* $@183: %empty  */
#line 6985 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 10455 "fe/idl.tab.cpp"
    break;

  case 591: /* port_export: attribute $@183 ';'  */
#line 6989 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 10463 "fe/idl.tab.cpp"
    break;

  case 592: /* extended_port_decl: IDL_PORT scoped_name IDENTIFIER  */
#line 6996 "fe/idl.ypp"
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
#line 10534 "fe/idl.tab.cpp"
    break;

  case 593: /* extended_port_decl: IDL_MIRRORPORT scoped_name IDENTIFIER  */
#line 7063 "fe/idl.ypp"
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
#line 10583 "fe/idl.tab.cpp"
    break;

  case 594: /* at_least_one_actual_parameter: annotations_maybe actual_parameter actual_parameters  */
#line 7111 "fe/idl.ypp"
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
#line 10600 "fe/idl.tab.cpp"
    break;

  case 595: /* actual_parameters: actual_parameters ',' annotations_maybe actual_parameter  */
#line 7127 "fe/idl.ypp"
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
#line 10617 "fe/idl.tab.cpp"
    break;

  case 596: /* actual_parameters: %empty  */
#line 7140 "fe/idl.ypp"
        {
          (yyval.alval) = 0;
        }
#line 10625 "fe/idl.tab.cpp"
    break;

  case 597: /* actual_parameter: expression  */
#line 7147 "fe/idl.ypp"
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
#line 10685 "fe/idl.tab.cpp"
    break;

  case 598: /* connector_decl: connector_header connector_body  */
#line 7206 "fe/idl.ypp"
        {
          (yyval.dcval) = 0;
        }
#line 10693 "fe/idl.tab.cpp"
    break;

  case 599: /* $@184: %empty  */
#line 7213 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSeen);
        }
#line 10701 "fe/idl.tab.cpp"
    break;

  case 600: /* $@185: %empty  */
#line 7217 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorIDSeen);
        }
#line 10709 "fe/idl.tab.cpp"
    break;

  case 601: /* connector_header: IDL_CONNECTOR $@184 annotations_maybe IDENTIFIER $@185 component_inheritance_spec  */
#line 7221 "fe/idl.ypp"
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
#line 10767 "fe/idl.tab.cpp"
    break;

  case 602: /* $@186: %empty  */
#line 7278 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSqSeen);
        }
#line 10775 "fe/idl.tab.cpp"
    break;

  case 603: /* $@187: %empty  */
#line 7282 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorBodySeen);
        }
#line 10783 "fe/idl.tab.cpp"
    break;

  case 604: /* connector_body: '{' $@186 connector_exports $@187 '}'  */
#line 7286 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorQsSeen);

          // Done with this connector - pop it off the scope stack.
          idl_global->scopes ().pop ();
        }
#line 10794 "fe/idl.tab.cpp"
    break;

  case 607: /* $@188: %empty  */
#line 7301 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 10802 "fe/idl.tab.cpp"
    break;

  case 608: /* connector_export: provides_decl $@188 ';'  */
#line 7305 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10810 "fe/idl.tab.cpp"
    break;

  case 609: /* $@189: %empty  */
#line 7309 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 10818 "fe/idl.tab.cpp"
    break;

  case 610: /* connector_export: uses_decl $@189 ';'  */
#line 7313 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10826 "fe/idl.tab.cpp"
    break;

  case 611: /* $@190: %empty  */
#line 7317 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 10834 "fe/idl.tab.cpp"
    break;

  case 612: /* connector_export: attribute $@190 ';'  */
#line 7321 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10842 "fe/idl.tab.cpp"
    break;

  case 613: /* $@191: %empty  */
#line 7325 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 10850 "fe/idl.tab.cpp"
    break;

  case 614: /* connector_export: extended_port_decl $@191 ';'  */
#line 7329 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10858 "fe/idl.tab.cpp"
    break;


#line 10862 "fe/idl.tab.cpp"

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

#line 7334 "fe/idl.ypp"

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
