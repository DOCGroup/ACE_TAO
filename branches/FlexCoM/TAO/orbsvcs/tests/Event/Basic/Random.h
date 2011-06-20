/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Random.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef EC_RANDOM_H
#define EC_RANDOM_H

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminS.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class RND_Driver;

/**
 * Simple consumer object
 */
class RND_Consumer
  : public POA_RtecEventComm::PushConsumer
{
public:
  /// Constructor
  RND_Consumer (RND_Driver *driver);

  void push (const RtecEventComm::EventSet &event);
  void disconnect_push_consumer (void);

  void connect (RtecEventChannelAdmin::ConsumerAdmin_ptr admin,
                const RtecEventChannelAdmin::ConsumerQOS &qos);
  void disconnect (void);

protected:
  /// The driver
  RND_Driver *driver_;

  /// The supplier.
  RtecEventChannelAdmin::ProxyPushSupplier_var proxy_;

  /// Synch
  TAO_SYNCH_MUTEX lock_;
};

inline
RND_Consumer::RND_Consumer (RND_Driver *driver)
  :  driver_ (driver)
{
}

// ****************************************************************

class RND_Timer : public RND_Consumer
{
public:
  RND_Timer (RND_Driver *driver);

  void push (const RtecEventComm::EventSet &event);
};

inline
RND_Timer::RND_Timer (RND_Driver *driver)
  :  RND_Consumer (driver)
{
}

// ****************************************************************

/**
 * Simple supplier object
 */
class RND_Supplier
  : public POA_RtecEventComm::PushSupplier
  , public ACE_Task_Base
{
public:
  /// Constructor
  RND_Supplier (int verbose);

  void connect (RtecEventChannelAdmin::SupplierAdmin_ptr admin,
                const RtecEventChannelAdmin::SupplierQOS &qos);
  void disconnect (void);

  /// Push a single event...
  void push_new_event (void);
  void push (RtecEventComm::EventSet &event);

  virtual void disconnect_push_supplier (void);

  /// Active method
  virtual int svc (void);

private:
  /// The supplier.
  RtecEventChannelAdmin::ProxyPushConsumer_var proxy_;

  /// Synch
  TAO_SYNCH_MUTEX lock_;

  /// Be verbose about the progress of the test
  int verbose_;
};

inline
RND_Supplier::RND_Supplier (int verbose)
  :  verbose_ (verbose)
{
}

// ****************************************************************

class RND_Driver
{
public:
  RND_Driver (void);

  /// Run the test
  int run (int argc, ACE_TCHAR *argv[]);

  /// The main timer has expired
  void timer (const RtecEventComm::Event &e);

  /// One of the consumers has received an event
  void event (const RtecEventComm::Event &e);

private:
  RND_Driver (const RND_Driver &);
  RND_Driver& operator= (const RND_Driver &);

private:
  /// The main timer
  RND_Timer timer_;

  /// The supplier
  RND_Supplier supplier_;

  /// Number of suppliers
  int nsuppliers_;

  /// The suppliers
  RND_Supplier **suppliers_;

  /// Number of consumers
  int nconsumers_;

  /// The consumers
  RND_Consumer **consumers_;

  /// Maximum recursion
  int max_recursion_;

  /// Be verbose about the progress of the test
  int verbose_;

  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin_;
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin_;
};

#endif /* EC_RANDOM_H */
