/* -*- C++ -*- */
// $Id$

// UPIPE_Stream.i

ACE_INLINE ACE_HANDLE 
ACE_UPIPE_Stream::get_handle (void) const
{
  ACE_TRACE ("ACE_UPIPE_Stream::get_handle");
  return this->ACE_SPIPE::get_handle ();
}

