/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Supplier
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef SUPPLIER_H
#define SUPPLIER_H

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/Event/EC_Event_Channel.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Supplier
 *
 * @brief Simple supplier object
 *
 * This class is a supplier of events.
 * It simply register for two event typesone event type
 * The class is just a helper to simplify common tasks in EC
 * tests, such as subscribing for a range of events, disconnecting
 * from the EC, informing the driver of shutdown messages, etc.
 * There are several ways to connect and disconnect this class,
 * and it is up to the driver program to use the right one.
 */
class Supplier : public POA_RtecEventComm::PushSupplier
{
public:
  /// Constructor
  Supplier (RtecEventComm::EventSourceID id,
            const RtecEventChannelAdmin::ProxyPushConsumer_ptr consumer_proxy);

  // = The RtecEventComm::PushSupplier methods

  /// The skeleton methods.
  virtual void disconnect_push_supplier (void);

  void timeout_occured (void);

private:
  RtecEventComm::EventSourceID id_;
  const RtecEventChannelAdmin::ProxyPushConsumer_ptr consumer_proxy_;
};

/**
 * @class Timeout_Consumer
 *
 * @brief Timer consumer object
 *
 * This class is a consumer of timeout events from EC.
 * It registers for timeout event with EC and calls
 * the
 */
class Timeout_Consumer : public POA_RtecEventComm::PushConsumer
{
public:
  /// Constructor
  Timeout_Consumer (Supplier * supplier_impl);

  // = The RtecEventComm::PushConsumer methods

  /// The skeleton methods.
  virtual void push (const RtecEventComm::EventSet& events);
  virtual void disconnect_push_consumer (void);

private:
  Supplier *supplier_impl_;
};

#endif /* SUPPLIER_H */
