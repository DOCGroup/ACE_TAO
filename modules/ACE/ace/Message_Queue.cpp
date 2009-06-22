// $Id$

#include "ace/Message_Queue.h"
#include "ace/Log_Msg.h"

#if !defined (__ACE_INLINE__)
#include "ace/Message_Queue.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (ace,
           Message_Queue,
           "$Id$")

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_Message_Queue_Base::~ACE_Message_Queue_Base (void)
{
}

int
ACE_Message_Queue_Base::state (void)
{
  ACE_TRACE ("ACE_Message_Queue_Base::state");

  return this->state_;
}

ACE_END_VERSIONED_NAMESPACE_DECL
