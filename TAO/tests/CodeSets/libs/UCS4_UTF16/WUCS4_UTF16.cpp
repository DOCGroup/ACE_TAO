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
//    Defines the actions required to convert between UCS-4, a 4 byte wide char
//    codeset as the native wchar codeset, and UCS-16, aka unicode, a 2-byte
//    codeset for the transport wchar codeset.
//
//    This translator does not alter any codepoint values, but it does
//    illustrate how to deal with the unicode byte order marker (BOM) and how
//    to deal with codepoint size differences.
//
//    Note that the BOM is an optional value. The write_*() methods do not
//    encode this value. When BOM is not encoded, the byte order of the
//    encapsulation is used.
//
// = AUTHOR
//    Phil Mesnier <mesnier_p@ociweb.com>
//
// ============================================================================

#include "WUCS4_UTF16.h"
#include "ace/OS_Memory.h"

ACE_RCSID(UCS4_UTF16, WUCS4_UTF16, "$Id$")

// ****************************************************************

WUCS4_UTF16::WUCS4_UTF16 (void)
  : tcs_size_ (2)
{

}

WUCS4_UTF16::~WUCS4_UTF16 (void)
{

}

// = Documented in $ACE_ROOT/ace/CDR_Stream.h
ACE_CDR::Boolean
WUCS4_UTF16::read_wchar (ACE_InputCDR &cdr, ACE_CDR::WChar &x)
{
  ACE_CDR::UShort sx = 0;
  if (ACE_static_cast (ACE_CDR::Short, this->major_version(cdr)) == 1
      && ACE_static_cast (ACE_CDR::Short, this->minor_version(cdr)) == 2)
    {
      ACE_CDR::Octet len;
      if (!this->read_1 (cdr, &len))
        return 0;
      if (! this->read_2 (cdr,&sx))
        return 0;
      // We need to consume the optional byte-order-marker
      if (sx == 0xFEFE || sx == 0xFFFE)
        {
          int old_bo = cdr.byte_order();
          cdr.reset_byte_order (sx == 0xFFFE);
          this->read_2 (cdr,&sx);
          cdr.reset_byte_order (old_bo);
          return 1;
        }
    }
  else
    {
      if (!this->read_2 (cdr, &sx))
        return 0;
    }
  x = ACE_static_cast(ACE_CDR::WChar, sx);
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
      if (ACE_static_cast (ACE_CDR::Short, this->major_version(cdr)) == 1
          && ACE_static_cast (ACE_CDR::Short, this->minor_version(cdr)) == 2)
        {
          len /= this->tcs_size_;

          //allocating one extra for the null character needed by applications
          ACE_NEW_RETURN (x,
                          ACE_CDR::WChar [len + 1],
                          0);
          if (this->read_wchar_array (cdr, x, len))
            {
              //Null character used by applications to find the end of
              //the wstring
              //Is this okay with the GIOP 1.2 spec??
              x[len] = '\x00';
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
WUCS4_UTF16::read_wchar_array (ACE_InputCDR & cdr,
                               ACE_CDR::WChar *x,
                               ACE_CDR::ULong length)
{
  if (length == 0)
    return 1;
  char* buf;
  size_t align = ACE_CDR::SHORT_ALIGN;
  if (cdr.adjust (this->tcs_size_ * length, align, buf) == 0)
    {
      ACE_CDR::UShort *sb = ACE_reinterpret_cast(ACE_CDR::UShort *, buf);
      int old_bo = cdr.byte_order();
      if (*sb == 0xFEFE || *sb == 0xFFFE)
        {
          cdr.reset_byte_order (*sb == 0xFFFE);
          sb ++;
          length --;
        }
      for (size_t i = 0; i < length; i++)
#if defined (ACE_DISABLE_SWAP_ON_READ)
        x[i] = ACE_static_cast (ACE_CDR::WChar, sb[i]);
#else
      if (!cdr.do_byte_swap ())
        x[i] = ACE_static_cast (ACE_CDR::WChar, sb[i]);
      else
        {
          ACE_CDR::UShort sx;
          ACE_CDR::swap_2 (&buf[i*2], ACE_reinterpret_cast(char *,&sx));
          x[i] = ACE_static_cast (ACE_CDR::WChar,sx);
        }
#endif /* ACE_DISABLE_SWAP_ON_READ */
      cdr.reset_byte_order (old_bo);

      return 1;
    }
  return 0;
}

ACE_CDR::Boolean
WUCS4_UTF16::write_wchar (ACE_OutputCDR &cdr,
                          ACE_CDR::WChar x)
{
  ACE_CDR::UShort sx = ACE_static_cast(ACE_CDR::Short,x);
  if (ACE_static_cast (ACE_CDR::Short, this->major_version(cdr)) == 1
      && ACE_static_cast (ACE_CDR::Short, this->minor_version(cdr)) == 2)
    {
      if (this->write_1 (cdr, &this->tcs_size_))
        return this->write_array(cdr, &sx, this->tcs_size_, ACE_CDR::SHORT_ALIGN, 1);
      else
        return 0;
    }
  else if (ACE_static_cast (ACE_CDR::Short, this->minor_version(cdr)) == 0)
    { // wchar is not allowed with GIOP 1.0.
      errno = EINVAL;
      return 0;
    }
  return this->write_2 (cdr, &sx);
}

ACE_CDR::Boolean
WUCS4_UTF16::write_wstring (ACE_OutputCDR & cdr,
                            ACE_CDR::ULong len,
                            const ACE_CDR::WChar *x)
{
  ACE_CDR::ULong l = 0;
  if (ACE_static_cast (ACE_CDR::Short, this->major_version(cdr)) == 1
      && ACE_static_cast (ACE_CDR::Short, this->minor_version(cdr)) == 2)
    {
      l = len * this->tcs_size_;
      if (this->write_4 (cdr, &l) && x != 0)
        return this->write_wchar_array (cdr, x, len);
    }
  else
    {
      l = len + 1;
      if (this->write_4 (cdr, &l))
        if (x != 0)
          return this->write_wchar_array (cdr, x, len + 1);
        else
          {
            ACE_CDR::UShort s = 0;
            return this->write_2 (cdr,&s);
          }
    }
  return 0;
}

ACE_CDR::Boolean
WUCS4_UTF16::write_wchar_array (ACE_OutputCDR & cdr,
                                const ACE_CDR::WChar *x,
                                ACE_CDR::ULong length)
{
  if (length == 0)
    return 1;
  char* buf;
  size_t align = ACE_CDR::SHORT_ALIGN;
  if (cdr.adjust (this->tcs_size_ * length, align, buf) == 0)
    {
      ACE_CDR::UShort *sb = ACE_reinterpret_cast(ACE_CDR::UShort *, buf);
      for (size_t i = 0; i < length; i++)
#if !defined (ACE_ENABLE_SWAP_ON_WRITE)
        sb[i] = ACE_static_cast (ACE_CDR::UShort, x[i]);
#else
      if (!cdr.do_byte_swap())
        sb[i] = ACE_static_cast (ACE_CDR::UShort, x[i]);;
      else
        {
          ACE_CDR::UShort sx = ACE_static_cast (ACE_CDR::UShort, x[i]);
          ACE_CDR::swap_2 (ACE_reinterpret_cast(char *,&sx),&buf[i*2]);
        }
#endif /* ACE_DISABLE_SWAP_ON_READ */
      return 1;
    }
  return 0;
}
