// -*- C++ -*-
//
// $Id$


#include "ace/Malloc_Base.h"
#include "ace/Min_Max.h"

// Default constructor.

template <class CHAR> ACE_INLINE
ACE_String_Base<CHAR>::ACE_String_Base (ACE_Allocator *the_allocator)
  : allocator_ (the_allocator ? the_allocator : ACE_Allocator::instance ()),
    len_ (0),
    buf_len_ (0),
    rep_ (&ACE_String_Base<CHAR>::NULL_String_),
    release_ (0)
{
  ACE_TRACE ("ACE_String_Base<CHAR>::ACE_String_Base");
}

// Constructor that actually copies memory.

template <class CHAR> ACE_INLINE
ACE_String_Base<CHAR>::ACE_String_Base (const CHAR *s,
                                        ACE_Allocator *the_allocator,
                                        int release)
  : allocator_ (the_allocator ? the_allocator : ACE_Allocator::instance ()),
    len_ (0),
    buf_len_ (0),
    rep_ (0),
    release_ (0)
{
  ACE_TRACE ("ACE_String_Base<CHAR>::ACE_String_Base");

  size_t length;
  if (s != 0)
    length = ACE_OS_String::strlen (s);
  else
    length = 0;

  this->set (s, length, release);
}

template <class CHAR> ACE_INLINE
ACE_String_Base<CHAR>::ACE_String_Base (CHAR c,
                                        ACE_Allocator *the_allocator)
  : allocator_ (the_allocator ? the_allocator : ACE_Allocator::instance ()),
    len_ (0),
    buf_len_ (0),
    rep_ (0),
    release_ (0)
{
  ACE_TRACE ("ACE_String_Base<CHAR>::ACE_String_Base");

  this->set (&c, 1, 1);
}

// Constructor that actually copies memory.

template <class CHAR> ACE_INLINE
ACE_String_Base<CHAR>::ACE_String_Base (const CHAR *s,
                                        size_t len,
                                        ACE_Allocator *the_allocator,
                                        int release)
  : allocator_ (the_allocator ? the_allocator : ACE_Allocator::instance ()),
    len_ (0),
    buf_len_ (0),
    rep_ (0),
    release_ (0)
{
  ACE_TRACE ("ACE_String_Base<CHAR>::ACE_String_Base");

  this->set (s, len, release);
}

// Copy constructor.

template <class CHAR> ACE_INLINE
ACE_String_Base<CHAR>::ACE_String_Base (const ACE_String_Base<CHAR> &s)
  : allocator_ (s.allocator_ ? s.allocator_ : ACE_Allocator::instance ()),
    len_ (0),
    buf_len_ (0),
    rep_ (0),
    release_ (0)
{
  ACE_TRACE ("ACE_String_Base<CHAR>::ACE_String_Base");

  this->set (s.rep_, s.len_, 1);
}

template <class CHAR> ACE_INLINE
ACE_String_Base<CHAR>::ACE_String_Base (size_t len, CHAR c, ACE_Allocator *the_allocator)
  : allocator_ (the_allocator ? the_allocator : ACE_Allocator::instance ()),
    len_ (0),
    buf_len_ (0),
    rep_ (0),
    release_ (0)
{
  ACE_TRACE ("ACE_String_Base<CHAR>::ACE_String_Base");

  this->resize (len, c);
}

template <class CHAR> ACE_INLINE
ACE_String_Base<CHAR>::~ACE_String_Base (void)
{
  ACE_TRACE ("ACE_String_Base<CHAR>::~ACE_String_Base");

  this->set (0, 0, 0);
}

template <class CHAR> ACE_INLINE void
ACE_String_Base<CHAR>::dump (void) const
{
  ACE_TRACE ("ACE_String_Base<CHAR>::dump");
}

// Assignment operator (does copy memory).
template <class CHAR> ACE_INLINE ACE_String_Base<CHAR> &
ACE_String_Base<CHAR>::operator= (const ACE_String_Base<CHAR> &s)
{
  ACE_TRACE ("ACE_String_Base<CHAR>::operator=");

  // Check for identify.
  if (this != &s)
    {
      this->set (s.rep_, s.len_, 1);
    }

  return *this;
}

// Assignment method (does not copy memory)
template <class CHAR> ACE_INLINE ACE_String_Base<CHAR> &
ACE_String_Base<CHAR>::assign_nocopy (const ACE_String_Base<CHAR> &s)
{
  this->set (s.rep_, s.len_, 0);
  return *this;
}

template <class CHAR> ACE_INLINE void
ACE_String_Base<CHAR>::set (const CHAR *s, int release)
{
  size_t length;
  if (s != 0)
    length = ACE_OS_String::strlen (s);
  else
    length = 0;

  this->set (s, length, release);
}

template <class CHAR> ACE_INLINE size_t
ACE_String_Base<CHAR>::length (void) const
{
  ACE_TRACE ("ACE_String_Base<CHAR>::length");
  return this->len_;
}

template <class CHAR> ACE_INLINE void
ACE_String_Base<CHAR>::clear (int release)
{
  this->set(0, 0, release);
}

template <class CHAR> ACE_INLINE ACE_String_Base<CHAR>
ACE_String_Base<CHAR>::substr (size_t offset,
                               ssize_t length) const
{
  return this->substring (offset, length);
}

// Return the <slot'th> character in the string.

template <class CHAR> ACE_INLINE const CHAR &
ACE_String_Base<CHAR>::operator[] (size_t slot) const
{
  ACE_TRACE ("ACE_String_Base<CHAR>::operator[]");
  return this->rep_[slot];
}

// Return the <slot'th> character in the string by reference.

template <class CHAR> ACE_INLINE CHAR &
ACE_String_Base<CHAR>::operator[] (size_t slot)
{
  ACE_TRACE ("ACE_String_Base<CHAR>::operator[]");
  return this->rep_[slot];
}

// Get a copy of the underlying representation.

template <class CHAR> ACE_INLINE CHAR *
ACE_String_Base<CHAR>::rep (void) const
{
  ACE_TRACE ("ACE_String_Base<CHAR>::rep");

  CHAR *new_string;
  ACE_NEW_RETURN (new_string, CHAR[this->len_ + 1], 0);
  ACE_OS_String::strsncpy (new_string, this->rep_, this->len_+1);

  return new_string;
}

template <class CHAR> ACE_INLINE const CHAR *
ACE_String_Base<CHAR>::fast_rep (void) const
{
  return this->rep_;
}

template <class CHAR> ACE_INLINE const CHAR *
ACE_String_Base<CHAR>::c_str (void) const
{
  return this->rep_;
}

template <class CHAR> ACE_INLINE int
ACE_String_Base<CHAR>::compare (const ACE_String_Base<CHAR> &s) const
{
  ACE_TRACE ("ACE_String_Base<CHAR>::compare");

  // Pick smaller of the two lengths and perform the comparison.
  size_t smaller_length = ace_min (this->len_, s.len_);

  int result = ACE_OS_String::memcmp (this->rep_,
                               s.rep_,
                               smaller_length * sizeof (CHAR));

  if (!result)
    result = ACE_static_cast (int, (this->len_ - s.len_));
  return result;
}


// Comparison operator.

template <class CHAR> ACE_INLINE int
ACE_String_Base<CHAR>::operator== (const ACE_String_Base<CHAR> &s) const
{
  ACE_TRACE ("ACE_String_Base<CHAR>::operator==");

  return compare (s) == 0;
}

// Less than comparison operator.

template <class CHAR> ACE_INLINE int
ACE_String_Base<CHAR>::operator < (const ACE_String_Base<CHAR> &s) const
{
  ACE_TRACE ("ACE_String_Base<CHAR>::operator <");
  return compare (s) < 0;
}

// Greater than comparison operator.

template <class CHAR> ACE_INLINE int
ACE_String_Base<CHAR>::operator > (const ACE_String_Base &s) const
{
  ACE_TRACE ("ACE_String_Base<CHAR>::operator >");
  return compare (s) > 0;
}


// Comparison operator.

template <class CHAR> ACE_INLINE int
ACE_String_Base<CHAR>::operator!= (const ACE_String_Base<CHAR> &s) const
{
  ACE_TRACE ("ACE_String_Base<CHAR>::operator!=");
  return !(*this == s);
}

template <class CHAR> ACE_INLINE ssize_t
ACE_String_Base<CHAR>::find (const CHAR *s, size_t pos) const
{
  CHAR *substr = this->rep_ + pos;
  size_t len = ACE_OS_String::strlen (s);
  CHAR *pointer = ACE_OS_String::strnstr (substr, s, len);
  if (pointer == 0)
    return ACE_String_Base<CHAR>::npos;
  else
    return pointer - this->rep_;
}

template <class CHAR> ACE_INLINE ssize_t
ACE_String_Base<CHAR>::find (CHAR c, size_t pos) const
{
  CHAR *substr = this->rep_ + pos;
  CHAR *pointer = ACE_OS_String::strnchr (substr, c, this->len_ - pos);
  if (pointer == 0)
    return ACE_String_Base<CHAR>::npos;
  else
    return pointer - this->rep_;
}

template <class CHAR> ACE_INLINE ssize_t
ACE_String_Base<CHAR>::find (const ACE_String_Base<CHAR>&str, size_t pos) const
{
  return this->find (str.rep_, pos);
}

template <class CHAR> ACE_INLINE ssize_t
ACE_String_Base<CHAR>::strstr (const ACE_String_Base<CHAR> &s) const
{
  ACE_TRACE ("ACE_String_Base<CHAR>::strstr");

  return this->find (s.rep_);
}

template <class CHAR> ACE_INLINE ssize_t
ACE_String_Base<CHAR>::rfind (CHAR c, ssize_t pos) const
{
  if (pos == npos || pos > ACE_static_cast (ssize_t, this->len_))
    pos = ACE_static_cast (ssize_t, this->len_);

  for (ssize_t i = pos - 1; i >= 0; i--)
    if (this->rep_[i] == c)
      return i;

  return ACE_String_Base<CHAR>::npos;
}

template <class CHAR> ACE_INLINE ACE_String_Base<CHAR>
operator+ (const ACE_String_Base<CHAR> &s, const ACE_String_Base<CHAR> &t)
{
  ACE_String_Base<CHAR> temp (s);
  temp += t;
  return temp;
}

template <class CHAR> ACE_INLINE ACE_String_Base<CHAR>
operator+ (const CHAR *s, const ACE_String_Base<CHAR> &t)
{
  ACE_String_Base<CHAR> temp (s);
  temp += t;
  return temp;
}

template <class CHAR> ACE_INLINE ACE_String_Base<CHAR>
operator+ (const ACE_String_Base<CHAR> &s, const CHAR *t)
{
  ACE_String_Base<CHAR> temp (s);
  temp += t;
  return temp;
}

template <class CHAR> ACE_INLINE
ACE_String_Base<CHAR> operator + (const ACE_String_Base<CHAR> &t,
                                  const CHAR c)
{
  ACE_String_Base<CHAR> temp (t);
  temp += c;
  return temp;
}

template <class CHAR> ACE_INLINE
ACE_String_Base<CHAR> operator + (const CHAR c,
                                  const ACE_String_Base<CHAR> &t)
{
  ACE_String_Base<CHAR> temp (c);
  temp += t;
  return temp;
}
