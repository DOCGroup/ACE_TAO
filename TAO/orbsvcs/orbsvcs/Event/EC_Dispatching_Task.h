/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Dispatching_Task
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//   Based on previous work by Tim Harrison (harrison@cs.wustl.edu)
//   and other members of the DOC group.
//   More details can be found in:
//   http://www.cs.wustl.edu/~schmidt/oopsla.ps.gz
//   http://www.cs.wustl.edu/~schmidt/JSAC-98.ps.gz
//
//
// ============================================================================

#ifndef TAO_EC_DISPATCHING_TASK_H
#define TAO_EC_DISPATCHING_TASK_H

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecEventCommC.h"

class TAO_EC_ProxyPushSupplier;

class TAO_ORBSVCS_Export TAO_EC_Dispatching_Task : public ACE_Task<ACE_SYNCH>
{
  // = TITLE
  //   Implement the dispatching queues for FIFO and Priority
  //   dispatching.
  //
  // = DESCRIPTION
  //
public:
  TAO_EC_Dispatching_Task (ACE_Thread_Manager* thr_manager = 0);
  // Constructor

  virtual int svc (void);
  // Process the events in the queue.
};

// ****************************************************************

class TAO_ORBSVCS_Export TAO_EC_Dispatch_Command : public ACE_Message_Block
{
public:
  virtual ~TAO_EC_Dispatch_Command (void);
  // Destructor

  virtual int execute (CORBA::Environment&) = 0;
  // Command callback
};

// ****************************************************************

class TAO_ORBSVCS_Export TAO_EC_Shutdown_Command : public TAO_EC_Dispatch_Command
{
public:
  TAO_EC_Shutdown_Command (void);
  // Constructor

  virtual int execute (CORBA::Environment&);
  // Command callback
};

// ****************************************************************

class TAO_ORBSVCS_Export TAO_EC_Push_Command : public TAO_EC_Dispatch_Command
{
public:
  TAO_EC_Push_Command (TAO_EC_ProxyPushSupplier* proxy,
                       RtecEventComm::EventSet& event);
  // Constructor

  virtual int execute (CORBA::Environment&);
  // Command callback

private:
  TAO_EC_ProxyPushSupplier* proxy_;
  // The proxy

  RtecEventComm::EventSet event_;
  // The event
};

#if defined (__ACE_INLINE__)
#include "EC_Dispatching_Task.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_DISPATCHING_TASK_H */
