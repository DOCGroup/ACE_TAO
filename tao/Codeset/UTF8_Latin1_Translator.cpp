// -*- C++ -*-

//=============================================================================
/**
 *  @file    UTF8_Latin1_Translator.cpp
 *
 *  $Id$
 *
 *  Defines the methods required to convert UTF-8 based unicode strings
 *  to the Latin-1 codeset.
 *
 *
 *  @author Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================

#include "tao/Codeset/UTF8_Latin1_Translator.h"
#include "tao/debug.h"
#include "ace/OS_Memory.h"

// ****************************************************************

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/////////////////////////////
// UTF8_Latin1_Translator implementation

TAO_UTF8_Latin1_Translator::TAO_UTF8_Latin1_Translator ()
{
}

TAO_UTF8_Latin1_Translator::~TAO_UTF8_Latin1_Translator (void)
{
}

// = Documented in $ACE_ROOT/ace/CDR_Stream.h
ACE_CDR::Boolean
TAO_UTF8_Latin1_Translator::read_char (ACE_InputCDR &cdr, ACE_CDR::Char &x)
{
  // We cannot have a codepoint > 0xBF at this point, since we are expecting
  // only one single char.
  ACE_CDR::Octet ox;
  if (this->read_1 (cdr, &ox))
    {
      if (ox < 0xC0)
        {
          x = ox;
          return 1;
        }
    }
  return 0;
}

ACE_CDR::ULong
TAO_UTF8_Latin1_Translator::read_char_i (ACE_InputCDR &cdr, ACE_CDR::Char &x)
{
  // This will read up to 2 octets and combine them into one char if possible
  ACE_CDR::Octet upper;
  if (this->read_1 (cdr, &upper))
    {
      if ( upper >= 0xC4) // Anything with a leading char > 110001xx converts
                          // to a codepoint value > 0x00FF, thus won't fit in
                          // a single char.
        return 0;
      if ( upper < 0xC0 )
        {
          x = static_cast<ACE_CDR::Char>(upper);
          return 1;
        }
      ACE_CDR::Octet lower;
      if (this->read_1 (cdr, &lower))
        {
          ACE_CDR::Octet final = ((upper & 0xBF) << 6) + (lower & 0xC0);
          x = static_cast<ACE_CDR::Char>(final);
          return 2;
        };
    }
  return 0;
}

ACE_CDR::Boolean
TAO_UTF8_Latin1_Translator::read_string (ACE_InputCDR &cdr,
                                    ACE_CDR::Char *&x)
{
  ACE_CDR::ULong len;
  if (!cdr.read_ulong (len))
    return 0;

  // A check for the length being too great is done later in the
  // call to read_char_array but we want to have it done before
  // the memory is allocated.
  if (len > 0 && len <= cdr.length())
    {
      ACE_NEW_RETURN (x,
                      ACE_CDR::Char [len],
                      0);
      // pos keeps track of the character position, it will never be
      // greater than len
      size_t pos = 0;
      ACE_CDR::ULong incr = 1;
      for (ACE_CDR::ULong i = 0; incr > 0 && i < len; i += incr)
        {
          incr = this->read_char_i(cdr,x[pos++]);
        }
      if (incr > 0)
        return 1;
      delete [] x;
    }
  else if (len == 0)
    {
      // Convert any null strings to empty strings since empty
      // strings can cause crashes. (See bug 58.)
      ACE_NEW_RETURN (x,
                      ACE_CDR::Char[1],
                      0);
      x[0] = '\x00';
      return 1;
    }
  x = 0;
  return 0;
}

ACE_CDR::Boolean
TAO_UTF8_Latin1_Translator::read_char_array (ACE_InputCDR & cdr,
                                        ACE_CDR::Char *x,
                                        ACE_CDR::ULong length)
{
  if (length == 0)
    return 1;

  for (size_t i = 0; i < length; ++i)
    if (!this->read_char(cdr,x[i]))
      return 0;

  return 1;
}

ACE_CDR::Boolean
TAO_UTF8_Latin1_Translator::write_char (ACE_OutputCDR &cdr,
                                   ACE_CDR::Char x)
{
  ACE_CDR::Octet ox = x;
  if (ox < 0xC0)
    return this->write_1 (cdr,&ox);
  else
    { // character cannot be represented in a single octet
      errno = EINVAL;
      return 0;
    }
}

ACE_CDR::Boolean
TAO_UTF8_Latin1_Translator::write_char_i (ACE_OutputCDR &cdr,
                                      ACE_CDR::Char x)
{
  // @@@ Strictly speaking, we should test for 7F < x < C0 and do
  // something else in that case, but for now we will just let it
  // pass.

  ACE_CDR::Octet ox = x;
  if (ox < 0xC0)
    return this->write_1 (cdr,&ox);
  else
    { // character cannot be represented in a single octet
      // Since the source will never be > 0xFF, we don't have to worry about
      // using a third octet.
      ACE_CDR::Octet upper = 0xC0 + (ox >> 6);
      ACE_CDR::Octet lower = 0x80 + (ox & 0x3F);
      if (this->write_1(cdr, &upper))
        return this->write_1(cdr, &lower);
    }
  return 0;
}

ACE_CDR::Boolean
TAO_UTF8_Latin1_Translator::write_string (ACE_OutputCDR & cdr,
                                     ACE_CDR::ULong len,
                                     const ACE_CDR::Char *x)
{
  // we'll accept a null pointer but only for an empty string
  if (x == 0 && len != 0)
    return 0;

  ACE_CDR::ULong l = len;
  // Compute the real buffer size by adding in multi-byte codepoints.
  for (ACE_CDR::ULong i = 0; i < len; i++)
    if (static_cast<ACE_CDR::Octet>(x[i]) > 0xbf) l++;

  // Always add one for the nul
  l++;
  if (cdr.write_ulong (l))
    {
      for (ACE_CDR::ULong i = 0; i < len; ++i)
        {
          if (this->write_char_i (cdr,x[i]) == 0)
            return 0;
        }
      ACE_CDR::Octet s = 0;
      return this->write_1 (cdr, &s);
    }
  return 0;
}

ACE_CDR::Boolean
TAO_UTF8_Latin1_Translator::write_char_array (ACE_OutputCDR & cdr,
                                         const ACE_CDR::Char *x,
                                         ACE_CDR::ULong length)
{
  if (length == 0)
    return 1;

  for (size_t i = 0; i < length; ++i)
    // We still have to write each char individually, as any translated
    // value may fail to fit in a single octet.
    if (this->write_char (cdr, x[i]) == 0)
      return 0;

  return 1;
}

TAO_END_VERSIONED_NAMESPACE_DECL
