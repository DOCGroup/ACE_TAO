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
  RND_Consumer (RND_Driver *driver);

  void push (const CORBA::Any &event);
  void disconnect_push_consumer ();

  void connect (CosEventChannelAdmin::ConsumerAdmin_ptr admin);
  void disconnect ();

protected:
  /// The driver
  RND_Driver *driver_;

  /// The supplier.
  CosEventChannelAdmin::ProxyPushSupplier_var proxy_;

  /// Synch
  TAO_SYNCH_MUTEX lock_;
};

inline
RND_Consumer::RND_Consumer (RND_Driver *driver)
  :  driver_ (driver)
{
}

// ****************************************************************

// ****************************************************************

/// Simple supplier object
class RND_Supplier
  : public POA_CosEventComm::PushSupplier
  , public ACE_Task_Base
{
public:
  /// Constructor
  RND_Supplier ();

  void connect (CosEventChannelAdmin::SupplierAdmin_ptr admin);
  void disconnect ();

  /// Push a single event...
  void push_new_event ();
  void push (CORBA::Any &event);

  virtual void disconnect_push_supplier ();

  /// Active method
  virtual int svc ();

private:
  /// The supplier.
  CosEventChannelAdmin::ProxyPushConsumer_var proxy_;

  /// Synch
  TAO_SYNCH_MUTEX lock_;
};

inline
RND_Supplier::RND_Supplier ()
{
}

// ****************************************************************

class RND_Driver
{
public:
  RND_Driver ();

  /// Run the test
  int run (int argc, ACE_TCHAR *argv[]);

  /// The main timer has expired
  void timer (const CORBA::Any &e);

  /// One of the consumers has received an event
  void event (const CORBA::Any &e);

private:
  RND_Driver (const RND_Driver &);
  RND_Driver& operator= (const RND_Driver &);

private:
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

  CosEventChannelAdmin::ConsumerAdmin_var consumer_admin_;
  CosEventChannelAdmin::SupplierAdmin_var supplier_admin_;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* EC_RANDOM_H */
