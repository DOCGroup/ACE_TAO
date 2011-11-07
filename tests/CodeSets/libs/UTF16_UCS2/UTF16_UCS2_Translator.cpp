// -*- C++ -*-

//=============================================================================
/**
 *  @file    UTF16_UCS2_Translator.cpp
 *
 *  $Id$
 *
 *  Defines the arrays required to convert between UCS-2, a 2 byte wide char
 *  codeset, and UCS-16, aka unicode, a 2-byte codeset. As a bit-pattern
 *  these two should be identical, with the exception that UCS-2 cannot
 *  certain characters that UTF16 can, using escape codepoints.
 *
 *  This translator can be dynamically loaded into TAO at run-time using
 *  a service configurator file with the following contents:
 *
 *  dynamic UTF16_UCS2_Factory Service_Object * UTF16_UCS2:_make_UTF16_UCS2_Factory ()
 *
 *  static Resource_Factory
 *    "-ORBWCharCodesetTranslator UTF16_UCS2_Factory"
 *
 *  This code used UTF16_BOM_Translator as a starting point.
 *
 *
 *  @author Iliyan Jeliazkov <jeliazkov_i@ociweb.com> Byron Harris <harrisb@ociweb.com>
 */
//=============================================================================

#include "UTF16_UCS2_Translator.h"
#include "tao/debug.h"
#include "ace/Log_Msg.h"
// ****************************************************************

typedef ACE_CDR::UShort ACE_UTF16_T;
static const size_t ACE_UTF16_CODEPOINT_SIZE = sizeof (ACE_UTF16_T);

/////////////////////////////
// UTF16_UCS2_Translator implementation

UTF16_UCS2_Translator::UTF16_UCS2_Translator (CONV_FRAME::CodeSetId tcs)
  : tcs_ (tcs)
{
  if (TAO_debug_level > 1)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - UTF16_UCS2_Translator: tcs=0x%x\n"),
         this->tcs_));
}

UTF16_UCS2_Translator::~UTF16_UCS2_Translator (void)
{
}

// = Documented in $ACE_ROOT/ace/CDR_Stream.h
ACE_CDR::Boolean
UTF16_UCS2_Translator::read_wchar (ACE_InputCDR &cdr, ACE_CDR::WChar &x)
{
  if (static_cast <ACE_CDR::Short> (this->major_version(cdr)) == 1 &&
      static_cast <ACE_CDR::Short> (this->minor_version(cdr)) == 2)
    {
      ACE_CDR::Octet len;
      if (! this->read_1 (cdr, &len))
        return 0;

      ACE_ASSERT (len == 2); // no BOM present
          ACE_CDR::Short sx;

          if (!this->read_array (cdr,
                                 reinterpret_cast <char *> (&sx),1,1,2))
            return 0;

#if defined (ACE_LITTLE_ENDIAN)
          ACE_CDR::Short ux;
          ACE_CDR::swap_2 (reinterpret_cast <const char*> (&sx),
                           reinterpret_cast <char *> (&ux));
          x = static_cast <ACE_CDR::WChar> (ux);
#else
          x = static_cast <ACE_CDR::WChar> (sx);
#endif // ACE_LITTLE_ENDIAN
          return 1;
    }
  else
    {
      ACE_UTF16_T sx;
      if (this->read_2 (cdr, &sx))
        {
          x = static_cast <ACE_CDR::WChar> (sx);
          return 1;
        }
    }
  return 0;
}

ACE_CDR::Boolean
UTF16_UCS2_Translator::read_wstring (ACE_InputCDR &cdr,
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
      if (static_cast <ACE_CDR::Short> (this->major_version(cdr)) == 1
          && static_cast <ACE_CDR::Short> (this->minor_version(cdr)) == 2)
        {
          len /= ACE_UTF16_CODEPOINT_SIZE;

          //allocating one extra for the null character needed by applications
          ACE_NEW_RETURN (x,
                          ACE_CDR::WChar [len + 1],
                          0);

          x[len] = L'\x00';
          if (this->read_wchar_array_i (cdr, x, len, 1))
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
UTF16_UCS2_Translator::read_wchar_array_i (ACE_InputCDR & cdr,
                                           ACE_CDR::WChar *x,
                                           ACE_CDR::ULong &length,
                                           int)
{
  int must_swap = 0;
  char* buf;
  static const size_t align = ACE_CDR::SHORT_ALIGN;
  if (cdr.adjust (ACE_UTF16_CODEPOINT_SIZE * length, align, buf) == 0)
    {
      ACE_UTF16_T *sb = reinterpret_cast <ACE_UTF16_T *> (buf);
#if defined (ACE_LITTLE_ENDIAN)
          must_swap = 1;
#endif // ACE_LITTLE_ENDIAN

      for (size_t i = 0; i < length; ++i)
#if defined (ACE_DISABLE_SWAP_ON_READ)
        x[i] = static_cast <ACE_CDR::WChar> (sb[i]);
#else
      if (!must_swap)
        {
          x[i] = static_cast <ACE_CDR::WChar> (sb[i]);
        }
      else
        {
          ACE_CDR::UShort sx;
          ACE_CDR::swap_2 (&buf[i*2], reinterpret_cast <char *> (&sx));
          x[i] = static_cast <ACE_CDR::WChar> (sx);
        }
#endif /* ACE_DISABLE_SWAP_ON_READ */

      return 1;
    }
  return 0;
}

ACE_CDR::Boolean
UTF16_UCS2_Translator::read_wchar_array (ACE_InputCDR & cdr,
                                        ACE_CDR::WChar *x,
                                        ACE_CDR::ULong length)
{
  if (length == 0)
    return 1;

  if (static_cast <ACE_CDR::Short> (this->major_version(cdr)) == 1
      && static_cast <ACE_CDR::Short> (this->minor_version(cdr)) == 2)
    {
      for (size_t i = 0; i < length; ++i)
        if (!this->read_wchar(cdr,x[i]))
          return 0;

      return 1;
    }
  else
    return this->read_wchar_array_i (cdr, x, length);
}

ACE_CDR::Boolean
UTF16_UCS2_Translator::write_wchar (ACE_OutputCDR &cdr,
                                   ACE_CDR::WChar x)
{
  return this->write_wchar_i (cdr, x);
}

ACE_CDR::Boolean
UTF16_UCS2_Translator::write_wchar_i (ACE_OutputCDR &cdr, ACE_CDR::WChar x)
{
  if (static_cast <ACE_CDR::Short> (this->major_version(cdr)) == 1
      && static_cast <ACE_CDR::Short> (this->minor_version(cdr)) == 2)
    {
      int len = 1;
      ACE_CDR::UShort buffer[2];

      buffer[0] = static_cast <ACE_CDR::Short> (x);

      unsigned char tcsize = static_cast <unsigned char>
                                              (len * ACE_UTF16_CODEPOINT_SIZE);
      if (this->write_1 (cdr, &tcsize))
        return this->write_array(cdr, &buffer, tcsize, 1, 1);
      else
        return 0;
    }
  else if (static_cast <ACE_CDR::Short> (this->minor_version(cdr)) != 0)
    {
      // GIOP 1.1 simple support
      ACE_UTF16_T sx = static_cast <ACE_UTF16_T> (x);
      return this->write_2 (cdr, &sx);
    }
  else
    { // wchar is not allowed with GIOP 1.0.
      errno = EINVAL;
      return 0;
    }
}

ACE_CDR::Boolean
UTF16_UCS2_Translator::write_wstring (ACE_OutputCDR & cdr,
                                     ACE_CDR::ULong len,
                                     const ACE_CDR::WChar *x)
{
  // we'll accept a null pointer but only for an empty string
  ACE_ASSERT (x != 0 || len == 0);
  if (static_cast <ACE_CDR::Short> (this->major_version(cdr)) == 1
      && static_cast <ACE_CDR::Short> (this->minor_version(cdr)) == 2)
    {
      if (len == 0) // for zero length strings, only write a length of
                    // zero.
        return this->write_4(cdr, &len);

      ACE_CDR::ULong l = len * ACE_UTF16_CODEPOINT_SIZE;
      if (this->write_4 (cdr, &l) && x != 0)
        {
          if (cdr.do_byte_swap())
            {
              return this->write_swapped_wchar_array_i (cdr, x, len);
            }
          else
            {
              return this->write_wchar_array_i (cdr, x, len);
            }
        }
    }
  else
    {
      // pre GIOP 1.2:  include null terminator in length
      ACE_CDR::ULong l = len + 1;

      if (this->write_4 (cdr, &l))
        {
          if (x != 0)
            {
              return this->write_wchar_array_i (cdr, x, len + 1);
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
UTF16_UCS2_Translator::write_wchar_array (ACE_OutputCDR & cdr,
                                         const ACE_CDR::WChar *x,
                                         ACE_CDR::ULong length)
{
  if (static_cast <ACE_CDR::Short> (this->major_version(cdr)) == 1
      && static_cast <ACE_CDR::Short> (this->minor_version(cdr)) == 2)
    {
      for (size_t i = 0; i < length; ++i)
        if (this->write_wchar_i (cdr, x[i]) == 0)
          return 0;

      return 1;
    }

  return this->write_wchar_array_i (cdr, x, length);
}

ACE_CDR::Boolean
UTF16_UCS2_Translator::write_wchar_array_i (ACE_OutputCDR & cdr,
                                           const ACE_CDR::WChar *x,
                                           ACE_CDR::ULong length)
{
  if (length == 0)
    return 1;
  char* buf;
  static const size_t align = ACE_CDR::SHORT_ALIGN;
  if (cdr.adjust (ACE_UTF16_CODEPOINT_SIZE * length, align, buf)
      != 0)
    {
      return 0;
    }

  ACE_UTF16_T *sb = reinterpret_cast <ACE_UTF16_T *> (buf);

  for (size_t i = 0; i < length; ++i)
    {
      sb[i] = static_cast <ACE_UTF16_T> (x[i]);
    }
  return 1;
}

ACE_CDR::Boolean
UTF16_UCS2_Translator::write_swapped_wchar_array_i (ACE_OutputCDR & cdr,
                                           const ACE_CDR::WChar *x,
                                           ACE_CDR::ULong length)
{
  if (length == 0)
    return 1;
  char* buf;
  static const size_t align = ACE_CDR::SHORT_ALIGN;
  if (cdr.adjust (ACE_UTF16_CODEPOINT_SIZE * length, align, buf)
      != 0)
    {
      return 0;
    }

  ACE_UTF16_T *sb = reinterpret_cast <ACE_UTF16_T *> (buf);

  for (size_t i = 0; i < length; ++i)
    {
      ACE_CDR::swap_2 (reinterpret_cast <const char*> (&x[i]),
                       reinterpret_cast <char *> (&sb[i]));
    }
  return 1;
}
