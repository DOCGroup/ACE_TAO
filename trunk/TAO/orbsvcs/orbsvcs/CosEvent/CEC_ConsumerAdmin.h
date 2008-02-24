// -*- C++ -*-
//=============================================================================
/**
 *  @file   CEC_ConsumerAdmin.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_CEC_CONSUMERADMIN_H
#define TAO_CEC_CONSUMERADMIN_H

#include /**/ "ace/pre.h"

#include "orbsvcs/CosEventChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ESF/ESF_Proxy_Admin.h"

#include "orbsvcs/CosEvent/CEC_ProxyPushSupplier.h"
#include "orbsvcs/CosEvent/CEC_ProxyPullSupplier.h"
#include "orbsvcs/CosEvent/event_serv_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_CEC_EventChannel;

/**
 * @class TAO_CEC_ConsumerAdmin
 *
 * @brief ConsumerAdmin
 *
 * Implements the ConsumerAdmin interface, i.e. the factory for
 * ProxyPushSupplier objects.
 * = MEMORY MANAGMENT
 * It does not assume ownership of the TAO_CEC_EventChannel
 * object; but it *does* assume ownership of the
 * TAO_CEC_ProxyPushSupplier_Set object.
 * = LOCKING
 * No provisions for locking, access must be serialized
 * externally.
 * = TODO
 */
class TAO_Event_Serv_Export TAO_CEC_ConsumerAdmin
  : public POA_CosEventChannelAdmin::ConsumerAdmin
{
public:
  /**
   * constructor. If <supplier_set> is nil then it builds one using
   * the @a event_channel argument.
   * In any case it assumes ownership.
   */
  TAO_CEC_ConsumerAdmin (TAO_CEC_EventChannel* event_channel);

  /// destructor...
  virtual ~TAO_CEC_ConsumerAdmin (void);

  /// For each elements call <worker->work()>.
  void for_each (TAO_ESF_Worker<TAO_CEC_ProxyPushSupplier> *worker);
  void for_each (TAO_ESF_Worker<TAO_CEC_ProxyPullSupplier> *worker);

  /// Push the event to all the consumers
  void push (const CORBA::Any &event);

  /// Used to inform the EC that a Supplier has connected or
  /// disconnected from it.
  virtual void connected (TAO_CEC_ProxyPushSupplier*);
  virtual void reconnected (TAO_CEC_ProxyPushSupplier*);
  virtual void disconnected (TAO_CEC_ProxyPushSupplier*);
  virtual void connected (TAO_CEC_ProxyPullSupplier*);
  virtual void reconnected (TAO_CEC_ProxyPullSupplier*);
  virtual void disconnected (TAO_CEC_ProxyPullSupplier*);

  /// The event channel is shutting down, inform all the consumers of
  /// this
  virtual void shutdown (void);

  // = The CosEventChannelAdmin::ConsumerAdmin methods...
  virtual CosEventChannelAdmin::ProxyPushSupplier_ptr
      obtain_push_supplier (void);
  virtual CosEventChannelAdmin::ProxyPullSupplier_ptr
      obtain_pull_supplier (void);

  // = The PortableServer::ServantBase methods
  virtual PortableServer::POA_ptr _default_POA (void);

private:
  /// The Event Channel we belong to
  TAO_CEC_EventChannel *event_channel_;

  /// Store the default POA.
  PortableServer::POA_var default_POA_;

  /// Implement the push side of this class
  TAO_ESF_Proxy_Admin<TAO_CEC_EventChannel,TAO_CEC_ProxyPushSupplier,CosEventChannelAdmin::ProxyPushSupplier> push_admin_;

  /// Implement the pull side of this class
  TAO_ESF_Proxy_Admin<TAO_CEC_EventChannel,TAO_CEC_ProxyPullSupplier,CosEventChannelAdmin::ProxyPullSupplier> pull_admin_;

};

// ****************************************************************

class TAO_CEC_Propagate_Event_Push : public TAO_ESF_Worker<TAO_CEC_ProxyPushSupplier>
{
public:
  TAO_CEC_Propagate_Event_Push (const CORBA::Any& event);

  void work (TAO_CEC_ProxyPushSupplier *supplier);

private:
  /// The event
  CORBA::Any event_;
};

// ****************************************************************

class TAO_CEC_Propagate_Event_Pull : public TAO_ESF_Worker<TAO_CEC_ProxyPullSupplier>
{
public:
  TAO_CEC_Propagate_Event_Pull (const CORBA::Any& event);

  void work (TAO_CEC_ProxyPullSupplier *supplier);

private:
  /// The event
  CORBA::Any event_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_ConsumerAdmin.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CEC_CONSUMERADMIN_H */
