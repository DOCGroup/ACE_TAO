/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_ConsumerAdmin
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
// ============================================================================

#ifndef TAO_EC_CONSUMERADMIN_H
#include "ace/pre.h"
#define TAO_EC_CONSUMERADMIN_H

#include "orbsvcs/RtecEventChannelAdminS.h"
#include "event_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "EC_Proxy_Collection.h"
#include "EC_Worker.h"

class TAO_EC_Event_Channel;
class TAO_EC_ProxyPushSupplier;
class TAO_EC_ProxyPushConsumer;

class TAO_RTEvent_Export TAO_EC_ConsumerAdmin : public POA_RtecEventChannelAdmin::ConsumerAdmin
{
  // = TITLE
  //   ConsumerAdmin
  //
  // = DESCRIPTION
  //   Implements the ConsumerAdmin interface, i.e. the factory for
  //   ProxyPushSupplier objects.
  //
  // = MEMORY MANAGMENT
  //   It does not assume ownership of the TAO_EC_Event_Channel
  //   object; but it *does* assume ownership of the
  //   TAO_EC_ProxyPushSupplier_Set object.
  //
  // = LOCKING
  //   No provisions for locking, access must be serialized
  //   externally.
  //
  // = TODO
  //
public:
  TAO_EC_ConsumerAdmin (TAO_EC_Event_Channel* event_channel);
  // constructor. If <supplier_set> is nil then it builds one using
  // the <event_channel> argument.
  // In any case it assumes ownership.

  virtual ~TAO_EC_ConsumerAdmin (void);
  // destructor...

  void for_each (TAO_EC_Worker<TAO_EC_ProxyPushSupplier> *worker,
                 CORBA::Environment &ACE_TRY_ENV);
  // For each elements call <worker->work()>.

  virtual void connected (TAO_EC_ProxyPushConsumer*,
                          CORBA::Environment&);
  virtual void reconnected (TAO_EC_ProxyPushConsumer*,
                            CORBA::Environment&);
  virtual void disconnected (TAO_EC_ProxyPushConsumer*,
                             CORBA::Environment&);
  // Used to inform the EC that a Consumer has connected or
  // disconnected from it.

  virtual void connected (TAO_EC_ProxyPushSupplier*,
                          CORBA::Environment&);
  virtual void reconnected (TAO_EC_ProxyPushSupplier*,
                            CORBA::Environment&);
  virtual void disconnected (TAO_EC_ProxyPushSupplier*,
                             CORBA::Environment&);
  // Used to inform the EC that a Supplier has connected or
  // disconnected from it.

  virtual void shutdown (CORBA::Environment&);
  // The event channel is shutting down, inform all the consumers of
  // this

  // = The RtecEventChannelAdmin::ConsumerAdmin methods...
  virtual RtecEventChannelAdmin::ProxyPushSupplier_ptr
      obtain_push_supplier (CORBA::Environment &)
          ACE_THROW_SPEC ((CORBA::SystemException));

  // = The PortableServer::ServantBase methods
  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment& env);

private:
  TAO_EC_Event_Channel *event_channel_;
  // The Event Channel we belong to

  typedef TAO_EC_Proxy_Collection<TAO_EC_ProxyPushSupplier> Collection;

  Collection *collection_;
  // The supplier container.

  PortableServer::POA_var default_POA_;
  // Store the default POA.
};

// ****************************************************************

class TAO_EC_Connect_Consumer : public TAO_EC_Worker<TAO_EC_ProxyPushSupplier>
{
  // = TITLE
  //   TAO_EC_Connect_Consumer
  //
  // = DESCRIPTION
  //   Worker class to connect the ProxyPushConsumer objects with all
  //   the ProxyPushSupplier objects in the collection.
  //
public:
  TAO_EC_Connect_Consumer (TAO_EC_ProxyPushConsumer *consumer);
  // Constructor

  void work (TAO_EC_ProxyPushSupplier *supplier,
             CORBA::Environment &ACE_TRY_ENV);

private:
  TAO_EC_ProxyPushConsumer *consumer_;
};

// ****************************************************************

class TAO_EC_Reconnect_Consumer : public TAO_EC_Worker<TAO_EC_ProxyPushSupplier>
{
  // = TITLE
  //   TAO_EC_Reconnect_Consumer
  //
  // = DESCRIPTION
  //   Worker class to reconnect the ProxyPushConsumer objects with all
  //   the ProxyPushSupplier objects in the collection.
  //
public:
  TAO_EC_Reconnect_Consumer (TAO_EC_ProxyPushConsumer *consumer);
  // Constructor

  void work (TAO_EC_ProxyPushSupplier *supplier,
             CORBA::Environment &ACE_TRY_ENV);

private:
  TAO_EC_ProxyPushConsumer *consumer_;
};

// ****************************************************************

class TAO_EC_Disconnect_Consumer : public TAO_EC_Worker<TAO_EC_ProxyPushSupplier>
{
  // = TITLE
  //   TAO_EC_Disconnect_Consumer
  //
  // = DESCRIPTION
  //   Worker class to disconnect the ProxyPushConsumer objects with all
  //   the ProxyPushSupplier objects in the collection.
  //
public:
  TAO_EC_Disconnect_Consumer (TAO_EC_ProxyPushConsumer *consumer);
  // Constructor

  void work (TAO_EC_ProxyPushSupplier *supplier,
             CORBA::Environment &ACE_TRY_ENV);

private:
  TAO_EC_ProxyPushConsumer *consumer_;
};

// ****************************************************************

class TAO_EC_Shutdown_Supplier : public TAO_EC_Worker<TAO_EC_ProxyPushSupplier>
{
public:
  TAO_EC_Shutdown_Supplier (void);

  void work (TAO_EC_ProxyPushSupplier *supplier,
             CORBA::Environment &ACE_TRY_ENV);
};

#if defined (__ACE_INLINE__)
#include "EC_ConsumerAdmin.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_CONSUMERADMIN_H */
