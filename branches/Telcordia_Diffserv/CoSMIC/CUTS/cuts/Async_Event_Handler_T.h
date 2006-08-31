// -*- C++ -*-

//=============================================================================
/**
 * @file        Async_Event_Hanlder_T.h
 *
 * $Id$
 *
 * @author      James H. Hill
 */
//=============================================================================

#ifndef _CUTS_ASYNC_EVENT_HANDLER_T_H_
#define _CUTS_ASYNC_EVENT_HANDLER_T_H_

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include "cuts/config.h"
#include "cuts/Event_Handler_T.h"
#include "ace/Atomic_Op.h"
#include "ace/Condition_Thread_Mutex.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Thread_Manager.h"
#include "ace/Unbounded_Queue.h"
#include "ace/Unbounded_Set.h"

//=============================================================================
/**
 * @class CUTS_Async_Event_Handler_T
 *
 * Implementation of an asynchronous event handler.
 */
//=============================================================================

template <typename COMPONENT, typename EVENTTYPE>
class CUTS_Async_Event_Handler_T :
  public CUTS_Event_Handler_Base_T <COMPONENT, EVENTTYPE>
{
public:
  /// Type definition for the super class.
  typedef CUTS_Event_Handler_Base_T <COMPONENT,
                                     EVENTTYPE> Event_Handler_Base;

  /// Type definition of the event handler configuration.
  typedef typename Event_Handler_Base::Config_Type Config_Type;

  /// Type definition for the component owning the event handler.
  typedef typename Event_Handler_Base::Component_Type Component_Type;

  /// Type definition for the event type.
  typedef typename Event_Handler_Base::Event_Type Event_Type;

  /// Type definition for the callback method.
  typedef typename Event_Handler_Base::Event_Method Event_Method;

  /**
   * Constructor.
   *
   * @param[in]     config       Shared configuration for the handler.
   */
  CUTS_Async_Event_Handler_T (Config_Type & config);

  /// Destructor.
  virtual ~CUTS_Async_Event_Handler_T (void);

  /// Activate the event handler.
  virtual void activate (void);

  /// Deactivate the event handler.
  virtual void deactivate (void);

  /// Handle the event. This will invoke the callback method.
  virtual void handle_event (EVENTTYPE * ev);

  /**
   * Set the priority.
   *
   * @param[in]     prio      The new priority.
   */
  virtual void priority (int prio);

  /**
   * Get the priority.
   *
   * @return        The current priority.
   */
  virtual int priority (void) const;

  /**
   * Set the number of threads.
   *
   * @param[in]     n     Number of threads.
   */
  virtual void thread_count (size_t n);

  /**
   * Get the number of threads.
   *
   * @return        The number of threads.
   */
  virtual size_t thread_count (void) const;

private:
  struct cuts_thread_t
  {
    /// Handle the thread (used for setting priority).
    ACE_hthread_t handle_;

    /// Active status for the thread.
    bool * active_;
  };

  /// Type definition of this object.
  typedef CUTS_Async_Event_Handler_T <COMPONENT, EVENTTYPE> THIS;

  enum Thread_Groups
  {
    /// Group id of active threads.
    GRP_ACTIVE = 1,

    /// Group id of kill threads.
    GRP_KILL   = 2
  };

  /// Spawn helper method.
  int spawn_i (size_t count);

  /// Service handler for the event handler.
  static ACE_THR_FUNC_RETURN event_loop (void * param);

  /**
   * Register a thread with the event handler.
   *
   * @param[in]     thr_id        Id of the thread.
   * @param[in]     active        Pointer to the threads status.
   */
  int register_thread (bool * active);

  /**
   * Unregister an existing thread. This is should only be
   * called by an exiting thread.
   *
   * @param[in]       thr_id      Id of the thread.
   */
  int unregister_thread (void);

  /// Priority of the event handler.
  int priority_;

  /// Collection of free activation records.
  ACE_Message_Queue_Ex <EVENTTYPE, ACE_MT_SYNCH> event_queue_;

  /// Thread manager for the event handler.
  ACE_Thread_Manager thr_manager_;

  /// Type definition of a mapping of threads to status.
  typedef ACE_Map_Manager <ACE_thread_t,
                           cuts_thread_t,
                           ACE_RW_Thread_Mutex> Thread_Map;

  /// Mapping of threads to active status.
  Thread_Map thread_map_;

  /// Collection of threads waiting for exit.
  typedef ACE_Unbounded_Set <ACE_hthread_t> Kill_List;

  /// Listing of threads being killed.
  Kill_List kill_list_;
};

#if defined (__CUTS_INLINE__)
#include "cuts/Async_Event_Handler_T.inl"
#endif

#include "cuts/Async_Event_Handler_T.cpp"

#endif  // !defined _CUTS_ASYNC_EVENT_HANDLER_T_H_
