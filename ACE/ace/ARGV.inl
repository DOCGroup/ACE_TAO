/* -*- C++ -*- */
// $Id$

#include "ace/Global_Macros.h"

// Open versioned namespace, if enabled by the user.
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Return the number of args
template <typename CHAR_TYPE>
ACE_INLINE int
ACE_ARGV_T<CHAR_TYPE>::argc (void) const
{
  ACE_TRACE ("ACE_ARGV_T::argc");
  return this->argc_;
}

// Return the arguments in a space-separated string
template <typename CHAR_TYPE>
ACE_INLINE const CHAR_TYPE *
ACE_ARGV_T<CHAR_TYPE>::buf (void)
{
  ACE_TRACE ("ACE_ARGV_T::buf");

  if (this->buf_ == 0 && this->iterative_)
    this->create_buf_from_queue ();

  return (const CHAR_TYPE *) this->buf_;
}

// Return the arguments in an entry-per-argument array

template <typename CHAR_TYPE>
ACE_INLINE CHAR_TYPE **
ACE_ARGV_T<CHAR_TYPE>::argv (void)
{
  ACE_TRACE ("ACE_ARGV_T::argv");

  // Try to create the argv_ if it isn't there
  if (this->argv_ == 0)
    {
      if (this->iterative_ && this->buf_ == 0)
        this->create_buf_from_queue ();

      // Convert buf_ to argv_
      if (this->string_to_argv () == -1)
        return (CHAR_TYPE **) 0;
    }

  return (CHAR_TYPE **) this->argv_;
}

// Subscript operator.

template <typename CHAR_TYPE>
ACE_INLINE const CHAR_TYPE *
ACE_ARGV_T<CHAR_TYPE>::operator[] (size_t i)
{
  ACE_TRACE ("ACE_ARGV_T::operator[]");

  // Don't go out of bounds.
  if (i >= static_cast<size_t> (this->argc_))
    return 0;

  return (const CHAR_TYPE *) this->argv ()[i];
}

// Close versioned namespace, if enabled by the user.
ACE_END_VERSIONED_NAMESPACE_DECL
