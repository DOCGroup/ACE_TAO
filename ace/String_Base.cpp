#ifndef ACE_STRING_BASE_CPP
#define ACE_STRING_BASE_CPP

#include "ace/ACE.h"
#include "ace/Malloc.h"
#include "ace/String_Base.h"
#include "ace/Auto_Ptr.h"

#if !defined (__ACE_INLINE__)
#include "ace/String_Base.i"
#endif /* __ACE_INLINE__ */


ACE_RCSID (ace,
           String_Base,
           "$Id$")


ACE_ALLOC_HOOK_DEFINE(ACE_String_Base)

template <class CHAR>
CHAR ACE_String_Base<CHAR>::NULL_String_ = '\0';

// this method might benefit from a little restructuring.
template <class CHAR> void
ACE_String_Base<CHAR>::set (const CHAR *s,
                            size_t len,
                            int release)
{
  // Case 1. Going from memory to more memory
  size_t new_buf_len = len + 1;
  if (s != 0 && len != 0 && release && this->buf_len_ < new_buf_len)
    {
      CHAR *temp;
      ACE_ALLOCATOR (temp,
                     (CHAR *) this->allocator_->malloc (new_buf_len * sizeof (CHAR)));

      if (this->release_)
        this->allocator_->free (this->rep_);

      this->rep_ = temp;
      this->buf_len_ = new_buf_len;
      this->release_ = 1;
      this->len_ = len;
      ACE_OS::memcpy (this->rep_, s, len * sizeof (CHAR));
      // NUL terminate.
      this->rep_[len] = '\0';
    }

  // Case 2. No memory allocation is necessary.
  else
    {
      // Free memory if necessary and figure out future ownership
      if (!release || s == 0 || len == 0)
        {
          if (this->release_)
            {
              this->allocator_->free (this->rep_);
              this->release_ = 0;
            }
        }
      // else - stay with whatever value for release_ we have.

      // Populate data.
      if (s == 0 || len == 0)
        {
          this->buf_len_ = 0;
          this->len_ = 0;
          this->rep_ = &ACE_String_Base<CHAR>::NULL_String_;
        }
      else if (!release)
        {
          this->buf_len_ = len;
          this->len_ = len;
          this->rep_ = (CHAR *) s;
        }
      else
        {
          ACE_OS::memcpy (this->rep_, s, len * sizeof (CHAR));
          // NUL terminate.
          this->rep_[len] = 0;
          this->len_ = len;
        }
    }
}

// Return substring.
template <class CHAR> ACE_String_Base<CHAR>
ACE_String_Base<CHAR>::substring (size_t offset,
                                  ssize_t length) const
{
  ACE_String_Base<CHAR> nill;
  size_t count = length;

  // case 1. empty string
  if (this->len_ == 0)
    return nill;

  // case 2. start pos past our end
  if (offset >= this->len_)
    return nill;
  // No length == empty string.
  else if (length == 0)
    return nill;
  // Get all remaining bytes.
  else if (length == -1 || count > (this->len_ - offset))
    count = this->len_ - offset;

  return ACE_String_Base<CHAR> (&this->rep_[offset],
                                count,
                                this->allocator_);
}

// Concat operator (does copy memory).

template <class CHAR> ACE_String_Base<CHAR> &
ACE_String_Base<CHAR>::operator+= (const ACE_String_Base<CHAR> &s)
{
  ACE_TRACE ("ACE_String_Base<CHAR>::operator+=");

  if (s.len_ > 0)
    {
      size_t new_buf_len = this->len_ + s.len_ + 1;

      // case 1. No memory allocation needed.
      if (this->buf_len_ >= new_buf_len)
        // Copy in data from new string.
        ACE_OS::memcpy (this->rep_ + this->len_,
                        s.rep_,
                        s.len_ * sizeof (CHAR));
      // case 2. Memory reallocation is needed
      else
        {
          CHAR *t = 0;

          ACE_ALLOCATOR_RETURN (t,
                                (CHAR *) this->allocator_->malloc (new_buf_len *
                                                                   sizeof (CHAR)),
                                *this);

          // Copy memory from old string into new string.
          ACE_OS::memcpy (t,
                          this->rep_,
                          this->len_ * sizeof (CHAR));

          ACE_OS::memcpy (t + this->len_,
                          s.rep_,
                          s.len_ * sizeof (CHAR));

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

template <class CHAR> u_long
ACE_String_Base<CHAR>::hash (void) const
{
  return ACE::hash_pjw ((ACE_reinterpret_cast (char *,
                                               ACE_const_cast (CHAR *,
                                                               this->rep_))),
                        this->len_ * sizeof (CHAR));
}

template <class CHAR> void
ACE_String_Base<CHAR>::resize (size_t len, CHAR c)
{
  ACE_TRACE ("ACE_String_Base<CHAR>::resize");

  // Only reallocate if we don't have enough space...
  if (this->buf_len_ <= len)
    {
      if (this->buf_len_ != 0)
        this->allocator_->free (this->rep_);

      this->rep_ = (CHAR *)
        this->allocator_->malloc ((len + 1) * sizeof (CHAR));
      this->buf_len_ = len + 1;
    }

  this->len_ = 0;
  ACE_OS::memset (this->rep_,
                  c,
                  this->buf_len_ * sizeof (CHAR));
}

#endif  /* ACE_STRING_BASE_CPP */
