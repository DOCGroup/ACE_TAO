/* -*- C++ -*- */
// $Id$

ACE_INLINE size_t
ACE_CString::length (void) const
{
  ACE_TRACE ("ACE_CString::length");
  return this->len_;
}

ACE_INLINE ACE_CString
ACE_CString::substr (size_t offset,
                     ssize_t length) const
{
  return this->substring (offset, length);
}

// Return the <index'th> character in the string.

ACE_INLINE char
ACE_CString::operator[] (size_t index) const
{
  ACE_TRACE ("ACE_CString::operator[]");
  return this->rep_[index];
}

// Return the <index'th> character in the string by reference.

ACE_INLINE char &
ACE_CString::operator[] (size_t index)
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

ACE_INLINE const char *
ACE_CString::c_str (void) const
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

ACE_INLINE int
ACE_CString::strstr (const ACE_CString &s) const
{
  ACE_TRACE ("ACE_CString::strstr");

  return this->find (s.rep_);
}

ACE_INLINE int
ACE_CString::find (const ACE_CString &str, int pos) const
{
  return this->find (str.rep_, pos);
}

ACE_INLINE int
ACE_CString::find (const char *s, int pos) const
{
  char *substr = this->rep_ + pos;
  char *pointer = ACE_OS::strstr (substr, s);
  if (pointer == NULL)
    return ACE_CString::npos;
  else
    return pointer - substr;
}

ACE_INLINE int
ACE_CString::find (char c, int pos) const
{
  char *substr = this->rep_ + pos;
  char *pointer = ACE_OS::strchr (substr, c);
  if (pointer == NULL)
    return ACE_CString::npos;
  else
    return pointer - substr;
}

ACE_INLINE int
ACE_CString::rfind (char c, int pos) const
{
  if (pos == ACE_CString::npos)
    pos = this->len_;

  for (int i = pos - 1; i >= 0; i--)
    {
      if (this->rep_[i] == c)
        return i;
    }

  return ACE_CString::npos;
}

ACE_INLINE u_long
ACE_CString::hash (void) const
{
  return ACE::hash_pjw (this->rep_);
}

ACE_INLINE ACE_SString
ACE_SString::substr (size_t offset,
                     ssize_t length) const
{
  return this->substring (offset, length);
}

// Return the <index'th> character in the string.

ACE_INLINE char
ACE_SString::operator[] (size_t index) const
{
  ACE_TRACE ("ACE_SString::operator[]");
  return this->rep_[index];
}

// Return the <index'th> character in the string by reference.

ACE_INLINE char &
ACE_SString::operator[] (size_t index)
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

// Get the underlying pointer (does not make a copy, so beware!).

ACE_INLINE const char *
ACE_SString::fast_rep (void) const
{
  ACE_TRACE ("ACE_SString::fast_rep");
  return this->rep_;
}

// Get the underlying pointer (does not make a copy, so beware!).

ACE_INLINE const char *
ACE_SString::c_str (void) const
{
  ACE_TRACE ("ACE_SString::c_str");
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

ACE_INLINE int
ACE_SString::strstr (const ACE_SString &s) const
{
  ACE_TRACE ("ACE_SString::strstr");

  return this->find (s.rep_);
}

ACE_INLINE int
ACE_SString::find (const ACE_SString &str, int pos) const
{
  return this->find (str.rep_, pos);
}

ACE_INLINE int
ACE_SString::find (const char *s, int pos) const
{
  char *substr = this->rep_ + pos;
  char *pointer = ACE_OS::strstr (substr, s);
  if (pointer == NULL)
    return ACE_SString::npos;
  else
    return pointer - substr;
}

ACE_INLINE int
ACE_SString::find (char c, int pos) const
{
  char *substr = this->rep_ + pos;
  char *pointer = ACE_OS::strchr (substr, c);
  if (pointer == NULL)
    return ACE_SString::npos;
  else
    return pointer - substr;
}

ACE_INLINE int
ACE_SString::rfind (char c, int pos) const
{
  if (pos == ACE_SString::npos)
    pos = this->len_;

  for (int i = pos - 1; i >= 0; i--)
    {
      if (this->rep_[i] == c)
        return i;
    }

  return ACE_SString::npos;
}

ACE_INLINE u_long
ACE_SString::hash (void) const
{
  return ACE::hash_pjw (this->rep_);
}

ACE_INLINE size_t
ACE_SString::length (void) const
{
  ACE_TRACE ("ACE_SString::length");
  return this->len_;
}

ACE_INLINE ACE_WString
ACE_WString::substr (size_t offset,
                     ssize_t length) const
{
  return this->substring (offset, length);
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

ACE_INLINE const ACE_USHORT16 *
ACE_WString::c_str (void) const
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

// Return the <index'th> character in the string.

ACE_INLINE ACE_USHORT16 &
ACE_WString::operator[] (size_t index)
{
  ACE_TRACE ("ACE_WString::operator[]");
  return this->rep_[index];
}

ACE_INLINE int
ACE_WString::strstr (const ACE_WString &s) const
{
  ACE_TRACE ("ACE_WString::strstr");

  return this->find (s.rep_);
}

ACE_INLINE int
ACE_WString::find (const ACE_WString &str, int pos) const
{
  return this->find (str.rep_, pos);
}

ACE_INLINE int
ACE_WString::find (const ACE_USHORT16 *s, int pos) const
{
  ACE_USHORT16 *substr = this->rep_ + pos;
  const ACE_USHORT16 *pointer = ACE_WString::strstr (substr, s);
  if (pointer == NULL)
    return ACE_WString::npos;
  else
    return pointer - substr;
}

ACE_INLINE int
ACE_WString::find (ACE_USHORT16 c, int pos) const
{
  if (pos == ACE_WString::npos)
    pos = this->len_;

  for (size_t i = pos; i < this->len_; i++)
    {
      if (this->rep_[i] == c)
        return ACE_static_cast (int, i);
    }

  return ACE_WString::npos;
}

ACE_INLINE int
ACE_WString::rfind (ACE_USHORT16 c, int pos) const
{
  if (pos == ACE_WString::npos)
    pos = this->len_;

  for (int i = pos - 1; i >= 0; i--)
    {
      if (this->rep_[i] == c)
        return i;
    }

  return ACE_WString::npos;
}

ACE_INLINE size_t
ACE_WString::length (void) const
{
  ACE_TRACE ("ACE_WString::length");
  return this->len_;
}

ACE_INLINE u_long
ACE_WString::hash (void) const
{
  return ACE::hash_pjw (this->rep_);
}
