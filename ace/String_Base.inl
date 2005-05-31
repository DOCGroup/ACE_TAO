// -*- C++ -*-
//
// $Id$


#include "ace/Malloc_Base.h"
#include "ace/Min_Max.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_Memory.h"


template <class CHAR> ACE_INLINE void
ACE_String_Base<CHAR>::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_String_Base<CHAR>::dump");
#endif /* ACE_HAS_DUMP */
}

// Assignment method (does not copy memory)
template <class CHAR> ACE_INLINE ACE_String_Base<CHAR> &
ACE_String_Base<CHAR>::assign_nocopy (const ACE_String_Base<CHAR> &s)
{
  ACE_TRACE ("ACE_String_Base<CHAR>::assign_nocopy");
  this->set (s.rep_, s.len_, 0);
  return *this;
}

template <class CHAR> ACE_INLINE size_t
ACE_String_Base<CHAR>::length (void) const
{
  ACE_TRACE ("ACE_String_Base<CHAR>::length");
  return this->len_;
}

template <class CHAR> ACE_INLINE ACE_String_Base<CHAR>
ACE_String_Base<CHAR>::substr (size_t offset,
                               ssize_t length) const
{
  ACE_TRACE ("ACE_String_Base<CHAR>::substr");
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

// Less than comparison operator.

template <class CHAR> ACE_INLINE bool
ACE_String_Base<CHAR>::operator < (const ACE_String_Base<CHAR> &s) const
{
  ACE_TRACE ("ACE_String_Base<CHAR>::operator <");
  return compare (s) < 0;
}

// Greater than comparison operator.

template <class CHAR> ACE_INLINE bool
ACE_String_Base<CHAR>::operator > (const ACE_String_Base &s) const
{
  ACE_TRACE ("ACE_String_Base<CHAR>::operator >");
  return compare (s) > 0;
}


// Comparison operator.

template <class CHAR> ACE_INLINE bool
ACE_String_Base<CHAR>::operator!= (const ACE_String_Base<CHAR> &s) const
{
  ACE_TRACE ("ACE_String_Base<CHAR>::operator!=");
  return !(*this == s);
}

template <class CHAR> ACE_INLINE ssize_t
ACE_String_Base<CHAR>::find (const ACE_String_Base<CHAR>&str, size_t pos) const
{
  ACE_TRACE ("ACE_String_Base<CHAR>::find");
  return this->find (str.rep_, pos);
}

template <class CHAR> ACE_INLINE ssize_t
ACE_String_Base<CHAR>::strstr (const ACE_String_Base<CHAR> &s) const
{
  ACE_TRACE ("ACE_String_Base<CHAR>::strstr");
  return this->find (s.rep_);
}

