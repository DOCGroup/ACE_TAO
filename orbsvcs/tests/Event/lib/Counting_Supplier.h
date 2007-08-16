/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Counting_Supplier.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef EC_COUNTING_SUPPLIER_H
#define EC_COUNTING_SUPPLIER_H

#include "ectest_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Channel_Clients_T.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "ace/Task.h"

/**
 * @class EC_Counting_Supplier
 *
 * @brief Simple supplier object
 *
 * This class is a supplier of events.
 */
class EC_Test_Export EC_Counting_Supplier : public POA_RtecEventComm::PushSupplier
{
public:
  /// Constructor
  EC_Counting_Supplier (void);

  // = The RtecEventComm::PushSupplier methods

  /**
   * Connect as a consumer to receive a TIMEOUT every <period>
   * milliseconds.
   * The class pushes an event (in its supplier role) every time it
   * receives the timeout.
   */
  void activate (RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
                 int period);
  void deactivate (void);

  /// Simple connect/disconnect methods..
  void connect (RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin,
                int published_source,
                int published_type,
                int event_source,
                int event_type);
  void connect (RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin,
                const RtecEventChannelAdmin::SupplierQOS &qos);
  void disconnect (void);

  /// The Consumer side methods.
  void push (const RtecEventComm::EventSet& events);
  void disconnect_push_consumer (void);

  /// The skeleton methods.
  virtual void disconnect_push_supplier (void);

  /// Count the number of events sent
  CORBA::ULong event_count;

  /// Count the number of disconnect_push_supplier calls
  CORBA::ULong disconnect_count;

private:
  /// Adapter...
  ACE_PushConsumer_Adapter<EC_Counting_Supplier> consumer_adapter_;

  /// The adapter proxy...
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;

  /// Our proxy
  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;

  /// The event source
  int event_source_;

  /// The event type
  int event_type_;
};

class EC_Test_Export EC_Counting_Supplier_Task : public ACE_Task_Base
{
public:
  /// Create the task...
  EC_Counting_Supplier_Task (EC_Counting_Supplier *supplier);

  // = Check the ACE_Task_Base documentation.
  int svc (void);

  void stop (void);
  CORBA::ULong push_count (void);

  /// Run a single iteration of the test
  void run (void);

private:
  /// The supplier we are turning into an active object
  EC_Counting_Supplier *supplier_;

  /// The event we push through the supplier
  RtecEventComm::EventSet event_;

  /// Set to 1 when the test must stop
  int stop_flag_;

  /// Count the number of push() calls
  CORBA::ULong push_count_;

  TAO_SYNCH_MUTEX lock_;
};

#endif /* ECT_SUPPLIER_H */
