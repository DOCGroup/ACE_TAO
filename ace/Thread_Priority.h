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

#include "ace/OS.h"

class ACE_Export ACE_Thread_Priority
  // = TITLE
  //    Provides OS-independent scheduling priority classes and thread
  //    priorities.
  //
  // = DESCRIPTION
  //    ACE_Thread_Priority provides OS-independent scheduling priority
  //    classes and thread priority values.  Applications that use
  //    these priority values don't have to be concerned that lower
  //    values indicate higher thread priorities on some platforms but
  //    lower on others.  ACE_Thread_Priorities are typically used with
  //    ACE_Scheduling_Parameters to specify scheduling behavior to the
  //    OS.
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
  // = This enum helps users create OS-independent priorities.
  //
  // For applications that don't require OS- independence, they can
  // take advantage of a greater range of thread priorities offered by
  // their platform.  This can be done by casting the OS-specific
  // priority to ACE_Thread_Priority::Thread_Priority when calling the
  // member functions that take it as an argument.  In other words,
  // this class will not "break" if a Thread_Priority outside of
  // [ACE_PRIORITY_MIN .. ACE_PRIORITY_MAX] is used.

  // = NOTE
  //
  // the following distinct ACE_Thread_Priority combinations map to
  // identical priority class/thread priority combinations on Win32.
  // They may map to distinct priorities on other platforms.  So, if
  // OS-independent behavior is desired, applications should not
  // depend on these having either the same or different OS-specific
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
  int priority_class (const Priority_Class);
  // Return 0 on success and -1 on failure (and sets errno).
  Priority_Class priority_class (void) const;

  // = Set/Get accessors for default thread priority.
  int default_thread_priority (const Thread_Priority);
  // Return 0 on success and -1 on failure (and sets errno).
  Thread_Priority default_thread_priority (void) const;

  // = Increment/decrement operations.  These increment/decrement
  //   the default thread priority, adjusting the priority class if
  //   necessary.  Return 0 on success if the priority class did not
  //   have to be adjusted; 1 on success if the priority class did have
  //   to be adjusted; or -1 if the the priority class/default priority
  //   were already at their upper/lower limit (and in which case they
  //   would not have been changed).
  //   These functions currently don't take into account the priority
  //   overlap described above on Windows NT.
  int increment (void);
  int decrement (void);

  ACE_id_t os_priority_class (void) const;
  // Get accessor for the OS-specific priority class.

  ACE_pri_t os_default_thread_priority (void) const;
  // Get accessor for the numeric default thread priority value, which
  // is OS-dependent.

private:
  Priority_Class priority_class_;
  // Priority class of this thread, OS-independent.

  Thread_Priority default_thread_priority_;
  // Default thread priority value, OS-independent.

  ACE_id_t os_priority_class_;
  // OS-specific value of priority_class_.

  ACE_pri_t os_default_thread_priority_;
  // OS-specific value of default_thread_priority_.

  int convert_to_os_priority (void);
  // Convert OS-independent priorities into OS-specific priorities.
};

#if defined (__ACE_INLINE__)
#include "ace/Thread_Priority.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_THREAD_PRIORITY_H */
