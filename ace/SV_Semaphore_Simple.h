/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    SV_Semaphore_Simple.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_SV_SEMAPHORE_SIMPLE_H)
#define ACE_SV_SEMAPHORE_SIMPLE_H

#include "ace/ACE.h"

class ACE_Export ACE_SV_Semaphore_Simple
  // = TITLE
  //     This is a simple semaphore package that assumes there are
  //     no race conditions for initialization (i.e., the order of
  //     process startup must be well defined).
{
public:
  enum 
  {
    ACE_CREATE = IPC_CREAT, 
    ACE_OPEN = 0
  };

  // = Initialization and termination methods.
  ACE_SV_Semaphore_Simple (void);
  ACE_SV_Semaphore_Simple (key_t key, 
			   int flags = ACE_SV_Semaphore_Simple::ACE_CREATE,
			   int initial_value = 1,
			   u_short nsems = 1, 
			   int perms = ACE_DEFAULT_FILE_PERMS);
  ACE_SV_Semaphore_Simple (const char *name, 
			   int flags = ACE_SV_Semaphore_Simple::ACE_CREATE, 
			   int initial_value = 1, 
			   u_short nsems = 1, 
			   int perms = ACE_DEFAULT_FILE_PERMS);
  ~ACE_SV_Semaphore_Simple (void);

  int open (const char *name, 
	    int flags = ACE_SV_Semaphore_Simple::ACE_CREATE, 
	    int initial_value = 1, 
	    u_short nsems = 1, 
	    int perms = ACE_DEFAULT_FILE_PERMS);

  int open (key_t key, 
	    int flags = ACE_SV_Semaphore_Simple::ACE_CREATE, 
	    int initial_value = 1, 
	    u_short nsems = 1, 
	    int perms = ACE_DEFAULT_FILE_PERMS);
  // Open or create one or more SV_Semaphores.  We return 0 if all is
  // OK, else -1.
 
  int close (void);
  // Close a ACE_SV_Semaphore, marking it as invalid for subsequent
  // operations...

  int remove (void) const;
  // Remove all SV_Semaphores associated with a particular key.  This
  // call is intended to be called from a server, for example, when it
  // is being shut down, as we do an IPC_RMID on the ACE_SV_Semaphore,
  // regardless of whether other processes may be using it or not.
  // Most other processes should use close() below.

  // = Semaphore acquire and release methods.
  int acquire (u_short n = 0, int flags = 0) const;
  // Wait until a ACE_SV_Semaphore's value is greater than 0, the
  // decrement it by 1 and return. Dijkstra's P operation, Tannenbaums
  // DOWN operation.

  int acquire_read (u_short n = 0, int flags = 0) const;
  // Acquire a semaphore for reading.

  int acquire_write (u_short n = 0, int flags = 0) const;
  // Acquire a semaphore for writing

  int tryacquire (u_short n = 0, int flags = 0) const;
  // Non-blocking version of <acquire>.

  int tryacquire_read (u_short n = 0, int flags = 0) const;
  // Try to acquire the semaphore for reading.

  int tryacquire_write (u_short n = 0, int flags = 0) const;
  // Try to acquire the semaphore for writing.

  int release (u_short n = 0, int flags = 0) const;
  // Increment ACE_SV_Semaphore by one. Dijkstra's V operation,
  // Tannenbaums UP operation.

  // = Semaphore operation methods.
  int op (int val, u_short semnum = 0, int flags = 0) const;
  // General ACE_SV_Semaphore operation. Increment or decrement by a
  // specific amount (positive or negative; amount can`t be zero). 

  int op (sembuf op_vec[], u_short nsems) const;
  // General ACE_SV_Semaphore operation on an array of SV_Semaphores.

  // = Semaphore control methods.
  int control (int cmd, semun arg, u_short n = 0) const;
  int control (int cmd, int value = 0, u_short n = 0) const;

  int get_id (void) const;
  // Get underlying internal id.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  key_t key_;
  int internal_id_;
  int sem_number_;

  int init (key_t k = ACE_INVALID_SEM_KEY, int i = -1);
  key_t name_2_key (const char *name);
  // Convert name to key This function is used internally to create
  // keys for the semaphores. A valid name contains letters and
  // digits only and MUST start with a letter.
  //
  // The method for generating names is not very sophisticated, so
  // caller should not pass strings which match each other for the first
  // LUSED characters when he wants to get a different key.
};

#include "ace/SV_Semaphore_Simple.i"
#endif /* _SV_SEMAPHORE_SIMPLE_H */
