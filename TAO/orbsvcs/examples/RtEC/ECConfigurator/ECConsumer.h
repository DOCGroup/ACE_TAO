/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel examples
//
// = FILENAME
//   ECConsumer
//
// = AUTHOR
//   Bryan Thrall
//
// ============================================================================

#ifndef CONSUMER_H
#define CONSUMER_H

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecSchedulerC.h"

#include "ace/Time_Value.h"

#include "ECSupplier.h"
#include "Service_Handler.h"

#include <vector>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ECConsumer : public POA_RtecEventComm::PushConsumer
{
  // = TITLE
  //   Simple consumer object
  //
  // = DESCRIPTION
  //   This class is a consumer of events.
  //   The class is just a helper to simplify common tasks in EC
  //   tests, such as subscribing for a range of events, disconnecting
  //   from the EC, informing the driver of shutdown messages, etc.
  //
  //   There are several ways to connect and disconnect this class,
  //   and it is up to the driver program to use the right one.
  //
public:
  typedef RtecEventComm::EventSourceID SourceID;
  typedef RtecEventComm::EventType EventType;
  typedef std::vector<EventType> EventTypeVector;
  typedef RtecScheduler::handle_t InfoHandle;
  typedef std::vector<ECSupplier*> SupplierVector;

  ECConsumer (EventTypeVector &sub_types,
              ACE_Time_Value& worktime, Service_Handler *handler = 0);
  // Constructor

  ECConsumer (EventTypeVector &sub_types, Service_Handler *handler = 0);
  // Constructor

  virtual ~ECConsumer(void);

  // = The RtecEventComm::PushConsumer methods

  virtual void push (const RtecEventComm::EventSet& events
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

  void setWorkTime(ACE_Time_Value& worktime);

  void rt_info(InfoHandle consumer_rt_info);
  InfoHandle rt_info(void);

  void handler(Service_Handler * handler);

  Service_Handler * handler(void) const;

  void pushDependant (ECSupplier *dep);

protected:
  ACE_Time_Value _worktime;

  SupplierVector dependants_;
  InfoHandle rt_info_;

  Service_Handler * handler_;

  EventTypeVector sub_types_;
};

#endif /* CONSUMER_H */
