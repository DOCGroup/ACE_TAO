// -*- C++ -*-
// $Id$

// ============================================================================
//    Manages the transformation between native and transmitted UTF-16. It is
//    Required because transmitted UTF-16 may carry a byte order marker (BOM)
//    that is not part of the data contents. If no BOM is present, then the
//    serialized UTF-16 data is big-endian, regardless of the byte order of
//    the containing encapsulation.
//
// AUTHOR
//    Phil Mesnier <mesnier_p@ociweb.com>
//
// ============================================================================

#include "tao/Codeset/UTF16_BOM_Translator.h"
#include "ace/OS_Memory.h"
#include "tao/debug.h"
#include "ace/Log_Msg.h"

// ****************************************************************

typedef ACE_CDR::UShort ACE_UTF16_T;
static const size_t ACE_UTF16_CODEPOINT_SIZE = sizeof (ACE_UTF16_T);
static const unsigned short ACE_UNICODE_BOM_CORRECT = 0xFEFFU;
static const unsigned short ACE_UNICODE_BOM_SWAPPED = 0xFFFEU;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/////////////////////////////
// TAO_UTF16_BOM_Translator implementation

TAO_UTF16_BOM_Translator::TAO_UTF16_BOM_Translator (bool forceBE)
  : forceBE_(forceBE)
{
  if (TAO_debug_level > 1)
    ACE_DEBUG((LM_DEBUG,
               ACE_TEXT ("TAO (%P|%t) - UTF16_BOM_Translator: ")
               ACE_TEXT("forceBE %d\n"), this->forceBE_ ? 1:0 ));
}

TAO_UTF16_BOM_Translator::~TAO_UTF16_BOM_Translator (void)
{
}

// = Documented in $ACE_ROOT/ace/CDR_Stream.h
ACE_CDR::Boolean
TAO_UTF16_BOM_Translator::read_wchar (ACE_InputCDR &cdr, ACE_CDR::WChar &x)
{
  if (static_cast<ACE_CDR::Short> (this->major_version (cdr)) == 1 &&
      static_cast<ACE_CDR::Short> (this->minor_version (cdr)) == 2)
    {
      ACE_CDR::Octet len;
      if (! this->read_1 (cdr, &len))
        return 0;

      if (len == 2) // no BOM present
        {
          ACE_CDR::Short sx;

          if (!this->read_array (cdr,
                                 reinterpret_cast<char *> (&sx), 1,1,2))
            return 0;

#if defined (ACE_LITTLE_ENDIAN)
          ACE_CDR::Short ux;
          ACE_CDR::swap_2 (reinterpret_cast<const char*> (&sx),
                           reinterpret_cast<char *> (&ux));
          x = static_cast<ACE_CDR::WChar> (ux);
#else
          x = static_cast<ACE_CDR::WChar> (sx);
#endif // ACE_LITTLE_ENDIAN
          return 1;
        }

      ACE_UTF16_T buf[2];
      if (len != 4 || !this->read_array (cdr,
                                         reinterpret_cast<char *> (buf),
                                         1,1,4)) // get BO & payload
        return 0;
      // Check for byte order mark, if found, consume and honor it.
      if (buf[0] == ACE_UNICODE_BOM_CORRECT ||
          buf[0] == ACE_UNICODE_BOM_SWAPPED)
        {
          // if we found it, but it came in in the wrong order
          // invert the byte order flag for the duration of this method
          if (buf[0] == ACE_UNICODE_BOM_SWAPPED)
            {
              ACE_CDR::Short ux;
              ACE_CDR::swap_2 (reinterpret_cast<const char*> (&buf[1]),
                               reinterpret_cast<char *> (&ux));
              x = static_cast<ACE_CDR::WChar> (ux);
            }
          else
            x = static_cast<ACE_CDR::WChar> (buf[1]);
          return 1;
        }
      // What do we do here? The length is > 2 but the first word
      // is not a BOM. Just return an error I suppose
      return 0;
    }

  ACE_UTF16_T sx;
  if (this->read_2 (cdr, &sx))
    {
      x = static_cast<ACE_CDR::WChar> (sx);
      return 1;
    }
  return 0;
}

ACE_CDR::Boolean
TAO_UTF16_BOM_Translator::read_wstring (ACE_InputCDR &cdr,
                                        ACE_CDR::WChar *&x)
{
  ACE_CDR::ULong len;
  if (!this->read_4 (cdr, &len))
    return 0;

  // A check for the length being too great is done later in the
  // call to read_char_array but we want to have it done before
  // the memory is allocated.
  if (len > 0 && len <= cdr.length ())
    {
      if (static_cast<ACE_CDR::Short> (this->major_version (cdr)) == 1
          && static_cast<ACE_CDR::Short> (this->minor_version (cdr)) > 1)
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
TAO_UTF16_BOM_Translator::read_wchar_array_i (ACE_InputCDR & cdr,
                                              ACE_CDR::WChar *x,
                                              ACE_CDR::ULong &length,
                                              int adjust_len)
{
  int has_bom = 0;
  int must_swap = 0;
  char* buf;
  static const size_t align = ACE_CDR::SHORT_ALIGN;
  if (cdr.adjust (ACE_UTF16_CODEPOINT_SIZE * length, align, buf) == 0)
    {
      // check for byte order mark.  If found, honor it then discard it
      ACE_UTF16_T *sb = reinterpret_cast<ACE_UTF16_T *> (buf);
      if (*sb == ACE_UNICODE_BOM_CORRECT || *sb == ACE_UNICODE_BOM_SWAPPED)
        {
          must_swap = (*sb == ACE_UNICODE_BOM_SWAPPED);
          has_bom = 1;
        }
      else
        {
#if defined (ACE_LITTLE_ENDIAN)
          must_swap = 1;
#endif // ACE_LITTLE_ENDIAN
        }

      if (has_bom)
        {
          buf += ACE_UTF16_CODEPOINT_SIZE;
          ++sb;

          if (adjust_len)
            length -= 1;
        }

      for (size_t i = 0; i < length; ++i)
#if defined (ACE_DISABLE_SWAP_ON_READ)
        x[i] = static_cast<ACE_CDR::WChar> (sb[i]);
#else
      if (!must_swap)
        {
          x[i] = static_cast<ACE_CDR::WChar> (sb[i]);
        }
      else
        {
          ACE_CDR::UShort sx;
          ACE_CDR::swap_2 (&buf[i*2], reinterpret_cast<char *> (&sx));
          x[i] = static_cast<ACE_CDR::WChar> (sx);
        }
#endif /* ACE_DISABLE_SWAP_ON_READ */

      if (has_bom && !adjust_len)
        {
          cdr.adjust (ACE_UTF16_CODEPOINT_SIZE, align, buf);
        }
      return 1;
    }
  return 0;
}

ACE_CDR::Boolean
TAO_UTF16_BOM_Translator::read_wchar_array (ACE_InputCDR & cdr,
                                            ACE_CDR::WChar *x,
                                            ACE_CDR::ULong length)
{
  if (length == 0)
    return 1;

  if (static_cast<ACE_CDR::Short> (this->major_version (cdr)) == 1
      && static_cast<ACE_CDR::Short> (this->minor_version (cdr)) > 1)
    {
      for (size_t i = 0; i < length; ++i)
        if (!this->read_wchar (cdr, x[i]))
          return 0;

      return 1;
    }
  else
    return this->read_wchar_array_i (cdr, x, length);
}

ACE_CDR::Boolean
TAO_UTF16_BOM_Translator::write_wchar (ACE_OutputCDR &cdr,
                                       ACE_CDR::WChar x)
{
  return this->write_wchar_i (cdr, x, true);
}

ACE_CDR::Boolean
TAO_UTF16_BOM_Translator::write_wchar_i (ACE_OutputCDR &cdr,
                                         ACE_CDR::WChar x,
                                         bool allow_BOM)
{
  if (static_cast<ACE_CDR::Short> (this->major_version (cdr)) == 1
      && static_cast<ACE_CDR::Short> (this->minor_version (cdr)) > 1)
    {
      int len = 0;
      ACE_CDR::UShort buffer[2];

      if( allow_BOM && cdr.byte_order())
        {
          len = 2;
#if defined (ACE_LITTLE_ENDIAN)
          if (this->forceBE_)
            {
              // force both the byte order mark and the data to Big Endian order
              buffer[0] = ACE_UNICODE_BOM_SWAPPED;
              ACE_CDR::swap_2 (reinterpret_cast<const char *> (&x),
                               reinterpret_cast<char *> (&buffer[1]));
            }
          else
#endif
            {
              // store both the byte order mark and the data in native order
              buffer[0] = ACE_UNICODE_BOM_CORRECT;
              buffer[1] = static_cast<ACE_CDR::Short> (x);
            }
        }
      else
        {
          // not using a byte order mark
          // force it to be big endian w/o BOM
          len = 1;
          if (cdr.byte_order ())
            ACE_CDR::swap_2 (reinterpret_cast<const char *> (&x),
                             reinterpret_cast<char *> (buffer));
          else
            buffer[0] = static_cast<ACE_CDR::Short> (x);
        }

      unsigned char tcsize =
        static_cast<unsigned char> (len * ACE_UTF16_CODEPOINT_SIZE);

      if (this->write_1 (cdr, &tcsize))
        return this->write_array(cdr, &buffer, tcsize, 1, 1);
      else
        return 0;
    }
  else if (static_cast<ACE_CDR::Short> (this->minor_version (cdr)) != 0)
    {
      // GIOP 1.1 simple support
      ACE_UTF16_T sx = static_cast<ACE_UTF16_T> (x);
      return this->write_2 (cdr, &sx);
    }
  else
    { // wchar is not allowed with GIOP 1.0.
      errno = EINVAL;
      return 0;
    }
}

ACE_CDR::Boolean
TAO_UTF16_BOM_Translator::write_wstring (ACE_OutputCDR & cdr,
                                         ACE_CDR::ULong len,
                                         const ACE_CDR::WChar *x)
{
  // we'll accept a null pointer but only for an empty string
  ACE_ASSERT (x != 0 || len == 0);
  if (static_cast<ACE_CDR::Short> (this->major_version (cdr)) == 1
      && static_cast<ACE_CDR::Short> (this->minor_version (cdr)) > 1)
    {
      if (len == 0) // for zero length strings, only write a length of
                    // zero. The BOM is not needed in this case.
        return this->write_4(cdr, &len);

      if (this->forceBE_ && cdr.byte_order())
        {
          ACE_CDR::ULong l = (len+1) *
                             static_cast<ACE_CDR::ULong> (
                                         ACE_UTF16_CODEPOINT_SIZE);
          if (this->write_4 (cdr, &l) &&
              this->write_2 (cdr, &ACE_UNICODE_BOM_SWAPPED) &&
              x != 0)
            return this->write_swapped_wchar_array_i (cdr, x, len);
        }
      else
        {
          ACE_CDR::ULong l = (len+1) *
                             static_cast<ACE_CDR::ULong> (
                                         ACE_UTF16_CODEPOINT_SIZE);
          if (this->write_4 (cdr, &l) &&
              this->write_2 (cdr, &ACE_UNICODE_BOM_CORRECT) &&
              x != 0)
            return this->write_wchar_array_i (cdr, x, len);
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
              return this->write_2 (cdr,&s);
            }
        }
    }

  return 0;
}

ACE_CDR::Boolean
TAO_UTF16_BOM_Translator::write_wchar_array (ACE_OutputCDR & cdr,
                                             const ACE_CDR::WChar *x,
                                             ACE_CDR::ULong length)
{
  if (static_cast<ACE_CDR::Short> (this->major_version (cdr)) == 1
      && static_cast<ACE_CDR::Short> (this->minor_version (cdr)) > 1)
    {
      for (size_t i = 0; i < length; ++i)
        if (this->write_wchar_i (cdr, x[i], false) == 0)
          return 0;

      return 1;
    }

  return this->write_wchar_array_i (cdr, x, length);
}

ACE_CDR::Boolean
TAO_UTF16_BOM_Translator::write_wchar_array_i (ACE_OutputCDR & cdr,
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

  ACE_UTF16_T *sb = reinterpret_cast<ACE_UTF16_T *> (buf);

  for (size_t i = 0; i < length; ++i)
    {
      sb[i] = static_cast<ACE_UTF16_T> (x[i]);
    }
  return 1;

}

ACE_CDR::Boolean
TAO_UTF16_BOM_Translator::write_swapped_wchar_array_i (ACE_OutputCDR & cdr,
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

  ACE_UTF16_T *sb = reinterpret_cast<ACE_UTF16_T *> (buf);

  for (size_t i = 0; i < length; ++i)
    {
      ACE_CDR::swap_2 (reinterpret_cast<const char*> (&x[i]),
                       reinterpret_cast<char *> (&sb[i]));
    }
  return 1;
}

TAO_END_VERSIONED_NAMESPACE_DECL
