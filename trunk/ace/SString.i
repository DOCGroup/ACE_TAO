/* -*- C++ -*- */
// $Id$

#include "ace/Malloc_Base.h"

// Default constructor.

ACE_INLINE
ACE_CString::ACE_CString (ACE_Allocator *alloc)
  : allocator_ (alloc ? alloc : ACE_Allocator::instance ()),
    len_ (0),
    buf_len_ (0),
    rep_ (&ACE_CString::NULL_CString_),
    release_ (0)
{
  ACE_TRACE ("ACE_CString::ACE_CString");
}

// Constructor that actually copies memory.

ACE_INLINE
ACE_CString::ACE_CString (const char *s,
                          ACE_Allocator *alloc,
                          int release)
  : allocator_ (alloc ? alloc : ACE_Allocator::instance ()),
    len_ (0),
    buf_len_ (0),
    rep_ (0),
    release_ (0)
{
  ACE_TRACE ("ACE_CString::ACE_CString");

  size_t length;
  if (s != 0)
    length = ACE_OS::strlen (s);
  else
    length = 0;

  this->set (s, length, release);
}

ACE_INLINE
ACE_CString::ACE_CString (char c,
                          ACE_Allocator *alloc)
  : allocator_ (alloc ? alloc : ACE_Allocator::instance ()),
    len_ (0),
    buf_len_ (0),
    rep_ (0),
    release_ (0)
{
  ACE_TRACE ("ACE_CString::ACE_CString");

  this->set (&c, 1, 1);
}

// Constructor that actually copies memory.

ACE_INLINE
ACE_CString::ACE_CString (const char *s,
                          size_t len,
                          ACE_Allocator *alloc,
                          int release)
  : allocator_ (alloc ? alloc : ACE_Allocator::instance ()),
    len_ (0),
    buf_len_ (0),
    rep_ (0),
    release_ (0)
{
  ACE_TRACE ("ACE_CString::ACE_CString");

  this->set (s, len, release);
}

// Copy constructor.

ACE_INLINE
ACE_CString::ACE_CString (const ACE_CString &s)
  : allocator_ (s.allocator_ ? s.allocator_ : ACE_Allocator::instance ()),
    len_ (0),
    buf_len_ (0),
    rep_ (0),
    release_ (0)
{
  ACE_TRACE ("ACE_CString::ACE_CString");

  this->set (s.rep_, s.len_, 1);
}

ACE_INLINE
ACE_CString::~ACE_CString (void)
{
  ACE_TRACE ("ACE_CString::~ACE_CString");

  this->set (0, 0, 0);
}

ACE_INLINE void
ACE_CString::dump (void) const
{
  ACE_TRACE ("ACE_CString::dump");
}

// Assignment operator (does copy memory).

ACE_INLINE ACE_CString &
ACE_CString::operator= (const ACE_CString &s)
{
  ACE_TRACE ("ACE_CString::operator=");

  // Check for identify.
  if (this != &s)
    this->set (s.rep_, s.len_, 1);

  return *this;
}

ACE_INLINE void
ACE_CString::set (const char *s, int release)
{
  size_t length;
  if (s != 0)
    length = ACE_OS::strlen (s);
  else
    length = 0;

  this->set (s, length, release);
}

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

ACE_INLINE const char &
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
  ACE_OS::strncpy (new_string, this->rep_, this->len_);
  new_string[this->len_] = '\0';

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
    && ACE_OS::strncmp (this->rep_, s.rep_, this->len_) == 0;
}

// Less than comparison operator.

ACE_INLINE int
ACE_CString::operator < (const ACE_CString &s) const
{
  ACE_TRACE ("ACE_CString::operator <");

  return (this->rep_ && s.rep_)
    ? ACE_OS::strcmp (this->rep_, s.rep_) < 0
    : ((s.rep_) ? 1 : 0 );
}

// Greater than comparison operator.

ACE_INLINE int
ACE_CString::operator > (const ACE_CString &s) const
{
  ACE_TRACE ("ACE_CString::operator >");

  return (this->rep_ && s.rep_)
    ? ACE_OS::strcmp (this->rep_, s.rep_) > 0
    : ((this->rep_) ? 1 : 0 );
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

  // We can't just pass both strings to strcmp, since they are not
  // guaranteed to be null-terminated.

  // Pick smaller of the two lengths and perform the comparison.
  int smaller_length = (this->len_ < s.len_) ? this->len_ : s.len_;
  int result = ACE_OS::strncmp (this->rep_,
                                s.rep_,
                                smaller_length);

  if (result != 0 || s.len_ == this->len_)
    return result;

  else
    // we need to differentiate based on length
    if (this->len_ > s.len_)
      return (this->rep_[smaller_length] - '\0');

    else
      return ('\0' - s.rep_[smaller_length]);
}

ACE_INLINE int
ACE_CString::find (const char *s, int pos) const
{
  char *substr = this->rep_ + pos;
  size_t len = ACE_OS::strlen (s);
  char *pointer = ACE_OS::strnstr (substr, s, len);
  if (pointer == 0)
    return ACE_CString::npos;
  else
    return pointer - substr;
}

ACE_INLINE int
ACE_CString::find (char c, int pos) const
{
  char *substr = this->rep_ + pos;
  char *pointer = ACE_OS::strnchr (substr, c, this->len_ - pos);
  if (pointer == 0)
    return ACE_CString::npos;
  else
    return pointer - substr;
}

ACE_INLINE int
ACE_CString::find (const ACE_CString &str, int pos) const
{
  return this->find (str.rep_, pos);
}

ACE_INLINE int
ACE_CString::strstr (const ACE_CString &s) const
{
  ACE_TRACE ("ACE_CString::strstr");

  return this->find (s.rep_);
}

ACE_INLINE int
ACE_CString::rfind (char c, int pos) const
{
  if (pos == ACE_CString::npos)
    pos = this->len_;

  for (int i = pos - 1; i >= 0; i--)
    if (this->rep_[i] == c)
      return i;

  return ACE_CString::npos;
}

ACE_INLINE u_long
ACE_CString::hash (void) const
{
  return ACE::hash_pjw (this->rep_, this->len_);
}

ACE_INLINE ACE_WString
operator+ (const ACE_WString &s, const ACE_WString &t)
{
  ACE_WString temp (s);
  temp += t;
  return temp;
}

ACE_INLINE ACE_CString
operator+ (const ACE_CString &s, const ACE_CString &t)
{
  ACE_CString temp (s);
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
    return pointer - substr;
}

ACE_INLINE int
ACE_SString::find (char c, int pos) const
{
  char *substr = this->rep_ + pos;
  char *pointer = ACE_OS::strchr (substr, c);
  if (pointer == 0)
    return ACE_SString::npos;
  else
    return pointer - substr;
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

      // 0 terminate
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

// Less than comparison operator.

ACE_INLINE int
ACE_WString::operator < (const ACE_WString &s) const
{
  ACE_TRACE ("ACE_WString::operator <");
  return (this->len_ < s.len_)
          ? (ACE_OS::memcmp ((const void *) this->rep_,
                             (const void *) s.rep_,
                             this->len_ * sizeof (ACE_USHORT16)) <= 0)
          : (ACE_OS::memcmp ((const void *) this->rep_,
                             (const void *) s.rep_,
                             s.len_ * sizeof (ACE_USHORT16)) < 0);
}

// Greater than comparison operator.

ACE_INLINE int
ACE_WString::operator > (const ACE_WString &s) const
{
  ACE_TRACE ("ACE_WString::operator >");
  return (this->len_ <= s.len_)
          ? (ACE_OS::memcmp ((const void *) this->rep_,
                             (const void *) s.rep_,
                             this->len_ * sizeof (ACE_USHORT16)) > 0)
          : (ACE_OS::memcmp ((const void *) this->rep_,
                             (const void *) s.rep_,
                             s.len_ * sizeof (ACE_USHORT16)) >= 0);
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
ACE_WString::find (const ACE_USHORT16 *s, int pos) const
{
  ACE_USHORT16 *substr = this->rep_ + pos;
  const ACE_USHORT16 *pointer = ACE_WString::strstr (substr, s);
  if (pointer == 0)
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
    if (this->rep_[i] == c)
      return ACE_static_cast (int, i);

  return ACE_WString::npos;
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
ACE_WString::rfind (ACE_USHORT16 c, int pos) const
{
  if (pos == ACE_WString::npos)
    pos = this->len_;

  for (int i = pos - 1; i >= 0; i--)
    if (this->rep_[i] == c)
      return i;

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

// ****************************************************************

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
