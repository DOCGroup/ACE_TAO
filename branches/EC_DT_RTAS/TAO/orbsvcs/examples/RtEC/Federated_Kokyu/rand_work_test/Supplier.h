/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel examples
//
// = FILENAME
//   Supplier
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef SUPPLIER_H
#define SUPPLIER_H

#include "ace/Event_Handler.h"

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecSchedulerC.h"
#include "orbsvcs/Event/EC_Event_Channel.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Supplier : public POA_RtecEventComm::PushSupplier
{
  // = TITLE
  //   Simple supplier object
  //
  // = DESCRIPTION
  //   This class is a supplier of events.
  //   It simply register for two event typesone event type
  //   The class is just a helper to simplify common tasks in EC
  //   tests, such as subscribing for a range of events, disconnecting
  //   from the EC, informing the driver of shutdown messages, etc.
  //
  //   There are several ways to connect and disconnect this class,
  //   and it is up to the driver program to use the right one.
  //
public:
  Supplier (RtecEventComm::EventSourceID id);
  // Constructor

  // = The RtecEventComm::PushSupplier methods

  virtual void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

  void timeout_occured (ACE_ENV_SINGLE_ARG_DECL);

  void set_consumer_proxy(const RtecEventChannelAdmin::ProxyPushConsumer_ptr consumer_proxy);

  void rt_info(RtecScheduler::handle_t supplier_rt_info);
  RtecScheduler::handle_t rt_info(void) const;

  const RtecEventComm::EventSourceID get_id(void) const;

private:
  RtecEventComm::EventSourceID id_;
  RtecEventChannelAdmin::ProxyPushConsumer_ptr consumer_proxy_;
  RtecScheduler::handle_t rt_info_;
};

class Timeout_Consumer : public POA_RtecEventComm::PushConsumer
{
  // = TITLE
  //   Timer consumer object
  //
  // = DESCRIPTION
  //   This class is a consumer of timeout events from EC.
  //   It registers for timeout event with EC and calls
  //   the
  //
public:
  Timeout_Consumer (Supplier * supplier_impl);
  // Constructor

  // = The RtecEventComm::PushConsumer methods

  virtual void push (const RtecEventComm::EventSet& events
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

private:
  Supplier *supplier_impl_;
}; //class Timeout_Consumer

class Supplier_Timeout_Handler : public ACE_Event_Handler
{
  // = TITLE
  //   Timer consumer object
  //
  // = DESCRIPTION
  //   This class is a consumer of timeout events from EC.
  //   It registers for timeout event with EC and calls
  //   the
  //
public:
  Supplier_Timeout_Handler (Supplier * supplier_impl);
  // Constructor

  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *act = 0);

private:
  Supplier *supplier_impl_;
};

#endif /* SUPPLIER_H */
