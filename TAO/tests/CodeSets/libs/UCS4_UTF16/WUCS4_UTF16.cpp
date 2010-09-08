// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    WUCS4_UTF16.cpp
//
// = DESCRIPTION
//    Defines the arrays required to convert between UCS-4 a 4 byte wide char
//    codeset, and UCS-16, aka unicode, a 2-byte codeset.
//
// = AUTHOR
//    Phil Mesnier <mesnier_p@ociweb.com>
//
// ============================================================================

#include "WUCS4_UTF16.h"
#include "ace/OS.h"

// ****************************************************************


// @@ TODO: Find a better home for these definition
// Note: unlike the UNICODE standard we define these as
// half-closed ranges i.e.
//    *BEGIN is the first value in the range
//    *END is the first value beyond the range (END is not included
//         in the range)
// Note the use of unsigned short for UTF-16 codepoints.  wchar_t may
// by four bytes
typedef ACE_CDR::UShort ACE_UTF16_T;
static const size_t ACE_UTF16_CODEPOINT_SIZE = sizeof(ACE_UTF16_T);

// surrogate high           1101.10HH.HHHH.HHHH
// surrogate low            1101.11LL.LLLL.LLLL
// 4 byte result:           0000.0000.0000.HHHH.HHHH.HHLL.LLLL.LLLL
// add offset               0000.0000.0000.0000.0001.0000.0000.0000

// range of surrogate values for high-order bits
static const unsigned short ACE_UTF16_SURROGATE_HIGH_BEGIN = 0xD800U;
static const unsigned short ACE_UTF16_SURROGATE_HIGH_END = 0xDC00U;

static const unsigned short ACE_UTF16_SURROGATE_LOW_BEGIN = 0xDC00U;
static const unsigned short ACE_UTF16_SURROGATE_LOW_END = 0xE000U;

// offset to UTF16 values encoded with surrogates start at 2^16
static const unsigned long ACE_UTF16_SURROGATE_OFFSET = 0x000010000UL;

// shift high order bits from surrogate into correct postion
static const int ACE_UTF16_SURROGATE_HIGH_SHIFT = 10;
static const unsigned short ACE_UTF16_SURROGATE_LOW_MASK = 0x3FF;

// largest value that can be represented in UTF16 without using surrogates + 1
static const unsigned long ACE_UTF16_RAW_END = 0x00010000LU;

// largest value that can be represented in UTF16 + 1
static const unsigned long ACE_UTF16_END = 0x00110000LU;

// largest value that can be represented in UTF-32 + 1
static const unsigned long ACE_UTF32_END = 0x80000000LU;

static const unsigned short ACE_UNICODE_SUBSTITUTE_CHARACTER = 0xFFFDU;
static const unsigned short ACE_UNICODE_BOM_CORRECT = 0xFEFFU;
static const unsigned short ACE_UNICODE_BOM_SWAPPED = 0xFFFEU;

/////////////////////////////////////////////////////
// Static inline routines to simplify conversion code
// @@ should be in anonymous namespace when ACE allows it
//    or better yet, there should be a UTF-16 support thingie(technical term)
//    that provides these methods.
// Performance: depends on compiler inlining + optimization for performance

/// load next two bytes from buffer into a short. Byte swapping as necessary
static
//ACE_INLINE
ACE_UTF16_T
load_raw_wchar (const char * buffer, size_t & pos, int do_byte_swap)
{
  // need a two byte object to load the UTF16 2 byte codepoint
  ACE_UTF16_T utf16_char = * reinterpret_cast<ACE_UTF16_T const *> (&buffer[pos*ACE_UTF16_CODEPOINT_SIZE]);
#if ! defined (ACE_DISABLE_SWAP_ON_READ)
  if (do_byte_swap)
    {
      ACE_CDR::swap_2 (
        &buffer[pos*ACE_UTF16_CODEPOINT_SIZE],
        reinterpret_cast<char *> (&utf16_char));
    }
#endif
  pos ++;
  return utf16_char;
}

/// convert UTF-16 surrogate pair to wchar_t
static
//ACE_INLINE
ACE_CDR::WChar
convert_surrogate_pair (ACE_UTF16_T high, ACE_UTF16_T low)
{
  return static_cast<ACE_CDR::WChar> (((high - ACE_UTF16_SURROGATE_HIGH_BEGIN) << ACE_UTF16_SURROGATE_HIGH_SHIFT)
    + (low - ACE_UTF16_SURROGATE_LOW_BEGIN)
    + ACE_UTF16_SURROGATE_OFFSET
    );
}

/// load wchar from utf16 buffer
/// converts surrogate pairs
/// substitutes SUBSTITUTE_CHAR for bad encoding
static
//ACE_INLINE
ACE_CDR::WChar
load_wchar (const char * buffer, size_t & pos, size_t length, int do_byte_swap)
{
  ACE_CDR::WChar rc = ACE_UNICODE_SUBSTITUTE_CHARACTER;
  if (pos < length)
    {
      rc = static_cast<ACE_CDR::WChar> (load_raw_wchar (buffer, pos, do_byte_swap));
      // Is this a UTF16 surrogate?
      // note assumpton that SURROGATE_HIGH_END == SURROGATE_LOW_BEGIN
      if (rc >= ACE_UTF16_SURROGATE_HIGH_BEGIN && rc < ACE_UTF16_SURROGATE_LOW_END)
        {
          // if we still have two bytes available
          if (pos < length)
            {
              // expecting high surrogate
              if (rc < ACE_UTF16_SURROGATE_HIGH_END)
              {
                ACE_UTF16_T low = load_raw_wchar (buffer, pos, do_byte_swap);
                if (low >= ACE_UTF16_SURROGATE_LOW_BEGIN
                  && low < ACE_UTF16_SURROGATE_LOW_END)
                  {
                    rc = convert_surrogate_pair (
                      static_cast<ACE_UTF16_T> (rc), low);
                  }
                else
                  {
                    rc = ACE_UNICODE_SUBSTITUTE_CHARACTER;
                  }
              }
              else
              {
                rc = ACE_UNICODE_SUBSTITUTE_CHARACTER;
              }
            }
          else
            {
              rc = ACE_UNICODE_SUBSTITUTE_CHARACTER;
            }
        }
    }
  return rc;
}

static
//ACE_INLINE
size_t encode_utf16 (ACE_UTF16_T * buffer, ACE_CDR::WChar value)
{
  buffer[0] = static_cast<ACE_UTF16_T> (value);
  size_t length = 1;

  // On platforms where sizeof(ACE_CDR::WChar) == 2, the test using
  // ul_value will always be false, since we are improperly using
  // a 4-byte native wchar codeset. But since this is for a simple
  // test that has to run on machines with 4 byte wchars, this cast
  // avoids compile time issues of comparing a value that starts out
  // as a short with a constant that is too big for a short.
  unsigned long ul_value = static_cast<unsigned long>(value);
  if (value >= ACE_UTF16_SURROGATE_HIGH_BEGIN)
    {
      if (value < ACE_UTF16_SURROGATE_LOW_END)
        {
          buffer[0] = ACE_UNICODE_SUBSTITUTE_CHARACTER;
        }
      else if (ul_value >= ACE_UTF16_RAW_END)
        {
          if (ul_value >= ACE_UTF16_END)
            {
              buffer[0] = ACE_UNICODE_SUBSTITUTE_CHARACTER;
            }
          else
            {
              ACE_CDR::WChar offset = static_cast<ACE_CDR::WChar> (value - ACE_UTF16_SURROGATE_OFFSET);
              buffer[0] = (offset >> ACE_UTF16_SURROGATE_HIGH_SHIFT)
                + ACE_UTF16_SURROGATE_HIGH_BEGIN;
              buffer[1] = (offset & ACE_UTF16_SURROGATE_LOW_MASK)
                + ACE_UTF16_SURROGATE_LOW_BEGIN;
              length = 2;
            }
        }
    }
  return length;
}

/// count number of characters in native WString that will be converted
/// to UTF-16 surrogate pairs
static
size_t count_potential_surrogates (
    const ACE_CDR::WChar *buffer,
    ACE_CDR::ULong len)
{
  size_t count = 0;
  for (size_t i = 0; i < len; ++i)
    {
      // see comments above in encode_utf16().
      unsigned long ul_value = static_cast<unsigned long>(buffer[i]);
      if (ul_value >= ACE_UTF16_RAW_END &&
          ul_value < ACE_UTF16_END)
        {
          count += 1;
        }
    }
  return count;
}


/////////////////////////////
// WUCS4_UTF16 implementation

WUCS4_UTF16::WUCS4_UTF16 (void)
{

}

WUCS4_UTF16::~WUCS4_UTF16 (void)
{

}

// = Documented in $ACE_ROOT/ace/CDR_Stream.h
ACE_CDR::Boolean
WUCS4_UTF16::read_wchar (ACE_InputCDR &cdr, ACE_CDR::WChar &x)
{
  if (static_cast<ACE_CDR::Short> (this->major_version(cdr)) == 1
      && static_cast<ACE_CDR::Short> (this->minor_version(cdr)) > 1)
    {
      ACE_CDR::Octet len;
      if (! this->read_1 (cdr, &len))
        {
          return 0;
        }

      int old_bo = cdr.byte_order();

      ACE_UTF16_T sx = 0;
      if (! this->read_2 (cdr,&sx))
        {
          return 0;
        }

      // Check for byte order mark, if found, consume and honor it.
      if (sx == ACE_UNICODE_BOM_CORRECT || sx == ACE_UNICODE_BOM_SWAPPED)
        {
          // if we found it, but it came in in the wrong order
          // invert the byte order flag for the duration of this method
          if (sx == ACE_UNICODE_BOM_SWAPPED)
            {
              cdr.reset_byte_order (! old_bo);
            }
          this->read_2 (cdr,&sx);
        }

      // check for UTF-16 surrogate pair, and if found interpret it
      if (sx >= ACE_UTF16_SURROGATE_HIGH_BEGIN
        && sx < ACE_UTF16_SURROGATE_LOW_END)
        {
          if (sx >= ACE_UTF16_SURROGATE_HIGH_END)
            {
              cdr.reset_byte_order (old_bo);
              return 0;
            }

            ACE_UTF16_T low;
            if (! this->read_2 (cdr, &low))
              {
                cdr.reset_byte_order (old_bo);
                return 0;;
              }
            if (low < ACE_UTF16_SURROGATE_LOW_BEGIN
              || low >= ACE_UTF16_SURROGATE_LOW_END)
              {
                cdr.reset_byte_order (old_bo);
                return 0;
              }
            x = convert_surrogate_pair (sx, low);
        }
      else
        {
          x = static_cast<ACE_CDR::WChar> (sx);
        }

        cdr.reset_byte_order (old_bo);
    }
  else
    {
      ACE_UTF16_T sx = 0;
      if (!this->read_2 (cdr, &sx))
        {
          return 0;
        }
      x = static_cast<ACE_CDR::WChar> (sx);
    }
  return 1;
}

ACE_CDR::Boolean
WUCS4_UTF16::read_wstring (ACE_InputCDR &cdr,
                           ACE_CDR::WChar *&x)
{
  ACE_CDR::ULong len;
  if (!this->read_4 (cdr, &len))
    return 0;

  // A check for the length being too great is done later in the
  // call to read_char_array but we want to have it done before
  // the memory is allocated.
  if (len > 0 && len <= cdr.length())
    {
      if (static_cast<ACE_CDR::Short> (this->major_version(cdr)) == 1
          && static_cast<ACE_CDR::Short> (this->minor_version(cdr)) > 1)
        {
          len /= ACE_UTF16_CODEPOINT_SIZE;

          //allocating one extra for the null character needed by applications
          ACE_NEW_RETURN (x,
                          ACE_CDR::WChar [len + 1],
                          0);
          x[len] = L'\x00';
          if (this->read_wchar_array_i (cdr, x, len,1))
            {
              // Since reading the array may have adjusted the length,
              // we simply rewrite the null terminator
              x[len] = L'\x00';
              return 1;
            }
        }
      else
        {
          ACE_NEW_RETURN (x,
                          ACE_CDR::WChar [len],
                          0);
          if (this->read_wchar_array (cdr, x, len))
            return 1;
        }
      delete [] x;
    }
  else if (len == 0)
    {
      // Convert any null strings to empty strings since empty
      // strings can cause crashes. (See bug 58.)
      ACE_NEW_RETURN (x,
                      ACE_CDR::WChar[1],
                      0);
      x[0] = '\x00';
      return 1;
    }
  x = 0;
  return 0;
}

ACE_CDR::Boolean
WUCS4_UTF16::read_wchar_array_i (ACE_InputCDR & cdr,
                                 ACE_CDR::WChar *x,
                                 ACE_CDR::ULong &length,
                                 int adjust_len)
{
  if (length == 0)
    return 1;
  char* buf;
  size_t align = ACE_CDR::SHORT_ALIGN;
  if (cdr.adjust (ACE_UTF16_CODEPOINT_SIZE * length, align, buf) == 0)
    {
      int byte_swap = cdr.do_byte_swap();
      size_t pos = 0;

      // check for byte order mark.  If found, honor it then discard it
      ACE_UTF16_T bom = load_raw_wchar (buf, pos, byte_swap);
      if (bom == ACE_UNICODE_BOM_CORRECT || bom == ACE_UNICODE_BOM_SWAPPED)
        {
          if (bom == ACE_UNICODE_BOM_SWAPPED)
            {
              byte_swap = !byte_swap;
            }
          buf += ACE_UTF16_CODEPOINT_SIZE;
          if (adjust_len)
            length -= 1;
        }
      size_t bpos = 0;
      for (size_t xpos = 0; xpos < length; ++xpos)
        {
          x[xpos] = load_wchar (buf, bpos, length, byte_swap);
        }

      return 1;
    }
  return 0;

}


ACE_CDR::Boolean
WUCS4_UTF16::read_wchar_array (ACE_InputCDR & cdr,
                               ACE_CDR::WChar *x,
                               ACE_CDR::ULong length)
{
  if (length == 0)
    return 1;

  if (static_cast<ACE_CDR::Short> (this->major_version(cdr)) == 1
      && static_cast<ACE_CDR::Short> (this->minor_version(cdr)) > 1)
    {
      for (size_t i = 0; i < length; i++)
        if (!this->read_wchar(cdr,x[i]))
          return 0;
      return 1;
    }
  else
    return this->read_wchar_array_i(cdr,x,length);
}

ACE_CDR::Boolean
WUCS4_UTF16::write_wchar (ACE_OutputCDR &cdr,
                          ACE_CDR::WChar x)
{
  int encode_len = 1;
  if (static_cast<ACE_CDR::Short> (this->minor_version(cdr)) == 0)
    { // wchar is not allowed with GIOP 1.0
      errno = EINVAL;
      return 0;
    }
  else if (static_cast<ACE_CDR::Short> (this->minor_version(cdr)) == 1)
    encode_len = 0;

  return write_wchar_i(cdr,x,1,encode_len);
}

ACE_CDR::Boolean
WUCS4_UTF16::write_wchar_i (ACE_OutputCDR &cdr,
                            ACE_CDR::WChar x,
                            int use_BOM,
                            int encode_len)
{
  // If the desired char cannot be translated into a single unicode char,
  // we must raise a marshal exception.
  //
  // see the comment in encode_utf16() regarding the cast.
  unsigned long ul_x = static_cast<unsigned long>(x);
  if (ul_x >= ACE_UTF16_RAW_END &&
      ul_x < ACE_UTF16_END)
    return 0;

  int len = 0;
  ACE_CDR::UShort buffer[2];
  if (use_BOM)
    {
      len = 2;
      buffer[0] = ACE_UNICODE_BOM_CORRECT;
      buffer[1] = static_cast<ACE_CDR::Short> (x);
    }
  else
    {
      len = 1;
      if (cdr.byte_order())
        ACE_CDR::swap_2 (reinterpret_cast<const char *> (&x),
                         reinterpret_cast<char *> (buffer));
      else
        buffer[0] = static_cast<ACE_CDR::Short> (x);
    }

  if (encode_len)
    {
      unsigned char tcsize = static_cast<unsigned char> (len * ACE_UTF16_CODEPOINT_SIZE);
      if (this->write_1 (cdr, &tcsize))
        return this->write_array(cdr, &buffer, tcsize, 1, 1);
      else
        return 0;
    }
  if (this->write_2 (cdr, buffer) == 0)
    return 0;
  if (len == 2)
    return this->write_2 (cdr,buffer+1);
  return 1;
}

ACE_CDR::Boolean
WUCS4_UTF16::write_wstring (ACE_OutputCDR & cdr,
                ACE_CDR::ULong len,
                const ACE_CDR::WChar *x)
{
  if (static_cast<ACE_CDR::Short> (this->major_version(cdr)) == 1
      && static_cast<ACE_CDR::Short> (this->minor_version(cdr)) > 1)
    {
      // count characters that will require surrogates to
      // determine transmission length
      len++; // make room for BOM
      ACE_UTF16_T bom = ACE_UNICODE_BOM_CORRECT;
      ACE_CDR::ULong length = len + count_potential_surrogates (x, len);
      ACE_CDR::ULong l = length * ACE_UTF16_CODEPOINT_SIZE;

      if (this->write_4 (cdr, &l) && x != 0)
        {
          this->write_2 (cdr, &bom);
          return this->write_measured_wchar_array (cdr, x, len, length);
        }
    }
  else
    {
      ACE_CDR::ULong l = len + 1;

      if (this->write_4 (cdr, &l))
        {
          if (x != 0)
            {
              return this->write_wchar_array (cdr, x, len + 1);
            }
          else
            {
              ACE_UTF16_T s = 0;
              return this->write_2 (cdr, &s);
            }
        }
    }

  return 0;
}

ACE_CDR::Boolean
WUCS4_UTF16::write_wchar_array (ACE_OutputCDR & cdr,
                const ACE_CDR::WChar *x,
                ACE_CDR::ULong length)
{
#if 0
  // I do not believe this is correct, because this could yield an array
  // with an incorrect number of elements for the space allotted.
  return this->write_measured_wchar_array (
    cdr,
    x,
    length,
    length + count_potential_surrogates (x, length));
#endif

  int encode_len = 1;
  if (static_cast<ACE_CDR::Short> (this->minor_version(cdr)) == 0)
    { // wchar is not allowed with GIOP 1.0
      errno = EINVAL;
      return 0;
    }
  else if (static_cast<ACE_CDR::Short> (this->minor_version(cdr)) == 1)
    encode_len = 0;

  for (size_t i = 0; i < length; i++)
    if (this->write_wchar_i (cdr,x[i],0,encode_len) == 0)
      return 0;
  return 1;
}

ACE_CDR::Boolean
WUCS4_UTF16::write_measured_wchar_array (ACE_OutputCDR & cdr,
                                         const ACE_CDR::WChar *x,
                                         ACE_CDR::ULong length,
                                         ACE_CDR::ULong transmission_length)
{
  if (length == 0)
    return 1;
  char* buf;
  size_t align = ACE_CDR::SHORT_ALIGN;
  if (cdr.adjust (ACE_UTF16_CODEPOINT_SIZE * transmission_length, align, buf)
      != 0)
    {
      return 0;
    }

  ACE_UTF16_T *sb = reinterpret_cast<ACE_UTF16_T *> (buf);
  size_t sbpos = 0;

  for (size_t i = 0; i < length; i++)
    {
      sbpos += encode_utf16 (& sb[sbpos], x[i]);
    }
#if defined (ACE_ENABLE_SWAP_ON_WRITE)
  // NOTE this will rarely be enabled.  See the comments in ace/OS.h
  if (cdr.do_byte_swap())
    {
      // note can't use swap_2_array because in-place swaps are not safe :-<
      // and we don't want to allocate a new array
      for (size_t i = 0; i < sbpos; i++)
        {
          char * pchar = reinterpret_cast<char *> (&sb[i]);
          // ACE_CDR::swap_2 (pchar, pchar);
          // can't use swap_2 because inplace swaps are not safe
          // and work-arounds like copying to another buffer lose
          // any performance improvement from
          // that fancy asm code, so we might as well just:
          char temp = pchar[0];
          pchar[0] = pchar[1];
          pchar[1] = temp;
          //@@TODO write swap_2(char * inplace_buffer);
        }
    }
#endif /* ACE_ENABLE_SWAP_ON_WRITE */
  return 1;
}
