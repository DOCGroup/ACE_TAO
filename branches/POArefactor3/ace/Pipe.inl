/* -*- C++ -*- */
// $Id$

#include "ace/Global_Macros.h"

ACE_INLINE
ACE_Pipe::~ACE_Pipe (void)
{
  ACE_TRACE ("ACE_Pipe::~ACE_Pipe");
  // Notice that the destructor doesn't close the handles for you.
}

ACE_INLINE ACE_HANDLE
ACE_Pipe::read_handle (void) const
{
  ACE_TRACE ("ACE_Pipe::read_handle");
  return this->handles_[0];
}

ACE_INLINE ACE_HANDLE
ACE_Pipe::write_handle (void) const
{
  ACE_TRACE ("ACE_Pipe::write_handle");
  return this->handles_[1];
}
