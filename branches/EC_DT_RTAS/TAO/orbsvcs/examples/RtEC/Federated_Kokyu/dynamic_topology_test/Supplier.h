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
#include "ace/Vector_T.h"

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecSchedulerC.h"
#include "orbsvcs/Event/EC_Event_Channel.h"

#include "Service_Handler.h"

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
  enum mode_t {
    NORMAL,
    FAULT_TOLERANT
  };

  typedef ACE_Vector<RtecEventChannelAdmin::ProxyPushConsumer_var> PushConsumer_Vector;
  typedef ACE_Vector<RtecScheduler::handle_t> RT_Info_Vector;

  Supplier (RtecEventComm::EventSourceID id, RtecEventComm::EventType norm_type, RtecEventComm::EventType ft_type,
            Service_Handler * handler = 0);
  // Constructor

  virtual ~Supplier(void);

  // = The RtecEventComm::PushSupplier methods

  virtual void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

  virtual void timeout_occured (ACE_ENV_SINGLE_ARG_DECL);

  void set_consumer_proxies(RtecEventComm::EventType type, RT_Info_Vector& rt_infos,
                            PushConsumer_Vector& consumer_proxies);
  //void set_consumer_proxies(RtecScheduler::handle_t rt_info, PushConsumer_Vector consumer_proxies);

  //void normal_rt_info(RT_Info_Vector& supplier_rt_info);
  //void ft_rt_info(RT_Info_Vector& supplier_rt_info);
  RT_Info_Vector& normal_rt_infos(void);
  RT_Info_Vector& ft_rt_infos(void);
  RT_Info_Vector& all_rt_infos(void);

  RtecEventComm::EventSourceID get_id(void) const;

  void mode(mode_t mode);
  mode_t mode(void) const;

  Service_Handler * handler(void) const;

protected:
  RtecEventComm::EventSourceID id_;
  RtecEventComm::EventType norm_type_;
  RtecEventComm::EventType ft_type_;

  PushConsumer_Vector normal_consumer_proxies_;
  PushConsumer_Vector ft_consumer_proxies_;

  mode_t mode_;

  RT_Info_Vector all_rt_infos_;
  RT_Info_Vector normal_rt_infos_;
  RT_Info_Vector ft_rt_infos_;

  Service_Handler *handler_;
}; //class Supplier

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
}; //class Supplier_Timeout_Handler

#endif /* SUPPLIER_H */
