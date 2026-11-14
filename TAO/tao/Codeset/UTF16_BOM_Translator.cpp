// -*- C++ -*-
/// Manages the transformation between native and transmitted UTF-16. It is
/// required because transmitted UTF-16 may carry a byte order marker (BOM)
/// that is not part of the data contents. If no BOM is present, then the
/// serialized UTF-16 data is big-endian, regardless of the byte order of
/// the containing encapsulation.
///
/// @author Phil Mesnier <mesnier_p@ociweb.com>

#include "tao/Codeset/UTF16_BOM_Translator.h"
#include "ace/OS_Memory.h"
#include "tao/debug.h"
#include "ace/Log_Msg.h"

#include <memory>

// ****************************************************************

using ACE_UTF16_T = ACE_CDR::UShort;
static constexpr size_t ACE_UTF16_CODEPOINT_SIZE = sizeof (ACE_UTF16_T);
static constexpr ACE_CDR::ULong ACE_UL_UTF16_CODEPOINT_SIZE = static_cast<ACE_CDR::ULong>(ACE_UTF16_CODEPOINT_SIZE);
static constexpr ACE_CDR::UShort ACE_UNICODE_BOM_CORRECT = 0xFEFFU;
static constexpr ACE_CDR::UShort ACE_UNICODE_BOM_SWAPPED = 0xFFFEU;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/////////////////////////////
// TAO_UTF16_BOM_Translator implementation

TAO_UTF16_BOM_Translator::TAO_UTF16_BOM_Translator (bool forceBE)
  : forceBE_ (forceBE)
{
  if (TAO_debug_level > 1)
    TAOLIB_DEBUG ((LM_DEBUG,
                   ACE_TEXT ("TAO (%P|%t) - UTF16_BOM_Translator: ")
                   ACE_TEXT ("forceBE %d\n"), this->forceBE_ ? 1 : 0));
}

ACE_CDR::Boolean
TAO_UTF16_BOM_Translator::read_wchar (ACE_InputCDR &cdr, ACE_CDR::WChar &x)
{
  if (static_cast<ACE_CDR::Short> (this->major_version (cdr)) == 1 &&
      static_cast<ACE_CDR::Short> (this->minor_version (cdr)) == 2)
    {
      ACE_CDR::Octet len;
      if (!this->read_1 (cdr, &len))
        return false;

      if (len == 2) // no BOM present
        {
          ACE_CDR::Short sx;

          if (!this->read_array (cdr,
                                 reinterpret_cast<char *> (&sx), 1, 1, 2))
            return false;

#if defined (ACE_LITTLE_ENDIAN)
          ACE_CDR::Short ux;
          ACE_CDR::swap_2 (reinterpret_cast<char const *> (&sx),
                           reinterpret_cast<char *> (&ux));
          x = static_cast<ACE_CDR::WChar> (ux);
#else
          x = static_cast<ACE_CDR::WChar> (sx);
#endif // ACE_LITTLE_ENDIAN
          return true;
        }

      ACE_UTF16_T buf[2];
      if (len != 4 || !this->read_array (cdr,
                                         reinterpret_cast<char *> (buf),
                                         1, 1, 4)) // get BO & payload
        return false;
      // Check for byte order mark, if found, consume and honor it.
      if (buf[0] == ACE_UNICODE_BOM_CORRECT ||
          buf[0] == ACE_UNICODE_BOM_SWAPPED)
        {
          // if we found it, but it came in in the wrong order
          // invert the byte order flag for the duration of this method
          if (buf[0] == ACE_UNICODE_BOM_SWAPPED)
            {
              ACE_CDR::Short ux;
              ACE_CDR::swap_2 (reinterpret_cast<char const *> (&buf[1]),
                               reinterpret_cast<char *> (&ux));
              x = static_cast<ACE_CDR::WChar> (ux);
            }
          else
            x = static_cast<ACE_CDR::WChar> (buf[1]);
          return true;
        }
      // What do we do here? The length is > 2 but the first word
      // is not a BOM. Just return an error I suppose
      return false;
    }

  ACE_UTF16_T sx;
  if (this->read_2 (cdr, &sx))
    {
      x = static_cast<ACE_CDR::WChar> (sx);
      return true;
    }
  return false;
}

ACE_CDR::Boolean
TAO_UTF16_BOM_Translator::read_wstring (ACE_InputCDR &cdr,
                                        ACE_CDR::WChar *&x)
{
  ACE_CDR::ULong len;
  if (!this->read_4 (cdr, &len))
    return false;

  // A check for the length being too great is done later in the
  // call to read_char_array but we want to have it done before
  // the memory is allocated.
  if (len > 0 && len <= cdr.length ())
    {
      x = nullptr;
      ACE_CDR::WChar *buffer {};
      if (static_cast<ACE_CDR::Short> (this->major_version (cdr)) == 1
          && static_cast<ACE_CDR::Short> (this->minor_version (cdr)) > 1)
        {
          len /= ACE_UTF16_CODEPOINT_SIZE;

          //allocating one extra for the null character needed by applications
          ACE_NEW_RETURN (buffer,
                          ACE_CDR::WChar [len + 1],
                          false);
          std::unique_ptr<ACE_CDR::WChar[]> buffer_guard (buffer);

          buffer_guard[len] = L'\x00';
          if (this->read_wchar_array_i (cdr, buffer_guard.get (), len, true))
            {
              // Since reading the array may have adjusted the length,
              // we simply rewrite the null terminator
              buffer_guard[len] = L'\x00';
              x = buffer_guard.release ();
              return true;
            }
        }
      else
        {
          ACE_NEW_RETURN (buffer,
                          ACE_CDR::WChar [len],
                          false);
          std::unique_ptr<ACE_CDR::WChar[]> buffer_guard (buffer);
          if (this->read_wchar_array (cdr, buffer_guard.get (), len))
            {
              x = buffer_guard.release ();
              return true;
            }
        }
    }
  else if (len == 0)
    {
      // Convert any null strings to empty strings since empty
      // strings can cause crashes. (See bug 58.)
      x = nullptr;
      ACE_CDR::WChar *buffer {};
      ACE_NEW_RETURN (buffer,
                      ACE_CDR::WChar[1],
                      false);
      std::unique_ptr<ACE_CDR::WChar[]> buffer_guard (buffer);
      buffer_guard[0] = '\x00';
      x = buffer_guard.release ();
      return true;
    }
  x = nullptr;
  return false;
}

#if !defined(ACE_LACKS_STD_WSTRING)
ACE_CDR::Boolean
TAO_UTF16_BOM_Translator::read_wstring (ACE_InputCDR &cdr,
                                        std::wstring &x)
{
  ACE_CDR::ULong len;
  if (!this->read_4 (cdr, &len))
    return false;

  // A check for the length being too great is done later in the
  // call to read_char_array but we want to have it done before
  // the memory is allocated.
  if (len > 0 && len <= cdr.length ())
    {
      if (static_cast<ACE_CDR::Short> (this->major_version (cdr)) == 1
          && static_cast<ACE_CDR::Short> (this->minor_version (cdr)) > 1)
        {
          len /= ACE_UTF16_CODEPOINT_SIZE;

          try
            {
              x.resize (len);
            }
          catch (std::bad_alloc const &)
            {
              return false;
            }

          if (this->read_wchar_array_i (cdr, x.data (), len, true))
            {
              // Since reading the array may have adjusted the length,
              // shrink to fit
              x.resize (len);
              return true;
            }
        }
      else
        {
          try
            {
              x.resize (len);
            }
          catch (std::bad_alloc const &)
            {
              return false;
            }

          if (this->read_wchar_array (cdr, x.data (), len))
            {
              x.resize (len-1); // drop terminating zero wchar read from stream
              return true;
            }
        }
    }
  else if (len == 0)
    {
      x.clear ();
      return true;
    }
  x.clear ();
  return false;
}
#endif

ACE_CDR::Boolean
TAO_UTF16_BOM_Translator::read_wchar_array_i (ACE_InputCDR & cdr,
                                              ACE_CDR::WChar *x,
                                              ACE_CDR::ULong &length,
                                              bool adjust_len)
{
  bool has_bom {};
  bool must_swap {};
  char* buf;
  static constexpr size_t align = ACE_CDR::SHORT_ALIGN;
  if (cdr.adjust (ACE_UTF16_CODEPOINT_SIZE * length, align, buf) == 0)
    {
      // check for byte order mark.  If found, honor it then discard it
      ACE_UTF16_T *sb = reinterpret_cast<ACE_UTF16_T *> (buf);
      if (*sb == ACE_UNICODE_BOM_CORRECT || *sb == ACE_UNICODE_BOM_SWAPPED)
        {
          must_swap = (*sb == ACE_UNICODE_BOM_SWAPPED);
          has_bom = true;
        }
      else
        {
#if defined (ACE_LITTLE_ENDIAN)
          must_swap = true;
#endif // ACE_LITTLE_ENDIAN
        }

      if (has_bom)
        {
          buf += ACE_UTF16_CODEPOINT_SIZE;
          ++sb;

          if (adjust_len)
            length -= 1;
        }

      for (size_t i {}; i < length; ++i)
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
      return true;
    }
  return false;
}

ACE_CDR::Boolean
TAO_UTF16_BOM_Translator::read_wchar_array (ACE_InputCDR & cdr,
                                            ACE_CDR::WChar *x,
                                            ACE_CDR::ULong length)
{
  if (length == 0)
    return true;

  if (static_cast<ACE_CDR::Short> (this->major_version (cdr)) == 1
      && static_cast<ACE_CDR::Short> (this->minor_version (cdr)) > 1)
    {
      for (size_t i {}; i < length; ++i)
        if (!this->read_wchar (cdr, x[i]))
          return false;

      return true;
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
      ACE_CDR::Octet len {};
      ACE_CDR::UShort buffer[2];

      if (allow_BOM && cdr.byte_order ())
        {
          len = 2;
#if defined (ACE_LITTLE_ENDIAN)
          if (this->forceBE_)
            {
              // force both the byte order mark and the data to Big Endian order
              buffer[0] = ACE_UNICODE_BOM_SWAPPED;
              ACE_CDR::swap_2 (reinterpret_cast<char const *> (&x),
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
            ACE_CDR::swap_2 (reinterpret_cast<char const *> (&x),
                             reinterpret_cast<char *> (buffer));
          else
            buffer[0] = static_cast<ACE_CDR::Short> (x);
        }

      ACE_CDR::Octet tcsize =
        static_cast<ACE_CDR::Octet> (len * ACE_UTF16_CODEPOINT_SIZE);

      if (this->write_1 (cdr, &tcsize))
        return this->write_array (cdr, &buffer, tcsize, 1, 1);
      else
        return false;
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
      return false;
    }
}

ACE_CDR::Boolean
TAO_UTF16_BOM_Translator::write_wstring (ACE_OutputCDR & cdr,
                                         ACE_CDR::ULong len,
                                         ACE_CDR::WChar const *x)
{
  // we'll accept a null pointer but only for an empty string
  ACE_ASSERT ((x != nullptr || len == 0) &&
              len < (ACE_UINT32_MAX - 1) / ACE_UL_UTF16_CODEPOINT_SIZE);
  if (static_cast<ACE_CDR::Short> (this->major_version (cdr)) == 1
      && static_cast<ACE_CDR::Short> (this->minor_version (cdr)) > 1)
    {
      if (len == 0) // for zero length strings, only write a length of
                    // zero. The BOM is not needed in this case.
        return this->write_4 (cdr, &len);

      if (this->forceBE_ && cdr.byte_order ())
        {
          ACE_CDR::ULong l = (len+1) * ACE_UL_UTF16_CODEPOINT_SIZE;
          if (this->write_4 (cdr, &l) &&
              this->write_2 (cdr, &ACE_UNICODE_BOM_SWAPPED) &&
              x != nullptr)
            return this->write_swapped_wchar_array_i (cdr, x, len);
        }
      else
        {
          ACE_CDR::ULong l = (len+1) * ACE_UL_UTF16_CODEPOINT_SIZE;
          if (this->write_4 (cdr, &l) &&
              this->write_2 (cdr, &ACE_UNICODE_BOM_CORRECT) &&
              x != nullptr)
            return this->write_wchar_array_i (cdr, x, len);
        }
    }
  else
    {
      // pre GIOP 1.2:  include null terminator in length
      ACE_CDR::ULong l = len + 1;

      if (this->write_4 (cdr, &l))
        {
          if (x != nullptr)
            {
              return this->write_wchar_array_i (cdr, x, len + 1);
            }
          else
            {
              ACE_UTF16_T s {};
              return this->write_2 (cdr, &s);
            }
        }
    }

  return false;
}

ACE_CDR::Boolean
TAO_UTF16_BOM_Translator::write_wchar_array (ACE_OutputCDR & cdr,
                                             ACE_CDR::WChar const *x,
                                             ACE_CDR::ULong length)
{
  if (static_cast<ACE_CDR::Short> (this->major_version (cdr)) == 1
      && static_cast<ACE_CDR::Short> (this->minor_version (cdr)) > 1)
    {
      for (size_t i {}; i < length; ++i)
        if (!this->write_wchar_i (cdr, x[i], false))
          return false;

      return true;
    }

  return this->write_wchar_array_i (cdr, x, length);
}

ACE_CDR::Boolean
TAO_UTF16_BOM_Translator::write_wchar_array_i (ACE_OutputCDR & cdr,
                                               ACE_CDR::WChar const *x,
                                               ACE_CDR::ULong length)
{
  if (length == 0)
    return true;
  char* buf;
  static constexpr size_t align = ACE_CDR::SHORT_ALIGN;
  if (cdr.adjust (ACE_UTF16_CODEPOINT_SIZE * length, align, buf)
      != 0)
    {
      return false;
    }

  ACE_UTF16_T *sb = reinterpret_cast<ACE_UTF16_T *> (buf);

  for (size_t i {}; i < length; ++i)
    {
      sb[i] = static_cast<ACE_UTF16_T> (x[i]);
    }
  return true;
}

ACE_CDR::Boolean
TAO_UTF16_BOM_Translator::write_swapped_wchar_array_i (ACE_OutputCDR & cdr,
                                                       ACE_CDR::WChar const *x,
                                                       ACE_CDR::ULong length)
{
  if (length == 0)
    return true;
  char* buf;
  static constexpr size_t align = ACE_CDR::SHORT_ALIGN;
  if (cdr.adjust (ACE_UTF16_CODEPOINT_SIZE * length, align, buf)
      != 0)
    {
      return false;
    }

  ACE_UTF16_T *sb = reinterpret_cast<ACE_UTF16_T *> (buf);

  for (size_t i {}; i < length; ++i)
    {
      ACE_CDR::swap_2 (reinterpret_cast<char const *> (&x[i]),
                       reinterpret_cast<char *> (&sb[i]));
    }
  return true;
}

TAO_END_VERSIONED_NAMESPACE_DECL
