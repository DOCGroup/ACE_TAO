/* A Bison parser, made by GNU Bison 3.7.6.  */

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
#define YYBISON 30706

/* Bison version string.  */
#define YYBISON_VERSION "3.7.6"

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
  YYSYMBOL_96_ = 96,                       /* ';'  */
  YYSYMBOL_97_ = 97,                       /* '{'  */
  YYSYMBOL_98_ = 98,                       /* '}'  */
  YYSYMBOL_99_ = 99,                       /* '<'  */
  YYSYMBOL_100_ = 100,                     /* '>'  */
  YYSYMBOL_101_ = 101,                     /* ':'  */
  YYSYMBOL_102_ = 102,                     /* ','  */
  YYSYMBOL_103_ = 103,                     /* '='  */
  YYSYMBOL_104_ = 104,                     /* '|'  */
  YYSYMBOL_105_ = 105,                     /* '^'  */
  YYSYMBOL_106_ = 106,                     /* '&'  */
  YYSYMBOL_107_ = 107,                     /* '+'  */
  YYSYMBOL_108_ = 108,                     /* '-'  */
  YYSYMBOL_109_ = 109,                     /* '*'  */
  YYSYMBOL_110_ = 110,                     /* '/'  */
  YYSYMBOL_111_ = 111,                     /* '%'  */
  YYSYMBOL_112_ = 112,                     /* '~'  */
  YYSYMBOL_113_ = 113,                     /* '('  */
  YYSYMBOL_114_ = 114,                     /* ')'  */
  YYSYMBOL_115_ = 115,                     /* '['  */
  YYSYMBOL_116_ = 116,                     /* ']'  */
  YYSYMBOL_YYACCEPT = 117,                 /* $accept  */
  YYSYMBOL_start = 118,                    /* start  */
  YYSYMBOL_definitions = 119,              /* definitions  */
  YYSYMBOL_at_least_one_definition = 120,  /* at_least_one_definition  */
  YYSYMBOL_definition = 121,               /* definition  */
  YYSYMBOL_fixed_definition = 122,         /* fixed_definition  */
  YYSYMBOL_123_1 = 123,                    /* $@1  */
  YYSYMBOL_124_2 = 124,                    /* $@2  */
  YYSYMBOL_125_3 = 125,                    /* $@3  */
  YYSYMBOL_126_4 = 126,                    /* $@4  */
  YYSYMBOL_127_5 = 127,                    /* $@5  */
  YYSYMBOL_128_6 = 128,                    /* $@6  */
  YYSYMBOL_129_7 = 129,                    /* $@7  */
  YYSYMBOL_130_8 = 130,                    /* $@8  */
  YYSYMBOL_131_9 = 131,                    /* $@9  */
  YYSYMBOL_132_10 = 132,                   /* $@10  */
  YYSYMBOL_133_11 = 133,                   /* $@11  */
  YYSYMBOL_134_12 = 134,                   /* $@12  */
  YYSYMBOL_135_13 = 135,                   /* $@13  */
  YYSYMBOL_136_14 = 136,                   /* $@14  */
  YYSYMBOL_137_15 = 137,                   /* $@15  */
  YYSYMBOL_module_header = 138,            /* module_header  */
  YYSYMBOL_139_16 = 139,                   /* $@16  */
  YYSYMBOL_module = 140,                   /* module  */
  YYSYMBOL_141_17 = 141,                   /* @17  */
  YYSYMBOL_142_18 = 142,                   /* $@18  */
  YYSYMBOL_143_19 = 143,                   /* $@19  */
  YYSYMBOL_template_module_header = 144,   /* template_module_header  */
  YYSYMBOL_template_module = 145,          /* template_module  */
  YYSYMBOL_146_20 = 146,                   /* $@20  */
  YYSYMBOL_147_21 = 147,                   /* $@21  */
  YYSYMBOL_148_22 = 148,                   /* $@22  */
  YYSYMBOL_149_23 = 149,                   /* $@23  */
  YYSYMBOL_150_24 = 150,                   /* $@24  */
  YYSYMBOL_at_least_one_tpl_definition = 151, /* at_least_one_tpl_definition  */
  YYSYMBOL_tpl_definitions = 152,          /* tpl_definitions  */
  YYSYMBOL_tpl_definition = 153,           /* tpl_definition  */
  YYSYMBOL_template_module_ref = 154,      /* template_module_ref  */
  YYSYMBOL_155_25 = 155,                   /* $@25  */
  YYSYMBOL_156_26 = 156,                   /* $@26  */
  YYSYMBOL_template_module_inst = 157,     /* template_module_inst  */
  YYSYMBOL_158_27 = 158,                   /* $@27  */
  YYSYMBOL_159_28 = 159,                   /* $@28  */
  YYSYMBOL_interface_def = 160,            /* interface_def  */
  YYSYMBOL_interface = 161,                /* interface  */
  YYSYMBOL_162_29 = 162,                   /* $@29  */
  YYSYMBOL_163_30 = 163,                   /* $@30  */
  YYSYMBOL_164_31 = 164,                   /* $@31  */
  YYSYMBOL_interface_decl = 165,           /* interface_decl  */
  YYSYMBOL_166_32 = 166,                   /* $@32  */
  YYSYMBOL_interface_header = 167,         /* interface_header  */
  YYSYMBOL_inheritance_spec = 168,         /* inheritance_spec  */
  YYSYMBOL_169_33 = 169,                   /* $@33  */
  YYSYMBOL_value_def = 170,                /* value_def  */
  YYSYMBOL_valuetype = 171,                /* valuetype  */
  YYSYMBOL_value_concrete_decl = 172,      /* value_concrete_decl  */
  YYSYMBOL_173_34 = 173,                   /* @34  */
  YYSYMBOL_174_35 = 174,                   /* $@35  */
  YYSYMBOL_175_36 = 175,                   /* $@36  */
  YYSYMBOL_value_abs_decl = 176,           /* value_abs_decl  */
  YYSYMBOL_177_37 = 177,                   /* $@37  */
  YYSYMBOL_178_38 = 178,                   /* $@38  */
  YYSYMBOL_179_39 = 179,                   /* $@39  */
  YYSYMBOL_value_header = 180,             /* value_header  */
  YYSYMBOL_181_40 = 181,                   /* $@40  */
  YYSYMBOL_value_decl = 182,               /* value_decl  */
  YYSYMBOL_183_41 = 183,                   /* $@41  */
  YYSYMBOL_opt_truncatable = 184,          /* opt_truncatable  */
  YYSYMBOL_supports_spec = 185,            /* supports_spec  */
  YYSYMBOL_value_forward_decl = 186,       /* value_forward_decl  */
  YYSYMBOL_value_box_decl = 187,           /* value_box_decl  */
  YYSYMBOL_value_elements = 188,           /* value_elements  */
  YYSYMBOL_value_element = 189,            /* value_element  */
  YYSYMBOL_190_42 = 190,                   /* @42  */
  YYSYMBOL_visibility = 191,               /* visibility  */
  YYSYMBOL_state_member = 192,             /* state_member  */
  YYSYMBOL_exports = 193,                  /* exports  */
  YYSYMBOL_at_least_one_export = 194,      /* at_least_one_export  */
  YYSYMBOL_export = 195,                   /* export  */
  YYSYMBOL_196_43 = 196,                   /* $@43  */
  YYSYMBOL_197_44 = 197,                   /* $@44  */
  YYSYMBOL_198_45 = 198,                   /* $@45  */
  YYSYMBOL_199_46 = 199,                   /* $@46  */
  YYSYMBOL_200_47 = 200,                   /* $@47  */
  YYSYMBOL_201_48 = 201,                   /* $@48  */
  YYSYMBOL_202_49 = 202,                   /* $@49  */
  YYSYMBOL_203_50 = 203,                   /* $@50  */
  YYSYMBOL_at_least_one_scoped_name = 204, /* at_least_one_scoped_name  */
  YYSYMBOL_scoped_names = 205,             /* scoped_names  */
  YYSYMBOL_206_51 = 206,                   /* $@51  */
  YYSYMBOL_scoped_name = 207,              /* scoped_name  */
  YYSYMBOL_208_52 = 208,                   /* $@52  */
  YYSYMBOL_209_53 = 209,                   /* $@53  */
  YYSYMBOL_id = 210,                       /* id  */
  YYSYMBOL_defining_id = 211,              /* defining_id  */
  YYSYMBOL_interface_forward = 212,        /* interface_forward  */
  YYSYMBOL_const_dcl = 213,                /* const_dcl  */
  YYSYMBOL_214_54 = 214,                   /* $@54  */
  YYSYMBOL_215_55 = 215,                   /* $@55  */
  YYSYMBOL_216_56 = 216,                   /* $@56  */
  YYSYMBOL_217_57 = 217,                   /* $@57  */
  YYSYMBOL_const_type = 218,               /* const_type  */
  YYSYMBOL_expression = 219,               /* expression  */
  YYSYMBOL_const_expr = 220,               /* const_expr  */
  YYSYMBOL_or_expr = 221,                  /* or_expr  */
  YYSYMBOL_xor_expr = 222,                 /* xor_expr  */
  YYSYMBOL_and_expr = 223,                 /* and_expr  */
  YYSYMBOL_shift_expr = 224,               /* shift_expr  */
  YYSYMBOL_add_expr = 225,                 /* add_expr  */
  YYSYMBOL_mult_expr = 226,                /* mult_expr  */
  YYSYMBOL_unary_expr = 227,               /* unary_expr  */
  YYSYMBOL_primary_expr = 228,             /* primary_expr  */
  YYSYMBOL_literal = 229,                  /* literal  */
  YYSYMBOL_positive_int_expr = 230,        /* positive_int_expr  */
  YYSYMBOL_annotation_dcl = 231,           /* annotation_dcl  */
  YYSYMBOL_232_58 = 232,                   /* $@58  */
  YYSYMBOL_annotation_body = 233,          /* annotation_body  */
  YYSYMBOL_annotation_statement = 234,     /* annotation_statement  */
  YYSYMBOL_235_59 = 235,                   /* $@59  */
  YYSYMBOL_annotation_member_type = 236,   /* annotation_member_type  */
  YYSYMBOL_annotation_member = 237,        /* annotation_member  */
  YYSYMBOL_annotation_member_default = 238, /* annotation_member_default  */
  YYSYMBOL_at_least_one_annotation = 239,  /* at_least_one_annotation  */
  YYSYMBOL_annotations_maybe = 240,        /* annotations_maybe  */
  YYSYMBOL_annotation_appl = 241,          /* annotation_appl  */
  YYSYMBOL_242_60 = 242,                   /* @60  */
  YYSYMBOL_annotation_appl_params_maybe = 243, /* annotation_appl_params_maybe  */
  YYSYMBOL_annotation_appl_params = 244,   /* annotation_appl_params  */
  YYSYMBOL_named_annotation_appl_params = 245, /* named_annotation_appl_params  */
  YYSYMBOL_more_named_annotation_appl_params = 246, /* more_named_annotation_appl_params  */
  YYSYMBOL_named_annotation_appl_param = 247, /* named_annotation_appl_param  */
  YYSYMBOL_type_dcl = 248,                 /* type_dcl  */
  YYSYMBOL_249_61 = 249,                   /* $@61  */
  YYSYMBOL_type_declarator = 250,          /* type_declarator  */
  YYSYMBOL_251_62 = 251,                   /* $@62  */
  YYSYMBOL_type_spec = 252,                /* type_spec  */
  YYSYMBOL_simple_type_spec = 253,         /* simple_type_spec  */
  YYSYMBOL_base_type_spec = 254,           /* base_type_spec  */
  YYSYMBOL_template_type_spec = 255,       /* template_type_spec  */
  YYSYMBOL_constructed_type_spec = 256,    /* constructed_type_spec  */
  YYSYMBOL_constructed_forward_type_spec = 257, /* constructed_forward_type_spec  */
  YYSYMBOL_at_least_one_declarator = 258,  /* at_least_one_declarator  */
  YYSYMBOL_declarators = 259,              /* declarators  */
  YYSYMBOL_260_63 = 260,                   /* $@63  */
  YYSYMBOL_declarator = 261,               /* declarator  */
  YYSYMBOL_at_least_one_simple_declarator = 262, /* at_least_one_simple_declarator  */
  YYSYMBOL_simple_declarators = 263,       /* simple_declarators  */
  YYSYMBOL_264_64 = 264,                   /* $@64  */
  YYSYMBOL_simple_declarator = 265,        /* simple_declarator  */
  YYSYMBOL_complex_declarator = 266,       /* complex_declarator  */
  YYSYMBOL_integer_type = 267,             /* integer_type  */
  YYSYMBOL_signed_int = 268,               /* signed_int  */
  YYSYMBOL_unsigned_int = 269,             /* unsigned_int  */
  YYSYMBOL_floating_pt_type = 270,         /* floating_pt_type  */
  YYSYMBOL_fixed_type = 271,               /* fixed_type  */
  YYSYMBOL_char_type = 272,                /* char_type  */
  YYSYMBOL_octet_type = 273,               /* octet_type  */
  YYSYMBOL_boolean_type = 274,             /* boolean_type  */
  YYSYMBOL_any_type = 275,                 /* any_type  */
  YYSYMBOL_object_type = 276,              /* object_type  */
  YYSYMBOL_struct_decl = 277,              /* struct_decl  */
  YYSYMBOL_278_65 = 278,                   /* $@65  */
  YYSYMBOL_struct_type = 279,              /* struct_type  */
  YYSYMBOL_280_66 = 280,                   /* $@66  */
  YYSYMBOL_281_67 = 281,                   /* $@67  */
  YYSYMBOL_282_68 = 282,                   /* $@68  */
  YYSYMBOL_at_least_one_member = 283,      /* at_least_one_member  */
  YYSYMBOL_members = 284,                  /* members  */
  YYSYMBOL_member = 285,                   /* member  */
  YYSYMBOL_member_i = 286,                 /* member_i  */
  YYSYMBOL_287_69 = 287,                   /* $@69  */
  YYSYMBOL_288_70 = 288,                   /* $@70  */
  YYSYMBOL_289_71 = 289,                   /* $@71  */
  YYSYMBOL_union_decl = 290,               /* union_decl  */
  YYSYMBOL_291_72 = 291,                   /* $@72  */
  YYSYMBOL_union_type = 292,               /* union_type  */
  YYSYMBOL_293_73 = 293,                   /* $@73  */
  YYSYMBOL_294_74 = 294,                   /* $@74  */
  YYSYMBOL_295_75 = 295,                   /* $@75  */
  YYSYMBOL_296_76 = 296,                   /* $@76  */
  YYSYMBOL_297_77 = 297,                   /* $@77  */
  YYSYMBOL_298_78 = 298,                   /* $@78  */
  YYSYMBOL_switch_type_spec = 299,         /* switch_type_spec  */
  YYSYMBOL_at_least_one_case_branch = 300, /* at_least_one_case_branch  */
  YYSYMBOL_case_branches = 301,            /* case_branches  */
  YYSYMBOL_case_branch = 302,              /* case_branch  */
  YYSYMBOL_303_79 = 303,                   /* $@79  */
  YYSYMBOL_304_80 = 304,                   /* $@80  */
  YYSYMBOL_305_81 = 305,                   /* $@81  */
  YYSYMBOL_at_least_one_case_label = 306,  /* at_least_one_case_label  */
  YYSYMBOL_case_labels = 307,              /* case_labels  */
  YYSYMBOL_case_label = 308,               /* case_label  */
  YYSYMBOL_309_82 = 309,                   /* $@82  */
  YYSYMBOL_310_83 = 310,                   /* $@83  */
  YYSYMBOL_311_84 = 311,                   /* $@84  */
  YYSYMBOL_element_spec = 312,             /* element_spec  */
  YYSYMBOL_313_85 = 313,                   /* $@85  */
  YYSYMBOL_struct_forward_type = 314,      /* struct_forward_type  */
  YYSYMBOL_union_forward_type = 315,       /* union_forward_type  */
  YYSYMBOL_enum_type = 316,                /* enum_type  */
  YYSYMBOL_317_86 = 317,                   /* $@86  */
  YYSYMBOL_318_87 = 318,                   /* $@87  */
  YYSYMBOL_319_88 = 319,                   /* $@88  */
  YYSYMBOL_320_89 = 320,                   /* $@89  */
  YYSYMBOL_at_least_one_enumerator = 321,  /* at_least_one_enumerator  */
  YYSYMBOL_enumerators = 322,              /* enumerators  */
  YYSYMBOL_323_90 = 323,                   /* $@90  */
  YYSYMBOL_enumerator = 324,               /* enumerator  */
  YYSYMBOL_sequence_type_spec = 325,       /* sequence_type_spec  */
  YYSYMBOL_326_91 = 326,                   /* $@91  */
  YYSYMBOL_327_92 = 327,                   /* $@92  */
  YYSYMBOL_seq_head = 328,                 /* seq_head  */
  YYSYMBOL_329_93 = 329,                   /* $@93  */
  YYSYMBOL_330_94 = 330,                   /* $@94  */
  YYSYMBOL_fixed_type_spec = 331,          /* fixed_type_spec  */
  YYSYMBOL_string_type_spec = 332,         /* string_type_spec  */
  YYSYMBOL_333_95 = 333,                   /* $@95  */
  YYSYMBOL_334_96 = 334,                   /* $@96  */
  YYSYMBOL_string_head = 335,              /* string_head  */
  YYSYMBOL_wstring_type_spec = 336,        /* wstring_type_spec  */
  YYSYMBOL_337_97 = 337,                   /* $@97  */
  YYSYMBOL_338_98 = 338,                   /* $@98  */
  YYSYMBOL_wstring_head = 339,             /* wstring_head  */
  YYSYMBOL_array_declarator = 340,         /* array_declarator  */
  YYSYMBOL_341_99 = 341,                   /* $@99  */
  YYSYMBOL_at_least_one_array_dim = 342,   /* at_least_one_array_dim  */
  YYSYMBOL_array_dims = 343,               /* array_dims  */
  YYSYMBOL_array_dim = 344,                /* array_dim  */
  YYSYMBOL_345_100 = 345,                  /* $@100  */
  YYSYMBOL_346_101 = 346,                  /* $@101  */
  YYSYMBOL_attribute = 347,                /* attribute  */
  YYSYMBOL_attribute_readonly = 348,       /* attribute_readonly  */
  YYSYMBOL_349_102 = 349,                  /* $@102  */
  YYSYMBOL_350_103 = 350,                  /* $@103  */
  YYSYMBOL_351_104 = 351,                  /* $@104  */
  YYSYMBOL_352_105 = 352,                  /* $@105  */
  YYSYMBOL_attribute_readwrite = 353,      /* attribute_readwrite  */
  YYSYMBOL_354_106 = 354,                  /* $@106  */
  YYSYMBOL_355_107 = 355,                  /* $@107  */
  YYSYMBOL_356_108 = 356,                  /* $@108  */
  YYSYMBOL_357_109 = 357,                  /* $@109  */
  YYSYMBOL_exception = 358,                /* exception  */
  YYSYMBOL_359_110 = 359,                  /* $@110  */
  YYSYMBOL_360_111 = 360,                  /* @111  */
  YYSYMBOL_361_112 = 361,                  /* $@112  */
  YYSYMBOL_362_113 = 362,                  /* $@113  */
  YYSYMBOL_operation = 363,                /* operation  */
  YYSYMBOL_364_114 = 364,                  /* $@114  */
  YYSYMBOL_365_115 = 365,                  /* $@115  */
  YYSYMBOL_366_116 = 366,                  /* $@116  */
  YYSYMBOL_367_117 = 367,                  /* $@117  */
  YYSYMBOL_opt_op_attribute = 368,         /* opt_op_attribute  */
  YYSYMBOL_op_type_spec = 369,             /* op_type_spec  */
  YYSYMBOL_init_decl = 370,                /* init_decl  */
  YYSYMBOL_371_118 = 371,                  /* $@118  */
  YYSYMBOL_372_119 = 372,                  /* @119  */
  YYSYMBOL_373_120 = 373,                  /* $@120  */
  YYSYMBOL_init_parameter_list = 374,      /* init_parameter_list  */
  YYSYMBOL_375_121 = 375,                  /* $@121  */
  YYSYMBOL_376_122 = 376,                  /* $@122  */
  YYSYMBOL_at_least_one_in_parameter = 377, /* at_least_one_in_parameter  */
  YYSYMBOL_in_parameters = 378,            /* in_parameters  */
  YYSYMBOL_379_123 = 379,                  /* $@123  */
  YYSYMBOL_in_parameter = 380,             /* in_parameter  */
  YYSYMBOL_381_124 = 381,                  /* $@124  */
  YYSYMBOL_382_125 = 382,                  /* $@125  */
  YYSYMBOL_parameter_list = 383,           /* parameter_list  */
  YYSYMBOL_384_126 = 384,                  /* $@126  */
  YYSYMBOL_385_127 = 385,                  /* $@127  */
  YYSYMBOL_at_least_one_parameter = 386,   /* at_least_one_parameter  */
  YYSYMBOL_parameters = 387,               /* parameters  */
  YYSYMBOL_388_128 = 388,                  /* $@128  */
  YYSYMBOL_parameter = 389,                /* parameter  */
  YYSYMBOL_390_129 = 390,                  /* $@129  */
  YYSYMBOL_391_130 = 391,                  /* $@130  */
  YYSYMBOL_param_type_spec = 392,          /* param_type_spec  */
  YYSYMBOL_direction = 393,                /* direction  */
  YYSYMBOL_opt_raises = 394,               /* opt_raises  */
  YYSYMBOL_395_131 = 395,                  /* $@131  */
  YYSYMBOL_396_132 = 396,                  /* $@132  */
  YYSYMBOL_opt_getraises = 397,            /* opt_getraises  */
  YYSYMBOL_398_133 = 398,                  /* $@133  */
  YYSYMBOL_399_134 = 399,                  /* $@134  */
  YYSYMBOL_opt_setraises = 400,            /* opt_setraises  */
  YYSYMBOL_401_135 = 401,                  /* $@135  */
  YYSYMBOL_402_136 = 402,                  /* $@136  */
  YYSYMBOL_opt_context = 403,              /* opt_context  */
  YYSYMBOL_404_137 = 404,                  /* $@137  */
  YYSYMBOL_405_138 = 405,                  /* $@138  */
  YYSYMBOL_at_least_one_string_literal = 406, /* at_least_one_string_literal  */
  YYSYMBOL_string_literals = 407,          /* string_literals  */
  YYSYMBOL_408_139 = 408,                  /* $@139  */
  YYSYMBOL_typeid_dcl = 409,               /* typeid_dcl  */
  YYSYMBOL_typeprefix_dcl = 410,           /* typeprefix_dcl  */
  YYSYMBOL_component = 411,                /* component  */
  YYSYMBOL_component_forward_decl = 412,   /* component_forward_decl  */
  YYSYMBOL_component_decl = 413,           /* component_decl  */
  YYSYMBOL_414_140 = 414,                  /* @140  */
  YYSYMBOL_415_141 = 415,                  /* $@141  */
  YYSYMBOL_416_142 = 416,                  /* $@142  */
  YYSYMBOL_component_header = 417,         /* component_header  */
  YYSYMBOL_418_143 = 418,                  /* $@143  */
  YYSYMBOL_419_144 = 419,                  /* $@144  */
  YYSYMBOL_component_inheritance_spec = 420, /* component_inheritance_spec  */
  YYSYMBOL_421_145 = 421,                  /* $@145  */
  YYSYMBOL_component_exports = 422,        /* component_exports  */
  YYSYMBOL_component_export = 423,         /* component_export  */
  YYSYMBOL_424_146 = 424,                  /* $@146  */
  YYSYMBOL_425_147 = 425,                  /* $@147  */
  YYSYMBOL_426_148 = 426,                  /* $@148  */
  YYSYMBOL_427_149 = 427,                  /* $@149  */
  YYSYMBOL_428_150 = 428,                  /* $@150  */
  YYSYMBOL_429_151 = 429,                  /* $@151  */
  YYSYMBOL_430_152 = 430,                  /* $@152  */
  YYSYMBOL_provides_decl = 431,            /* provides_decl  */
  YYSYMBOL_interface_type = 432,           /* interface_type  */
  YYSYMBOL_uses_decl = 433,                /* uses_decl  */
  YYSYMBOL_uses_opt_multiple = 434,        /* uses_opt_multiple  */
  YYSYMBOL_opt_multiple = 435,             /* opt_multiple  */
  YYSYMBOL_emits_decl = 436,               /* emits_decl  */
  YYSYMBOL_publishes_decl = 437,           /* publishes_decl  */
  YYSYMBOL_consumes_decl = 438,            /* consumes_decl  */
  YYSYMBOL_home_decl = 439,                /* home_decl  */
  YYSYMBOL_440_153 = 440,                  /* $@153  */
  YYSYMBOL_home_header = 441,              /* home_header  */
  YYSYMBOL_442_154 = 442,                  /* $@154  */
  YYSYMBOL_443_155 = 443,                  /* $@155  */
  YYSYMBOL_444_156 = 444,                  /* $@156  */
  YYSYMBOL_445_157 = 445,                  /* $@157  */
  YYSYMBOL_446_158 = 446,                  /* $@158  */
  YYSYMBOL_447_159 = 447,                  /* $@159  */
  YYSYMBOL_home_inheritance_spec = 448,    /* home_inheritance_spec  */
  YYSYMBOL_449_160 = 449,                  /* $@160  */
  YYSYMBOL_primary_key_spec = 450,         /* primary_key_spec  */
  YYSYMBOL_home_body = 451,                /* home_body  */
  YYSYMBOL_452_161 = 452,                  /* $@161  */
  YYSYMBOL_453_162 = 453,                  /* $@162  */
  YYSYMBOL_home_exports = 454,             /* home_exports  */
  YYSYMBOL_home_export = 455,              /* home_export  */
  YYSYMBOL_456_163 = 456,                  /* $@163  */
  YYSYMBOL_457_164 = 457,                  /* $@164  */
  YYSYMBOL_factory_decl = 458,             /* factory_decl  */
  YYSYMBOL_459_165 = 459,                  /* $@165  */
  YYSYMBOL_460_166 = 460,                  /* $@166  */
  YYSYMBOL_finder_decl = 461,              /* finder_decl  */
  YYSYMBOL_462_167 = 462,                  /* $@167  */
  YYSYMBOL_463_168 = 463,                  /* $@168  */
  YYSYMBOL_event = 464,                    /* event  */
  YYSYMBOL_event_forward_decl = 465,       /* event_forward_decl  */
  YYSYMBOL_event_concrete_forward_decl = 466, /* event_concrete_forward_decl  */
  YYSYMBOL_event_abs_forward_decl = 467,   /* event_abs_forward_decl  */
  YYSYMBOL_event_abs_decl = 468,           /* event_abs_decl  */
  YYSYMBOL_469_169 = 469,                  /* $@169  */
  YYSYMBOL_470_170 = 470,                  /* $@170  */
  YYSYMBOL_471_171 = 471,                  /* $@171  */
  YYSYMBOL_event_abs_header = 472,         /* event_abs_header  */
  YYSYMBOL_event_custom_header = 473,      /* event_custom_header  */
  YYSYMBOL_event_plain_header = 474,       /* event_plain_header  */
  YYSYMBOL_event_rest_of_header = 475,     /* event_rest_of_header  */
  YYSYMBOL_476_172 = 476,                  /* $@172  */
  YYSYMBOL_event_decl = 477,               /* event_decl  */
  YYSYMBOL_478_173 = 478,                  /* @173  */
  YYSYMBOL_479_174 = 479,                  /* $@174  */
  YYSYMBOL_480_175 = 480,                  /* $@175  */
  YYSYMBOL_event_header = 481,             /* event_header  */
  YYSYMBOL_formal_parameter_type = 482,    /* formal_parameter_type  */
  YYSYMBOL_at_least_one_formal_parameter = 483, /* at_least_one_formal_parameter  */
  YYSYMBOL_formal_parameters = 484,        /* formal_parameters  */
  YYSYMBOL_formal_parameter = 485,         /* formal_parameter  */
  YYSYMBOL_at_least_one_formal_parameter_name = 486, /* at_least_one_formal_parameter_name  */
  YYSYMBOL_formal_parameter_names = 487,   /* formal_parameter_names  */
  YYSYMBOL_formal_parameter_name = 488,    /* formal_parameter_name  */
  YYSYMBOL_porttype_decl = 489,            /* porttype_decl  */
  YYSYMBOL_490_176 = 490,                  /* $@176  */
  YYSYMBOL_491_177 = 491,                  /* @177  */
  YYSYMBOL_492_178 = 492,                  /* $@178  */
  YYSYMBOL_493_179 = 493,                  /* $@179  */
  YYSYMBOL_at_least_one_port_export = 494, /* at_least_one_port_export  */
  YYSYMBOL_port_exports = 495,             /* port_exports  */
  YYSYMBOL_port_export = 496,              /* port_export  */
  YYSYMBOL_497_180 = 497,                  /* $@180  */
  YYSYMBOL_extended_port_decl = 498,       /* extended_port_decl  */
  YYSYMBOL_at_least_one_actual_parameter = 499, /* at_least_one_actual_parameter  */
  YYSYMBOL_actual_parameters = 500,        /* actual_parameters  */
  YYSYMBOL_actual_parameter = 501,         /* actual_parameter  */
  YYSYMBOL_connector_decl = 502,           /* connector_decl  */
  YYSYMBOL_connector_header = 503,         /* connector_header  */
  YYSYMBOL_504_181 = 504,                  /* $@181  */
  YYSYMBOL_505_182 = 505,                  /* $@182  */
  YYSYMBOL_connector_body = 506,           /* connector_body  */
  YYSYMBOL_507_183 = 507,                  /* $@183  */
  YYSYMBOL_508_184 = 508,                  /* $@184  */
  YYSYMBOL_connector_exports = 509,        /* connector_exports  */
  YYSYMBOL_connector_export = 510,         /* connector_export  */
  YYSYMBOL_511_185 = 511,                  /* $@185  */
  YYSYMBOL_512_186 = 512,                  /* $@186  */
  YYSYMBOL_513_187 = 513,                  /* $@187  */
  YYSYMBOL_514_188 = 514                   /* $@188  */
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
#define YYLAST   2102

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  117
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  398
/* YYNRULES -- Number of rules.  */
#define YYNRULES  607
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  888

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   350


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
       2,     2,     2,     2,     2,     2,     2,   111,   106,     2,
     113,   114,   109,   107,   102,   108,     2,   110,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   101,    96,
      99,   103,   100,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   115,     2,   116,   105,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    97,   104,    98,   112,     2,     2,     2,
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
      95
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   414,   414,   417,   418,   426,   441,   445,   446,   447,
     452,   451,   460,   459,   468,   467,   476,   475,   484,   483,
     492,   491,   500,   499,   508,   507,   516,   515,   524,   523,
     532,   531,   540,   539,   548,   547,   556,   555,   564,   563,
     577,   576,   588,   627,   631,   587,   647,   655,   669,   679,
     709,   713,   654,   738,   742,   743,   747,   748,   753,   758,
     752,   844,   849,   843,   920,   921,   926,   964,   968,   925,
     985,   984,   996,  1033,  1063,  1096,  1095,  1104,  1111,  1112,
    1113,  1114,  1118,  1123,  1128,  1175,  1179,  1127,  1208,  1251,
    1255,  1206,  1274,  1272,  1312,  1311,  1323,  1327,  1334,  1339,
    1346,  1371,  1399,  1465,  1484,  1488,  1492,  1493,  1505,  1504,
    1522,  1526,  1533,  1554,  1555,  1559,  1574,  1579,  1578,  1587,
    1586,  1595,  1594,  1603,  1602,  1611,  1610,  1619,  1618,  1627,
    1626,  1635,  1634,  1647,  1659,  1657,  1682,  1689,  1699,  1698,
    1724,  1722,  1747,  1757,  1768,  1812,  1839,  1871,  1875,  1879,
    1883,  1870,  1945,  1946,  1947,  1948,  1949,  1950,  1951,  1955,
    1959,  2027,  2029,  2031,  2032,  2044,  2045,  2057,  2058,  2070,
    2071,  2080,  2092,  2093,  2102,  2114,  2115,  2124,  2133,  2145,
    2146,  2155,  2164,  2176,  2233,  2234,  2241,  2245,  2250,  2257,
    2264,  2268,  2273,  2277,  2281,  2285,  2292,  2361,  2360,  2389,
    2390,  2394,  2395,  2396,  2398,  2397,  2406,  2407,  2411,  2467,
    2471,  2478,  2491,  2501,  2509,  2508,  2596,  2600,  2607,  2616,
    2623,  2631,  2637,  2644,  2657,  2656,  2665,  2669,  2673,  2677,
    2705,  2713,  2712,  2783,  2784,  2788,  2795,  2796,  2822,  2823,
    2824,  2825,  2826,  2827,  2828,  2829,  2833,  2834,  2835,  2836,
    2840,  2841,  2842,  2846,  2847,  2851,  2863,  2861,  2886,  2893,
    2894,  2898,  2910,  2908,  2933,  2940,  2956,  2974,  2975,  2979,
    2983,  2987,  2991,  2995,  2999,  3003,  3010,  3014,  3018,  3022,
    3026,  3030,  3034,  3041,  3045,  3049,  3056,  3063,  3067,  3074,
    3081,  3088,  3095,  3103,  3102,  3116,  3147,  3151,  3115,  3168,
    3171,  3172,  3176,  3194,  3198,  3193,  3256,  3255,  3268,  3267,
    3280,  3284,  3317,  3321,  3380,  3384,  3279,  3406,  3413,  3426,
    3435,  3442,  3443,  3552,  3555,  3556,  3561,  3565,  3560,  3601,
    3600,  3612,  3622,  3640,  3648,  3647,  3661,  3665,  3660,  3681,
    3680,  3730,  3755,  3779,  3783,  3814,  3818,  3778,  3842,  3847,
    3845,  3851,  3855,  3895,  3899,  3893,  3983,  4050,  4059,  4049,
    4073,  4083,  4087,  4081,  4129,  4155,  4164,  4168,  4162,  4210,
    4236,  4244,  4243,  4286,  4296,  4314,  4322,  4326,  4321,  4386,
    4387,  4392,  4396,  4400,  4404,  4391,  4463,  4467,  4471,  4475,
    4462,  4543,  4547,  4579,  4583,  4542,  4600,  4604,  4665,  4669,
    4599,  4706,  4711,  4716,  4723,  4724,  4735,  4740,  4783,  4734,
    4805,  4804,  4813,  4812,  4823,  4828,  4826,  4832,  4837,  4841,
    4836,  4880,  4879,  4888,  4887,  4898,  4903,  4901,  4907,  4912,
    4916,  4911,  4961,  4968,  4969,  4970,  5077,  5081,  5085,  5093,
    5097,  5092,  5106,  5114,  5118,  5113,  5127,  5135,  5139,  5134,
    5148,  5156,  5160,  5155,  5169,  5176,  5188,  5186,  5209,  5216,
    5246,  5285,  5286,  5290,  5321,  5363,  5367,  5320,  5386,  5390,
    5384,  5431,  5430,  5438,  5445,  5460,  5461,  5466,  5465,  5475,
    5474,  5484,  5483,  5493,  5492,  5502,  5501,  5511,  5510,  5520,
    5519,  5530,  5623,  5629,  5654,  5761,  5770,  5774,  5781,  5856,
    5928,  6004,  6003,  6053,  6057,  6061,  6065,  6069,  6073,  6052,
    6126,  6125,  6133,  6140,  6145,  6153,  6157,  6152,  6167,  6168,
    6172,  6174,  6173,  6182,  6181,  6194,  6217,  6192,  6243,  6270,
    6241,  6294,  6295,  6296,  6300,  6301,  6305,  6334,  6366,  6410,
    6414,  6364,  6431,  6440,  6458,  6469,  6468,  6506,  6557,  6561,
    6504,  6578,  6582,  6589,  6593,  6597,  6601,  6605,  6609,  6613,
    6617,  6621,  6625,  6633,  6664,  6677,  6684,  6709,  6727,  6734,
    6749,  6756,  6766,  6770,  6789,  6797,  6765,  6812,  6827,  6831,
    6832,  6836,  6837,  6839,  6838,  6849,  6916,  6964,  6980,  6993,
    7000,  7059,  7067,  7071,  7066,  7132,  7136,  7131,  7149,  7150,
    7155,  7154,  7163,  7162,  7171,  7170,  7179,  7178
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
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,    59,   123,   125,    60,
      62,    58,    44,    61,   124,    94,    38,    43,    45,    42,
      47,    37,   126,    40,    41,    91,    93
};
#endif

#define YYPACT_NINF (-672)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-576)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -672,    90,  1339,  -672,  -672,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,   141,   144,    57,    58,  -672,   141,
     141,  -672,    56,    56,  -672,  -672,   141,  -672,  -672,    60,
    -672,   461,    79,    81,  -672,  -672,    -8,  -672,  -672,  -672,
    -672,  -672,  -672,   537,  -672,  -672,  -672,  -672,  -672,  1539,
      61,  -672,  -672,    85,  -672,   146,  -672,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,    70,  -672,  -672,  -672,    70,  -672,
    -672,   119,   139,  2013,    56,   141,  1892,   141,   141,   141,
     141,  -672,  -672,  -672,    13,   141,    39,  -672,    40,   141,
    -672,    70,   141,   149,   150,   141,  -672,  -672,    26,  -672,
      33,   233,  -672,   153,  -672,   154,   157,  1651,  -672,  -672,
    -672,   158,   207,  -672,   168,   170,   172,   140,  -672,   198,
    -672,  -672,  -672,  -672,  -672,  -672,   171,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,
    -672,   184,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,   146,
    -672,  -672,  -672,    72,  -672,  -672,   175,  -672,   178,   182,
     185,  -672,    56,   188,   190,   187,  -672,   191,   194,   195,
     196,   197,   199,   202,   204,  -672,  -672,  -672,   205,   208,
    -672,  -672,  -672,  -672,   184,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,   184,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,  -672,   211,  -672,   206,  -672,  -672,   148,  -672,
     276,  -672,  -672,  -672,  -672,    44,  -672,  -672,  -672,  2013,
    -672,  -672,  -672,  -672,   181,  -672,  -672,  -672,  -672,   290,
    -672,  -672,    54,   210,  -672,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,   303,  -672,   135,   214,   267,  -672,  -672,  -672,
    -672,  -672,  -672,   184,  -672,  -672,   203,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,  -672,  -672,   267,   218,   223,  -672,
    -672,  -672,   141,   141,   224,   230,  -672,  -672,  -672,   227,
    -672,   276,   236,  -672,  -672,  -672,  -672,  -672,   327,  -672,
     238,   239,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,   183,   183,   183,   135,   184,  -672,  -672,   235,
     241,   234,   128,   114,    14,  -672,  -672,  -672,  -672,  -672,
      56,  -672,  -672,  -672,  -672,   240,  -672,    56,  -672,   135,
     135,   135,   231,  -672,  -672,  -672,  -672,  -672,  -672,  -672,
     133,  -672,     1,  -672,  -672,  -672,  -672,  -672,  -672,  -672,
    -672,    56,   267,  -672,  -672,  -672,  -672,   148,   565,  1453,
     245,   247,  -672,  1651,  -672,  -672,  -672,   244,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   246,   141,
    -672,   184,  1103,  -672,   825,   135,  -672,  -672,  -672,  -672,
    -672,   135,  -672,  1403,  -672,  -672,  -672,   381,  1011,  -672,
    -672,  -672,  -672,    68,   292,    56,    56,  -672,  -672,  -672,
    -672,  -672,    68,  -672,   253,  -672,   249,  -672,   252,  -672,
    -672,  1196,   184,  -672,    56,   267,  -672,  -672,  -672,  -672,
     263,  -672,  -672,   141,  -672,  -672,   264,   265,   359,   268,
    -672,  -672,   241,   234,   128,   114,   114,    14,    14,  -672,
    -672,  -672,  -672,  -672,   262,  -672,  -672,  -672,   269,  -672,
    -672,  1805,  -672,  -672,  -672,  -672,  1926,  -672,  -672,  -672,
    -672,  -672,   270,  -672,  1839,  -672,  -672,  1716,  -672,   266,
    1627,   271,   277,   278,   280,  -672,   255,  -672,   285,  -672,
    -672,  -672,   281,   282,   711,    56,    56,    56,   237,  -672,
     294,  -672,  -672,  -672,  -672,  -672,  -672,  -672,   141,   141,
    -672,   295,  -672,  -672,  -672,  1289,   918,   357,  1979,  -672,
     184,   276,  -672,  -672,    65,    66,   298,   302,   309,   276,
     310,  -672,  -672,     2,  -672,    47,  -672,  -672,   301,   311,
     184,  -672,   315,   160,  1892,  -672,   378,  -672,  -672,  -672,
    -672,    54,  -672,   312,  -672,   319,  -672,   322,   324,   325,
     328,  -672,   184,  -672,  -672,  -672,  -672,  -672,   329,   330,
     420,  -672,  -672,  -672,   338,  -672,  -672,  -672,  -672,  -672,
     135,  -672,   276,  -672,   339,   141,  -672,  -672,   428,   184,
    -672,  -672,  -672,  -672,  -672,  -672,    69,    69,    69,  -672,
     342,  -672,   343,   348,   349,   351,   355,   356,  -672,  -672,
    -672,   364,   365,   368,   372,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,  -672,   135,  -672,  -672,  -672,   141,
    -672,   373,   375,   376,  -672,   409,   382,   160,  -672,   387,
     390,  -672,   391,   135,   393,  1514,  -672,    56,  -672,  -672,
    -672,  -672,  -672,  -672,   487,  -672,  -672,  -672,  -672,  -672,
     280,   285,  -672,  -672,   377,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,  -672,   383,   383,  -672,  -672,  -672,
    -672,  1979,   141,  -672,   135,   384,  -672,  -672,  -672,  -672,
    -672,  -672,  -672,   401,  -672,  -672,  -672,  -672,  -672,    56,
    -672,  -672,  -672,  -672,   402,   184,  -672,   383,  -672,   404,
    -672,   466,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,
      56,  -672,   184,   405,   635,  -672,   389,  -672,  -672,   407,
     394,   470,   471,   471,   141,   456,   410,   397,  -672,   184,
     416,  -672,  -672,   403,  -672,   471,  -672,  -672,  -672,   406,
    -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,   462,
     519,   411,   152,   471,  -672,    75,  1979,  -672,   427,   417,
     471,   418,   472,   141,    56,  -672,  -672,   433,  -672,  -672,
    -672,  -672,  -672,   421,  -672,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,   184,  -672,   434,  -672,   436,  1979,   500,   445,
     135,   442,   446,    55,  -672,   201,   141,   470,    56,    56,
     451,   141,   519,  -672,  -672,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,  1603,  -672,  -672,  -672,   452,   453,  -672,  -672,
    -672,   152,   141,   457,   464,  -672,  -672,  -672,  -672,    56,
    -672,  -672,  -672,  -672,   141,   475,   473,   509,  -672,  -672,
    -672,  -672,   478,   483,  -672,  -672,   514,  -672
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       4,     0,     0,     3,     1,    38,   147,    40,    70,   224,
     293,   308,   343,   391,     0,     0,     0,     0,    94,     0,
       0,   503,     0,     0,   572,   592,     0,     6,     7,    42,
      24,    61,     0,     0,    22,    64,    77,    66,    26,    78,
      83,    79,    84,    77,    80,    81,    65,    18,    10,     0,
       0,    12,   230,   295,   226,   342,   227,   253,   254,   228,
      20,    14,    16,    28,   462,   461,   464,    30,   501,    32,
     533,   535,   534,   532,    77,   551,   552,   531,    77,    34,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   143,   265,   229,    77,     0,    77,    88,    77,     0,
      82,    77,     0,   468,   544,     0,   142,   138,     0,   137,
       0,     0,   213,     0,    46,     0,     0,     0,   213,     8,
       9,     0,    97,    72,     0,     0,     0,   269,   271,     0,
     283,   284,   287,   288,   289,   290,   286,   291,   292,   357,
     365,   370,   272,   279,   273,   280,   274,   281,   275,   282,
      92,   237,   102,   233,   235,   236,   234,   238,   267,   268,
     239,   243,   240,   242,   241,   244,   245,   295,   250,     0,
     251,   252,   246,     0,   249,   247,   364,   248,   369,     0,
       0,     5,     0,   211,     0,     0,   310,     0,     0,     0,
       0,     0,     0,     0,     0,   545,   538,   547,     0,     0,
     595,   591,    39,   286,   160,   148,   152,   156,   157,   153,
     154,   155,   158,   159,    41,    71,   225,   231,   294,   309,
     344,   392,    73,   542,    74,     0,   543,    95,   473,   504,
       0,   459,   140,   460,   573,     0,   197,    43,    25,     0,
     558,   554,   555,   560,   557,   561,   559,   556,   553,     0,
      48,   565,     0,     0,    23,    96,    75,    67,    27,    85,
     270,   285,   276,   278,     0,     0,    99,   356,   353,   361,
     366,    19,    11,   214,    13,   296,     0,    21,    15,    17,
      29,   465,    31,   515,   502,    33,    99,     0,     0,    35,
      37,   599,     0,     0,     0,     0,    89,   471,   469,   512,
     139,     0,     0,   593,   212,   200,     4,   562,     0,   566,
       0,   563,   186,   187,   188,   190,   193,   192,   194,   195,
     191,   189,     0,     0,     0,     0,   183,   590,   161,   162,
     163,   165,   167,   169,   172,   175,   179,   184,   589,    62,
       0,   114,   105,   277,   196,     0,   358,     0,    93,     0,
       0,     0,   217,   213,   311,   476,   519,   546,   539,   548,
     596,   149,   265,   232,   258,   259,   260,   266,   345,   393,
     114,     0,    99,   510,   505,   141,   574,   473,     0,     0,
       3,     0,    49,     0,   180,   181,   182,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   587,     0,
      76,   136,     0,   113,     0,     0,   213,    98,   354,   362,
     367,     0,   215,     0,   297,   301,   213,   213,     0,   114,
     105,   381,   386,     0,   497,     0,     0,   604,   379,   380,
     600,   602,     0,   606,     0,   598,     0,   213,   255,   213,
     301,     0,   472,   470,     0,    99,   580,   594,   204,   198,
       0,   206,   199,     0,   201,   207,     0,     0,     0,     0,
     564,   185,   164,   166,   168,   170,   171,   173,   174,   176,
     177,   178,   213,    63,   133,   131,   401,   402,     0,   116,
     123,     0,   117,   127,   125,   129,     0,   119,   121,   406,
     111,   110,     0,   104,     0,   106,   107,     0,   108,     0,
       0,     0,     0,     0,   137,   218,     0,   219,   222,   306,
     303,   302,     0,   213,     0,     0,     0,     0,     0,   487,
       0,   475,   477,   479,   481,   483,   485,   489,     0,     0,
     520,     0,   518,   521,   523,     0,     0,     0,     0,   493,
     492,     0,   496,   495,     0,     0,     0,     0,     0,     0,
       0,   597,   150,     0,   256,     0,   346,   351,   213,     0,
     511,   506,   579,   213,     0,   202,   210,   203,    45,   567,
      50,     0,   134,     0,    69,     0,   115,     0,     0,     0,
       0,   405,   435,   432,   433,   434,   396,   404,     0,     0,
       0,    87,   112,   103,     0,   360,   359,   355,   363,   368,
       0,   216,     0,   220,     0,     0,   298,   300,   269,   322,
     317,   318,   319,   320,   312,   321,     0,     0,     0,   474,
       0,   467,     0,     0,     0,     0,     0,     0,   525,   528,
     517,     0,     0,     0,     0,   382,   387,   491,   585,   586,
     605,   601,   603,   494,   607,     0,   376,   372,   375,     0,
     352,     0,   348,     0,    91,     0,     0,     0,   583,     0,
       0,   578,     0,     0,     0,     0,   588,     0,   132,   124,
     118,   128,   126,   130,     0,   120,   122,   407,   109,   223,
       0,   222,   307,   304,     0,   500,   498,   499,   488,   478,
     480,   482,   484,   486,   490,     0,     0,   522,   524,   541,
     550,     0,     0,   151,     0,   373,   257,   347,   349,   395,
     507,   576,   577,     0,   581,   582,   205,   209,   208,     0,
      56,    42,    51,    55,     0,   135,   397,     0,   221,     0,
     313,   410,   526,   529,   383,   388,   264,   377,   374,   213,
       0,   584,    58,     0,     0,    57,     0,   408,   305,     0,
       0,     0,   442,   442,     0,   446,   261,     0,   350,   508,
       0,    52,    54,   423,   398,   442,   314,   411,   418,     0,
     417,   439,   527,   530,   384,   443,   389,   262,   378,   514,
       0,     0,     0,   442,   409,     0,     0,   413,   414,     0,
     442,     0,   450,     0,     0,   509,   571,     0,   570,   422,
     436,   437,   438,     0,   428,   429,   399,   329,   336,   334,
     315,   325,   326,   333,   419,   415,   440,   385,   444,   447,
     390,   263,   513,    59,   568,   424,   425,     0,   454,     0,
       0,     0,     0,     0,   213,   331,     0,     0,     0,     0,
       0,     0,     0,   426,   430,   451,   400,   330,   337,   335,
     316,   324,     0,   332,   420,   416,     0,     0,   448,    60,
     569,     0,     0,     0,     0,   339,   327,   441,   445,     0,
     427,   431,   452,   338,     0,     0,     0,     0,   340,   328,
     449,   458,     0,   455,   453,   456,     0,   457
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -672,  -672,   287,   288,   546,  -592,  -672,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,
    -672,  -579,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,  -672,  -672,  -148,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,    37,  -672,
    -672,    24,  -672,  -672,  -672,   580,  -672,  -672,  -672,  -672,
    -672,  -672,  -672,   582,  -672,   215,  -672,  -672,  -246,  -672,
    -672,   179,   103,  -672,  -672,  -672,  -319,  -672,  -360,  -672,
    -672,  -672,  -672,  -672,  -672,  -672,  -672,  -332,  -672,  -672,
     -22,  -672,  -672,  -188,   -10,  -672,     6,  -672,  -672,  -672,
    -672,  -196,   -44,  -226,  -672,   217,   213,   216,  -158,  -154,
    -201,  -119,  -672,  -318,  -672,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,    23,   -82,   553,  -672,  -672,  -672,  -672,   -74,
       7,    16,  -672,    46,  -672,   -31,   108,  -457,  -672,  -672,
    -672,     8,  -672,  -672,  -610,  -143,  -672,  -672,    -7,  -672,
     -66,  -672,  -672,   -61,   -42,   -59,   -56,   -55,   250,  -672,
     -40,  -672,   -38,  -672,  -672,  -672,  -672,   174,   259,   121,
    -672,  -672,  -672,   -37,  -672,   -32,  -672,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,  -672,  -217,  -672,  -672,  -672,  -672,
    -672,  -218,  -672,  -672,  -672,  -672,  -672,  -672,  -672,   -41,
    -672,  -672,  -672,  -672,  -672,  -672,  -672,  -112,  -672,  -672,
    -672,  -672,  -672,  -672,  -672,   -70,  -672,  -672,  -672,   -69,
    -672,  -672,  -672,  -672,  -672,  -672,  -672,   -76,  -672,  -672,
    -326,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,
    -672,    17,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,  -672,  -672,  -672,  -622,  -672,  -672,
    -672,  -672,  -672,  -207,  -672,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,  -227,  -672,  -672,  -501,  -672,  -671,  -672,  -672,
    -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,    18,    21,  -672,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,  -672,   258,  -672,  -672,   125,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,  -325,   212,  -322,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,   559,  -672,
    -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,   283,  -672,
    -672,  -183,  -672,  -672,  -672,  -672,  -672,  -672,  -672,     3,
    -672,   304,  -672,  -672,    94,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,     3,    27,    28,   180,   184,   188,   189,
     179,   187,   121,   116,   125,   190,   192,   194,   198,   199,
      82,    29,    84,    30,   115,   306,   457,    31,    32,   117,
     310,   459,   665,   743,   722,   744,   723,   724,   760,   841,
      33,   118,   399,    34,    35,   124,   341,   478,    36,    85,
      37,   150,   340,    38,    39,    40,   126,   342,   492,    41,
     225,   370,   559,    42,   266,    43,   102,   256,   348,    44,
      45,   404,   493,   594,   494,   495,   402,   403,   479,   577,
     588,   589,   575,   579,   578,   580,   573,   400,   474,   667,
     326,   230,   301,   109,   362,    46,   480,    83,   292,   436,
     645,   205,   327,   344,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   345,    48,   305,   378,   452,   564,   453,
     454,   664,   481,    50,   304,   352,   412,   506,   507,   603,
     508,   482,    86,   216,   293,   217,   153,   154,   155,   156,
      52,   363,   438,   649,   364,   735,   756,   793,   365,   366,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      53,    87,    54,   185,   353,   512,   414,   513,   607,   511,
     605,   729,   604,    55,    88,    56,   276,   416,   684,   749,
     785,   832,   614,   810,   833,   811,   834,   875,   829,   812,
     835,   813,   831,   830,   864,   866,   874,    57,    58,    59,
      89,   294,   439,   651,   556,   652,   739,   557,   172,   349,
     501,   173,   265,   406,   174,   175,   350,   502,   176,   177,
     351,   503,   178,   367,   437,   647,   705,   648,   704,   757,
     483,   428,   537,   701,   754,   790,   429,   538,   702,   755,
     792,   484,    90,   295,   440,   653,   485,   674,   746,   783,
     828,   486,   586,   498,   590,   727,   765,   732,   750,   751,
     769,   788,   837,   770,   786,   836,   764,   781,   782,   803,
     826,   861,   804,   827,   862,   587,   805,   772,   789,   838,
     776,   791,   839,   820,   840,   869,   846,   863,   877,   882,
     883,   886,   487,   488,    63,    64,    65,   191,   355,   520,
      66,   228,   372,   298,   371,   417,   521,   622,   623,   624,
     625,   626,   620,   627,   522,   541,   523,   432,   543,   524,
     525,   526,    67,   193,    68,   105,   299,   445,   655,   740,
     779,   374,   444,   795,   284,   356,   531,   418,   532,   631,
     632,   533,   695,   752,   534,   696,   753,    69,    70,    71,
      72,    73,   287,   419,   633,    74,    75,    76,   196,   286,
      77,   288,   420,   634,    78,   249,   250,   311,   251,   797,
     824,   798,    79,   111,   302,   446,   656,   562,   563,   661,
     713,   527,   253,   398,   338,    80,    81,   112,   377,   201,
     291,   434,   360,   435,   547,   548,   546,   550
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     108,   110,   171,   167,    92,   168,   169,    93,    47,   103,
     104,   170,   152,   212,   213,   407,   113,   206,    51,    60,
      61,   151,   207,    62,   209,    49,   328,   210,   211,   583,
     235,   408,   409,   410,   427,   430,   252,   636,   431,   706,
     357,   208,   300,   307,   496,   171,   167,   303,   168,   169,
     650,   441,    94,    96,   170,    47,   807,   106,   530,   106,
     123,   204,   214,     8,   151,    51,    60,    61,   638,   639,
      62,   106,   106,   720,   733,   215,   807,   218,   219,   220,
     221,   583,   773,   808,   809,   223,   721,   539,  -144,   226,
       4,   519,   227,   122,   784,   229,  -371,   182,   195,   387,
     535,   231,   195,   808,   809,   747,    18,    18,   233,  -145,
      95,    99,   806,   375,   122,   232,  -371,   646,   222,   817,
     224,   576,   232,   395,   396,   397,   443,   312,   313,   314,
     315,   316,   317,   318,   319,  -146,  -100,   496,   106,   182,
     122,   122,   182,   107,    91,   107,   320,   321,   260,   182,
       8,   261,   720,  -323,   232,   232,   182,   107,   232,   114,
     273,   322,   323,   421,   422,   721,   324,   325,   186,   212,
     213,   122,   267,   206,   268,   119,   496,   120,   207,   499,
     209,  -341,   451,   210,   211,   505,   106,   800,   801,   802,
     421,   422,   519,   423,   469,   470,   471,   208,   424,   561,
     734,   425,   426,   384,   385,   386,   262,   263,   312,   313,
     314,   315,   316,   317,   318,   319,   200,   204,   391,   392,
     423,   393,   394,   504,   107,   424,   854,   320,   321,   808,
     809,    98,   101,   465,   466,   202,   234,   658,   659,   467,
     468,   660,   322,   323,   583,  -463,  -536,   324,   325,   297,
     236,   237,   871,   238,   254,   255,   312,   313,   314,   315,
     316,   317,   318,   319,   878,   257,   258,   421,   422,   259,
     264,   413,   107,   232,   269,   320,   321,   270,   271,   106,
     308,   272,   361,  -212,   275,   814,   274,   277,   515,   516,
     278,   279,   280,   309,   281,   282,   325,   423,   517,   283,
     285,   289,   424,   296,   290,   425,   426,  -537,   212,   213,
     339,   343,   206,   346,   347,   358,   354,   207,   401,   209,
     359,   368,   210,   211,   500,   401,   844,   369,   373,   583,
     381,   658,   659,   376,   514,   660,   208,   456,   382,   388,
     390,   383,   405,   -44,   411,   328,   389,   458,   472,   442,
     542,   551,   552,   637,   554,   553,   204,   555,   461,   565,
     567,   643,   569,   568,   572,   570,   595,   574,   591,   601,
     583,   597,   171,   167,   679,   168,   169,   598,   599,   606,
    -299,   170,   510,   600,   450,    47,   737,   602,   635,   473,
     571,   151,   621,   630,   640,    51,    60,    61,   641,  -394,
      62,   540,    49,   544,   545,   642,   644,   663,   668,   654,
     540,   421,   422,  -575,   680,   669,   584,   585,   670,   328,
     671,   672,   560,   677,   673,   675,   676,   497,   685,   686,
     687,   413,   515,   516,   678,   682,   260,   717,   688,   689,
     518,   423,   517,   566,   690,   691,   424,   692,   610,   425,
     426,   693,   694,   171,   167,   611,   168,   169,   612,   613,
     697,   698,   170,   510,   582,   -47,   699,   -47,   584,   585,
     700,   707,   151,   615,   709,   710,   413,   708,   151,  -466,
     711,   -47,   -47,   714,   -47,   -47,   715,   716,   -47,   718,
     726,   730,   609,   616,   617,   618,   731,   741,   745,   646,
     748,  -412,   763,   761,   766,   768,   856,   857,   767,   771,
     -47,   775,   777,   778,   -47,   780,   582,  -421,   628,   629,
     787,   794,   796,   171,   167,   799,   168,   169,   -47,   815,
     816,   818,   170,   823,   819,   825,   842,   876,   843,   845,
     106,   847,   151,   849,   850,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    10,    11,   497,
      12,   139,   140,   141,   858,   873,   867,   868,   106,     6,
     872,   879,   448,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   203,   137,   881,   885,   657,   880,    12,   887,
     140,   141,   884,   379,   380,   181,   762,   100,    97,   536,
     593,   703,   463,   183,   848,   462,   464,   728,   596,   681,
     662,   774,   415,   683,   558,   592,   851,   853,   142,   143,
     144,   145,   146,   147,   148,   149,   107,   758,   455,   738,
     855,   584,   585,  -101,   870,   447,     5,   197,   122,     6,
       7,     8,     9,   619,   549,   725,   142,   143,   144,   145,
     146,   147,   148,   149,   107,    10,    11,   555,    12,   860,
     712,     0,    13,   449,   433,   666,   460,     0,     0,     0,
       0,    47,     0,     0,     0,    14,    15,    16,    17,   582,
       0,    51,    60,    61,    18,    19,    62,     0,    20,     0,
       0,    21,    92,     0,     0,   736,     0,   742,    22,    23,
       0,     0,     0,     0,     0,    24,    25,   719,     0,     0,
       0,     0,     0,     0,   106,     0,   584,   585,   759,   608,
     128,   129,     0,     0,   132,   133,   134,   135,     0,    26,
       0,     0,     0,   -53,    12,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,   736,     0,     0,
      47,     0,   852,     0,     0,     0,     0,   584,   585,     0,
      51,    60,    61,     0,   582,    62,     0,     0,     0,     0,
       0,     0,   822,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,   821,     0,     0,     0,
       0,     0,   142,   143,   144,   145,   146,   147,   148,   149,
     107,     0,     0,     0,     0,   582,   182,     0,     0,     0,
       0,   171,   167,     0,   168,   169,   401,   401,     0,     0,
     170,   865,     0,     0,     0,     0,   475,     0,  -403,     6,
     151,   859,     9,  -403,  -403,  -403,  -403,  -403,  -403,  -403,
    -403,  -403,  -403,  -403,  -403,    10,    11,   401,    12,     0,
    -403,  -403,    13,     0,     0,   421,   422,   476,   477,  -403,
       0,     0,     0,     0,     0,    14,     0,     0,     0,   489,
     490,   491,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,    23,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -403,  -403,  -403,  -403,
    -403,  -403,  -403,  -403,  -403,     0,     0,     0,     0,   475,
    -213,  -403,     6,   -86,     0,     9,  -403,  -403,  -403,  -403,
    -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,    10,    11,
       0,    12,     0,  -403,  -403,    13,     0,     0,   421,   422,
     476,   477,  -403,     0,     0,     0,     0,     0,    14,     0,
       0,     0,   489,   490,   491,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,    23,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -403,
    -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,     0,     0,
       0,     0,   475,  -213,  -403,     6,  -549,     0,     9,  -403,
    -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,
    -403,    10,    11,     0,    12,     0,  -403,  -403,    13,     0,
       0,   421,   422,   476,   477,  -403,     0,     0,     0,     0,
       0,    14,     0,     0,     0,   528,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   529,     0,     0,     0,     0,
       0,     0,     0,     0,    22,    23,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,
    -403,     0,     0,     0,   475,     0,  -403,     6,     0,  -516,
       9,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,
    -403,  -403,  -403,    10,    11,     0,    12,     0,  -403,  -403,
      13,     0,     0,   421,   422,   476,   477,  -403,     0,     0,
       0,     0,     0,    14,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    22,    23,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -403,  -403,  -403,  -403,  -403,  -403,
    -403,  -403,  -403,     0,     0,     0,     0,   475,  -213,  -403,
       6,   -68,     0,     9,  -403,  -403,  -403,  -403,  -403,  -403,
    -403,  -403,  -403,  -403,  -403,  -403,    10,    11,     0,    12,
       0,  -403,  -403,    13,     0,     0,   421,   422,   476,   477,
    -403,     0,     0,     0,     0,     0,    14,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
      23,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -403,  -403,  -403,
    -403,  -403,  -403,  -403,  -403,  -403,     0,     0,     0,     0,
     475,  -213,  -403,     6,   -90,     0,     9,  -403,  -403,  -403,
    -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,    10,
      11,     0,    12,     0,  -403,  -403,    13,     0,     0,   421,
     422,   476,   477,  -403,     0,     0,     0,     0,     0,    14,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    -2,
       5,     0,     0,     6,     7,     8,     9,     0,     0,     0,
       0,     0,    22,    23,     0,     0,     0,     0,     0,    10,
      11,     0,    12,     0,     0,     0,    13,     0,     0,     0,
    -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,    14,
      15,    16,    17,     0,  -213,     0,     0,  -540,    18,    19,
       0,     0,    20,     0,     0,    21,     0,     0,     0,     0,
       0,     0,    22,    23,   509,     0,   106,     0,     0,    24,
      25,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    10,    11,     0,    12,   139,   140,   141,
       0,     0,     0,    26,  -213,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,     0,     0,     6,     7,     8,
       9,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    10,    11,     0,    12,     0,     0,     0,
      13,     0,     0,     0,   142,   143,   144,   145,   146,   147,
     148,   149,   107,    14,    15,    16,    17,     0,   182,     0,
       0,     0,    18,    19,     0,     0,    20,     0,     0,    21,
       0,     0,     0,     0,     0,     5,    22,    23,     6,     7,
       8,     9,     0,    24,    25,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    10,    11,     0,    12,     0,     0,
       5,    13,     0,     6,     7,     8,     9,    26,  -213,     0,
       0,     0,     0,     0,    14,    15,    16,    17,     0,    10,
      11,     0,    12,    18,    19,     0,    13,    20,     0,     0,
      21,     0,     0,     0,     0,     0,     0,    22,    23,    14,
      15,    16,    17,     0,    24,    25,   719,     0,    18,    19,
       0,     0,    20,     0,     0,    21,     0,     0,     0,     0,
       0,     0,    22,    23,     0,     0,   106,     0,    26,    24,
      25,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    10,    11,     0,    12,   139,   140,   141,
     106,     0,     0,    26,     0,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,     0,     0,     0,
       0,   139,   140,   141,     0,   239,     0,   240,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   241,   242,     0,   243,   244,     0,     0,   245,     0,
       0,     0,     0,     0,   142,   143,   144,   145,   146,   147,
     148,   149,   107,     0,     0,     0,     0,     0,   182,     0,
     246,     0,     0,     0,   247,     0,     0,     0,   142,   143,
     144,   145,   146,   147,   148,   149,   107,   475,   248,  -403,
       6,     0,   182,     9,  -403,  -403,  -403,  -403,  -403,  -403,
    -403,  -403,  -403,  -403,  -403,  -403,    10,    11,     0,    12,
       0,  -403,  -403,    13,     0,     0,   421,   422,   476,   477,
    -403,     0,     0,     0,     0,     0,    14,     0,     0,     0,
     489,   490,   491,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
      23,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -403,  -403,  -403,
    -403,  -403,  -403,  -403,  -403,  -403,   475,     0,  -403,     6,
       0,     0,     9,  -403,  -403,  -403,  -403,  -403,  -403,  -403,
    -403,  -403,  -403,  -403,  -403,    10,    11,     0,    12,     0,
    -403,  -403,    13,     0,     0,   421,   422,   476,   477,  -403,
     509,     0,   106,     0,     0,    14,     0,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,    10,
      11,     0,    12,   139,   140,   141,     0,     0,    22,    23,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -403,  -403,  -403,  -403,
    -403,  -403,  -403,  -403,  -403,   106,     0,     0,     0,     0,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,    10,    11,     0,    12,   139,   140,   141,     0,
     142,   143,   144,   145,   146,   147,   148,   149,   107,   106,
       0,     0,     0,     0,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   203,   137,   138,     0,     0,     0,     0,
       0,   140,   141,     0,     0,     0,     0,     0,     0,     0,
     581,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   142,   143,   144,   145,   146,   147,   148,
     149,   107,   106,     0,     0,     0,     0,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   203,   137,   138,     0,
       0,     0,     0,     0,   140,   141,     0,   142,   143,   144,
     145,   146,   147,   148,   149,   107,   106,     0,     0,     0,
       0,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     203,     0,     0,     0,     0,     0,     0,     0,   140,   141,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     142,   143,   144,   145,   146,   147,   148,   149,   107,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   142,   143,   144,   145,   146,   147,
     148,   149,   107
};

static const yytype_int16 yycheck[] =
{
      22,    23,    43,    43,    14,    43,    43,    14,     2,    19,
      20,    43,    43,    83,    83,   347,    26,    83,     2,     2,
       2,    43,    83,     2,    83,     2,   252,    83,    83,   486,
     112,   349,   350,   351,   360,   360,   118,   538,   360,   649,
     286,    83,   230,   239,   404,    86,    86,     3,    86,    86,
       3,   370,    15,    16,    86,    49,     1,     3,   418,     3,
      36,    83,    84,     6,    86,    49,    49,    49,     3,     3,
      49,     3,     3,   665,   696,    85,     1,    87,    88,    89,
      90,   538,   753,    28,    29,    95,   665,    19,    96,    99,
       0,   417,   102,   101,   765,   105,    95,    95,    74,   325,
     419,    75,    78,    28,    29,   727,    49,    49,    75,    96,
      53,    53,   783,   301,   101,    89,   115,   115,    94,   790,
      96,   481,    89,   109,   110,   111,   372,    73,    74,    75,
      76,    77,    78,    79,    80,    96,    96,   497,     3,    95,
     101,   101,    95,    89,     3,    89,    92,    93,     8,    95,
       6,    11,   744,    98,    89,    89,    95,    89,    89,    99,
     182,   107,   108,    30,    31,   744,   112,   113,    22,   239,
     239,   101,   100,   239,   102,    96,   536,    96,   239,   405,
     239,    96,   378,   239,   239,   411,     3,    35,    36,    37,
      30,    31,   518,    60,   395,   396,   397,   239,    65,   445,
     701,    68,    69,   322,   323,   324,     8,     9,    73,    74,
      75,    76,    77,    78,    79,    80,    97,   239,    90,    91,
      60,   107,   108,   411,    89,    65,   836,    92,    93,    28,
      29,    16,    17,   391,   392,    96,     3,   563,   563,   393,
     394,   563,   107,   108,   701,    96,    96,   112,   113,   101,
      97,    97,   862,    96,    96,    48,    73,    74,    75,    76,
      77,    78,    79,    80,   874,    97,    96,    30,    31,    97,
      99,   353,    89,    89,    99,    92,    93,    99,    96,     3,
      99,    96,   292,    95,    97,   786,    96,    96,    51,    52,
      96,    96,    96,     3,    97,    96,   113,    60,    61,    97,
      96,    96,    65,    97,    96,    68,    69,    96,   378,   378,
     100,     8,   378,    99,    47,    97,   113,   378,   340,   378,
      97,    97,   378,   378,   406,   347,   827,    97,   101,   786,
       3,   657,   657,    97,   416,   657,   378,   378,   100,   104,
     106,   102,   102,    98,   113,   571,   105,   100,   102,   371,
      58,    98,   103,   541,   102,   437,   378,   439,   114,    96,
      96,   549,     3,    98,   102,    97,   100,    98,    98,   114,
     827,   100,   413,   413,   600,   413,   413,   100,   100,    98,
      98,   413,   413,   103,   378,   379,   704,   102,    31,   399,
     472,   413,    98,    98,    96,   379,   379,   379,    96,    98,
     379,   423,   379,   425,   426,    96,    96,    29,    96,    98,
     432,    30,    31,    98,   602,    96,   486,   486,    96,   645,
      96,    96,   444,     3,    96,    96,    96,   404,   616,   617,
     618,   513,    51,    52,    96,    96,     8,   663,    96,    96,
     417,    60,    61,   453,    96,    96,    65,    96,   514,    68,
      69,    96,    96,   494,   494,   514,   494,   494,   514,   514,
      96,    96,   494,   494,   486,     4,    98,     6,   538,   538,
      98,    98,   494,   514,    98,    66,   558,   102,   500,    98,
      98,    20,    21,    96,    23,    24,    96,    96,    27,    96,
       3,   114,   514,   515,   516,   517,   113,    96,    96,   115,
      96,    35,   113,    98,    97,    35,   838,   839,   114,    38,
      49,    55,   102,   116,    53,    99,   538,   114,   528,   529,
     114,    59,     3,   564,   564,   114,   564,   564,    67,   102,
     113,   113,   564,   100,    62,   114,   102,   869,   102,    39,
       3,    96,   564,   101,    98,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,   536,
      23,    24,    25,    26,   113,   101,   114,   114,     3,     4,
     113,    96,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    75,   102,   563,   114,    23,    75,
      25,    26,   114,   306,   306,    49,   744,    17,    16,   420,
     497,   645,   389,    50,   830,   388,   390,   681,   500,   602,
     564,   754,   353,   605,   440,   494,   833,   835,    81,    82,
      83,    84,    85,    86,    87,    88,    89,   739,   378,   705,
     837,   701,   701,    96,   861,   377,     1,    78,   101,     4,
       5,     6,     7,   518,   432,   667,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    20,    21,   739,    23,   842,
     657,    -1,    27,    98,   360,   571,   383,    -1,    -1,    -1,
      -1,   665,    -1,    -1,    -1,    40,    41,    42,    43,   701,
      -1,   665,   665,   665,    49,    50,   665,    -1,    53,    -1,
      -1,    56,   702,    -1,    -1,   702,    -1,   719,    63,    64,
      -1,    -1,    -1,    -1,    -1,    70,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,     3,    -1,   786,   786,   740,     8,
       9,    10,    -1,    -1,    13,    14,    15,    16,    -1,    94,
      -1,    -1,    -1,    98,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   754,    -1,    -1,   754,    -1,    -1,
     744,    -1,   834,    -1,    -1,    -1,    -1,   827,   827,    -1,
     744,   744,   744,    -1,   786,   744,    -1,    -1,    -1,    -1,
      -1,    -1,   794,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   793,    -1,    -1,   793,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    -1,    -1,    -1,   827,    95,    -1,    -1,    -1,
      -1,   852,   852,    -1,   852,   852,   838,   839,    -1,    -1,
     852,   852,    -1,    -1,    -1,    -1,     1,    -1,     3,     4,
     852,   841,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,   869,    23,    -1,
      25,    26,    27,    -1,    -1,    30,    31,    32,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    44,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    -1,    -1,    -1,     1,
      95,     3,     4,    98,    -1,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    23,    -1,    25,    26,    27,    -1,    -1,    30,    31,
      32,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    44,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    -1,
      -1,    -1,     1,    95,     3,     4,    98,    -1,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    -1,    25,    26,    27,    -1,
      -1,    30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    -1,    -1,     1,    -1,     3,     4,    -1,    98,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    23,    -1,    25,    26,
      27,    -1,    -1,    30,    31,    32,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    -1,    -1,    -1,     1,    95,     3,
       4,    98,    -1,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      -1,    25,    26,    27,    -1,    -1,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    -1,    -1,    -1,
       1,    95,     3,     4,    98,    -1,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    -1,    25,    26,    27,    -1,    -1,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,
       1,    -1,    -1,     4,     5,     6,     7,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    23,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    40,
      41,    42,    43,    -1,    95,    -1,    -1,    98,    49,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,     1,    -1,     3,    -1,    -1,    70,
      71,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    23,    24,    25,    26,
      -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     1,    -1,    -1,     4,     5,     6,
       7,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    23,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    40,    41,    42,    43,    -1,    95,    -1,
      -1,    -1,    49,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    -1,    -1,    -1,     1,    63,    64,     4,     5,
       6,     7,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    23,    -1,    -1,
       1,    27,    -1,     4,     5,     6,     7,    94,    95,    -1,
      -1,    -1,    -1,    -1,    40,    41,    42,    43,    -1,    20,
      21,    -1,    23,    49,    50,    -1,    27,    53,    -1,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    40,
      41,    42,    43,    -1,    70,    71,    72,    -1,    49,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,     3,    -1,    94,    70,
      71,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    23,    24,    25,    26,
       3,    -1,    -1,    94,    -1,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    24,    25,    26,    -1,     4,    -1,     6,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    23,    24,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    81,    82,
      83,    84,    85,    86,    87,    88,    89,     1,    67,     3,
       4,    -1,    95,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      -1,    25,    26,    27,    -1,    -1,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      44,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      84,    85,    86,    87,    88,    89,     1,    -1,     3,     4,
      -1,    -1,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    23,    -1,
      25,    26,    27,    -1,    -1,    30,    31,    32,    33,    34,
       1,    -1,     3,    -1,    -1,    40,    -1,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    25,    26,    -1,    -1,    63,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,
      85,    86,    87,    88,    89,     3,    -1,    -1,    -1,    -1,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    23,    24,    25,    26,    -1,
      81,    82,    83,    84,    85,    86,    87,    88,    89,     3,
      -1,    -1,    -1,    -1,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    84,    85,    86,    87,
      88,    89,     3,    -1,    -1,    -1,    -1,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    26,    -1,    81,    82,    83,
      84,    85,    86,    87,    88,    89,     3,    -1,    -1,    -1,
      -1,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    89
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   118,   119,   120,     0,     1,     4,     5,     6,     7,
      20,    21,    23,    27,    40,    41,    42,    43,    49,    50,
      53,    56,    63,    64,    70,    71,    94,   121,   122,   138,
     140,   144,   145,   157,   160,   161,   165,   167,   170,   171,
     172,   176,   180,   182,   186,   187,   212,   213,   231,   239,
     240,   248,   257,   277,   279,   290,   292,   314,   315,   316,
     358,   409,   410,   411,   412,   413,   417,   439,   441,   464,
     465,   466,   467,   468,   472,   473,   474,   477,   481,   489,
     502,   503,   137,   214,   139,   166,   249,   278,   291,   317,
     359,     3,   211,   265,   165,    53,   165,   180,   182,    53,
     172,   182,   183,   211,   211,   442,     3,    89,   207,   210,
     207,   490,   504,   211,    99,   141,   130,   146,   158,    96,
      96,   129,   101,   168,   162,   131,   173,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    24,
      25,    26,    81,    82,    83,    84,    85,    86,    87,    88,
     168,   207,   252,   253,   254,   255,   256,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   279,   290,
     292,   316,   325,   328,   331,   332,   335,   336,   339,   127,
     123,   121,    95,   241,   124,   280,    22,   128,   125,   126,
     132,   414,   133,   440,   134,   168,   475,   475,   135,   136,
      97,   506,    96,    17,   207,   218,   267,   270,   271,   272,
     273,   274,   332,   336,   207,   211,   250,   252,   211,   211,
     211,   211,   168,   211,   168,   177,   211,   211,   418,   211,
     208,    75,    89,    75,     3,   240,    97,    97,    96,     4,
       6,    20,    21,    23,    24,    27,    49,    53,    67,   482,
     483,   485,   240,   499,    96,    48,   184,    97,    96,    97,
       8,    11,     8,     9,    99,   329,   181,   100,   102,    99,
      99,    96,    96,   207,    96,    97,   293,    96,    96,    96,
      96,    97,    96,    97,   451,    96,   476,   469,   478,    96,
      96,   507,   215,   251,   318,   360,    97,   101,   420,   443,
     210,   209,   491,     3,   241,   232,   142,   218,    99,     3,
     147,   484,    73,    74,    75,    76,    77,    78,    79,    80,
      92,    93,   107,   108,   112,   113,   207,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   501,   100,
     169,   163,   174,     8,   220,   230,    99,    47,   185,   326,
     333,   337,   242,   281,   113,   415,   452,   185,    97,    97,
     509,   211,   211,   258,   261,   265,   266,   340,    97,    97,
     178,   421,   419,   101,   448,   210,    97,   505,   233,   119,
     120,     3,   100,   102,   228,   228,   228,   220,   104,   105,
     106,    90,    91,   107,   108,   109,   110,   111,   500,   159,
     204,   207,   193,   194,   188,   102,   330,   204,   230,   230,
     230,   113,   243,   240,   283,   285,   294,   422,   454,   470,
     479,    30,    31,    60,    65,    68,    69,   347,   348,   353,
     431,   433,   434,   498,   508,   510,   216,   341,   259,   319,
     361,   193,   207,   185,   449,   444,   492,   420,     7,    98,
     213,   218,   234,   236,   237,   275,   316,   143,   100,   148,
     485,   114,   222,   223,   224,   225,   225,   226,   226,   227,
     227,   227,   102,   211,   205,     1,    32,    33,   164,   195,
     213,   239,   248,   347,   358,   363,   368,   409,   410,    44,
      45,    46,   175,   189,   191,   192,   195,   239,   370,   220,
     240,   327,   334,   338,   210,   220,   244,   245,   247,     1,
     252,   286,   282,   284,   240,    51,    52,    61,   239,   347,
     416,   423,   431,   433,   436,   437,   438,   498,    44,    54,
     195,   453,   455,   458,   461,   193,   188,   349,   354,    19,
     207,   432,    58,   435,   207,   207,   513,   511,   512,   432,
     514,    98,   103,   240,   102,   240,   321,   324,   284,   179,
     207,   185,   494,   495,   235,    96,   211,    96,    98,     3,
      97,   240,   102,   203,    98,   199,   195,   196,   201,   200,
     202,    34,   207,   254,   332,   336,   369,   392,   197,   198,
     371,    98,   286,   189,   190,   100,   253,   100,   100,   100,
     103,   114,   102,   246,   289,   287,    98,   285,     8,   207,
     267,   272,   273,   274,   299,   316,   207,   207,   207,   423,
     429,    98,   424,   425,   426,   427,   428,   430,   211,   211,
      98,   456,   457,   471,   480,    31,   392,   210,     3,     3,
      96,    96,    96,   210,    96,   217,   115,   342,   344,   260,
       3,   320,   322,   362,    98,   445,   493,   239,   347,   431,
     433,   496,   250,    29,   238,   149,   501,   206,    96,    96,
      96,    96,    96,    96,   364,    96,    96,     3,    96,   220,
     210,   247,    96,   258,   295,   210,   210,   210,    96,    96,
      96,    96,    96,    96,    96,   459,   462,    96,    96,    98,
      98,   350,   355,   219,   345,   343,   261,    98,   102,    98,
      66,    98,   496,   497,    96,    96,    96,   220,    96,    72,
     122,   138,   151,   153,   154,   207,     3,   372,   246,   288,
     114,   113,   374,   374,   392,   262,   265,   230,   344,   323,
     446,    96,   207,   150,   152,    96,   365,   374,    96,   296,
     375,   376,   460,   463,   351,   356,   263,   346,   324,   207,
     155,    98,   153,   113,   383,   373,    97,   114,    35,   377,
     380,    38,   394,   394,   262,    55,   397,   102,   116,   447,
      99,   384,   385,   366,   394,   297,   381,   114,   378,   395,
     352,   398,   357,   264,    59,   450,     3,   486,   488,   114,
      35,    36,    37,   386,   389,   393,   394,     1,    28,    29,
     300,   302,   306,   308,   392,   102,   113,   394,   113,    62,
     400,   265,   207,   100,   487,   114,   387,   390,   367,   305,
     310,   309,   298,   301,   303,   307,   382,   379,   396,   399,
     401,   156,   102,   102,   392,    39,   403,    96,   220,   101,
      98,   302,   240,   308,   261,   380,   204,   204,   113,   211,
     488,   388,   391,   404,   311,   252,   312,   114,   114,   402,
     389,   261,   113,   101,   313,   304,   204,   405,   261,    96,
     114,    75,   406,   407,   114,   102,   408,    75
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   117,   118,   119,   119,   120,   120,   121,   121,   121,
     123,   122,   124,   122,   125,   122,   126,   122,   127,   122,
     128,   122,   129,   122,   130,   122,   131,   122,   132,   122,
     133,   122,   134,   122,   135,   122,   136,   122,   137,   122,
     139,   138,   141,   142,   143,   140,   144,   146,   147,   148,
     149,   150,   145,   151,   152,   152,   153,   153,   155,   156,
     154,   158,   159,   157,   160,   160,   162,   163,   164,   161,
     166,   165,   167,   167,   167,   169,   168,   168,   170,   170,
     170,   170,   171,   171,   173,   174,   175,   172,   177,   178,
     179,   176,   181,   180,   183,   182,   184,   184,   185,   185,
     186,   186,   187,   188,   188,   188,   189,   189,   190,   189,
     191,   191,   192,   193,   193,   194,   194,   196,   195,   197,
     195,   198,   195,   199,   195,   200,   195,   201,   195,   202,
     195,   203,   195,   204,   206,   205,   205,   207,   208,   207,
     209,   207,   210,   211,   212,   212,   212,   214,   215,   216,
     217,   213,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   219,   220,   221,   221,   222,   222,   223,   223,   224,
     224,   224,   225,   225,   225,   226,   226,   226,   226,   227,
     227,   227,   227,   228,   228,   228,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   230,   232,   231,   233,
     233,   234,   234,   234,   235,   234,   236,   236,   237,   238,
     238,   239,   240,   240,   242,   241,   243,   243,   244,   244,
     245,   246,   246,   247,   249,   248,   248,   248,   248,   248,
     248,   251,   250,   252,   252,   253,   253,   253,   254,   254,
     254,   254,   254,   254,   254,   254,   255,   255,   255,   255,
     256,   256,   256,   257,   257,   258,   260,   259,   259,   261,
     261,   262,   264,   263,   263,   265,   266,   267,   267,   268,
     268,   268,   268,   268,   268,   268,   269,   269,   269,   269,
     269,   269,   269,   270,   270,   270,   271,   272,   272,   273,
     274,   275,   276,   278,   277,   280,   281,   282,   279,   283,
     284,   284,   285,   287,   288,   286,   289,   286,   291,   290,
     293,   294,   295,   296,   297,   298,   292,   299,   299,   299,
     299,   299,   299,   300,   301,   301,   303,   304,   302,   305,
     302,   306,   307,   307,   309,   308,   310,   311,   308,   313,
     312,   314,   315,   317,   318,   319,   320,   316,   321,   323,
     322,   322,   324,   326,   327,   325,   325,   329,   330,   328,
     331,   333,   334,   332,   332,   335,   337,   338,   336,   336,
     339,   341,   340,   342,   343,   343,   345,   346,   344,   347,
     347,   349,   350,   351,   352,   348,   354,   355,   356,   357,
     353,   359,   360,   361,   362,   358,   364,   365,   366,   367,
     363,   368,   368,   368,   369,   369,   371,   372,   373,   370,
     375,   374,   376,   374,   377,   379,   378,   378,   381,   382,
     380,   384,   383,   385,   383,   386,   388,   387,   387,   390,
     391,   389,   392,   392,   392,   392,   393,   393,   393,   395,
     396,   394,   394,   398,   399,   397,   397,   401,   402,   400,
     400,   404,   405,   403,   403,   406,   408,   407,   407,   409,
     410,   411,   411,   412,   414,   415,   416,   413,   418,   419,
     417,   421,   420,   420,   422,   422,   422,   424,   423,   425,
     423,   426,   423,   427,   423,   428,   423,   429,   423,   430,
     423,   431,   432,   432,   433,   434,   435,   435,   436,   437,
     438,   440,   439,   442,   443,   444,   445,   446,   447,   441,
     449,   448,   448,   450,   450,   452,   453,   451,   454,   454,
     455,   456,   455,   457,   455,   459,   460,   458,   462,   463,
     461,   464,   464,   464,   465,   465,   466,   467,   469,   470,
     471,   468,   472,   473,   474,   476,   475,   478,   479,   480,
     477,   481,   481,   482,   482,   482,   482,   482,   482,   482,
     482,   482,   482,   483,   484,   484,   485,   485,   486,   487,
     487,   488,   490,   491,   492,   493,   489,   494,   494,   495,
     495,   496,   496,   497,   496,   498,   498,   499,   500,   500,
     501,   502,   504,   505,   503,   507,   508,   506,   509,   509,
     511,   510,   512,   510,   513,   510,   514,   510
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
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
       2,     1,     1,     1,     1,     1,     2,     3,     2,     1,
       1,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     0,     3,     0,     0,     0,     7,     2,
       2,     0,     2,     0,     0,     5,     0,     3,     0,     3,
       0,     0,     0,     0,     0,     0,    15,     1,     1,     1,
       1,     1,     1,     2,     2,     0,     0,     0,     6,     0,
       3,     2,     2,     0,     0,     3,     0,     0,     5,     0,
       3,     1,     1,     0,     0,     0,     0,     9,     2,     0,
       4,     0,     2,     0,     0,     6,     2,     0,     0,     6,
       6,     0,     0,     6,     1,     1,     0,     0,     6,     1,
       1,     0,     4,     2,     2,     0,     0,     0,     5,     1,
       1,     0,     0,     0,     0,     9,     0,     0,     0,     0,
       9,     0,     0,     0,     0,     9,     0,     0,     0,     0,
      10,     1,     1,     0,     1,     1,     0,     0,     0,     7,
       0,     3,     0,     4,     2,     0,     4,     0,     0,     0,
       5,     0,     3,     0,     4,     2,     0,     4,     0,     0,
       0,     5,     1,     1,     1,     1,     1,     1,     1,     0,
       0,     6,     0,     0,     0,     6,     0,     0,     0,     6,
       0,     0,     0,     6,     0,     2,     0,     4,     0,     3,
       3,     1,     1,     2,     0,     0,     0,     7,     0,     0,
       6,     0,     3,     0,     3,     2,     0,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     3,     1,     1,     3,     2,     1,     0,     3,     3,
       3,     0,     3,     0,     0,     0,     0,     0,     0,    13,
       0,     3,     0,     2,     0,     0,     0,     5,     2,     0,
       1,     0,     3,     0,     3,     0,     0,     6,     0,     0,
       6,     1,     1,     1,     1,     1,     2,     3,     0,     0,
       0,     8,     3,     3,     2,     0,     3,     0,     0,     0,
       8,     1,     1,     1,     1,     1,     1,     1,     1,     1,
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
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
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
  case 5: /* at_least_one_definition: definitions at_least_one_annotation definition  */
#line 427 "fe/idl.ypp"
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
#line 452 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AnnotationDeclSeen);
        }
#line 2749 "fe/idl.tab.cpp"
    break;

  case 11: /* fixed_definition: annotation_dcl $@1 ';'  */
#line 456 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2757 "fe/idl.tab.cpp"
    break;

  case 12: /* $@2: %empty  */
#line 460 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 2765 "fe/idl.tab.cpp"
    break;

  case 13: /* fixed_definition: type_dcl $@2 ';'  */
#line 464 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2773 "fe/idl.tab.cpp"
    break;

  case 14: /* $@3: %empty  */
#line 468 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 2781 "fe/idl.tab.cpp"
    break;

  case 15: /* fixed_definition: typeid_dcl $@3 ';'  */
#line 472 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2789 "fe/idl.tab.cpp"
    break;

  case 16: /* $@4: %empty  */
#line 476 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 2797 "fe/idl.tab.cpp"
    break;

  case 17: /* fixed_definition: typeprefix_dcl $@4 ';'  */
#line 480 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2805 "fe/idl.tab.cpp"
    break;

  case 18: /* $@5: %empty  */
#line 484 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 2813 "fe/idl.tab.cpp"
    break;

  case 19: /* fixed_definition: const_dcl $@5 ';'  */
#line 488 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2821 "fe/idl.tab.cpp"
    break;

  case 20: /* $@6: %empty  */
#line 492 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 2829 "fe/idl.tab.cpp"
    break;

  case 21: /* fixed_definition: exception $@6 ';'  */
#line 496 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2837 "fe/idl.tab.cpp"
    break;

  case 22: /* $@7: %empty  */
#line 500 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceDeclSeen);
        }
#line 2845 "fe/idl.tab.cpp"
    break;

  case 23: /* fixed_definition: interface_def $@7 ';'  */
#line 504 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2853 "fe/idl.tab.cpp"
    break;

  case 24: /* $@8: %empty  */
#line 508 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleDeclSeen);
        }
#line 2861 "fe/idl.tab.cpp"
    break;

  case 25: /* fixed_definition: module $@8 ';'  */
#line 512 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2869 "fe/idl.tab.cpp"
    break;

  case 26: /* $@9: %empty  */
#line 516 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeDeclSeen);
        }
#line 2877 "fe/idl.tab.cpp"
    break;

  case 27: /* fixed_definition: value_def $@9 ';'  */
#line 520 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2885 "fe/idl.tab.cpp"
    break;

  case 28: /* $@10: %empty  */
#line 524 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentDeclSeen);
        }
#line 2893 "fe/idl.tab.cpp"
    break;

  case 29: /* fixed_definition: component $@10 ';'  */
#line 528 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2901 "fe/idl.tab.cpp"
    break;

  case 30: /* $@11: %empty  */
#line 532 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeDeclSeen);
        }
#line 2909 "fe/idl.tab.cpp"
    break;

  case 31: /* fixed_definition: home_decl $@11 ';'  */
#line 536 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2917 "fe/idl.tab.cpp"
    break;

  case 32: /* $@12: %empty  */
#line 540 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventDeclSeen);
        }
#line 2925 "fe/idl.tab.cpp"
    break;

  case 33: /* fixed_definition: event $@12 ';'  */
#line 544 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2933 "fe/idl.tab.cpp"
    break;

  case 34: /* $@13: %empty  */
#line 548 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeDeclSeen);
        }
#line 2941 "fe/idl.tab.cpp"
    break;

  case 35: /* fixed_definition: porttype_decl $@13 ';'  */
#line 552 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2949 "fe/idl.tab.cpp"
    break;

  case 36: /* $@14: %empty  */
#line 556 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorDeclSeen);
        }
#line 2957 "fe/idl.tab.cpp"
    break;

  case 37: /* fixed_definition: connector_decl $@14 ';'  */
#line 560 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2965 "fe/idl.tab.cpp"
    break;

  case 38: /* $@15: %empty  */
#line 564 "fe/idl.ypp"
        {
          idl_global->err ()->syntax_error (idl_global->parse_state ());
        }
#line 2973 "fe/idl.tab.cpp"
    break;

  case 39: /* fixed_definition: error $@15 ';'  */
#line 568 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
          (yyval.dcval) = 0;
        }
#line 2983 "fe/idl.tab.cpp"
    break;

  case 40: /* $@16: %empty  */
#line 577 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSeen);
        }
#line 2991 "fe/idl.tab.cpp"
    break;

  case 41: /* module_header: IDL_MODULE $@16 scoped_name  */
#line 581 "fe/idl.ypp"
        {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 2999 "fe/idl.tab.cpp"
    break;

  case 42: /* @17: %empty  */
#line 588 "fe/idl.ypp"
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
#line 627 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSqSeen);
        }
#line 3050 "fe/idl.tab.cpp"
    break;

  case 44: /* $@19: %empty  */
#line 631 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleBodySeen);
        }
#line 3058 "fe/idl.tab.cpp"
    break;

  case 45: /* module: module_header @17 '{' $@18 at_least_one_definition $@19 '}'  */
#line 635 "fe/idl.ypp"
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
#line 648 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleIDSeen);
        }
#line 3080 "fe/idl.tab.cpp"
    break;

  case 47: /* $@20: %empty  */
#line 655 "fe/idl.ypp"
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
#line 669 "fe/idl.ypp"
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
#line 679 "fe/idl.ypp"
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
#line 709 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleSqSeen);
        }
#line 3154 "fe/idl.tab.cpp"
    break;

  case 51: /* $@24: %empty  */
#line 713 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleBodySeen);
        }
#line 3162 "fe/idl.tab.cpp"
    break;

  case 52: /* template_module: template_module_header $@20 at_least_one_formal_parameter $@21 '>' $@22 '{' $@23 at_least_one_tpl_definition $@24 '}'  */
#line 717 "fe/idl.ypp"
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
#line 753 "fe/idl.ypp"
        {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefSeen);
        }
#line 3194 "fe/idl.tab.cpp"
    break;

  case 59: /* $@26: %empty  */
#line 758 "fe/idl.ypp"
        {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefParamsSeen);
        }
#line 3203 "fe/idl.tab.cpp"
    break;

  case 60: /* template_module_ref: IDL_ALIAS scoped_name $@25 '<' at_least_one_formal_parameter_name '>' $@26 defining_id  */
#line 763 "fe/idl.ypp"
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
#line 3285 "fe/idl.tab.cpp"
    break;

  case 61: /* $@27: %empty  */
#line 844 "fe/idl.ypp"
        {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleSeen);
        }
#line 3294 "fe/idl.tab.cpp"
    break;

  case 62: /* $@28: %empty  */
#line 849 "fe/idl.ypp"
        {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleArgsSeen);
        }
#line 3303 "fe/idl.tab.cpp"
    break;

  case 63: /* template_module_inst: template_module_header $@27 at_least_one_actual_parameter '>' $@28 defining_id  */
#line 854 "fe/idl.ypp"
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
#line 926 "fe/idl.ypp"
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
#line 964 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
#line 3421 "fe/idl.tab.cpp"
    break;

  case 68: /* $@31: %empty  */
#line 968 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
#line 3429 "fe/idl.tab.cpp"
    break;

  case 69: /* interface: interface_header $@29 '{' $@30 exports $@31 '}'  */
#line 972 "fe/idl.ypp"
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
#line 985 "fe/idl.ypp"
         {
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSeen);
         }
#line 3451 "fe/idl.tab.cpp"
    break;

  case 71: /* interface_decl: IDL_INTERFACE $@32 defining_id  */
#line 989 "fe/idl.ypp"
         {
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceIDSeen);
           (yyval.idval) = (yyvsp[0].idval);
         }
#line 3460 "fe/idl.tab.cpp"
    break;

  case 72: /* interface_header: interface_decl inheritance_spec  */
#line 997 "fe/idl.ypp"
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
#line 1034 "fe/idl.ypp"
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
#line 1064 "fe/idl.ypp"
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
#line 1096 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 3574 "fe/idl.tab.cpp"
    break;

  case 76: /* inheritance_spec: ':' opt_truncatable $@33 at_least_one_scoped_name  */
#line 1100 "fe/idl.ypp"
        {
          (yyvsp[0].nlval)->truncatable ((yyvsp[-2].bval));
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3583 "fe/idl.tab.cpp"
    break;

  case 77: /* inheritance_spec: %empty  */
#line 1105 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 3591 "fe/idl.tab.cpp"
    break;

  case 82: /* valuetype: IDL_CUSTOM value_concrete_decl  */
#line 1119 "fe/idl.ypp"
        {
          idl_global->err ()->unsupported_error ("custom is not supported");
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 3600 "fe/idl.tab.cpp"
    break;

  case 84: /* @34: %empty  */
#line 1128 "fe/idl.ypp"
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
#line 1175 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3659 "fe/idl.tab.cpp"
    break;

  case 86: /* $@36: %empty  */
#line 1179 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3667 "fe/idl.tab.cpp"
    break;

  case 87: /* value_concrete_decl: value_header @34 '{' $@35 value_elements $@36 '}'  */
#line 1183 "fe/idl.ypp"
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
#line 1208 "fe/idl.ypp"
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
#line 1251 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3747 "fe/idl.tab.cpp"
    break;

  case 90: /* $@39: %empty  */
#line 1255 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3755 "fe/idl.tab.cpp"
    break;

  case 91: /* value_abs_decl: IDL_ABSTRACT value_header $@37 '{' $@38 exports $@39 '}'  */
#line 1259 "fe/idl.ypp"
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
#line 1274 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 3778 "fe/idl.tab.cpp"
    break;

  case 93: /* value_header: value_decl inheritance_spec $@40 supports_spec  */
#line 1278 "fe/idl.ypp"
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
#line 1312 "fe/idl.ypp"
        {
           idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSeen);
        }
#line 3821 "fe/idl.tab.cpp"
    break;

  case 95: /* value_decl: IDL_VALUETYPE $@41 defining_id  */
#line 1316 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 3830 "fe/idl.tab.cpp"
    break;

  case 96: /* opt_truncatable: IDL_TRUNCATABLE  */
#line 1324 "fe/idl.ypp"
        {
          (yyval.bval) = true;
        }
#line 3838 "fe/idl.tab.cpp"
    break;

  case 97: /* opt_truncatable: %empty  */
#line 1328 "fe/idl.ypp"
        {
          (yyval.bval) = false;
        }
#line 3846 "fe/idl.tab.cpp"
    break;

  case 98: /* supports_spec: IDL_SUPPORTS at_least_one_scoped_name  */
#line 1336 "fe/idl.ypp"
        {
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3854 "fe/idl.tab.cpp"
    break;

  case 99: /* supports_spec: %empty  */
#line 1340 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 3862 "fe/idl.tab.cpp"
    break;

  case 100: /* value_forward_decl: IDL_ABSTRACT value_decl  */
#line 1348 "fe/idl.ypp"
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
#line 1372 "fe/idl.ypp"
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
#line 1400 "fe/idl.ypp"
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
#line 1466 "fe/idl.ypp"
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
#line 1485 "fe/idl.ypp"
        {
          delete (yyvsp[0].decls_val);
        }
#line 4016 "fe/idl.tab.cpp"
    break;

  case 107: /* value_element: export  */
#line 1494 "fe/idl.ypp"
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
#line 1505 "fe/idl.ypp"
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
#line 1516 "fe/idl.ypp"
        {
          (yyval.decls_val) = (yyvsp[-1].decls_val);
        }
#line 4054 "fe/idl.tab.cpp"
    break;

  case 110: /* visibility: IDL_PUBLIC  */
#line 1523 "fe/idl.ypp"
        {
          (yyval.vival) = AST_Field::vis_PUBLIC;
        }
#line 4062 "fe/idl.tab.cpp"
    break;

  case 111: /* visibility: IDL_PRIVATE  */
#line 1527 "fe/idl.ypp"
        {
          (yyval.vival) = AST_Field::vis_PRIVATE;
        }
#line 4070 "fe/idl.tab.cpp"
    break;

  case 112: /* state_member: visibility member_i  */
#line 1534 "fe/idl.ypp"
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
#line 1560 "fe/idl.ypp"
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
#line 1579 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 4119 "fe/idl.tab.cpp"
    break;

  case 118: /* export: type_dcl $@43 ';'  */
#line 1583 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4127 "fe/idl.tab.cpp"
    break;

  case 119: /* $@44: %empty  */
#line 1587 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 4135 "fe/idl.tab.cpp"
    break;

  case 120: /* export: typeid_dcl $@44 ';'  */
#line 1591 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4143 "fe/idl.tab.cpp"
    break;

  case 121: /* $@45: %empty  */
#line 1595 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 4151 "fe/idl.tab.cpp"
    break;

  case 122: /* export: typeprefix_dcl $@45 ';'  */
#line 1599 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4159 "fe/idl.tab.cpp"
    break;

  case 123: /* $@46: %empty  */
#line 1603 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 4167 "fe/idl.tab.cpp"
    break;

  case 124: /* export: const_dcl $@46 ';'  */
#line 1607 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4175 "fe/idl.tab.cpp"
    break;

  case 125: /* $@47: %empty  */
#line 1611 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 4183 "fe/idl.tab.cpp"
    break;

  case 126: /* export: exception $@47 ';'  */
#line 1615 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4191 "fe/idl.tab.cpp"
    break;

  case 127: /* $@48: %empty  */
#line 1619 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 4199 "fe/idl.tab.cpp"
    break;

  case 128: /* export: attribute $@48 ';'  */
#line 1623 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4207 "fe/idl.tab.cpp"
    break;

  case 129: /* $@49: %empty  */
#line 1627 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpDeclSeen);
        }
#line 4215 "fe/idl.tab.cpp"
    break;

  case 130: /* export: operation $@49 ';'  */
#line 1631 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 4223 "fe/idl.tab.cpp"
    break;

  case 131: /* $@50: %empty  */
#line 1635 "fe/idl.ypp"
        {
          idl_global->err ()->syntax_error (idl_global->parse_state ());
        }
#line 4231 "fe/idl.tab.cpp"
    break;

  case 132: /* export: error $@50 ';'  */
#line 1639 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
          (yyval.dcval) = 0;
        }
#line 4241 "fe/idl.tab.cpp"
    break;

  case 133: /* at_least_one_scoped_name: scoped_name scoped_names  */
#line 1648 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.nlval),
                          UTL_NameList ((yyvsp[-1].idlist),
                                        (yyvsp[0].nlval)),
                          1);
        }
#line 4252 "fe/idl.tab.cpp"
    break;

  case 134: /* $@51: %empty  */
#line 1659 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SNListCommaSeen);
        }
#line 4260 "fe/idl.tab.cpp"
    break;

  case 135: /* scoped_names: scoped_names ',' $@51 scoped_name  */
#line 1663 "fe/idl.ypp"
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
#line 1683 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 4292 "fe/idl.tab.cpp"
    break;

  case 137: /* scoped_name: id  */
#line 1690 "fe/idl.ypp"
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
#line 1699 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);
        }
#line 4313 "fe/idl.tab.cpp"
    break;

  case 139: /* scoped_name: IDL_SCOPE_DELIMITOR $@52 id  */
#line 1703 "fe/idl.ypp"
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
#line 1724 "fe/idl.ypp"
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
#line 1734 "fe/idl.ypp"
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
#line 1748 "fe/idl.ypp"
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
#line 1758 "fe/idl.ypp"
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
#line 1769 "fe/idl.ypp"
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
#line 4438 "fe/idl.tab.cpp"
    break;

  case 145: /* interface_forward: IDL_LOCAL interface_decl  */
#line 1813 "fe/idl.ypp"
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
#line 1840 "fe/idl.ypp"
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
#line 1871 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstSeen);
        }
#line 4508 "fe/idl.tab.cpp"
    break;

  case 148: /* $@55: %empty  */
#line 1875 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstTypeSeen);
        }
#line 4516 "fe/idl.tab.cpp"
    break;

  case 149: /* $@56: %empty  */
#line 1879 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstIDSeen);
        }
#line 4524 "fe/idl.tab.cpp"
    break;

  case 150: /* $@57: %empty  */
#line 1883 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstAssignSeen);
        }
#line 4532 "fe/idl.tab.cpp"
    break;

  case 151: /* const_dcl: IDL_CONST $@54 const_type $@55 defining_id $@56 '=' $@57 expression  */
#line 1887 "fe/idl.ypp"
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
#line 4592 "fe/idl.tab.cpp"
    break;

  case 158: /* const_type: string_type_spec  */
#line 1952 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_string;
        }
#line 4600 "fe/idl.tab.cpp"
    break;

  case 159: /* const_type: wstring_type_spec  */
#line 1956 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_wstring;
        }
#line 4608 "fe/idl.tab.cpp"
    break;

  case 160: /* const_type: scoped_name  */
#line 1960 "fe/idl.ypp"
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
#line 2033 "fe/idl.ypp"
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
#line 2046 "fe/idl.ypp"
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
#line 2059 "fe/idl.ypp"
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
#line 2072 "fe/idl.ypp"
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
#line 2081 "fe/idl.ypp"
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
#line 2094 "fe/idl.ypp"
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
#line 2103 "fe/idl.ypp"
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
#line 2116 "fe/idl.ypp"
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
#line 2125 "fe/idl.ypp"
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
#line 2134 "fe/idl.ypp"
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
#line 2147 "fe/idl.ypp"
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
#line 2156 "fe/idl.ypp"
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
#line 2165 "fe/idl.ypp"
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
#line 2177 "fe/idl.ypp"
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
#line 2235 "fe/idl.ypp"
        {
          (yyval.exval) = (yyvsp[-1].exval);
        }
#line 4916 "fe/idl.tab.cpp"
    break;

  case 186: /* literal: IDL_INTEGER_LITERAL  */
#line 2242 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].ival));
        }
#line 4924 "fe/idl.tab.cpp"
    break;

  case 187: /* literal: IDL_UINTEGER_LITERAL  */
#line 2246 "fe/idl.ypp"
        {
          (yyval.exval) =
            idl_global->gen ()->create_expr ((yyvsp[0].uival));
        }
#line 4933 "fe/idl.tab.cpp"
    break;

  case 188: /* literal: IDL_STRING_LITERAL  */
#line 2251 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].sval));
          (yyvsp[0].sval)->destroy ();
          delete (yyvsp[0].sval);
          (yyvsp[0].sval) = 0;
        }
#line 4944 "fe/idl.tab.cpp"
    break;

  case 189: /* literal: IDL_WSTRING_LITERAL  */
#line 2258 "fe/idl.ypp"
        {
          char *wide_string = (yyvsp[0].wsval);
          (yyval.exval) = idl_global->gen ()->create_expr (wide_string);
          ACE_OS::free (wide_string);
          (yyvsp[0].wsval) = 0;
        }
#line 4955 "fe/idl.tab.cpp"
    break;

  case 190: /* literal: IDL_CHARACTER_LITERAL  */
#line 2265 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].cval));
        }
#line 4963 "fe/idl.tab.cpp"
    break;

  case 191: /* literal: IDL_WCHAR_LITERAL  */
#line 2269 "fe/idl.ypp"
        {
          ACE_OutputCDR::from_wchar wc ((yyvsp[0].wcval));
          (yyval.exval) = idl_global->gen ()->create_expr (wc);
        }
#line 4972 "fe/idl.tab.cpp"
    break;

  case 192: /* literal: IDL_FIXED_PT_LITERAL  */
#line 2274 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].fixval));
        }
#line 4980 "fe/idl.tab.cpp"
    break;

  case 193: /* literal: IDL_FLOATING_PT_LITERAL  */
#line 2278 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].dval));
        }
#line 4988 "fe/idl.tab.cpp"
    break;

  case 194: /* literal: IDL_TRUETOK  */
#line 2282 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr (true);
        }
#line 4996 "fe/idl.tab.cpp"
    break;

  case 195: /* literal: IDL_FALSETOK  */
#line 2286 "fe/idl.ypp"
        {
          (yyval.exval) = idl_global->gen ()->create_expr (false);
        }
#line 5004 "fe/idl.tab.cpp"
    break;

  case 196: /* positive_int_expr: const_expr  */
#line 2293 "fe/idl.ypp"
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
#line 2361 "fe/idl.ypp"
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
#line 5094 "fe/idl.tab.cpp"
    break;

  case 198: /* annotation_dcl: IDL_ANNOTATION_DECL defining_id '{' $@58 annotation_body '}'  */
#line 2378 "fe/idl.ypp"
        {
          Identifier *id = (yyvsp[-4].idval);
          idl_global->scopes ().pop ();
          id->destroy ();
          delete id;

          (yyval.dcval) = 0;
        }
#line 5107 "fe/idl.tab.cpp"
    break;

  case 204: /* $@59: %empty  */
#line 2398 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
          idl_global->in_typedef (true);
        }
#line 5116 "fe/idl.tab.cpp"
    break;

  case 208: /* annotation_member: annotation_member_type defining_id annotation_member_default ';'  */
#line 2412 "fe/idl.ypp"
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
#line 5173 "fe/idl.tab.cpp"
    break;

  case 209: /* annotation_member_default: IDL_DEFAULT const_expr  */
#line 2468 "fe/idl.ypp"
        {
          (yyval.exval) = (yyvsp[0].exval);
        }
#line 5181 "fe/idl.tab.cpp"
    break;

  case 210: /* annotation_member_default: %empty  */
#line 2472 "fe/idl.ypp"
        {
          (yyval.exval) = 0;
        }
#line 5189 "fe/idl.tab.cpp"
    break;

  case 211: /* at_least_one_annotation: annotations_maybe annotation_appl  */
#line 2479 "fe/idl.ypp"
        {
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          AST_Annotation_Appl *annotation = (yyvsp[0].annotation_val);
          if (annotation)
            {
              annotations->add (annotation);
            }
          (yyval.annotations_val) = annotations;
        }
#line 5203 "fe/idl.tab.cpp"
    break;

  case 212: /* annotations_maybe: annotations_maybe annotation_appl  */
#line 2492 "fe/idl.ypp"
        {
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          AST_Annotation_Appl *annotation = (yyvsp[0].annotation_val);
          if (annotation)
            {
              annotations->add (annotation);
            }
          (yyval.annotations_val) = annotations;
        }
#line 5217 "fe/idl.tab.cpp"
    break;

  case 213: /* annotations_maybe: %empty  */
#line 2502 "fe/idl.ypp"
        {
          (yyval.annotations_val) = new AST_Annotation_Appls ();
        }
#line 5225 "fe/idl.tab.cpp"
    break;

  case 214: /* @60: %empty  */
#line 2509 "fe/idl.ypp"
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
#line 5286 "fe/idl.tab.cpp"
    break;

  case 215: /* annotation_appl: IDL_ANNOTATION_SYMBOL scoped_name @60 annotation_appl_params_maybe  */
#line 2566 "fe/idl.ypp"
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
#line 5318 "fe/idl.tab.cpp"
    break;

  case 216: /* annotation_appl_params_maybe: '(' annotation_appl_params ')'  */
#line 2597 "fe/idl.ypp"
        {
          (yyval.annotation_params_val) = (yyvsp[-1].annotation_params_val);
        }
#line 5326 "fe/idl.tab.cpp"
    break;

  case 217: /* annotation_appl_params_maybe: %empty  */
#line 2601 "fe/idl.ypp"
        {
          (yyval.annotation_params_val) = 0;
        }
#line 5334 "fe/idl.tab.cpp"
    break;

  case 218: /* annotation_appl_params: const_expr  */
#line 2608 "fe/idl.ypp"
        {
          AST_Annotation_Appl::Params *params = new AST_Annotation_Appl::Params;
          AST_Annotation_Appl::Param *param = new AST_Annotation_Appl::Param;
          param->id = 0;
          param->expr = (yyvsp[0].exval);
          params->push (param);
          (yyval.annotation_params_val) = params;
        }
#line 5347 "fe/idl.tab.cpp"
    break;

  case 219: /* annotation_appl_params: named_annotation_appl_params  */
#line 2617 "fe/idl.ypp"
        {
          (yyval.annotation_params_val) = (yyvsp[0].annotation_params_val);
        }
#line 5355 "fe/idl.tab.cpp"
    break;

  case 220: /* named_annotation_appl_params: named_annotation_appl_param more_named_annotation_appl_params  */
#line 2624 "fe/idl.ypp"
        {
          AST_Annotation_Appl::Params *params = (yyvsp[0].annotation_params_val);
          params->push ((yyvsp[-1].annotation_param_val));
          (yyval.annotation_params_val) = params;
        }
#line 5365 "fe/idl.tab.cpp"
    break;

  case 221: /* more_named_annotation_appl_params: ',' named_annotation_appl_param more_named_annotation_appl_params  */
#line 2632 "fe/idl.ypp"
        {
          AST_Annotation_Appl::Params *params = (yyvsp[0].annotation_params_val);
          params->push ((yyvsp[-1].annotation_param_val));
          (yyval.annotation_params_val) = params;
        }
#line 5375 "fe/idl.tab.cpp"
    break;

  case 222: /* more_named_annotation_appl_params: %empty  */
#line 2638 "fe/idl.ypp"
        {
          (yyval.annotation_params_val) = new AST_Annotation_Appl::Params;
        }
#line 5383 "fe/idl.tab.cpp"
    break;

  case 223: /* named_annotation_appl_param: id '=' const_expr  */
#line 2645 "fe/idl.ypp"
        {
          AST_Annotation_Appl::Param *param = new AST_Annotation_Appl::Param;
          param->id = (yyvsp[-2].idval);
          // Annotation Member Names Can't Clash with C++ keywords
          FE_Utils::original_local_name (param->id);
          param->expr = (yyvsp[0].exval);
          (yyval.annotation_param_val) = param;
        }
#line 5396 "fe/idl.tab.cpp"
    break;

  case 224: /* $@61: %empty  */
#line 2657 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
          idl_global->in_typedef (true);
        }
#line 5405 "fe/idl.tab.cpp"
    break;

  case 225: /* type_dcl: IDL_TYPEDEF $@61 type_declarator  */
#line 2662 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5413 "fe/idl.tab.cpp"
    break;

  case 226: /* type_dcl: struct_type  */
#line 2666 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5421 "fe/idl.tab.cpp"
    break;

  case 227: /* type_dcl: union_type  */
#line 2670 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5429 "fe/idl.tab.cpp"
    break;

  case 228: /* type_dcl: enum_type  */
#line 2674 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5437 "fe/idl.tab.cpp"
    break;

  case 229: /* type_dcl: IDL_NATIVE simple_declarator  */
#line 2678 "fe/idl.ypp"
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
#line 5469 "fe/idl.tab.cpp"
    break;

  case 230: /* type_dcl: constructed_forward_type_spec  */
#line 2706 "fe/idl.ypp"
        {
          (yyval.dcval) = 0;
        }
#line 5477 "fe/idl.tab.cpp"
    break;

  case 231: /* $@62: %empty  */
#line 2713 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeSpecSeen);
        }
#line 5485 "fe/idl.tab.cpp"
    break;

  case 232: /* type_declarator: type_spec $@62 at_least_one_declarator  */
#line 2717 "fe/idl.ypp"
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
#line 5553 "fe/idl.tab.cpp"
    break;

  case 235: /* simple_type_spec: base_type_spec  */
#line 2789 "fe/idl.ypp"
        {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5564 "fe/idl.tab.cpp"
    break;

  case 237: /* simple_type_spec: scoped_name  */
#line 2797 "fe/idl.ypp"
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
#line 5591 "fe/idl.tab.cpp"
    break;

  case 255: /* at_least_one_declarator: declarator declarators  */
#line 2852 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 5602 "fe/idl.tab.cpp"
    break;

  case 256: /* $@63: %empty  */
#line 2863 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 5610 "fe/idl.tab.cpp"
    break;

  case 257: /* declarators: declarators ',' $@63 declarator  */
#line 2867 "fe/idl.ypp"
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
#line 5634 "fe/idl.tab.cpp"
    break;

  case 258: /* declarators: %empty  */
#line 2887 "fe/idl.ypp"
        {
          (yyval.dlval) = 0;
        }
#line 5642 "fe/idl.tab.cpp"
    break;

  case 261: /* at_least_one_simple_declarator: simple_declarator simple_declarators  */
#line 2899 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 5653 "fe/idl.tab.cpp"
    break;

  case 262: /* $@64: %empty  */
#line 2910 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 5661 "fe/idl.tab.cpp"
    break;

  case 263: /* simple_declarators: simple_declarators ',' $@64 simple_declarator  */
#line 2914 "fe/idl.ypp"
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
#line 5685 "fe/idl.tab.cpp"
    break;

  case 264: /* simple_declarators: %empty  */
#line 2934 "fe/idl.ypp"
        {
          (yyval.dlval) = 0;
        }
#line 5693 "fe/idl.tab.cpp"
    break;

  case 265: /* simple_declarator: defining_id  */
#line 2941 "fe/idl.ypp"
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
#line 5710 "fe/idl.tab.cpp"
    break;

  case 266: /* complex_declarator: array_declarator  */
#line 2957 "fe/idl.ypp"
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
#line 5729 "fe/idl.tab.cpp"
    break;

  case 269: /* signed_int: IDL_LONG  */
#line 2980 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_long;
        }
#line 5737 "fe/idl.tab.cpp"
    break;

  case 270: /* signed_int: IDL_LONG IDL_LONG  */
#line 2984 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_longlong;
        }
#line 5745 "fe/idl.tab.cpp"
    break;

  case 271: /* signed_int: IDL_SHORT  */
#line 2988 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_short;
        }
#line 5753 "fe/idl.tab.cpp"
    break;

  case 272: /* signed_int: IDL_INT8  */
#line 2992 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_int8;
        }
#line 5761 "fe/idl.tab.cpp"
    break;

  case 273: /* signed_int: IDL_INT16  */
#line 2996 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_short;
        }
#line 5769 "fe/idl.tab.cpp"
    break;

  case 274: /* signed_int: IDL_INT32  */
#line 3000 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_long;
        }
#line 5777 "fe/idl.tab.cpp"
    break;

  case 275: /* signed_int: IDL_INT64  */
#line 3004 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_longlong;
        }
#line 5785 "fe/idl.tab.cpp"
    break;

  case 276: /* unsigned_int: IDL_UNSIGNED IDL_LONG  */
#line 3011 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ulong;
        }
#line 5793 "fe/idl.tab.cpp"
    break;

  case 277: /* unsigned_int: IDL_UNSIGNED IDL_LONG IDL_LONG  */
#line 3015 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ulonglong;
        }
#line 5801 "fe/idl.tab.cpp"
    break;

  case 278: /* unsigned_int: IDL_UNSIGNED IDL_SHORT  */
#line 3019 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ushort;
        }
#line 5809 "fe/idl.tab.cpp"
    break;

  case 279: /* unsigned_int: IDL_UINT8  */
#line 3023 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_uint8;
        }
#line 5817 "fe/idl.tab.cpp"
    break;

  case 280: /* unsigned_int: IDL_UINT16  */
#line 3027 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ushort;
        }
#line 5825 "fe/idl.tab.cpp"
    break;

  case 281: /* unsigned_int: IDL_UINT32  */
#line 3031 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ulong;
        }
#line 5833 "fe/idl.tab.cpp"
    break;

  case 282: /* unsigned_int: IDL_UINT64  */
#line 3035 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_ulonglong;
        }
#line 5841 "fe/idl.tab.cpp"
    break;

  case 283: /* floating_pt_type: IDL_DOUBLE  */
#line 3042 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_double;
        }
#line 5849 "fe/idl.tab.cpp"
    break;

  case 284: /* floating_pt_type: IDL_FLOAT  */
#line 3046 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_float;
        }
#line 5857 "fe/idl.tab.cpp"
    break;

  case 285: /* floating_pt_type: IDL_LONG IDL_DOUBLE  */
#line 3050 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_longdouble;
        }
#line 5865 "fe/idl.tab.cpp"
    break;

  case 286: /* fixed_type: IDL_FIXED  */
#line 3057 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_fixed;
        }
#line 5873 "fe/idl.tab.cpp"
    break;

  case 287: /* char_type: IDL_CHAR  */
#line 3064 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_char;
        }
#line 5881 "fe/idl.tab.cpp"
    break;

  case 288: /* char_type: IDL_WCHAR  */
#line 3068 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_wchar;
        }
#line 5889 "fe/idl.tab.cpp"
    break;

  case 289: /* octet_type: IDL_OCTET  */
#line 3075 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_octet;
        }
#line 5897 "fe/idl.tab.cpp"
    break;

  case 290: /* boolean_type: IDL_BOOLEAN  */
#line 3082 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_bool;
        }
#line 5905 "fe/idl.tab.cpp"
    break;

  case 291: /* any_type: IDL_ANY  */
#line 3089 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_any;
        }
#line 5913 "fe/idl.tab.cpp"
    break;

  case 292: /* object_type: IDL_OBJECT  */
#line 3096 "fe/idl.ypp"
        {
          (yyval.etval) = AST_Expression::EV_object;
        }
#line 5921 "fe/idl.tab.cpp"
    break;

  case 293: /* $@65: %empty  */
#line 3103 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSeen);
        }
#line 5929 "fe/idl.tab.cpp"
    break;

  case 294: /* struct_decl: IDL_STRUCT $@65 defining_id  */
#line 3107 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 5938 "fe/idl.tab.cpp"
    break;

  case 295: /* $@66: %empty  */
#line 3116 "fe/idl.ypp"
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
#line 5973 "fe/idl.tab.cpp"
    break;

  case 296: /* $@67: %empty  */
#line 3147 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSqSeen);
        }
#line 5981 "fe/idl.tab.cpp"
    break;

  case 297: /* $@68: %empty  */
#line 3151 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructBodySeen);
        }
#line 5989 "fe/idl.tab.cpp"
    break;

  case 298: /* struct_type: struct_decl $@66 '{' $@67 at_least_one_member $@68 '}'  */
#line 3155 "fe/idl.ypp"
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
#line 6005 "fe/idl.tab.cpp"
    break;

  case 302: /* member: annotations_maybe member_i  */
#line 3177 "fe/idl.ypp"
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
#line 6023 "fe/idl.tab.cpp"
    break;

  case 303: /* $@69: %empty  */
#line 3194 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberTypeSeen);
        }
#line 6031 "fe/idl.tab.cpp"
    break;

  case 304: /* $@70: %empty  */
#line 3198 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsSeen);
        }
#line 6039 "fe/idl.tab.cpp"
    break;

  case 305: /* member_i: type_spec $@69 at_least_one_declarator $@70 ';'  */
#line 3202 "fe/idl.ypp"
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
#line 6097 "fe/idl.tab.cpp"
    break;

  case 306: /* $@71: %empty  */
#line 3256 "fe/idl.ypp"
        {
          idl_global->err ()->syntax_error (idl_global->parse_state ());
        }
#line 6105 "fe/idl.tab.cpp"
    break;

  case 307: /* member_i: error $@71 ';'  */
#line 3260 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 6114 "fe/idl.tab.cpp"
    break;

  case 308: /* $@72: %empty  */
#line 3268 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSeen);
        }
#line 6122 "fe/idl.tab.cpp"
    break;

  case 309: /* union_decl: IDL_UNION $@72 defining_id  */
#line 3272 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 6131 "fe/idl.tab.cpp"
    break;

  case 310: /* $@73: %empty  */
#line 3280 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchSeen);
        }
#line 6139 "fe/idl.tab.cpp"
    break;

  case 311: /* $@74: %empty  */
#line 3284 "fe/idl.ypp"
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
#line 6176 "fe/idl.tab.cpp"
    break;

  case 312: /* $@75: %empty  */
#line 3317 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchTypeSeen);
        }
#line 6184 "fe/idl.tab.cpp"
    break;

  case 313: /* $@76: %empty  */
#line 3321 "fe/idl.ypp"
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
#line 6247 "fe/idl.tab.cpp"
    break;

  case 314: /* $@77: %empty  */
#line 3380 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSqSeen);
        }
#line 6255 "fe/idl.tab.cpp"
    break;

  case 315: /* $@78: %empty  */
#line 3384 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionBodySeen);
        }
#line 6263 "fe/idl.tab.cpp"
    break;

  case 316: /* union_type: union_decl IDL_SWITCH $@73 '(' $@74 annotations_maybe switch_type_spec $@75 ')' $@76 '{' $@77 at_least_one_case_branch $@78 '}'  */
#line 3388 "fe/idl.ypp"
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
#line 6283 "fe/idl.tab.cpp"
    break;

  case 317: /* switch_type_spec: integer_type  */
#line 3407 "fe/idl.ypp"
        {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 6294 "fe/idl.tab.cpp"
    break;

  case 318: /* switch_type_spec: char_type  */
#line 3414 "fe/idl.ypp"
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
#line 6311 "fe/idl.tab.cpp"
    break;

  case 319: /* switch_type_spec: octet_type  */
#line 3427 "fe/idl.ypp"
        {
          /* octets are not allowed. */
          idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 6324 "fe/idl.tab.cpp"
    break;

  case 320: /* switch_type_spec: boolean_type  */
#line 3436 "fe/idl.ypp"
        {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 6335 "fe/idl.tab.cpp"
    break;

  case 322: /* switch_type_spec: scoped_name  */
#line 3444 "fe/idl.ypp"
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
#line 6446 "fe/idl.tab.cpp"
    break;

  case 326: /* $@79: %empty  */
#line 3561 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionLabelSeen);
        }
#line 6454 "fe/idl.tab.cpp"
    break;

  case 327: /* $@80: %empty  */
#line 3565 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemSeen);
        }
#line 6462 "fe/idl.tab.cpp"
    break;

  case 328: /* case_branch: at_least_one_case_label $@79 annotations_maybe element_spec $@80 ';'  */
#line 3569 "fe/idl.ypp"
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
#line 6498 "fe/idl.tab.cpp"
    break;

  case 329: /* $@81: %empty  */
#line 3601 "fe/idl.ypp"
        {
          idl_global->err ()->syntax_error (idl_global->parse_state ());
        }
#line 6506 "fe/idl.tab.cpp"
    break;

  case 330: /* case_branch: error $@81 ';'  */
#line 3605 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 6515 "fe/idl.tab.cpp"
    break;

  case 331: /* at_least_one_case_label: case_label case_labels  */
#line 3613 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.llval),
                          UTL_LabelList ((yyvsp[-1].ulval),
                                         (yyvsp[0].llval)),
                          1);
        }
#line 6526 "fe/idl.tab.cpp"
    break;

  case 332: /* case_labels: case_labels case_label  */
#line 3623 "fe/idl.ypp"
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
#line 6548 "fe/idl.tab.cpp"
    break;

  case 333: /* case_labels: %empty  */
#line 3641 "fe/idl.ypp"
        {
          (yyval.llval) = 0;
        }
#line 6556 "fe/idl.tab.cpp"
    break;

  case 334: /* $@82: %empty  */
#line 3648 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DefaultSeen);
        }
#line 6564 "fe/idl.tab.cpp"
    break;

  case 335: /* case_label: IDL_DEFAULT $@82 ':'  */
#line 3652 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen ()->create_union_label (
                                       AST_UnionLabel::UL_default,
                                       0
                                     );
        }
#line 6577 "fe/idl.tab.cpp"
    break;

  case 336: /* $@83: %empty  */
#line 3661 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_CaseSeen);
        }
#line 6585 "fe/idl.tab.cpp"
    break;

  case 337: /* $@84: %empty  */
#line 3665 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelExprSeen);
        }
#line 6593 "fe/idl.tab.cpp"
    break;

  case 338: /* case_label: IDL_CASE $@83 const_expr $@84 ':'  */
#line 3669 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen ()->create_union_label (
                                      AST_UnionLabel::UL_label,
                                      (yyvsp[-2].exval)
                                    );
        }
#line 6606 "fe/idl.tab.cpp"
    break;

  case 339: /* $@85: %empty  */
#line 3681 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemTypeSeen);
        }
#line 6614 "fe/idl.tab.cpp"
    break;

  case 340: /* element_spec: type_spec $@85 declarator  */
#line 3685 "fe/idl.ypp"
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
#line 6661 "fe/idl.tab.cpp"
    break;

  case 341: /* struct_forward_type: struct_decl  */
#line 3731 "fe/idl.ypp"
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
#line 6687 "fe/idl.tab.cpp"
    break;

  case 342: /* union_forward_type: union_decl  */
#line 3756 "fe/idl.ypp"
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
#line 6711 "fe/idl.tab.cpp"
    break;

  case 343: /* $@86: %empty  */
#line 3779 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSeen);
        }
#line 6719 "fe/idl.tab.cpp"
    break;

  case 344: /* $@87: %empty  */
#line 3783 "fe/idl.ypp"
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
#line 6754 "fe/idl.tab.cpp"
    break;

  case 345: /* $@88: %empty  */
#line 3814 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSqSeen);
        }
#line 6762 "fe/idl.tab.cpp"
    break;

  case 346: /* $@89: %empty  */
#line 3818 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumBodySeen);
        }
#line 6770 "fe/idl.tab.cpp"
    break;

  case 347: /* enum_type: IDL_ENUM $@86 defining_id $@87 '{' $@88 at_least_one_enumerator $@89 '}'  */
#line 3822 "fe/idl.ypp"
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
#line 6793 "fe/idl.tab.cpp"
    break;

  case 349: /* $@90: %empty  */
#line 3847 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumCommaSeen);
        }
#line 6801 "fe/idl.tab.cpp"
    break;

  case 352: /* enumerator: annotations_maybe IDENTIFIER  */
#line 3856 "fe/idl.ypp"
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
#line 6840 "fe/idl.tab.cpp"
    break;

  case 353: /* $@91: %empty  */
#line 3895 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceCommaSeen);
        }
#line 6848 "fe/idl.tab.cpp"
    break;

  case 354: /* $@92: %empty  */
#line 3899 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceExprSeen);
        }
#line 6856 "fe/idl.tab.cpp"
    break;

  case 355: /* sequence_type_spec: seq_head ',' $@91 positive_int_expr $@92 '>'  */
#line 3903 "fe/idl.ypp"
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
#line 6941 "fe/idl.tab.cpp"
    break;

  case 356: /* sequence_type_spec: seq_head '>'  */
#line 3985 "fe/idl.ypp"
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
#line 7007 "fe/idl.tab.cpp"
    break;

  case 357: /* $@93: %empty  */
#line 4050 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSeen);

          /*
           * Push a sequence marker on scopes stack.
           */
          idl_global->scopes ().push (0);
        }
#line 7020 "fe/idl.tab.cpp"
    break;

  case 358: /* $@94: %empty  */
#line 4059 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSqSeen);
        }
#line 7028 "fe/idl.tab.cpp"
    break;

  case 359: /* seq_head: IDL_SEQUENCE $@93 '<' $@94 annotations_maybe simple_type_spec  */
#line 4063 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceTypeSeen);
          Decl_Annotations_Pair *seq_head = new Decl_Annotations_Pair;
          seq_head->decl = (yyvsp[0].dcval);
          seq_head->annotations = (yyvsp[-1].annotations_val);
          (yyval.decl_annotations_pair_val) = seq_head;
        }
#line 7040 "fe/idl.tab.cpp"
    break;

  case 360: /* fixed_type_spec: IDL_FIXED '<' positive_int_expr ',' const_expr '>'  */
#line 4074 "fe/idl.ypp"
        {
          (yyvsp[-1].exval)->evaluate (AST_Expression::EK_positive_int);
          (yyval.dcval) = idl_global->gen ()->create_fixed ((yyvsp[-3].exval), (yyvsp[-1].exval));
        }
#line 7049 "fe/idl.tab.cpp"
    break;

  case 361: /* $@95: %empty  */
#line 4083 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 7057 "fe/idl.tab.cpp"
    break;

  case 362: /* $@96: %empty  */
#line 4087 "fe/idl.ypp"
        {
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 7065 "fe/idl.tab.cpp"
    break;

  case 363: /* string_type_spec: string_head '<' $@95 positive_int_expr $@96 '>'  */
#line 4091 "fe/idl.ypp"
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
#line 7108 "fe/idl.tab.cpp"
    break;

  case 364: /* string_type_spec: string_head  */
#line 4130 "fe/idl.ypp"
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
#line 7135 "fe/idl.tab.cpp"
    break;

  case 365: /* string_head: IDL_STRING  */
#line 4156 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 7143 "fe/idl.tab.cpp"
    break;

  case 366: /* $@97: %empty  */
#line 4164 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 7151 "fe/idl.tab.cpp"
    break;

  case 367: /* $@98: %empty  */
#line 4168 "fe/idl.ypp"
        {
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 7159 "fe/idl.tab.cpp"
    break;

  case 368: /* wstring_type_spec: wstring_head '<' $@97 positive_int_expr $@98 '>'  */
#line 4172 "fe/idl.ypp"
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
#line 7202 "fe/idl.tab.cpp"
    break;

  case 369: /* wstring_type_spec: wstring_head  */
#line 4211 "fe/idl.ypp"
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
#line 7229 "fe/idl.tab.cpp"
    break;

  case 370: /* wstring_head: IDL_WSTRING  */
#line 4237 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 7237 "fe/idl.tab.cpp"
    break;

  case 371: /* $@99: %empty  */
#line 4244 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayIDSeen);
        }
#line 7245 "fe/idl.tab.cpp"
    break;

  case 372: /* array_declarator: defining_id $@99 annotations_maybe at_least_one_array_dim  */
#line 4248 "fe/idl.ypp"
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
#line 7285 "fe/idl.tab.cpp"
    break;

  case 373: /* at_least_one_array_dim: array_dim array_dims  */
#line 4287 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.elval),
                          UTL_ExprList ((yyvsp[-1].exval),
                                        (yyvsp[0].elval)),
                          1);
        }
#line 7296 "fe/idl.tab.cpp"
    break;

  case 374: /* array_dims: array_dims array_dim  */
#line 4297 "fe/idl.ypp"
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
#line 7318 "fe/idl.tab.cpp"
    break;

  case 375: /* array_dims: %empty  */
#line 4315 "fe/idl.ypp"
        {
          (yyval.elval) = 0;
        }
#line 7326 "fe/idl.tab.cpp"
    break;

  case 376: /* $@100: %empty  */
#line 4322 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimSqSeen);
        }
#line 7334 "fe/idl.tab.cpp"
    break;

  case 377: /* $@101: %empty  */
#line 4326 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimExprSeen);
        }
#line 7342 "fe/idl.tab.cpp"
    break;

  case 378: /* array_dim: '[' $@100 positive_int_expr $@101 ']'  */
#line 4330 "fe/idl.ypp"
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
#line 7400 "fe/idl.tab.cpp"
    break;

  case 381: /* $@102: %empty  */
#line 4392 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrROSeen);
        }
#line 7408 "fe/idl.tab.cpp"
    break;

  case 382: /* $@103: %empty  */
#line 4396 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 7416 "fe/idl.tab.cpp"
    break;

  case 383: /* $@104: %empty  */
#line 4400 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 7424 "fe/idl.tab.cpp"
    break;

  case 384: /* $@105: %empty  */
#line 4404 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 7432 "fe/idl.tab.cpp"
    break;

  case 385: /* attribute_readonly: IDL_READONLY $@102 IDL_ATTRIBUTE $@103 param_type_spec $@104 at_least_one_simple_declarator $@105 opt_raises  */
#line 4408 "fe/idl.ypp"
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
#line 7488 "fe/idl.tab.cpp"
    break;

  case 386: /* $@106: %empty  */
#line 4463 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 7496 "fe/idl.tab.cpp"
    break;

  case 387: /* $@107: %empty  */
#line 4467 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 7504 "fe/idl.tab.cpp"
    break;

  case 388: /* $@108: %empty  */
#line 4471 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 7512 "fe/idl.tab.cpp"
    break;

  case 389: /* $@109: %empty  */
#line 4475 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseCompleted);
        }
#line 7520 "fe/idl.tab.cpp"
    break;

  case 390: /* attribute_readwrite: IDL_ATTRIBUTE $@106 param_type_spec $@107 at_least_one_simple_declarator $@108 opt_getraises $@109 opt_setraises  */
#line 4479 "fe/idl.ypp"
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
#line 7585 "fe/idl.tab.cpp"
    break;

  case 391: /* $@110: %empty  */
#line 4543 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSeen);
        }
#line 7593 "fe/idl.tab.cpp"
    break;

  case 392: /* @111: %empty  */
#line 4547 "fe/idl.ypp"
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
#line 7629 "fe/idl.tab.cpp"
    break;

  case 393: /* $@112: %empty  */
#line 4579 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSqSeen);
        }
#line 7637 "fe/idl.tab.cpp"
    break;

  case 394: /* $@113: %empty  */
#line 4583 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptBodySeen);
        }
#line 7645 "fe/idl.tab.cpp"
    break;

  case 395: /* exception: IDL_EXCEPTION $@110 defining_id @111 '{' $@112 members $@113 '}'  */
#line 4587 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptQsSeen);
          /*
           * Done with this exception. Pop its scope from the scope stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 7659 "fe/idl.tab.cpp"
    break;

  case 396: /* $@114: %empty  */
#line 4600 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 7667 "fe/idl.tab.cpp"
    break;

  case 397: /* $@115: %empty  */
#line 4604 "fe/idl.ypp"
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
#line 7732 "fe/idl.tab.cpp"
    break;

  case 398: /* $@116: %empty  */
#line 4665 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 7740 "fe/idl.tab.cpp"
    break;

  case 399: /* $@117: %empty  */
#line 4669 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);
        }
#line 7748 "fe/idl.tab.cpp"
    break;

  case 400: /* operation: opt_op_attribute op_type_spec $@114 IDENTIFIER $@115 parameter_list $@116 opt_raises $@117 opt_context  */
#line 4673 "fe/idl.ypp"
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
#line 7783 "fe/idl.tab.cpp"
    break;

  case 401: /* opt_op_attribute: IDL_ONEWAY  */
#line 4707 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_oneway;
        }
#line 7792 "fe/idl.tab.cpp"
    break;

  case 402: /* opt_op_attribute: IDL_IDEMPOTENT  */
#line 4712 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_idempotent;
        }
#line 7801 "fe/idl.tab.cpp"
    break;

  case 403: /* opt_op_attribute: %empty  */
#line 4717 "fe/idl.ypp"
        {
          (yyval.ofval) = AST_Operation::OP_noflags;
        }
#line 7809 "fe/idl.tab.cpp"
    break;

  case 405: /* op_type_spec: IDL_VOID  */
#line 4725 "fe/idl.ypp"
        {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                  AST_Expression::EV_void
                                                );
        }
#line 7820 "fe/idl.tab.cpp"
    break;

  case 406: /* $@118: %empty  */
#line 4735 "fe/idl.ypp"
        {
          //@@ PS_FactorySeen?
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 7829 "fe/idl.tab.cpp"
    break;

  case 407: /* @119: %empty  */
#line 4740 "fe/idl.ypp"
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
#line 7876 "fe/idl.tab.cpp"
    break;

  case 408: /* $@120: %empty  */
#line 4783 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 7884 "fe/idl.tab.cpp"
    break;

  case 409: /* init_decl: IDL_FACTORY $@118 IDENTIFIER @119 init_parameter_list $@120 opt_raises  */
#line 4787 "fe/idl.ypp"
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
#line 7903 "fe/idl.tab.cpp"
    break;

  case 410: /* $@121: %empty  */
#line 4805 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7911 "fe/idl.tab.cpp"
    break;

  case 411: /* init_parameter_list: '(' $@121 ')'  */
#line 4809 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7919 "fe/idl.tab.cpp"
    break;

  case 412: /* $@122: %empty  */
#line 4813 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7927 "fe/idl.tab.cpp"
    break;

  case 413: /* init_parameter_list: '(' $@122 at_least_one_in_parameter ')'  */
#line 4818 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7935 "fe/idl.tab.cpp"
    break;

  case 415: /* $@123: %empty  */
#line 4828 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 7943 "fe/idl.tab.cpp"
    break;

  case 418: /* $@124: %empty  */
#line 4837 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 7951 "fe/idl.tab.cpp"
    break;

  case 419: /* $@125: %empty  */
#line 4841 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 7959 "fe/idl.tab.cpp"
    break;

  case 420: /* in_parameter: IDL_IN $@124 param_type_spec $@125 declarator  */
#line 4845 "fe/idl.ypp"
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
#line 7995 "fe/idl.tab.cpp"
    break;

  case 421: /* $@126: %empty  */
#line 4880 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 8003 "fe/idl.tab.cpp"
    break;

  case 422: /* parameter_list: '(' $@126 ')'  */
#line 4884 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 8011 "fe/idl.tab.cpp"
    break;

  case 423: /* $@127: %empty  */
#line 4888 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 8019 "fe/idl.tab.cpp"
    break;

  case 424: /* parameter_list: '(' $@127 at_least_one_parameter ')'  */
#line 4893 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 8027 "fe/idl.tab.cpp"
    break;

  case 426: /* $@128: %empty  */
#line 4903 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 8035 "fe/idl.tab.cpp"
    break;

  case 429: /* $@129: %empty  */
#line 4912 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 8043 "fe/idl.tab.cpp"
    break;

  case 430: /* $@130: %empty  */
#line 4916 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 8051 "fe/idl.tab.cpp"
    break;

  case 431: /* parameter: direction $@129 param_type_spec $@130 declarator  */
#line 4920 "fe/idl.ypp"
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
#line 8094 "fe/idl.tab.cpp"
    break;

  case 432: /* param_type_spec: base_type_spec  */
#line 4962 "fe/idl.ypp"
        {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 8105 "fe/idl.tab.cpp"
    break;

  case 435: /* param_type_spec: scoped_name  */
#line 4971 "fe/idl.ypp"
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
#line 8213 "fe/idl.tab.cpp"
    break;

  case 436: /* direction: IDL_IN  */
#line 5078 "fe/idl.ypp"
        {
          (yyval.dival) = AST_Argument::dir_IN;
        }
#line 8221 "fe/idl.tab.cpp"
    break;

  case 437: /* direction: IDL_OUT  */
#line 5082 "fe/idl.ypp"
        {
          (yyval.dival) = AST_Argument::dir_OUT;
        }
#line 8229 "fe/idl.tab.cpp"
    break;

  case 438: /* direction: IDL_INOUT  */
#line 5086 "fe/idl.ypp"
        {
          (yyval.dival) = AST_Argument::dir_INOUT;
        }
#line 8237 "fe/idl.tab.cpp"
    break;

  case 439: /* $@131: %empty  */
#line 5093 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSeen);
        }
#line 8245 "fe/idl.tab.cpp"
    break;

  case 440: /* $@132: %empty  */
#line 5097 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSqSeen);
        }
#line 8253 "fe/idl.tab.cpp"
    break;

  case 441: /* opt_raises: IDL_RAISES $@131 '(' $@132 at_least_one_scoped_name ')'  */
#line 5102 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 8262 "fe/idl.tab.cpp"
    break;

  case 442: /* opt_raises: %empty  */
#line 5107 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 8270 "fe/idl.tab.cpp"
    break;

  case 443: /* $@133: %empty  */
#line 5114 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSeen);
        }
#line 8278 "fe/idl.tab.cpp"
    break;

  case 444: /* $@134: %empty  */
#line 5118 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSqSeen);
        }
#line 8286 "fe/idl.tab.cpp"
    break;

  case 445: /* opt_getraises: IDL_GETRAISES $@133 '(' $@134 at_least_one_scoped_name ')'  */
#line 5123 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 8295 "fe/idl.tab.cpp"
    break;

  case 446: /* opt_getraises: %empty  */
#line 5128 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 8303 "fe/idl.tab.cpp"
    break;

  case 447: /* $@135: %empty  */
#line 5135 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSeen);
        }
#line 8311 "fe/idl.tab.cpp"
    break;

  case 448: /* $@136: %empty  */
#line 5139 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSqSeen);
        }
#line 8319 "fe/idl.tab.cpp"
    break;

  case 449: /* opt_setraises: IDL_SETRAISES $@135 '(' $@136 at_least_one_scoped_name ')'  */
#line 5144 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 8328 "fe/idl.tab.cpp"
    break;

  case 450: /* opt_setraises: %empty  */
#line 5149 "fe/idl.ypp"
        {
          (yyval.nlval) = 0;
        }
#line 8336 "fe/idl.tab.cpp"
    break;

  case 451: /* $@137: %empty  */
#line 5156 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSeen);
        }
#line 8344 "fe/idl.tab.cpp"
    break;

  case 452: /* $@138: %empty  */
#line 5160 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSqSeen);
        }
#line 8352 "fe/idl.tab.cpp"
    break;

  case 453: /* opt_context: IDL_CONTEXT $@137 '(' $@138 at_least_one_string_literal ')'  */
#line 5165 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextQsSeen);
          (yyval.slval) = (yyvsp[-1].slval);
        }
#line 8361 "fe/idl.tab.cpp"
    break;

  case 454: /* opt_context: %empty  */
#line 5170 "fe/idl.ypp"
        {
          (yyval.slval) = 0;
        }
#line 8369 "fe/idl.tab.cpp"
    break;

  case 455: /* at_least_one_string_literal: IDL_STRING_LITERAL string_literals  */
#line 5177 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.slval),
                          UTL_StrList ((yyvsp[-1].sval),
                                       (yyvsp[0].slval)),
                          1);
        }
#line 8380 "fe/idl.tab.cpp"
    break;

  case 456: /* $@139: %empty  */
#line 5188 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextCommaSeen);
        }
#line 8388 "fe/idl.tab.cpp"
    break;

  case 457: /* string_literals: string_literals ',' $@139 IDL_STRING_LITERAL  */
#line 5192 "fe/idl.ypp"
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
#line 8410 "fe/idl.tab.cpp"
    break;

  case 458: /* string_literals: %empty  */
#line 5210 "fe/idl.ypp"
        {
          (yyval.slval) = 0;
        }
#line 8418 "fe/idl.tab.cpp"
    break;

  case 459: /* typeid_dcl: IDL_TYPEID scoped_name IDL_STRING_LITERAL  */
#line 5217 "fe/idl.ypp"
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
#line 8449 "fe/idl.tab.cpp"
    break;

  case 460: /* typeprefix_dcl: IDL_TYPEPREFIX scoped_name IDL_STRING_LITERAL  */
#line 5247 "fe/idl.ypp"
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
#line 8489 "fe/idl.tab.cpp"
    break;

  case 463: /* component_forward_decl: IDL_COMPONENT defining_id  */
#line 5292 "fe/idl.ypp"
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
#line 8519 "fe/idl.tab.cpp"
    break;

  case 464: /* @140: %empty  */
#line 5321 "fe/idl.ypp"
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
#line 8565 "fe/idl.tab.cpp"
    break;

  case 465: /* $@141: %empty  */
#line 5363 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentSqSeen);
        }
#line 8573 "fe/idl.tab.cpp"
    break;

  case 466: /* $@142: %empty  */
#line 5367 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentBodySeen);
        }
#line 8581 "fe/idl.tab.cpp"
    break;

  case 467: /* component_decl: component_header @140 '{' $@141 component_exports $@142 '}'  */
#line 5371 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentQsSeen);

          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 8596 "fe/idl.tab.cpp"
    break;

  case 468: /* $@143: %empty  */
#line 5386 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentIDSeen);
        }
#line 8604 "fe/idl.tab.cpp"
    break;

  case 469: /* $@144: %empty  */
#line 5390 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 8612 "fe/idl.tab.cpp"
    break;

  case 470: /* component_header: IDL_COMPONENT defining_id $@143 component_inheritance_spec $@144 supports_spec  */
#line 5394 "fe/idl.ypp"
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
#line 8650 "fe/idl.tab.cpp"
    break;

  case 471: /* $@145: %empty  */
#line 5431 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 8658 "fe/idl.tab.cpp"
    break;

  case 472: /* component_inheritance_spec: ':' $@145 scoped_name  */
#line 5435 "fe/idl.ypp"
        {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8666 "fe/idl.tab.cpp"
    break;

  case 473: /* component_inheritance_spec: %empty  */
#line 5439 "fe/idl.ypp"
        {
          (yyval.idlist) = 0;
        }
#line 8674 "fe/idl.tab.cpp"
    break;

  case 474: /* component_exports: component_exports at_least_one_annotation component_export  */
#line 5446 "fe/idl.ypp"
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
#line 8693 "fe/idl.tab.cpp"
    break;

  case 477: /* $@146: %empty  */
#line 5466 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 8701 "fe/idl.tab.cpp"
    break;

  case 478: /* component_export: provides_decl $@146 ';'  */
#line 5470 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8710 "fe/idl.tab.cpp"
    break;

  case 479: /* $@147: %empty  */
#line 5475 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 8718 "fe/idl.tab.cpp"
    break;

  case 480: /* component_export: uses_decl $@147 ';'  */
#line 5479 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8727 "fe/idl.tab.cpp"
    break;

  case 481: /* $@148: %empty  */
#line 5484 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EmitsDeclSeen);
        }
#line 8735 "fe/idl.tab.cpp"
    break;

  case 482: /* component_export: emits_decl $@148 ';'  */
#line 5488 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8744 "fe/idl.tab.cpp"
    break;

  case 483: /* $@149: %empty  */
#line 5493 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PublishesDeclSeen);
        }
#line 8752 "fe/idl.tab.cpp"
    break;

  case 484: /* component_export: publishes_decl $@149 ';'  */
#line 5497 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8761 "fe/idl.tab.cpp"
    break;

  case 485: /* $@150: %empty  */
#line 5502 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConsumesDeclSeen);
        }
#line 8769 "fe/idl.tab.cpp"
    break;

  case 486: /* component_export: consumes_decl $@150 ';'  */
#line 5506 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8778 "fe/idl.tab.cpp"
    break;

  case 487: /* $@151: %empty  */
#line 5511 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 8786 "fe/idl.tab.cpp"
    break;

  case 488: /* component_export: attribute $@151 ';'  */
#line 5515 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8795 "fe/idl.tab.cpp"
    break;

  case 489: /* $@152: %empty  */
#line 5520 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 8803 "fe/idl.tab.cpp"
    break;

  case 490: /* component_export: extended_port_decl $@152 ';'  */
#line 5524 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 8812 "fe/idl.tab.cpp"
    break;

  case 491: /* provides_decl: IDL_PROVIDES interface_type id  */
#line 5531 "fe/idl.ypp"
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
#line 8906 "fe/idl.tab.cpp"
    break;

  case 492: /* interface_type: scoped_name  */
#line 5624 "fe/idl.ypp"
        {
          // Lookups and checking are done where the 'interface_type'
          // token is used, in 'provides_decl' and 'uses_decl'.
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8916 "fe/idl.tab.cpp"
    break;

  case 493: /* interface_type: IDL_OBJECT  */
#line 5630 "fe/idl.ypp"
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
#line 8943 "fe/idl.tab.cpp"
    break;

  case 494: /* uses_decl: uses_opt_multiple interface_type id  */
#line 5655 "fe/idl.ypp"
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
#line 9051 "fe/idl.tab.cpp"
    break;

  case 495: /* uses_opt_multiple: IDL_USES opt_multiple  */
#line 5762 "fe/idl.ypp"
        {
          // We use this extra rule here to use in both uses_decl and
          // extended_uses_decl, so the LALR(1) parser can avoid conflicts.
          (yyval.bval) = (yyvsp[0].bval);
        }
#line 9061 "fe/idl.tab.cpp"
    break;

  case 496: /* opt_multiple: IDL_MULTIPLE  */
#line 5771 "fe/idl.ypp"
        {
          (yyval.bval) = true;
        }
#line 9069 "fe/idl.tab.cpp"
    break;

  case 497: /* opt_multiple: %empty  */
#line 5775 "fe/idl.ypp"
        {
          (yyval.bval) = false;
        }
#line 9077 "fe/idl.tab.cpp"
    break;

  case 498: /* emits_decl: IDL_EMITS scoped_name id  */
#line 5782 "fe/idl.ypp"
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
#line 9153 "fe/idl.tab.cpp"
    break;

  case 499: /* publishes_decl: IDL_PUBLISHES scoped_name id  */
#line 5857 "fe/idl.ypp"
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
#line 9226 "fe/idl.tab.cpp"
    break;

  case 500: /* consumes_decl: IDL_CONSUMES scoped_name id  */
#line 5929 "fe/idl.ypp"
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
#line 9302 "fe/idl.tab.cpp"
    break;

  case 501: /* $@153: %empty  */
#line 6004 "fe/idl.ypp"
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
#line 9343 "fe/idl.tab.cpp"
    break;

  case 502: /* home_decl: home_header $@153 home_body  */
#line 6041 "fe/idl.ypp"
        {
          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = 0;
        }
#line 9356 "fe/idl.tab.cpp"
    break;

  case 503: /* $@154: %empty  */
#line 6053 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSeen);
        }
#line 9364 "fe/idl.tab.cpp"
    break;

  case 504: /* $@155: %empty  */
#line 6057 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeIDSeen);
        }
#line 9372 "fe/idl.tab.cpp"
    break;

  case 505: /* $@156: %empty  */
#line 6061 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 9380 "fe/idl.tab.cpp"
    break;

  case 506: /* $@157: %empty  */
#line 6065 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);
        }
#line 9388 "fe/idl.tab.cpp"
    break;

  case 507: /* $@158: %empty  */
#line 6069 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesSeen);
        }
#line 9396 "fe/idl.tab.cpp"
    break;

  case 508: /* $@159: %empty  */
#line 6073 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesIDSeen);
        }
#line 9404 "fe/idl.tab.cpp"
    break;

  case 509: /* home_header: IDL_HOME $@154 defining_id $@155 home_inheritance_spec $@156 supports_spec $@157 IDL_MANAGES $@158 scoped_name $@159 primary_key_spec  */
#line 6077 "fe/idl.ypp"
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
#line 9454 "fe/idl.tab.cpp"
    break;

  case 510: /* $@160: %empty  */
#line 6126 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 9462 "fe/idl.tab.cpp"
    break;

  case 511: /* home_inheritance_spec: ':' $@160 scoped_name  */
#line 6130 "fe/idl.ypp"
        {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 9470 "fe/idl.tab.cpp"
    break;

  case 512: /* home_inheritance_spec: %empty  */
#line 6134 "fe/idl.ypp"
        {
          (yyval.idlist) = 0;
        }
#line 9478 "fe/idl.tab.cpp"
    break;

  case 513: /* primary_key_spec: IDL_PRIMARYKEY scoped_name  */
#line 6142 "fe/idl.ypp"
        {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 9486 "fe/idl.tab.cpp"
    break;

  case 514: /* primary_key_spec: %empty  */
#line 6146 "fe/idl.ypp"
        {
          (yyval.idlist) = 0;
        }
#line 9494 "fe/idl.tab.cpp"
    break;

  case 515: /* $@161: %empty  */
#line 6153 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSqSeen);
        }
#line 9502 "fe/idl.tab.cpp"
    break;

  case 516: /* $@162: %empty  */
#line 6157 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeBodySeen);
        }
#line 9510 "fe/idl.tab.cpp"
    break;

  case 517: /* home_body: '{' $@161 home_exports $@162 '}'  */
#line 6161 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeQsSeen);
        }
#line 9518 "fe/idl.tab.cpp"
    break;

  case 521: /* $@163: %empty  */
#line 6174 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_FactoryDeclSeen);
        }
#line 9526 "fe/idl.tab.cpp"
    break;

  case 522: /* home_export: factory_decl $@163 ';'  */
#line 6178 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 9534 "fe/idl.tab.cpp"
    break;

  case 523: /* $@164: %empty  */
#line 6182 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_FinderDeclSeen);
        }
#line 9542 "fe/idl.tab.cpp"
    break;

  case 524: /* home_export: finder_decl $@164 ';'  */
#line 6186 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 9550 "fe/idl.tab.cpp"
    break;

  case 525: /* $@165: %empty  */
#line 6194 "fe/idl.ypp"
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
#line 9577 "fe/idl.tab.cpp"
    break;

  case 526: /* $@166: %empty  */
#line 6217 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 9585 "fe/idl.tab.cpp"
    break;

  case 527: /* factory_decl: IDL_FACTORY defining_id $@165 init_parameter_list $@166 opt_raises  */
#line 6221 "fe/idl.ypp"
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
#line 9607 "fe/idl.tab.cpp"
    break;

  case 528: /* $@167: %empty  */
#line 6243 "fe/idl.ypp"
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
#line 9638 "fe/idl.tab.cpp"
    break;

  case 529: /* $@168: %empty  */
#line 6270 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 9646 "fe/idl.tab.cpp"
    break;

  case 530: /* finder_decl: IDL_FINDER defining_id $@167 init_parameter_list $@168 opt_raises  */
#line 6274 "fe/idl.ypp"
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
#line 9668 "fe/idl.tab.cpp"
    break;

  case 536: /* event_concrete_forward_decl: IDL_EVENTTYPE defining_id  */
#line 6307 "fe/idl.ypp"
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
#line 9697 "fe/idl.tab.cpp"
    break;

  case 537: /* event_abs_forward_decl: IDL_ABSTRACT IDL_EVENTTYPE defining_id  */
#line 6337 "fe/idl.ypp"
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
#line 9726 "fe/idl.tab.cpp"
    break;

  case 538: /* $@169: %empty  */
#line 6366 "fe/idl.ypp"
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
#line 9774 "fe/idl.tab.cpp"
    break;

  case 539: /* $@170: %empty  */
#line 6410 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 9782 "fe/idl.tab.cpp"
    break;

  case 540: /* $@171: %empty  */
#line 6414 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 9790 "fe/idl.tab.cpp"
    break;

  case 541: /* event_abs_decl: event_abs_header event_rest_of_header $@169 '{' $@170 exports $@171 '}'  */
#line 6418 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();

          (yyval.dcval) = 0;
        }
#line 9805 "fe/idl.tab.cpp"
    break;

  case 542: /* event_abs_header: IDL_ABSTRACT IDL_EVENTTYPE defining_id  */
#line 6434 "fe/idl.ypp"
        {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9813 "fe/idl.tab.cpp"
    break;

  case 543: /* event_custom_header: IDL_CUSTOM IDL_EVENTTYPE defining_id  */
#line 6443 "fe/idl.ypp"
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
#line 9830 "fe/idl.tab.cpp"
    break;

  case 544: /* event_plain_header: IDL_EVENTTYPE defining_id  */
#line 6460 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9840 "fe/idl.tab.cpp"
    break;

  case 545: /* $@172: %empty  */
#line 6469 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 9848 "fe/idl.tab.cpp"
    break;

  case 546: /* event_rest_of_header: inheritance_spec $@172 supports_spec  */
#line 6473 "fe/idl.ypp"
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
#line 9881 "fe/idl.tab.cpp"
    break;

  case 547: /* @173: %empty  */
#line 6506 "fe/idl.ypp"
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
#line 9936 "fe/idl.tab.cpp"
    break;

  case 548: /* $@174: %empty  */
#line 6557 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 9944 "fe/idl.tab.cpp"
    break;

  case 549: /* $@175: %empty  */
#line 6561 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 9952 "fe/idl.tab.cpp"
    break;

  case 550: /* event_decl: event_header event_rest_of_header @173 '{' $@174 value_elements $@175 '}'  */
#line 6565 "fe/idl.ypp"
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

  case 551: /* event_header: event_custom_header  */
#line 6579 "fe/idl.ypp"
        {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9975 "fe/idl.tab.cpp"
    break;

  case 552: /* event_header: event_plain_header  */
#line 6583 "fe/idl.ypp"
        {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9983 "fe/idl.tab.cpp"
    break;

  case 553: /* formal_parameter_type: IDL_TYPENAME  */
#line 6590 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_type;
        }
#line 9991 "fe/idl.tab.cpp"
    break;

  case 554: /* formal_parameter_type: IDL_STRUCT  */
#line 6594 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_struct;
        }
#line 9999 "fe/idl.tab.cpp"
    break;

  case 555: /* formal_parameter_type: IDL_UNION  */
#line 6598 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_union;
        }
#line 10007 "fe/idl.tab.cpp"
    break;

  case 556: /* formal_parameter_type: IDL_EVENTTYPE  */
#line 6602 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_eventtype;
        }
#line 10015 "fe/idl.tab.cpp"
    break;

  case 557: /* formal_parameter_type: IDL_SEQUENCE  */
#line 6606 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_sequence;
        }
#line 10023 "fe/idl.tab.cpp"
    break;

  case 558: /* formal_parameter_type: IDL_INTERFACE  */
#line 6610 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_interface;
        }
#line 10031 "fe/idl.tab.cpp"
    break;

  case 559: /* formal_parameter_type: IDL_VALUETYPE  */
#line 6614 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_valuetype;
        }
#line 10039 "fe/idl.tab.cpp"
    break;

  case 560: /* formal_parameter_type: IDL_ENUM  */
#line 6618 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_enum;
        }
#line 10047 "fe/idl.tab.cpp"
    break;

  case 561: /* formal_parameter_type: IDL_EXCEPTION  */
#line 6622 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_except;
        }
#line 10055 "fe/idl.tab.cpp"
    break;

  case 562: /* formal_parameter_type: IDL_CONST const_type  */
#line 6626 "fe/idl.ypp"
        {
          (yyval.ntval) = AST_Decl::NT_const;
          t_param_const_type = (yyvsp[0].etval);
        }
#line 10064 "fe/idl.tab.cpp"
    break;

  case 563: /* at_least_one_formal_parameter: formal_parameter formal_parameters  */
#line 6634 "fe/idl.ypp"
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

  case 564: /* formal_parameters: formal_parameters ',' formal_parameter  */
#line 6665 "fe/idl.ypp"
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

  case 565: /* formal_parameters: %empty  */
#line 6678 "fe/idl.ypp"
        {
          (yyval.plval) = 0;
        }
#line 10121 "fe/idl.tab.cpp"
    break;

  case 566: /* formal_parameter: formal_parameter_type IDENTIFIER  */
#line 6685 "fe/idl.ypp"
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

  case 567: /* formal_parameter: IDL_SEQUENCE '<' IDENTIFIER '>' IDENTIFIER  */
#line 6710 "fe/idl.ypp"
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

  case 568: /* at_least_one_formal_parameter_name: formal_parameter_name formal_parameter_names  */
#line 6728 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.slval), UTL_StrList ((yyvsp[-1].sval), (yyvsp[0].slval)), 1);
        }
#line 10177 "fe/idl.tab.cpp"
    break;

  case 569: /* formal_parameter_names: formal_parameter_names ',' formal_parameter_name  */
#line 6735 "fe/idl.ypp"
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

  case 570: /* formal_parameter_names: %empty  */
#line 6750 "fe/idl.ypp"
        {
          (yyval.slval) = 0;
        }
#line 10204 "fe/idl.tab.cpp"
    break;

  case 571: /* formal_parameter_name: IDENTIFIER  */
#line 6757 "fe/idl.ypp"
        {
          ACE_NEW_RETURN ((yyval.sval),
                          UTL_String ((yyvsp[0].strval), true),
                          1);
        }
#line 10214 "fe/idl.tab.cpp"
    break;

  case 572: /* $@176: %empty  */
#line 6766 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSeen);
        }
#line 10222 "fe/idl.tab.cpp"
    break;

  case 573: /* @177: %empty  */
#line 6770 "fe/idl.ypp"
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

  case 574: /* $@178: %empty  */
#line 6789 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSqSeen);
        }
#line 10253 "fe/idl.tab.cpp"
    break;

  case 575: /* $@179: %empty  */
#line 6797 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeBodySeen);
        }
#line 10261 "fe/idl.tab.cpp"
    break;

  case 576: /* porttype_decl: IDL_PORTTYPE $@176 IDENTIFIER @177 '{' $@178 at_least_one_port_export $@179 '}'  */
#line 6801 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeQsSeen);

          // Done with this port type - pop it off the scopes stack.
          idl_global->scopes ().pop ();

          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 10274 "fe/idl.tab.cpp"
    break;

  case 577: /* at_least_one_port_export: port_exports at_least_one_annotation port_export  */
#line 6813 "fe/idl.ypp"
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

  case 583: /* $@180: %empty  */
#line 6839 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 10301 "fe/idl.tab.cpp"
    break;

  case 584: /* port_export: attribute $@180 ';'  */
#line 6843 "fe/idl.ypp"
        {
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 10309 "fe/idl.tab.cpp"
    break;

  case 585: /* extended_port_decl: IDL_PORT scoped_name IDENTIFIER  */
#line 6850 "fe/idl.ypp"
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
#line 10380 "fe/idl.tab.cpp"
    break;

  case 586: /* extended_port_decl: IDL_MIRRORPORT scoped_name IDENTIFIER  */
#line 6917 "fe/idl.ypp"
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
#line 10429 "fe/idl.tab.cpp"
    break;

  case 587: /* at_least_one_actual_parameter: annotations_maybe actual_parameter actual_parameters  */
#line 6965 "fe/idl.ypp"
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

  case 588: /* actual_parameters: actual_parameters ',' annotations_maybe actual_parameter  */
#line 6981 "fe/idl.ypp"
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

  case 589: /* actual_parameters: %empty  */
#line 6994 "fe/idl.ypp"
        {
          (yyval.alval) = 0;
        }
#line 10471 "fe/idl.tab.cpp"
    break;

  case 590: /* actual_parameter: expression  */
#line 7001 "fe/idl.ypp"
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

  case 591: /* connector_decl: connector_header connector_body  */
#line 7060 "fe/idl.ypp"
        {
          (yyval.dcval) = 0;
        }
#line 10539 "fe/idl.tab.cpp"
    break;

  case 592: /* $@181: %empty  */
#line 7067 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSeen);
        }
#line 10547 "fe/idl.tab.cpp"
    break;

  case 593: /* $@182: %empty  */
#line 7071 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorIDSeen);
        }
#line 10555 "fe/idl.tab.cpp"
    break;

  case 594: /* connector_header: IDL_CONNECTOR $@181 annotations_maybe IDENTIFIER $@182 component_inheritance_spec  */
#line 7075 "fe/idl.ypp"
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
#line 10613 "fe/idl.tab.cpp"
    break;

  case 595: /* $@183: %empty  */
#line 7132 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSqSeen);
        }
#line 10621 "fe/idl.tab.cpp"
    break;

  case 596: /* $@184: %empty  */
#line 7136 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorBodySeen);
        }
#line 10629 "fe/idl.tab.cpp"
    break;

  case 597: /* connector_body: '{' $@183 connector_exports $@184 '}'  */
#line 7140 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorQsSeen);

          // Done with this connector - pop it off the scope stack.
          idl_global->scopes ().pop ();
        }
#line 10640 "fe/idl.tab.cpp"
    break;

  case 600: /* $@185: %empty  */
#line 7155 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 10648 "fe/idl.tab.cpp"
    break;

  case 601: /* connector_export: provides_decl $@185 ';'  */
#line 7159 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10656 "fe/idl.tab.cpp"
    break;

  case 602: /* $@186: %empty  */
#line 7163 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 10664 "fe/idl.tab.cpp"
    break;

  case 603: /* connector_export: uses_decl $@186 ';'  */
#line 7167 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10672 "fe/idl.tab.cpp"
    break;

  case 604: /* $@187: %empty  */
#line 7171 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 10680 "fe/idl.tab.cpp"
    break;

  case 605: /* connector_export: attribute $@187 ';'  */
#line 7175 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10688 "fe/idl.tab.cpp"
    break;

  case 606: /* $@188: %empty  */
#line 7179 "fe/idl.ypp"
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 10696 "fe/idl.tab.cpp"
    break;

  case 607: /* connector_export: extended_port_decl $@188 ';'  */
#line 7183 "fe/idl.ypp"
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
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
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

#line 7188 "fe/idl.ypp"

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
