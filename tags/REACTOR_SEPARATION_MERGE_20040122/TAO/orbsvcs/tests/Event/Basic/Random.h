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

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class RND_Driver;

class RND_Consumer
  : public POA_RtecEventComm::PushConsumer
  , public PortableServer::RefCountServantBase
{
  // = TITLE
  //   Simple consumer object
  //
  // = DESCRIPTION
  //
public:
  /// Constructor
  RND_Consumer (RND_Driver *driver);

  void push (const RtecEventComm::EventSet &event
             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void connect (RtecEventChannelAdmin::ConsumerAdmin_ptr admin,
                const RtecEventChannelAdmin::ConsumerQOS &qos
                ACE_ENV_ARG_DECL);
  void disconnect (ACE_ENV_SINGLE_ARG_DECL);

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

  void push (const RtecEventComm::EventSet &event
             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

inline
RND_Timer::RND_Timer (RND_Driver *driver)
  :  RND_Consumer (driver)
{
}

// ****************************************************************

class RND_Supplier
  : public POA_RtecEventComm::PushSupplier
  , public PortableServer::RefCountServantBase
  , public ACE_Task_Base
{
  // = TITLE
  //   Simple supplier object
  //
  // = DESCRIPTION
  //
public:
  /// Constructor
  RND_Supplier (int verbose);

  void connect (RtecEventChannelAdmin::SupplierAdmin_ptr admin,
                const RtecEventChannelAdmin::SupplierQOS &qos
                ACE_ENV_ARG_DECL);
  void disconnect (ACE_ENV_SINGLE_ARG_DECL);

  /// Push a single event...
  void push_new_event (ACE_ENV_SINGLE_ARG_DECL);
  void push (RtecEventComm::EventSet &event
             ACE_ENV_ARG_DECL);

  virtual void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

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
  int run (int argc, char *argv[]);

  /// The main timer has expired
  void timer (const RtecEventComm::Event &e
              ACE_ENV_ARG_DECL);

  /// One of the consumers has received an event
  void event (const RtecEventComm::Event &e
              ACE_ENV_ARG_DECL);

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

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* EC_RANDOM_H */
