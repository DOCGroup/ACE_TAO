/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Stream.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_STREAM_H)
#define ACE_STREAM_H

//#include "ace/Multiplexor.h"

#include "ace/ACE.h"
#include "ace/IO_Cntl_Msg.h"
#include "ace/Message_Block.h"
#include "ace/Time_Value.h"
#include "ace/Module.h"

// Forward decls.
template<ACE_SYNCH_1> class ACE_Stream_Iterator;
//template<ACE_SYNCH_1> class ACE_Module;

template <ACE_SYNCH_1>
class ACE_Stream
  // = TITLE
  //    This class is the primary abstraction for the ASX framework.
  //    It is moduled after System V Stream.
  //
  // = DESCRIPTION
  //    A Stream consists of a stack of <ACE_Modules>, each of which
  //    contains two <ACE_Tasks>.
{
friend class ACE_Stream_Iterator<ACE_SYNCH_2>;
public:
  enum
  {
    M_DELETE = 3
    // Indicates that close() deletes the Tasks.  Don't change this
    // value without updating the same enum in class ACE_Module...
  };

  // = Initializatation and termination methods.
  ACE_Stream (void *arg = 0, 
	      ACE_Module<ACE_SYNCH_2> *head = 0, 
	      ACE_Module<ACE_SYNCH_2> *tail = 0);
  // Create a Stream consisting of <head> and <tail> as the Stream
  // head and Stream tail, respectively.  If these are 0 then the
  // <ACE_Stream_Head> and <ACE_Stream_Tail> are used, respectively.
  // <arg> is the value past in to the open() methods of the tasks.

  int open (void *arg,
	    ACE_Module<ACE_SYNCH_2> *head = 0, 
	    ACE_Module<ACE_SYNCH_2> *tail = 0);
  // Create a Stream consisting of <head> and <tail> as the Stream
  // head and Stream tail, respectively.  If these are 0 then the
  // <ACE_Stream_Head> and <ACE_Stream_Tail> are used, respectively.
  // <arg> is the value past in to the open() methods of the tasks.

  int close (int flags = M_DELETE);
  // Close down the stream and release all the resources.
  
  ~ACE_Stream (void);
  // Close down the stream and release all the resources.

  // = ACE_Stream plumbing operations 

  int push (ACE_Module<ACE_SYNCH_2> *mod);
  // Add a new module <mod> right below the Stream head.

  int pop (int flags = M_DELETE);
  // Remove the <mod> right below the Stream head and close it down.

  int top (ACE_Module<ACE_SYNCH_2> *&mod);
  // Return the top module on the stream (right below the stream
  // head).

  int remove (const char *mod, int flags = M_DELETE);
  // Remove the named module <mod> from the stream.  This bypasses the
  // strict LIFO ordering of push() and pop().

  ACE_Module<ACE_SYNCH_2> *head (void);	
  // Return current stream head.

  ACE_Module<ACE_SYNCH_2> *tail (void);	
  // Return current stream tail.

  ACE_Module<ACE_SYNCH_2> *find (const char *mod); 
  // Find a particular ACE_Module.

  int link (ACE_Stream<ACE_SYNCH_2> &);
  // Create a pipe between two Streams.

  int unlink (void);
  // Remove a pipe formed between two Streams.

  // = Blocking data transfer operations 
  int put (ACE_Message_Block *mb, ACE_Time_Value *timeout);
  // Send the message <mb> down the stream, starting at the Module
  // below the Stream head.  Wait for upto <timeout> amount of time
  // for the operation to complete (or block forever if <timeout> ==
  // 0).

  int get (ACE_Message_Block *&mb, ACE_Time_Value *timeout);
  // Read the message <mb> that is stored in the the stream head.
  // Wait for upto <timeout> amount of time for the operation to
  // complete (or block forever if <timeout> == 0).

  int control (ACE_IO_Cntl_Msg::ACE_IO_Cntl_Cmds cmd, void *args);
  // Send control message down the stream.

  int wait (void);		
  // Synchronize with the final close of the stream.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  int unlink_i (void);
  // Actually perform the unlinking of two Streams (must be called
  // with locks held). 

  int link_i (ACE_Stream<ACE_SYNCH_2> &);
  // Actually perform the linking of two Streams (must be called with
  // locks held).

  int push_module (ACE_Module<ACE_SYNCH_2> *,
		   ACE_Module<ACE_SYNCH_2> * = 0, 
		   ACE_Module<ACE_SYNCH_2> * = 0);
  // Must a new module onto the Stream.

  ACE_Module<ACE_SYNCH_2> *stream_head_;
  // Pointer to the head of the stream.

  ACE_Module<ACE_SYNCH_2> *stream_tail_;
  // Pointer to the tail of the stream.

  ACE_Stream<ACE_SYNCH_2> *linked_us_;
  // Pointer to an adjoining linked stream.

  // = Synchronization objects used for thread-safe streams.
  ACE_SYNCH_MUTEX_T lock_;
  // Protect the stream against race conditions.
  
  ACE_SYNCH_CONDITION_T final_close_;
  // Use to tell all threads waiting on the close that we are done.
};

template <ACE_SYNCH_1>
class ACE_Stream_Iterator
  // = TITLE
  //     Iterate through an <ACE_Stream>.
{
public:
  // = Initialization method.
  ACE_Stream_Iterator (const ACE_Stream<ACE_SYNCH_2> &sr);

  // = Iteration methods.

  int next (const ACE_Module<ACE_SYNCH_2> *&next_item);
  // Pass back the <next_item> that hasn't been seen in the set.
  // Returns 0 when all items have been seen, else 1.

  int done (void) const;
  // Returns 1 when all items have been seen, else 0.

  int advance (void);
  // Move forward by one element in the set.  Returns 0 when all the
  // items in the set have been seen, else 1.

private:
  ACE_Module<ACE_SYNCH_2> *next_;
  // Next <Module> that we haven't yet seen.
};

#if defined (__ACE_INLINE__)
#include "ace/Stream.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Stream.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Stream.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_STREAM_H */
