#include "ace/OS_NS_string.h"

#include <cstring>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
ACE_SizeCDR::ACE_SizeCDR (ACE_CDR::Octet major_version,
                          ACE_CDR::Octet minor_version)
    : major_version_ (major_version),
      minor_version_ (minor_version)
{
}

ACE_INLINE bool
ACE_SizeCDR::good_bit () const
{
  return this->good_bit_;
}

ACE_INLINE void
ACE_SizeCDR::reset ()
{
  this->size_ = 0;
}

ACE_INLINE size_t
ACE_SizeCDR::total_length () const
{
  return this->size_;
}

// Encode the CDR stream.

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_octet (ACE_CDR::Octet x)
{
  return this->write_1 (reinterpret_cast<ACE_CDR::Octet const *> (&x));
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_boolean (ACE_CDR::Boolean x)
{
  return this->write_octet (x ? (ACE_CDR::Octet) 1 : (ACE_CDR::Octet) 0);
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_char (ACE_CDR::Char x)
{
  // Note: translator framework is not supported.
  //
  return this->write_1 (reinterpret_cast<ACE_CDR::Octet const *> (&x));
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_short (ACE_CDR::Short x)
{
  return this->write_2 (reinterpret_cast<ACE_CDR::UShort const *> (&x));
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_ushort (ACE_CDR::UShort x)
{
  return this->write_2 (reinterpret_cast<ACE_CDR::UShort const *> (&x));
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_long (ACE_CDR::Long x)
{
  return this->write_4 (reinterpret_cast<ACE_CDR::ULong const *> (&x));
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_ulong (ACE_CDR::ULong x)
{
  return this->write_4 (reinterpret_cast<ACE_CDR::ULong const *> (&x));
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_longlong (ACE_CDR::LongLong const &x)
{
  return this->write_8 (reinterpret_cast<ACE_CDR::ULongLong const *> (&x));
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_ulonglong (ACE_CDR::ULongLong const &x)
{
  void const *temp = &x;
  return this->write_8 (reinterpret_cast<ACE_CDR::ULongLong const *> (temp));
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_float (ACE_CDR::Float x)
{
  void const *temp = &x;
  return this->write_4 (reinterpret_cast<ACE_CDR::ULong const *> (temp));
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_double (ACE_CDR::Double const &x)
{
  void const *temp = &x;
  return this->write_8 (reinterpret_cast<ACE_CDR::ULongLong const *> (temp));
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_longdouble (ACE_CDR::LongDouble const &x)
{
  void const *temp = &x;
  return this->write_16 (reinterpret_cast<ACE_CDR::LongDouble const *> (temp));
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_fixed (ACE_CDR::Fixed const &x)
{
  return this->write_array (&x, ACE_CDR::OCTET_SIZE, ACE_CDR::OCTET_ALIGN,
                            (x.fixed_digits () + 2) / 2);
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_string (ACE_CDR::Char const *x)
{
  if (x != nullptr)
    {
      ACE_CDR::ULong const len =
        static_cast<ACE_CDR::ULong> (std::strlen (x));
      return this->write_string (len, x);
    }
  return this->write_string (0, nullptr);
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_wstring (ACE_CDR::WChar const *x)
{
  if (x != nullptr)
    {
      ACE_CDR::ULong const len =
        static_cast<ACE_CDR::ULong> (ACE_OS::strlen (x));
      return this->write_wstring (len, x);
    }
  return this->write_wstring (0, nullptr);
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_string (std::string const &x)
{
  ACE_CDR::ULong len =
    static_cast<ACE_CDR::ULong> (x.size ());
  return this->write_string (len,
                             x.empty () ? nullptr : x.c_str ());
}

#if !defined (ACE_LACKS_STD_STRING_VIEW)
ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_string_view (std::string_view const &x)
{
  ACE_CDR::ULong len =
    static_cast<ACE_CDR::ULong> (x.size ());
  return this->write_string (len,
                             x.empty () ? nullptr : x.data ());
}
#endif

#if !defined(ACE_LACKS_STD_WSTRING)
ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_wstring (std::wstring const &x)
{
  ACE_CDR::ULong len =
    static_cast<ACE_CDR::ULong> (x.size ());
  return this->write_wstring (len,
                              x.empty () ? nullptr : x.c_str ());
}
#endif

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_char_array (ACE_CDR::Char const *x,
                                 ACE_CDR::ULong length)
{
  // Note: translator framework is not supported.
  //
  return this->write_array (x,
                            ACE_CDR::OCTET_SIZE,
                            ACE_CDR::OCTET_ALIGN,
                            length);
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_wchar_array (ACE_CDR::WChar const * x,
                                  ACE_CDR::ULong length)
{
  // Note: translator framework is not supported.
  //
  if (ACE_OutputCDR::wchar_maxbytes () == 0)
    {
      errno = EACCES;
      return (this->good_bit_ = false);
    }
  if (ACE_OutputCDR::wchar_maxbytes () == sizeof (ACE_CDR::WChar))
    {
      return this->write_array (x,
                                sizeof (ACE_CDR::WChar),
                                sizeof (ACE_CDR::WChar) == 2
                                ? ACE_CDR::SHORT_ALIGN
                                : ACE_CDR::LONG_ALIGN,
                                length);
    }
  return this->write_wchar_array_i (x,length);
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_octet_array (ACE_CDR::Octet const * x,
                                  ACE_CDR::ULong length)
{
  return this->write_array (x,
                            ACE_CDR::OCTET_SIZE,
                            ACE_CDR::OCTET_ALIGN,
                            length);
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_short_array (ACE_CDR::Short const *x,
                                  ACE_CDR::ULong length)
{
  return this->write_array (x,
                            ACE_CDR::SHORT_SIZE,
                            ACE_CDR::SHORT_ALIGN,
                            length);
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_ushort_array (ACE_CDR::UShort const *x,
                                 ACE_CDR::ULong length)
{
  return this->write_array (x,
                            ACE_CDR::SHORT_SIZE,
                            ACE_CDR::SHORT_ALIGN,
                            length);
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_long_array (ACE_CDR::Long const *x,
                               ACE_CDR::ULong length)
{
  return this->write_array (x,
                            ACE_CDR::LONG_SIZE,
                            ACE_CDR::LONG_ALIGN,
                            length);
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_ulong_array (ACE_CDR::ULong const *x,
                                ACE_CDR::ULong length)
{
  return this->write_array (x,
                            ACE_CDR::LONG_SIZE,
                            ACE_CDR::LONG_ALIGN,
                            length);
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_longlong_array (ACE_CDR::LongLong const *x,
                                     ACE_CDR::ULong length)
{
  return this->write_array (x,
                            ACE_CDR::LONGLONG_SIZE,
                            ACE_CDR::LONGLONG_ALIGN,
                            length);
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_ulonglong_array (ACE_CDR::ULongLong const *x,
                                      ACE_CDR::ULong length)
{
  return this->write_array (x,
                            ACE_CDR::LONGLONG_SIZE,
                            ACE_CDR::LONGLONG_ALIGN,
                            length);
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_float_array (ACE_CDR::Float const *x,
                                  ACE_CDR::ULong length)
{
  return this->write_array (x,
                            ACE_CDR::LONG_SIZE,
                            ACE_CDR::LONG_ALIGN,
                            length);
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_double_array (ACE_CDR::Double const *x,
                                   ACE_CDR::ULong length)
{
  return this->write_array (x,
                            ACE_CDR::LONGLONG_SIZE,
                            ACE_CDR::LONGLONG_ALIGN,
                            length);
}

ACE_INLINE ACE_CDR::Boolean
ACE_SizeCDR::write_longdouble_array (ACE_CDR::LongDouble const * x,
                                       ACE_CDR::ULong length)
{
  return this->write_array (x,
                            ACE_CDR::LONGDOUBLE_SIZE,
                            ACE_CDR::LONGDOUBLE_ALIGN,
                            length);
}

// ****************************************************************

ACE_INLINE ACE_CDR::Boolean
operator<< (ACE_SizeCDR &ss, ACE_CDR::Char x)
{
  ss.write_char (x);
  return ss.good_bit ();
}

ACE_INLINE ACE_CDR::Boolean
operator<< (ACE_SizeCDR &ss, ACE_CDR::Short x)
{
  ss.write_short (x);
  return ss.good_bit ();
}

ACE_INLINE ACE_CDR::Boolean
operator<< (ACE_SizeCDR &ss, ACE_CDR::UShort x)
{
  ss.write_ushort (x);
  return ss.good_bit ();
}

ACE_INLINE ACE_CDR::Boolean
operator<< (ACE_SizeCDR &ss, ACE_CDR::Long x)
{
  ss.write_long (x);
  return ss.good_bit ();
}

ACE_INLINE ACE_CDR::Boolean
operator<< (ACE_SizeCDR &ss, ACE_CDR::ULong x)
{
  ss.write_ulong (x);
  return ss.good_bit ();
}

ACE_INLINE ACE_CDR::Boolean
operator<< (ACE_SizeCDR &ss, ACE_CDR::LongLong x)
{
  ss.write_longlong (x);
  return ss.good_bit ();
}

ACE_INLINE ACE_CDR::Boolean
operator<< (ACE_SizeCDR &ss, ACE_CDR::ULongLong x)
{
  ss.write_ulonglong (x);
  return ss.good_bit ();
}

ACE_INLINE ACE_CDR::Boolean
operator<< (ACE_SizeCDR &ss, ACE_CDR::LongDouble x)
{
  ss.write_longdouble (x);
  return ss.good_bit ();
}

ACE_INLINE ACE_CDR::Boolean
operator<< (ACE_SizeCDR &ss, ACE_CDR::Float x)
{
  ss.write_float (x);
  return ss.good_bit ();
}

ACE_INLINE ACE_CDR::Boolean
operator<< (ACE_SizeCDR &ss, ACE_CDR::Double x)
{
  ss.write_double (x);
  return ss.good_bit ();
}

ACE_INLINE ACE_CDR::Boolean
operator<< (ACE_SizeCDR &ss, ACE_CDR::Fixed const &x)
{
  ss.write_fixed (x);
  return ss.good_bit ();
}

ACE_INLINE ACE_CDR::Boolean
operator<< (ACE_SizeCDR &ss, ACE_CDR::Char const *x)
{
  ss.write_string (x);
  return ss.good_bit ();
}

ACE_INLINE ACE_CDR::Boolean
operator<< (ACE_SizeCDR &ss, ACE_CDR::WChar const *x)
{
  ss.write_wstring (x);
  return ss.good_bit ();
}

ACE_INLINE ACE_CDR::Boolean
operator<< (ACE_SizeCDR &ss, std::string const & x)
{
  ss.write_string (x);
  return ss.good_bit ();
}

#if !defined (ACE_LACKS_STD_STRING_VIEW)
ACE_INLINE ACE_CDR::Boolean
operator<< (ACE_SizeCDR &ss, std::string_view const & x)
{
  ss.write_string_view (x);
  return ss.good_bit ();
}
#endif

#if !defined(ACE_LACKS_STD_WSTRING)
ACE_INLINE ACE_CDR::Boolean
operator<< (ACE_SizeCDR &ss, std::wstring const & x)
{
  ss.write_wstring (x);
  return ss.good_bit ();
}
#endif

// The following use the helper classes
ACE_INLINE ACE_CDR::Boolean
operator<< (ACE_SizeCDR &ss, ACE_OutputCDR::from_boolean x)
{
  ss.write_boolean (x.val_);
  return ss.good_bit ();
}

ACE_INLINE ACE_CDR::Boolean
operator<< (ACE_SizeCDR &ss, ACE_OutputCDR::from_char x)
{
  ss.write_char (x.val_);
  return ss.good_bit ();
}

ACE_INLINE ACE_CDR::Boolean
operator<< (ACE_SizeCDR &ss, ACE_OutputCDR::from_wchar x)
{
  ss.write_wchar (x.val_);
  return ss.good_bit ();
}

ACE_INLINE ACE_CDR::Boolean
operator<< (ACE_SizeCDR &ss, ACE_OutputCDR::from_octet x)
{
  ss.write_octet (x.val_);
  return ss.good_bit ();
}

ACE_INLINE ACE_CDR::Boolean
operator<< (ACE_SizeCDR &ss, ACE_OutputCDR::from_string x)
{
  ACE_CDR::ULong len = 0;

  if (x.val_ != nullptr)
    {
      len = static_cast<ACE_CDR::ULong> (std::strlen (x.val_));
    }

  ss.write_string (len, x.val_);
  return (ss.good_bit () && (!x.bound_ || len <= x.bound_));
}

ACE_INLINE ACE_CDR::Boolean
operator<< (ACE_SizeCDR &ss, ACE_OutputCDR::from_wstring x)
{
  ACE_CDR::ULong len = 0;

  if (x.val_ != nullptr)
    {
        len = static_cast<ACE_CDR::ULong> (ACE_OS::strlen (x.val_));
    }

  ss.write_wstring (len, x.val_);
  return (ss.good_bit () && (!x.bound_ || len <= x.bound_));
}

ACE_END_VERSIONED_NAMESPACE_DECL
