// -*- C++ -*-
// $Id$

// ****************************************************************

ACE_INLINE void
CDR::swap_2 (const char *orig, char* target)
{
  target[1] = *orig++;
  target[0] = *orig++;
}

ACE_INLINE void
CDR::swap_4 (const char *orig, char* target)
{
  target [3] = *orig++;
  target [2] = *orig++;
  target [1] = *orig++;
  target [0] = *orig++;
}

ACE_INLINE void
CDR::swap_8 (const char *orig, char* target)
{
  target [7] = *orig++;
  target [6] = *orig++;
  target [5] = *orig++;
  target [4] = *orig++;
  target [3] = *orig++;
  target [2] = *orig++;
  target [1] = *orig++;
  target [0] = *orig++;
}

ACE_INLINE void
CDR::swap_16 (const char *orig, char* target)
{
  target [15] = *orig++;
  target [14] = *orig++;
  target [13] = *orig++;
  target [12] = *orig++;
  target [11] = *orig++;
  target [10] = *orig++;
  target [9] = *orig++;
  target [8] = *orig++;
  target [7] = *orig++;
  target [6] = *orig++;
  target [5] = *orig++;
  target [4] = *orig++;
  target [3] = *orig++;
  target [2] = *orig++;
  target [1] = *orig++;
  target [0] = *orig++;
}

ACE_INLINE void
CDR::mb_align (ACE_Message_Block *mb)
{
  char *start = ptr_align_binary (mb->base (),
                                  CDR::MAX_ALIGNMENT);
  mb->rd_ptr (start);
  mb->wr_ptr (start);

#if defined (ACE_HAS_PURIFY)
  // This is a good place to zero the buffer; this is not needed
  // by IIOP, but makes Purify happy.
  if (mb->base () != 0)
    {
      (void) ACE_OS::memset (mb->base (), 0, mb->size ());
    }
#endif /* ACE_HAS_PURIFY */
}

// ****************************************************************

// implementing the special types
ACE_INLINE
ACE_OutputCDR::from_boolean::from_boolean (CDR::Boolean b)
  : val_ (b)
{
}

ACE_INLINE
ACE_InputCDR::to_boolean::to_boolean (CDR::Boolean &b)
  : ref_ (b)
{
}

ACE_INLINE
ACE_OutputCDR::from_octet::from_octet (CDR::Octet o)
  : val_ (o)
{
}

ACE_INLINE
ACE_InputCDR::to_octet::to_octet (CDR::Octet &o)
  : ref_ (o)
{
}

ACE_INLINE
ACE_OutputCDR::from_char::from_char (CDR::Char c)
  : val_ (c)
{
}

ACE_INLINE
ACE_InputCDR::to_char::to_char (CDR::Char &c)
  : ref_ (c)
{
}

ACE_INLINE
ACE_OutputCDR::from_wchar::from_wchar (CDR::WChar wc)
  : val_ (wc)
{
}

ACE_INLINE
ACE_InputCDR::to_wchar::to_wchar (CDR::WChar &wc)
  : ref_ (wc)
{
}

ACE_INLINE
ACE_OutputCDR::from_string::from_string (CDR::Char *s,
                                         CDR::ULong b,
                                         CDR::Boolean nocopy)
  : val_ (s),
    bound_ (b),
    nocopy_ (nocopy)
{
}

ACE_INLINE
ACE_InputCDR::to_string::to_string (CDR::Char *&s,
                                    CDR::ULong b)
  : val_ (s),
    bound_ (b)
{
}

// ****************************************************************

// Decode the CDR stream.

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_octet (CDR::Octet x)
{
  return this->write_1 (ACE_reinterpret_cast (const CDR::Octet*,&x));
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_boolean (CDR::Boolean x)
{
  return this->write_octet (x ? 1 : 0);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_char (CDR::Char x)
{
  return this->write_1 (ACE_reinterpret_cast (const CDR::Octet*, &x));
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_wchar (CDR::WChar x)
{
  return this->write_2 (ACE_reinterpret_cast (const CDR::UShort*,&x));
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_short (CDR::Short x)
{
  return this->write_2 (ACE_reinterpret_cast (const CDR::UShort*, &x));
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_ushort (CDR::UShort x)
{
  return this->write_2 (ACE_reinterpret_cast (const CDR::UShort*, &x));
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_long (CDR::Long x)
{
  return this->write_4 (ACE_reinterpret_cast (const CDR::ULong*, &x));
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_ulong (CDR::ULong x)
{
  return this->write_4 (ACE_reinterpret_cast (const CDR::ULong*, &x));
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_longlong (const CDR::LongLong &x)
{
  return this->write_8 (ACE_reinterpret_cast (const CDR::ULongLong*, &x));
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_ulonglong (const CDR::ULongLong &x)
{
  return this->write_8 (ACE_reinterpret_cast (const CDR::ULongLong*,&x));
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_float (CDR::Float x)
{
  return this->write_4 (ACE_reinterpret_cast (const CDR::ULong*, &x));
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_double (const CDR::Double &x)
{
  return this->write_8 (ACE_reinterpret_cast (const CDR::ULongLong*, &x));
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_longdouble (const CDR::LongDouble &x)
{
  return this->write_16 (ACE_reinterpret_cast (const CDR::LongDouble*,&x));
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_string (const CDR::Char *x)
{
  if (x != 0)
    return this->write_string (ACE_OS::strlen(x), x);
  return this->write_string (0, 0);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_string (const ACE_CString &x)
{
  return this->write_string (x.length(), x.c_str());
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_wstring (const CDR::WChar *x)
{
  if (x != 0)
    return this->write_wstring (ACE_OS::wslen (x), x);
  return this->write_wstring (0, 0);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_char_array (const CDR::Char *x,
                                 CDR::ULong length)
{
  return this->write_array (x,
                            CDR::OCTET_SIZE,
                            CDR::OCTET_ALIGN,
                            length);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_wchar_array (const CDR::WChar* x,
                                  CDR::ULong length)
{
  return this->write_array (x,
                            CDR::SHORT_SIZE,
                            CDR::SHORT_ALIGN,
                            length);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_octet_array (const CDR::Octet* x,
                                  CDR::ULong length)
{
  return this->write_array (x,
                            CDR::OCTET_SIZE,
                            CDR::OCTET_ALIGN,
                            length);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_short_array (const CDR::Short *x,
                                  CDR::ULong length)
{
  return this->write_array (x,
                            CDR::SHORT_SIZE,
                            CDR::SHORT_ALIGN,
                            length);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_ushort_array (const CDR::UShort *x,
                                   CDR::ULong length)
{
  return this->write_array (x,
                            CDR::SHORT_SIZE,
                            CDR::SHORT_ALIGN,
                            length);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_long_array (const CDR::Long *x,
                                 CDR::ULong length)
{
  return this->write_array (x,
                            CDR::LONG_SIZE,
                            CDR::LONG_ALIGN,
                            length);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_ulong_array (const CDR::ULong *x,
                                  CDR::ULong length)
{
  return this->write_array (x,
                            CDR::LONG_SIZE,
                            CDR::LONG_ALIGN,
                            length);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_longlong_array (const CDR::LongLong *x,
                                     CDR::ULong length)
{
  return this->write_array (x,
                            CDR::LONGLONG_SIZE,
                            CDR::LONGLONG_ALIGN,
                            length);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_ulonglong_array (const CDR::ULongLong *x,
                                      CDR::ULong length)
{
  return this->write_array (x,
                            CDR::LONGLONG_SIZE,
                            CDR::LONGLONG_ALIGN,
                            length);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_float_array (const CDR::Float *x,
                                  CDR::ULong length)
{
  return this->write_array (x,
                            CDR::LONG_SIZE,
                            CDR::LONG_ALIGN,
                            length);
}


ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_double_array (const CDR::Double *x,
                                   CDR::ULong length)
{
  return this->write_array (x,
                            CDR::LONGLONG_SIZE,
                            CDR::LONGLONG_ALIGN,
                            length);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::write_longdouble_array (const CDR::LongDouble* x,
                                       CDR::ULong length)
{
  return this->write_array (x,
                            CDR::LONGDOUBLE_SIZE,
                            CDR::LONGDOUBLE_ALIGN,
                            length);
}

ACE_INLINE int
ACE_OutputCDR::good_bit (void) const
{
  return this->good_bit_;
}

ACE_INLINE int
ACE_OutputCDR::adjust (size_t size,
                       size_t align,
                       char*& buf)
{
  buf = ptr_align_binary (this->current_->wr_ptr (),
                          align);
  char *end = buf + size;

  if (end <= this->current_->end ())
    {
      this->current_->wr_ptr (end);
      return 0;
    }

  return this->grow_and_adjust (size, align, buf);
}

ACE_INLINE int
ACE_OutputCDR::adjust (size_t size, char*& buf)
{
  return this->adjust (size, size, buf);
}

ACE_INLINE const ACE_Message_Block*
ACE_OutputCDR::begin (void) const
{
  return &this->start_;
}

ACE_INLINE const ACE_Message_Block*
ACE_OutputCDR::end (void) const
{
  return this->current_->cont ();
}

ACE_INLINE const char*
ACE_OutputCDR::buffer (void) const
{
  return this->start_.rd_ptr ();
}

ACE_INLINE size_t
ACE_OutputCDR::length (void) const
{
  return this->start_.length ();
}

ACE_INLINE int
ACE_OutputCDR::do_byte_swap (void) const
{
  return this->do_byte_swap_;
}

ACE_INLINE int
ACE_OutputCDR::align_write_ptr (size_t alignment)
{
  char *buf = ptr_align_binary (this->current_->wr_ptr (),
				alignment);

  if (buf <= this->current_->end ())
    {
      this->current_->wr_ptr (buf);
      return 0;
    }

  return this->grow_and_adjust (0, alignment, buf);
}  

// ****************************************************************

ACE_INLINE CDR::Boolean
ACE_InputCDR::read_octet (CDR::Octet& x)
{
  return this->read_1 (&x);
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::read_boolean (CDR::Boolean& x)
{
  CDR::Octet tmp;
  this->read_octet (tmp);
  x = tmp ? 1 : 0;
  return this->good_bit_;
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::read_char (CDR::Char &x)
{
  return this->read_1 (ACE_reinterpret_cast (CDR::Octet*, &x));
}


ACE_INLINE CDR::Boolean
ACE_InputCDR::read_wchar (CDR::WChar& x)
{
  return this->read_2 (ACE_reinterpret_cast (CDR::UShort*,&x));
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::read_short (CDR::Short &x)
{
  return this->read_2 (ACE_reinterpret_cast (CDR::UShort*, &x));
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::read_ushort (CDR::UShort &x)
{
  return this->read_2 (ACE_reinterpret_cast (CDR::UShort*, &x));
}


ACE_INLINE CDR::Boolean
ACE_InputCDR::read_long (CDR::Long &x)
{
  return this->read_4 (ACE_reinterpret_cast (CDR::ULong*, &x));
}


ACE_INLINE CDR::Boolean
ACE_InputCDR::read_ulong (CDR::ULong &x)
{
  return this->read_4 (ACE_reinterpret_cast (CDR::ULong*, &x));
}


ACE_INLINE CDR::Boolean
ACE_InputCDR::read_longlong (CDR::LongLong &x)
{
  return this->read_8 (ACE_reinterpret_cast (CDR::ULongLong*, &x));
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::read_ulonglong (CDR::ULongLong &x)
{
  return this->read_8 (ACE_reinterpret_cast (CDR::ULongLong*,&x));
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::read_float (CDR::Float &x)
{
  return this->read_4 (ACE_reinterpret_cast (CDR::ULong*, &x));
}


ACE_INLINE CDR::Boolean
ACE_InputCDR::read_double (CDR::Double &x)
{
  return this->read_8 (ACE_reinterpret_cast (CDR::ULongLong*, &x));
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::read_longdouble (CDR::LongDouble &x)
{
  return this->read_16 (ACE_reinterpret_cast (CDR::LongDouble*,&x));
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::read_char_array (CDR::Char* x,
                               CDR::ULong length)
{
  return this->read_array (x,
                           CDR::OCTET_SIZE,
                           CDR::OCTET_ALIGN,
                           length);
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::read_wchar_array (CDR::WChar* x,
                                CDR::ULong length)
{
  return this->read_array (x,
                           CDR::SHORT_SIZE,
                           CDR::SHORT_ALIGN,
                           length);
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::read_octet_array (CDR::Octet* x,
                                CDR::ULong length)
{
  return this->read_array (x,
                           CDR::OCTET_SIZE,
                           CDR::OCTET_ALIGN,
                           length);
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::read_short_array (CDR::Short *x,
                                CDR::ULong length)
{
  return this->read_array (x,
                           CDR::SHORT_SIZE,
                           CDR::SHORT_ALIGN,
                           length);
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::read_ushort_array (CDR::UShort *x,
                                 CDR::ULong length)
{
  return this->read_array (x,
                           CDR::SHORT_SIZE,
                           CDR::SHORT_ALIGN,
                           length);
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::read_long_array (CDR::Long *x,
                               CDR::ULong length)
{
  return this->read_array (x,
                           CDR::LONG_SIZE,
                           CDR::LONG_ALIGN,
                           length);
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::read_ulong_array (CDR::ULong *x,
                                CDR::ULong length)
{
  return this->read_array (x,
                           CDR::LONG_SIZE,
                           CDR::LONG_ALIGN,
                           length);
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::read_longlong_array (CDR::LongLong *x,
                                   CDR::ULong length)
{
  return this->read_array (x,
                           CDR::LONGLONG_SIZE,
                           CDR::LONGLONG_ALIGN,
                           length);
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::read_ulonglong_array (CDR::ULongLong *x,
                                    CDR::ULong length)
{
  return this->read_array (x,
                           CDR::LONGLONG_SIZE,
                           CDR::LONGLONG_ALIGN,
                           length);
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::read_float_array (CDR::Float *x,
                                CDR::ULong length)
{
  return this->read_array (x,
                           CDR::LONG_SIZE,
                           CDR::LONG_ALIGN,
                           length);
}


ACE_INLINE CDR::Boolean
ACE_InputCDR::read_double_array (CDR::Double *x,
                                 CDR::ULong length)
{
  return this->read_array (x,
                           CDR::LONGLONG_SIZE,
                           CDR::LONGLONG_ALIGN,
                           length);
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::read_longdouble_array (CDR::LongDouble* x,
                                     CDR::ULong length)
{
  return this->read_array (x,
                           CDR::LONGDOUBLE_SIZE,
                           CDR::LONGDOUBLE_ALIGN,
                           length);
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::skip_char (void)
{
  CDR::Char x;
  return this->read_1 (ACE_reinterpret_cast (CDR::Octet*,&x));
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::skip_wchar (void)
{
  CDR::WChar x;
  return this->read_2 (ACE_reinterpret_cast (CDR::UShort*,&x));
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::skip_octet (void)
{
  CDR::Octet x;
  return this->read_1 (&x);
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::skip_boolean (void)
{
  CDR::Octet tmp;
  this->read_octet (tmp);
  return this->good_bit_;
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::skip_short (void)
{
  CDR::Short x;
  return this->read_2 (ACE_reinterpret_cast (CDR::UShort*,&x));
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::skip_ushort (void)
{
  CDR::UShort x;
  return this->read_2 (ACE_reinterpret_cast (CDR::UShort*,&x));
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::skip_long (void)
{
  CDR::Long x;
  return this->read_4 (ACE_reinterpret_cast (CDR::ULong*,&x));
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::skip_ulong (void)
{
  CDR::ULong x;
  return this->read_4 (ACE_reinterpret_cast (CDR::ULong*,&x));
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::skip_longlong (void)
{
  CDR::LongLong x;
  return this->read_8 (ACE_reinterpret_cast (CDR::ULongLong*,&x));
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::skip_ulonglong (void)
{
  CDR::ULongLong x;
  return this->read_8 (ACE_reinterpret_cast (CDR::ULongLong*,&x));
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::skip_float (void)
{
  float x;
  return this->read_4 (ACE_reinterpret_cast (CDR::ULong*,&x));
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::skip_double (void)
{
  double x;
  return this->read_8 (ACE_reinterpret_cast (CDR::ULongLong*,&x));
}

ACE_INLINE CDR::Boolean
ACE_InputCDR::skip_longdouble (void)
{
  CDR::LongDouble x;
  return this->read_16 (ACE_reinterpret_cast (CDR::LongDouble*,&x));
}

ACE_INLINE char*
ACE_InputCDR::end (void)
{
  return this->start_.end ();
}

ACE_INLINE void
ACE_InputCDR::rd_ptr (size_t offset)
{
  this->start_.rd_ptr (offset);
}

ACE_INLINE char*
ACE_InputCDR::rd_ptr (void)
{
  return this->start_.rd_ptr ();
}

ACE_INLINE int
ACE_InputCDR::adjust (size_t size,
                      size_t align,
                      char*& buf)
{
  buf = ptr_align_binary (this->rd_ptr (), align);
  char *end = buf + size;
  if (end <= this->end ())
    {
      this->start_.rd_ptr (end);
      return 0;
    }

  this->good_bit_ = 0;
  return -1;
}

ACE_INLINE int
ACE_InputCDR::adjust (size_t size,
		      char*& buf)
{
  return this->adjust (size, size, buf);
}

ACE_INLINE size_t
ACE_InputCDR::length (void) const
{
  return this->start_.length ();
}

ACE_INLINE const ACE_Message_Block*
ACE_InputCDR::start (void) const
{
  return &this->start_;
}

ACE_INLINE int
ACE_InputCDR::good_bit (void) const
{
  return this->good_bit_;
}

// ****************************************************************

ACE_INLINE CDR::Boolean
operator<< (ACE_OutputCDR &os, CDR::Short x)
{
  os.write_short (x);
  return os.good_bit ();
}

ACE_INLINE CDR::Boolean
operator<< (ACE_OutputCDR &os, CDR::UShort x)
{
  os.write_ushort (x);
  return os.good_bit ();
}

ACE_INLINE CDR::Boolean
operator<< (ACE_OutputCDR &os, CDR::Long x)
{
  os.write_long (x);
  return os.good_bit ();
}

ACE_INLINE CDR::Boolean
operator<< (ACE_OutputCDR &os, CDR::ULong x)
{
  os.write_ulong (x);
  return os.good_bit ();
}

ACE_INLINE CDR::Boolean
operator<< (ACE_OutputCDR &os, CDR::LongLong x)
{
  os.write_longlong (x);
  return os.good_bit ();
}

ACE_INLINE CDR::Boolean
operator<< (ACE_OutputCDR &os, CDR::ULongLong x)
{
  os.write_ulonglong (x);
  return os.good_bit ();
}

ACE_INLINE CDR::Boolean
operator<< (ACE_OutputCDR &os, CDR::LongDouble x)
{
  os.write_longdouble (x);
  return os.good_bit ();
}

ACE_INLINE CDR::Boolean
operator<< (ACE_OutputCDR &os, CDR::Float x)
{
  os.write_float (x);
  return os.good_bit ();
}

ACE_INLINE CDR::Boolean
operator<< (ACE_OutputCDR &os, CDR::Double x)
{
  os.write_double (x);
  return os.good_bit ();
}

ACE_INLINE CDR::Boolean
operator<< (ACE_OutputCDR &os, const CDR::Char *x)
{
  os.write_string (x);
  return os.good_bit ();
}

ACE_INLINE CDR::Boolean
operator<< (ACE_OutputCDR &os, const ACE_CString &x)
{
  os.write_string (x);
  return os.good_bit ();
}

// The following use the helper classes
ACE_INLINE CDR::Boolean
operator<< (ACE_OutputCDR &os, ACE_OutputCDR::from_boolean x)
{
  os.write_boolean (x.val_);
  return os.good_bit ();
}

ACE_INLINE CDR::Boolean
operator<< (ACE_OutputCDR &os, ACE_OutputCDR::from_char x)
{
  os.write_char (x.val_);
  return os.good_bit ();
}

ACE_INLINE CDR::Boolean
operator<< (ACE_OutputCDR &os, ACE_OutputCDR::from_wchar x)
{
  os.write_wchar (x.val_);
  return os.good_bit ();
}

ACE_INLINE CDR::Boolean
operator<< (ACE_OutputCDR &os, ACE_OutputCDR::from_octet x)
{
  os.write_octet (x.val_);
  return os.good_bit ();
}

ACE_INLINE CDR::Boolean
operator<< (ACE_OutputCDR &os, ACE_OutputCDR::from_string x)
{
  os.write_string (x.bound_, x.val_);
  return os.good_bit ();
}

// ****************************************************************

ACE_INLINE CDR::Boolean
operator>> (ACE_InputCDR &is, CDR::Short &x)
{
  is.read_short (x);
  return is.good_bit ();
}

ACE_INLINE CDR::Boolean
operator>> (ACE_InputCDR &is, CDR::UShort &x)
{
  is.read_ushort (x);
  return is.good_bit ();
}

ACE_INLINE CDR::Boolean
operator>>(ACE_InputCDR &is, CDR::Long &x)
{
  is.read_long (x);
  return is.good_bit ();
}

ACE_INLINE CDR::Boolean
operator>> (ACE_InputCDR &is, CDR::ULong &x)
{
  is.read_ulong (x);
  return is.good_bit ();
}

ACE_INLINE CDR::Boolean
operator>> (ACE_InputCDR& is, CDR::LongLong &x)
{
  is.read_longlong (x);
  return is.good_bit ();
}

ACE_INLINE CDR::Boolean
operator>> (ACE_InputCDR& is, CDR::ULongLong &x)
{
  is.read_ulonglong (x);
  return is.good_bit ();
}

ACE_INLINE CDR::Boolean
operator>> (ACE_InputCDR& is, CDR::LongDouble &x)
{
  is.read_longdouble (x);
  return is.good_bit ();
}

ACE_INLINE CDR::Boolean
operator>> (ACE_InputCDR &is, CDR::Float &x)
{
  is.read_float (x);
  return is.good_bit ();
}

ACE_INLINE CDR::Boolean
operator>> (ACE_InputCDR &is, CDR::Double &x)
{
  is.read_double (x);
  return is.good_bit ();
}

ACE_INLINE CDR::Boolean
operator>> (ACE_InputCDR &is, CDR::Char *&x)
{
  is.read_string (x);
  return is.good_bit ();
}

ACE_INLINE CDR::Boolean
operator>> (ACE_InputCDR &is, ACE_CString &x)
{
  is.read_string (x);
  return is.good_bit ();
}

// The following use the helper classes
ACE_INLINE CDR::Boolean
operator>> (ACE_InputCDR &is, ACE_InputCDR::to_boolean x)
{
  is.read_boolean (x.ref_);
  return is.good_bit ();
}

ACE_INLINE CDR::Boolean
operator>> (ACE_InputCDR &is, ACE_InputCDR::to_char x)
{
  is.read_char (x.ref_);
  return is.good_bit ();
}

ACE_INLINE CDR::Boolean
operator>> (ACE_InputCDR &is, ACE_InputCDR::to_wchar x)
{
  is.read_wchar (x.ref_);
  return is.good_bit ();
}

ACE_INLINE CDR::Boolean
operator>> (ACE_InputCDR &is, ACE_InputCDR::to_octet x)
{
  is.read_octet (x.ref_);
  return is.good_bit ();
}

ACE_INLINE CDR::Boolean
operator>> (ACE_InputCDR &is, ACE_InputCDR::to_string x)
{
  is.read_string (x.val_);
  // check if the bounds are satisfied
  return (is.good_bit () &&
          (ACE_OS::strlen (x.val_) <= x.bound_));
}

// ***************************************************************************
// We must define these methods here because they use the "read_*" inlined
// methods of the ACE_InputCDR class
// ***************************************************************************

ACE_INLINE CDR::Boolean
ACE_OutputCDR::append_boolean (ACE_InputCDR &stream)
{
  CDR::Boolean x;
  return (stream.read_boolean (x) ? this->write_boolean (x) : 0);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::append_char (ACE_InputCDR &stream)
{
  CDR::Char x;
  return (stream.read_char (x) ? this->write_char (x) : 0);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::append_wchar (ACE_InputCDR &stream)
{
  CDR::WChar x;
  return (stream.read_wchar (x) ? this->write_wchar (x) : 0);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::append_octet (ACE_InputCDR &stream)
{
  CDR::Octet x;
  return (stream.read_octet (x) ? this->write_octet (x) : 0);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::append_short (ACE_InputCDR &stream)
{
  CDR::Short x;
  return (stream.read_short (x) ? this->write_short (x) : 0);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::append_ushort (ACE_InputCDR &stream)
{
  CDR::UShort x;
  return (stream.read_ushort (x) ? this->write_ushort (x) : 0);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::append_long (ACE_InputCDR &stream)
{
  CDR::Long x;
  return (stream.read_long (x) ? this->write_long (x) : 0);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::append_ulong (ACE_InputCDR &stream)
{
  CDR::ULong x;
  return (stream.read_ulong (x) ? this->write_ulong (x) : 0);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::append_longlong (ACE_InputCDR &stream)
{
  CDR::LongLong x;
  return (stream.read_longlong (x) ? this->write_longlong (x) : 0);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::append_ulonglong (ACE_InputCDR &stream)
{
  CDR::ULongLong x;
  return (stream.read_ulonglong (x) ? this->write_ulonglong (x) : 0);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::append_float (ACE_InputCDR &stream)
{
  CDR::Float x;
  return (stream.read_float (x) ? this->write_float (x) : 0);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::append_double (ACE_InputCDR &stream)
{
  CDR::Double x;
  return (stream.read_double (x) ? this->write_double (x) : 0);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::append_longdouble (ACE_InputCDR &stream)
{
  CDR::LongDouble x;
  return (stream.read_longdouble (x) ? this->write_longdouble (x) : 0);
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::append_string (ACE_InputCDR &stream)
{
  CDR::Char *x;
  CDR::Boolean flag =
    (stream.read_string (x) ? this->write_string (x) : 0);
  delete [] x;
  return flag;
}

ACE_INLINE CDR::Boolean
ACE_OutputCDR::append_wstring (ACE_InputCDR &stream)
{
  CDR::WChar *x;
  CDR::Boolean flag =
    (stream.read_wstring (x) ? this->write_wstring (x) : 0);
  delete [] x;
  return flag;
}

ACE_INLINE int
ACE_InputCDR::do_byte_swap (void) const
{
  return this->do_byte_swap_;
}

ACE_INLINE int
ACE_InputCDR::align_read_ptr (size_t alignment)
{
  char *buf = ptr_align_binary (this->rd_ptr (),
                                alignment);

  if (buf <= this->end ())
    {
      this->start_.rd_ptr (buf);
      return 0;
    }

  this->good_bit_ = 0;
  return -1;
}

