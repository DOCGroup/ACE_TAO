/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   CEC_Dispatching_Task.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_CEC_DISPATCHING_TASK_H
#define TAO_CEC_DISPATCHING_TASK_H
#include /**/ "ace/pre.h"

#include "ace/Task.h"
#include "ace/Message_Block.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosEvent/event_export.h"
#include "CEC_ProxyPushSupplier.h"

/**
 * @class TAO_CEC_Dispatching_Task
 *
 * @brief Implement the dispatching queues for FIFO and Priority
 * dispatching.
 *
 */
class TAO_Event_Export TAO_CEC_Dispatching_Task : public ACE_Task<ACE_SYNCH>
{
public:
  /// Constructor
  TAO_CEC_Dispatching_Task (ACE_Thread_Manager* thr_manager = 0);

  /// Process the events in the queue.
  virtual int svc (void);

  virtual void push (TAO_CEC_ProxyPushSupplier *proxy,
                     CORBA::Any& event
                     ACE_ENV_ARG_DECL);

private:
  /// An per-task allocator
  ACE_Allocator *allocator_;

  /// Helper data structure to minimize memory allocations...
  ACE_Locked_Data_Block<ACE_Lock_Adapter<TAO_SYNCH_MUTEX> > data_block_;
};

// ****************************************************************

class TAO_Event_Export TAO_CEC_Dispatch_Command : public ACE_Message_Block
{
public:
  /// Constructor, it will allocate its own data block
  TAO_CEC_Dispatch_Command (ACE_Allocator *mb_allocator = 0);

  /// Constructor, it assumes ownership of the data block
  TAO_CEC_Dispatch_Command (ACE_Data_Block*,
                           ACE_Allocator *mb_allocator = 0);

  /// Destructor
  virtual ~TAO_CEC_Dispatch_Command (void);

  /// Command callback
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL_NOT_USED) = 0;
};

// ****************************************************************

class TAO_Event_Export TAO_CEC_Shutdown_Task_Command : public TAO_CEC_Dispatch_Command
{
public:
  /// Constructor
  TAO_CEC_Shutdown_Task_Command (ACE_Allocator *mb_allocator = 0);

  /// Command callback
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL_NOT_USED);
};

// ****************************************************************

class TAO_Event_Export TAO_CEC_Push_Command : public TAO_CEC_Dispatch_Command
{
public:
  /// Constructor
  TAO_CEC_Push_Command (TAO_CEC_ProxyPushSupplier* proxy,
                        CORBA::Any& event,
                        ACE_Data_Block* data_block,
                        ACE_Allocator *mb_allocator);

  /// Destructor
  virtual ~TAO_CEC_Push_Command (void);

  /// Command callback
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL_NOT_USED);

private:
  /// The proxy
  TAO_CEC_ProxyPushSupplier* proxy_;

  /// The event
  CORBA::Any event_;
};

#if defined (__ACE_INLINE__)
#include "CEC_Dispatching_Task.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_CEC_DISPATCHING_TASK_H */
