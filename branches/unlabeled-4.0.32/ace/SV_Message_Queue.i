/* -*- C++ -*- */
// $Id$

// SV_Message_Queue.i

#include "ace/SV_Message_Queue.h"

// Open a message queue using the <external_id>.

inline int
ACE_SV_Message_Queue::open (key_t external_id, int create, int perms)
{
  ACE_TRACE ("ACE_SV_Message_Queue::open");
  return this->internal_id_ = ACE_OS::msgget (external_id, create | perms);
}

// What does it mean to close a message queue?!

inline int
ACE_SV_Message_Queue::close (void)
{
  ACE_TRACE ("ACE_SV_Message_Queue::close");
  this->internal_id_ = -1;
  return 1;
}

inline int
ACE_SV_Message_Queue::control (int option, void *arg)
{
  ACE_TRACE ("ACE_SV_Message_Queue::control");
  return ACE_OS::msgctl (this->internal_id_, option, 
			 (msqid_ds *) arg);
}

inline int
ACE_SV_Message_Queue::remove (void)
{
  ACE_TRACE ("ACE_SV_Message_Queue::remove");
  int result = this->control (IPC_RMID);
  this->internal_id_ = -1;
  return result;
}

inline int
ACE_SV_Message_Queue::get_id (void)
{
  ACE_TRACE ("ACE_SV_Message_Queue::get_id");
  return this->internal_id_;
}

inline void
ACE_SV_Message_Queue::set_id (int id)
{
  ACE_TRACE ("ACE_SV_Message_Queue::set_id");
  this->internal_id_ = id;
}

inline int
ACE_SV_Message_Queue::recv (ACE_SV_Message &mb, 
			    int length, 
			    long type, 
			    int mflags)
{
  ACE_TRACE ("ACE_SV_Message_Queue::recv");
  return ACE_OS::msgrcv (this->internal_id_, (void *) &mb, 
			 length, type, mflags);
}

inline int
ACE_SV_Message_Queue::send (const ACE_SV_Message &mb, 
			    int length, 
			    int mflags)
{
  ACE_TRACE ("ACE_SV_Message_Queue::send");
  return ACE_OS::msgsnd (this->internal_id_, (void *) &mb,
			 length, mflags);
}

