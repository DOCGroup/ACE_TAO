// $Id$
#include	"idl.h"
#include	"idl_extern.h"
#include	"fe_private.h"
/* starting time is 18:04:44 */
/* C++ code produced by gperf version 2.6 (GNU C++ version) */
/* Command-line: /home/gokhale/research/projects/ACE_wrappers/bin/gperf -M -J -c -C -D -E -T -f 0 -a -o -t -p -K keyword_ -L C++ -Z TAO_IDL_CPP_Keyword_Table -N lookup fe/keywords.dat  */
unsigned int
TAO_IDL_CPP_Keyword_Table::hash (const char *str, int len)
{
  static const unsigned char asso_values[] =
    {
     125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
     125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
     125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
     125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
     125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
     125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
     125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
     125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
     125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
     125, 125, 125, 125, 125, 125, 125,  45,  70,  30,
      15,   5,  45,  60,  65,   0, 125,  15,  10,  60,
      75,  60,  20,  20,   0,  25,   0,   0,  40,   5,
      85,  25, 125, 125, 125, 125, 125, 125,
    };
  return len + asso_values[str[len - 1]] + asso_values[str[0]];
}

const struct TAO_IDL_CPP_Keyword_Entry *
TAO_IDL_CPP_Keyword_Table::lookup (const char *str, int len)
{
  enum
    {
      TOTAL_KEYWORDS = 73,
      MIN_WORD_LENGTH = 2,
      MAX_WORD_LENGTH = 16,
      MIN_HASH_VALUE = 3,
      MAX_HASH_VALUE = 124,
      HASH_VALUE_RANGE = 122,
      DUPLICATES = 4
    };

  static const struct TAO_IDL_CPP_Keyword_Entry wordlist[] =
    {
      {"",}, {"",}, {"",}, 
      {"int",  "_cxx_int"},
      {"register",  "_cxx_register"},
      {"true",  "_cxx_true"},
      {"throw",  "_cxx_throw"},
      {"inline",  "_cxx_inline"},
      {"wchar_t",  "_cxx_wchar_t"},
      {"explicit",  "_cxx_explicit"},
      {"typename",  "_cxx_typename"},
      {"template",  "_cxx_template"},
      {"else",  "_cxx_else"},
      {"while",  "_cxx_while"},
      {"reinterpret_cast",  "_cxx_reinterpret_cast"},
      {"typeid",  "_cxx_typeid"},
      {"default",  "_cxx_default"},
      {"unsigned",  "_cxx_unsigned"},
      {"delete",  "_cxx_delete"},
      {"double",  "_cxx_double"},
      {"dynamic_cast",  "_cxx_dynamic_cast"},
      {"try",  "_cxx_try"},
      {"this",  "_cxx_this"},
      {"short",  "_cxx_short"},
      {"struct",  "_cxx_struct"},
      {"private",  "_cxx_private"},
      {"char",  "_cxx_char"},
      {"const",  "_cxx_const"},
      {"static_cast",  "_cxx_static_cast"},
      {"case",  "_cxx_case"},
      {"const_cast",  "_cxx_const_cast"},
      {"continue",  "_cxx_continue"},
      {"protected",  "_cxx_protected"},
      {"compl",  "_cxx_compl"},
      {"signed",  "_cxx_signed"},
      {"if",  "_cxx_if"},
      {"for",  "_cxx_for"},
      {"float",  "_cxx_float"},
      {"typedef",  "_cxx_typedef"},
      {"volatile",  "_cxx_volatile"},
      {"false",  "_cxx_false"},
      {"public",  "_cxx_public"},
      {"virtual",  "_cxx_virtual"},
      {"void",  "_cxx_void"},
      {"class",  "_cxx_class"},
      {"static",  "_cxx_static"},
      {"or",  "_cxx_or"},
      {"and",  "_cxx_and"},
      {"using",  "_cxx_using"},
      {"friend",  "_cxx_friend"},
      {"operator",  "_cxx_operator"},
      {"enum",  "_cxx_enum"},
      {"and_eq",  "_cxx_and_eq"},
      {"mutable",  "_cxx_mutable"},
      {"long",  "_cxx_long"},
      {"bitor",  "_cxx_bitor"},
      {"sizeof",  "_cxx_sizeof"},
      {"do",  "_cxx_do"},
      {"not",  "_cxx_not"},
      {"union",  "_cxx_union"},
      {"return",  "_cxx_return"},
      {"new",  "_cxx_new"},
      {"bool",  "_cxx_bool"},
      {"or_eq",  "_cxx_or_eq"},
      {"extern",  "_cxx_extern"},
      {"xor",  "_cxx_xor"},
      {"namespace",  "_cxx_namespace"},
      {"break",  "_cxx_break"},
      {"bitand",  "_cxx_bitand"},
      {"switch",  "_cxx_switch"},
      {"catch",  "_cxx_catch"},
      {"not_eq",  "_cxx_not_eq"},
      {"asm",  "_cxx_asm"},
      {"auto",  "_cxx_auto"},
      {"xor_eq",  "_cxx_xor_eq"},
      {"goto",  "_cxx_goto"},
    };

  static const short lookup[] =
    {
        -1,   -1,   -1,    3,   -1,   -1,   -1,   -1,    4,    5,    6,    7,    8,  128, 
        12,   13,   14,   -9,   -3,   -1,   -1,   15,   16,   17,   -1,   -1,  135,   20, 
        21,   22,   23,   24,   25,   -1,   26,   27,   28,  -18,   -2,   29,   30,   -1, 
        -1,   31,   32,   33,   34,   35,   36,   -1,   37,   -1,   38,   39,   -1,   40, 
        41,   42,   -1,   43,   44,   45,   46,   47,   -1,   48,   49,   -1,   50,   51, 
        -1,   52,   53,   -1,   54,   55,   56,   57,   58,   -1,   59,   60,   -1,   61, 
        62,   63,   64,   -1,   65,   66,   67,   68,   -1,   -1,   -1,   -1,   69,   -1, 
        -1,   -1,   70,   71,   -1,   -1,   -1,   -1,   -1,   -1,   72,   73,   -1,   74, 
        -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   75, 
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= MIN_HASH_VALUE)
        {
          int index = lookup[key];

          if (index >= 0 && index < MAX_HASH_VALUE)
            {
              const char *s = wordlist[index].keyword_;

              if (*str == *s && !strncmp (str + 1, s + 1, len - 1))
                return &wordlist[index];
            }
          else if (index < 0 && index >= -MAX_HASH_VALUE)
            return 0;
          else
            {
              int offset = key + index + (index > 0 ? -MAX_HASH_VALUE : MAX_HASH_VALUE);
              const struct TAO_IDL_CPP_Keyword_Entry *base = &wordlist[-lookup[offset]];
              const struct TAO_IDL_CPP_Keyword_Entry *ptr = base + -lookup[offset + 1];

              while (--ptr >= base)
                if (*str == *ptr->keyword_ && !strncmp (str + 1, ptr->keyword_ + 1, len - 1))
                  return ptr;
            }
        }
    }
  return 0;
}
/* ending time is 18:04:44 */
