/* -*- C++ -*- */
// $Id$

// Stream.i

#include "ace/Log_Msg.h"

template <ACE_SYNCH_1> ACE_INLINE ACE_Module<ACE_SYNCH_2> *
ACE_Stream<ACE_SYNCH_2>::head (void)
{
  ACE_TRACE ("ACE_Stream<ACE_SYNCH_2>::head");
  return this->stream_head_;
}

template <ACE_SYNCH_1> ACE_INLINE ACE_Module<ACE_SYNCH_2> *
ACE_Stream<ACE_SYNCH_2>::tail (void)
{
  ACE_TRACE ("ACE_Stream<ACE_SYNCH_2>::tail");
  return this->stream_tail_;
}

template <ACE_SYNCH_1> ACE_INLINE int
ACE_Stream<ACE_SYNCH_2>::wait (void)
{
  ACE_TRACE ("ACE_Stream<ACE_SYNCH_2>::wait");
  return this->final_close_.wait ();
}

template <ACE_SYNCH_1> ACE_INLINE int
ACE_Stream_Iterator<ACE_SYNCH_2>::next (const ACE_Module<ACE_SYNCH_2> *&mod)
{
  ACE_TRACE ("ACE_Stream_Iterator<ACE_SYNCH_2>::next");
  mod = this->next_;
  return this->next_ != 0;
}
