/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE
//
// = FILENAME
//    Sched_Params.h
//
// = CREATION DATE
//    28 January 1997
//
// = AUTHOR
//    David Levine
//
// ============================================================================

#if !defined (ACE_SCHED_PARAMS_H)
#define ACE_SCHED_PARAMS_H

#include "ace/OS.h"

class ACE_Export ACE_Sched_Params
  // = TITLE
  //    Container for thread scheduling-related parameters.
  //
  // = DESCRIPTION
  //    ACE_Sched_Params are passed via
  //    ACE_OS::set_sched_params () to the OS to specify scheduling
  //    parameters.  It is intended that ACE_OS::set_sched_params ()
  //    be called from main () before any threads have been spawned.
  //    If spawned threads inherit their parent's priority (I think
  //    that's the case for all of our platforms), then this sets
  //    the default base priority.  Individual thread priorities can
  //    be adjusted as usual using ACE_OS::thr_prio () or via the
  //    ACE_Thread interface.  See the parameter descriptions in the
  //    private: section below.
  //
  //    NOTE:  this class does not do any checking of parameters.
  //    It is just a container class.  If it is constructed with values
  //    that are not supported on a platform, the call to
  //    ACE_OS::set_sched_params () will fail by returning -1 with EINVAL
  //    (available through ACE_OS::last_error ()).
{
public:
  typedef int Policy;

  // = Initialization and termination methods.
  ACE_Sched_Params (const Policy policy,
                    const ACE_Sched_Priority priority,
                    const int scope = ACE_SCOPE_THREAD,
                    const ACE_Time_Value &quantum = ACE_Time_Value::zero);
  // Constructor.

  ~ACE_Sched_Params (void);
  // Termination.

  // = Get/Set methods:

  // = Get/Set policy
  Policy policy (void) const;
  void policy (const Policy);

  // = Get/Set priority.
  ACE_Sched_Priority priority (void) const;
  void priority (const ACE_Sched_Priority);

  // = Get/Set scope.
  int scope (void) const;
  void scope(const int);

  // = Get/Set quantum.
  const ACE_Time_Value &quantum (void) const;
  void quantum (const ACE_Time_Value &);

  // = Accessors for OS-specific priorities.
  static int priority_min (const Policy);
  static int priority_max (const Policy);

  static int next_priority (const Policy, const int priority);
  // The next higher priority.  "Higher" refers to scheduling priority,
  // not to the priority value itself.  (On some platforms, higher scheduling
  // priority is indicated by a lower priority value.)

  static int previous_priority (const Policy, const int priority);
  // The previous, lower priority.  "Lower" refers to scheduling priority,
  // not to the priority value itself.  (On some platforms, lower scheduling
  // priority is indicated by a higher priority value.)

private:
  Policy policy_;
  // Scheduling policy.

  ACE_Sched_Priority priority_;
  // Default <priority_>: sets the priority to be used for newly
  // spawned threads.

  int scope_;
  // <scope_> must one of the following:
  //   ACE_SCOPE_PROCESS:  sets the scheduling policy for the
  //     process, and the process priority.  On some platforms,
  //     such as Win32, the scheduling policy can _only_ be
  //     set at process scope.
  //   ACE_SCOPE_LWP: lightweight process scope, only used with
  //     Solaris threads.
  //   ACE_SCOPE_THREAD: sets the scheduling policy for the thread,
  //     if the OS supports it, such as with Posix threads, and the
  //     thread priority.

  ACE_Time_Value quantum_;
  // The <quantum_> is for time slicing.  An ACE_Time_Value of 0 has
  // special significance: it means time-slicing is disabled; with
  // that, a thread that is running on a CPU will continue to run
  // until it blocks or is preempted.  Currently ignored if the OS
  // doesn't directly support time slicing, such as on VxWorks, or
  // setting the quantum (can that be done on Win32?).
};

#if defined (__ACE_INLINE__)
#include "ace/Sched_Params.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_SCHED_PARAMS_H */
