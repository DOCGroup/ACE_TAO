/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel examples
//
// = FILENAME
//   ECSupplier
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef SUPPLIER_H
#define SUPPLIER_H

#include "ace/Event_Handler.h"

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecSchedulerC.h"
#include "orbsvcs/Event/EC_Event_Channel.h"

#include "Service_Handler.h"

#include <vector>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ECSupplier : public POA_RtecEventComm::PushSupplier
{
  // = TITLE
  //   Simple supplier object
  //
  // = DESCRIPTION
  //   This class is a supplier of events.
  //   The class is just a helper to simplify common tasks in EC
  //   tests, such as subscribing for a range of events, disconnecting
  //   from the EC, informing the driver of shutdown messages, etc.
  //
  //   There are several ways to connect and disconnect this class,
  //   and it is up to the driver program to use the right one.
  //
public:
  typedef RtecEventChannelAdmin::ProxyPushConsumer ConsumerProxy;
  typedef RtecEventComm::EventSourceID SourceID;
  typedef RtecEventComm::EventType EventType;
  typedef std::vector<EventType> EventTypeVector;
  typedef RtecScheduler::handle_t InfoHandle;

  ECSupplier (SourceID id, const EventTypeVector &pub_types,
            Service_Handler * handler = 0);
  // Constructor

  virtual ~ECSupplier(void);

  // = The RtecEventComm::PushSupplier methods

  virtual void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

  virtual void timeout_occured (ACE_ENV_SINGLE_ARG_DECL);

  void set_consumer_proxy(ConsumerProxy::_ptr_type consumer_proxies);

  void rt_info(InfoHandle supplier_rt_info);
  InfoHandle rt_info(void);

  SourceID get_id(void) const;

  Service_Handler * handler(void) const;

  const EventTypeVector &getPublishedTypes (void) const;

protected:
  SourceID id_;
  EventTypeVector pub_types_;

  ConsumerProxy::_var_type consumer_proxy_;

  InfoHandle rt_info_;

  Service_Handler *handler_;
}; //class ECSupplier
/*
class ECTimeout_Consumer : public POA_RtecEventComm::PushConsumer
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
  ECTimeout_Consumer (ECSupplier * supplier_impl);
  // Constructor

  // = The RtecEventComm::PushConsumer methods

  virtual void push (const RtecEventComm::EventSet& events
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

private:
  ECSupplier *supplier_impl_;
}; //class ECTimeout_Consumer
*/
class ECSupplier_Timeout_Handler : public ACE_Event_Handler
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
  ECSupplier_Timeout_Handler (ECSupplier * supplier_impl);
  // Constructor

  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *act = 0);

private:
  ECSupplier *supplier_impl_;
}; //class ECSupplier_Timeout_Handler

#endif /* SUPPLIER_H */
