// $Id$
#include	"idl.h"
#include	"idl_extern.h"
#include	"fe_private.h"
/* starting time is 14:07:18 */
/* C++ code produced by gperf version 2.7 (GNU C++ version) */
/* Command-line: /opt/gokhale/build/SunCC/bin/gperf -M -J -c -C -D -E -T -f 0 -a -o -t -p -K keyword_ -L C++ -Z TAO_IDL_CPP_Keyword_Table -N lookup -k1,2,$ fe/keywords.dat  */
unsigned int
TAO_IDL_CPP_Keyword_Table::hash (const char *str, unsigned int len)
{
  static const unsigned char asso_values[] =
    {
     243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
     243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
     243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
     243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
     243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
     243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
     243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
     243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
     243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
     243, 243, 243, 243, 243, 243, 243,  30,   0,  60,
       0,   0,  20,  35,  25,  55, 243,   0,  15,  45,
       5,   5,  50,  55,  30,  20,  95,   0,  70,  80,
      10,  45, 243, 243, 243, 243, 243, 243,
    };
  return len + asso_values[str[1]] + asso_values[str[0]] + asso_values[str[len - 1]];
}

const struct TAO_IDL_CPP_Keyword_Entry *
TAO_IDL_CPP_Keyword_Table::lookup (const char *str, unsigned int len)
{
  enum
    {
      TOTAL_KEYWORDS = 73,
      MIN_WORD_LENGTH = 2,
      MAX_WORD_LENGTH = 16,
      MIN_HASH_VALUE = 6,
      MAX_HASH_VALUE = 242,
      HASH_VALUE_RANGE = 237,
      DUPLICATES = 0
    };

  static const struct TAO_IDL_CPP_Keyword_Entry  wordlist[] =
    {
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"delete",  "_cxx_delete"},
      {"",}, {"",}, {"",}, {"",}, 
      {"double",  "_cxx_double"},
      {"do",  "_cxx_do"},
      {"unsigned",  "_cxx_unsigned"},
      {"",}, 
      {"union",  "_cxx_union"},
      {"",}, {"",}, {"",}, 
      {"else",  "_cxx_else"},
      {"",}, 
      {"extern",  "_cxx_extern"},
      {"",}, {"",}, 
      {"bool",  "_cxx_bool"},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, 
      {"break",  "_cxx_break"},
      {"",}, {"",}, 
      {"and",  "_cxx_and"},
      {"auto",  "_cxx_auto"},
      {"",}, 
      {"return",  "_cxx_return"},
      {"",}, {"",}, 
      {"namespace",  "_cxx_namespace"},
      {"",}, {"",}, {"",}, 
      {"xor",  "_cxx_xor"},
      {"goto",  "_cxx_goto"},
      {"",}, {"",}, 
      {"mutable",  "_cxx_mutable"},
      {"",}, 
      {"enum",  "_cxx_enum"},
      {"false",  "_cxx_false"},
      {"friend",  "_cxx_friend"},
      {"",}, 
      {"for",  "_cxx_for"},
      {"long",  "_cxx_long"},
      {"using",  "_cxx_using"},
      {"bitand",  "_cxx_bitand"},
      {"",}, {"",}, {"",}, {"",}, 
      {"inline",  "_cxx_inline"},
      {"or",  "_cxx_or"},
      {"register",  "_cxx_register"},
      {"",}, {"",}, 
      {"not_eq",  "_cxx_not_eq"},
      {"",}, 
      {"continue",  "_cxx_continue"},
      {"",}, {"",}, 
      {"xor_eq",  "_cxx_xor_eq"},
      {"",}, {"",}, 
      {"void",  "_cxx_void"},
      {"",}, 
      {"signed",  "_cxx_signed"},
      {"",}, 
      {"volatile",  "_cxx_volatile"},
      {"",}, 
      {"compl",  "_cxx_compl"},
      {"",}, 
      {"private",  "_cxx_private"},
      {"new",  "_cxx_new"},
      {"protected",  "_cxx_protected"},
      {"bitor",  "_cxx_bitor"},
      {"",}, {"",}, 
      {"operator",  "_cxx_operator"},
      {"case",  "_cxx_case"},
      {"or_eq",  "_cxx_or_eq"},
      {"and_eq",  "_cxx_and_eq"},
      {"if",  "_cxx_if"},
      {"asm",  "_cxx_asm"},
      {"",}, 
      {"class",  "_cxx_class"},
      {"sizeof",  "_cxx_sizeof"},
      {"default",  "_cxx_default"},
      {"template",  "_cxx_template"},
      {"",}, {"",}, {"",}, {"",}, 
      {"not",  "_cxx_not"},
      {"",}, 
      {"while",  "_cxx_while"},
      {"",}, {"",}, 
      {"explicit",  "_cxx_explicit"},
      {"",}, {"",}, 
      {"public",  "_cxx_public"},
      {"",}, {"",}, 
      {"char",  "_cxx_char"},
      {"catch",  "_cxx_catch"},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"true",  "_cxx_true"},
      {"",}, 
      {"switch",  "_cxx_switch"},
      {"",}, {"",}, {"",}, 
      {"float",  "_cxx_float"},
      {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"reinterpret_cast",  "_cxx_reinterpret_cast"},
      {"",}, {"",}, 
      {"this",  "_cxx_this"},
      {"short",  "_cxx_short"},
      {"typeid",  "_cxx_typeid"},
      {"virtual",  "_cxx_virtual"},
      {"typename",  "_cxx_typename"},
      {"",}, {"",}, {"",}, 
      {"dynamic_cast",  "_cxx_dynamic_cast"},
      {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"int",  "_cxx_int"},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"const",  "_cxx_const"},
      {"",}, 
      {"typedef",  "_cxx_typedef"},
      {"",}, {"",}, 
      {"const_cast",  "_cxx_const_cast"},
      {"",}, {"",}, 
      {"try",  "_cxx_try"},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"static",  "_cxx_static"},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"throw",  "_cxx_throw"},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, 
      {"struct",  "_cxx_struct"},
      {"",}, {"",}, {"",}, {"",}, 
      {"static_cast",  "_cxx_static_cast"},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, {"",}, 
      {"wchar_t",  "_cxx_wchar_t"},
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
/* ending time is 14:07:18 */
