ACE_INLINE
CDR::CDR (u_char *buf,
          u_int len,
          int byte_order,
          int consume_buf,
	  TAO_MarshalFactory *f)
  // Constructor ... buffer must be aligned for the strictest CDR
  // alignment requirement, since the algorithms used here only
  // maintain alignment with respect to &buffer [0].
  //
  // Yes, that complicates the grow() primitive.
  : real_buffer(buf),
    do_free(consume_buf),
    do_byteswap(byte_order != MY_BYTE_SEX),
    factory_(f),
    mobj_(0)
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
CDR::put_byte (
    char	c
)
{
    if (remaining < sizeof (char) && grow (0) == CORBA_B_FALSE)
	return CORBA_B_FALSE;

    *next++ = (unsigned char) c;
    remaining--;
    return CORBA_B_TRUE;
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
  if (remaining >= nbytes)
    {
      remaining -= nbytes;
      next += nbytes;
      return CORBA_B_TRUE;
    }
  return CORBA_B_FALSE;
}

ACE_INLINE CORBA_Boolean
CDR::skip_string ()			// ISO/1 or octet string
{
  CORBA_ULong	len;

  register unsigned char	*tmp_next;
  register unsigned		temp;

  //
  // Adjust pointer and count of remaining bytes
  //
  tmp_next = ptr_align_binary (next, LONG_SIZE);
  temp = LONG_SIZE + (tmp_next - next);
  if (temp <= remaining) {
    remaining -= temp;

    //
    // decode word, swapping as needed
    //
    if (!do_byteswap) {
      len =  *(CORBA_Long *)tmp_next;
      next = tmp_next + LONG_SIZE;
    } else {
      swap_long(tmp_next, *(CORBA_Long *)&len);
    }

    // skip that many bytes
    next += (unsigned) len;
    remaining -= (unsigned) len;
    return CORBA_B_TRUE;
  }
  return CORBA_B_FALSE;
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

ACE_INLINE CORBA_Boolean
CDR::put_short (
		CORBA_Short		s
		)
{
  register unsigned char	*tmp_next;
  register unsigned		temp;

  //
  // Adjust pointer and count of remaining bytes; maybe
  // grow the buffer if there's not enough left
  //
  tmp_next = ptr_align_binary (next, SHORT_SIZE);
  temp = SHORT_SIZE + (tmp_next - next);
  if (temp <= remaining) {
    remaining -= temp;

    //
    // copy the half word, native byte order
    //
    *(CORBA_Short *)tmp_next = s;
    next = tmp_next + SHORT_SIZE;
    return CORBA_B_TRUE;
  } else {
    if (grow (0) == CORBA_B_TRUE) {
      tmp_next = next + temp - SHORT_SIZE;
      remaining -= temp;

      //
      // copy the half word, native byte order
      //
      *(CORBA_Short *)tmp_next = s;
      next = tmp_next + SHORT_SIZE;
      return CORBA_B_TRUE;
    }
    return CORBA_B_FALSE;
  }
}

ACE_INLINE CORBA_Boolean
CDR::put_long (
	       CORBA_Long		l
	       )
{
  register unsigned char	*tmp_next;
  register unsigned		temp;

  //
  // Adjust pointer and count of remaining bytes; maybe
  // grow the buffer if there's not enough left
  //
  tmp_next = ptr_align_binary (next, LONG_SIZE);
  temp = LONG_SIZE + (tmp_next - next);
  if (temp <= remaining) 
    {
      remaining -= temp;

      //
      // copy the word, native byte order
      //
      *(CORBA_Long *)tmp_next =  l;

      next = tmp_next + LONG_SIZE;
      return CORBA_B_TRUE;
    } 
  else 
    {
      if (grow (0) == CORBA_B_TRUE)
	{
	  tmp_next = next + temp - LONG_SIZE;
	  remaining -= temp;

	  //
	  // copy the word, native byte order
	  //
	  *(CORBA_Long *)tmp_next =  l;

	  next = tmp_next + LONG_SIZE;
	  return CORBA_B_TRUE;
	}
      return CORBA_B_FALSE;
    }
}

ACE_INLINE CORBA_Boolean
CDR::put_longlong (
		   const CORBA_LongLong	&ll
		   )
{
  register unsigned char	*tmp_next;
  register unsigned		temp;

  //
  // Adjust pointer and count of remaining bytes; maybe
  // grow the buffer if there's not enough left
  //
  tmp_next = ptr_align_binary (next, LONGLONG_SIZE);
  temp = LONGLONG_SIZE + (tmp_next - next);
  if (temp <= remaining)
    {
      remaining -= temp;
      //
      // copy the double word in "native" byte order.
      //
      *(CORBA_LongLong *)tmp_next = ll;
      next = tmp_next + LONGLONG_SIZE;
      return CORBA_B_TRUE;
    }
  else 
    {
      if (grow (0) == CORBA_B_TRUE)
	{
	  tmp_next = next + temp - LONGLONG_SIZE;
	  remaining -= temp;
	  //
	  // copy the double word in "native" byte order.
	  //
	  *(CORBA_LongLong *)tmp_next = ll;
	  next = tmp_next + LONGLONG_SIZE;
	  return CORBA_B_TRUE;
	}
      return CORBA_B_FALSE;
    }
}

ACE_INLINE CORBA_Boolean
CDR::put_longdouble (
		     CORBA_LongDouble	&ld
		     )
{
  register unsigned char	*tmp_next;
  register unsigned		temp;

  //
  // Adjust pointer and count of remaining bytes; maybe
  // grow the buffer if there's not enough left
  //
  tmp_next = ptr_align_binary (next, LONGDOUBLE_SIZE);
  temp = LONGDOUBLE_SIZE + (tmp_next - next);
  if (temp <= remaining)
    {
      remaining -= temp;
      //
      // copy the longdouble in native byte order
      //
      *(CORBA_LongDouble *)tmp_next = ld;
      next = tmp_next + LONGDOUBLE_SIZE;
      return CORBA_B_TRUE;
    }
  else 
    {
      if (grow (0) == CORBA_B_TRUE)
	{
	  tmp_next = next + temp - LONGDOUBLE_SIZE;
	  remaining -= temp;
	  //
	  // copy the longdouble in native byte order
	  //
	  *(CORBA_LongDouble *)tmp_next = ld;
	  next = tmp_next + LONGDOUBLE_SIZE;
	  return CORBA_B_TRUE;
	}
      return CORBA_B_FALSE;
    }
}


//
// DECODING routines ... adjust pointer, then byteswap as needed.
//

ACE_INLINE CORBA_Boolean
CDR::get_byte (
	       char	&c
	       )
{
  if (remaining >= sizeof (char)) 
    {
      c = (char) *next++;
      remaining--;
      return CORBA_B_TRUE;
    }
  return CORBA_B_FALSE;
}

ACE_INLINE CORBA_Boolean
CDR::get_short (
		CORBA_Short		&s
		)
{
  register unsigned char	*tmp_next;
  register unsigned		temp;

  //
  // Adjust pointer and count of remaining bytes
  //
  tmp_next = ptr_align_binary (next, SHORT_SIZE);
  temp = SHORT_SIZE + (tmp_next - next);
  if (temp <= remaining) {
    remaining -= temp;
    //
    // decode halfword, swapping as needed
    //
    if (!do_byteswap) {
      s = *(CORBA_Short *)tmp_next;
      next = tmp_next + SHORT_SIZE;
      return CORBA_B_TRUE;  // put a return here to avoid a jump
    } else {
      // do swapping
      register unsigned char	*sp = (unsigned char *) &s;

      sp [1] = *tmp_next++;
      sp [0] = *tmp_next++;
      next = tmp_next;
      return CORBA_B_TRUE;
    }
  }
  return CORBA_B_FALSE;
}

ACE_INLINE CORBA_Boolean
CDR::get_long (
	       CORBA_Long		&l
	       )
{
  register unsigned char	*tmp_next;
  register unsigned		temp;

  //
  // Adjust pointer and count of remaining bytes
  //
  tmp_next = ptr_align_binary (next, LONG_SIZE);
  temp = LONG_SIZE + (tmp_next - next);
  if (temp <= remaining) {
    remaining -= temp;
    //
    // decode word, swapping as needed
    //
    if (!do_byteswap) {
      l =  *(CORBA_Long *)tmp_next;
      next = tmp_next + LONG_SIZE;
      return CORBA_B_TRUE;
    } else {
      //
      // NOTE:  environment-specific speedups abound for this kind
      // of stuff.  This generic code takes advantage of none of them.
      //
      swap_long(tmp_next, l);
      next = tmp_next + LONG_SIZE;
      return CORBA_B_TRUE;
    }
  }
  return CORBA_B_FALSE;
}

ACE_INLINE CORBA_Boolean
CDR::get_longlong (
		   CORBA_LongLong		&ll
		   )
{
  register unsigned char	*tmp_next;
  register unsigned		temp;

  //
  // Adjust pointer and count of remaining bytes
  //
  tmp_next = ptr_align_binary (next, LONGLONG_SIZE);
  temp = LONGLONG_SIZE + (tmp_next - next);
  if (temp <= remaining) {
    remaining -= temp;
    //
    // decode doubleword, swapping as needed
    //
    if (!do_byteswap) {
      ll = *(CORBA_LongLong *)tmp_next;
      next = tmp_next + LONGLONG_SIZE;
      return CORBA_B_TRUE;
    } else {
      swap_longlong(tmp_next, ll);
      next = tmp_next + LONGLONG_SIZE;
      return CORBA_B_TRUE;
    }
  }
  return CORBA_B_FALSE;
}

ACE_INLINE CORBA_Boolean
CDR::get_longdouble (
		     CORBA_LongDouble		&ld
		     )
{
  register unsigned char	*tmp_next;
  register unsigned		temp;

  //
  // Adjust pointer and count of remaining bytes
  //
  tmp_next = ptr_align_binary (next, LONGDOUBLE_SIZE);
  temp = LONGDOUBLE_SIZE + (tmp_next - next);
  if (temp <= remaining) {
    remaining -= temp;

    //
    // copy the long double, swapping bytes as needed
    //
    if (!do_byteswap) {
      ld = *(CORBA_LongDouble *)tmp_next;
      next = tmp_next + LONGDOUBLE_SIZE;
      return CORBA_B_TRUE;
    } else {
      swap_longdouble(tmp_next, ld);
      next = tmp_next + LONGDOUBLE_SIZE;
      return CORBA_B_TRUE;
    }
  }
  return CORBA_B_FALSE;
}

// Encode data into CDR stream
ACE_INLINE CORBA_TypeCode::traverse_status
CDR::encode (CORBA_TypeCode_ptr tc,
	     const void *data,
	     const void *data2,
	     CORBA_Environment &env)
{
  this->mobj_ = this->factory_->make_marshal_object(tc);
  if (this->mobj_)
    {
      return this->mobj_->encode(tc, data, data2, this, env);
    }
  else
    {
      env.exception(new CORBA_BAD_TYPECODE(COMPLETED_NO));
      return CORBA_TypeCode::TRAVERSE_STOP;
    }
}

// Decode the CDR stream
ACE_INLINE CORBA_TypeCode::traverse_status
CDR::decode (CORBA_TypeCode_ptr tc,
	     const void *data,
	     const void *data2,
	     CORBA_Environment &env)
{
  this->mobj_ = this->factory_->make_marshal_object(tc);
  if (this->mobj_)
    {
      return this->mobj_->decode(tc, data, data2, this, env);
    }
  else
    {
      env.exception(new CORBA_BAD_TYPECODE(COMPLETED_NO));
      return CORBA_TypeCode::TRAVERSE_STOP;
    }
}

#if 0
// allocate buffers and make a deep copy
ACE_INLINE CORBA_TypeCode::traverse_status
CDR::deep_copy (CORBA_TypeCode_ptr tc,
		const void *data,
		const void *data2,
		CORBA_Environment &env)
{
  this->mobj_ = this->factory_->make_marshal_object(tc);
  return this->mobj_->deep_copy(tc, data, data2, this, env);
}

// free the buffers allocated for the specified data structure
ACE_INLINE CORBA_TypeCode::traverse_status
CDR::deep_free (CORBA_TypeCode_ptr tc,
		const void *data,
		const void *data2,
		CORBA_Environment &env)
{
  this->mobj_ = this->factory_->make_marshal_object(tc);
  return this->mobj_->deep_free(tc, data, data2, this, env);
}
#endif



