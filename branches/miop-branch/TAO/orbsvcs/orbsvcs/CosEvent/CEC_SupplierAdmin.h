/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   CEC_SupplierAdmin.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/
 */
//=============================================================================


#ifndef TAO_CEC_SUPPLIERADMIN_H
#define TAO_CEC_SUPPLIERADMIN_H
#include "ace/pre.h"

#include "orbsvcs/CosEventChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ESF/ESF_Proxy_Admin.h"
#include "CEC_ProxyPushConsumer.h"
#include "CEC_ProxyPullConsumer.h"
#include "event_export.h"

class TAO_CEC_EventChannel;

/**
 * @class TAO_CEC_SupplierAdmin
 *
 * @brief ProxyPushSupplier
 *
 * Implement the CosEventChannelAdmin::SupplierAdmin interface.
 * This class is an Abstract Factory for the
 * TAO_CEC_ProxyPushConsumer.
 * = MEMORY MANAGMENT
 * It does not assume ownership of the TAO_CEC_EventChannel object
 * = LOCKING
 * @@ TODO
 * No provisions for locking, access must be serialized
 * externally.
 * = TODO
 */
class TAO_Event_Export TAO_CEC_SupplierAdmin : public POA_CosEventChannelAdmin::SupplierAdmin
{
public:
  /// constructor...
  TAO_CEC_SupplierAdmin (TAO_CEC_EventChannel* event_channel);

  /// destructor...
  virtual ~TAO_CEC_SupplierAdmin (void);

  /// For each elements call <worker->work()>.
  void for_each (TAO_ESF_Worker<TAO_CEC_ProxyPushConsumer> *worker
                 TAO_ENV_ARG_DECL);

  /// For each elements call <worker->work()>.
  void for_each (TAO_ESF_Worker<TAO_CEC_ProxyPullConsumer> *worker
                 TAO_ENV_ARG_DECL);

  /// Keep track of connected consumers.
  virtual void connected (TAO_CEC_ProxyPushConsumer*
                          TAO_ENV_ARG_DECL_NOT_USED);
  virtual void reconnected (TAO_CEC_ProxyPushConsumer*
                            TAO_ENV_ARG_DECL_NOT_USED);
  virtual void disconnected (TAO_CEC_ProxyPushConsumer*
                             TAO_ENV_ARG_DECL_NOT_USED);
  virtual void connected (TAO_CEC_ProxyPullConsumer*
                          TAO_ENV_ARG_DECL_NOT_USED);
  virtual void reconnected (TAO_CEC_ProxyPullConsumer*
                            TAO_ENV_ARG_DECL_NOT_USED);
  virtual void disconnected (TAO_CEC_ProxyPullConsumer*
                             TAO_ENV_ARG_DECL_NOT_USED);

  /// The event channel is shutting down, inform all the consumers of
  /// this
  virtual void shutdown (TAO_ENV_SINGLE_ARG_DECL_NOT_USED);

  // = The CosEventChannelAdmin::SupplierAdmin methods...
  virtual CosEventChannelAdmin::ProxyPushConsumer_ptr
      obtain_push_consumer (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
          ACE_THROW_SPEC ((CORBA::SystemException));
  virtual CosEventChannelAdmin::ProxyPullConsumer_ptr
      obtain_pull_consumer (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
          ACE_THROW_SPEC ((CORBA::SystemException));

  // = The PortableServer::ServantBase methods
  virtual PortableServer::POA_ptr _default_POA (TAO_ENV_SINGLE_ARG_DECL);

private:
  /// The Event Channel we belong to
  TAO_CEC_EventChannel *event_channel_;

  /// The push and pull aspects are implemented using these classes
  TAO_ESF_Proxy_Admin<TAO_CEC_EventChannel,TAO_CEC_ProxyPushConsumer,CosEventChannelAdmin::ProxyPushConsumer> push_admin_;
  TAO_ESF_Proxy_Admin<TAO_CEC_EventChannel,TAO_CEC_ProxyPullConsumer,CosEventChannelAdmin::ProxyPullConsumer> pull_admin_;

  /// Store the default POA.
  PortableServer::POA_var default_POA_;
};

#if defined (__ACE_INLINE__)
#include "CEC_SupplierAdmin.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_CEC_SUPPLIERADMIN_H */
