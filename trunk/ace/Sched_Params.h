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

#if !defined (ACE_SCHEDULING_PARAMS_H)
#define ACE_SCHEDULING_PARAMS_H

#include "ace/Thread_Priority.h"
// for ACE_Time_Value and ACE_SCOPE_PROCESS
#include "ace/OS.h"

class ACE_Export ACE_Scheduling_Params
  // = TITLE
  //    Container for thread scheduling-related parameters.
  //
  // = DESCRIPTION
  //    ACE_Scheduling_Params are passed via
  //    ACE_OS::set_sched_params () to the OS to specify scheduling
  //    parameters.  It is intended that ACE_OS::set_sched_params ()
  //    be called from main () before any threads have been spawned.
  //    If spawned threads inherit their parent's priority (I think
  //    that's the case for all of our platforms), then this sets
  //    the default base priority.  Individual thread priorities can
  //    be adjusted as usual using ACE_OS::thr_prio () or via the
  //    ACE_Thread interface.  See the parameter descriptions in the
  //    private: section below.
{
public:
  // = Initialization and termination methods.
  ACE_Scheduling_Params (const ACE_Thread_Priority &priority = ACE_Thread_Priority (),
                         const int scope = ACE_SCOPE_PROCESS,
                         const ACE_Time_Value &quantum = ACE_Time_Value::zero);
  // Constructor.

  ~ACE_Scheduling_Params (void);
  // Termination.

  // = Get/Set methods:

  // = Get/Set priority.
  const ACE_Thread_Priority &priority (void) const;
  void set_priority (const ACE_Thread_Priority &);

  // = Get/Set scope.
  int scope (void) const;
  void set_scope(const int);

  // = Get/Set quantum.
  const ACE_Time_Value &quantum (void) const;
  void set_quantum (const ACE_Time_Value &);

private:
  ACE_Thread_Priority priority_;
  // Default <priority_>: sets the priority to be used for newly
  // spawned threads.

  int scope_;
  // <scope_> must be either ACE_SCOPE_PROCESS or ACE_SCOPE_LWP (which
  // is only used on Solaris, and ignored on Win32 and VxWorks)

  ACE_Time_Value quantum_;
  // The <quantum_> is for time slicing.  An ACE_Time_Value of 0 has
  // special significance: it means time-slicing is disabled; with
  // that, a thread that is running on a CPU will continue to run
  // until it blocks or is preempted.  Currently ignored if the OS
  // doesn't directly support time slicing, such as on VxWorks, or
  // setting the quantum (can that be done on Win32?).
};

#if defined (__ACE_INLINE__)
#include "ace/Scheduling_Params.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_SCHEDULING_PARAMS_H */
