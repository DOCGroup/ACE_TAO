/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   CEC_SupplierAdmin
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//   Based on previous work by Tim Harrison (harrison@cs.wustl.edu)
//   and other members of the DOC group.
//   More details can be found in:
//   http://www.cs.wustl.edu/~schmidt/oopsla.ps.gz
//   http://www.cs.wustl.edu/~schmidt/JSAC-98.ps.gz
//
//
// ============================================================================

#ifndef TAO_CEC_SUPPLIERADMIN_H
#define TAO_CEC_SUPPLIERADMIN_H

#include "orbsvcs/CosEventChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Event/EC_Proxy_Collection.h"
#include "orbsvcs/Event/EC_Worker.h"

class TAO_CEC_EventChannel;
class TAO_CEC_ProxyPushSupplier;
class TAO_CEC_ProxyPushConsumer;

class TAO_ORBSVCS_Export TAO_CEC_SupplierAdmin : public POA_CosEventChannelAdmin::SupplierAdmin
{
  // = TITLE
  //   ProxyPushSupplier
  //
  // = DESCRIPTION
  //   Implement the CosEventChannelAdmin::SupplierAdmin interface.
  //   This class is an Abstract Factory for the
  //   TAO_CEC_ProxyPushConsumer.
  //
  // = MEMORY MANAGMENT
  //   It does not assume ownership of the TAO_CEC_EventChannel object
  //
  // = LOCKING
  //   @@ TODO
  //   No provisions for locking, access must be serialized
  //   externally.
  //
  // = TODO
  //
public:
  TAO_CEC_SupplierAdmin (TAO_CEC_EventChannel* event_channel);
  // constructor...

  virtual ~TAO_CEC_SupplierAdmin (void);
  // destructor...

  void for_each (TAO_EC_Worker<TAO_CEC_ProxyPushConsumer> *worker,
                 CORBA::Environment &ACE_TRY_ENV);
  // For each elements call <worker->work()>.

  virtual void connected (TAO_CEC_ProxyPushConsumer*,
                          CORBA::Environment&);
  virtual void reconnected (TAO_CEC_ProxyPushConsumer*,
                            CORBA::Environment&);
  virtual void disconnected (TAO_CEC_ProxyPushConsumer*,
                             CORBA::Environment&);
  // Used to inform the EC that a Consumer has connected or
  // disconnected from it.

  virtual void connected (TAO_CEC_ProxyPushSupplier*,
                          CORBA::Environment&);
  virtual void reconnected (TAO_CEC_ProxyPushSupplier*,
                            CORBA::Environment&);
  virtual void disconnected (TAO_CEC_ProxyPushSupplier*,
                             CORBA::Environment&);
  // Used to inform the EC that a Supplier has connected or
  // disconnected from it.

  virtual void shutdown (CORBA::Environment&);
  // The event channel is shutting down, inform all the consumers of
  // this

  // = The CosEventChannelAdmin::SupplierAdmin methods...
  virtual CosEventChannelAdmin::ProxyPushConsumer_ptr
      obtain_push_consumer (CORBA::Environment &)
          ACE_THROW_SPEC ((CORBA::SystemException));
  virtual CosEventChannelAdmin::ProxyPullConsumer_ptr
      obtain_pull_consumer (CORBA::Environment &)
          ACE_THROW_SPEC ((CORBA::SystemException));

  // = The PortableServer::ServantBase methods
  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment& env);

private:
  TAO_CEC_EventChannel *event_channel_;
  // The Event Channel we belong to

  typedef TAO_EC_Proxy_Collection<TAO_CEC_ProxyPushConsumer> Collection;

  Collection *collection_;
  // The consumer container

  PortableServer::POA_var default_POA_;
  // Store the default POA.
};

// ****************************************************************

class TAO_CEC_Connect_Supplier : public TAO_EC_Worker<TAO_CEC_ProxyPushConsumer>
{
  // = TITLE
  //   TAO_CEC_Connect_Supplier
  //
  // = DESCRIPTION
  //   Worker class to connect the ProxyPushConsumer objects with all
  //   the ProxyPushConsumer objects in the collection.
  //
public:
  TAO_CEC_Connect_Supplier (TAO_CEC_ProxyPushSupplier *supplier);
  // Constructor

  void work (TAO_CEC_ProxyPushConsumer *consumer,
             CORBA::Environment &ACE_TRY_ENV);

private:
  TAO_CEC_ProxyPushSupplier *supplier_;
};

// ****************************************************************

class TAO_CEC_Reconnect_Supplier : public TAO_EC_Worker<TAO_CEC_ProxyPushConsumer>
{
  // = TITLE
  //   TAO_CEC_Reconnect_Supplier
  //
  // = DESCRIPTION
  //   Worker class to reconnect the ProxyPushConsumer objects with all
  //   the ProxyPushConsumer objects in the collection.
  //
public:
  TAO_CEC_Reconnect_Supplier (TAO_CEC_ProxyPushSupplier *supplier);
  // Constructor

  void work (TAO_CEC_ProxyPushConsumer *consumer,
             CORBA::Environment &ACE_TRY_ENV);

private:
  TAO_CEC_ProxyPushSupplier *supplier_;
};

// ****************************************************************

class TAO_CEC_Disconnect_Supplier : public TAO_EC_Worker<TAO_CEC_ProxyPushConsumer>
{
  // = TITLE
  //   TAO_CEC_Disconnect_Supplier
  //
  // = DESCRIPTION
  //   Worker class to disconnect the ProxyPushConsumer objects with all
  //   the ProxyPushConsumer objects in the collection.
  //
public:
  TAO_CEC_Disconnect_Supplier (TAO_CEC_ProxyPushSupplier *supplier);
  // Constructor

  void work (TAO_CEC_ProxyPushConsumer *consumer,
             CORBA::Environment &ACE_TRY_ENV);

private:
  TAO_CEC_ProxyPushSupplier *supplier_;
};

// ****************************************************************

class TAO_CEC_Shutdown_Consumer : public TAO_EC_Worker<TAO_CEC_ProxyPushConsumer>
{
public:
  TAO_CEC_Shutdown_Consumer (void);

  void work (TAO_CEC_ProxyPushConsumer *consumer,
             CORBA::Environment &ACE_TRY_ENV);
};

#if defined (__ACE_INLINE__)
#include "CEC_SupplierAdmin.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_CEC_SUPPLIERADMIN_H */
