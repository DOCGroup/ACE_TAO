/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    SV_Message_Queue.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_MESSAGE_QUEUE_H)
#define ACE_MESSAGE_QUEUE_H

#include "ace/ACE.h"
#include "ace/SV_Message.h"

class ACE_Export ACE_SV_Message_Queue
  // = TITLE
  //     Defines the header file for the C++ wrapper for System V IPC
  //     message queues.  
{
public: 
  // = Useful symbolic constants.
  enum 
  {
    ACE_CREATE = IPC_CREAT, 
    ACE_OPEN   = 0, 
    ACE_NOWAIT = IPC_NOWAIT
  };

  // = Initialization and termination methods.
  ACE_SV_Message_Queue (void);
  ACE_SV_Message_Queue (key_t external_id, 
			int create = ACE_SV_Message_Queue::ACE_OPEN,
			int perms = ACE_DEFAULT_PERMS);
  int open (key_t external_id, 
	    int create = ACE_SV_Message_Queue::ACE_OPEN,
	    int perms = ACE_DEFAULT_PERMS);
   // Open a message queue using the <external_id>.

  ~ACE_SV_Message_Queue (void);

  int close (void);
  // Close down this instance of the message queue without removing it
  // from the system.

  int remove (void);
  // Close down and remove the message queue from the system.


  // = Message transfer methods.
  int recv (ACE_SV_Message &mb, 
	    int length, 
	    long mtype = 0, 
	    int mflags = 0);

  int send (const ACE_SV_Message &mb, 
	    int length, 
	    int mflags = 0);

  int control (int option, void *arg = 0);
  // Access the underlying control operations.

  // = Get/set the underly internal id.
  int get_id (void);
  void set_id (int);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  int internal_id_;
  // Returned from the underlying msgget() system call.
};

#include "ace/SV_Message_Queue.i"
#endif /* ACE_MESSAGE_QUEUE_H */
