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
    M_DELETE_NONE = 0,
    // Indicates that close() should not delete any Tasks.

    M_DELETE_READER = 1,
    // Indicates that close() should delete the writer Task.

    M_DELETE_WRITER = 2,
    // Indicates that close() should delete the reader Task.

    M_DELETE = 3 
    // Indicates that close() deletes the Tasks.  Don't change this
    // value without updating the same enum in class ACE_Stream...
    // The <M_DELETE_READER> and <M_DELETE_WRITER> flags may be or'ed
    // together.
  };

  // = Initialization and termination methods.
  ACE_Module (void);
  // Create an empty Module.

  ~ACE_Module (void);		
  // Shutdown the Module.

  ACE_Module (const char *module_name, 
	      ACE_Task<ACE_SYNCH_2> *writer = 0, 
	      ACE_Task<ACE_SYNCH_2> *reader = 0, 
	      void *args = 0,
	      int flags = M_DELETE);
  // Create an initialized module with <module_name> as its identity
  // and <reader> and <writer> as its tasks.

  int open (const char *module_name, 
	    ACE_Task<ACE_SYNCH_2> *writer = 0, 
	    ACE_Task<ACE_SYNCH_2> *reader = 0, 
	    void *a = 0,
	    int flags = M_DELETE);
  // Create an initialized module with <module_name> as its identity
  // and <reader> and <writer> as its tasks.  Previously register
  // reader or writers or closed down and deleted according to the
  // value of flags_.  Should not be called from within
  // ACE_Task::module_closed().

  int close (int flags = M_DELETE_NONE);
  // Close down the Module and its Tasks.  The flags argument can be
  // used to override the default behaviour, which depends on previous
  // <flags> values in calls to c'tor(), open(), reader() and
  // writer().  A previous value M_DELETE[_XXX] can not be overridden.
  // Should not be called from within ACE_Task::module_closed().
  
  // = ACE_Task manipulation routines 
  ACE_Task<ACE_SYNCH_2> *writer (void);
  // Get the writer task.

  void writer (ACE_Task<ACE_SYNCH_2> *q, int flags = M_DELETE_WRITER);
  // Set the writer task. <flags> can be used to indicate that the
  // module should delete the writer during a call to close or to the
  // destructor. If a previous writer exists, it is closed.  It may
  // also be deleted, depending on the old flags_ value.  Should not
  // be called from within ACE_Task::module_closed().

  ACE_Task<ACE_SYNCH_2> *reader (void);
  // Get the reader task.

  void reader (ACE_Task<ACE_SYNCH_2> *q, int flags = M_DELETE_READER);
  // Set the reader task. <flags> can be used to indicate that the
  // module should delete the reader during a call to close or to the
  // destructor. If a previous reader exists, it is closed.  It may
  // also be deleted, depending on the old flags_ value.  Should not
  // be called from within ACE_Task::module_closed()

  ACE_Task<ACE_SYNCH_2> *sibling (ACE_Task<ACE_SYNCH_2> *orig); 
  // Set and get pointer to sibling ACE_Task in ACE_Module.

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
  int close_i (int which, int flags);
  // Implements the close operation for either the reader or the
  // writer task (depending on <which>).

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

  int flags_;
  // Holds flags which are used to determine if the reader and writer
  // task have to be deleted on exit
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
