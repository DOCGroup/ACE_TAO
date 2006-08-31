// -*- C++ -*-

//=============================================================================
/**
 * @file    EventGuard_T.h
 *
 * $Id$
 *
 * @author James H. Hill <hillj@isis.vanderbilt.edu>
 */
//=============================================================================

#ifndef _CUTS_EVENT_GUARD_T_H_
#define _CUTS_EVENT_GUARD_T_H_

#include "cuts/Event_Guard.h"

//=============================================================================
/**
 * @class CUTS_Event_Guard_T
 */
//=============================================================================

template <typename COMPONENT>
class CUTS_Event_Guard_T :
  public CUTS_Event_Guard
{
public:
  /// Type definition for the component owning the guard.
  typedef COMPONENT Component_Type;

  /// Type definition for the event method of the component.
  typedef void (COMPONENT::* Method_Pointer) (void);

  /// Constructor.
  CUTS_Event_Guard_T (Component_Type * parent, Method_Pointer method);

  /// Destructor.
  ~CUTS_Event_Guard_T (void);

  /**
   * Execute the guarded method. This will only allow one thread of
   * of execution to invoke this method when a guard is enabled. The
   * thread that is able to execute this method will reset all the
   * event counters once it is done.
   */
  void try_execute (void);

  /**
   * Execute the guarded method. This will only allow one thread of
   * of execution to invoke this method when a guard is enabled. The
   * thread that is able to execute this method will reset all the
   * event counters once it is done.
   */
  void try_execute_and_reset (void);

private:
  /// Parent of the method invocation.
  Component_Type * parent_;

  /// Component method protected by this guard.
  Method_Pointer method_;

  /// Thread lock to prevent multiple invocations of execute.
  ACE_Thread_Mutex exclusive_;
};

#include "cuts/Event_Guard_T.cpp"

#endif  // !defined _CUTS_EVENT_GUARD_T_H_
