/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Cos Event Channel
//
// = FILENAME
//   CEC_ConsumerAdmin
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_CEC_CONSUMERADMIN_H
#define TAO_CEC_CONSUMERADMIN_H

#include "orbsvcs/CosEventChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Event/EC_Proxy_Collection.h"
#include "orbsvcs/Event/EC_Worker.h"

class TAO_CEC_EventChannel;
class TAO_CEC_ProxyPushSupplier;
class TAO_CEC_ProxyPushConsumer;

class TAO_ORBSVCS_Export TAO_CEC_ConsumerAdmin : public POA_CosEventChannelAdmin::ConsumerAdmin
{
  // = TITLE
  //   ConsumerAdmin
  //
  // = DESCRIPTION
  //   Implements the ConsumerAdmin interface, i.e. the factory for
  //   ProxyPushSupplier objects.
  //
  // = MEMORY MANAGMENT
  //   It does not assume ownership of the TAO_CEC_EventChannel
  //   object; but it *does* assume ownership of the
  //   TAO_CEC_ProxyPushSupplier_Set object.
  //
  // = LOCKING
  //   No provisions for locking, access must be serialized
  //   externally.
  //
  // = TODO
  //
public:
  typedef TAO_EC_Proxy_Collection<TAO_CEC_ProxyPushSupplier> Collection;
  TAO_CEC_ConsumerAdmin (TAO_CEC_EventChannel* event_channel);
  // constructor. If <supplier_set> is nil then it builds one using
  // the <event_channel> argument.
  // In any case it assumes ownership.

  virtual ~TAO_CEC_ConsumerAdmin (void);
  // destructor...

  void for_each (TAO_EC_Worker<TAO_CEC_ProxyPushSupplier> *worker,
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

  // = The CosEventChannelAdmin::ConsumerAdmin methods...
  virtual CosEventChannelAdmin::ProxyPushSupplier_ptr
      obtain_push_supplier (CORBA::Environment &)
          ACE_THROW_SPEC ((CORBA::SystemException));
  virtual CosEventChannelAdmin::ProxyPullSupplier_ptr
      obtain_pull_supplier (CORBA::Environment &)
          ACE_THROW_SPEC ((CORBA::SystemException));

  // = The PortableServer::ServantBase methods
  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment& env);

private:
  TAO_CEC_EventChannel *event_channel_;
  // The Event Channel we belong to

  Collection *collection_;
  // The supplier container.

  PortableServer::POA_var default_POA_;
  // Store the default POA.
};

// ****************************************************************

class TAO_CEC_Connect_Consumer : public TAO_EC_Worker<TAO_CEC_ProxyPushSupplier>
{
  // = TITLE
  //   TAO_CEC_Connect_Consumer
  //
  // = DESCRIPTION
  //   Worker class to connect the ProxyPushConsumer objects with all
  //   the ProxyPushSupplier objects in the collection.
  //
public:
  TAO_CEC_Connect_Consumer (TAO_CEC_ProxyPushConsumer *consumer);
  // Constructor

  void work (TAO_CEC_ProxyPushSupplier *supplier,
             CORBA::Environment &ACE_TRY_ENV);

private:
  TAO_CEC_ProxyPushConsumer *consumer_;
};

// ****************************************************************

class TAO_CEC_Reconnect_Consumer : public TAO_EC_Worker<TAO_CEC_ProxyPushSupplier>
{
  // = TITLE
  //   TAO_CEC_Reconnect_Consumer
  //
  // = DESCRIPTION
  //   Worker class to reconnect the ProxyPushConsumer objects with all
  //   the ProxyPushSupplier objects in the collection.
  //
public:
  TAO_CEC_Reconnect_Consumer (TAO_CEC_ProxyPushConsumer *consumer);
  // Constructor

  void work (TAO_CEC_ProxyPushSupplier *supplier,
             CORBA::Environment &ACE_TRY_ENV);

private:
  TAO_CEC_ProxyPushConsumer *consumer_;
};

// ****************************************************************

class TAO_CEC_Disconnect_Consumer : public TAO_EC_Worker<TAO_CEC_ProxyPushSupplier>
{
  // = TITLE
  //   TAO_CEC_Disconnect_Consumer
  //
  // = DESCRIPTION
  //   Worker class to disconnect the ProxyPushConsumer objects with all
  //   the ProxyPushSupplier objects in the collection.
  //
public:
  TAO_CEC_Disconnect_Consumer (TAO_CEC_ProxyPushConsumer *consumer);
  // Constructor

  void work (TAO_CEC_ProxyPushSupplier *supplier,
             CORBA::Environment &ACE_TRY_ENV);

private:
  TAO_CEC_ProxyPushConsumer *consumer_;
};

// ****************************************************************

class TAO_CEC_Shutdown_Supplier : public TAO_EC_Worker<TAO_CEC_ProxyPushSupplier>
{
public:
  TAO_CEC_Shutdown_Supplier (void);

  void work (TAO_CEC_ProxyPushSupplier *supplier,
             CORBA::Environment &ACE_TRY_ENV);
};

#if defined (__ACE_INLINE__)
#include "CEC_ConsumerAdmin.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_CEC_CONSUMERADMIN_H */
