// SV_Message_Queue.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/SV_Message_Queue.h"

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
  if (this->open (external_id, create, perms) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", 
		"ACE_SV_Message_Queue::ACE_SV_Message_Queue"));
}
