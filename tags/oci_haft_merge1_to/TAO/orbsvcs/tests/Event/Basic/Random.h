/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel tests
//
// = FILENAME
//   Random.h
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

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
  RND_Consumer (RND_Driver *driver);
  // Constructor

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
  RND_Driver *driver_;
  // The driver

  RtecEventChannelAdmin::ProxyPushSupplier_var proxy_;
  // The supplier.

  TAO_SYNCH_MUTEX lock_;
  // Synch
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
  RND_Supplier (int verbose);
  // Constructor

  void connect (RtecEventChannelAdmin::SupplierAdmin_ptr admin,
                const RtecEventChannelAdmin::SupplierQOS &qos
                ACE_ENV_ARG_DECL);
  void disconnect (ACE_ENV_SINGLE_ARG_DECL);

  void push_new_event (ACE_ENV_SINGLE_ARG_DECL);
  void push (RtecEventComm::EventSet &event
             ACE_ENV_ARG_DECL);
  // Push a single event...

  virtual void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual int svc (void);
  // Active method

private:
  RtecEventChannelAdmin::ProxyPushConsumer_var proxy_;
  // The supplier.

  TAO_SYNCH_MUTEX lock_;
  // Synch

  int verbose_;
  // Be verbose about the progress of the test
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

  int run (int argc, char *argv[]);
  // Run the test

  void timer (const RtecEventComm::Event &e
              ACE_ENV_ARG_DECL);
  // The main timer has expired

  void event (const RtecEventComm::Event &e
              ACE_ENV_ARG_DECL);
  // One of the consumers has received an event

private:
  RND_Driver (const RND_Driver &);
  RND_Driver& operator= (const RND_Driver &);

private:
  RND_Timer timer_;
  // The main timer

  RND_Supplier supplier_;
  // The supplier

  int nsuppliers_;
  // Number of suppliers

  RND_Supplier **suppliers_;
  // The suppliers

  int nconsumers_;
  // Number of consumers

  RND_Consumer **consumers_;
  // The consumers

  int max_recursion_;
  // Maximum recursion

  int verbose_;
  // Be verbose about the progress of the test

  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin_;
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* EC_RANDOM_H */
