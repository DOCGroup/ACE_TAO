/* -*- C++ -*- */
// $Id$

// Return the number of args
ACE_INLINE size_t
ACE_ARGV::argc (void) const
{
  ACE_TRACE ("ACE_ARGV::argc");
  return this->argc_;
}

// Return the state of this ACE_ARGV
ACE_INLINE int
ACE_ARGV::state(void) const
{
  ACE_TRACE ("ACE_ARGV::state");
  return this->state_;
}

// Return the arguments in a space-separated string
ACE_INLINE const ACE_TCHAR *
ACE_ARGV::buf (void)
{
  ACE_TRACE ("ACE_ARGV::buf");

  if (this->buf_ == 0 && this->state_ == ITERATIVE) 
    this->create_buf_from_queue ();

  return (const ACE_TCHAR *) this->buf_;
}

// Return the arguments in an entry-per-argument array

ACE_INLINE ACE_TCHAR **
ACE_ARGV::argv (void)
{
  ACE_TRACE ("ACE_ARGV::argv");

  // Try to create the argv_ if it isn't there
  if (this->argv_ == 0) 
    {
      if (this->state_ == ITERATIVE && this->buf_ == 0)
        this->create_buf_from_queue ();

      // Convert buf_ to argv_
      if (this->string_to_argv () == -1)
        return (ACE_TCHAR **) 0;
    }

  return (ACE_TCHAR **) this->argv_;
}

// Subscript operator.  

ACE_INLINE const ACE_TCHAR *
ACE_ARGV::operator[] (size_t i)
{
  ACE_TRACE ("ACE_ARGV::operator[]");

  // Don't go out of bounds.
  if (i >= this->argc_)
    return 0;

  return (const ACE_TCHAR *) this->argv ()[i];
}



