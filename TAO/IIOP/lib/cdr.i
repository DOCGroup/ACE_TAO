ACE_INLINE
CDR::CDR (u_char *buf = 0,
          u_int len = 0,
          int byte_order = MY_BYTE_SEX,
          int consume_buf = 0) 
  // Constructor ... buffer must be aligned for the strictest CDR
  // alignment requirement, since the algorithms used here only
  // maintain alignment with respect to &buffer [0].
  //
  // Yes, that complicates the grow() primitive.
  : real_buffer (buf),
    do_free (consume_buf),
    do_byteswap (byte_order != MY_BYTE_SEX)
{
  ptr_arith_t temp = (ptr_arith_t) buf;

  temp += MAX_ALIGNMENT - 1;
  temp &= ~((ptr_arith_t) MAX_ALIGNMENT - 1);
  buffer = next = (u_char *) temp;

  remaining = len - (u_int) (buffer - real_buffer);
  length = remaining;
}

ACE_INLINE
CDR::~CDR (void)
{
  if (do_free)
    delete real_buffer;
}

ACE_INLINE void *
CDR::operator new (size_t, void *_FAR p)
{
  return p;
}

ACE_INLINE void *
CDR::operator new (size_t s)
{
  return ::operator new (s);
}

ACE_INLINE void
CDR::operator delete (void *p)
{
  ::operator delete (p);
}

ACE_INLINE CORBA_Boolean
CDR::put_char (CORBA_Char c)
{
  return put_byte ((char) c);
}

ACE_INLINE CORBA_Boolean
CDR::put_wchar (CORBA_WChar wc)
{
  // "wchar_t" isn't always 2 bytes, such systems might need further
  // conversion (e.g. hosts with multibyte characters native, rather
  // than UNICODE)

  return put_short ((short) wc);
}
    
ACE_INLINE CORBA_Boolean
CDR::put_boolean (CORBA_Boolean b)
{
  return put_byte ((char) (b != CORBA_B_FALSE));
}

ACE_INLINE CORBA_Boolean
CDR::put_octet (CORBA_Octet o)
{
  return put_byte ((char) o);
}

ACE_INLINE CORBA_Boolean
CDR::put_ushort (CORBA_UShort s)
{
  return put_short ((CORBA_Short) s);
}

ACE_INLINE CORBA_Boolean
CDR::put_ulong (CORBA_ULong l)
{
  return put_long ((CORBA_Long) l);
}

ACE_INLINE CORBA_Boolean
CDR::put_ulonglong (const CORBA_ULongLong &ll)
{
  return put_longlong ((CORBA_LongLong &) ll);
}
				    
ACE_INLINE CORBA_Boolean
CDR::put_float (float f)
{
  return put_long (*(CORBA_Long *) &f);
}

ACE_INLINE CORBA_Boolean
CDR::put_double (const double &d)
{
  return put_longlong (*(CORBA_LongLong *) &d);
}

ACE_INLINE CORBA_Boolean
CDR::get_char (CORBA_Char &o)
{
  return this->get_byte ((char &) o);
}

ACE_INLINE CORBA_Boolean
CDR::get_wchar (CORBA_WChar &wc)
{
  short s;

  // wchar_t isn't always "short"

  CORBA_Boolean retval = this->get_short (s);
  wc = s;
  return retval;
}

ACE_INLINE CORBA_Boolean
CDR::get_boolean (CORBA_Boolean &b)
{
  CORBA_Char c;

  //
  // CORBA_Boolean is rarely 'char'
  //
  CORBA_Boolean retval = this->get_char (c);
  b = (c == 1);
  return retval;
}

ACE_INLINE CORBA_Boolean
CDR::get_octet (CORBA_Octet &o)
{
  return this->get_byte ((char &) o);
}

ACE_INLINE CORBA_Boolean
CDR::get_ushort (CORBA_UShort &s)
{
  return this->get_short ((short&) s);
}

ACE_INLINE CORBA_Boolean
CDR::get_ulong (CORBA_ULong &l)
{
  return this->get_long ((CORBA_Long &) l);
}

ACE_INLINE CORBA_Boolean
CDR::get_ulonglong (const CORBA_ULongLong &ull)
{
  return this->get_longlong ((CORBA_LongLong &) ull);
}

ACE_INLINE CORBA_Boolean
CDR::get_float (float &f)
{
  return this->get_long ((CORBA_Long &) f);
}

ACE_INLINE CORBA_Boolean
CDR::get_double (double &d)
{
  return this->get_longlong ((CORBA_LongLong &) d);
}

ACE_INLINE CORBA_Boolean
CDR::skip_bytes (u_int nbytes)
{
  if (remaining < nbytes)
    return CORBA_B_FALSE;
  remaining -= nbytes;
  next += nbytes;
  return CORBA_B_TRUE;
}

ACE_INLINE void
CDR::setup_encapsulation (u_char *buf, u_int len)
{
  // Also used when interpreting typecodes, but more generally when
  // getting ready to read from encapsulations.  In such cases the
  // buffer alignment guarantees must be provided by the caller, this
  // code doesn't verify them.  These streams are "read only".
  next = buf + 1;
  remaining = len - 1;
  do_byteswap = (*buf != MY_BYTE_SEX);
  do_free = 0;
}

ACE_INLINE size_t
CDR::bytes_remaining (void)
{
  return remaining;
}
