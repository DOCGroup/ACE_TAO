// $Id$
#include	"idl.h"
#include	"idl_extern.h"
#include	"fe_private.h"
/* starting time is 17:20:28 */
/* C++ code produced by gperf version 2.7 (GNU C++ version) */
/* Command-line: /project/macarena/coryan/head/ACE_wrappers/bin/gperf -M -J -c -C -D -E -T -f 0 -a -o -t -p -K keyword_ -L C++ -Z TAO_IDL_CPP_Keyword_Table -N lookup -k1,2,$ fe/keywords.dat  */
unsigned int
TAO_IDL_CPP_Keyword_Table::hash (const char *str, unsigned int len)
{
  static const unsigned char asso_values[] =
    {
     252, 252, 252, 252, 252, 252, 252, 252, 252, 252,
     252, 252, 252, 252, 252, 252, 252, 252, 252, 252,
     252, 252, 252, 252, 252, 252, 252, 252, 252, 252,
     252, 252, 252, 252, 252, 252, 252, 252, 252, 252,
     252, 252, 252, 252, 252, 252, 252, 252, 252, 252,
     252, 252, 252, 252, 252, 252, 252, 252, 252, 252,
     252, 252, 252, 252, 252, 252, 252, 252, 252, 252,
     252, 252, 252, 252, 252, 252, 252, 252, 252, 252,
     252, 252, 252, 252, 252, 252, 252, 252, 252, 252,
     252, 252, 252, 252, 252, 252, 252,  15,  35,  65,
       0,   0,  35,   5,   0,  60, 252,   0,  30,   0,
       5,   5,  20,  50,  40,  20, 110,   0,  45,  35,
      95,  55, 252, 252, 252, 252, 252, 252,
    };
  return len + asso_values[str[1]] + asso_values[str[0]] + asso_values[str[len - 1]];
}

const struct TAO_IDL_CPP_Keyword_Entry *
TAO_IDL_CPP_Keyword_Table::lookup (const char *str, unsigned int len)
{
  enum
    {
      TOTAL_KEYWORDS = 74,
      MIN_WORD_LENGTH = 2,
      MAX_WORD_LENGTH = 16,
      MIN_HASH_VALUE = 6,
      MAX_HASH_VALUE = 251,
      HASH_VALUE_RANGE = 246,
      DUPLICATES = 0
    };

  static const struct TAO_IDL_CPP_Keyword_Entry  wordlist[] =
    {
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"delete",  "_cxx_delete"},
      {"mutable",  "_cxx_mutable"},
      {"",}, 
      {"enum",  "_cxx_enum"},
      {"",}, 
      {"double",  "_cxx_double"},
      {"do",  "_cxx_do"},
      {"unsigned",  "_cxx_unsigned"},
      {"",}, 
      {"union",  "_cxx_union"},
      {"",}, {"",}, {"",}, 
      {"goto",  "_cxx_goto"},
      {"",}, {"",}, {"",}, 
      {"and",  "_cxx_and"},
      {"auto",  "_cxx_auto"},
      {"",}, {"",}, {"",}, {"",}, 
      {"namespace",  "_cxx_namespace"},
      {"using",  "_cxx_using"},
      {"",}, {"",}, {"",}, 
      {"else",  "_cxx_else"},
      {"",}, {"",}, {"",}, 
      {"asm",  "_cxx_asm"},
      {"",}, 
      {"while",  "_cxx_while"},
      {"",}, {"",}, 
      {"new",  "_cxx_new"},
      {"long",  "_cxx_long"},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"return",  "_cxx_return"},
      {"",}, {"",}, 
      {"void",  "_cxx_void"},
      {"false",  "_cxx_false"},
      {"",}, {"",}, 
      {"volatile",  "_cxx_volatile"},
      {"",}, {"",}, 
      {"switch",  "_cxx_switch"},
      {"",}, {"",}, {"",}, {"",}, 
      {"not_eq",  "_cxx_not_eq"},
      {"private",  "_cxx_private"},
      {"",}, 
      {"protected",  "_cxx_protected"},
      {"",}, 
      {"inline",  "_cxx_inline"},
      {"",}, 
      {"operator",  "_cxx_operator"},
      {"bool",  "_cxx_bool"},
      {"",}, 
      {"and_eq",  "_cxx_and_eq"},
      {"",}, 
      {"continue",  "_cxx_continue"},
      {"",}, 
      {"break",  "_cxx_break"},
      {"friend",  "_cxx_friend"},
      {"",}, 
      {"for",  "_cxx_for"},
      {"case",  "_cxx_case"},
      {"catch",  "_cxx_catch"},
      {"signed",  "_cxx_signed"},
      {"or",  "_cxx_or"},
      {"register",  "_cxx_register"},
      {"",}, {"",}, 
      {"public",  "_cxx_public"},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"or_eq",  "_cxx_or_eq"},
      {"bitand",  "_cxx_bitand"},
      {"",}, {"",}, {"",}, 
      {"compl",  "_cxx_compl"},
      {"extern",  "_cxx_extern"},
      {"",}, {"",}, 
      {"char",  "_cxx_char"},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"default",  "_cxx_default"},
      {"template",  "_cxx_template"},
      {"",}, 
      {"class",  "_cxx_class"},
      {"sizeof",  "_cxx_sizeof"},
      {"",}, 
      {"not",  "_cxx_not"},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"if",  "_cxx_if"},
      {"",}, 
      {"this",  "_cxx_this"},
      {"short",  "_cxx_short"},
      {"",}, {"",}, {"",}, {"",}, 
      {"bitor",  "_cxx_bitor"},
      {"",}, 
      {"virtual",  "_cxx_virtual"},
      {"xor",  "_cxx_xor"},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"throw",  "_cxx_throw"},
      {"",}, {"",}, {"",}, 
      {"true",  "_cxx_true"},
      {"",}, 
      {"xor_eq",  "_cxx_xor_eq"},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"reinterpret_cast",  "_cxx_reinterpret_cast"},
      {"",}, {"",}, {"",}, {"",}, 
      {"typeid",  "_cxx_typeid"},
      {"",}, 
      {"typename",  "_cxx_typename"},
      {"",}, {"",}, {"",}, 
      {"dynamic_cast",  "_cxx_dynamic_cast"},
      {"int",  "_cxx_int"},
      {"",}, 
      {"float",  "_cxx_float"},
      {"",}, {"",}, {"",}, {"",}, 
      {"const",  "_cxx_const"},
      {"",}, {"",}, {"",}, {"",}, 
      {"const_cast",  "_cxx_const_cast"},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, 
      {"static",  "_cxx_static"},
      {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"typedef",  "_cxx_typedef"},
      {"try",  "_cxx_try"},
      {"",}, {"",}, 
      {"export",  "_cxx_export"},
      {"",}, 
      {"explicit",  "_cxx_explicit"},
      {"",}, {"",}, {"",}, 
      {"wchar_t",  "_cxx_wchar_t"},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, 
      {"struct",  "_cxx_struct"},
      {"",}, {"",}, {"",}, {"",}, 
      {"static_cast",  "_cxx_static_cast"},
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= MIN_HASH_VALUE)
        {
          const char *s = wordlist[key].keyword_;

          if (*str == *s && !strncmp (str + 1, s + 1, len - 1))
            return &wordlist[key];
        }
    }
  return 0;
}
/* ending time is 17:20:28 */
