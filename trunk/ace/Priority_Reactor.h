/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Priority_Reactor.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_PRIORITY_REACTOR_H
#define ACE_PRIORITY_REACTOR_H

#include "ace/Containers.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Select_Reactor.h"

class ACE_Export ACE_Priority_Reactor : public ACE_Select_Reactor
{
  // = TITLE
  //     Implements priority based dispatching.
  //
  // = DESCRIPTION
  //     This class refines the dispatching mechanism for the
  //     Select_Reactor by taking advantage of the priority method on
  //     ACE_Event_Handler.
public:
  // = Initialization and termination methods.

  ACE_Priority_Reactor (ACE_Sig_Handler * = 0,
                        ACE_Timer_Queue * = 0);
  // Initialize <ACE_Priority_Reactor> with the default size.

  ACE_Priority_Reactor (size_t size,
                        int restart = 0,
                        ACE_Sig_Handler * = 0,
                        ACE_Timer_Queue * = 0);
  // Initialize <ACE_Priority_Reactor> with size <size>.

  virtual ~ACE_Priority_Reactor (void);
  // Close down the select_reactor and release all of its resources.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  // = Dispatching methods.

  virtual int dispatch_io_set (int number_of_active_handles,
                               int &number_dispatched,
                               int mask,
                               ACE_Handle_Set &dispatch_mask,
                               ACE_Handle_Set &ready_mask,
                               ACE_EH_PTMF callback);
  // We simply override this function to implement the priority
  // dispatching.

private:
  void init_bucket (void);
  // A small helper to initialize the bucket.

  typedef ACE_Unbounded_Queue<ACE_Event_Tuple> QUEUE;
  QUEUE** bucket_;
  // There is a queue per-priority, which simply holds the
  // Event_Handlers until we knwo who goes first.

  ACE_Allocator* tuple_allocator_;
  // The queues themselves use this allocator to minimize dynamic
  // memory usage.

  ACE_Priority_Reactor (const ACE_Select_Reactor &);
  ACE_Priority_Reactor &operator = (const ACE_Select_Reactor &);
  // Deny access since member-wise won't work...
};

#if defined (__ACE_INLINE__)
#include "ace/Priority_Reactor.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_PRIORITY_REACTOR_H */
