// $Id$

#include "ace/Malloc.h"
#if !defined (ACE_HAS_WINCE)
# include "ace/Service_Config.h"
#endif /* !ACE_HAS_WINCE */
#include "ace/SString.h"
#include "ace/Auto_Ptr.h"

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
# include "ace/streams.h"
#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */

#if !defined (__ACE_INLINE__)
#include "ace/SString.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, SString, "$Id$")

ACE_Tokenizer::ACE_Tokenizer (ACE_TCHAR *buffer)
  : buffer_ (buffer),
    index_ (0),
    preserves_index_ (0),
    delimiter_index_ (0)
{
}

int
ACE_Tokenizer::delimiter (ACE_TCHAR d)
{
  if (delimiter_index_ == MAX_DELIMITERS)
    return -1;

  delimiters_[delimiter_index_].delimiter_ = d;
  delimiters_[delimiter_index_].replace_ = 0;
  delimiter_index_++;
  return 0;
}

int
ACE_Tokenizer::delimiter_replace (ACE_TCHAR d,
                                  ACE_TCHAR replacement)
{
  // Make it possible to replace delimiters on-the-fly, e.g., parse
  // string until certain token count and then copy rest of the
  // original string.
  for (int i = 0; i < delimiter_index_; i++)
    if (delimiters_[i].delimiter_ == d)
      {
        delimiters_[i].replacement_ = replacement;
        delimiters_[i].replace_ = 1;
        return 0;
      }

  if (delimiter_index_ >= MAX_DELIMITERS)
    return -1;

  delimiters_[delimiter_index_].delimiter_ = d;
  delimiters_[delimiter_index_].replacement_ = replacement;
  delimiters_[delimiter_index_].replace_ = 1;
  delimiter_index_++;
  return 0;
}

int
ACE_Tokenizer::preserve_designators (ACE_TCHAR start,
                                     ACE_TCHAR stop,
                                     int strip)
{
  if (preserves_index_ == MAX_PRESERVES)
    return -1;

  preserves_[preserves_index_].start_ = start;
  preserves_[preserves_index_].stop_ = stop;
  preserves_[preserves_index_].strip_ = strip;
  preserves_index_++;
  return 0;
}

int
ACE_Tokenizer::is_delimiter (ACE_TCHAR d,
                             int &replace,
                             ACE_TCHAR &r)
{
  replace = 0;

  for (int x = 0; x < delimiter_index_; x++)
    if (delimiters_[x].delimiter_ == d)
      {
        if (delimiters_[x].replace_)
          {
            r = delimiters_[x].replacement_;
            replace = 1;
          }
        return 1;
      }

  return 0;
}

int
ACE_Tokenizer::is_preserve_designator (ACE_TCHAR start,
                                       ACE_TCHAR &stop,
                                       int &strip)
{
  for (int x = 0; x < preserves_index_; x++)
    if (preserves_[x].start_ == start)
      {
        stop = preserves_[x].stop_;
        strip = preserves_[x].strip_;
        return 1;
      }

  return 0;
}

ACE_TCHAR *
ACE_Tokenizer::next (void)
{
  // Check if the previous pass was the last one in the buffer.
  if (index_ == -1)
    {
      index_ = 0;
      return 0;
    }

  ACE_TCHAR replacement;
  int replace;
  ACE_TCHAR *next_token;

  // Skip all leading delimiters.
  for (;;)
    {
      // Check for end of string.
      if (buffer_[index_] == '\0')
        {
          // If we hit EOS at the start, return 0.
          index_ = 0;
          return 0;
        }

      if (this->is_delimiter (buffer_[index_],
                              replace,
                              replacement))
        index_++;
      else
        break;
    }

  // When we reach this point, buffer_[index_] is a non-delimiter and
  // not EOS - the start of our next_token.
  next_token = buffer_ + index_;

  // A preserved region is it's own token.
  ACE_TCHAR stop;
  int strip;
  if (this->is_preserve_designator (buffer_[index_],
                                    stop,
                                    strip))
    {
      while (++index_)
        {
          if (buffer_[index_] == '\0')
            {
              index_ = -1;
              goto EXIT_LABEL;
            }

          if (buffer_[index_] == stop)
            break;
        }

      if (strip)
        {
          // Skip start preserve designator.
          next_token += 1;
          // Zap the stop preserve designator.
          buffer_[index_] = '\0';
          // Increment to the next token.
          index_++;
        }

      goto EXIT_LABEL;
    }

  // Step through finding the next delimiter or EOS.
  for (;;)
    {
      // Advance pointer.
      index_++;

      // Check for delimiter.
      if (this->is_delimiter (buffer_[index_],
                              replace,
                              replacement))
        {
          // Replace the delimiter.
          if (replace != 0)
            buffer_[index_] = replacement;

          // Move the pointer up and return.
          index_++;
          goto EXIT_LABEL;
        }

      // A preserve designator signifies the end of this token.
      if (this->is_preserve_designator (buffer_[index_],
                                        stop,
                                        strip))
        goto EXIT_LABEL;

      // Check for end of string.
      if (buffer_[index_] == '\0')
        {
          index_ = -1;
          goto EXIT_LABEL;
        }
    }

EXIT_LABEL:
  return next_token;
}

// ************************************************************

ACE_ALLOC_HOOK_DEFINE(ACE_CString)

char ACE_CString::NULL_CString_ = '\0';
const int ACE_CString::npos = -1;
const int ACE_SString::npos = -1;
const int ACE_WString::npos = -1;

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
ostream &
operator<< (ostream &os, const ACE_CString &cs)
{
  if (cs.fast_rep () != 0)
    os << cs.fast_rep ();
  return os;
}

ostream &
operator<< (ostream &os, const ACE_WString &ws)
{
  if (ws.fast_rep () != 0)
    {
      ACE_Auto_Basic_Array_Ptr<char> char_string(ws.char_rep ());
      os << char_string.get ();
    }
  return os;
}

ostream &
operator<< (ostream &os, const ACE_SString &ss)
{
  if (ss.fast_rep () != 0)
    os << ss.fast_rep ();
  return os;
}
#endif /* !ACE_LACKS_IOSTREAM_TOTALLY */

// Constructor that copies <s> into dynamically allocated memory.
// Probable loss of data. Please use with care.

ACE_CString::ACE_CString (const ACE_WSTRING_TYPE *s,
                          ACE_Allocator *alloc)
  : allocator_ (alloc ? alloc : ACE_Allocator::instance ()),
    len_ (0),
    buf_len_ (0),
    rep_ (0),
    release_ (0)
{
  ACE_TRACE ("ACE_CString::ACE_CString");

  if (s == 0 || s[0] == (ACE_WSTRING_TYPE) '\0')
    {
      this->release_ = 0;
      this->len_ = 0;
      this->rep_ = &ACE_CString::NULL_CString_;
    }
  else
    {
      this->release_ = 1;

      size_t len = ACE_WString::strlen (s);
      ACE_ALLOCATOR (this->rep_,
                     (char *) this->allocator_->malloc (len + 1));
      this->len_ = len;
      this->buf_len_ = len + 1;

      // Copy the ACE_WSTRING_TYPE * string byte-by-byte into the char *
      // string.
      for (size_t i = 0; i < this->len_; i++)
        this->rep_[i] = char (s[i]);

      this->rep_[this->len_] = '\0';
    }
}

// this method might benefit from a little restructuring.
void
ACE_CString::set (const char *s,
                  size_t len,
                  int release)
{
  // Case 1. Going from memory to more memory
  size_t new_buf_len = len + 1;
  if (s != 0 && len != 0 && s[0] != '\0' && release
      && this->buf_len_ < new_buf_len)
    {
      char *temp;
      ACE_ALLOCATOR (temp,
                     (char *) this->allocator_->malloc (new_buf_len));

      if (this->release_)
        this->allocator_->free (this->rep_);

      this->rep_ = temp;
      this->buf_len_ = new_buf_len;
      this->release_ = 1;
      this->len_ = len;
      ACE_OS::memcpy (this->rep_, s, len);
      // NUL terminate.
      this->rep_[len] = '\0';
    }

  // Case 2. No memory allocation is necessary.
  else
    {
      // Free memory if necessary and figure out future ownership
      if (!release || s == 0 || s[0] == '\0' || len == 0)
        if (this->release_)
          {
            this->allocator_->free (this->rep_);
            this->release_ = 0;
          }
      // else - stay with whatever value for release_ we have.

      // Populate data.
      if (s == 0 || s[0] == '\0' || len == 0)
        {
          this->buf_len_ = 0;
          this->len_ = 0;
          this->rep_ = &ACE_CString::NULL_CString_;
        }
      else if (!release)
        {
          this->buf_len_ = len;
          this->len_ = len;
          this->rep_ = (char *) s;
        }
      else
        {
          ACE_OS::memcpy (this->rep_, s, len);
          // NUL terminate.
          this->rep_[len] = 0;
          this->len_ = len;
        }
    }
}

// Return substring.
ACE_CString
ACE_CString::substring (size_t offset,
                        ssize_t length) const
{
  ACE_CString nil;
  size_t count = length;

  // case 1. empty string
  if (this->len_ == 0)
    return nil;

  // case 2. start pos past our end
  if (offset >= this->len_)
    return nil;
  // No length == empty string.
  else if (length == 0)
    return nil;
  // Get all remaining bytes.
  else if (length == -1)
    count = this->len_ - offset;

  return ACE_CString (&this->rep_[offset],
                      count,
                      this->allocator_);
}

// Concat operator (does copy memory).

ACE_CString &
ACE_CString::operator+= (const ACE_CString &s)
{
  ACE_TRACE ("ACE_CString::operator+=");

  if (s.len_ > 0)
    {
      size_t new_buf_len = this->len_ + s.len_ + 1;

      // case 1. No memory allocation needed.
      if (this->buf_len_ >= new_buf_len)
        // Copy in data from new string.
        ACE_OS::memcpy (this->rep_ + this->len_,
                        s.rep_,
                        s.len_);

      // case 2. Memory reallocation is needed
      else
        {
          char *t = 0;

          ACE_ALLOCATOR_RETURN (t,
                                (char *) this->allocator_->malloc (new_buf_len),
                                *this);

          // Copy memory from old string into new string.
          ACE_OS::memcpy (t,
                          this->rep_,
                          this->len_);

          ACE_OS::memcpy (t + this->len_,
                          s.rep_,
                          s.len_);

          if (this->release_)
            this->allocator_->free (this->rep_);

          this->release_ = 1;
          this->rep_ = t;
          this->buf_len_ = new_buf_len;
        }

      this->len_ += s.len_;
      this->rep_[this->len_] = '\0';
    }

  return *this;
}

ACE_ALLOC_HOOK_DEFINE(ACE_SString)

void
ACE_SString::dump (void) const
{
  ACE_TRACE ("ACE_SString::dump");
}

// Copy constructor.

ACE_SString::ACE_SString (const ACE_SString &s)
  : allocator_ (s.allocator_),
    len_ (s.len_)
{
  ACE_TRACE ("ACE_SString::ACE_SString");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  this->rep_ = (char *) this->allocator_->malloc (s.len_ + 1);
  ACE_OS::memcpy ((void *) this->rep_,
                  (const void *) s.rep_,
                  this->len_);
  this->rep_[this->len_] = '\0';
}

// Default constructor.

ACE_SString::ACE_SString (ACE_Allocator *alloc)
  : allocator_ (alloc),
    len_ (0),
    rep_ (0)

{
  ACE_TRACE ("ACE_SString::ACE_SString");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  this->len_ = 0;
  this->rep_ = (char *) this->allocator_->malloc (this->len_ + 1);
  this->rep_[this->len_] = '\0';
}

// Set the underlying pointer (does not copy memory).

void
ACE_SString::rep (char *s)
{
  ACE_TRACE ("ACE_SString::rep");

  this->rep_ = s;

  if (s == 0)
    this->len_ = 0;
  else
    this->len_ = ACE_OS::strlen (s);
}

// Constructor that actually copies memory.

ACE_SString::ACE_SString (const char *s,
                          ACE_Allocator *alloc)
  : allocator_ (alloc)
{
  ACE_TRACE ("ACE_SString::ACE_SString");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  if (s == 0)
    {
      this->len_ = 0;
      this->rep_ = (char *) this->allocator_->malloc (this->len_ + 1);
      this->rep_[this->len_] = '\0';
    }
  else
    {
      this->len_ = ACE_OS::strlen (s);
      this->rep_ = (char *) this->allocator_->malloc (this->len_ + 1);
      ACE_OS::strcpy (this->rep_, s);
    }
}

ACE_SString::ACE_SString (char c,
                          ACE_Allocator *alloc)
  : allocator_ (alloc)
{
  ACE_TRACE ("ACE_SString::ACE_SString");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  this->len_ = 1;
  this->rep_ = (char *) this->allocator_->malloc (this->len_ + 1);
  this->rep_[0] = c;
  this->rep_[this->len_] = '\0';
}

// Constructor that actually copies memory.

ACE_SString::ACE_SString (const char *s,
                          size_t len,
                          ACE_Allocator *alloc)
  : allocator_ (alloc)
{
  ACE_TRACE ("ACE_SString::ACE_SString");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  if (s == 0)
    {
      this->len_ = 0;
      this->rep_ = (char *) this->allocator_->malloc (this->len_ + 1);
      this->rep_[this->len_] = '\0';
    }
  else
    {
      this->len_ = len;
      this->rep_ = (char *) this->allocator_->malloc (this->len_ + 1);
      ACE_OS::memcpy (this->rep_, s, len);
      this->rep_[len] = '\0'; // Make sure to NUL terminate this!
    }
}

// Assignment operator (does copy memory).

ACE_SString &
ACE_SString::operator= (const ACE_SString &s)
{
  ACE_TRACE ("ACE_SString::operator=");
  // Check for identify.

  if (this != &s)
    {
      // Only reallocate if we don't have enough space...
      if (this->len_ < s.len_)
        {
          this->allocator_->free (this->rep_);
          this->rep_ = (char *) this->allocator_->malloc (s.len_ + 1);
        }
      this->len_ = s.len_;
      ACE_OS::strcpy (this->rep_, s.rep_);
    }

  return *this;
}

// Return substring.
ACE_SString
ACE_SString::substring (size_t offset,
                        ssize_t length) const
{
  ACE_SString nil;
  size_t count = length;

  // case 1. empty string
  if (len_ == 0)
    return nil;

  // case 2. start pos l
  if (offset >= len_)
    return nil;

  // get all remaining bytes
  if (length == -1)
    count = len_ - offset;

  return ACE_SString (&rep_[offset], count, this->allocator_);
}

ACE_ALLOC_HOOK_DEFINE(ACE_WString)

void
ACE_WString::dump (void) const
{
  ACE_TRACE ("ACE_WString::dump");
}

// Default constructor.

ACE_WString::ACE_WString (ACE_Allocator *alloc)
  : allocator_ (alloc),
    len_ (0),
    rep_ (0)
{
  ACE_TRACE ("ACE_WString::ACE_WString");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  this->len_ = 0;
  this->rep_ = (ACE_WSTRING_TYPE *) this->allocator_->malloc
    ((this->len_ + 1) * sizeof (ACE_WSTRING_TYPE));
  this->rep_[this->len_] = 0;
}

/* static */
size_t
ACE_WString::strlen (const ACE_WSTRING_TYPE *s)
{
  ACE_TRACE ("ACE_WString::strlen");

#if defined (ACE_HAS_WCHAR)
  return ACE_OS::strlen (s);
#else /* ACE_HAS_WCHAR */
  size_t len = 0;

  while (*s++ != 0)
    ++len;

  return len;
#endif /* ACE_HAS_WCHAR */
}

// Get the underlying pointer as an ASCII char.

char *
ACE_WString::char_rep (void) const
{
  ACE_TRACE ("ACE_WString::char_rep");
  if (this->len_ <= 0)
    return 0;
  else
    {
      char *t;

      ACE_NEW_RETURN (t,
                      char[this->len_ + 1],
                      0);

      for (size_t i = 0; i < this->len_; i++)
        // Note that this cast may lose data if wide chars are
        // actually used!
        t[i] = char (this->rep_[i]);

      t[this->len_] = '\0';
      return t;
    }
}

// Get the underlying pointer as an ASCII char.

ACE_USHORT16 *
ACE_WString::ushort_rep (void) const
{
  ACE_TRACE ("ACE_WString::char_rep");
  if (this->len_ <= 0)
    return 0;
  else
    {
      ACE_USHORT16 *t;

      ACE_NEW_RETURN (t,
                      ACE_USHORT16[this->len_ + 1],
                      0);

      for (size_t i = 0; i < this->len_; i++)
        // Note that this cast may lose data if wide chars are
        // actually used!
        t[i] = (ACE_USHORT16)this->rep_[i];

      t[this->len_] = 0;
      return t;
    }
}

// Constructor that actually copies memory.

ACE_WString::ACE_WString (const char *s,
                          ACE_Allocator *alloc)
  : allocator_ (alloc)
{
  ACE_TRACE ("ACE_WString::ACE_WString");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  if (s == 0)
    {
      this->len_ = 0;
      this->rep_ = (ACE_WSTRING_TYPE *)  this->allocator_->malloc
        ((this->len_ + 1) * sizeof (ACE_WSTRING_TYPE));
      this->rep_[this->len_] = 0;
    }
  else
    {
      this->len_ = ACE_OS::strlen (s);
      this->rep_ = (ACE_WSTRING_TYPE *) this->allocator_->malloc
        ((this->len_ + 1) * sizeof (ACE_WSTRING_TYPE));

      // Copy the char * string byte-by-byte into the ACE_WSTRING_TYPE *
      // string.
      for (size_t i = 0; i < this->len_; i++)
        this->rep_[i] = s[i];

      // null terminate
      this->rep_[this->len_] = 0;
    }
}

// Constructor that actually copies memory.

ACE_WString::ACE_WString (const ACE_WSTRING_TYPE *s,
                          ACE_Allocator *alloc)
  : allocator_ (alloc)
{
  ACE_TRACE ("ACE_WString::ACE_WString");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  if (s == 0)
    {
      this->len_ = 0;
      this->rep_ = (ACE_WSTRING_TYPE *)  this->allocator_->malloc
        ((this->len_ + 1) * sizeof (ACE_WSTRING_TYPE));
      this->rep_[this->len_] = 0;
    }
  else
    {
      this->len_ = ACE_WString::strlen (s);
      this->rep_ = (ACE_WSTRING_TYPE *) this->allocator_->malloc
        ((this->len_ + 1) * sizeof (ACE_WSTRING_TYPE));

      ACE_OS::memcpy (this->rep_,
                      s,
                      this->len_ * sizeof (ACE_WSTRING_TYPE));

      // null terminate
      this->rep_[this->len_] = 0;
    }
}

ACE_WString::ACE_WString (ACE_WSTRING_TYPE c,
                          ACE_Allocator *alloc)
  : allocator_ (alloc)
{
  ACE_TRACE ("ACE_WString::ACE_WString");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  this->len_ = 1;
  this->rep_ = (ACE_WSTRING_TYPE *) this->allocator_->malloc
    ((this->len_ + 1) * sizeof (ACE_WSTRING_TYPE));
  this->rep_[0] = c;
  this->rep_[this->len_] = 0;
}

// Constructor that actually copies memory.

ACE_WString::ACE_WString (const ACE_WSTRING_TYPE *s,
                          size_t len,
                          ACE_Allocator *alloc)
  : allocator_ (alloc)
{
  ACE_TRACE ("ACE_WString::ACE_WString");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  if (s == 0)
    {
      this->len_ = 0;
      this->rep_ = (ACE_WSTRING_TYPE *) this->allocator_->malloc
        ((this->len_ + 1) * sizeof (ACE_WSTRING_TYPE));
      this->rep_[this->len_] = 0;
    }
  else
    {
      this->len_ = len;
      this->rep_ = (ACE_WSTRING_TYPE *) this->allocator_->malloc
        ((this->len_ + 1) * sizeof (ACE_WSTRING_TYPE));

      ACE_OS::memcpy (this->rep_,
                      s,
                      len * sizeof (ACE_WSTRING_TYPE));

      // null terminate
      this->rep_[this->len_] = 0;
    }
}

#if defined (ACE_WSTRING_HAS_USHORT_SUPPORT)
ACE_WString::ACE_WString (const ACE_USHORT16 *s,
                          size_t len,
                          ACE_Allocator *alloc)
  : allocator_ (alloc)
{
  ACE_TRACE ("ACE_WString::ACE_WString");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  if (s == 0)
    {
      this->len_ = 0;
      this->rep_ = (ACE_WSTRING_TYPE *) this->allocator_->malloc
        ((this->len_ + 1) * sizeof (ACE_WSTRING_TYPE));
      this->rep_[this->len_] = 0;
    }
  else
    {
      this->len_ = len;
      this->rep_ = (ACE_WSTRING_TYPE *) this->allocator_->malloc
        ((this->len_ + 1) * sizeof (ACE_WSTRING_TYPE));

      for (size_t i = 0; i <= len; ++i)
        this->rep_[i] = (const ACE_WSTRING_TYPE) s[i];

      // null terminate
      this->rep_[this->len_] = 0;
    }
}
#endif /* ACE_WSTRING_HAS_USHORT_SUPPORT */

// Constructor that allocates empty memory

ACE_WString::ACE_WString (size_t len,
                          ACE_Allocator *alloc)
  : allocator_ (alloc)
{
  ACE_TRACE ("ACE_WString::ACE_WString");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  this->len_ = len;
  this->rep_ = (ACE_WSTRING_TYPE *) this->allocator_->malloc
    ((this->len_ + 1) * sizeof (ACE_WSTRING_TYPE));

  ACE_OS::memset (this->rep_,
                  0,
                  len * sizeof (ACE_WSTRING_TYPE));
  // NUL terminate.
  this->rep_[this->len_] = 0;
}

// Copy constructor.

ACE_WString::ACE_WString (const ACE_WString &s)
  : allocator_ (s.allocator_),
    len_ (s.len_)
{
  ACE_TRACE ("ACE_WString::ACE_WString");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  this->rep_ = (ACE_WSTRING_TYPE *) this->allocator_->malloc
    ((s.len_ + 1) * sizeof (ACE_WSTRING_TYPE));
  ACE_OS::memcpy ((void *) this->rep_,
                  (const void *) s.rep_,
                  this->len_ * sizeof (ACE_WSTRING_TYPE));
  this->rep_[this->len_] = 0;
}

// Assignment operator (does copy memory).

ACE_WString &
ACE_WString::operator= (const ACE_WString &s)
{
  ACE_TRACE ("ACE_WString::operator=");
  // Check for identify.

  if (this != &s)
    this->set (s.rep_, s.len_);

  return *this;
}

void
ACE_WString::set (const ACE_WSTRING_TYPE *s)
{
  this->set (s, ACE_WString::strlen (s));
}

void
ACE_WString::set (const ACE_WSTRING_TYPE *s, size_t len)
{
  // Only reallocate if we don't have enough space...
  if (this->len_ < len)
    {
      this->allocator_->free (this->rep_);
      this->rep_ = (ACE_WSTRING_TYPE *) this->allocator_->malloc
        ((len + 1) * sizeof (ACE_WSTRING_TYPE));
    }

  this->len_ = len;
  ACE_OS::memcpy (this->rep_,
                  s,
                  len * sizeof (ACE_WSTRING_TYPE));
  // NUL terminate.
  this->rep_[len] = 0;
}

// return substring
ACE_WString
ACE_WString::substring (size_t offset,
                        ssize_t length) const
{
  ACE_WString nil;
  size_t count = length;

  // case 1. empty string
  if (len_ == 0)
    return nil;

  // case 2. start pos l
  if (offset >= len_)
    return nil;

  // get all remaining bytes
  if (length == -1)
    count = len_ - offset;

  return ACE_WString (&rep_[offset],
                      count,
                      this->allocator_);
}

void
ACE_WString::resize (size_t len)
{
  ACE_TRACE ("ACE_WString::resize");

  // Only reallocate if we don't have enough space...
  if (this->len_ < len)
    {
      this->allocator_->free (this->rep_);
      this->rep_ = (ACE_WSTRING_TYPE *) this->allocator_->malloc
        ((len + 1) * sizeof (ACE_WSTRING_TYPE));
    }

  this->len_ = len;
  ACE_OS::memset (this->rep_,
                  0,
                  this->len_ * sizeof (ACE_WSTRING_TYPE));
  this->rep_[this->len_] = 0;
}

// Concat operator (does copy memory).

ACE_WString &
ACE_WString::operator+= (const ACE_WString &s)
{
  ACE_TRACE ("ACE_WString::operator+=");
  ACE_WSTRING_TYPE *t = (ACE_WSTRING_TYPE *) this->allocator_->malloc
    ((this->len_ + s.len_ + 1) * sizeof (ACE_WSTRING_TYPE));

  ACE_OS::memcpy ((void *) t,
                  (const void *) this->rep_,
                  this->len_ * sizeof (ACE_WSTRING_TYPE));
  ACE_OS::memcpy ((void *) (t + this->len_),
                  (const void *) s.rep_,
                  s.len_ * sizeof (ACE_WSTRING_TYPE));
  this->len_ += s.len_;

  // NUL terminate.
  t[this->len_] = 0;

  this->allocator_->free (this->rep_);
  this->rep_ = t;

  return *this;
}

ACE_WString::~ACE_WString (void)
{
  ACE_TRACE ("ACE_WString::~ACE_WString");
  this->allocator_->free (this->rep_);
}

/* static */
const ACE_WSTRING_TYPE *
ACE_WString::strstr (const ACE_WSTRING_TYPE *s1,
                     const ACE_WSTRING_TYPE *s2)
{
  ACE_TRACE ("ACE_WString::strstr");

  // Original string length
  size_t len1 = ACE_WString::strlen (s1);
  // Substring length
  size_t len2 = ACE_WString::strlen (s2);

  // Check if the substring is longer than the string being searched.
  if (len2 > len1)
    return 0;

  // Go upto <len>
  size_t len = len1 - len2;

  for (size_t i = 0; i <= len; i++)
    {
      if (ACE_OS::memcmp (s1 + i, s2, len2 * sizeof (ACE_WSTRING_TYPE)) == 0)
        // Found a match!  Return the index.
        return s1 + i;
    }

  return 0;
}
