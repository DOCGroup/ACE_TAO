#include "ace/OS.h"
#include "ace/Codecs.h"
#include "ace/Log_Msg.h"

ACE_RCSID (ace,
           Codecs,
           "$Id$")

const ACE_Byte ACE_Base64::alphabet_[] =
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

const ACE_Byte ACE_Base64::pad_ = '=';

int ACE_Base64::init_ = 0;

int ACE_Base64::max_columns_ = 72;

ACE_Byte ACE_Base64::decoder_[256];

ACE_Byte ACE_Base64::member_[256];

ACE_Byte*
ACE_Base64::encode (const ACE_Byte* input,
                    const size_t input_len,
                    size_t* output_len)
{
  if (!ACE_Base64::init_)
    ACE_Base64::init();

  if (!input)
    return 0;

  ACE_Byte* result = 0;

  size_t length = ((input_len + 2) / 3) * 4;
  size_t num_lines = length / ACE_Base64::max_columns_ + 1;
  length += num_lines + 1;
  ACE_NEW_RETURN (result, ACE_Byte[length], 0);

  int char_count = 0;
  int bits = 0;
  size_t pos = 0;
  int cols = 0;

  for (size_t i = 0; i < input_len; ++i)
    {
      bits += input[i];
      char_count++;

      if (char_count == 3)
        {
          result[pos++] = ACE_Base64::alphabet_[bits >> 18];
          result[pos++] = ACE_Base64::alphabet_[(bits >> 12) & 0x3f];
          result[pos++] = ACE_Base64::alphabet_[(bits >> 6) & 0x3f];
          result[pos++] = ACE_Base64::alphabet_[bits & 0x3f];
          cols += 4;
          if (cols == ACE_Base64::max_columns_) {
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
      result[pos++] = ACE_Base64::alphabet_[bits >> 18];
      result[pos++] = ACE_Base64::alphabet_[(bits >> 12) & 0x3f];
      if (char_count == 1)
        {
          result[pos++] = pad_;
          result[pos++] = pad_;
        }
      else
        {
          result[pos++] = ACE_Base64::alphabet_[(bits >> 6) & 0x3f];
          result[pos++] = pad_;
        }
      if (cols > 0)
        result[pos++] = '\n';
    }
  result[pos] = 0;
  *output_len = pos;
  return result;
}

size_t
ACE_Base64::length (const ACE_Byte* input)
{
  if (!ACE_Base64::init_)
    ACE_Base64::init();

  ACE_Byte* ptr = ACE_const_cast (ACE_Byte*, input);
  while (*ptr != 0 &&
         (member_[*(ptr)] == 1 || *ptr == pad_
          || ACE_OS_String::ace_isspace (*ptr)))
    ptr++;
  size_t len = ptr - input;
  len = ((len + 3) / 4) * 3 + 1 ;
  return len;
}

ACE_Byte*
ACE_Base64::decode (const ACE_Byte* input, size_t* output_len)
{
  if (!ACE_Base64::init_)
    ACE_Base64::init();

  if (!input)
    return 0;

  size_t result_len = ACE_Base64::length (input);
  ACE_Byte* result = 0;
  ACE_NEW_RETURN (result, ACE_Byte[result_len], 0);

  ACE_Byte* ptr = ACE_const_cast (ACE_Byte*, input);
  while (*ptr != 0 &&
         (member_[*(ptr)] == 1 || *ptr == pad_
          || ACE_OS_String::ace_isspace (*ptr)))
    ptr++;
  size_t input_len = ptr - input;

  int char_count = 0;
  int bits = 0;
  size_t pos = 0;

  size_t i = 0;
  for (; i < input_len; ++i)
    {
      if (input[i] == pad_)
        break;
      if (!ACE_Base64::member_[input[i]])
        continue;
      bits += decoder_[input[i]];
      char_count++;

      if (char_count == 4)
        {
          result[pos++] = bits >> 16;
          result[pos++] = (bits >> 8) & 0xff;
          result[pos++] = bits & 0xff;
          bits = 0;
          char_count = 0;
        }
      else
        {
          bits <<= 6;
        }
    }

  int errors = 0;
  if ( i == input_len)
    {
      if (char_count)
        {
          ACE_DEBUG ((LM_ERROR,
                      ACE_TEXT ("Decoding incomplete: atleast %d bits truncated\n"),
                      (4 - char_count) * 6));
          errors++;
        }
    }
  else
    {
      switch (char_count)
        {
        case 1:
          ACE_DEBUG ((LM_ERROR,
                      ACE_TEXT ("Decoding incomplete: atleast 2 bits missing\n")));
          errors++;
          break;
        case 2:
          result[pos++] = bits >> 10;
          break;
        case 3:
          result[pos++] = bits >> 16;
          result[pos++] = (bits >> 8) & 0xff;
          break;
        }
    }

  if (errors)
    {
      delete[] result;
      return 0;
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
      for (ACE_Byte i = 0; i < sizeof (ACE_Base64::alphabet_); ++i)
        {
          ACE_Base64::decoder_[ACE_Base64::alphabet_[i]] = i;
          ACE_Base64::member_[ACE_Base64::alphabet_[i]] = 1;
        }
      ACE_Base64::init_ = 1;
    }
  return;
}
