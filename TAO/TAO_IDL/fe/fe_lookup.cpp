// $Id$
#include	"idl.h"
#include	"idl_extern.h"
#include	"fe_private.h"
/* starting time is 2:40:07 */
/* C++ code produced by gperf version 2.8 (ACE version) */
/* Command-line: /project/adaptive/ACE_wrappers/build/SunOS5.5/bin/gperf -M -J -c -C -D -E -T -f 0 -a -o -t -p -K keyword_ -L C++ -Z TAO_IDL_CPP_Keyword_Table -N lookup -k1,2,$ fe/keywords.dat  */
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
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"delete",  "_cxx_delete"},
      {"mutable",  "_cxx_mutable"},
      {"",0},
      {"enum",  "_cxx_enum"},
      {"",0},
      {"double",  "_cxx_double"},
      {"do",  "_cxx_do"},
      {"unsigned",  "_cxx_unsigned"},
      {"",0},
      {"union",  "_cxx_union"},
      {"",0}, {"",0}, {"",0},
      {"goto",  "_cxx_goto"},
      {"",0}, {"",0}, {"",0},
      {"and",  "_cxx_and"},
      {"auto",  "_cxx_auto"},
      {"",0}, {"",0}, {"",0}, {"",0},
      {"namespace",  "_cxx_namespace"},
      {"using",  "_cxx_using"},
      {"",0}, {"",0}, {"",0},
      {"else",  "_cxx_else"},
      {"",0}, {"",0}, {"",0},
      {"asm",  "_cxx_asm"},
      {"",0},
      {"while",  "_cxx_while"},
      {"",0}, {"",0},
      {"new",  "_cxx_new"},
      {"long",  "_cxx_long"},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"return",  "_cxx_return"},
      {"",0}, {"",0},
      {"void",  "_cxx_void"},
      {"false",  "_cxx_false"},
      {"",0}, {"",0},
      {"volatile",  "_cxx_volatile"},
      {"",0}, {"",0},
      {"switch",  "_cxx_switch"},
      {"",0}, {"",0}, {"",0}, {"",0},
      {"not_eq",  "_cxx_not_eq"},
      {"private",  "_cxx_private"},
      {"",0},
      {"protected",  "_cxx_protected"},
      {"",0},
      {"inline",  "_cxx_inline"},
      {"",0},
      {"operator",  "_cxx_operator"},
      {"bool",  "_cxx_bool"},
      {"",0},
      {"and_eq",  "_cxx_and_eq"},
      {"",0},
      {"continue",  "_cxx_continue"},
      {"",0},
      {"break",  "_cxx_break"},
      {"friend",  "_cxx_friend"},
      {"",0},
      {"for",  "_cxx_for"},
      {"case",  "_cxx_case"},
      {"catch",  "_cxx_catch"},
      {"signed",  "_cxx_signed"},
      {"or",  "_cxx_or"},
      {"register",  "_cxx_register"},
      {"",0}, {"",0},
      {"public",  "_cxx_public"},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"or_eq",  "_cxx_or_eq"},
      {"bitand",  "_cxx_bitand"},
      {"",0}, {"",0}, {"",0},
      {"compl",  "_cxx_compl"},
      {"extern",  "_cxx_extern"},
      {"",0}, {"",0},
      {"char",  "_cxx_char"},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"default",  "_cxx_default"},
      {"template",  "_cxx_template"},
      {"",0},
      {"class",  "_cxx_class"},
      {"sizeof",  "_cxx_sizeof"},
      {"",0},
      {"not",  "_cxx_not"},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"if",  "_cxx_if"},
      {"",0},
      {"this",  "_cxx_this"},
      {"short",  "_cxx_short"},
      {"",0}, {"",0}, {"",0}, {"",0},
      {"bitor",  "_cxx_bitor"},
      {"",0},
      {"virtual",  "_cxx_virtual"},
      {"xor",  "_cxx_xor"},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"throw",  "_cxx_throw"},
      {"",0}, {"",0}, {"",0},
      {"true",  "_cxx_true"},
      {"",0},
      {"xor_eq",  "_cxx_xor_eq"},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"reinterpret_cast",  "_cxx_reinterpret_cast"},
      {"",0}, {"",0}, {"",0}, {"",0},
      {"typeid",  "_cxx_typeid"},
      {"",0},
      {"typename",  "_cxx_typename"},
      {"",0}, {"",0}, {"",0},
      {"dynamic_cast",  "_cxx_dynamic_cast"},
      {"int",  "_cxx_int"},
      {"",0},
      {"float",  "_cxx_float"},
      {"",0}, {"",0}, {"",0}, {"",0},
      {"const",  "_cxx_const"},
      {"",0}, {"",0}, {"",0}, {"",0},
      {"const_cast",  "_cxx_const_cast"},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"",0},
      {"static",  "_cxx_static"},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"typedef",  "_cxx_typedef"},
      {"try",  "_cxx_try"},
      {"",0}, {"",0},
      {"export",  "_cxx_export"},
      {"",0},
      {"explicit",  "_cxx_explicit"},
      {"",0}, {"",0}, {"",0},
      {"wchar_t",  "_cxx_wchar_t"},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"",0},
      {"struct",  "_cxx_struct"},
      {"",0}, {"",0}, {"",0}, {"",0},
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
/* ending time is 2:40:07 */
