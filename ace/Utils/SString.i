/* -*- C++ -*- */
// $Id$

#include "ace/Memory/Malloc_Base.h"

ACE_INLINE
ACE_NS_WString::ACE_NS_WString (ACE_Allocator *alloc)
  : ACE_WString (alloc)
{
}

ACE_INLINE
ACE_NS_WString::ACE_NS_WString (const ACE_WSTRING_TYPE *s,
                                size_t len,
                                ACE_Allocator *alloc)
  : ACE_WString (s, len, alloc)
{
}

ACE_INLINE
ACE_NS_WString::ACE_NS_WString (const ACE_WSTRING_TYPE *s,
                                ACE_Allocator *alloc)
  : ACE_WString (s, alloc)
{
}

ACE_INLINE
ACE_NS_WString::ACE_NS_WString (size_t len, ACE_Allocator *alloc)
  : ACE_WString (len, alloc)
{
}

ACE_INLINE
ACE_NS_WString::ACE_NS_WString (const ACE_NS_WString &s)
  : ACE_WString (s)
{
}

ACE_INLINE
ACE_NS_WString::ACE_NS_WString (ACE_WSTRING_TYPE c, ACE_Allocator *alloc)
  : ACE_WString (c, alloc)
{
}

ACE_INLINE ACE_NS_WString
operator+ (const ACE_NS_WString &s, const ACE_NS_WString &t)
{
  ACE_NS_WString temp (s);
  temp += t;
  return temp;
}

ACE_INLINE
ACE_SString::~ACE_SString (void)
{
}

ACE_INLINE ACE_SString
ACE_SString::substr (size_t offset,
                     ssize_t length) const
{
  return this->substring (offset, length);
}

// Return the <slot'th> character in the string.

ACE_INLINE char
ACE_SString::operator[] (size_t slot) const
{
  ACE_TRACE ("ACE_SString::operator[]");
  return this->rep_[slot];
}

// Return the <slot'th> character in the string by reference.

ACE_INLINE char &
ACE_SString::operator[] (size_t slot)
{
  ACE_TRACE ("ACE_SString::operator[]");
  return this->rep_[slot];
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

// Less than comparison operator.

ACE_INLINE int
ACE_SString::operator < (const ACE_SString &s) const
{
  ACE_TRACE ("ACE_SString::operator <");

  return (this->rep_ && s.rep_)
    ? ACE_OS::strcmp (this->rep_, s.rep_) < 0
    : ((s.rep_) ? 1 : 0 );
}

// Greater than comparison operator.

ACE_INLINE int
ACE_SString::operator > (const ACE_SString &s) const
{
  ACE_TRACE ("ACE_SString::operator >");

  return (this->rep_ && s.rep_)
    ? ACE_OS::strcmp (this->rep_, s.rep_) > 0
    : ((this->rep_) ? 1 : 0 );
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
  ACE_TRACE ("ACE_SString::compare");
  return ACE_OS::strcmp (this->rep_, s.rep_);
}

ACE_INLINE int
ACE_SString::find (const char *s, int pos) const
{
  char *substr = this->rep_ + pos;
  char *pointer = ACE_OS::strstr (substr, s);
  if (pointer == 0)
    return ACE_SString::npos;
  else
    return pointer - this->rep_;
}

ACE_INLINE int
ACE_SString::find (char c, int pos) const
{
  char *substr = this->rep_ + pos;
  char *pointer = ACE_OS::strchr (substr, c);
  if (pointer == 0)
    return ACE_SString::npos;
  else
    return pointer - this->rep_;
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
ACE_SString::rfind (char c, int pos) const
{
  if (pos == ACE_SString::npos)
    pos = this->len_;

  for (int i = pos - 1; i >= 0; i--)
    if (this->rep_[i] == c)
      return i;

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

ACE_INLINE
ACE_Auto_String_Free::ACE_Auto_String_Free (char* p)
  :  p_ (p)
{
}

ACE_INLINE
ACE_Auto_String_Free::ACE_Auto_String_Free (ACE_Auto_String_Free& rhs)
  :  p_ (rhs.p_)
{
  rhs.p_ = 0;
}

ACE_INLINE void
ACE_Auto_String_Free::reset (char* p)
{
  if (this->p_ != 0)
    ACE_OS::free (this->p_);
  this->p_ = p;
}

ACE_INLINE ACE_Auto_String_Free&
ACE_Auto_String_Free::operator= (ACE_Auto_String_Free& rhs)
{
  if (this != &rhs)
    {
      this->reset (rhs.p_);
      rhs.p_ = 0;
    }
  return *this;
}

ACE_INLINE
ACE_Auto_String_Free::~ACE_Auto_String_Free (void)
{
  this->reset (0);
}

ACE_INLINE char*
ACE_Auto_String_Free::operator* (void) const
{
  return this->p_;
}

ACE_INLINE char
ACE_Auto_String_Free::operator[] (int i) const
{
  return this->p_[i];
}

ACE_INLINE char*
ACE_Auto_String_Free::get (void) const
{
  return this->p_;
}

ACE_INLINE char*
ACE_Auto_String_Free::release (void)
{
  char* p = this->p_;
  this->p_ = 0;
  return p;
}
