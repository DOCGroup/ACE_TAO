#include "ace/Codecs.h"
#include "ace/Log_Category.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_ctype.h"

#include <array>

#if defined (ACE_HAS_ALLOC_HOOKS)
# include "ace/Malloc_Base.h"
#endif /* ACE_HAS_ALLOC_HOOKS */

namespace
{
  // Just in case ...
#undef alphabet
#undef pad
#undef max_columns

  // Symbols which form the Base64 alphabet (Defined as per RFC 2045)
  constexpr ACE_Byte alphabet[] =
     "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  // The padding character used in the encoding
  constexpr ACE_Byte pad = '=';

  // Number of columns per line of encoded output (Can have a maximum
  // value of 76).
  constexpr int max_columns = 72;
}

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

bool ACE_Base64::init_ {};

std::array<ACE_Byte, 256> ACE_Base64::decoder_ {};

std::array<ACE_Byte, 256> ACE_Base64::member_ {};

ACE_Byte *
ACE_Base64::encode (ACE_Byte const *input,
                    size_t const input_len,
                    size_t *output_len,
                    bool is_chunked)
{
  if (!ACE_Base64::init_)
    ACE_Base64::init();

  if (input == nullptr)
    return nullptr;

  ACE_Byte *result {};

  size_t length = ((input_len + 2) / 3) * 4;
  size_t const num_lines = length / max_columns + 1;
  length += num_lines + 1;
#if defined (ACE_HAS_ALLOC_HOOKS)
  ACE_ALLOCATOR_RETURN (
    result,
    static_cast<ACE_Byte *> (
      ACE_Allocator::instance ()->malloc (sizeof (ACE_Byte) * length)),
    0);
#else
  ACE_NEW_RETURN (result, ACE_Byte[length], nullptr);
#endif /* ACE_HAS_ALLOC_HOOKS */

  int char_count {};
  int bits {};
  size_t pos {};
  int cols {};

  for (size_t i {}; i < input_len; ++i)
    {
      bits += input[i];
      ++char_count;

      if (char_count == 3)
        {
          result[pos++] = alphabet[bits >> 18];
          result[pos++] = alphabet[(bits >> 12) & 0x3f];
          result[pos++] = alphabet[(bits >> 6) & 0x3f];
          result[pos++] = alphabet[bits & 0x3f];
          cols += 4;
          if (cols == max_columns)
            {
              if (is_chunked)
                result[pos++] = '\n';
              cols = 0;
            }
          bits = 0;
          char_count = 0;
        }
      else
        {
          bits <<= 8;
        }
    }

  if (char_count != 0)
    {
      bits <<= (16 - (8 * char_count));
      result[pos++] = alphabet[bits >> 18];
      result[pos++] = alphabet[(bits >> 12) & 0x3f];
      cols += 2;
      if (char_count == 1)
        {
          result[pos++] = pad;
          result[pos++] = pad;
          cols += 2;
        }
      else
        {
          result[pos++] = alphabet[(bits >> 6) & 0x3f];
          result[pos++] = pad;
          cols += 2;
        }
    }

  if (cols > 0 && is_chunked)
    result[pos++] = '\n';

  result[pos] = 0;
  *output_len = pos;
  return result;
}

size_t
ACE_Base64::length (ACE_Byte const *input)
{
  if (!ACE_Base64::init_)
    ACE_Base64::init();

  ACE_Byte const *ptr = input;
  while (*ptr != 0 &&
         (member_[*ptr] == 1 || *ptr == pad
          || ACE_OS::ace_isspace (*ptr)))
    ++ptr;
  size_t len = static_cast<size_t> (ptr - input);
  len = ((len + 3) / 4) * 3 + 1 ;
  return len;
}

ACE_Byte *
ACE_Base64::decode (ACE_Byte const *input, size_t *output_len)
{
  if (!ACE_Base64::init_)
    ACE_Base64::init();

  if (input == nullptr)
    return nullptr;

  size_t const result_len = ACE_Base64::length (input);
  ACE_Byte *result {};

#if defined (ACE_HAS_ALLOC_HOOKS)
  ACE_ALLOCATOR_RETURN (
    result,
    static_cast<ACE_Byte *> (
      ACE_Allocator::instance ()->malloc (sizeof (ACE_Byte) * result_len)),
    0);
#else
  ACE_NEW_RETURN (result, ACE_Byte[result_len], nullptr);
#endif /* ACE_HAS_ALLOC_HOOKS */

  ACE_Byte const *ptr = input;
  while (*ptr != 0 &&
         (member_[*ptr] == 1 || *ptr == pad
          || ACE_OS::ace_isspace (*ptr)))
    ++ptr;
  size_t const input_len = static_cast<size_t> (ptr - input);

  int char_count {};
  int bits {};
  size_t pos {};

  size_t i {};
  for (; i < input_len; ++i)
    {
      if (input[i] == pad)
        break;
      if (!ACE_Base64::member_[input[i]])
        continue;
      bits += decoder_[input[i]];
      ++char_count;

      if (char_count == 4)
        {
          result[pos++] = static_cast<ACE_Byte> (bits >> 16);
          result[pos++] = static_cast<ACE_Byte> ((bits >> 8) & 0xff);
          result[pos++] = static_cast<ACE_Byte> (bits & 0xff);
          bits = 0;
          char_count = 0;
        }
      else
        {
          bits <<= 6;
        }
    }

  bool has_error {};
  if ( i == input_len)
    {
      if (char_count)
        {
          ACELIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("Decoding incomplete: atleast %d bits truncated\n"),
                      (4 - char_count) * 6));
          has_error = true;
        }
    }
  else
    {
      switch (char_count)
        {
        case 1:
          ACELIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("Decoding incomplete: atleast 2 bits missing\n")));
          has_error = true;
          break;
        case 2:
          result[pos++] = static_cast<ACE_Byte> (bits >> 10);
          break;
        case 3:
          result[pos++] = static_cast<ACE_Byte> (bits >> 16);
          result[pos++] = static_cast<ACE_Byte> ((bits >> 8) & 0xff);
          break;
        }
    }

  if (has_error)
    {
#if defined (ACE_HAS_ALLOC_HOOKS)
      ACE_Allocator::instance()->free(result);
#else
      delete[] result;
#endif /* ACE_HAS_ALLOC_HOOKS */
      return nullptr;
    }
  result[pos] = 0;
  *output_len = pos;
  return result;
}

void
ACE_Base64::init ()
{
  if (!ACE_Base64::init_)
    {
      for (ACE_Byte i {}; i < sizeof (alphabet); ++i)
        {
          ACE_Base64::decoder_[alphabet[i]] = i;
          ACE_Base64::member_ [alphabet[i]] = 1;
        }
      ACE_Base64::init_ = true;
    }
  return;
}

ACE_END_VERSIONED_NAMESPACE_DECL
