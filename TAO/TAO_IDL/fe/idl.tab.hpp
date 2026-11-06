// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton interface for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


/**
 ** \file fe/idl.tab.hpp
 ** Define the tao_yy::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

#ifndef YY_TAO_YY_FE_IDL_TAB_HPP_INCLUDED
# define YY_TAO_YY_FE_IDL_TAB_HPP_INCLUDED
// "%code requires" blocks.
#line 171 "fe/idl.ypp"

#include <string>
#include <utility>
#include <utl_strlist.h>

#line 55 "fe/idl.tab.hpp"


# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
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

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

#line 73 "fe/idl.ypp"
namespace tao_yy {
#line 191 "fe/idl.tab.hpp"




  /// A Bison parser.
  class parser
  {
  public:
#ifdef YYSTYPE
# ifdef __GNUC__
#  pragma GCC message "bison: do not #define YYSTYPE in C++, use %define api.value.type"
# endif
    typedef YYSTYPE value_type;
#else
    /// Symbol semantic values.
    union value_type
    {
#line 187 "fe/idl.ypp"

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
  std::string                   *sval;          /* String value         */
  std::string                   *wsval;         /* WString value        */
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

#line 255 "fe/idl.tab.hpp"

    };
#endif
    /// Backward compatibility (Bison 3.8).
    typedef value_type semantic_type;


    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const std::string& m)
        : std::runtime_error (m)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;
    };

    /// Token kinds.
    struct token
    {
      enum token_kind_type
      {
        YYEMPTY = -2,
    YYEOF = 0,                     // "end of file"
    YYerror = 256,                 // error
    YYUNDEF = 257,                 // "invalid token"
    IDENTIFIER = 258,              // IDENTIFIER
    IDL_CONST = 259,               // IDL_CONST
    IDL_MODULE = 260,              // IDL_MODULE
    IDL_INTERFACE = 261,           // IDL_INTERFACE
    IDL_TYPEDEF = 262,             // IDL_TYPEDEF
    IDL_LONG = 263,                // IDL_LONG
    IDL_SHORT = 264,               // IDL_SHORT
    IDL_UNSIGNED = 265,            // IDL_UNSIGNED
    IDL_DOUBLE = 266,              // IDL_DOUBLE
    IDL_FLOAT = 267,               // IDL_FLOAT
    IDL_CHAR = 268,                // IDL_CHAR
    IDL_WCHAR = 269,               // IDL_WCHAR
    IDL_OCTET = 270,               // IDL_OCTET
    IDL_BOOLEAN = 271,             // IDL_BOOLEAN
    IDL_FIXED = 272,               // IDL_FIXED
    IDL_ANY = 273,                 // IDL_ANY
    IDL_OBJECT = 274,              // IDL_OBJECT
    IDL_STRUCT = 275,              // IDL_STRUCT
    IDL_UNION = 276,               // IDL_UNION
    IDL_SWITCH = 277,              // IDL_SWITCH
    IDL_ENUM = 278,                // IDL_ENUM
    IDL_SEQUENCE = 279,            // IDL_SEQUENCE
    IDL_STRING = 280,              // IDL_STRING
    IDL_WSTRING = 281,             // IDL_WSTRING
    IDL_EXCEPTION = 282,           // IDL_EXCEPTION
    IDL_CASE = 283,                // IDL_CASE
    IDL_DEFAULT = 284,             // IDL_DEFAULT
    IDL_READONLY = 285,            // IDL_READONLY
    IDL_ATTRIBUTE = 286,           // IDL_ATTRIBUTE
    IDL_ONEWAY = 287,              // IDL_ONEWAY
    IDL_IDEMPOTENT = 288,          // IDL_IDEMPOTENT
    IDL_VOID = 289,                // IDL_VOID
    IDL_IN = 290,                  // IDL_IN
    IDL_OUT = 291,                 // IDL_OUT
    IDL_INOUT = 292,               // IDL_INOUT
    IDL_RAISES = 293,              // IDL_RAISES
    IDL_CONTEXT = 294,             // IDL_CONTEXT
    IDL_NATIVE = 295,              // IDL_NATIVE
    IDL_LOCAL = 296,               // IDL_LOCAL
    IDL_ABSTRACT = 297,            // IDL_ABSTRACT
    IDL_CUSTOM = 298,              // IDL_CUSTOM
    IDL_FACTORY = 299,             // IDL_FACTORY
    IDL_PRIVATE = 300,             // IDL_PRIVATE
    IDL_PUBLIC = 301,              // IDL_PUBLIC
    IDL_SUPPORTS = 302,            // IDL_SUPPORTS
    IDL_TRUNCATABLE = 303,         // IDL_TRUNCATABLE
    IDL_VALUETYPE = 304,           // IDL_VALUETYPE
    IDL_COMPONENT = 305,           // IDL_COMPONENT
    IDL_CONSUMES = 306,            // IDL_CONSUMES
    IDL_EMITS = 307,               // IDL_EMITS
    IDL_EVENTTYPE = 308,           // IDL_EVENTTYPE
    IDL_FINDER = 309,              // IDL_FINDER
    IDL_GETRAISES = 310,           // IDL_GETRAISES
    IDL_HOME = 311,                // IDL_HOME
    IDL_IMPORT = 312,              // IDL_IMPORT
    IDL_MULTIPLE = 313,            // IDL_MULTIPLE
    IDL_PRIMARYKEY = 314,          // IDL_PRIMARYKEY
    IDL_PROVIDES = 315,            // IDL_PROVIDES
    IDL_PUBLISHES = 316,           // IDL_PUBLISHES
    IDL_SETRAISES = 317,           // IDL_SETRAISES
    IDL_TYPEID = 318,              // IDL_TYPEID
    IDL_TYPEPREFIX = 319,          // IDL_TYPEPREFIX
    IDL_USES = 320,                // IDL_USES
    IDL_MANAGES = 321,             // IDL_MANAGES
    IDL_TYPENAME = 322,            // IDL_TYPENAME
    IDL_PORT = 323,                // IDL_PORT
    IDL_MIRRORPORT = 324,          // IDL_MIRRORPORT
    IDL_PORTTYPE = 325,            // IDL_PORTTYPE
    IDL_CONNECTOR = 326,           // IDL_CONNECTOR
    IDL_ALIAS = 327,               // IDL_ALIAS
    IDL_INTEGER_LITERAL = 328,     // IDL_INTEGER_LITERAL
    IDL_UINTEGER_LITERAL = 329,    // IDL_UINTEGER_LITERAL
    IDL_STRING_LITERAL = 330,      // IDL_STRING_LITERAL
    IDL_CHARACTER_LITERAL = 331,   // IDL_CHARACTER_LITERAL
    IDL_FLOATING_PT_LITERAL = 332, // IDL_FLOATING_PT_LITERAL
    IDL_FIXED_PT_LITERAL = 333,    // IDL_FIXED_PT_LITERAL
    IDL_TRUETOK = 334,             // IDL_TRUETOK
    IDL_FALSETOK = 335,            // IDL_FALSETOK
    IDL_INT8 = 336,                // IDL_INT8
    IDL_UINT8 = 337,               // IDL_UINT8
    IDL_INT16 = 338,               // IDL_INT16
    IDL_UINT16 = 339,              // IDL_UINT16
    IDL_INT32 = 340,               // IDL_INT32
    IDL_UINT32 = 341,              // IDL_UINT32
    IDL_INT64 = 342,               // IDL_INT64
    IDL_UINT64 = 343,              // IDL_UINT64
    IDL_SCOPE_DELIMITOR = 344,     // IDL_SCOPE_DELIMITOR
    IDL_LEFT_SHIFT = 345,          // IDL_LEFT_SHIFT
    IDL_RIGHT_SHIFT = 346,         // IDL_RIGHT_SHIFT
    IDL_WCHAR_LITERAL = 347,       // IDL_WCHAR_LITERAL
    IDL_WSTRING_LITERAL = 348,     // IDL_WSTRING_LITERAL
    IDL_ANNOTATION_DECL = 349,     // IDL_ANNOTATION_DECL
    IDL_ANNOTATION_SYMBOL = 350,   // IDL_ANNOTATION_SYMBOL
    IDL_BITFIELD = 351,            // IDL_BITFIELD
    IDL_BITMASK = 352,             // IDL_BITMASK
    IDL_BITSET = 353,              // IDL_BITSET
    IDL_MAP = 354                  // IDL_MAP
      };
      /// Backward compatibility alias (Bison 3.6).
      typedef token_kind_type yytokentype;
    };

    /// Token kind, as returned by yylex.
    typedef token::token_kind_type token_kind_type;

    /// Backward compatibility alias (Bison 3.6).
    typedef token_kind_type token_type;

    /// Symbol kinds.
    struct symbol_kind
    {
      enum symbol_kind_type
      {
        YYNTOKENS = 121, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // "end of file"
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S_IDENTIFIER = 3,                        // IDENTIFIER
        S_IDL_CONST = 4,                         // IDL_CONST
        S_IDL_MODULE = 5,                        // IDL_MODULE
        S_IDL_INTERFACE = 6,                     // IDL_INTERFACE
        S_IDL_TYPEDEF = 7,                       // IDL_TYPEDEF
        S_IDL_LONG = 8,                          // IDL_LONG
        S_IDL_SHORT = 9,                         // IDL_SHORT
        S_IDL_UNSIGNED = 10,                     // IDL_UNSIGNED
        S_IDL_DOUBLE = 11,                       // IDL_DOUBLE
        S_IDL_FLOAT = 12,                        // IDL_FLOAT
        S_IDL_CHAR = 13,                         // IDL_CHAR
        S_IDL_WCHAR = 14,                        // IDL_WCHAR
        S_IDL_OCTET = 15,                        // IDL_OCTET
        S_IDL_BOOLEAN = 16,                      // IDL_BOOLEAN
        S_IDL_FIXED = 17,                        // IDL_FIXED
        S_IDL_ANY = 18,                          // IDL_ANY
        S_IDL_OBJECT = 19,                       // IDL_OBJECT
        S_IDL_STRUCT = 20,                       // IDL_STRUCT
        S_IDL_UNION = 21,                        // IDL_UNION
        S_IDL_SWITCH = 22,                       // IDL_SWITCH
        S_IDL_ENUM = 23,                         // IDL_ENUM
        S_IDL_SEQUENCE = 24,                     // IDL_SEQUENCE
        S_IDL_STRING = 25,                       // IDL_STRING
        S_IDL_WSTRING = 26,                      // IDL_WSTRING
        S_IDL_EXCEPTION = 27,                    // IDL_EXCEPTION
        S_IDL_CASE = 28,                         // IDL_CASE
        S_IDL_DEFAULT = 29,                      // IDL_DEFAULT
        S_IDL_READONLY = 30,                     // IDL_READONLY
        S_IDL_ATTRIBUTE = 31,                    // IDL_ATTRIBUTE
        S_IDL_ONEWAY = 32,                       // IDL_ONEWAY
        S_IDL_IDEMPOTENT = 33,                   // IDL_IDEMPOTENT
        S_IDL_VOID = 34,                         // IDL_VOID
        S_IDL_IN = 35,                           // IDL_IN
        S_IDL_OUT = 36,                          // IDL_OUT
        S_IDL_INOUT = 37,                        // IDL_INOUT
        S_IDL_RAISES = 38,                       // IDL_RAISES
        S_IDL_CONTEXT = 39,                      // IDL_CONTEXT
        S_IDL_NATIVE = 40,                       // IDL_NATIVE
        S_IDL_LOCAL = 41,                        // IDL_LOCAL
        S_IDL_ABSTRACT = 42,                     // IDL_ABSTRACT
        S_IDL_CUSTOM = 43,                       // IDL_CUSTOM
        S_IDL_FACTORY = 44,                      // IDL_FACTORY
        S_IDL_PRIVATE = 45,                      // IDL_PRIVATE
        S_IDL_PUBLIC = 46,                       // IDL_PUBLIC
        S_IDL_SUPPORTS = 47,                     // IDL_SUPPORTS
        S_IDL_TRUNCATABLE = 48,                  // IDL_TRUNCATABLE
        S_IDL_VALUETYPE = 49,                    // IDL_VALUETYPE
        S_IDL_COMPONENT = 50,                    // IDL_COMPONENT
        S_IDL_CONSUMES = 51,                     // IDL_CONSUMES
        S_IDL_EMITS = 52,                        // IDL_EMITS
        S_IDL_EVENTTYPE = 53,                    // IDL_EVENTTYPE
        S_IDL_FINDER = 54,                       // IDL_FINDER
        S_IDL_GETRAISES = 55,                    // IDL_GETRAISES
        S_IDL_HOME = 56,                         // IDL_HOME
        S_IDL_IMPORT = 57,                       // IDL_IMPORT
        S_IDL_MULTIPLE = 58,                     // IDL_MULTIPLE
        S_IDL_PRIMARYKEY = 59,                   // IDL_PRIMARYKEY
        S_IDL_PROVIDES = 60,                     // IDL_PROVIDES
        S_IDL_PUBLISHES = 61,                    // IDL_PUBLISHES
        S_IDL_SETRAISES = 62,                    // IDL_SETRAISES
        S_IDL_TYPEID = 63,                       // IDL_TYPEID
        S_IDL_TYPEPREFIX = 64,                   // IDL_TYPEPREFIX
        S_IDL_USES = 65,                         // IDL_USES
        S_IDL_MANAGES = 66,                      // IDL_MANAGES
        S_IDL_TYPENAME = 67,                     // IDL_TYPENAME
        S_IDL_PORT = 68,                         // IDL_PORT
        S_IDL_MIRRORPORT = 69,                   // IDL_MIRRORPORT
        S_IDL_PORTTYPE = 70,                     // IDL_PORTTYPE
        S_IDL_CONNECTOR = 71,                    // IDL_CONNECTOR
        S_IDL_ALIAS = 72,                        // IDL_ALIAS
        S_IDL_INTEGER_LITERAL = 73,              // IDL_INTEGER_LITERAL
        S_IDL_UINTEGER_LITERAL = 74,             // IDL_UINTEGER_LITERAL
        S_IDL_STRING_LITERAL = 75,               // IDL_STRING_LITERAL
        S_IDL_CHARACTER_LITERAL = 76,            // IDL_CHARACTER_LITERAL
        S_IDL_FLOATING_PT_LITERAL = 77,          // IDL_FLOATING_PT_LITERAL
        S_IDL_FIXED_PT_LITERAL = 78,             // IDL_FIXED_PT_LITERAL
        S_IDL_TRUETOK = 79,                      // IDL_TRUETOK
        S_IDL_FALSETOK = 80,                     // IDL_FALSETOK
        S_IDL_INT8 = 81,                         // IDL_INT8
        S_IDL_UINT8 = 82,                        // IDL_UINT8
        S_IDL_INT16 = 83,                        // IDL_INT16
        S_IDL_UINT16 = 84,                       // IDL_UINT16
        S_IDL_INT32 = 85,                        // IDL_INT32
        S_IDL_UINT32 = 86,                       // IDL_UINT32
        S_IDL_INT64 = 87,                        // IDL_INT64
        S_IDL_UINT64 = 88,                       // IDL_UINT64
        S_IDL_SCOPE_DELIMITOR = 89,              // IDL_SCOPE_DELIMITOR
        S_IDL_LEFT_SHIFT = 90,                   // IDL_LEFT_SHIFT
        S_IDL_RIGHT_SHIFT = 91,                  // IDL_RIGHT_SHIFT
        S_IDL_WCHAR_LITERAL = 92,                // IDL_WCHAR_LITERAL
        S_IDL_WSTRING_LITERAL = 93,              // IDL_WSTRING_LITERAL
        S_IDL_ANNOTATION_DECL = 94,              // IDL_ANNOTATION_DECL
        S_IDL_ANNOTATION_SYMBOL = 95,            // IDL_ANNOTATION_SYMBOL
        S_IDL_BITFIELD = 96,                     // IDL_BITFIELD
        S_IDL_BITMASK = 97,                      // IDL_BITMASK
        S_IDL_BITSET = 98,                       // IDL_BITSET
        S_IDL_MAP = 99,                          // IDL_MAP
        S_100_ = 100,                            // ';'
        S_101_ = 101,                            // '{'
        S_102_ = 102,                            // '}'
        S_103_ = 103,                            // '<'
        S_104_ = 104,                            // '>'
        S_105_ = 105,                            // ':'
        S_106_ = 106,                            // ','
        S_107_ = 107,                            // '='
        S_108_ = 108,                            // '|'
        S_109_ = 109,                            // '^'
        S_110_ = 110,                            // '&'
        S_111_ = 111,                            // '+'
        S_112_ = 112,                            // '-'
        S_113_ = 113,                            // '*'
        S_114_ = 114,                            // '/'
        S_115_ = 115,                            // '%'
        S_116_ = 116,                            // '~'
        S_117_ = 117,                            // '('
        S_118_ = 118,                            // ')'
        S_119_ = 119,                            // '['
        S_120_ = 120,                            // ']'
        S_YYACCEPT = 121,                        // $accept
        S_start = 122,                           // start
        S_definitions = 123,                     // definitions
        S_at_least_one_definition = 124,         // at_least_one_definition
        S_definition = 125,                      // definition
        S_fixed_definition = 126,                // fixed_definition
        S_127_1 = 127,                           // $@1
        S_128_2 = 128,                           // $@2
        S_129_3 = 129,                           // $@3
        S_130_4 = 130,                           // $@4
        S_131_5 = 131,                           // $@5
        S_132_6 = 132,                           // $@6
        S_133_7 = 133,                           // $@7
        S_134_8 = 134,                           // $@8
        S_135_9 = 135,                           // $@9
        S_136_10 = 136,                          // $@10
        S_137_11 = 137,                          // $@11
        S_138_12 = 138,                          // $@12
        S_139_13 = 139,                          // $@13
        S_140_14 = 140,                          // $@14
        S_141_15 = 141,                          // $@15
        S_module_header = 142,                   // module_header
        S_143_16 = 143,                          // $@16
        S_module = 144,                          // module
        S_145_17 = 145,                          // @17
        S_146_18 = 146,                          // $@18
        S_147_19 = 147,                          // $@19
        S_template_module_header = 148,          // template_module_header
        S_template_module = 149,                 // template_module
        S_150_20 = 150,                          // $@20
        S_151_21 = 151,                          // $@21
        S_152_22 = 152,                          // $@22
        S_153_23 = 153,                          // $@23
        S_154_24 = 154,                          // $@24
        S_at_least_one_tpl_definition = 155,     // at_least_one_tpl_definition
        S_tpl_definitions = 156,                 // tpl_definitions
        S_tpl_definition = 157,                  // tpl_definition
        S_template_module_ref = 158,             // template_module_ref
        S_159_25 = 159,                          // $@25
        S_160_26 = 160,                          // $@26
        S_template_module_inst = 161,            // template_module_inst
        S_162_27 = 162,                          // $@27
        S_163_28 = 163,                          // $@28
        S_interface_def = 164,                   // interface_def
        S_interface = 165,                       // interface
        S_166_29 = 166,                          // $@29
        S_167_30 = 167,                          // $@30
        S_168_31 = 168,                          // $@31
        S_interface_decl = 169,                  // interface_decl
        S_170_32 = 170,                          // $@32
        S_interface_header = 171,                // interface_header
        S_inheritance_spec = 172,                // inheritance_spec
        S_173_33 = 173,                          // $@33
        S_value_def = 174,                       // value_def
        S_valuetype = 175,                       // valuetype
        S_value_concrete_decl = 176,             // value_concrete_decl
        S_177_34 = 177,                          // @34
        S_178_35 = 178,                          // $@35
        S_179_36 = 179,                          // $@36
        S_value_abs_decl = 180,                  // value_abs_decl
        S_181_37 = 181,                          // $@37
        S_182_38 = 182,                          // $@38
        S_183_39 = 183,                          // $@39
        S_value_header = 184,                    // value_header
        S_185_40 = 185,                          // $@40
        S_value_decl = 186,                      // value_decl
        S_187_41 = 187,                          // $@41
        S_opt_truncatable = 188,                 // opt_truncatable
        S_supports_spec = 189,                   // supports_spec
        S_value_forward_decl = 190,              // value_forward_decl
        S_value_box_decl = 191,                  // value_box_decl
        S_value_elements = 192,                  // value_elements
        S_value_element = 193,                   // value_element
        S_194_42 = 194,                          // @42
        S_visibility = 195,                      // visibility
        S_state_member = 196,                    // state_member
        S_exports = 197,                         // exports
        S_at_least_one_export = 198,             // at_least_one_export
        S_export = 199,                          // export
        S_200_43 = 200,                          // $@43
        S_201_44 = 201,                          // $@44
        S_202_45 = 202,                          // $@45
        S_203_46 = 203,                          // $@46
        S_204_47 = 204,                          // $@47
        S_205_48 = 205,                          // $@48
        S_206_49 = 206,                          // $@49
        S_207_50 = 207,                          // $@50
        S_at_least_one_scoped_name = 208,        // at_least_one_scoped_name
        S_scoped_names = 209,                    // scoped_names
        S_210_51 = 210,                          // $@51
        S_scoped_name = 211,                     // scoped_name
        S_212_52 = 212,                          // $@52
        S_213_53 = 213,                          // $@53
        S_id = 214,                              // id
        S_defining_id = 215,                     // defining_id
        S_interface_forward = 216,               // interface_forward
        S_const_dcl = 217,                       // const_dcl
        S_218_54 = 218,                          // $@54
        S_219_55 = 219,                          // $@55
        S_220_56 = 220,                          // $@56
        S_221_57 = 221,                          // $@57
        S_const_type = 222,                      // const_type
        S_expression = 223,                      // expression
        S_const_expr = 224,                      // const_expr
        S_or_expr = 225,                         // or_expr
        S_xor_expr = 226,                        // xor_expr
        S_and_expr = 227,                        // and_expr
        S_shift_expr = 228,                      // shift_expr
        S_add_expr = 229,                        // add_expr
        S_mult_expr = 230,                       // mult_expr
        S_unary_expr = 231,                      // unary_expr
        S_primary_expr = 232,                    // primary_expr
        S_literal = 233,                         // literal
        S_positive_int_expr = 234,               // positive_int_expr
        S_annotation_dcl = 235,                  // annotation_dcl
        S_236_58 = 236,                          // $@58
        S_annotation_body = 237,                 // annotation_body
        S_annotation_statement = 238,            // annotation_statement
        S_239_59 = 239,                          // $@59
        S_annotation_member_type = 240,          // annotation_member_type
        S_annotation_member = 241,               // annotation_member
        S_annotation_member_default = 242,       // annotation_member_default
        S_at_least_one_annotation = 243,         // at_least_one_annotation
        S_annotations_maybe = 244,               // annotations_maybe
        S_annotation_appl = 245,                 // annotation_appl
        S_246_60 = 246,                          // @60
        S_annotation_appl_params_maybe = 247,    // annotation_appl_params_maybe
        S_annotation_appl_params = 248,          // annotation_appl_params
        S_named_annotation_appl_params = 249,    // named_annotation_appl_params
        S_more_named_annotation_appl_params = 250, // more_named_annotation_appl_params
        S_named_annotation_appl_param = 251,     // named_annotation_appl_param
        S_type_dcl = 252,                        // type_dcl
        S_253_61 = 253,                          // $@61
        S_type_declarator = 254,                 // type_declarator
        S_255_62 = 255,                          // $@62
        S_type_spec = 256,                       // type_spec
        S_simple_type_spec = 257,                // simple_type_spec
        S_base_type_spec = 258,                  // base_type_spec
        S_template_type_spec = 259,              // template_type_spec
        S_constructed_type_spec = 260,           // constructed_type_spec
        S_constructed_forward_type_spec = 261,   // constructed_forward_type_spec
        S_at_least_one_declarator = 262,         // at_least_one_declarator
        S_declarators = 263,                     // declarators
        S_264_63 = 264,                          // $@63
        S_declarator = 265,                      // declarator
        S_at_least_one_simple_declarator = 266,  // at_least_one_simple_declarator
        S_simple_declarators = 267,              // simple_declarators
        S_268_64 = 268,                          // $@64
        S_simple_declarator = 269,               // simple_declarator
        S_complex_declarator = 270,              // complex_declarator
        S_integer_type = 271,                    // integer_type
        S_signed_int = 272,                      // signed_int
        S_unsigned_int = 273,                    // unsigned_int
        S_floating_pt_type = 274,                // floating_pt_type
        S_fixed_type = 275,                      // fixed_type
        S_char_type = 276,                       // char_type
        S_octet_type = 277,                      // octet_type
        S_boolean_type = 278,                    // boolean_type
        S_any_type = 279,                        // any_type
        S_object_type = 280,                     // object_type
        S_struct_decl = 281,                     // struct_decl
        S_282_65 = 282,                          // $@65
        S_struct_type = 283,                     // struct_type
        S_284_66 = 284,                          // $@66
        S_285_67 = 285,                          // $@67
        S_struct_body = 286,                     // struct_body
        S_struct_body_with_members = 287,        // struct_body_with_members
        S_288_68 = 288,                          // $@68
        S_members = 289,                         // members
        S_member = 290,                          // member
        S_member_i = 291,                        // member_i
        S_292_69 = 292,                          // $@69
        S_293_70 = 293,                          // $@70
        S_294_71 = 294,                          // $@71
        S_union_decl = 295,                      // union_decl
        S_296_72 = 296,                          // $@72
        S_union_type = 297,                      // union_type
        S_298_73 = 298,                          // $@73
        S_299_74 = 299,                          // $@74
        S_300_75 = 300,                          // $@75
        S_301_76 = 301,                          // $@76
        S_302_77 = 302,                          // $@77
        S_303_78 = 303,                          // $@78
        S_switch_type_spec = 304,                // switch_type_spec
        S_at_least_one_case_branch = 305,        // at_least_one_case_branch
        S_case_branches = 306,                   // case_branches
        S_case_branch = 307,                     // case_branch
        S_308_79 = 308,                          // $@79
        S_309_80 = 309,                          // $@80
        S_310_81 = 310,                          // $@81
        S_at_least_one_case_label = 311,         // at_least_one_case_label
        S_case_labels = 312,                     // case_labels
        S_case_label = 313,                      // case_label
        S_314_82 = 314,                          // $@82
        S_315_83 = 315,                          // $@83
        S_316_84 = 316,                          // $@84
        S_element_spec = 317,                    // element_spec
        S_318_85 = 318,                          // $@85
        S_struct_forward_type = 319,             // struct_forward_type
        S_union_forward_type = 320,              // union_forward_type
        S_enum_type = 321,                       // enum_type
        S_322_86 = 322,                          // $@86
        S_323_87 = 323,                          // $@87
        S_324_88 = 324,                          // $@88
        S_325_89 = 325,                          // $@89
        S_at_least_one_enumerator = 326,         // at_least_one_enumerator
        S_enumerators = 327,                     // enumerators
        S_328_90 = 328,                          // $@90
        S_enumerator = 329,                      // enumerator
        S_map_type_spec = 330,                   // map_type_spec
        S_map_head = 331,                        // map_head
        S_332_91 = 332,                          // $@91
        S_333_92 = 333,                          // $@92
        S_sequence_type_spec = 334,              // sequence_type_spec
        S_335_93 = 335,                          // $@93
        S_336_94 = 336,                          // $@94
        S_seq_head = 337,                        // seq_head
        S_338_95 = 338,                          // $@95
        S_339_96 = 339,                          // $@96
        S_fixed_type_spec = 340,                 // fixed_type_spec
        S_string_type_spec = 341,                // string_type_spec
        S_342_97 = 342,                          // $@97
        S_343_98 = 343,                          // $@98
        S_string_head = 344,                     // string_head
        S_wstring_type_spec = 345,               // wstring_type_spec
        S_346_99 = 346,                          // $@99
        S_347_100 = 347,                         // $@100
        S_wstring_head = 348,                    // wstring_head
        S_array_declarator = 349,                // array_declarator
        S_350_101 = 350,                         // $@101
        S_at_least_one_array_dim = 351,          // at_least_one_array_dim
        S_array_dims = 352,                      // array_dims
        S_array_dim = 353,                       // array_dim
        S_354_102 = 354,                         // $@102
        S_355_103 = 355,                         // $@103
        S_attribute = 356,                       // attribute
        S_attribute_readonly = 357,              // attribute_readonly
        S_358_104 = 358,                         // $@104
        S_359_105 = 359,                         // $@105
        S_360_106 = 360,                         // $@106
        S_361_107 = 361,                         // $@107
        S_attribute_readwrite = 362,             // attribute_readwrite
        S_363_108 = 363,                         // $@108
        S_364_109 = 364,                         // $@109
        S_365_110 = 365,                         // $@110
        S_366_111 = 366,                         // $@111
        S_exception = 367,                       // exception
        S_368_112 = 368,                         // $@112
        S_369_113 = 369,                         // @113
        S_370_114 = 370,                         // $@114
        S_371_115 = 371,                         // $@115
        S_operation = 372,                       // operation
        S_373_116 = 373,                         // $@116
        S_374_117 = 374,                         // $@117
        S_375_118 = 375,                         // $@118
        S_376_119 = 376,                         // $@119
        S_opt_op_attribute = 377,                // opt_op_attribute
        S_op_type_spec = 378,                    // op_type_spec
        S_init_decl = 379,                       // init_decl
        S_380_120 = 380,                         // $@120
        S_381_121 = 381,                         // @121
        S_382_122 = 382,                         // $@122
        S_init_parameter_list = 383,             // init_parameter_list
        S_384_123 = 384,                         // $@123
        S_385_124 = 385,                         // $@124
        S_at_least_one_in_parameter = 386,       // at_least_one_in_parameter
        S_in_parameters = 387,                   // in_parameters
        S_388_125 = 388,                         // $@125
        S_in_parameter = 389,                    // in_parameter
        S_390_126 = 390,                         // $@126
        S_391_127 = 391,                         // $@127
        S_parameter_list = 392,                  // parameter_list
        S_393_128 = 393,                         // $@128
        S_394_129 = 394,                         // $@129
        S_at_least_one_parameter = 395,          // at_least_one_parameter
        S_parameters = 396,                      // parameters
        S_397_130 = 397,                         // $@130
        S_parameter = 398,                       // parameter
        S_399_131 = 399,                         // $@131
        S_400_132 = 400,                         // $@132
        S_param_type_spec = 401,                 // param_type_spec
        S_direction = 402,                       // direction
        S_opt_raises = 403,                      // opt_raises
        S_404_133 = 404,                         // $@133
        S_405_134 = 405,                         // $@134
        S_opt_getraises = 406,                   // opt_getraises
        S_407_135 = 407,                         // $@135
        S_408_136 = 408,                         // $@136
        S_opt_setraises = 409,                   // opt_setraises
        S_410_137 = 410,                         // $@137
        S_411_138 = 411,                         // $@138
        S_opt_context = 412,                     // opt_context
        S_413_139 = 413,                         // $@139
        S_414_140 = 414,                         // $@140
        S_at_least_one_string_literal = 415,     // at_least_one_string_literal
        S_string_literals = 416,                 // string_literals
        S_417_141 = 417,                         // $@141
        S_typeid_dcl = 418,                      // typeid_dcl
        S_typeprefix_dcl = 419,                  // typeprefix_dcl
        S_component = 420,                       // component
        S_component_forward_decl = 421,          // component_forward_decl
        S_component_decl = 422,                  // component_decl
        S_423_142 = 423,                         // @142
        S_424_143 = 424,                         // $@143
        S_425_144 = 425,                         // $@144
        S_component_header = 426,                // component_header
        S_427_145 = 427,                         // $@145
        S_428_146 = 428,                         // $@146
        S_component_inheritance_spec = 429,      // component_inheritance_spec
        S_430_147 = 430,                         // $@147
        S_component_exports = 431,               // component_exports
        S_component_export = 432,                // component_export
        S_433_148 = 433,                         // $@148
        S_434_149 = 434,                         // $@149
        S_435_150 = 435,                         // $@150
        S_436_151 = 436,                         // $@151
        S_437_152 = 437,                         // $@152
        S_438_153 = 438,                         // $@153
        S_439_154 = 439,                         // $@154
        S_provides_decl = 440,                   // provides_decl
        S_interface_type = 441,                  // interface_type
        S_uses_decl = 442,                       // uses_decl
        S_uses_opt_multiple = 443,               // uses_opt_multiple
        S_opt_multiple = 444,                    // opt_multiple
        S_emits_decl = 445,                      // emits_decl
        S_publishes_decl = 446,                  // publishes_decl
        S_consumes_decl = 447,                   // consumes_decl
        S_home_decl = 448,                       // home_decl
        S_449_155 = 449,                         // $@155
        S_home_header = 450,                     // home_header
        S_451_156 = 451,                         // $@156
        S_452_157 = 452,                         // $@157
        S_453_158 = 453,                         // $@158
        S_454_159 = 454,                         // $@159
        S_455_160 = 455,                         // $@160
        S_456_161 = 456,                         // $@161
        S_home_inheritance_spec = 457,           // home_inheritance_spec
        S_458_162 = 458,                         // $@162
        S_primary_key_spec = 459,                // primary_key_spec
        S_home_body = 460,                       // home_body
        S_461_163 = 461,                         // $@163
        S_462_164 = 462,                         // $@164
        S_home_exports = 463,                    // home_exports
        S_home_export = 464,                     // home_export
        S_465_165 = 465,                         // $@165
        S_466_166 = 466,                         // $@166
        S_factory_decl = 467,                    // factory_decl
        S_468_167 = 468,                         // $@167
        S_469_168 = 469,                         // $@168
        S_finder_decl = 470,                     // finder_decl
        S_471_169 = 471,                         // $@169
        S_472_170 = 472,                         // $@170
        S_event = 473,                           // event
        S_event_forward_decl = 474,              // event_forward_decl
        S_event_concrete_forward_decl = 475,     // event_concrete_forward_decl
        S_event_abs_forward_decl = 476,          // event_abs_forward_decl
        S_event_abs_decl = 477,                  // event_abs_decl
        S_478_171 = 478,                         // $@171
        S_479_172 = 479,                         // $@172
        S_480_173 = 480,                         // $@173
        S_event_abs_header = 481,                // event_abs_header
        S_event_custom_header = 482,             // event_custom_header
        S_event_plain_header = 483,              // event_plain_header
        S_event_rest_of_header = 484,            // event_rest_of_header
        S_485_174 = 485,                         // $@174
        S_event_decl = 486,                      // event_decl
        S_487_175 = 487,                         // @175
        S_488_176 = 488,                         // $@176
        S_489_177 = 489,                         // $@177
        S_event_header = 490,                    // event_header
        S_formal_parameter_type = 491,           // formal_parameter_type
        S_at_least_one_formal_parameter = 492,   // at_least_one_formal_parameter
        S_formal_parameters = 493,               // formal_parameters
        S_formal_parameter = 494,                // formal_parameter
        S_at_least_one_formal_parameter_name = 495, // at_least_one_formal_parameter_name
        S_formal_parameter_names = 496,          // formal_parameter_names
        S_formal_parameter_name = 497,           // formal_parameter_name
        S_porttype_decl = 498,                   // porttype_decl
        S_499_178 = 499,                         // $@178
        S_500_179 = 500,                         // @179
        S_501_180 = 501,                         // $@180
        S_502_181 = 502,                         // $@181
        S_at_least_one_port_export = 503,        // at_least_one_port_export
        S_port_exports = 504,                    // port_exports
        S_port_export = 505,                     // port_export
        S_506_182 = 506,                         // $@182
        S_extended_port_decl = 507,              // extended_port_decl
        S_at_least_one_actual_parameter = 508,   // at_least_one_actual_parameter
        S_actual_parameters = 509,               // actual_parameters
        S_actual_parameter = 510,                // actual_parameter
        S_connector_decl = 511,                  // connector_decl
        S_connector_header = 512,                // connector_header
        S_513_183 = 513,                         // $@183
        S_514_184 = 514,                         // $@184
        S_connector_body = 515,                  // connector_body
        S_516_185 = 516,                         // $@185
        S_517_186 = 517,                         // $@186
        S_connector_exports = 518,               // connector_exports
        S_connector_export = 519,                // connector_export
        S_520_187 = 520,                         // $@187
        S_521_188 = 521,                         // $@188
        S_522_189 = 522,                         // $@189
        S_523_190 = 523                          // $@190
      };
    };

    /// (Internal) symbol kind.
    typedef symbol_kind::symbol_kind_type symbol_kind_type;

    /// The number of tokens.
    static const symbol_kind_type YYNTOKENS = symbol_kind::YYNTOKENS;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol kind
    /// via kind ().
    ///
    /// Provide access to semantic value.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol () YY_NOEXCEPT
        : value ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that)
        : Base (std::move (that))
        , value (std::move (that.value))
      {}
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);
      /// Constructor for valueless symbols.
      basic_symbol (typename Base::kind_type t);

      /// Constructor for symbols with semantic value.
      basic_symbol (typename Base::kind_type t,
                    YY_RVREF (value_type) v);

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }



      /// Destroy contents, and record that is empty.
      void clear () YY_NOEXCEPT
      {
        Base::clear ();
      }

#if YYDEBUG || 0
      /// The user-facing name of this symbol.
      const char *name () const YY_NOEXCEPT
      {
        return parser::symbol_name (this->kind ());
      }
#endif // #if YYDEBUG || 0


      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      value_type value;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_kind
    {
      /// The symbol kind as needed by the constructor.
      typedef token_kind_type kind_type;

      /// Default constructor.
      by_kind () YY_NOEXCEPT;

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_kind (by_kind&& that) YY_NOEXCEPT;
#endif

      /// Copy constructor.
      by_kind (const by_kind& that) YY_NOEXCEPT;

      /// Constructor from (external) token numbers.
      by_kind (kind_type t) YY_NOEXCEPT;



      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_kind& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// The symbol kind.
      /// \a S_YYEMPTY when empty.
      symbol_kind_type kind_;
    };

    /// Backward compatibility for a private implementation detail (Bison 3.6).
    typedef by_kind by_type;

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_kind>
    {};

    /// Build a parser object.
    parser ();
    virtual ~parser ();

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    parser (const parser&) = delete;
    /// Non copyable.
    parser& operator= (const parser&) = delete;
#endif

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param msg    a description of the syntax error.
    virtual void error (const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

#if YYDEBUG || 0
    /// The user-facing name of the symbol whose (internal) number is
    /// YYSYMBOL.  No bounds checking.
    static const char *symbol_name (symbol_kind_type yysymbol);
#endif // #if YYDEBUG || 0




  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    parser (const parser&);
    /// Non copyable.
    parser& operator= (const parser&);
#endif


    /// Stored state numbers (used for stacks).
    typedef short state_type;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    static state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT;

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT;

    static const short yypact_ninf_;
    static const short yytable_ninf_;

    /// Convert a scanner token kind \a t to a symbol kind.
    /// In theory \a t should be a token_kind_type, but character literals
    /// are valid, yet not members of the token_kind_type enum.
    static symbol_kind_type yytranslate_ (int t) YY_NOEXCEPT;

#if YYDEBUG || 0
    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#endif // #if YYDEBUG || 0


    // Tables.
    // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
    // STATE-NUM.
    static const short yypact_[];

    // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
    // Performed when YYTABLE does not specify something else to do.  Zero
    // means the default is an error.
    static const short yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const short yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const short yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const short yytable_[];

    static const short yycheck_[];

    // YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
    // state STATE-NUM.
    static const short yystos_[];

    // YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.
    static const short yyr1_[];

    // YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.
    static const signed char yyr2_[];


#if YYDEBUG
    // YYRLINE[YYN] -- Source line where rule number YYN was defined.
    static const short yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r) const;
    /// Print the state stack on the debug stream.
    virtual void yy_stack_print_ () const;

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol kind, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state () YY_NOEXCEPT;

      /// The symbol kind as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_state& that);

      /// The symbol kind (corresponding to \a state).
      /// \a symbol_kind::S_YYEMPTY when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      /// We use the initial state, as it does not have a value.
      enum { empty_state = 0 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);

      /// Assignment, needed by push_back by other implementations.
      /// Needed by some other old implementations.
      stack_symbol_type& operator= (const stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::iterator iterator;
      typedef typename S::const_iterator const_iterator;
      typedef typename S::size_type size_type;
      typedef typename std::ptrdiff_t index_type;

      stack (size_type n = 200) YY_NOEXCEPT
        : seq_ (n)
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Non copyable.
      stack (const stack&) = delete;
      /// Non copyable.
      stack& operator= (const stack&) = delete;
#endif

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (index_type i) const
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (index_type i)
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (std::ptrdiff_t n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      index_type
      size () const YY_NOEXCEPT
      {
        return index_type (seq_.size ());
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.begin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.end ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, index_type range) YY_NOEXCEPT
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (index_type i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        index_type range_;
      };

    private:
#if YY_CPLUSPLUS < 201103L
      /// Non copyable.
      stack (const stack&);
      /// Non copyable.
      stack& operator= (const stack&);
#endif
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1) YY_NOEXCEPT;

    /// Constants.
    enum
    {
      yylast_ = 2046,     ///< Last index in yytable_.
      yynnts_ = 403,  ///< Number of nonterminal symbols.
      yyfinal_ = 4 ///< Termination state number.
    };



  };


#line 73 "fe/idl.ypp"
} // tao_yy
#line 1420 "fe/idl.tab.hpp"




#endif // !YY_TAO_YY_FE_IDL_TAB_HPP_INCLUDED
