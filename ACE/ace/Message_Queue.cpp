#include "ace/Message_Queue.h"
#include "ace/Log_Category.h"

#if !defined (__ACE_INLINE__)
#include "ace/Message_Queue.inl"
#endif /* __ACE_INLINE__ */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

int
ACE_Message_Queue_Base::state ()
{
  ACE_TRACE ("ACE_Message_Queue_Base::state");

  return this->state_;
}

ACE_END_VERSIONED_NAMESPACE_DECL
