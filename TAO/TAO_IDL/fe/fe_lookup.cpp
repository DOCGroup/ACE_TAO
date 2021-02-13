#include "fe_private.h"
#include "ace/OS_NS_string.h"

/* C++ code produced by gperf version 2.8 (ACE version) */
/* Command-line: ace_gperf -M -J -c -C -D -E -T -f 0 -a -o -t -p -K keyword_ -L C++ -Z TAO_IDL_CPP_Keyword_Table -N lookup -k1,2,$ fe/keywords.dat */

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
  return len + asso_values[static_cast<int>(str[1])] + asso_values[static_cast<int>(str[0])] + asso_values[static_cast<int>(str[len - 1])];
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
      DUPLICATES = 0,
      WORDLIST_SIZE = 80
    };

  static const struct TAO_IDL_CPP_Keyword_Entry wordlist[] =
    {
      {"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},
      {"delete",  "_cxx_delete"},
      {"mutable",  "_cxx_mutable"},
      {"",nullptr},
      {"enum",  "_cxx_enum"},
      {"",nullptr},
      {"double",  "_cxx_double"},
      {"do",  "_cxx_do"},
      {"unsigned",  "_cxx_unsigned"},
      {"",nullptr},
      {"union",  "_cxx_union"},
      {"",nullptr},{"",nullptr},{"",nullptr},
      {"goto",  "_cxx_goto"},
      {"",nullptr},{"",nullptr},{"",nullptr},
      {"and",  "_cxx_and"},
      {"auto",  "_cxx_auto"},
      {"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},
      {"namespace",  "_cxx_namespace"},
      {"using",  "_cxx_using"},
      {"",nullptr},{"",nullptr},{"",nullptr},
      {"else",  "_cxx_else"},
      {"",nullptr},{"",nullptr},{"",nullptr},
      {"asm",  "_cxx_asm"},
      {"",nullptr},
      {"while",  "_cxx_while"},
      {"",nullptr},{"",nullptr},
      {"new",  "_cxx_new"},
      {"long",  "_cxx_long"},
      {"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},
      {"return",  "_cxx_return"},
      {"",nullptr},{"",nullptr},
      {"void",  "_cxx_void"},
      {"false",  "_cxx_false"},
      {"",nullptr},{"",nullptr},
      {"volatile",  "_cxx_volatile"},
      {"",nullptr},{"",nullptr},
      {"switch",  "_cxx_switch"},
      {"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},
      {"not_eq",  "_cxx_not_eq"},
      {"private",  "_cxx_private"},
      {"",nullptr},
      {"protected",  "_cxx_protected"},
      {"",nullptr},
      {"inline",  "_cxx_inline"},
      {"",nullptr},
      {"operator",  "_cxx_operator"},
      {"bool",  "_cxx_bool"},
      {"",nullptr},
      {"and_eq",  "_cxx_and_eq"},
      {"",nullptr},
      {"continue",  "_cxx_continue"},
      {"",nullptr},
      {"break",  "_cxx_break"},
      {"friend",  "_cxx_friend"},
      {"",nullptr},
      {"for",  "_cxx_for"},
      {"case",  "_cxx_case"},
      {"catch",  "_cxx_catch"},
      {"signed",  "_cxx_signed"},
      {"or",  "_cxx_or"},
      {"register",  "_cxx_register"},
      {"",nullptr},{"",nullptr},
      {"public",  "_cxx_public"},
      {"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},
      {"or_eq",  "_cxx_or_eq"},
      {"bitand",  "_cxx_bitand"},
      {"",nullptr},{"",nullptr},{"",nullptr},
      {"compl",  "_cxx_compl"},
      {"extern",  "_cxx_extern"},
      {"",nullptr},{"",nullptr},
      {"char",  "_cxx_char"},
      {"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},
      {"default",  "_cxx_default"},
      {"template",  "_cxx_template"},
      {"",nullptr},
      {"class",  "_cxx_class"},
      {"sizeof",  "_cxx_sizeof"},
      {"",nullptr},
      {"not",  "_cxx_not"},
      {"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},
      {"if",  "_cxx_if"},
      {"",nullptr},
      {"this",  "_cxx_this"},
      {"short",  "_cxx_short"},
      {"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},
      {"bitor",  "_cxx_bitor"},
      {"",nullptr},
      {"virtual",  "_cxx_virtual"},
      {"xor",  "_cxx_xor"},
      {"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},
      {"throw",  "_cxx_throw"},
      {"",nullptr},{"",nullptr},{"",nullptr},
      {"true",  "_cxx_true"},
      {"",nullptr},
      {"xor_eq",  "_cxx_xor_eq"},
      {"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},
      {"reinterpret_cast",  "_cxx_reinterpret_cast"},
      {"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},
      {"typeid",  "_cxx_typeid"},
      {"",nullptr},
      {"typename",  "_cxx_typename"},
      {"",nullptr},{"",nullptr},{"",nullptr},
      {"dynamic_cast",  "_cxx_dynamic_cast"},
      {"int",  "_cxx_int"},
      {"",nullptr},
      {"float",  "_cxx_float"},
      {"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},
      {"const",  "_cxx_const"},
      {"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},
      {"const_cast",  "_cxx_const_cast"},
      {"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},
      {"",nullptr},
      {"static",  "_cxx_static"},
      {"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},
      {"typedef",  "_cxx_typedef"},
      {"try",  "_cxx_try"},
      {"",nullptr},{"",nullptr},
      {"export",  "_cxx_export"},
      {"",nullptr},
      {"explicit",  "_cxx_explicit"},
      {"",nullptr},{"",nullptr},{"",nullptr},
      {"wchar_t",  "_cxx_wchar_t"},
      {"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},
      {"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},
      {"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},
      {"",nullptr},
      {"struct",  "_cxx_struct"},
      {"",nullptr},{"",nullptr},{"",nullptr},{"",nullptr},
      {"static_cast",  "_cxx_static_cast"},
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= MIN_HASH_VALUE)
        {
          const char *s = wordlist[key].keyword_;

          if (len == static_cast<unsigned int> (ACE_OS::strlen (s)) && *str == *s && !ACE_OS::strncmp (str + 1, s + 1, len - 1))
            return &wordlist[key];
        }
    }
  return nullptr;
}
