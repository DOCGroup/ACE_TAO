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

#if ! defined (ACE_THREAD_PRIORITY_H)
#define ACE_THREAD_PRIORITY_H

class ACE_Thread_Priority
  // = TITLE
  //
  // = DESCRIPTION
{
public:
  enum Priority_Class
  {
    ACE_LOW_PRIORITY_CLASS,
    ACE_NORMAL_PRIORITY_CLASS,
    ACE_HIGH_PRIORITY_CLASS,
    ACE_REALTIME_PRIORITY_CLASS
  };

  // = This enum help users create OS-independent priorities.

  // For applications that don't require OS- independence, they can
  // take advantage of a greater range of thread priorities offered by
  // their platform.  This can be done by casting the OS-dependent
  // priority to ACE_Thread_Priority::Thread_Priority when calling the
  // member functions that take it as an argument.  In other words,
  // this class will not "break" if a Thread_Priority outside of
  // [ACE_PRIORITY_MIN .. ACE_PRIORITY_MAX] is used.

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

  // = NOTE

  // the following distinct ACE_Thread_Priority combinations map to
  // identical priority class/thread priority combinations on Win32.
  // They may map to distinct priorities on other platforms.  So, if
  // OS-independent behavior is desired, applications should not
  // depend on these having either the same or different OS
  // priorities:
  //
  // * ACE_LOW_PRIORITY_CLASS/ACE_PRIORITY_0 ==
  //       ACE_NORMAL_PRIORITY_CLASS/ACE_PRIORITY_0 ==
  //       ACE_HIGH_PRIORITY_CLASS/ACE_PRIORITY_0
  // * ACE_NORMAL_PRIORITY_CLASS/ACE_PRIORITY_5 ==
  //       ACE_HIGH_PRIORITY_CLASS/ACE_PRIORITY_1
  // * ACE_LOW_PRIORITY_CLASS/ACE_PRIORITY_6 ==
  //       ACE_NORMAL_PRIORITY_CLASS/ACE_PRIORITY_6 ==
  //       ACE_HIGH_PRIORITY_CLASS/ACE_PRIORITY_5 ==
  //       ACE_HIGH_PRIORITY_CLASS/ACE_PRIORITY_6

  ACE_Thread_Priority (Priority_Class priority_class = ACE_NORMAL_PRIORITY_CLASS,
		       Thread_Priority default_thread_priority = ACE_PRIORITY_MIN);
  // There can be more than one ACE_Thread_Priority instance per
  // process, e.g., one per Solaris Lightweight Process.

  ~ACE_Thread_Priority (void);

  // = Set/Get accessors for priority class.
  long priority_class (const Priority_Class);
  // Return 0 on success and -1 on failure (and sets errno).
  Priority_Class priority_class (void) const;

  // = Set/Get accessors for default thread priority.
  long default_thread_priority (const Thread_Priority);
  Thread_Priority default_thread_priority (void) const;

  ACE_id_t os_priority_class (void) const;
  // Get accessor for the OS-specific priority class

  ACE_pri_t os_default_thread_priority (void) const;
  // Get accessor for the numeric default thread priority value, which
  // is OS-dependent.

private:
  Priority_Class priority_class_;
  // Priority class of this thread.

  Thread_Priority default_thread_priority_;
  // Default thread priority for this class.

  ACE_id_t os_priority_class_;
  // Please document me.

  ACE_pri_t os_default_thread_priority_;
  // Please document me.

  long normalize (void);
  // Convert OS-dependent priorities into OS-independent priorities.
};

#if defined (__ACE_INLINE__)
#include "ace/Thread_Priority.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_THREAD_PRIORITY_H */
