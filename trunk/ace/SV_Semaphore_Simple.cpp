// SV_Semaphore_Simple.cpp
// $Id$

/* -*- C++ -*- */

#define ACE_BUILD_DLL
#include "ace/Log_Msg.h"
#include "ace/SV_Semaphore_Simple.h"

ACE_ALLOC_HOOK_DEFINE(ACE_SV_Semaphore_Simple)

void
ACE_SV_Semaphore_Simple::dump (void) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::dump");
}

int
ACE_SV_Semaphore_Simple::control (int cmd, 
				  int value, 
				  int semnum) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::control");
  if (this->internal_id_ == -1)
    return -1;
  else
    {
      semun semctl_arg;

      semctl_arg.val = value;
      return ACE_OS::semctl (this->internal_id_, semnum, 
			     cmd, semctl_arg);
    }
}

int
ACE_SV_Semaphore_Simple::init (key_t k, int i)
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::init");
  this->key_ = k;
  this->internal_id_ = i;
  return 0;
}

// General ACE_SV_Semaphore operation. Increment or decrement by a
// specific amount (positive or negative; amount can`t be zero). 
     
int 
ACE_SV_Semaphore_Simple::op (int val, int n, int flags) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::op");
  sembuf op_op;

  op_op.sem_num = n;
  op_op.sem_flg = flags;

  if (this->internal_id_ == -1)
    return -1;
  else if ((op_op.sem_op = val) == 0)
    return -1;
  else
    return ACE_OS::semop (this->internal_id_, &op_op, 1);
}

// Open or create one or more SV_Semaphores.  We return 0 if all is
// OK, else -1.

int 
ACE_SV_Semaphore_Simple::open (key_t k, 
			       int flags, 
			       int initial_value, 
			       int n, 
			       int perms)
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::open");
  union semun ivalue;

  if (k == IPC_PRIVATE || k == ACE_INVALID_SEM_KEY)
    return -1;

  ivalue.val = initial_value;
  this->key_ = k;
  this->sem_number_ = n;

  this->internal_id_ = ACE_OS::semget (this->key_, n, perms | flags);

  if (this->internal_id_ == -1)
    return -1;

  if (flags == IPC_CREAT)
    {
      for (int i = 0; i < n; i++)
	if (ACE_OS::semctl (this->internal_id_, i, SETVAL, ivalue) == -1)
	  return -1;
    }

  return 0;
}

ACE_SV_Semaphore_Simple::ACE_SV_Semaphore_Simple (key_t k, 
						  int flags, 
						  int initial_value, 
						  int n, 
						  int perms)
  : key_ (k)
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::ACE_SV_Semaphore_Simple");
  if (this->open (k, flags, initial_value, n, perms) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_SV_Semaphore::ACE_SV_Semaphore"));
}

// Convert name to key.  This function is used internally to create keys
// for the semaphores.  A valid name contains letters and digits
// only and MUST start with a letter.
//
// The method for generating names is not very sophisticated, so
// caller should not pass strings which match each other for the first
// LUSED characters when he wants to get a different key.  

key_t 
ACE_SV_Semaphore_Simple::name_2_key (const char *name)
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::name_2_key");
  
  if (name == 0 || !isalpha (*name))
    {
      errno = EINVAL;
      return ACE_INVALID_SEM_KEY;
    }
  
  // The key is the character value of the first LUSED chars from name
  // placed in proto.

  u_long proto = 0;

  for (int i = 0; i < LUSED; ++i)
    {
      if (*name == '\0')
	break;
      proto <<= 8;
      proto |= *name++ & 0xff;
    }

  return (key_t) proto;
}

// Open or create a ACE_SV_Semaphore.  We return 1 if all is OK, else
// 0.
     
int 
ACE_SV_Semaphore_Simple::open (const char *name, 
			       int flags, 
			       int initial_value, 
			       int n, 
			       int perms)
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::open");
  return this->open (this->name_2_key (name), 
		     flags, initial_value, n, perms);
}

ACE_SV_Semaphore_Simple::ACE_SV_Semaphore_Simple (const char *name, 
						  int flags, 
						  int initial_value, 
						  int n, 
						  int perms)
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::ACE_SV_Semaphore_Simple");
  if (this->open (name, flags, initial_value, n, perms) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", 
		"ACE_SV_Semaphore_Simple::ACE_SV_Semaphore_Simple"));
}

ACE_SV_Semaphore_Simple::~ACE_SV_Semaphore_Simple (void)
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::~ACE_SV_Semaphore_Simple");
  this->close ();
}

