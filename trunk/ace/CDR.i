// -*- C++ -*-
// $Id$

// ****************************************************************

ACE_INLINE void 
ACE_CDR::swap_2 (const char *orig, char* target)
{
  target[1] = *orig++;
  target[0] = *orig++;
}
 
ACE_INLINE void 
ACE_CDR::swap_4 (const char *orig, char* target)
{
  target [3] = *orig++;
  target [2] = *orig++;
  target [1] = *orig++;
  target [0] = *orig++;
}

ACE_INLINE void 
ACE_CDR::swap_8 (const char *orig, char* target)
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
ACE_CDR::swap_16 (const char *orig, char* target)
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
ACE_CDR::mb_align (ACE_Message_Block *mb)
{
  char *start = ptr_align_binary (mb->base (),
                                  ACE_CDR::MAX_ALIGNMENT);
  mb->rd_ptr (start);
  mb->wr_ptr (start);
}

#if defined (ACE_HAS_PURIFY)
  // This is a good place to zero the buffer. It makes Purify happy.
  if (mb->base () != 0)
    {
      (void) ACE_OS::memset (mb->base (), 0, mb->size ());
    }
#endif /* ACE_HAS_PURIFY */

// ****************************************************************

// implementing the special types
ACE_INLINE
ACE_OutputCDR::from_boolean::from_boolean (CDR_Boolean b)
  : val_ (b)
{
}

ACE_INLINE
ACE_InputCDR::to_boolean::to_boolean (CDR_Boolean &b)
  : ref_ (b)
{
}

ACE_INLINE
ACE_OutputCDR::from_octet::from_octet (CDR_Octet o)
  : val_ (o)
{
}

ACE_INLINE
ACE_InputCDR::to_octet::to_octet (CDR_Octet &o)
  : ref_ (o)
{
}

ACE_INLINE
ACE_OutputCDR::from_char::from_char (CDR_Char c)
  : val_ (c)
{
}

ACE_INLINE
ACE_InputCDR::to_char::to_char (CDR_Char &c)
  : ref_ (c)
{
}

ACE_INLINE
ACE_OutputCDR::from_wchar::from_wchar (ACE_OS::WChar wc)
  : val_ (wc)
{
}

ACE_INLINE
ACE_InputCDR::to_wchar::to_wchar (ACE_OS::WChar &wc)
  : ref_ (wc)
{
}

ACE_INLINE
ACE_OutputCDR::from_string::from_string (CDR_Char *s,
                                         CDR_ULong b,
                                         CDR_Boolean nocopy)
  : val_ (s),
    bound_ (b),
    nocopy_ (nocopy)
{
}

ACE_INLINE
ACE_InputCDR::to_string::to_string (CDR_Char *&s,
                                    CDR_ULong b)
  : val_ (s),
    bound_ (b)
{
}

// ****************************************************************

// Decode the CDR stream.

ACE_INLINE CDR_Boolean
ACE_OutputCDR::write_octet (CDR_Octet x)
{
  return this->write_1 (ACE_reinterpret_cast (const CDR_Octet*,&x));
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::write_boolean (CDR_Boolean x)
{
  return this->write_octet (x ? 1 : 0);
}

ACE_INLINE CDR_Boolean 
ACE_OutputCDR::write_char (CDR_Char x)
{
  return this->write_1 (ACE_reinterpret_cast (CDR_Octet*, &x));
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::write_wchar (ACE_OS::WChar x)
{
  return this->write_2 (ACE_reinterpret_cast (const CDR_UShort*,&x));
}

ACE_INLINE CDR_Boolean 
ACE_OutputCDR::write_short (CDR_Short x)
{
  return this->write_2 (ACE_reinterpret_cast (const CDR_UShort*, &x));
}

ACE_INLINE CDR_Boolean 
ACE_OutputCDR::write_ushort (CDR_UShort x)
{
  return this->write_2 (ACE_reinterpret_cast (const CDR_UShort*, &x));
}

ACE_INLINE CDR_Boolean 
ACE_OutputCDR::write_long (CDR_Long x)
{
  return this->write_4 (ACE_reinterpret_cast (const CDR_ULong*, &x));
}

ACE_INLINE CDR_Boolean 
ACE_OutputCDR::write_ulong (CDR_ULong x)
{
  return this->write_4 (ACE_reinterpret_cast (const CDR_ULong*, &x));
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::write_longlong (const CDR_LongLong &x)
{
  return this->write_8 (ACE_reinterpret_cast (const CDR_ULongLong*, &x));
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::write_ulonglong (const CDR_ULongLong &x)
{
  return this->write_8 (ACE_reinterpret_cast (const CDR_ULongLong*,&x));
}

ACE_INLINE CDR_Boolean 
ACE_OutputCDR::write_float (CDR_Float x)
{
  return this->write_4 (ACE_reinterpret_cast (const CDR_ULong*, &x));
}

ACE_INLINE CDR_Boolean 
ACE_OutputCDR::write_double (const CDR_Double &x)
{
  return this->write_8 (ACE_reinterpret_cast (const CDR_ULongLong*, &x));
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::write_longdouble (const CDR_LongDouble &x)
{
  return this->write_16 (ACE_reinterpret_cast (const CDR_LongDouble*,&x));
}

ACE_INLINE CDR_Boolean 
ACE_OutputCDR::write_string (const CDR_Char *x)
{
  if (x != 0)
    return this->write_string (ACE_OS::strlen(x), x);
  return this->write_string (0, 0);
}

ACE_INLINE CDR_Boolean 
ACE_OutputCDR::write_string (const ACE_CString &x)
{
  return this->write_string (x.length(), x.c_str());
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::write_wstring (const ACE_OS::WChar *x)
{
  if (x != 0)
    return this->write_wstring (ACE_OS::wslen (x), x);
  return this->write_wstring (0, 0);
}

ACE_INLINE CDR_Boolean 
ACE_OutputCDR::write_char_array (const CDR_Char *x, 
                                 CDR_ULong length)
{
  return this->write_array (x, 
			    ACE_CDR::OCTET_SIZE,
			    ACE_CDR::OCTET_ALIGN,
			    length);
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::write_wchar_array (const ACE_OS::WChar* x,
                                  CDR_ULong length)
{
  return this->write_array (x,
                            ACE_CDR::SHORT_SIZE,
                            ACE_CDR::SHORT_ALIGN,
                            length);
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::write_octet_array (const CDR_Octet* x,
                                  CDR_ULong length)
{
  return this->write_array (x,
                            ACE_CDR::OCTET_SIZE,
                            ACE_CDR::OCTET_ALIGN,
                            length);
}

ACE_INLINE CDR_Boolean 
ACE_OutputCDR::write_short_array (const CDR_Short *x, 
                                  CDR_ULong length)
{
  return this->write_array (x,
			    ACE_CDR::SHORT_SIZE,
			    ACE_CDR::SHORT_ALIGN,
			    length);
}

ACE_INLINE CDR_Boolean 
ACE_OutputCDR::write_ushort_array (const CDR_UShort *x, 
                                   CDR_ULong length)
{
  return this->write_array (x,
			    ACE_CDR::SHORT_SIZE,
			    ACE_CDR::SHORT_ALIGN,
			    length);
}

ACE_INLINE CDR_Boolean 
ACE_OutputCDR::write_long_array (const CDR_Long *x, 
                                 CDR_ULong length)
{
  return this->write_array (x,
			    ACE_CDR::LONG_SIZE,
			    ACE_CDR::LONG_ALIGN,
			    length);
}
  
ACE_INLINE CDR_Boolean 
ACE_OutputCDR::write_ulong_array (const CDR_ULong *x, 
                                  CDR_ULong length)
{   
  return this->write_array (x,
			    ACE_CDR::LONG_SIZE,
			    ACE_CDR::LONG_ALIGN,
			    length);
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::write_longlong_array (const CDR_LongLong *x,
                                     CDR_ULong length)
{
  return this->write_array (x,
                            ACE_CDR::LONGLONG_SIZE,
                            ACE_CDR::LONGLONG_ALIGN,
                            length);
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::write_ulonglong_array (const CDR_ULongLong *x,
                                      CDR_ULong length)
{
  return this->write_array (x,
                            ACE_CDR::LONGLONG_SIZE,
                            ACE_CDR::LONGLONG_ALIGN,
                            length);
}

ACE_INLINE CDR_Boolean 
ACE_OutputCDR::write_float_array (const CDR_Float *x, 
                                  CDR_ULong length)
{
  return this->write_array (x,
			    ACE_CDR::LONG_SIZE,
			    ACE_CDR::LONG_ALIGN,
			    length);
}


ACE_INLINE CDR_Boolean 
ACE_OutputCDR::write_double_array (const CDR_Double *x, 
                                   CDR_ULong length)
{
  return this->write_array (x,
			    ACE_CDR::LONGLONG_SIZE,
			    ACE_CDR::LONGLONG_ALIGN,
			    length);
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::write_longdouble_array (const CDR_LongDouble* x,
                                       CDR_ULong length)
{
  return this->write_array (x,
                            ACE_CDR::LONGDOUBLE_SIZE,
                            ACE_CDR::LONGDOUBLE_ALIGN,
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
  buf = ptr_align_binary (this->current_->wr_ptr(), 
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

// ****************************************************************

ACE_INLINE CDR_Boolean
ACE_InputCDR::read_octet (CDR_Octet& x)
{
  return this->read_1 (&x);
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::read_boolean (CDR_Boolean& x)
{
  CDR_Octet tmp;
  this->read_octet (tmp);
  x = tmp ? 1 : 0;
  return this->good_bit_;
}

ACE_INLINE CDR_Boolean 
ACE_InputCDR::read_char (CDR_Char &x)
{
  return this->read_1 (ACE_reinterpret_cast (CDR_Octet*, &x));
}


ACE_INLINE CDR_Boolean
ACE_InputCDR::read_wchar (ACE_OS::WChar& x)
{
  return this->read_2 (ACE_reinterpret_cast (CDR_UShort*,&x));
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::read_short (CDR_Short &x)
{
  return this->read_2 (ACE_reinterpret_cast (CDR_UShort*, &x));
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::read_ushort (CDR_UShort &x)
{
  return this->read_2 (ACE_reinterpret_cast (CDR_UShort*, &x));
}


ACE_INLINE CDR_Boolean
ACE_InputCDR::read_long (CDR_Long &x)
{
  return this->read_4 (ACE_reinterpret_cast (CDR_ULong*, &x));
}


ACE_INLINE CDR_Boolean
ACE_InputCDR::read_ulong (CDR_ULong &x)
{
  return this->read_4 (ACE_reinterpret_cast (CDR_ULong*, &x));
}


ACE_INLINE CDR_Boolean
ACE_InputCDR::read_longlong (CDR_LongLong &x)
{
  return this->read_8 (ACE_reinterpret_cast (CDR_ULongLong*, &x));
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::read_ulonglong (CDR_ULongLong &x)
{
  return this->read_8 (ACE_reinterpret_cast (CDR_ULongLong*,&x));
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::read_float (CDR_Float &x)
{
  return this->read_4 (ACE_reinterpret_cast (CDR_ULong*, &x));
}


ACE_INLINE CDR_Boolean
ACE_InputCDR::read_double (CDR_Double &x)
{
  return this->read_8 (ACE_reinterpret_cast (CDR_ULongLong*, &x));
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::read_longdouble (CDR_LongDouble &x)
{
  return this->read_16 (ACE_reinterpret_cast (CDR_LongDouble*,&x));
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::read_char_array (CDR_Char* x, 
                               CDR_ULong length)
{
  return this->read_array (x,
			   ACE_CDR::OCTET_SIZE,
			   ACE_CDR::OCTET_ALIGN,
			   length);
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::read_wchar_array (ACE_OS::WChar* x,
                                CDR_ULong length)
{
  return this->read_array (x,
                           ACE_CDR::SHORT_SIZE,
                           ACE_CDR::SHORT_ALIGN,
                           length);
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::read_octet_array (CDR_Octet* x,
                                CDR_ULong length)
{
  return this->read_array (x,
                           ACE_CDR::OCTET_SIZE,
                           ACE_CDR::OCTET_ALIGN,
                           length);
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::read_short_array (short *x, 
                                CDR_ULong length)
{
  return this->read_array (x,
			   ACE_CDR::SHORT_SIZE,
			   ACE_CDR::SHORT_ALIGN,
			   length);
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::read_ushort_array (u_short *x, 
                                 CDR_ULong length)
{
  return this->read_array (x,
			   ACE_CDR::SHORT_SIZE,
			   ACE_CDR::SHORT_ALIGN,
			   length);
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::read_long_array (CDR_Long *x, 
                               CDR_ULong length)
{
  return this->read_array (x,
			   ACE_CDR::LONG_SIZE,
			   ACE_CDR::LONG_ALIGN,
			   length);
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::read_ulong_array (CDR_ULong *x, 
                                CDR_ULong length)
{
  return this->read_array (x,
			   ACE_CDR::LONG_SIZE,
			   ACE_CDR::LONG_ALIGN,
			   length);
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::read_longlong_array (CDR_LongLong *x,
                                   CDR_ULong length)
{
  return this->read_array (x,
                           ACE_CDR::LONGLONG_SIZE,
                           ACE_CDR::LONGLONG_ALIGN,
                           length);
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::read_ulonglong_array (CDR_ULongLong *x,
                                    CDR_ULong length)
{
  return this->read_array (x,
                           ACE_CDR::LONGLONG_SIZE,
                           ACE_CDR::LONGLONG_ALIGN,
                           length);
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::read_float_array (CDR_Float *x, 
                                CDR_ULong length)
{
  return this->read_array (x,
			   ACE_CDR::LONG_SIZE,
			   ACE_CDR::LONG_ALIGN,
			   length);
}


ACE_INLINE CDR_Boolean
ACE_InputCDR::read_double_array (CDR_Double *x, 
                                 CDR_ULong length)
{
  return this->read_array (x,
			   ACE_CDR::LONGLONG_SIZE,
			   ACE_CDR::LONGLONG_ALIGN,
			   length);
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::read_longdouble_array (CDR_LongDouble* x,
                                     CDR_ULong length)
{
  return this->read_array (x,
                            ACE_CDR::LONGDOUBLE_SIZE,
                            ACE_CDR::LONGDOUBLE_ALIGN,
                            length);
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::skip_char (void)
{
  CDR_Char x;
  return this->read_1 (ACE_reinterpret_cast (CDR_Octet*,&x));
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::skip_wchar (void)
{
  ACE_OS::WChar x;
  return this->read_2 (ACE_reinterpret_cast (ACE_UINT16*,&x));
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::skip_octet (void)
{
  CDR_Octet x;
  return this->read_1 (&x);
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::skip_boolean (void)
{
  CDR_Octet tmp;
  this->read_octet (tmp);
  return this->good_bit_;
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::skip_short (void)
{
  ACE_INT16 x;
  return this->read_2 (ACE_reinterpret_cast (ACE_UINT16*,&x));
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::skip_ushort (void)
{
  ACE_UINT16 x;
  return this->read_2 (ACE_reinterpret_cast (ACE_UINT16*,&x));
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::skip_long (void)
{
  ACE_INT32 x;
  return this->read_4 (ACE_reinterpret_cast (ACE_UINT32*,&x));
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::skip_ulong (void)
{
  ACE_UINT32 x;
  return this->read_4 (ACE_reinterpret_cast (ACE_UINT32*,&x));
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::skip_longlong (void)
{
  CDR_LongLong x;
  return this->read_8 (ACE_reinterpret_cast (ACE_UINT64*,&x));
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::skip_ulonglong (void)
{
  ACE_UINT64 x;
  return this->read_8 (ACE_reinterpret_cast (ACE_UINT64*,&x));
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::skip_float (void)
{
  float x;
  return this->read_4 (ACE_reinterpret_cast (ACE_UINT32*,&x));
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::skip_double (void)
{
  double x;
  return this->read_8 (ACE_reinterpret_cast (ACE_UINT64*,&x));
}

ACE_INLINE CDR_Boolean
ACE_InputCDR::skip_longdouble (void)
{
  CDR_LongDouble x;
  return this->read_16 (ACE_reinterpret_cast (CDR_LongDouble*,&x));
}

ACE_INLINE char*
ACE_InputCDR::end (void)
{
  return this->start_.end ();
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

ACE_INLINE CDR_Boolean
operator<< (ACE_OutputCDR &os, short x)
{
  os.write_short (x);
  return os.good_bit ();
}

ACE_INLINE CDR_Boolean
operator<< (ACE_OutputCDR &os, u_short x)
{
  os.write_ushort (x);
  return os.good_bit ();
}

ACE_INLINE CDR_Boolean
operator<< (ACE_OutputCDR &os, CDR_Long x)
{
  os.write_long (x);
  return os.good_bit ();
}

ACE_INLINE CDR_Boolean
operator<< (ACE_OutputCDR &os, CDR_ULong x)
{
  os.write_ulong (x);
  return os.good_bit ();
}

ACE_INLINE CDR_Boolean
operator<< (ACE_OutputCDR &os, CDR_LongLong x)
{
  os.write_longlong (x);
  return os.good_bit ();
}

ACE_INLINE CDR_Boolean
operator<< (ACE_OutputCDR &os, CDR_ULongLong x)
{
  os.write_ulonglong (x);
  return os.good_bit ();
}

ACE_INLINE CDR_Boolean
operator<< (ACE_OutputCDR &os, CDR_LongDouble x)
{
  os.write_longdouble (x);
  return os.good_bit ();
}

ACE_INLINE CDR_Boolean
operator<< (ACE_OutputCDR &os, float x)
{
  os.write_float (x);
  return os.good_bit ();
}

ACE_INLINE CDR_Boolean
operator<< (ACE_OutputCDR &os, double x)
{
  os.write_double (x);
  return os.good_bit ();
}

ACE_INLINE CDR_Boolean
operator<< (ACE_OutputCDR &os, const char *x)
{
  os.write_string (x);
  return os.good_bit ();
}

ACE_INLINE CDR_Boolean
operator<< (ACE_OutputCDR &os, const ACE_CString &x)
{
  os.write_string (x);
  return os.good_bit ();
}

// The following use the helper classes
ACE_INLINE CDR_Boolean
operator<< (ACE_OutputCDR &os, ACE_OutputCDR::from_boolean x)
{
  os.write_boolean (x.val_);
  return os.good_bit ();
}

ACE_INLINE CDR_Boolean
operator<< (ACE_OutputCDR &os, ACE_OutputCDR::from_char x)
{
  os.write_char (x.val_);
  return os.good_bit ();
}

ACE_INLINE CDR_Boolean
operator<< (ACE_OutputCDR &os, ACE_OutputCDR::from_wchar x)
{
  os.write_wchar (x.val_);
  return os.good_bit ();
}

ACE_INLINE CDR_Boolean
operator<< (ACE_OutputCDR &os, ACE_OutputCDR::from_octet x)
{
  os.write_octet (x.val_);
  return os.good_bit ();
}

ACE_INLINE CDR_Boolean
operator<< (ACE_OutputCDR &os, ACE_OutputCDR::from_string x)
{
  os.write_string (x.bound_, x.val_);
  return os.good_bit ();
}

// ****************************************************************

ACE_INLINE CDR_Boolean
operator>> (ACE_InputCDR &is, short &x)
{
  is.read_short (x);
  return is.good_bit ();
}

ACE_INLINE CDR_Boolean
operator>> (ACE_InputCDR &is, u_short &x)
{
  is.read_ushort (x);
  return is.good_bit ();
}

ACE_INLINE CDR_Boolean
operator>>(ACE_InputCDR &is, CDR_Long &x)
{
  is.read_long (x);
  return is.good_bit ();
}

ACE_INLINE CDR_Boolean
operator>> (ACE_InputCDR &is, CDR_ULong &x)
{ 
  is.read_ulong (x);
  return is.good_bit ();
}

ACE_INLINE CDR_Boolean
operator>> (ACE_InputCDR& is, CDR_LongLong &x)
{
  is.read_longlong (x);
  return is.good_bit ();
}

ACE_INLINE CDR_Boolean
operator>> (ACE_InputCDR& is, CDR_ULongLong &x)
{
  is.read_ulonglong (x);
  return is.good_bit ();
}

ACE_INLINE CDR_Boolean
operator>> (ACE_InputCDR& is, CDR_LongDouble &x)
{
  is.read_longdouble (x);
  return is.good_bit ();
}

ACE_INLINE CDR_Boolean
operator>> (ACE_InputCDR &is, float &x)
{
  is.read_float (x);
  return is.good_bit ();
}

ACE_INLINE CDR_Boolean
operator>> (ACE_InputCDR &is, double &x)
{
  is.read_double (x);
  return is.good_bit ();
}

ACE_INLINE CDR_Boolean
operator>> (ACE_InputCDR &is, char *&x)
{
  is.read_string (x);
  return is.good_bit ();
}

ACE_INLINE CDR_Boolean
operator>> (ACE_InputCDR &is, ACE_CString &x)
{
  is.read_string (x);
  return is.good_bit ();
}

// The following use the helper classes
ACE_INLINE CDR_Boolean
operator>> (ACE_InputCDR &is, ACE_InputCDR::to_boolean x)
{
  is.read_boolean (x.ref_);
  return is.good_bit ();
}

ACE_INLINE CDR_Boolean
operator>> (ACE_InputCDR &is, ACE_InputCDR::to_char x)
{
  is.read_char (x.ref_);
  return is.good_bit ();
}

ACE_INLINE CDR_Boolean
operator>> (ACE_InputCDR &is, ACE_InputCDR::to_wchar x)
{
  is.read_wchar (x.ref_);
  return is.good_bit ();
}

ACE_INLINE CDR_Boolean
operator>> (ACE_InputCDR &is, ACE_InputCDR::to_octet x)
{
  is.read_octet (x.ref_);
  return is.good_bit ();
}

ACE_INLINE CDR_Boolean
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

ACE_INLINE CDR_Boolean
ACE_OutputCDR::append_boolean (ACE_InputCDR &stream)
{
  CDR_Boolean x;
  return (stream.read_boolean (x) ? this->write_boolean (x) : 0);
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::append_char (ACE_InputCDR &stream)
{
  char x;
  return (stream.read_char (x) ? this->write_char (x) : 0);
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::append_wchar (ACE_InputCDR &stream)
{
  ACE_OS::WChar x;
  return (stream.read_wchar (x) ? this->write_wchar (x) : 0);
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::append_octet (ACE_InputCDR &stream)
{
  CDR_Octet x;
  return (stream.read_octet (x) ? this->write_octet (x) : 0);
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::append_short (ACE_InputCDR &stream)
{
  short x;
  return (stream.read_short (x) ? this->write_short (x) : 0);
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::append_ushort (ACE_InputCDR &stream)
{
  u_short x;
  return (stream.read_ushort (x) ? this->write_ushort (x) : 0);
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::append_long (ACE_InputCDR &stream)
{
  CDR_Long x;
  return (stream.read_long (x) ? this->write_long (x) : 0);
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::append_ulong (ACE_InputCDR &stream)
{
  CDR_ULong x;
  return (stream.read_ulong (x) ? this->write_ulong (x) : 0);
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::append_longlong (ACE_InputCDR &stream)
{
  CDR_LongLong x;
  return (stream.read_longlong (x) ? this->write_longlong (x) : 0);
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::append_ulonglong (ACE_InputCDR &stream)
{
  ACE_UINT64 x;
  return (stream.read_ulonglong (x) ? this->write_ulonglong (x) : 0);
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::append_float (ACE_InputCDR &stream)
{
  float x;
  return (stream.read_float (x) ? this->write_float (x) : 0);
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::append_double (ACE_InputCDR &stream)
{
  double x;
  return (stream.read_double (x) ? this->write_double (x) : 0);
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::append_longdouble (ACE_InputCDR &stream)
{
  CDR_LongDouble x;
  return (stream.read_longdouble (x) ? this->write_longdouble (x) : 0);
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::append_string (ACE_InputCDR &stream)
{
  char *x;
  CDR_Boolean flag = (stream.read_string (x) ? this->write_string (x) : 0);
  return flag;
}

ACE_INLINE CDR_Boolean
ACE_OutputCDR::append_wstring (ACE_InputCDR &stream)
{
  ACE_OS::WChar *x;
  CDR_Boolean flag = (stream.read_wstring (x) ? this->write_wstring (x) : 0);
  return flag;
}

