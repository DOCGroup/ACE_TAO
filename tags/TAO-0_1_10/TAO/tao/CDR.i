// This may look like C, but it's really -*- C++ -*-
// $Id$

// Decode the CDR stream.

ACE_INLINE CORBA::TypeCode::traverse_status
CDR::decode (CORBA::TypeCode_ptr tc,
             const void *data,
             const void *data2,
             CORBA::Environment &env)
{
  this->mobj_ = this->factory_->make_marshal_object (tc, env);

  if (this->mobj_)
    return this->mobj_->decode (tc, data, data2, this, env);
  else
    return CORBA::TypeCode::TRAVERSE_STOP;
}

// Encode data into CDR stream.

ACE_INLINE CORBA::Boolean
CDR::skip_string (void)
{
  char *dummy0;
  CORBA::ULong dummy1;
  return this->get_encapsulation (dummy0, dummy1);
}

ACE_INLINE CORBA::TypeCode::traverse_status
CDR::encode (CORBA::TypeCode_ptr tc,
             const void *data,
             const void *data2,
             CORBA::Environment &env)
{
  this->mobj_ = this->factory_->make_marshal_object (tc, env);

  if (this->mobj_)
    return this->mobj_->encode (tc, data, data2, this, env);
  else
    return CORBA::TypeCode::TRAVERSE_STOP;
}

ACE_INLINE size_t
CDR::bytes_remaining (void)
{
  return this->mb_->length();
}

ACE_INLINE int
CDR::good_bit (void) const
{
  return this->good_bit_;
}

ACE_INLINE char*
CDR::buffer (void) const
{
  return this->mb_->rd_ptr();
}

ACE_INLINE size_t
CDR::length (void) const
{
  return this->mb_->length ();
}

ACE_INLINE size_t
CDR::size (void) const
{
  return this->mb_->size ();
}

// ****************************************************************
// put_ routines

ACE_INLINE CORBA::Boolean
CDR::put_byte (char c)
{
  if (this->mb_->wr_ptr () + 1 <= this->mb_->end ()
      || grow (0) == CORBA::B_TRUE)
    {
      this->mb_->copy (&c, 1);
      return CORBA::B_TRUE;
    }
  this->good_bit_ = 0;
  return CORBA::B_FALSE;
}

ACE_INLINE CORBA::Boolean
CDR::put_char (CORBA::Char c)
{
  return put_byte ((char) c);
}

ACE_INLINE CORBA::Boolean
CDR::put_wchar (CORBA::WChar wc)
{
  // "wchar_t" isn't always 2 bytes, such systems might need further
  // conversion (e.g. hosts with multibyte characters native, rather
  // than UNICODE)

  return put_short ((short) wc);
}

ACE_INLINE CORBA::Boolean
CDR::put_boolean (CORBA::Boolean b)
{
  return put_byte ((char) (b != CORBA::B_FALSE));
}

ACE_INLINE CORBA::Boolean
CDR::put_octet (CORBA::Octet o)
{
  return put_byte ((char) o);
}

ACE_INLINE CORBA::Boolean
CDR::put_ushort (CORBA::UShort s)
{
  return put_short ((CORBA::Short) s);
}

ACE_INLINE CORBA::Boolean
CDR::put_ulong (CORBA::ULong l)
{
  return put_long ((CORBA::Long) l);
}

ACE_INLINE CORBA::Boolean
CDR::put_longlong (const CORBA::LongLong &ll)
{
  return put_ulonglong ((CORBA::ULongLong &) ll);
}

ACE_INLINE CORBA::Boolean
CDR::put_float (CORBA::Float f)
{
  return put_long (*(CORBA::Long *) &f);
}

ACE_INLINE CORBA::Boolean
CDR::put_double (const CORBA::Double &d)
{
  return put_ulonglong (*(CORBA::ULongLong *) &d);
}

// ****************************************************************
// DECODING routines ... adjust pointer, then byteswap as needed.

ACE_INLINE CORBA::Boolean
CDR::get_byte (char &c)
{
  if (this->mb_->rd_ptr () + 1 <= this->mb_->end ())
    {
      c = (char) *this->mb_->rd_ptr ();
      this->mb_->rd_ptr (1);
      return CORBA::B_TRUE;
    }
  this->good_bit_ = 0;
  return CORBA::B_FALSE;
}

ACE_INLINE CORBA::Boolean
CDR::get_char (CORBA::Char &o)
{
  return this->get_byte ((char &) o);
}

ACE_INLINE CORBA::Boolean
CDR::get_wchar (CORBA::WChar &wc)
{
  short s;

  // wchar_t isn't always "short"

  CORBA::Boolean retval = this->get_short (s);
  wc = s;
  return retval;
}

ACE_INLINE CORBA::Boolean
CDR::get_boolean (CORBA::Boolean &b)
{
  CORBA::Char c;

  // CORBA::Boolean is rarely 'char'
  CORBA::Boolean retval = this->get_char (c);
  b = (c == 1);
  return retval;
}

ACE_INLINE CORBA::Boolean
CDR::get_octet (CORBA::Octet &o)
{
  return this->get_byte ((char &) o);
}

ACE_INLINE CORBA::Boolean
CDR::get_ushort (CORBA::UShort &s)
{
  return this->get_short ((short&) s);
}

ACE_INLINE CORBA::Boolean
CDR::get_ulong (CORBA::ULong &l)
{
  return this->get_long ((CORBA::Long &) l);
}

ACE_INLINE CORBA::Boolean
CDR::get_longlong (CORBA::LongLong &ll)
{
  return this->get_ulonglong ((CORBA::ULongLong &) ll);
}

ACE_INLINE CORBA::Boolean
CDR::get_float (float &f)
{
  return this->get_long ((CORBA::Long &) f);
}

ACE_INLINE CORBA::Boolean
CDR::get_double (double &d)
{
  return this->get_ulonglong ((CORBA::ULongLong &) d);
}

ACE_INLINE CORBA::Boolean
CDR::rd_ptr (size_t n)
{
  if (this->mb_->rd_ptr () + n <= this->mb_->end ())
    {
      this->mb_->rd_ptr (n);
      return CORBA::B_TRUE;
    }
  this->good_bit_ = 0;
  return CORBA::B_FALSE;
}

ACE_INLINE CORBA::Boolean
CDR::wr_ptr (size_t n)
{
  if (this->mb_->wr_ptr () + n <= this->mb_->end ())
    {
      this->mb_->wr_ptr (n);
      return CORBA::B_TRUE;
    }
  this->good_bit_ = 0;
  return CORBA::B_FALSE;
}

// ****************************************************************
// TODO: Use the CORBA_* types when they become available.

ACE_INLINE CDR &
operator<< (CDR &cdr, CORBA::Octet x)
{
  cdr.put_octet (x);
  return cdr;
}

ACE_INLINE CDR &
operator<< (CDR &cdr, CORBA::Short x)
{
  cdr.put_short (x);
  return cdr;
}

ACE_INLINE CDR &
operator<< (CDR &cdr, CORBA::UShort x)
{
  cdr.put_short (x);
  return cdr;
}

ACE_INLINE CDR &
operator<< (CDR &cdr, CORBA::Long x)
{
  cdr.put_long (x);
  return cdr;
}

ACE_INLINE CDR &
operator<< (CDR &cdr, CORBA::ULong x)
{
  cdr.put_long (x);
  return cdr;
}

ACE_INLINE CDR &
operator<< (CDR &cdr, CORBA::LongLong x)
{
  cdr.put_longlong (x);
  return cdr;
}

ACE_INLINE CDR &
operator<< (CDR &cdr, CORBA::ULongLong x)
{
  cdr.put_ulonglong (x);
  return cdr;
}

ACE_INLINE CDR &
operator<< (CDR &cdr, CORBA::Float x)
{
  cdr.put_float (x);
  return cdr;
}

ACE_INLINE CDR &
operator<< (CDR &cdr, CORBA::Double x)
{
  cdr.put_double (x);
  return cdr;
}

ACE_INLINE CDR &
operator<< (CDR &cdr, CORBA::Char x)
{
  cdr.put_char (x);
  return cdr;
}

ACE_INLINE CDR &
operator>> (CDR &cdr, CORBA::Octet &x)
{
  cdr.get_octet (x);
  return cdr;
}

ACE_INLINE CDR &
operator>> (CDR &cdr, CORBA::Short &x)
{
  cdr.get_short (x);
  return cdr;
}

ACE_INLINE CDR &
operator>> (CDR &cdr, CORBA::UShort &x)
{
  cdr.get_ushort (x);
  return cdr;
}

ACE_INLINE CDR &
operator>> (CDR &cdr, CORBA::Long &x)
{
  cdr.get_long (x);
  return cdr;
}

ACE_INLINE CDR &
operator>> (CDR &cdr, CORBA::ULong &x)
{
  cdr.get_ulong (x);
  return cdr;
}

ACE_INLINE CDR &
operator>> (CDR &cdr, CORBA::LongLong &x)
{
  cdr.get_longlong (x);
  return cdr;
}

ACE_INLINE CDR &
operator>> (CDR &cdr, CORBA::ULongLong &x)
{
  cdr.get_ulonglong (x);
  return cdr;
}

ACE_INLINE CDR &
operator>> (CDR &cdr, CORBA::Float &x)
{
  cdr.get_float (x);
  return cdr;
}

ACE_INLINE CDR &
operator>> (CDR &cdr, CORBA::Double &x)
{
  cdr.get_double (x);
  return cdr;
}

ACE_INLINE CDR &
operator>> (CDR &cdr, CORBA::Char &x)
{
  cdr.get_char (x);
  return cdr;
}
