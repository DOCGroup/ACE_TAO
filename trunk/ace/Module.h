/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Module.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_MODULE_H)
#define ACE_MODULE_H

#include "ace/ACE.h"
#include "ace/Task.h"

// Forward decl.
// template <ACE_SYNCH_1> class ACE_Task;

template <ACE_SYNCH_1>
class ACE_Module
  // = TITLE
  //     An abstraction for managing a bi-directional flow of messages.
  // 
  // = DESCRIPTION
  //     This is based on the Module concept in System V Streams,
  //     which contains a pair of Tasks, one for handling upstream
  //     processing, one for handling downstream processing.
{
  friend class ACE_Shutup_GPlusPlus;  // Turn off g++ warning
public:
  enum
  {
    M_DELETE = 1 
    // Indicates that close() deletes the Tasks.  Don't change this
    // value without updating the same enum in class ACE_Stream...
  };

  // = Initialization and termination methods.
  ACE_Module (void);
  // Create an empty Module.

  ~ACE_Module (void);		
  // Shutdown the Module.

  ACE_Module (const char *module_name, 
	      ACE_Task<ACE_SYNCH_2> *writer = 0, 
	      ACE_Task<ACE_SYNCH_2> *reader = 0, 
	      void *a = 0);
  // Create an initialized module with <module_name> as its identity
  // and <reader> and <writer> as its tasks.

  int open (const char *module_name, 
	    ACE_Task<ACE_SYNCH_2> *writer = 0, 
	    ACE_Task<ACE_SYNCH_2> *reader = 0, 
	    void *a = 0);
  // Create an initialized module with <module_name> as its identity
  // and <reader> and <writer> as its tasks.

  int close (u_long flags = M_DELETE);
  // Close down the Module and its Tasks.  If the <M_DELETE> argument
  // is given then delete all the memory too.

  // = ACE_Task manipulation routines 
  ACE_Task<ACE_SYNCH_2> *writer (void);
  // Get the writer task.
  void writer (ACE_Task<ACE_SYNCH_2> *q);
  // Set the writer task.

  ACE_Task<ACE_SYNCH_2> *reader (void);
  // Get the reader task.
  void reader (ACE_Task<ACE_SYNCH_2> *q);
  // Set the reader task.

  ACE_Task<ACE_SYNCH_2> *sibling (ACE_Task<ACE_SYNCH_2> *orig); 
  // Set and get pointer to sibling ACE_Task in ACE_Module 

  // = Identify the module 
  const char *name (void) const;
  // Get the module name.
  void name (const char *);
  // Set the module name.

  // = Argument to the Tasks. 
  void *arg (void) const;
  // Get the argument passed to the tasks.

  void arg (void *);
  // Set the argument passed to the tasks.

  void link (ACE_Module<ACE_SYNCH_2> *m);
  // Link to other modules in the ustream stack 

  ACE_Module<ACE_SYNCH_2> *next (void);
  // Get the next pointer to the module above in the stream.

  void next (ACE_Module<ACE_SYNCH_2> *m);
  // Set the next pointer to the module above in the stream.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Task<ACE_SYNCH_2> *q_pair_[2];   
  // Pair of Tasks that form the "read-side" and "write-side" of the
  // ACE_Module partitioning.

  char name_[MAXNAMLEN + 1];
  // Name of the ACE_Module.

  ACE_Module<ACE_SYNCH_2> *next_;
  // Next ACE_Module in the stack.

  void *arg_;
  // Argument passed through to the reader and writer task when they
  // are opened.
};

#if defined (__ACE_INLINE__)
#include "ace/Module.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Module.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Module.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_MODULE_H */
