/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Cos Event Channel
//
// = FILENAME
//   CEC_Dispatching_Task
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_CEC_DISPATCHING_TASK_H
#define TAO_CEC_DISPATCHING_TASK_H

#include "ace/Task.h"
#include "ace/Message_Block.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/orbsvcs_export.h"
#include "tao/corba.h"

class TAO_CEC_ProxyPushSupplier;

class TAO_ORBSVCS_Export TAO_CEC_Dispatching_Task : public ACE_Task<ACE_SYNCH>
{
  // = TITLE
  //   Implement the dispatching queues for FIFO and Priority
  //   dispatching.
  //
  // = DESCRIPTION
  //
public:
  TAO_CEC_Dispatching_Task (ACE_Thread_Manager* thr_manager = 0);
  // Constructor

  virtual int svc (void);
  // Process the events in the queue.

  virtual void push (TAO_CEC_ProxyPushSupplier *proxy,
                     CORBA::Any& event,
                     CORBA::Environment &env);

private:
  ACE_Allocator *allocator_;
  // An per-task allocator

  ACE_Locked_Data_Block<ACE_Lock_Adapter<ACE_SYNCH_MUTEX> > data_block_;
  // Helper data structure to minimize memory allocations...
};

// ****************************************************************

class TAO_ORBSVCS_Export TAO_CEC_Dispatch_Command : public ACE_Message_Block
{
public:
  TAO_CEC_Dispatch_Command (ACE_Allocator *mb_allocator = 0);
  // Constructor, it will allocate its own data block

  TAO_CEC_Dispatch_Command (ACE_Data_Block*,
                           ACE_Allocator *mb_allocator = 0);
  // Constructor, it assumes ownership of the data block

  virtual ~TAO_CEC_Dispatch_Command (void);
  // Destructor

  virtual int execute (CORBA::Environment&) = 0;
  // Command callback
};

// ****************************************************************

class TAO_ORBSVCS_Export TAO_CEC_Shutdown_Task_Command : public TAO_CEC_Dispatch_Command
{
public:
  TAO_CEC_Shutdown_Task_Command (ACE_Allocator *mb_allocator = 0);
  // Constructor

  virtual int execute (CORBA::Environment&);
  // Command callback
};

// ****************************************************************

class TAO_ORBSVCS_Export TAO_CEC_Push_Command : public TAO_CEC_Dispatch_Command
{
public:
  TAO_CEC_Push_Command (TAO_CEC_ProxyPushSupplier* proxy,
                        CORBA::Any& event,
                        ACE_Data_Block* data_block,
                        ACE_Allocator *mb_allocator);
  // Constructor

  virtual int execute (CORBA::Environment&);
  // Command callback

private:
  TAO_CEC_ProxyPushSupplier* proxy_;
  // The proxy

  CORBA::Any event_;
  // The event
};

#if defined (__ACE_INLINE__)
#include "CEC_Dispatching_Task.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_CEC_DISPATCHING_TASK_H */
