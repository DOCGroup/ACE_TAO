/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE
//
// = FILENAME
//    Thread_Priority.h
//
// = CREATION DATE
//    28 January 1997
//
// = AUTHOR
//    David Levine
//
// ============================================================================

#if ! defined (THREAD_PRIORITY_H)
#define THREAD_PRIORITY_H

class ACE_Thread_Priority
{
public:
  enum Priority_Class
  {
    ACE_LOW_PRIORITY_CLASS,
    ACE_NORMAL_PRIORITY_CLASS,
    ACE_HIGH_PRIORITY_CLASS,
    ACE_REALTIME_PRIORITY_CLASS
  };

  enum Thread_Priority
  {
    ACE_PRIORITY_0 = 0,
    ACE_PRIORITY_MIN = ACE_PRIORITY_0,
    ACE_PRIORITY_1,
    ACE_PRIORITY_2,
    ACE_PRIORITY_3,
    ACE_PRIORITY_4,
    ACE_PRIORITY_5,
    ACE_PRIORITY_6,
    ACE_PRIORITY_MAX = ACE_PRIORITY_6
  };
  // This enum is provided to help users create OS-independent priorities
  // (but see the NOTE below).  For applications that don't require OS-
  // independence, they can take advantage of a greater range of thread
  // priorities offered by their platform.  This can be done by casting
  // the OS-dependent priority to ACE_Thread_Priority::Thread_Priority
  // when calling the member functions that take it as an argument.
  // In other words, this class will not "break" if a Thread_Priority
  // outside of [ACE_PRIORITY_MIN .. ACE_PRIORITY_MAX] is used.
  //
  // NOTE: the following distinct ACE_Thread_Priority combinations map to
  // identical priority class/thread priority combinations on Win32.  They
  // may map to distinct priorities on other platforms.  So, if OS-independent
  // behavior is desired, applications should not depend on these having
  // either the same or different OS priorities:
  // * ACE_LOW_PRIORITY_CLASS/ACE_PRIORITY_0 ==
  //       ACE_NORMAL_PRIORITY_CLASS/ACE_PRIORITY_0 ==
  //       ACE_HIGH_PRIORITY_CLASS/ACE_PRIORITY_0
  // * ACE_NORMAL_PRIORITY_CLASS/ACE_PRIORITY_5 ==
  //       ACE_HIGH_PRIORITY_CLASS/ACE_PRIORITY_1
  // * ACE_LOW_PRIORITY_CLASS/ACE_PRIORITY_6 ==
  //       ACE_NORMAL_PRIORITY_CLASS/ACE_PRIORITY_6 ==
  //       ACE_HIGH_PRIORITY_CLASS/ACE_PRIORITY_5 ==
  //       ACE_HIGH_PRIORITY_CLASS/ACE_PRIORITY_6

  ACE_Thread_Priority (
    Priority_Class priority_class = ACE_NORMAL_PRIORITY_CLASS,
    Thread_Priority default_thread_priority = ACE_PRIORITY_MIN);
  // There can be more than one ACE_Thread_Priority instance per process,
  // e.g., one per Solaris Lightweight Process.

  ~ACE_Thread_Priority ();

  long set_priority_class (const Priority_Class);
  long set_default_thread_priority (const Thread_Priority);
  // set accessors:  return 0 on success and -1 on failure (and set errno)

  Priority_Class priority_class () const;
  Thread_Priority default_thread_priority () const;
  // get accessors for OS-independent values

  ACE_id_t os_priority_class () const;
  // get accessor for the OS-specific priority class

  ACE_pri_t os_default_thread_priority () const;
  // get accessor for the numeric default thread priority value, which
  // is OS-dependent

private:
  Priority_Class priority_class_;
  Thread_Priority default_thread_priority_;

  ACE_id_t os_priority_class_;
  ACE_pri_t os_default_thread_priority_;

  // for internal use, to convert OS-dependent priorities into
  // OS-independent priorities.
  long normalize ();
};


#if defined (__ACE_INLINE__)
#include "ace/Thread_Priority.i"
#endif /* __ACE_INLINE__ */

#endif /* THREAD_PRIORITY_H */


// EOF
