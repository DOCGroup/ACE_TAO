/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE
//
// = FILENAME
//    Scheduling_Params.h
//
// = CREATION DATE
//    28 January 1997
//
// = AUTHOR
//    David Levine
//
// ============================================================================

#if ! defined (SCHEDULING_PARAMS_H)
#define SCHEDULING_PARAMS_H

#include "ace/Thread_Priority.h"
#include "ace/OS.h"  // for ACE_Time_Value and ACE_SCOPE_PROCESS

class ACE_Scheduling_Params
{
public:
  ACE_Scheduling_Params (
    const ACE_Thread_Priority &priority = ACE_Thread_Priority (),
    const int scope = ACE_SCOPE_PROCESS,
    const ACE_Time_Value &quantum = ACE_Time_Value::zero);
  // default priority:  sets the priority to be used for newly spawned threads.
  // It is intended that this function be called from main () before
  // any threads have been spawned.  If spawned threads inherit their
  // parent's priority (I think that's the case for all of our platforms),
  // then this sets the default base priority.  Individual thread
  // priorities can be adjusted as usual.
  //
  // "scope" must be either ACE_SCOPE_PROCESS or ACE_SCOPE_LWP (which is
  // only used on Solaris, and ignored on Win32 and VxWorks)
  //
  // The "quantum" is for time slicing.  An ACE_Time_Value of 0 has
  // special significance:  it means time-slicing is disabled; with
  // that, a thread that is running on a CPU will continue to run until
  // it blocks or is preempted.  Currently ignored if the OS doesn't
  // directly support time slicing, such as on VxWorks, or setting the
  // quantum (can that be done on Win32?).

  ~ACE_Scheduling_Params ();

  // get/set accessors:

  const ACE_Thread_Priority &priority () const;
  void set_priority (const ACE_Thread_Priority &);

  int scope () const;
  void set_scope(const int);

  const ACE_Time_Value &quantum () const;
  void set_quantum (const ACE_Time_Value &);

private:
  ACE_Thread_Priority priority_;
  int scope_;
  ACE_Time_Value quantum_;
};


#if defined (__ACE_INLINE__)
#include "ace/Scheduling_Params.i"
#endif /* __ACE_INLINE__ */

#endif /* SCHEDULING_PARAMS_H */


// EOF
