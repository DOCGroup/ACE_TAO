/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Task_T.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_TASK_T_H)
#define ACE_TASK_T_H

#include "ace/Message_Queue.h"
#include "ace/Synch_T.h"
#include "ace/Task.h"

// Forward decls...
template <ACE_SYNCH_1> class ACE_Module;

template <ACE_SYNCH_1>
class ACE_Task : public ACE_Task_Base
  // = TITLE
  //    Primary interface for application message processing, as well 
  //    as input and output message queueing.
  //
  // = DESCRIPTION
  //    This class serves as the basis for passive and active objects
  //    in ACE.  
{
friend class ACE_Module<ACE_SYNCH_2>;
friend class ACE_Module_Type;
public:
  // Trait.
  typedef ACE_SYNCH_2 SYNCH;

  // = Initialization/termination methods.
  ACE_Task (ACE_Thread_Manager *thr_mgr = 0, 
	    ACE_Message_Queue<ACE_SYNCH_2> *mq = 0);
  // Initialize a Task, supplying a thread manager and a message
  // queue.  If the user doesn't supply a ACE_Message_Queue pointer
  // then we'll allocate one dynamically.  Otherwise, we'll use the
  // one they give.

  virtual ~ACE_Task (void);	
  // Destructor.

  ACE_Message_Queue<ACE_SYNCH_2> *msg_queue (void);
  // Gets the message queue associated with this task.

  void msg_queue (ACE_Message_Queue<ACE_SYNCH_2> *);
  // Sets the message queue associated with this task.

public: // Should be protected: 
  // = Message queue manipulation methods.

  int putq (ACE_Message_Block *, ACE_Time_Value *tv = 0);    
  // Insert message into the message list.

  int getq (ACE_Message_Block *&mb, ACE_Time_Value *tv = 0);
  // Extract the first message from the list (blocking).

  int ungetq (ACE_Message_Block *, ACE_Time_Value *tv = 0);	 
  // Return a message to the queue.

  int can_put (ACE_Message_Block *); 
  // Tests whether we can enqueue a message without blocking.

  int reply (ACE_Message_Block *, ACE_Time_Value *tv = 0);	     
  // Turn the message around and send it back down the Stream.

  int put_next (ACE_Message_Block *msg, ACE_Time_Value *tv = 0);
  // Transfer message to the adjacent ACE_Task in a ACE_Stream.

  // = ACE_Task utility routines to identify names et al.
  const char *name (void) const;
  // Return the name of the enclosing Module if there's one associated
  // with the Task, else returns 0.

  // = Pointers to next ACE_Task_Base (if ACE is part of an ACE_Stream).
  ACE_Task<ACE_SYNCH_2> *next (void);
  // Get next Task pointer.
  void next (ACE_Task<ACE_SYNCH_2> *);
  // Set next Task pointer.

  ACE_Task<ACE_SYNCH_2> *sibling (void);
  // Return the Task's sibling if there's one associated with the
  // Task's Module, else returns 0.

  ACE_Module<ACE_SYNCH_2> *module (void) const;
  // Return the Task's Module if there is one, else returns 0.

  int flush (u_long flag = ACE_Task_Flags::ACE_FLUSHALL); 
  // Flush the queue.  Note that if this conflicts with the C++
  // iostream flush() function, just rewrite the iostream function as
  // ::flush().

  // = Special routines corresponding to certain message types.

  void water_marks (ACE_IO_Cntl_Msg::ACE_IO_Cntl_Cmds, size_t);
  // Manipulate watermarks.

  ACE_Message_Queue<ACE_SYNCH_2> *msg_queue_;	
  // List of messages on the ACE_Task..

  int delete_msg_queue_;
  // 1 if should delete Message_Queue, 0 otherwise.

  ACE_Module<ACE_SYNCH_2> *mod_;		
  // Back-pointer to the enclosing module.

  ACE_Task<ACE_SYNCH_2> *next_;
  // Pointer to adjacent ACE_Task.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#if defined (__ACE_INLINE__)
#include "ace/Task_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Task_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Task_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_TASK_T_H */
