// SV_Semaphore_Simple.cpp
// $Id$

/* -*- C++ -*- */

#define ACE_BUILD_DLL
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
				  u_short semnum) const
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
ACE_SV_Semaphore_Simple::op (int val, u_short n, int flags) const
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
			       u_short n, 
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

  if (ACE_BIT_ENABLED (flags, IPC_CREAT))
    for (int i = 0; i < n; i++)
      if (ACE_OS::semctl (this->internal_id_, i, SETVAL, ivalue) == -1)
	return -1;

  return 0;
}

ACE_SV_Semaphore_Simple::ACE_SV_Semaphore_Simple (key_t k, 
						  int flags, 
						  int initial_value, 
						  u_short n, 
						  int perms)
  : key_ (k)
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::ACE_SV_Semaphore_Simple");
  if (this->open (k, flags, initial_value, n, perms) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_SV_Semaphore::ACE_SV_Semaphore"));
}

// Convert name to key.  This function is used internally to create keys
// for the semaphores.
//
// The method for generating names is a 32 bit CRC, but still we
// measured close to collition ratio of nearly 0.1% for
// ACE::unique_name()-like strings.

key_t 
ACE_SV_Semaphore_Simple::name_2_key (const char *name)
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::name_2_key");
  
  if (name == 0)
    {
      errno = EINVAL;
      return ACE_INVALID_SEM_KEY;
    }

  // Basically "hash" the values in the <name>.  This won't
  // necessarily guarantee uniqueness of all keys.
  // But (IMHO) CRC32 is good enough for most purposes (Carlos)
  return (key_t) ACE::crc32 (name);
}

// Open or create a ACE_SV_Semaphore.  We return 1 if all is OK, else
// 0.
     
int 
ACE_SV_Semaphore_Simple::open (const char *name, 
			       int flags, 
			       int initial_value, 
			       u_short n, 
			       int perms)
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::open");

  key_t key;
  
  if (name == 0)
    key = ACE_DEFAULT_SEM_KEY;
  else 
    key = this->name_2_key (name);

  return this->open (key, flags, initial_value, n, perms);
}

ACE_SV_Semaphore_Simple::ACE_SV_Semaphore_Simple (const char *name, 
						  int flags, 
						  int initial_value, 
						  u_short n, 
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

ACE_SV_Semaphore_Simple::ACE_SV_Semaphore_Simple (void)
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::ACE_SV_Semaphore_Simple");
  this->init ();
}

// Remove all SV_Semaphores associated with a particular key.  This
// call is intended to be called from a server, for example, when it
// is being shut down, as we do an IPC_RMID on the ACE_SV_Semaphore,
// regardless of whether other processes may be using it or not.  Most
// other processes should use close() below.
     
int 
ACE_SV_Semaphore_Simple::remove (void) const
{
  ACE_TRACE ("ACE_SV_Semaphore_Simple::remove");
  int result = this->control (IPC_RMID);
  ((ACE_SV_Semaphore_Simple *) this)->init ();
  return result;
}

