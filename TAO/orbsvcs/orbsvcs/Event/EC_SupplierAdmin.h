/* -*- C++ -*- */
/**
 *  @file   EC_SupplierAdmin.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_SUPPLIERADMIN_H
#define TAO_EC_SUPPLIERADMIN_H
#include "ace/pre.h"

#include "EC_ProxyConsumer.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ESF/ESF_Peer_Admin.h"
#include "event_export.h"

class TAO_EC_Event_Channel_Base;
class TAO_EC_ProxyPushSupplier;

/**
 * @class TAO_EC_SupplierAdmin
 *
 * @brief Implement the RtecEventChannelAdmin::SupplierAdmin interface.
 *
 *
 * <H2>Memory Management</H2>
 * It does not assume ownership of the TAO_EC_Event_Channel_Base object
 */
class TAO_RTEvent_Export TAO_EC_SupplierAdmin
  : public POA_RtecEventChannelAdmin::SupplierAdmin
  , public TAO_ESF_Peer_Admin<TAO_EC_Event_Channel_Base,TAO_EC_ProxyPushConsumer,RtecEventChannelAdmin::ProxyPushConsumer,TAO_EC_ProxyPushSupplier>
{
public:
  /// constructor...
  TAO_EC_SupplierAdmin (TAO_EC_Event_Channel_Base* event_channel);

  /// destructor...
  virtual ~TAO_EC_SupplierAdmin (void);

  // = The RtecEventChannelAdmin::SupplierAdmin methods...
  virtual RtecEventChannelAdmin::ProxyPushConsumer_ptr
      obtain_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
          ACE_THROW_SPEC ((CORBA::SystemException));

  // = The PortableServer::ServantBase methods
  virtual PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL);

private:
  /// Store the default POA.
  PortableServer::POA_var default_POA_;
};

#if defined (__ACE_INLINE__)
#include "EC_SupplierAdmin.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_SUPPLIERADMIN_H */
