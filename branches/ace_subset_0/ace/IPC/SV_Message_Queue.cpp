// SV_Message_Queue.cpp
// $Id$

#include "ace/IPC/SV_Message_Queue.h"

#ifdef ACE_SUBSET_0
#include "ace/Logging/Log_Msg.h"
#endif

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/IPC/SV_Message_Queue.i"
#endif

ACE_RCSID(ace, SV_Message_Queue, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_SV_Message_Queue)

void
ACE_SV_Message_Queue::dump (void) const
{
  ACE_TRACE ("ACE_SV_Message_Queue::dump");
}

ACE_SV_Message_Queue::ACE_SV_Message_Queue (void)
{
  ACE_TRACE ("ACE_SV_Message_Queue::ACE_SV_Message_Queue");
}

ACE_SV_Message_Queue::~ACE_SV_Message_Queue (void)
{
  ACE_TRACE ("ACE_SV_Message_Queue::~ACE_SV_Message_Queue");
}

ACE_SV_Message_Queue::ACE_SV_Message_Queue (key_t external_id,
					    int create,
					    int perms)
{
  ACE_TRACE ("ACE_SV_Message_Queue::ACE_SV_Message_Queue");
#ifdef ACE_SUBSET_0
  if (this->open (external_id, create, perms) == -1)
    ACE_ERROR ((LM_ERROR, ACE_LIB_TEXT ("%p\n"),
		ACE_LIB_TEXT ("ACE_SV_Message_Queue::ACE_SV_Message_Queue")));
#else
  this->open (external_id, create, perms);
#endif
}
