/* -*- C++ -*- */
// $Id$

// SString.i

ACE_INLINE u_long
ACE_CString::hash (void) const
{
  return ACE::hash_pjw (this->rep_);
}

ACE_INLINE u_long
ACE_SString::hash (void) const
{
  return ACE::hash_pjw (this->rep_);
}

ACE_INLINE u_long
ACE_WString::hash (void) const
{
  return ACE::hash_pjw (this->rep_);
}

// Return the <index'th> character in the string.

ACE_INLINE char 
ACE_CString::operator[] (size_t index) const
{
  ACE_TRACE ("ACE_CString::operator[]");
  return this->rep_[index];
}

// Get a copy of the underlying representation.

ACE_INLINE char *
ACE_CString::rep (void) const
{
  ACE_TRACE ("ACE_CString::rep");
  
  char *new_string;
  ACE_NEW_RETURN (new_string, char[this->len_ + 1], 0);
  ACE_OS::strcpy (new_string, this->rep_);

  return new_string;
}

ACE_INLINE const char *
ACE_CString::fast_rep (void) const
{
  return this->rep_;
}

// Comparison operator.

ACE_INLINE int 
ACE_CString::operator== (const ACE_CString &s) const
{
  ACE_TRACE ("ACE_CString::operator==");

  return this->len_ == s.len_
    && ACE_OS::strcmp (this->rep_, s.rep_) == 0;
}

// Comparison operator.

ACE_INLINE int 
ACE_CString::operator!= (const ACE_CString &s) const
{
  ACE_TRACE ("ACE_CString::operator!=");
  return !(*this == s);
}

ACE_INLINE int 
ACE_CString::compare (const ACE_CString &s) const
{
  ACE_TRACE ("ACE_CString::compare");
  return ACE_OS::strcmp (this->rep_, s.rep_);
}

// Return the <index'th> character in the string.

ACE_INLINE char 
ACE_SString::operator[] (size_t index) const
{
  ACE_TRACE ("ACE_SString::operator[]");
  return this->rep_[index];
}

// Get the underlying pointer (does not make a copy, so beware!).

ACE_INLINE const char *
ACE_SString::rep (void) const
{
  ACE_TRACE ("ACE_SString::rep");
  return this->rep_;
}

// Comparison operator.

ACE_INLINE int 
ACE_SString::operator== (const ACE_SString &s) const
{
  ACE_TRACE ("ACE_SString::operator==");
  return this->len_ == s.len_
    && ACE_OS::strcmp (this->rep_, s.rep_) == 0;
}

// Comparison operator.

ACE_INLINE int 
ACE_SString::operator!= (const ACE_SString &s) const
{
  ACE_TRACE ("ACE_SString::operator!=");
  return !(*this == s);
}

ACE_INLINE int 
ACE_SString::compare (const ACE_SString &s) const
{
  ACE_TRACE ("ACE_CString::compare");
  return ACE_OS::strcmp (this->rep_, s.rep_);
}

// Get a copy of the underlying representation.

ACE_INLINE ACE_USHORT16 *
ACE_WString::rep (void) const
{
  ACE_TRACE ("ACE_WString::rep");
  if (this->len_ <= 0)
    return 0;
  else
    {
      ACE_USHORT16 *t;
      ACE_NEW_RETURN (t, ACE_USHORT16[this->len_ + 1], 0);
      ACE_OS::memcpy (t, this->rep_, this->len_ * sizeof (ACE_USHORT16));

      // null terminate 
      t[this->len_] = 0;

      return t;
    }
}

// Get at the underlying representation directly!

ACE_INLINE const ACE_USHORT16 *
ACE_WString::fast_rep (void) const
{
  return this->rep_;
}

// Comparison operator.

ACE_INLINE int 
ACE_WString::operator== (const ACE_WString &s) const
{
  ACE_TRACE ("ACE_WString::operator==");
  return this->len_ == s.len_
    && ACE_OS::memcmp ((const void *) this->rep_, 
		       (const void *) s.rep_,
		       this->len_ * sizeof (ACE_USHORT16)) == 0;
}

// Comparison operator.

ACE_INLINE int 
ACE_WString::operator!= (const ACE_WString &s) const
{
  ACE_TRACE ("ACE_WString::operator!=");
  return !(*this == s);
}

ACE_INLINE int 
ACE_WString::compare (const ACE_WString &s) const
{
  ACE_TRACE ("ACE_WString::compare");

  return ACE_OS::memcmp ((const void *) this->rep_, 
			 (const void *) s.rep_,
			 this->len_ * sizeof (ACE_USHORT16));
}

// Return the <index'th> character in the string.

ACE_INLINE ACE_USHORT16
ACE_WString::operator[] (size_t index) const
{
  ACE_TRACE ("ACE_WString::operator[]");
  return this->rep_[index];
}
