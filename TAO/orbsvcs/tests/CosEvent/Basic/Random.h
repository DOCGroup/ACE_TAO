/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Random.h
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================

#ifndef EC_RANDOM_H
#define EC_RANDOM_H

#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class RND_Driver;

/// Simple consumer object
class RND_Consumer
  : public POA_CosEventComm::PushConsumer
{
public:
  /// Constructor
  explicit RND_Consumer (RND_Driver* driver);
  RND_Consumer (RND_Consumer const&) = delete;
  RND_Consumer (RND_Consumer&&) = delete;
  RND_Consumer& operator= (RND_Consumer const&) = delete;
  RND_Consumer& operator= (RND_Consumer&&) = delete;

  void push (CORBA::Any const& event) override;
  void disconnect_push_consumer () override;

  void connect (CosEventChannelAdmin::ConsumerAdmin_ptr admin);
  void disconnect ();

protected:
  /// The driver
  /// Non-owning; the driver outlives its consumers.
  RND_Driver* driver_ {};

  /// The supplier.
  CosEventChannelAdmin::ProxyPushSupplier_var proxy_;

  /// Synch
  TAO_SYNCH_MUTEX lock_;
};

inline
RND_Consumer::RND_Consumer (RND_Driver* driver)
  : driver_ (driver)
{
}

/// Simple supplier object
class RND_Supplier
  : public POA_CosEventComm::PushSupplier
  , public ACE_Task_Base
{
public:
  /// Constructor
  RND_Supplier () = default;
  RND_Supplier (RND_Supplier const&) = delete;
  RND_Supplier (RND_Supplier&&) = delete;
  RND_Supplier& operator= (RND_Supplier const&) = delete;
  RND_Supplier& operator= (RND_Supplier&&) = delete;

  void connect (CosEventChannelAdmin::SupplierAdmin_ptr admin);
  void disconnect ();

  /// Push a single event...
  void push_new_event ();
  void push (CORBA::Any& event);

  void disconnect_push_supplier () override;

  /// Active method
  int svc () override;

private:
  /// The supplier.
  CosEventChannelAdmin::ProxyPushConsumer_var proxy_;

  /// Synch
  TAO_SYNCH_MUTEX lock_;
};

// ****************************************************************

class RND_Driver
{
public:
  RND_Driver ();
  RND_Driver (RND_Driver const&) = delete;
  RND_Driver (RND_Driver&&) = delete;
  RND_Driver& operator= (RND_Driver const&) = delete;
  RND_Driver& operator= (RND_Driver&&) = delete;

  /// Run the test
  int run (int argc, ACE_TCHAR *argv[]);

  /// The main timer has expired
  void timer (CORBA::Any const& e);

  /// One of the consumers has received an event
  void event (CORBA::Any const& e);

private:
  /// The supplier
  RND_Supplier supplier_;

  /// Number of suppliers
  int nsuppliers_ {4};

  /// The suppliers
  /// ACE-allocated pointer array; servants are deactivated and released in run().
  RND_Supplier** suppliers_ {};

  /// Number of consumers
  int nconsumers_ {4};

  /// The consumers
  /// ACE-allocated pointer array; servants are deactivated and released in run().
  RND_Consumer** consumers_ {};

  /// Maximum recursion
  int max_recursion_ {1};

  CosEventChannelAdmin::ConsumerAdmin_var consumer_admin_;
  CosEventChannelAdmin::SupplierAdmin_var supplier_admin_;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* EC_RANDOM_H */
