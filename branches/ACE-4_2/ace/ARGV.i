/* -*- C++ -*- */
// $Id$

// ARGV.i

ACE_INLINE size_t
ACE_ARGV::argc (void) const
{
  ACE_TRACE ("ACE_ARGV::argc");
  return this->argc_;
}

ACE_INLINE char *
ACE_ARGV::buf (void) const
{
  ACE_TRACE ("ACE_ARGV::buf");
  return this->buf_;
}

ACE_INLINE char *
ACE_ARGV::operator[] (int i) const
{
  ACE_TRACE ("ACE_ARGV::operator[]");
  return this->argv_[i];
}

ACE_INLINE char **
ACE_ARGV::argv (void) const
{
  ACE_TRACE ("ACE_ARGV::argv");
  return this->argv_;
}
