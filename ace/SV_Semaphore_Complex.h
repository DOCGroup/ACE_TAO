/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//     ACE_SV_Semaphore_Complex.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_SV_SEMAPHORE_COMPLEX_H)
#define ACE_SV_SEMAPHORE_COMPLEX_H

#include "ace/SV_Semaphore_Simple.h"

class ACE_Export ACE_SV_Semaphore_Complex : private ACE_SV_Semaphore_Simple
  // = TITLE 
  //     This is a more complex semaphore wrapper that handles race
  //     conditions for initialization correctly...
  //
  // = DESCRIPTION
  //      This code is a port to C++, inspired by: W. Richard Stevens
  //      from his book: UNIX Network Programming (Prentice Hall, ISBN
  //      0-13-949876-1 - 1990) ACE_SV_Semaphore Interface: we provide
  //      a simpler and easier to understand interface to the System V
  //      ACE_SV_Semaphore calls.  We create and use a 2 + n-member
  //      set for the requested ACE_SV_Semaphore. The first member,
  //      [0], is a counter used to know when all processes have
  //      finished with the ACE_SV_Semaphore. The counter is
  //      initialized to a large number, decremented on every create
  //      or open and incremented on every close. This way we can use
  //      the "adjust" feature provided by System V so that any
  //      process that exit's without calling close() is accounted
  //      for. It doesn't help us if the last process does this (as we
  //      have no way of getting control to remove the
  //      ACE_SV_Semaphore) but it will work if any process other than
  //      the last does an exit (intentional or unintentional).
  //
  //      The second member, [1], of the ACE_SV_Semaphore is used as a
  //      lock variable to avoid any race conditions in the create()
  //      and close() functions.
  //
  //      The members beyond [1] are actual ACE_SV_Semaphore values in
  //      the array of SV_Semaphores (which may be sized by the user
  //      in the constructor).
{
public:
  enum 
  {
    ACE_CREATE = IPC_CREAT, 
    ACE_OPEN   = 0
  };

  // = Initialization and termination methods.
  ACE_SV_Semaphore_Complex (void);
  ACE_SV_Semaphore_Complex (key_t key,
			    int create = ACE_SV_Semaphore_Complex::ACE_CREATE,
			    int initial_value = 1, 
			    u_short nsems = 1, 
			    int perms = ACE_DEFAULT_FILE_PERMS);
  ACE_SV_Semaphore_Complex (const char *name, 
			    int create = ACE_SV_Semaphore_Complex::ACE_CREATE, 
			    int initial_value = 1, 
			    u_short nsems = 1, 
			    int perms = ACE_DEFAULT_FILE_PERMS);
  ~ACE_SV_Semaphore_Complex (void);

  int open (const char *name, 
	    int flags = ACE_SV_Semaphore_Simple::ACE_CREATE, 
	    int initial_value = 1, 
	    u_short nsems = 1, 
	    int perms = ACE_DEFAULT_FILE_PERMS);
  // Open or create an array of SV_Semaphores.  We return 0 if all is
  // OK, else -1.

  int open (key_t key, 
	    int flags = ACE_SV_Semaphore_Simple::ACE_CREATE, 
	    int initial_value = 1, 
	    u_short nsems = 1, 
	    int perms = ACE_DEFAULT_FILE_PERMS);
  // Open or create an array of SV_Semaphores.  We return 0 if all is
  // OK, else -1.

  int close (void);
  // Close an ACE_SV_Semaphore. Unlike the <remove> method, this
  // method is for a process to call before it exits, when it is done
  // with the ACE_SV_Semaphore. We "decrement" the counter of
  // processes using the ACE_SV_Semaphore, and if this was the last
  // one, we can remove the ACE_SV_Semaphore.

  // = Semaphore acquire and release methods.

  int acquire (u_short n = 0, int flags = 0) const;
  // Acquire the semaphore.

  int acquire_read (u_short n = 0, int flags = 0) const;
  // Acquire a semaphore for reading.

  int acquire_write (u_short n = 0, int flags = 0) const;
  // Acquire a semaphore for writing

  int tryacquire (u_short n = 0, int flags = 0) const;
  // Try to acquire the semaphore.

  int tryacquire_read (u_short n = 0, int flags = 0) const;
  // Try to acquire the semaphore for reading.

  int tryacquire_write (u_short n = 0, int flags = 0) const;
  // Try to acquire the semaphore for writing.

  int release (u_short n = 0, int flags = 0) const;
  // Release the semaphore.

  // = Semaphore operation methods.
  int op (int val, u_short n = 0, int flags = 0) const;
  int op (sembuf op_vec[], u_short n) const;

  // = Semaphore control methods.
  int control (int cmd, semun arg, u_short n = 0) const;
  int control (int cmd, int value = 0, u_short n = 0) const;

  // = Upgrade access control...
  ACE_USING ACE_SV_Semaphore_Simple::get_id;
  ACE_USING ACE_SV_Semaphore_Simple::remove;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  static const int BIGCOUNT_;
  static sembuf op_lock_[2];
  static sembuf op_endcreate_[2];
  static sembuf op_open_[1];
  static sembuf op_close_[3];
  static sembuf op_unlock_[1];
};

#include "ace/SV_Semaphore_Complex.i"
#endif /* ACE_SV_SEMAPHORE_COMPLEX_H */
